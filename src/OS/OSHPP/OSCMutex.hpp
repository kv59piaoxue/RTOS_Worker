#ifndef  __OSCMUYEX_HPP
#define __OSCMUYEX_HPP
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
class  OSCMutex :public  OSCEvent{//任务级使用的信号量
	private:
	int32   	Cnt;//互斥量计数器
	OSCTask  *Thread;//当前占用互斥量的线程
	uint32   	ThreadPrio;//占用互斥量的线程的优先级
	public :
	static 	OSCMutex*  MutexID;//调试使用变量
	OSCMutex(int32  Mutex):OSCEvent(){//
		Cnt = Mutex ;
		Thread = NULL;
		ThreadPrio = 0;
		MutexID = this;
	}
		//无等待
		bool	 MutexGetMedi();
		bool  MutexPutMedi();
		//有等待
		bool  MutexGetSwap(uint32 Dly);
		bool  MutexPutSwap();
};
#endif
//END