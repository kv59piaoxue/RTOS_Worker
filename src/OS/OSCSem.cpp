//л����
#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
OSCSem *OSCSem::SemID = NULL;
//�����ź���
bool  OSCSem::SemGetSwap(uint32 Dly){
	bool		TF = FALSE;//
	OSCTask *CurPtr = OSTaskCurPtr ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//û�п�����Դ,�������
		OSCTask::TaskUnlock();
		EventPend(CurPtr,Dly,Task_Info_PendMutex);
		OSCTask::Schedul();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//��ʱ����
			EventListOut(CurPtr);
			CurPtr -> TaskInfo &=~ Task_Info_PendSem;//����ȴ��ź�����ʶ
			return  FALSE ;//����ź���ʧ��
		}else{//  if(CurPtr -> TaskInfo & Task_Info_SemGet)
			CurPtr -> TaskInfo &=~ Task_Info_SemGet;
			TF = TRUE;
		}
	}
	if(TF){
		CurPtr -> TaskInfo &= ~Task_Info_PendSem ;//����ȴ��ź�����ʶ
	}else{
		OSCTask::TaskUnlock();
	}
	return TRUE;
}


//�ź�������
bool  OSCSem::SemGetMedi(){
	bool  TF = TRUE ;
	OSCTask::TaskLock();
	Cnt --;
	if(Cnt < 0){//û�п�����Դ,�������
		TF = FALSE ;
	}
	OSCTask::TaskUnlock();
	return TF;
}	
//�ź�������
bool  OSCSem::SemPutMedi(){
	OSCTask *CurPtr = OSTaskCurPtr;
	OSCTask::TaskLock();
	Cnt ++;
	if(Cnt <= 0){//�������ڵȴ����ָ�һ������
		OSCTask::TaskUnlock();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//��������ǳ�ʱ���أ���
			CurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//�����ʱ��ʶ
			return FALSE;
		}
		OSCTask *TaskP = EventResume(Task_Info_SemGet);//�ȴ����лָ�һ������
		if(NULL != TaskP){
			TaskP -> TaskInfo &= ~Task_Info_PendSem ;//����ȴ���ʶ
			return TRUE;
		}
		return FALSE;
	}
	OSCTask::TaskUnlock();
	return FALSE ;		
}	
//
bool  OSCSem::SemPutSwap(){//�ָ�һ������
	if(SemPutMedi()){
		OSCTask::Schedul();
		return TRUE;	
	}
	return FALSE;
}	
//END	
	
