#ifndef  __OSCTHREAD_HPP
#define __OSCTHREAD_HPP
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年

class  OSCThread:public  OSCTask{
	friend  class  OSCTimer;
	friend class  OSCIdlThread;//定义为友元防止其他派生类调用OSCThread();空闲任务构造函数
	 private:
	 OSCThread();//空闲任务构造函数这能被OSCIdlThread调用
 	OS_STK   Stack[DF_ThreadStackDeep];
 	 static   void Task(void*Pdata){//
 		((OSCThread*)Pdata) -> ThreadMain();
 	}
 	uint32 	ThreadTickCnt;//时间服务计数器
	uint32 	ThreadTick;//
 	protected:
 	OSCThread(uint32 Prio);//不可以创建线程类，用户线程构造函数
 	virtual    bool	  ThreadTimer();//线程设定的时间Tick到就会调用这个函数
 	virtual   	void   ThreadMain();//
 	virtual	bool	  ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM lParam);
 };

#endif
//END