#ifndef __OSCPRIOMNG_HPP
#define __OSCPRIOMNG_HPP

	class  OSCPrio{
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
		friend  class OSCPrioMng;
	private:
		OSCPendRdy    PendRdy;
		uint32   MaxRunTime;
		uint32   MaxWaitTime;
		//Function
		OSCPrio(){}
		OSCPrio(uint32 RunTime,uint32 WaitTime){//设置运行和等待时间，单位毫秒
		MaxRunTime = DF_TICKS_PSEC*RunTime/1000;//RunTime  (ms)
		MaxWaitTime = DF_TICKS_PSEC*WaitTime/1000;//WaitTime (ms)
		}
	};
	
class  OSCPrioMng{
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
	friend  class   OSCTask;
	friend class     OSCTimer;
	friend class 	OSCGuardThread;
	friend class  	OSCEvent;
private:
	static     	OSCPrio   PrioArry[DF_MINPRIO+1];
	static     	uint32     HighPrio;
	OSCPrioMng(){}
	static 	void 	Init();
	static     	bool  	Find();//获得一个就绪任务，存放到当前任务指针中
	static     	bool    	Leave(OSCTask *TaskPtr);//取出任务
	static     	bool         Wait(OSCTask *TaskPtr);//任务放到就绪队列中等待
public:	
//以下函数虽然对用户公开，但最好不要改变系统预先设定的值
	static     	uint32     GetRunTime(uint32  Prio){return PrioArry[Prio].MaxRunTime;}//
	static     	uint32     GetWaitTime(uint32  Prio){return PrioArry[Prio].MaxWaitTime;}//
	static     	uint32   	SetRunTime(uint32  Prio ,  uint32 RunTime);//设置最大运行时间，返回旧的最大运行时间
	static     	uint32   	SetWaitTime(uint32  Prio , uint32 WaitTime);//
};
#endif
//END