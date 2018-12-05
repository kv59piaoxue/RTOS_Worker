//
#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
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
 
 //�ָ���ʱ������еȴ��ľ�������
  bool  OSCTimer::RdyResume(){
  	OSCTask   *TaskP;
  	while(!PendTRdy.IsEmpty()){
  		TaskP = PendTRdy.GetFirst();
  		if(PendTRdy.ListOut(TaskP)){
  			//TaskP -> RemainTick = 0;//ʱ��Ƭ
  			OSCPrioMng::Wait(TaskP);
  		}
  	}
  	return TRUE;
 } //end function
 
bool		OSCTimer::RdyLeave(OSCTask *TaskPtr){
	return  PendTRdy.ListOut(TaskPtr);	
}

bool  	OSCTimer::RdyWait(OSCTask *TaskPtr){
		if(PendTRdy.ListIn(TaskPtr)){//����ȴ�����		
			TaskPtr -> TaskDly =  OSCPrioMng::GetWaitTime(TaskPtr -> TaskPrio);//�������ȴ�ʱ��
			return TRUE;
		}
		return FALSE;
}
 //�뿪ʱ�����
 bool  OSCTimer::TimeLeave(OSCTask*TaskPtr){
 	if(PendTime.ListOut(TaskPtr)){
 		TaskPtr -> TaskInfo &= ~Task_Info_TimeOut ;//�����뿪�����ǳ�ʱ
 		return   TRUE;
 	}
 	return FALSE;
 }
 
 
 //����ʱ����еȴ�
  bool  OSCTimer::TimeWait(OSCTask*TaskPtr,uint32  Dly){
 	 if(PendTime.ListIn(TaskPtr)){
 	 	 TaskPtr -> TaskDly = Dly ;
 	 	 return TRUE;
 	 }
 	 return FALSE;
 }
 //ʱ����������жϺ�������
 void  OSCTimer::OnTime(){//��Ϊ�������ֻ����ϵͳ���ȼ���ߵ�ʱ������̵߳��ã�
 		if(!PendTime.IsEmpty()){//��������ʱ�����
  		 	uint32  TempCount = PendTime.GetCount();//��������������ĸ���
  			OSCTask  *TempTask1,*TempTask2 = PendTime.GetFirst();//���ص�һ������
  			while(TempCount -- ){
  				if( 0 == TempTask2 -> TaskDly --){//������ʱ����
 					TempTask1 = TempTask2 -> TimeNextPtr ;//������һ������ĵ�ַ
 					PendTime.ListOut(TempTask2);//
 					TempTask2 -> TaskInfo  |= Task_Info_TimeOut ;//��������ķ���״̬Ϊ��ʱ
 					OSCPrioMng::Wait(TempTask2);//���������еȴ�
 					TempTask2 = TempTask1 ;//
 				}else{
 					TempTask2 = TempTask2 -> TimeNextPtr ;
 				}//end if
 		 	}//end while
 		}//end if	
 		//�����������ȴ�ʱ�䵽
 		 if(!PendTRdy.IsEmpty()){
  		 	uint32  TempCount = PendTRdy.GetCount();//��������������ĸ���
  			OSCTask  *TempTask1,*TempTask2 = PendTRdy.GetFirst();//���ص�һ������
  			while(TempCount -- ){
  				if( 0 == TempTask2 -> TaskDly -- ){//������ʱ����
 					TempTask1 = TempTask2 -> TimeNextPtr ;//������һ������ĵ�ַ
 					PendTRdy.ListOut(TempTask2);//
 					OSCPrioMng::Wait(TempTask2);//���������еȴ�
 					TempTask2 = TempTask1 ;//
 				}else{
 					TempTask2 = TempTask2 -> TimeNextPtr ;
 				}//end if
 		 	}//end while
 		}//end if	
 }
 
 
 bool	OSCTimer::OnTimeInt(){
 //��ʱ���жϺ������ã�����ʱ����񣬽���ʱ���жϵ���Ϣ���͸�
 //ʱ������̣߳�����ʱ������߳̽���ʱ�����
 //������������ϵͳ�ж���Ӧ��ʱ��
 		OSCTask *CurPtr = NULL;	
 		OSCInt::IntLock();
 		if(CurTick){//����0--��ɺܴ�һ����Ŷ
 		    	CurTick -- ;
 			if(!CurTick){//��ǰ�����������ʱ�䵽
 				CurPtr = OSTaskCurPtr ;//
 			}
 		}
 		OSCInt::IntUnlock();
 		//ʱ������߳��յ���Ϣ���������ָ��ΪNULL��ʾ�����л�����
 		OSMsg   Msg(OSCGuardThread::ThreadID,TM_TIMERINT,(WPARAM)CurPtr);
 		//���Ѳ�����ʱ������̣߳���ʱ���ж���
 		OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
 		return TRUE;
 }   
 
bool	  OSCTimer::ThreadTimeServer(){
//��ע�����߳�ʱ���жϵ��߳�ʱ���������߳��趨��ʱ�䵽��ͻ����̷߳���ʱ�䵽��Ϣ
//���һָ���ʱ���������û��������߳������غ���OnThreadTime()��ʵ��������ʱ����
	uint32  TempCount = PendThreadTick.GetCount();//��������������ĸ���
  	OSCThread  *TempTask = (OSCThread*)PendThreadTick.GetFirst();//���ص�һ������
  	while(TempCount -- ){
  		if( 0 == TempTask -> ThreadTickCnt -- ){//�߳��趨��ʱ�䵽��
			TempTask -> ThreadTickCnt = TempTask -> ThreadTick;
			TempTask -> ThreadTimer();//�����̵߳Ķ�ʱ���ﺯ����������Ϣ֪ͨ�߳�ʱ�䵽��	
 		}
 		TempTask = (OSCThread*)TempTask -> ThreadTickNextPtr;//�ƶ�ָ��
 	}//end while
	return TRUE;	
}

bool		OSCTimer::TimeServerRegister(OSCThread *ThreadPtr,uint32  Tick){
//ע���߳�ʱ�ӷ���
	ThreadPtr -> ThreadTick = Tick;
	ThreadPtr -> ThreadTickCnt = Tick;//��ֹһע�����һ����Ϣ
	return 	PendThreadTick.ListIn(ThreadPtr);
}

bool		OSCTimer::TimeServerUnregister(OSCThread *ThreadPtr){
//ע���߳�ʱ�����
	return 	PendThreadTick.ListOut(ThreadPtr);
}

bool 	OSCTimer::StartTimer(){
	MovTimerInit();
	return TRUE;
}
//END  
