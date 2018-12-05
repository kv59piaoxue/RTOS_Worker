#ifndef  __OSCGUARD_THREAD_HPP
#define   __OSCGUARD_THREAD_HPP

	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
class  OSCGuardThread:public OSCThread{
//这个线程是连接中断与用户线程的桥梁，转发中断函数发
//给用户线程的消息，确保系统中断响应时间为常数
//次线程还是系统的时间服务线程。这个线程不在就绪队列中，
//可是只要这个线程收到消息并且系统没有上任务锁
//就会第一时间得到执行
	 private:
	 //消息
	OSCIntMsg		IntMsg;//中断函数将消息发送给任务	
	 bool	IntMsgAcceptFromInt(OSMsg *PMsg);//接收中断函数发来的消息
	//重载线程主函数，实现不同的消息循环。因为这个线程的消息来源和用户线程的消息来源不一样
	 virtual   	void  ThreadMain();
	 virtual	bool	  ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM  lParam);	 
	 public:
	 static	OSCGuardThread  * ThreadID;//用于任务通信的指针，只能初始化一次
	 OSCGuardThread();
	 //不会引起任务的切换，中断级函数,中断函数将消息发送给任务 
	 bool	IntMsgSendToTask(OSMsg *PMsg);
};
#endif
//END 