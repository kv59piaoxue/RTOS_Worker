//
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
OSCPendTime       OSCTimer::PendTime ;
OSCPendTime	OSCTimer::PendTRdy ;
OSCPendThreadTick	 OSCTimer::PendThreadTick;
uint32     OSCTimer::CurTick  =  0;

//
void  OSCTimer::Init(){
  	OSCPendTime   	Temp;
  	OSCPendThreadTick   InitThreadTick;
  	PendThreadTick = InitThreadTick;
  	PendTime = Temp;
  	PendTRdy = Temp;
 	CurTick = 0;
 }
 
 //恢复在时间队列中等待的就绪任务
  bool  OSCTimer::RdyResume(){
  	OSCTask   *TaskP;
  	while(!PendTRdy.IsEmpty()){
  		TaskP = PendTRdy.GetFirst();
  		if(PendTRdy.ListOut(TaskP)){
  			//TaskP -> RemainTick = 0;//时间片
  			OSCPrioMng::Wait(TaskP);
  		}
  	}
  	return TRUE;
 } //end function
 
bool		OSCTimer::RdyLeave(OSCTask *TaskPtr){
	return  PendTRdy.ListOut(TaskPtr);	
}

bool  	OSCTimer::RdyWait(OSCTask *TaskPtr){
		if(PendTRdy.ListIn(TaskPtr)){//加入等待队列		
			TaskPtr -> TaskDly =  OSCPrioMng::GetWaitTime(TaskPtr -> TaskPrio);//设置最大等待时间
			return TRUE;
		}
		return FALSE;
}
 //离开时间队列
 bool  OSCTimer::TimeLeave(OSCTask*TaskPtr){
 	if(PendTime.ListOut(TaskPtr)){
 		TaskPtr -> TaskInfo &= ~Task_Info_TimeOut ;//主动离开，不是超时
 		return   TRUE;
 	}
 	return FALSE;
 }
 
 
 //进入时间队列等待
  bool  OSCTimer::TimeWait(OSCTask*TaskPtr,uint32  Dly){
 	 if(PendTime.ListIn(TaskPtr)){
 	 	 TaskPtr -> TaskDly = Dly ;
 	 	 return TRUE;
 	 }
 	 return FALSE;
 }
 //时间服务函数，中断函数调用
 void  OSCTimer::OnTime(){//因为这个函数只能由系统优先级最高的时间服务线程调用，
 		if(!PendTime.IsEmpty()){//挂起任务时间服务
  		 	uint32  TempCount = PendTime.GetCount();//返回链表中任务的个数
  			OSCTask  *TempTask1,*TempTask2 = PendTime.GetFirst();//返回第一个任务
  			while(TempCount -- ){
  				if( 0 == TempTask2 -> TaskDly --){//任务延时到达
 					TempTask1 = TempTask2 -> TimeNextPtr ;//保存下一个任务的地址
 					PendTime.ListOut(TempTask2);//
 					TempTask2 -> TaskInfo  |= Task_Info_TimeOut ;//设置任务的返回状态为超时
 					OSCPrioMng::Wait(TempTask2);//就绪队列中等待
 					TempTask2 = TempTask1 ;//
 				}else{
 					TempTask2 = TempTask2 -> TimeNextPtr ;
 				}//end if
 		 	}//end while
 		}//end if	
 		//就绪任务最大等大时间到
 		 if(!PendTRdy.IsEmpty()){
  		 	uint32  TempCount = PendTRdy.GetCount();//返回链表中任务的个数
  			OSCTask  *TempTask1,*TempTask2 = PendTRdy.GetFirst();//返回第一个任务
  			while(TempCount -- ){
  				if( 0 == TempTask2 -> TaskDly -- ){//任务延时到达
 					TempTask1 = TempTask2 -> TimeNextPtr ;//保存下一个任务的地址
 					PendTRdy.ListOut(TempTask2);//
 					OSCPrioMng::Wait(TempTask2);//就绪队列中等待
 					TempTask2 = TempTask1 ;//
 				}else{
 					TempTask2 = TempTask2 -> TimeNextPtr ;
 				}//end if
 		 	}//end while
 		}//end if	
 }
 
 
 bool	OSCTimer::OnTimeInt(){
 //定时器中断函数调用，用于时间服务，将定时器中断的消息发送给
 //时间服务线程，唤醒时间服务线程进行时间服务，
 //这样可以缩短系统中断响应的时间
 		OSCTask *CurPtr = NULL;	
 		OSCInt::IntLock();
 		if(CurTick){//害怕0--变成很大一个数哦
 		    	CurTick -- ;
 			if(!CurTick){//当前任务最大运行时间到
 				CurPtr = OSTaskCurPtr ;//
 			}
 		}
 		OSCInt::IntUnlock();
 		//时间服务线程收到消息，如果任务指针为NULL表示不用切换任务
 		OSMsg   Msg(OSCGuardThread::ThreadID,TM_TIMERINT,(WPARAM)CurPtr);
 		//唤醒并告诉时间服务线程，定时器中断了
 		OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
 		return TRUE;
 }   
 
bool	  OSCTimer::ThreadTimeServer(){
//给注册了线程时钟中断的线程时间服务，如果线程设定的时间到达，就会向线程发送时间到消息
//并且恢复定时计数器，用户可以在线程中重载函数OnThreadTime()，实现其他定时功能
	uint32  TempCount = PendThreadTick.GetCount();//返回链表中任务的个数
  	OSCThread  *TempTask = (OSCThread*)PendThreadTick.GetFirst();//返回第一个任务
  	while(TempCount -- ){
  		if( 0 == TempTask -> ThreadTickCnt -- ){//线程设定的时间到达
			TempTask -> ThreadTickCnt = TempTask -> ThreadTick;
			TempTask -> ThreadTimer();//调用线程的定时到达函数，发送消息通知线程时间到达	
 		}
 		TempTask = (OSCThread*)TempTask -> ThreadTickNextPtr;//移动指针
 	}//end while
	return TRUE;	
}

bool		OSCTimer::TimeServerRegister(OSCThread *ThreadPtr,uint32  Tick){
//注册线程时钟服务
	ThreadPtr -> ThreadTick = Tick;
	ThreadPtr -> ThreadTickCnt = Tick;//防止一注册就来一个消息
	return 	PendThreadTick.ListIn(ThreadPtr);
}

bool		OSCTimer::TimeServerUnregister(OSCThread *ThreadPtr){
//注销线程时间服务
	return 	PendThreadTick.ListOut(ThreadPtr);
}

bool 	OSCTimer::StartTimer(){
	MovTimerInit();
	return TRUE;
}
//END  
