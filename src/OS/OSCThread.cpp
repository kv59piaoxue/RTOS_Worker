#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
 OSCThread::OSCThread(uint32 Prio):OSCTask(Task,this,&Stack[DF_ThreadStackDeep -1],Prio)
 {//�û��̵߳��õĹ��캯��
 	 ThreadTickCnt = 0;
 	ThreadTick = 0;
 }
 
 OSCThread::OSCThread():OSCTask(Task,this,&Stack[DF_ThreadStackDeep -1])
 {//����������ã����������ܵ���
  	ThreadTickCnt = 0;
 	ThreadTick = 0;
 }
void  	OSCThread::ThreadMain(){//virtual
//�������Ϊ�߳����������û�����ͨ�������������
//�����������������أ��߳̽�û����Ϣѭ�����ܣ�
		OSMsg  Msg;
		while(1){//�߳���Ϣѭ������
			while(!AsyMsgAccept(&Msg)){//����������Ϣ���ǿգ���������ȴ���Ϣ
				IsHasMsg = FALSE;
				TaskPend();//û����Ϣ��Ҫ����ͽ������״̬
				Schedul();//������������
			}
				//ȡ����Ϣ�ɷ����̹߳��̺������û�
				//ֻ�������麯��ThreadProc�����߳���Ϣ�����еļ���
			ThreadProc(Msg.STaskPtr, Msg.MsgID, Msg.wParam, Msg.lParam);
		}
}

  bool	  OSCThread::ThreadTimer(){
  	OSMsg  Msg(NULL,TM_THREADONTIME);
         return  	AsyMsgSend(&Msg);
  }
bool	OSCThread::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM lParam)
 {//virtual
 //�̹߳��̺������û���Ҫ����������������߳���Ϣ�����е���Ϣ
 //�����û����������Ϣ�����Խ������̵߳��̹߳��̺���������
 	STaskP = STaskP;
 	wParam = wParam;
 	lParam = lParam;
  	switch(mParam)
 	{
 	
 	}
 	return  TRUE;
 }
//END
