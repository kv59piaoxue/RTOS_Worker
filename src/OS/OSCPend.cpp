//谢国经

#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
//就绪队列入列
bool  OSCPendRdy::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Rdy){//在就绪队列中
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_Rdy ;//保存任务的状态	
	
	++ Count;//任务数加一
	if(Count == 1){//任务数为空	
		PendPtr = TaskPtr ;//挂起任务
		TaskPtr -> RdyNextPtr = TaskPtr ;//指向自己
		TaskPtr -> RdyPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> RdyNextPtr = PendPtr ;
		TaskPtr -> RdyPrevPtr = PendPtr -> RdyPrevPtr ;
		PendPtr -> RdyPrevPtr -> RdyNextPtr = TaskPtr ;
		PendPtr -> RdyPrevPtr = TaskPtr ;
	}
	
	OSCTask::TaskUnlock();
	return TRUE;
}
//就绪队列出列
bool  OSCPendRdy::ListOut(OSCTask*TaskPtr){
	bool TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Rdy){//任务在就绪队列中
		TaskPtr -> TaskStat  &=  ~Task_Stat_Rdy ;
		Count --;
		if(0 == Count){//一个任务
			PendPtr = NULL ;
		}else{//两个以上的任务
			if(PendPtr == TaskPtr){//要取的任务是第一个任务
				PendPtr = TaskPtr -> RdyNextPtr ;
			}
			TaskPtr -> RdyNextPtr -> RdyPrevPtr = TaskPtr -> RdyPrevPtr ;
			TaskPtr -> RdyPrevPtr -> RdyNextPtr = TaskPtr -> RdyNextPtr ;
		}
		
		TaskPtr -> RdyNextPtr = NULL;
		TaskPtr -> RdyPrevPtr = NULL;
		TF = TRUE ;
	}
	OSCTask::TaskUnlock();
	return TF;	
}
//时间队列入列
bool  OSCPendTime::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Time){//在时间队列中
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_Time ;//保存任务的状态
	Count ++ ;//任务数加一
	if(1 == Count){//任务数为空
		PendPtr = TaskPtr ;//挂起任务
		TaskPtr -> TimeNextPtr = TaskPtr ;//指向自己
		TaskPtr -> TimePrevPtr = TaskPtr;//
	}else{
		TaskPtr -> TimeNextPtr = PendPtr ;
		TaskPtr -> TimePrevPtr = PendPtr -> TimePrevPtr ;
		PendPtr -> TimePrevPtr -> TimeNextPtr = TaskPtr ;
		PendPtr -> TimePrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//时间队列出列
bool  OSCPendTime::ListOut(OSCTask*TaskPtr){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Time){
	TaskPtr -> TaskStat  &=  ~Task_Stat_Time ;
	Count --;
	if(0 == Count){//一个任务
		PendPtr = NULL ;
	}else{//两个以上的任务
		if(PendPtr == TaskPtr){//要取的任务是第一个任务
			PendPtr = TaskPtr -> TimeNextPtr ;
		}
		TaskPtr -> TimeNextPtr -> TimePrevPtr = TaskPtr -> TimePrevPtr ;
		TaskPtr -> TimePrevPtr -> TimeNextPtr = TaskPtr -> TimeNextPtr ;
	}
	TaskPtr -> TimeNextPtr = NULL;
	TaskPtr -> TimePrevPtr = NULL;
	TF =  TRUE ;
	}
	OSCTask::TaskUnlock();
	return TF;
}
//事件队列入列
bool  OSCPendEvent::ListIn(OSCTask*TaskPtr){
	 OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Event){
		OSCTask::TaskUnlock();
	 	return FALSE;
	 }
	TaskPtr -> TaskStat |= Task_Stat_Event ;//保存任务的状态
	Count ++ ;//任务数加一
	if(1 == Count){//任务数为空
		PendPtr = TaskPtr ;//挂起任务
		TaskPtr -> EventNextPtr = TaskPtr ;//指向自己
		TaskPtr -> EventPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> EventNextPtr = PendPtr ;
		TaskPtr -> EventPrevPtr = PendPtr -> EventPrevPtr ;
		PendPtr -> EventPrevPtr -> EventNextPtr = TaskPtr ;
		PendPtr -> EventPrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//事件队列出列
bool  OSCPendEvent::ListOut(OSCTask*TaskPtr){
	bool TF =  FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Event){
	TaskPtr -> TaskStat  &=  ~Task_Stat_Event ;
	Count --;
	if(0 == Count){//一个任务
		PendPtr = NULL ;
	}else{//两个以上的任务
		if(PendPtr == TaskPtr){//要取的任务是第一个任务
			PendPtr = TaskPtr -> EventNextPtr ;
		}
		TaskPtr -> EventNextPtr -> EventPrevPtr = TaskPtr -> EventPrevPtr ;
		TaskPtr -> EventPrevPtr -> EventNextPtr = TaskPtr -> EventNextPtr ;
	}
	
	TaskPtr -> EventNextPtr = NULL;
	TaskPtr -> EventPrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;
}


///PendPd
bool  OSCPendPd::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat &Task_Stat_Pd){
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_Pd ;//保存任务的状态
	Count ++ ;//任务数加一
	if(1 == Count){//任务数为空
		PendPtr = TaskPtr ;//挂起任务
		TaskPtr -> PdNextPtr = TaskPtr ;//指向自己
		TaskPtr -> PdPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> PdNextPtr = PendPtr ;
		TaskPtr -> PdPrevPtr = PendPtr -> PdPrevPtr ;
		PendPtr -> PdPrevPtr -> PdNextPtr = TaskPtr ;
		PendPtr -> PdPrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//任务队列出列
bool  OSCPendPd::ListOut(OSCTask*TaskPtr){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Pd){
	TaskPtr -> TaskStat  &=  ~Task_Stat_Pd ;
	Count --;
	if(0 == Count){//一个任务
		PendPtr = NULL ;
	}else{//两个以上的任务
		if(PendPtr == TaskPtr){//要取的任务是第一个任务
			PendPtr = TaskPtr -> PdNextPtr ;
		}
		TaskPtr -> PdNextPtr -> PdPrevPtr = TaskPtr -> PdPrevPtr ;
		TaskPtr -> PdPrevPtr -> PdNextPtr = TaskPtr -> PdNextPtr ;
	}
	TaskPtr -> PdNextPtr = NULL;
	TaskPtr -> PdPrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;
}

//
bool  OSCPendThreadTick::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat &Task_Stat_ThreadTick){
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_ThreadTick ;//保存任务的状态
	Count ++ ;//任务数加一
	if(1 == Count){//任务数为空
		PendPtr = TaskPtr ;//挂起任务
		TaskPtr -> ThreadTickNextPtr = TaskPtr ;//指向自己
		TaskPtr -> ThreadTickPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> ThreadTickNextPtr = PendPtr ;
		TaskPtr -> ThreadTickPrevPtr = PendPtr -> ThreadTickPrevPtr ;
		PendPtr -> ThreadTickPrevPtr -> ThreadTickNextPtr = TaskPtr ;
		PendPtr -> ThreadTickPrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//任务队列出列
bool  OSCPendThreadTick::ListOut(OSCTask*TaskPtr){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_ThreadTick){
	TaskPtr -> TaskStat  &=  ~Task_Stat_ThreadTick ;
	Count --;
	if(0 == Count){//一个任务
		PendPtr = NULL ;
	}else{//两个以上的任务
		if(PendPtr == TaskPtr){//要取的任务是第一个任务
			PendPtr = TaskPtr -> ThreadTickNextPtr ;
		}
		TaskPtr -> ThreadTickNextPtr -> ThreadTickPrevPtr = TaskPtr -> ThreadTickPrevPtr ;
		TaskPtr -> ThreadTickPrevPtr -> ThreadTickNextPtr = TaskPtr -> ThreadTickNextPtr ;
	}
	TaskPtr -> ThreadTickNextPtr = NULL;
	TaskPtr -> ThreadTickPrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;
}
//END