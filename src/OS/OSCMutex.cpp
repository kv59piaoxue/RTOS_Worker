//л����
#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
//�����ź���
OSCMutex  *OSCMutex::MutexID = NULL;

bool  OSCMutex::MutexGetSwap(uint32 Dly){
	bool		TF = FALSE;//��ʾ���뵽�ź���
	OSCTask *CurPtr = OSTaskCurPtr ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//û�п�����Դ,�������
		OSCTask::TaskUnlock();
		if(NULL != Thread){//���߳�ռ������Դ
		//ռ����Դ���߳����ȼ��Ƿ�ȵȴ���������ȼ���
			if(Thread -> TaskPrio > CurPtr -> TaskPrio){
			//��������ռ����Դ�������������
				Thread -> ChangePrio(CurPtr -> TaskPrio);
			}
		}
		EventPend(CurPtr,Dly,Task_Info_PendMutex);
		OSCTask::Schedul();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//��ʱ����
			EventListOut(CurPtr);
			//���ﲻ�������ʱ��Ϣ����Ϊ����ǳ�ʱ���أ�
			//�ڹ黹�źż�������ʱ��Ͳ���Ҫ����һ������
			//CurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//�����ʱ��ʶ
			CurPtr -> TaskInfo &=~ Task_Info_PendMutex;//����ȴ��ź�����ʶ
			return  FALSE ;//����ź���ʧ��
		}else{//(CurPtr -> TaskInfo & Task_Info_MutexGet)
			CurPtr -> TaskInfo &=~ Task_Info_MutexGet;
			TF = TRUE;
		}
	}
	Thread = CurPtr;
	ThreadPrio = CurPtr -> TaskPrio;
	if(TF){
		CurPtr -> TaskInfo &= ~Task_Info_PendMutex ;//����ȴ��ź�����ʶ
	}else{
		OSCTask::TaskUnlock();
	}
	return TRUE;
}

//�ź�������
bool  OSCMutex::MutexGetMedi(){
	OSCTask*CurPtr = OSTaskCurPtr;
	bool  TF = TRUE ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//û�п�����Դ,�������
		TF = FALSE ;
	}
	if(TF){
		Thread = CurPtr;
		ThreadPrio = CurPtr -> TaskPrio;
	}
	OSCTask::TaskUnlock();
	return TF;
}	
//�ź�������
bool  OSCMutex::MutexPutMedi(){
	if(NULL != Thread){
	//�ڰѿ�����������ϵͳ֮ǰ���Լ����ź������ԭ��
		Thread -> ChangePrio(ThreadPrio);
		Thread = NULL;
	}
	OSCTask::TaskLock();
	Cnt ++;
	if(Cnt <= 0){//�������ڵȴ����ָ�һ������
		OSCTask::TaskUnlock();
		if(OSTaskCurPtr -> TaskInfo & Task_Info_TimeOut){
			OSTaskCurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//�����ʱ��ʶ
			return  FALSE;//û�����񱻻��ѷ���FALSE
		}
		OSCTask *TaskP = EventResume(Task_Info_MutexGet);//�ȴ����лָ�һ������
		if(NULL != TaskP){
			TaskP -> TaskInfo &= ~Task_Info_PendMutex ;//����ȴ���ʶ
			return TRUE;//�����񱻻����򷵻�TRUE
		}
		return  FALSE;
	}
	OSCTask::TaskUnlock();
	return FALSE;		
}	
//
bool  OSCMutex::MutexPutSwap(){//�ָ�һ������
	if(MutexPutMedi()){//�����񱻻��Ѿͽ��е���
		OSCTask::Schedul();
		return TRUE;//�����ϲ������񱻻���
	}
	return FALSE;	
}	

//END	
	
				
		
