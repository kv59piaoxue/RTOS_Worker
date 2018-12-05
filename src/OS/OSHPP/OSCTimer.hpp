#ifndef  __OSCTIMER_HPP
#define __OSCTIMER_HPP

class OSCTimer{
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
	friend  class 	OSCGuardThread;
	friend class	OSCTask;
	friend class	OSCInt;
	friend class 	OSCPrioMng;
	friend class	OSCEvent;
private:
	static   	OSCPendTime       PendTime;	//需要时间服务的任务队列
	static   	OSCPendTime	PendTRdy;	//
	static 	OSCPendThreadTick 		PendThreadTick;//每个线程都可以注册时间服务
	static   	uint32     CurTick;//当前任务剩余时间，当变回零时就是当前任务	
	 OSCTimer(){}
	 static 	void   	Init();
	 static 	bool	  	ThreadTimeServer(); //为每个线程提供时间片服务
	 static   	bool    	RdyResume();//恢复就绪的任务到就绪队列
	 static   	bool    	RdyLeave(OSCTask*TaskPtr);//就绪任务离开
	 static   	bool    	RdyWait(OSCTask *TaskPtr);//就绪任务等待，时间片用完就要等待
	 static   	void    	OnTime();//中断响应函数，找到定时完成的任务并恢复
	 static   	bool    	TimeLeave(OSCTask*TaskPtr);//把指定任务拿出时间队列 
	 static   	bool    	TimeWait(OSCTask *TaskPtr ,uint32  Dly);//当指定务放到时间队列等待
	 static 	bool		StartTimer();//启动定时器
public:
	 //定时器中断调用函数，向时间服务线程发送消息，唤醒时间服务线程进行时间服务
	 static 	bool	    	OnTimeInt();//中断调用
	 static   	bool		TimeServerRegister(OSCThread *ThreadPtr,uint32 Tick);//注册线程时钟服务
	 static   	bool		TimeServerUnregister(OSCThread *ThreadPtr);//注销线程时间服务	
};								

#endif	
//END