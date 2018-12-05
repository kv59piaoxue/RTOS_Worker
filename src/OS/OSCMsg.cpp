//Creater: 谢国经
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
OSCMsg::OSCMsg(){
Front = 0;
Rear = 0;
}
//
bool OSCMsg::Add(OSMsg *TMsg){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(!IsFull()){
		MsgArry[Rear] = *TMsg;
		Rear = (Rear +1) %DF_MsgLength;
		TF  = TRUE;
	}
	OSCTask::TaskUnlock();
	return  TF;
}
//
bool 	OSCMsg::Delete(OSMsg*TMsg){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(!IsEmpty()){
		*TMsg = MsgArry[Front];
		Front = (Front +1)%DF_MsgLength;
		TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return  TF; 
}


bool OSCIntMsg::Add(OSMsg *TMsg){
	bool  TF = FALSE;
	OSCInt::IntLock();
	if(!IsFull()){
		MsgArry[Rear] = *TMsg;
		Rear = (Rear +1) %DF_MsgLength;
		TF  = TRUE;
	}
	OSCInt::IntUnlock();
	return  TF;
}
//
bool 	OSCIntMsg::Delete(OSMsg*TMsg){
	bool  TF = FALSE;
	OSCInt::IntLock();
	if(!IsEmpty()){
		*TMsg = MsgArry[Front];
		 Front = (Front +1)%DF_MsgLength;
		TF = TRUE;
	}
	OSCInt::IntUnlock();
	return  TF; 
}

//END
		

