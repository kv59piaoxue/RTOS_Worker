#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
OSCGuardThread  *OSCGuardThread::ThreadID = NULL;

OSCGuardThread::OSCGuardThread():OSCThread(0)//
{//
	ThreadID = this;//
}

//重载线程过程函数处理线程消息
bool	  OSCGuardThread::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM  lParam)
 {
 //这个函数可以将中断函数发来的消息转发给用户线程，这样减少中断函数执行代码数量
 //提高系统中断响应能力,发给这个线程处理的消息STaskP必须等于this 否则任务消息不是
 //发给自己的，就直接转发消息
 	if(this != STaskP){//是需要转发的消息，转发了事
 		OSMsg    Msg(NULL,mParam,wParam,lParam);
 		STaskP -> AsyMsgSend(&Msg);//转发中断发送过来的消息
 		return TRUE;
 	}
 	//下面处理发给自己的消息
  	switch(mParam)
 	{	
 		case TM_CREATE: 
 		//响应线程创建消息，将自己从就绪队列中取出，特殊线程特殊处理
 				OSCPrioMng::Leave(this);//守护进程不在就绪队列中
 		 		break;
		case TM_TIMERINT: 
				//响应系统定时器中断发来的消息，给每个线程提供时间服务
				OSCTimer::OnTime(); 
				//为每一个线程提供时间服务，只要线程
				//设定的时间片到达，就会向对应线程发送消息
				OSCTimer::ThreadTimeServer();
				if(NULL != wParam){
					//将用完时间片的任务挂起
					if(this == (OSCTask*)wParam)  break;
					OSCPrioMng::Leave((OSCTask*)wParam);
					OSCTimer::RdyWait((OSCTask*)wParam);
				}
				break;
		case  TM_MUTEXPUTMEDI:
				((OSCMutex*)wParam) -> MutexPutMedi();
				break;  	
		default: 	//将不处理的消息交给OSCThread处理
		 		return OSCThread::ThreadProc(STaskP,mParam,wParam,lParam); 
 	}
 	return  TRUE;
 }
 
 
bool		OSCGuardThread::IntMsgSendToTask(OSMsg *TMsg){
//不会引起任务的切换，中断级函数,中断函数将消息发送给任务
//中断函数将消息发送给系统的守护线程，可能要系统线程进行某种服务，
//也可能是要他代为转发消息,这样可以避免中断函数直接与用户线程进行通信
//减少临界资源的硬保护，提高系统中断响应能力
	if(IntMsg.Add(TMsg)){
		IsHasMsg = TRUE;
		return  TRUE;
	}
	return FALSE ;
}
bool		OSCGuardThread::IntMsgAcceptFromInt(OSMsg *PMsg){//不会引起任务的切换,中断级函数,任务取中断发来的消息
//系统守护线程取中断函数发送过来的消息
//取出消息将会被派发到线程过程函数进行处理
  	if(IntMsg.Delete(PMsg)){//如果消息队列不为空
  		return  TRUE;
	}
  	return FALSE;
}

//重载线程主函数
void  	OSCGuardThread::ThreadMain()
{
//这个系统守护线程重载了线程类的ThreadMain函数实现了不同的消息循环
//这个线程是实现把中断函数的消息传达到用户线程，因此要有与中断通信的消息队列
	OSMsg  Msg;
	while(1){//线程消息循环部分
		while(!IntMsgAcceptFromInt(&Msg) && !AsyMsgAccept(&Msg)){
		//消息是空，切换到用户线程执行
			IsHasMsg = FALSE;//线程没有消息了
			Schedul();
		}
		//只需重载ThreadProc函数处理消息即可
		ThreadProc(Msg.STaskPtr, Msg.MsgID, Msg.wParam, Msg.lParam);
	}
}
//END
