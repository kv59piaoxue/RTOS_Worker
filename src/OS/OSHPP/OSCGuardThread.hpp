#ifndef  __OSCGUARD_THREAD_HPP
#define   __OSCGUARD_THREAD_HPP

	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
class  OSCGuardThread:public OSCThread{
//����߳��������ж����û��̵߳�������ת���жϺ�����
//���û��̵߳���Ϣ��ȷ��ϵͳ�ж���Ӧʱ��Ϊ����
//���̻߳���ϵͳ��ʱ������̡߳�����̲߳��ھ��������У�
//����ֻҪ����߳��յ���Ϣ����ϵͳû����������
//�ͻ��һʱ��õ�ִ��
	 private:
	 //��Ϣ
	OSCIntMsg		IntMsg;//�жϺ�������Ϣ���͸�����	
	 bool	IntMsgAcceptFromInt(OSMsg *PMsg);//�����жϺ�����������Ϣ
	//�����߳���������ʵ�ֲ�ͬ����Ϣѭ������Ϊ����̵߳���Ϣ��Դ���û��̵߳���Ϣ��Դ��һ��
	 virtual   	void  ThreadMain();
	 virtual	bool	  ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM  lParam);	 
	 public:
	 static	OSCGuardThread  * ThreadID;//��������ͨ�ŵ�ָ�룬ֻ�ܳ�ʼ��һ��
	 OSCGuardThread();
	 //��������������л����жϼ�����,�жϺ�������Ϣ���͸����� 
	 bool	IntMsgSendToTask(OSMsg *PMsg);
};
#endif
//END 