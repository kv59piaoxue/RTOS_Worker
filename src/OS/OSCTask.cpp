
//Creater :谢国经
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
 	OSCTask  *OSTaskCurPtr;
	OSCTask   *OSTaskNextPtr;//任务的构造函数
  	int8  OSIntNesting;
  	int8  OSEnterSum;
  	
	OSCPendPd     OSCTask::PendPd;//挂起队列  	
 	int32  OSCTask::LockCount = 0;  	
 	bool   OSCTask::IsRunning = FALSE;
 
OSCTask::OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop,uint32 Prio){
//任务的创建函数，主要对各个变量进行初始化，然后把任务放到就绪队列中等待系统的调度,
//如果用户分给任务的优先级不在最低优先级和最高优先级减一这个区间里，
//任务的优先级将会被自动调整,
	if(Prio > DF_MINPRIO - 1){//因为这个Prio是无符号数不可能小于零
		Prio = DF_MINPRIO -1;//确保任务在系统的优先级队列中
	 }
 	TaskPrio = Prio ;
 	TaskDly = 0;
 	TaskStat = 0;  
 	TaskInfo = 0;
 	RemainTick = 0;
 	EventList = NULL;
 	SynTaskPtr = NULL;
 	EventNextPtr = NULL;
 	EventPrevPtr = NULL ;
 	TimeNextPtr = NULL ;
 	TimePrevPtr = NULL ;
 	PdNextPtr = NULL;
 	PdPrevPtr = NULL;
 	ThreadTickNextPtr = NULL;
 	ThreadTickPrevPtr = NULL;
 	StkPtr = MovTaskStkInit (Task, Pdata, Ptop, EnIRQ|EnFIQ);//堆栈初始化，与具体CPU相关
 	OSMsg   Msg(this,TM_CREATE);//
 	AsyMsgSend(&Msg);//任务创建，发送创建消息
 	OSCPrioMng::Wait(this);//新创建的任务放到就绪队列等待调度	
}

 
OSCTask::OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop){
//创建空闲任务的构造函数
//这个函数只能执行一次
 	TaskPrio = DF_MINPRIO;
 	TaskDly = 0;
 	TaskStat = 0;  
 	TaskInfo = 0;
 	 RemainTick = 0;
 	 EventList = NULL;
 	 SynTaskPtr = NULL;
 	EventNextPtr = NULL;
 	EventPrevPtr = NULL ;
 	TimeNextPtr = NULL ;
 	TimePrevPtr = NULL ;
 	PdNextPtr = NULL;
 	PdPrevPtr = NULL;
 	StkPtr = MovTaskStkInit (Task, Pdata, Ptop, EnIRQ|EnFIQ);//堆栈初始化，与具体CPU相关
 	OSMsg   Msg(this,TM_CREATE);//
 	AsyMsgSend(&Msg);//任务创建，发送创建消息
 	OSCPrioMng::Wait(this);//新创建的任务放到就绪队列等待调度	
}
//返回任务最大运行时间	 
void  OSCTask::SetRunTime(){
	if(RemainTick){//
		 OSCTimer::CurTick = RemainTick;
	}else{
	  	OSCTimer::CurTick = OSCPrioMng::GetRunTime(TaskPrio);
	}
}	 

//改变任务优先级
uint32 OSCTask::ChangePrio(uint32 Prio){
	uint32  OldPrio = TaskPrio ;
	if(OldPrio == Prio) 	return Prio;//需要设置的值与原来的值一样
	if(Prio > DF_MINPRIO - 1){//因为这个Prio是无符号数不可能小于零
		Prio = DF_MINPRIO -1;//确保任务在系统的优先级队列中
	 }
	if(TaskStat & Task_Stat_Rdy){//在就绪队列
		TaskLock();//如果this就是当前最高优先级任务时，可能会出错，所以要锁任务调度
		OSCPrioMng::Leave(this);//从就绪队列中取出
		TaskPrio = Prio ;
		OSCPrioMng::Wait(this);//放回就绪队列
		TaskUnlock();
	}	
	if(TaskStat &Task_Stat_Event){		
		EventList[TaskPrio].ListOut(this);
		TaskPrio = Prio ;
		EventList[Prio].ListIn(this);
	}
	TaskPrio = Prio ;
	return OldPrio;
}
//
bool  OSCTask::AsyMsgPost(OSMsg *TMsg){
//发送消息函数，调用者向被调用者发送消息
//也可以向自身发送消息
//如  TaskPtr -> MsgPost(&Msg) 消息将会发送到TaskPtr任务
//并且如果TaskPtr任务获得消息就绪，且优先级高于调用者，
//则会发生任务的切换
	bool		TF = FALSE;
	if(TaskMsg.Add(TMsg)){
		IsHasMsg = TRUE;
		if(TaskStat & Task_Stat_Pd){
			TaskResume();
		}
		Schedul();
		TF = TRUE;
	}
	return TF ;
}

//不会引起任务的切换
//当前任务向指定任务发送消息
bool  OSCTask::AsyMsgSend(OSMsg *PMsg){
	bool		TF = FALSE;
	if(TaskMsg.Add(PMsg)){
		IsHasMsg = TRUE;
		if(TaskStat & Task_Stat_Pd){
			TaskResume();
		}
		TF = TRUE;
	}
	return TF ;
}
//返回当前任务快速消息
bool		OSCTask::AsyMsgAccept(OSMsg *PMsg){
  	if(TaskMsg.Delete(PMsg)){//如果消息队列不为空
  		return  TRUE;
	}
  	return FALSE;
  }
  
//任务调度函数	
void  OSCTask::Schedul(){//static
	if(!IsRunning)  return ;
	if(OSIntNesting > 0) return ;//不可以中断调用
	if(TRUE ==  OSCPrioMng::Find()){//查找下一个最高优先级任务
		if(OSTaskNextPtr != OSTaskCurPtr){
			OSTaskCurPtr -> RemainTick = OSCTimer::CurTick;//在任务发生抢占时，记录任务剩下的时间
			OSTaskNextPtr -> SetRunTime();//设置将要调度任务的最大运行时间
			OS_TASK_SW();
		}
	}
}
//
void  OSCTask::Sleep(uint32  Dly){//static
//会把调用者挂起设定的时间
	OSCTask *Cur = OSTaskCurPtr;
	if(Dly > 0){
		OSCTimer::TimeWait(Cur,Dly);
		OSCPrioMng::Leave(Cur);
		Schedul();
		Cur -> TaskInfo  &= ~Task_Info_TimeOut ;//
	}
}	

void  OSCTask::StartUp(){
	if(IsRunning)  return ; 
	if(OSCPrioMng::Find()){//查找下一个最高优先级任务
		OSCTimer::CurTick = 0;//第一调度的一定是特殊线程
		OSCTimer::StartTimer();
		OSEnterSum	= 0;//允许系统中断
		IsRunning = TRUE;
		OS_SYS_STARTUP();
	}
}

void  	OSCTask::SysInit(){
	LockCount  =  0;  	
	OSIntNesting = 0;
	OSEnterSum = 1;//防止IntUnlock（）误开中断
	OSTaskCurPtr  = NULL;
	OSTaskNextPtr = NULL;
	IsRunning = FALSE;
	OSCPendPd    TempPd;
	PendPd = TempPd;
	MovTargetInit();//根据不同的目标板进行初始化，移植函数
	OSCPrioMng::Init();   //初始化静态成员
	OSCTimer::Init();	//初始化静态成员
	OSCInt::Init();     //初始化静态成员
}

bool		OSCTask::TaskPend(){//挂起任务到  PendPd链表
		PendPd.ListIn(this);
		OSCPrioMng::Leave(this);
		return TRUE;
}
bool		OSCTask::TaskResume(){
//从PendPd链表上 移除任务 ，并回复到就绪队列
		bool		TF = FALSE;
		if(PendPd.ListOut(this)){
			OSCPrioMng::Wait(this);
			TF = TRUE;
		}
		return TF;
}

 void    OSCTask::TaskLock()
 {//static
 //加任务锁，任务执行这个函数后，不可以进行任务的
 //调度，确保系统的临界资源得到保护
 	OSCInt::IntLock();
 	LockCount ++;
 	OSCInt::IntUnlock();
 }//
void 	OSCTask::TaskUnlock()
{//static   
//解任务锁，在接触任务锁后进行一次任务的调度，确保系统守护线程
//和其他就绪高优先级任务得到及时的调度
	OSCInt::IntLock();
	LockCount -- ;
	SchedulGuardThread();
	OSCInt::IntUnlock();
}//
void 	OSCTask::TaskUnlockMedi(){
	OSCInt::IntLock();
	LockCount -- ;
	OSCInt::IntUnlock();
}
void  OSCTask::SchedulGuardThread(){
	if(!IsRunning)  return ;
	if(OSIntNesting > 0) return ;//不可以中断调用
	if(OSCTask::LockCount > 0){
		return ;
	}else if(OSTaskCurPtr ==  OSCGuardThread::ThreadID){
		return ;
	}else if(OSCGuardThread::ThreadID -> IsHasMsg){//任务调度器没有被锁、系统守护线程有消息
		OSTaskCurPtr -> RemainTick = OSCTimer::CurTick;//在任务发生抢占时，记录任务剩下的时间
		OSCTimer::CurTick =0;
		OSTaskNextPtr  = OSCGuardThread::ThreadID;//切换到系统守护线程，处理消息
		OS_TASK_SW();
	}
}

bool		OSCTask::SynMsgPost(OSCTask*TaskP,OSMsg*PMsg){
	bool		TF = FALSE;
	bool		IsPrioChange = FALSE;
	uint32	OldPrio;
	OSCTask  *Cur = OSTaskCurPtr;
	TaskLock();
	if(TaskP -> SynTaskPtr == NULL){
		TaskP -> SynTaskPtr = Cur;
		TaskUnlock();
		Cur -> SynMsg = *PMsg;
		if(TaskP -> TaskPrio > Cur ->TaskPrio){
			OldPrio =TaskP -> ChangePrio(Cur -> TaskPrio);
			IsPrioChange = TRUE;
		}
		OSCPrioMng::Wait(TaskP);
		OSCPrioMng::Leave(Cur);
		Schedul();
		if(IsPrioChange){
			TaskP -> ChangePrio(OldPrio);
		}
		TF = TRUE;
	}	
	TaskUnlock();
 	return TF;	
}

bool		OSCTask::SynMsgWait(OSMsg *PMsg){
	bool		TF = FALSE;
	OSCTask  *Cur = OSTaskCurPtr;
	TaskLock();
	if(Cur -> SynTaskPtr != NULL){
		*PMsg = Cur -> SynTaskPtr -> SynMsg;
		OSCPrioMng::Wait(Cur -> SynTaskPtr);
		Cur -> SynTaskPtr = NULL;
		TaskUnlock();
		Schedul();
		TF = TRUE;
	}else{//没有收到同步消息，则挂起自身等待消息	
		while(Cur -> SynTaskPtr == NULL){
			TaskUnlock();
			OSCPrioMng::Leave(Cur);
			Schedul();
			TaskLock();
		}
		*PMsg = Cur -> SynTaskPtr -> SynMsg;
		OSCPrioMng::Wait(Cur -> SynTaskPtr);
		Cur -> SynTaskPtr = NULL;
		TaskUnlock();
		Schedul();
		TF = TRUE;
	}
	return TF;	
}

bool		OSCTask::AsyMsgTelevise(OSMsg*PMsg){//消息广播
	OSCTask   *TaskP;
  	while(!PendPd.IsEmpty()){
  		TaskP = PendPd.GetFirst();
  		if(PendPd.ListOut(TaskP)){
  			OSCPrioMng::Wait(TaskP);
  			TaskP -> TaskMsg.Add(PMsg);
  			TaskP -> IsHasMsg = TRUE;
  		}
  	}
  	Schedul();
  	return TRUE;  
}
//END
	
	
	
	
	