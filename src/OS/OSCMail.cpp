	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��

#include "config.hpp"
OSCMail *OSCMail::MailID = NULL;

bool		 OSCMail::MsgGetSwap(OSMsg *Msg,uint32 Dly ){
	if(OSIntNesting > 0) return  FALSE ;
	OSCTask *CurPtr = OSTaskCurPtr ;
	bool 	TF = FALSE;
	if(!TaskMsg.Delete(Msg)){//û����Ϣ����ȡ���������ȴ���Ϣ
		EventPend(CurPtr,Dly,Task_Info_PendMailQ);
		OSCTask::Schedul();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//��ʱ����
			EventListOut(CurPtr);
			CurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//�����ʱ��ʶ
			CurPtr -> TaskInfo &= ~Task_Info_PendMailQ;
		}else if(CurPtr -> TaskInfo & Task_Info_MailQGet){//û�г�ʱ����Ϣ���͵�����
			CurPtr -> TaskInfo &= ~Task_Info_MailQGet;
			*Msg = CurPtr -> MailMsg;
			TF = TRUE;
		}
	}else{//��Ϣ���в��ǿգ�ֱ��ȡ����Ϣ
		TF = TRUE;
	}
	return  TF ;
}
//
bool   OSCMail::MsgPutSwap(OSMsg *Msg){
	bool	 TF = FALSE;
	TF = MsgPutMedi(Msg);
	if(TF){
		OSCTask::Schedul();
	}
	return TF;
}
//
bool		 OSCMail::MsgGetMedi(OSMsg *Msg){
	if(TaskMsg.Delete(Msg)){//����Ϣ��ȡ��û�оͷ��ؿ�
		return  TRUE;
	}
	return  FALSE;
}

//
bool   OSCMail::MsgPutMedi(OSMsg*Msg){
	bool	  TF = FALSE;
	if(TaskMsg.IsEmpty()){//��Ϣ����Ϊ��
		OSCTask*Temp = EventResume(Task_Info_MailQGet);
		if(Temp != NULL){//������ָ�����
			Temp -> MailMsg = *Msg;
			Temp -> TaskInfo &= ~ Task_Info_PendMailQ;//�ָ�����������ȴ���ʶ
			TF = TRUE;//�����񱻻��Ѿͷ���TRUE
		}else{//������û�еȴ�������Ϣ����Ҳ�ǿ�
			TaskMsg.Add(Msg);
		}
	}else{//z��Ϣ����������Ϣ��˵�����������һ��û�������ڵȴ�
		TaskMsg.Add(Msg);
	}
	return TF ;
}
bool		OSCMail::MsgTeleviseSwap(OSMsg*Msg){
	bool	 TF = FALSE;
	TF = MsgTeleviseMedi(Msg);
	if(TF){
		OSCTask::Schedul();
	}
	return  TF;
}

bool		OSCMail::MsgTeleviseMedi(OSMsg*Msg){
	bool		TF = FALSE;
	OSCTask*Temp = EventResume(Task_Info_MailQGet);
	while(Temp != NULL){//������ָ�����
		Temp -> MailMsg = *Msg;
		Temp -> TaskInfo &= ~ Task_Info_PendMailQ;//�ָ�����������ȴ���ʶ
		Temp = EventResume(Task_Info_MailQGet);
		TF = TRUE;
	}
	return  TF;
}
//END
