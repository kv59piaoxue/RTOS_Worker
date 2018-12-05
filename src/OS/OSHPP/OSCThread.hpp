#ifndef  __OSCTHREAD_HPP
#define __OSCTHREAD_HPP
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��

class  OSCThread:public  OSCTask{
	friend  class  OSCTimer;
	friend class  OSCIdlThread;//����Ϊ��Ԫ��ֹ�������������OSCThread();���������캯��
	 private:
	 OSCThread();//���������캯�����ܱ�OSCIdlThread����
 	OS_STK   Stack[DF_ThreadStackDeep];
 	 static   void Task(void*Pdata){//
 		((OSCThread*)Pdata) -> ThreadMain();
 	}
 	uint32 	ThreadTickCnt;//ʱ����������
	uint32 	ThreadTick;//
 	protected:
 	OSCThread(uint32 Prio);//�����Դ����߳��࣬�û��̹߳��캯��
 	virtual    bool	  ThreadTimer();//�߳��趨��ʱ��Tick���ͻ�����������
 	virtual   	void   ThreadMain();//
 	virtual	bool	  ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM lParam);
 };

#endif
//END