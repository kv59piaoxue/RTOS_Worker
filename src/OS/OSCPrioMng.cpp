//л����
#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
uint32   OSCPrioMng::HighPrio = 0;
OSCPrio   OSCPrioMng::PrioArry[DF_MINPRIO+1] ; 

//���ȼ����������캯��
void   OSCPrioMng::Init(){
//��ʼ��ȫ�ֱ���
	uint32  RunTime = 20;
	uint32 WaitTime = 60;
	for(int32 i = 0; i <= DF_MINPRIO; i++){
	//���ȼ�ÿ����һ�����ȴ�ʱ�������ʱ�䶼��һ��
		OSCPrio  TempPrio(RunTime*(2^i),WaitTime*(2^i));
		PrioArry[i] = TempPrio;
	}
	HighPrio = 0;//��������ȼ���ʼ����
}
//�����������ʱ�䣬���ؾ�ֵ
uint32  OSCPrioMng::SetRunTime(uint32 Prio,uint32 RunTime){
//�ı�һ�����ȼ��ĵ��������ʱ��
	OSCTask::TaskLock();
	uint32 Temp = PrioArry[Prio].MaxRunTime ;
	PrioArry[Prio].MaxRunTime = RunTime ;
	OSCTask::TaskUnlock();
	return Temp;
}
//�������ȴ�ʱ�䣬���ؾ�ֵ
uint32  OSCPrioMng::SetWaitTime(uint32 Prio,uint32 WaitTime){
//�ı�һ�����ȼ������ȴ�ʱ��
	OSCTask::TaskLock();
	uint32 Temp = PrioArry[Prio].MaxWaitTime ;
	PrioArry[Prio].MaxWaitTime = WaitTime ;
	OSCTask::TaskUnlock();
	return Temp;
}
//����һ��������ȼ�������
bool  OSCPrioMng::Find(){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(OSCTask::LockCount > 1){
		OSTaskNextPtr = OSTaskCurPtr;
		OSCTask::TaskUnlockMedi();
		return  FALSE;
	}else if(OSCGuardThread::ThreadID -> IsHasMsg){//���������û�б�����ϵͳ�ػ��߳�����Ϣ
		OSTaskNextPtr  = OSCGuardThread::ThreadID;//�л���ϵͳ�ػ��̣߳�������Ϣ
		OSCTask::TaskUnlockMedi();
		return TRUE;
	}
	while(HighPrio <  DF_MINPRIO ){
		if(!PrioArry[HighPrio].PendRdy.IsEmpty()){//��Ϊ��
			OSTaskNextPtr = PrioArry[HighPrio].PendRdy.GetFirst();//���ص�һ��
			TF = TRUE;
			break ;
		}
		HighPrio ++ ;
	}
	if( FALSE == TF){//���û�ҵ���������ȼ����������������
		OSCTimer::RdyResume();//���Իָ���������������ٴβ���
		while(HighPrio <=  DF_MINPRIO ){//�����û�о��������������ҵ�ϵͳ��������
			if(!PrioArry[HighPrio].PendRdy.IsEmpty()){//��Ϊ��
				OSTaskNextPtr = PrioArry[HighPrio].PendRdy.GetFirst();//���ص�һ��
				TF = TRUE;
				break ;
			}
			HighPrio ++ ;
		}	
	}
	OSCTask::TaskUnlockMedi();
	return TF ;
}
//����������ȡ��ָ������
bool	 OSCPrioMng::Leave(OSCTask *TaskPtr){
	return  PrioArry[TaskPtr -> TaskPrio].PendRdy.ListOut(TaskPtr);
}
//����ŵ����������еȴ�����
bool  OSCPrioMng::Wait(OSCTask *TaskPtr){
	if(TaskPtr -> TaskPrio < HighPrio )
		HighPrio = TaskPtr -> TaskPrio ;
	return   PrioArry[TaskPtr -> TaskPrio].PendRdy.ListIn(TaskPtr);//	 
}	
	
//END	
	
	
	
	
	
	