//谢国经
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
//申请信号量
OSCMutex  *OSCMutex::MutexID = NULL;

bool  OSCMutex::MutexGetSwap(uint32 Dly){
	bool		TF = FALSE;//表示申请到信号量
	OSCTask *CurPtr = OSTaskCurPtr ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//没有可用资源,任务挂起
		OSCTask::TaskUnlock();
		if(NULL != Thread){//有线程占用了资源
		//占用资源的线程优先级是否比等待任务的优先级高
			if(Thread -> TaskPrio > CurPtr -> TaskPrio){
			//调高正在占用资源的任务的有限性
				Thread -> ChangePrio(CurPtr -> TaskPrio);
			}
		}
		EventPend(CurPtr,Dly,Task_Info_PendMutex);
		OSCTask::Schedul();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//超时返回
			EventListOut(CurPtr);
			//这里不能清楚超时信息，因为如果是超时返回，
			//在归还信号计数器的时候就不需要唤醒一个任务
			//CurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//清除超时标识
			CurPtr -> TaskInfo &=~ Task_Info_PendMutex;//清除等待信号量标识
			return  FALSE ;//获得信号量失败
		}else{//(CurPtr -> TaskInfo & Task_Info_MutexGet)
			CurPtr -> TaskInfo &=~ Task_Info_MutexGet;
			TF = TRUE;
		}
	}
	Thread = CurPtr;
	ThreadPrio = CurPtr -> TaskPrio;
	if(TF){
		CurPtr -> TaskInfo &= ~Task_Info_PendMutex ;//清除等待信号量标识
	}else{
		OSCTask::TaskUnlock();
	}
	return TRUE;
}

//信号量申请
bool  OSCMutex::MutexGetMedi(){
	OSCTask*CurPtr = OSTaskCurPtr;
	bool  TF = TRUE ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//没有可用资源,任务挂起
		TF = FALSE ;
	}
	if(TF){
		Thread = CurPtr;
		ThreadPrio = CurPtr -> TaskPrio;
	}
	OSCTask::TaskUnlock();
	return TF;
}	
//信号量发送
bool  OSCMutex::MutexPutMedi(){
	if(NULL != Thread){
	//在把开关量返还给系统之前把自己的信号量变回原来
		Thread -> ChangePrio(ThreadPrio);
		Thread = NULL;
	}
	OSCTask::TaskLock();
	Cnt ++;
	if(Cnt <= 0){//有任务在等待，恢复一个任务
		OSCTask::TaskUnlock();
		if(OSTaskCurPtr -> TaskInfo & Task_Info_TimeOut){
			OSTaskCurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//清除超时标识
			return  FALSE;//没有任务被唤醒返回FALSE
		}
		OSCTask *TaskP = EventResume(Task_Info_MutexGet);//等待队列恢复一个任务
		if(NULL != TaskP){
			TaskP -> TaskInfo &= ~Task_Info_PendMutex ;//清除等待标识
			return TRUE;//有任务被唤醒则返回TRUE
		}
		return  FALSE;
	}
	OSCTask::TaskUnlock();
	return FALSE;		
}	
//
bool  OSCMutex::MutexPutSwap(){//恢复一个任务
	if(MutexPutMedi()){//有任务被唤醒就进行调度
		OSCTask::Schedul();
		return TRUE;//告诉上层有任务被唤醒
	}
	return FALSE;	
}	

//END	
	
				
		
