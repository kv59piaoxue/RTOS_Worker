#ifndef  __OSCTASK_HPP
#define __OSCTASK_HPP
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
class OSCTask{
//�߳̿��ƿ�PCB
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
	uint32  		TaskStat;//�����λ��
	uint32		TaskInfo;//�����֪ͨ
	uint32 		TaskPrio;//
	uint32 		TaskDly;//������ʱ
	//�̱߳���ռʱʣ���ʱ��Ƭ��Ϊ���ʾ�Ѿ����굱ǰʱ��Ƭ
	uint32 		RemainTick;//
	OSMsg 		MailMsg;//������Ϣ
	OSMsg		SynMsg;//����ͬ����Ϣ
	OSCTask*	SynTaskPtr;//����ͬ����Ϣ���Լ����߳�
	OSCMsg		TaskMsg;//�������Ϣ����
	OSCTask		*EventNextPtr,*EventPrevPtr ;//�¼�����
	OSCTask		*RdyNextPtr,*RdyPrevPtr ;//��������
	OSCTask		*TimeNextPtr,*TimePrevPtr ;//ʱ�����
	OSCTask		*PdNextPtr,*PdPrevPtr;//�������
	OSCTask		*ThreadTickNextPtr,*ThreadTickPrevPtr;	
	OSCPendEvent   *EventList;//ָ��EventList�����ָ��
	static 	OSCPendPd     PendPd;//�������
	static   	int32		 LockCount;//
	static 	bool 		 IsRunning;
	//
	void 		SetRunTime();//���������������ʱ�������
	static 	void  SchedulGuardThread();//���жϽ�����鿴OSCGuardThread�Ƿ���Ҫ����
	static   void 	TaskUnlockMedi();
	bool			TaskResume();//��PendPd������ �Ƴ����� �����ظ�����������
	protected:
	
	bool			IsHasMsg;   //��ʶ�߳��Ƿ����յ���Ϣ
	static   void      Schedul();//������Ⱥ���
	bool			TaskPend();//��������  PendPd����

	//�û��̹߳��캯��
	OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop, uint32 Prio);
	//���������캯��
	OSCTask(void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop);
	
	public:	
	static   void    	TaskLock();
	static   void 	TaskUnlock();
	static 	bool	SynMsgPost(OSCTask*TaskP,OSMsg*PMsg);//ͬ����Ϣ���ͣ�����Ϣ�������ˣ����ܷ���
	static 	bool	SynMsgWait(OSMsg*PMsg);//ͬ����Ϣ����,���û����Ϣ�����������
	static  	bool	AsyMsgTelevise(OSMsg*PMsg);//��Ϣ�㲥
	bool			AsyMsgPost(OSMsg *TMsg);//������Ϣ,���񼶺���
	bool			AsyMsgSend(OSMsg *TMsg);//������Ϣ,���񼶺���
	bool		  	AsyMsgAccept(OSMsg *PMsg);//������Ϣ,���񼶺���
	uint32		ChangePrio(uint32 Prio);//�ı��������ȼ�	
	static   void  	Sleep(uint32 Dly);//
	static   void     SysInit();//ϵͳ��ʼ��
	static   void 	StartUp();//���������񻷾�
	
};

#endif
//END
