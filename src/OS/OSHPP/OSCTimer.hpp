#ifndef  __OSCTIMER_HPP
#define __OSCTIMER_HPP

class OSCTimer{
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
	friend  class 	OSCGuardThread;
	friend class	OSCTask;
	friend class	OSCInt;
	friend class 	OSCPrioMng;
	friend class	OSCEvent;
private:
	static   	OSCPendTime       PendTime;	//��Ҫʱ�������������
	static   	OSCPendTime	PendTRdy;	//
	static 	OSCPendThreadTick 		PendThreadTick;//ÿ���̶߳�����ע��ʱ�����
	static   	uint32     CurTick;//��ǰ����ʣ��ʱ�䣬�������ʱ���ǵ�ǰ����	
	 OSCTimer(){}
	 static 	void   	Init();
	 static 	bool	  	ThreadTimeServer(); //Ϊÿ���߳��ṩʱ��Ƭ����
	 static   	bool    	RdyResume();//�ָ����������񵽾�������
	 static   	bool    	RdyLeave(OSCTask*TaskPtr);//���������뿪
	 static   	bool    	RdyWait(OSCTask *TaskPtr);//��������ȴ���ʱ��Ƭ�����Ҫ�ȴ�
	 static   	void    	OnTime();//�ж���Ӧ�������ҵ���ʱ��ɵ����񲢻ָ�
	 static   	bool    	TimeLeave(OSCTask*TaskPtr);//��ָ�������ó�ʱ����� 
	 static   	bool    	TimeWait(OSCTask *TaskPtr ,uint32  Dly);//��ָ����ŵ�ʱ����еȴ�
	 static 	bool		StartTimer();//������ʱ��
public:
	 //��ʱ���жϵ��ú�������ʱ������̷߳�����Ϣ������ʱ������߳̽���ʱ�����
	 static 	bool	    	OnTimeInt();//�жϵ���
	 static   	bool		TimeServerRegister(OSCThread *ThreadPtr,uint32 Tick);//ע���߳�ʱ�ӷ���
	 static   	bool		TimeServerUnregister(OSCThread *ThreadPtr);//ע���߳�ʱ�����	
};								

#endif	
//END