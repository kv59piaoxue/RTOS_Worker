#ifndef __OSCPRIOMNG_HPP
#define __OSCPRIOMNG_HPP

	class  OSCPrio{
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
		friend  class OSCPrioMng;
	private:
		OSCPendRdy    PendRdy;
		uint32   MaxRunTime;
		uint32   MaxWaitTime;
		//Function
		OSCPrio(){}
		OSCPrio(uint32 RunTime,uint32 WaitTime){//�������к͵ȴ�ʱ�䣬��λ����
		MaxRunTime = DF_TICKS_PSEC*RunTime/1000;//RunTime  (ms)
		MaxWaitTime = DF_TICKS_PSEC*WaitTime/1000;//WaitTime (ms)
		}
	};
	
class  OSCPrioMng{
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
	friend  class   OSCTask;
	friend class     OSCTimer;
	friend class 	OSCGuardThread;
	friend class  	OSCEvent;
private:
	static     	OSCPrio   PrioArry[DF_MINPRIO+1];
	static     	uint32     HighPrio;
	OSCPrioMng(){}
	static 	void 	Init();
	static     	bool  	Find();//���һ���������񣬴�ŵ���ǰ����ָ����
	static     	bool    	Leave(OSCTask *TaskPtr);//ȡ������
	static     	bool         Wait(OSCTask *TaskPtr);//����ŵ����������еȴ�
public:	
//���º�����Ȼ���û�����������ò�Ҫ�ı�ϵͳԤ���趨��ֵ
	static     	uint32     GetRunTime(uint32  Prio){return PrioArry[Prio].MaxRunTime;}//
	static     	uint32     GetWaitTime(uint32  Prio){return PrioArry[Prio].MaxWaitTime;}//
	static     	uint32   	SetRunTime(uint32  Prio ,  uint32 RunTime);//�����������ʱ�䣬���ؾɵ��������ʱ��
	static     	uint32   	SetWaitTime(uint32  Prio , uint32 WaitTime);//
};
#endif
//END