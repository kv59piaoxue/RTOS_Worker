#ifndef  __OSCTASK_HPP
#define __OSCTASK_HPP
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
class OSCTask{
//线程控制块PCB
friend  	class  	OSCPendRdy;
friend  	class  	OSCPendTime;
friend  	class 	OSCPendEvent;
friend 	class		OSCPendPd;
friend 	class 	OSCPendThreadTick;
friend  	class 	OSCPrioMng;
friend 	class		OSCTimer;
friend	class		OSCInt;
friend 	class         OSCEvent;
friend	class    	OSCMutex;
friend	class    	OSCSem;
friend	class     	OSCMsg;
friend	class     	OSCMail;
	private:
	OS_STK 		*StkPtr;
	uint32  		TaskStat;//任务的位置
	uint32		TaskInfo;//任务的通知
	uint32 		TaskPrio;//
	uint32 		TaskDly;//任务延时
	//线程被抢占时剩余的时间片，为零表示已经用完当前时间片
	uint32 		RemainTick;//
	OSMsg 		MailMsg;//邮箱消息
	OSMsg		SynMsg;//发送同步消息
	OSCTask*	SynTaskPtr;//发送同步消息给自己的线程
	OSCMsg		TaskMsg;//任务的消息队列
	OSCTask		*EventNextPtr,*EventPrevPtr ;//事件队列
	OSCTask		*RdyNextPtr,*RdyPrevPtr ;//就绪队列
	OSCTask		*TimeNextPtr,*TimePrevPtr ;//时间队列
	OSCTask		*PdNextPtr,*PdPrevPtr;//挂起队列
	OSCTask		*ThreadTickNextPtr,*ThreadTickPrevPtr;	
	OSCPendEvent   *EventList;//指向EventList数组的指针
	static 	OSCPendPd     PendPd;//挂起队列
	static   	int32		 LockCount;//
	static 	bool 		 IsRunning;
	//
	void 		SetRunTime();//设置任务运行最大时间计数器
	static 	void  SchedulGuardThread();//在中断解锁后查看OSCGuardThread是否需要调度
	static   void 	TaskUnlockMedi();
	bool			TaskResume();//从PendPd链表上 移除任务 ，并回复到就绪队列
	protected:
	
	bool			IsHasMsg;   //标识线程是否有收到消息
	static   void      Schedul();//任务调度函数
	bool			TaskPend();//挂起任务到  PendPd链表

	//用户线程构造函数
	OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop, uint32 Prio);
	//空闲任务构造函数
	OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop);
	
	public:	
	static   void    	TaskLock();
	static   void 	TaskUnlock();
	static 	bool	SynMsgPost(OSCTask*TaskP,OSMsg*PMsg);//同步消息发送，等消息被接受了，才能返回
	static 	bool	SynMsgWait(OSMsg*PMsg);//同步消息接收,如果没有消息将会挂起自身
	static  	bool	AsyMsgTelevise(OSMsg*PMsg);//消息广播
	bool			AsyMsgPost(OSMsg *TMsg);//发送消息,任务级函数
	bool			AsyMsgSend(OSMsg *TMsg);//发送消息,任务级函数
	bool		  	AsyMsgAccept(OSMsg *PMsg);//接收消息,任务级函数
	uint32		ChangePrio(uint32 Prio);//改变任务优先级	
	static   void  	Sleep(uint32 Dly);//
	static   void     SysInit();//系统初始化
	static   void 	StartUp();//启动多任务环境
	
};

#endif
//END
