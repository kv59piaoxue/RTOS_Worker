	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年

#include "config.hpp"
OSCMail *OSCMail::MailID = NULL;

bool		 OSCMail::MsgGetSwap(OSMsg *Msg,uint32 Dly ){
	if(OSIntNesting > 0) return  FALSE ;
	OSCTask *CurPtr = OSTaskCurPtr ;
	bool 	TF = FALSE;
	if(!TaskMsg.Delete(Msg)){//没有消息可以取，任务挂起等待消息
		EventPend(CurPtr,Dly,Task_Info_PendMailQ);
		OSCTask::Schedul();
		if(CurPtr -> TaskInfo & Task_Info_TimeOut){//超时返回
			EventListOut(CurPtr);
			CurPtr -> TaskInfo &= ~Task_Info_TimeOut ;//清除超时标识
			CurPtr -> TaskInfo &= ~Task_Info_PendMailQ;
		}else if(CurPtr -> TaskInfo & Task_Info_MailQGet){//没有超时，消息发送到任务
			CurPtr -> TaskInfo &= ~Task_Info_MailQGet;
			*Msg = CurPtr -> MailMsg;
			TF = TRUE;
		}
	}else{//消息队列不是空，直接取走消息
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
	if(TaskMsg.Delete(Msg)){//有消息就取，没有就返回空
		return  TRUE;
	}
	return  FALSE;
}

//
bool   OSCMail::MsgPutMedi(OSMsg*Msg){
	bool	  TF = FALSE;
	if(TaskMsg.IsEmpty()){//消息队列为空
		OSCTask*Temp = EventResume(Task_Info_MailQGet);
		if(Temp != NULL){//有任务恢复出来
			Temp -> MailMsg = *Msg;
			Temp -> TaskInfo &= ~ Task_Info_PendMailQ;//恢复了任务，清除等待标识
			TF = TRUE;//有任务被唤醒就返回TRUE
		}else{//队列中没有等待任务，消息队列也是空
			TaskMsg.Add(Msg);
		}
	}else{//z消息队列中有消息，说明任务队列中一定没有任务在等待
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
	while(Temp != NULL){//有任务恢复出来
		Temp -> MailMsg = *Msg;
		Temp -> TaskInfo &= ~ Task_Info_PendMailQ;//恢复了任务，清除等待标识
		Temp = EventResume(Task_Info_MailQGet);
		TF = TRUE;
	}
	return  TF;
}
//END
