#ifndef  __OSCMUYEX_HPP
#define __OSCMUYEX_HPP
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
class  OSCMutex :public  OSCEvent{//����ʹ�õ��ź���
	private:
	int32   	Cnt;//������������
	OSCTask  *Thread;//��ǰռ�û��������߳�
	uint32   	ThreadPrio;//ռ�û��������̵߳����ȼ�
	public :
	static 	OSCMutex*  MutexID;//����ʹ�ñ���
	OSCMutex(int32  Mutex):OSCEvent(){//
		Cnt = Mutex ;
		Thread = NULL;
		ThreadPrio = 0;
		MutexID = this;
	}
		//�޵ȴ�
		bool	 MutexGetMedi();
		bool  MutexPutMedi();
		//�еȴ�
		bool  MutexGetSwap(uint32 Dly);
		bool  MutexPutSwap();
};
#endif
//END