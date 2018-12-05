//谢国经
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
uint32   OSCPrioMng::HighPrio = 0;
OSCPrio   OSCPrioMng::PrioArry[DF_MINPRIO+1] ; 

//优先级管理器构造函数
void   OSCPrioMng::Init(){
//初始化全局变量
	uint32  RunTime = 20;
	uint32 WaitTime = 60;
	for(int32 i = 0; i <= DF_MINPRIO; i++){
	//优先级每增大一个，等待时间和运行时间都翻一翻
		OSCPrio  TempPrio(RunTime*(2^i),WaitTime*(2^i));
		PrioArry[i] = TempPrio;
	}
	HighPrio = 0;//从最高优先级开始查找
}
//设置最大运行时间，返回旧值
uint32  OSCPrioMng::SetRunTime(uint32 Prio,uint32 RunTime){
//改变一种优先级的的最大运行时间
	OSCTask::TaskLock();
	uint32 Temp = PrioArry[Prio].MaxRunTime ;
	PrioArry[Prio].MaxRunTime = RunTime ;
	OSCTask::TaskUnlock();
	return Temp;
}
//设置最大等待时间，返回旧值
uint32  OSCPrioMng::SetWaitTime(uint32 Prio,uint32 WaitTime){
//改变一种优先级的最大等待时间
	OSCTask::TaskLock();
	uint32 Temp = PrioArry[Prio].MaxWaitTime ;
	PrioArry[Prio].MaxWaitTime = WaitTime ;
	OSCTask::TaskUnlock();
	return Temp;
}
//查找一个最高优先级的任务
bool  OSCPrioMng::Find(){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(OSCTask::LockCount > 1){
		OSTaskNextPtr = OSTaskCurPtr;
		OSCTask::TaskUnlockMedi();
		return  FALSE;
	}else if(OSCGuardThread::ThreadID -> IsHasMsg){//任务调度器没有被锁、系统守护线程有消息
		OSTaskNextPtr  = OSCGuardThread::ThreadID;//切换到系统守护线程，处理消息
		OSCTask::TaskUnlockMedi();
		return TRUE;
	}
	while(HighPrio <  DF_MINPRIO ){
		if(!PrioArry[HighPrio].PendRdy.IsEmpty()){//不为空
			OSTaskNextPtr = PrioArry[HighPrio].PendRdy.GetFirst();//返回第一个
			TF = TRUE;
			break ;
		}
		HighPrio ++ ;
	}
	if( FALSE == TF){//如果没找到除最低优先级任务外的其他任务
		OSCTimer::RdyResume();//尝试恢复就绪挂起的任务，再次查找
		while(HighPrio <=  DF_MINPRIO ){//如果还没有就绪的任务则会查找到系统空闲任务
			if(!PrioArry[HighPrio].PendRdy.IsEmpty()){//不为空
				OSTaskNextPtr = PrioArry[HighPrio].PendRdy.GetFirst();//返回第一个
				TF = TRUE;
				break ;
			}
			HighPrio ++ ;
		}	
	}
	OSCTask::TaskUnlockMedi();
	return TF ;
}
//就绪队列中取走指定任务
bool	 OSCPrioMng::Leave(OSCTask *TaskPtr){
	return  PrioArry[TaskPtr -> TaskPrio].PendRdy.ListOut(TaskPtr);
}
//任务放到就绪队列中等待调度
bool  OSCPrioMng::Wait(OSCTask *TaskPtr){
	if(TaskPtr -> TaskPrio < HighPrio )
		HighPrio = TaskPtr -> TaskPrio ;
	return   PrioArry[TaskPtr -> TaskPrio].PendRdy.ListIn(TaskPtr);//	 
}	
	
//END	
	
	
	
	
	
	