
//Creater :л����
#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
 	OSCTask  *OSTaskCurPtr;
	OSCTask   *OSTaskNextPtr;//����Ĺ��캯��
  	int8  OSIntNesting;
  	int8  OSEnterSum;
  	
	OSCPendPd     OSCTask::PendPd;//�������  	
 	int32  OSCTask::LockCount = 0;  	
 	bool   OSCTask::IsRunning = FALSE;
 
OSCTask::OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop,uint32 Prio){
//����Ĵ�����������Ҫ�Ը����������г�ʼ����Ȼ�������ŵ����������еȴ�ϵͳ�ĵ���,
//����û��ָ���������ȼ�����������ȼ���������ȼ���һ��������
//��������ȼ����ᱻ�Զ�����,
	if(Prio > DF_MINPRIO - 1){//��Ϊ���Prio���޷�����������С����
		Prio = DF_MINPRIO -1;//ȷ��������ϵͳ�����ȼ�������
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
 	StkPtr = MovTaskStkInit (Task, Pdata, Ptop, EnIRQ|EnFIQ);//��ջ��ʼ���������CPU���
 	OSMsg   Msg(this,TM_CREATE);//
 	AsyMsgSend(&Msg);//���񴴽������ʹ�����Ϣ
 	OSCPrioMng::Wait(this);//�´���������ŵ��������еȴ�����	
}

 
OSCTask::OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop){
//������������Ĺ��캯��
//�������ֻ��ִ��һ��
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
 	StkPtr = MovTaskStkInit (Task, Pdata, Ptop, EnIRQ|EnFIQ);//��ջ��ʼ���������CPU���
 	OSMsg   Msg(this,TM_CREATE);//
 	AsyMsgSend(&Msg);//���񴴽������ʹ�����Ϣ
 	OSCPrioMng::Wait(this);//�´���������ŵ��������еȴ�����	
}
//���������������ʱ��	 
void  OSCTask::SetRunTime(){
	if(RemainTick){//
		 OSCTimer::CurTick = RemainTick;
	}else{
	  	OSCTimer::CurTick = OSCPrioMng::GetRunTime(TaskPrio);
	}
}	 

//�ı��������ȼ�
uint32 OSCTask::ChangePrio(uint32 Prio){
	uint32  OldPrio = TaskPrio ;
	if(OldPrio == Prio) 	return Prio;//��Ҫ���õ�ֵ��ԭ����ֵһ��
	if(Prio > DF_MINPRIO - 1){//��Ϊ���Prio���޷�����������С����
		Prio = DF_MINPRIO -1;//ȷ��������ϵͳ�����ȼ�������
	 }
	if(TaskStat & Task_Stat_Rdy){//�ھ�������
		TaskLock();//���this���ǵ�ǰ������ȼ�����ʱ�����ܻ��������Ҫ���������
		OSCPrioMng::Leave(this);//�Ӿ���������ȡ��
		TaskPrio = Prio ;
		OSCPrioMng::Wait(this);//�Żؾ�������
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
//������Ϣ�������������򱻵����߷�����Ϣ
//Ҳ��������������Ϣ
//��  TaskPtr -> MsgPost(&Msg) ��Ϣ���ᷢ�͵�TaskPtr����
//�������TaskPtr��������Ϣ�����������ȼ����ڵ����ߣ�
//��ᷢ��������л�
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

//��������������л�
//��ǰ������ָ����������Ϣ
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
//���ص�ǰ���������Ϣ
bool		OSCTask::AsyMsgAccept(OSMsg *PMsg){
  	if(TaskMsg.Delete(PMsg)){//�����Ϣ���в�Ϊ��
  		return  TRUE;
	}
  	return FALSE;
  }
  
//������Ⱥ���	
void  OSCTask::Schedul(){//static
	if(!IsRunning)  return ;
	if(OSIntNesting > 0) return ;//�������жϵ���
	if(TRUE ==  OSCPrioMng::Find()){//������һ��������ȼ�����
		if(OSTaskNextPtr != OSTaskCurPtr){
			OSTaskCurPtr -> RemainTick = OSCTimer::CurTick;//����������ռʱ����¼����ʣ�µ�ʱ��
			OSTaskNextPtr -> SetRunTime();//���ý�Ҫ����������������ʱ��
			OS_TASK_SW();
		}
	}
}
//
void  OSCTask::Sleep(uint32  Dly){//static
//��ѵ����߹����趨��ʱ��
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
	if(OSCPrioMng::Find()){//������һ��������ȼ�����
		OSCTimer::CurTick = 0;//��һ���ȵ�һ���������߳�
		OSCTimer::StartTimer();
		OSEnterSum	= 0;//����ϵͳ�ж�
		IsRunning = TRUE;
		OS_SYS_STARTUP();
	}
}

void  	OSCTask::SysInit(){
	LockCount  =  0;  	
	OSIntNesting = 0;
	OSEnterSum = 1;//��ֹIntUnlock�������ж�
	OSTaskCurPtr  = NULL;
	OSTaskNextPtr = NULL;
	IsRunning = FALSE;
	OSCPendPd    TempPd;
	PendPd = TempPd;
	MovTargetInit();//���ݲ�ͬ��Ŀ�����г�ʼ������ֲ����
	OSCPrioMng::Init();   //��ʼ����̬��Ա
	OSCTimer::Init();	//��ʼ����̬��Ա
	OSCInt::Init();     //��ʼ����̬��Ա
}

bool		OSCTask::TaskPend(){//��������  PendPd����
		PendPd.ListIn(this);
		OSCPrioMng::Leave(this);
		return TRUE;
}
bool		OSCTask::TaskResume(){
//��PendPd������ �Ƴ����� �����ظ�����������
		bool		TF = FALSE;
		if(PendPd.ListOut(this)){
			OSCPrioMng::Wait(this);
			TF = TRUE;
		}
		return TF;
}

 void    OSCTask::TaskLock()
 {//static
 //��������������ִ����������󣬲����Խ��������
 //���ȣ�ȷ��ϵͳ���ٽ���Դ�õ�����
 	OSCInt::IntLock();
 	LockCount ++;
 	OSCInt::IntUnlock();
 }//
void 	OSCTask::TaskUnlock()
{//static   
//�����������ڽӴ������������һ������ĵ��ȣ�ȷ��ϵͳ�ػ��߳�
//���������������ȼ�����õ���ʱ�ĵ���
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
	if(OSIntNesting > 0) return ;//�������жϵ���
	if(OSCTask::LockCount > 0){
		return ;
	}else if(OSTaskCurPtr ==  OSCGuardThread::ThreadID){
		return ;
	}else if(OSCGuardThread::ThreadID -> IsHasMsg){//���������û�б�����ϵͳ�ػ��߳�����Ϣ
		OSTaskCurPtr -> RemainTick = OSCTimer::CurTick;//����������ռʱ����¼����ʣ�µ�ʱ��
		OSCTimer::CurTick =0;
		OSTaskNextPtr  = OSCGuardThread::ThreadID;//�л���ϵͳ�ػ��̣߳�������Ϣ
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
	}else{//û���յ�ͬ����Ϣ�����������ȴ���Ϣ	
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

bool		OSCTask::AsyMsgTelevise(OSMsg*PMsg){//��Ϣ�㲥
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
	
	
	
	
	