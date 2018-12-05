
//谢国经
#include "config.hpp"

	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
	
//当前任务等待事件
bool  OSCEvent :: EventPend(OSCTask*TaskP,uint32 Dly,uint32 TaskInfo){
	if(PrioIndex > TaskP -> TaskPrio){
		PrioIndex = TaskP -> TaskPrio;
	}
	ArryEvent[TaskP -> TaskPrio].ListIn(TaskP);
	TaskP -> EventList = ArryEvent;//记录挂起任务的队列
	if(Dly > 0){
		OSCTimer::TimeWait(TaskP,Dly);//时间队列等待
	}
	TaskP -> TaskInfo |= TaskInfo;
	OSCPrioMng::Leave(TaskP);// 就绪队列取出任务
	return TRUE ;
}
// 当前任务获得事件
OSCTask*  OSCEvent :: EventResume(uint32 TaskInfo){
	OSCTask *TaskPtr =NULL ;
	while(PrioIndex < DF_MINPRIO){
		if(!ArryEvent[PrioIndex].IsEmpty()){//等待队列不是空
			OSCTask::TaskLock();
			TaskPtr = ArryEvent[PrioIndex].GetFirst();
			if(NULL != TaskPtr){
				if(ArryEvent[PrioIndex].ListOut(TaskPtr)){
					if( TaskPtr -> TaskStat & Task_Stat_Time){
						OSCTimer::TimeLeave(TaskPtr);
					}
					TaskPtr -> TaskInfo |= TaskInfo;
					TaskPtr -> EventList = NULL;//记录挂起任务的队列
					OSCPrioMng::Wait(TaskPtr);
				}
			}
			OSCTask::TaskUnlock();
			return TaskPtr;
		}
		PrioIndex ++;
	}
	return  NULL;
}

///
bool  OSCEvent :: EventListOut(OSCTask *TaskP){
	return  ArryEvent[TaskP -> TaskPrio].ListOut(TaskP);//等待队列出列
}
		