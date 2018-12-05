#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
 OSCThread::OSCThread(uint32 Prio):OSCTask(Task,this,&Stack[DF_ThreadStackDeep -1],Prio)
 {//用户线程调用的构造函数
 	 ThreadTickCnt = 0;
 	ThreadTick = 0;
 }
 
 OSCThread::OSCThread():OSCTask(Task,this,&Stack[DF_ThreadStackDeep -1])
 {//空闲任务调用，其他任务不能调用
  	ThreadTickCnt = 0;
 	ThreadTick = 0;
 }
void  	OSCThread::ThreadMain(){//virtual
//这个函数为线程主函数，用户可以通过重载这个函数
//但如果这个函数被重载，线程将没有消息循环功能，
		OSMsg  Msg;
		while(1){//线程消息循环部分
			while(!AsyMsgAccept(&Msg)){//连个队列消息都是空，挂起任务等待消息
				IsHasMsg = FALSE;
				TaskPend();//没有消息需要处理就进入挂起状态
				Schedul();//调度其他任务
			}
				//取到消息派发到线程过程函数，用户
				//只需重载虚函数ThreadProc处理线程消息队列中的即可
			ThreadProc(Msg.STaskPtr, Msg.MsgID, Msg.wParam, Msg.lParam);
		}
}

  bool	  OSCThread::ThreadTimer(){
  	OSMsg  Msg(NULL,TM_THREADONTIME);
         return  	AsyMsgSend(&Msg);
  }
bool	OSCThread::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM lParam)
 {//virtual
 //线程过程函数，用户需要重载这个函数处理线程消息队列中的消息
 //对于用户不处理的消息，可以交个父线程的线程过程函数来处理
 	STaskP = STaskP;
 	wParam = wParam;
 	lParam = lParam;
  	switch(mParam)
 	{
 	
 	}
 	return  TRUE;
 }
//END
