//谢国经
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
OSCSem *OSCSem::SemID = NULL;
//申请信号量
bool  OSCSem::SemGetSwap(uint32 Dly){
	bool		TF = FALSE;//
	OSCTask *CurPtr = OSTaskCurPtr ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//没有可用资源,任务挂起
		OSCTask::TaskUnlock();
		EventPend(CurPtr,Dly,Task_Info_PendMutex);
		OSCTask::Schedul();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//超时返回
			EventListOut(CurPtr);
			CurPtr -> TaskInfo &=~ Task_Info_PendSem;//清除等待信号量标识
			return  FALSE ;//获得信号量失败
		}else{//  if(CurPtr -> TaskInfo & Task_Info_SemGet)
			CurPtr -> TaskInfo &=~ Task_Info_SemGet;
			TF = TRUE;
		}
	}
	if(TF){
		CurPtr -> TaskInfo &= ~Task_Info_PendSem ;//清除等待信号量标识
	}else{
		OSCTask::TaskUnlock();
	}
	return TRUE;
}


//信号量申请
bool  OSCSem::SemGetMedi(){
	bool  TF = TRUE ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//没有可用资源,任务挂起
		TF = FALSE ;
	}
	OSCTask::TaskUnlock();
	return TF;
}	
//信号量发送
bool  OSCSem::SemPutMedi(){
	OSCTask *CurPtr = OSTaskCurPtr;
	OSCTask::TaskLock();
	Cnt ++;
	if(Cnt <= 0){//有任务在等待，恢复一个任务
		OSCTask::TaskUnlock();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//如果任务是超时返回，则
			CurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//清除超时标识
			return FALSE;
		}
		OSCTask *TaskP = EventResume(Task_Info_SemGet);//等待队列恢复一个任务
		if(NULL != TaskP){
			TaskP -> TaskInfo &= ~Task_Info_PendSem ;//清除等待标识
			return TRUE;
		}
		return FALSE;
	}
	OSCTask::TaskUnlock();
	return FALSE ;		
}	
//
bool  OSCSem::SemPutSwap(){//恢复一个任务
	if(SemPutMedi()){
		OSCTask::Schedul();
		return TRUE;	
	}
	return FALSE;
}	
//END	
	
