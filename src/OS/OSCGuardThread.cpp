#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
OSCGuardThread  *OSCGuardThread::ThreadID = NULL;

OSCGuardThread::OSCGuardThread():OSCThread(0)//
{//
	ThreadID = this;//
}

//�����̹߳��̺��������߳���Ϣ
bool	  OSCGuardThread::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM  lParam)
 {
 //����������Խ��жϺ�����������Ϣת�����û��̣߳����������жϺ���ִ�д�������
 //���ϵͳ�ж���Ӧ����,��������̴߳������ϢSTaskP�������this ����������Ϣ����
 //�����Լ��ģ���ֱ��ת����Ϣ
 	if(this != STaskP){//����Ҫת������Ϣ��ת������
 		OSMsg    Msg(NULL,mParam,wParam,lParam);
 		STaskP -> AsyMsgSend(&Msg);//ת���жϷ��͹�������Ϣ
 		return TRUE;
 	}
 	//���洦�����Լ�����Ϣ
  	switch(mParam)
 	{	
 		case TM_CREATE: 
 		//��Ӧ�̴߳�����Ϣ�����Լ��Ӿ���������ȡ���������߳����⴦��
 				OSCPrioMng::Leave(this);//�ػ����̲��ھ���������
 		 		break;
		case TM_TIMERINT: 
				//��Ӧϵͳ��ʱ���жϷ�������Ϣ����ÿ���߳��ṩʱ�����
				OSCTimer::OnTime(); 
				//Ϊÿһ���߳��ṩʱ�����ֻҪ�߳�
				//�趨��ʱ��Ƭ����ͻ����Ӧ�̷߳�����Ϣ
				OSCTimer::ThreadTimeServer();
				if(NULL != wParam){
					//������ʱ��Ƭ���������
					if(this == (OSCTask*)wParam)  break;
					OSCPrioMng::Leave((OSCTask*)wParam);
					OSCTimer::RdyWait((OSCTask*)wParam);
				}
				break;
		case  TM_MUTEXPUTMEDI:
				((OSCMutex*)wParam) -> MutexPutMedi();
				break;  	
		default: 	//�����������Ϣ����OSCThread����
		 		return OSCThread::ThreadProc(STaskP,mParam,wParam,lParam); 
 	}
 	return  TRUE;
 }
 
 
bool		OSCGuardThread::IntMsgSendToTask(OSMsg *TMsg){
//��������������л����жϼ�����,�жϺ�������Ϣ���͸�����
//�жϺ�������Ϣ���͸�ϵͳ���ػ��̣߳�����Ҫϵͳ�߳̽���ĳ�ַ���
//Ҳ������Ҫ����Ϊת����Ϣ,�������Ա����жϺ���ֱ�����û��߳̽���ͨ��
//�����ٽ���Դ��Ӳ���������ϵͳ�ж���Ӧ����
	if(IntMsg.Add(TMsg)){
		IsHasMsg = TRUE;
		return  TRUE;
	}
	return FALSE ;
}
bool		OSCGuardThread::IntMsgAcceptFromInt(OSMsg *PMsg){//��������������л�,�жϼ�����,����ȡ�жϷ�������Ϣ
//ϵͳ�ػ��߳�ȡ�жϺ������͹�������Ϣ
//ȡ����Ϣ���ᱻ�ɷ����̹߳��̺������д���
  	if(IntMsg.Delete(PMsg)){//�����Ϣ���в�Ϊ��
  		return  TRUE;
	}
  	return FALSE;
}

//�����߳�������
void  	OSCGuardThread::ThreadMain()
{
//���ϵͳ�ػ��߳��������߳����ThreadMain����ʵ���˲�ͬ����Ϣѭ��
//����߳���ʵ�ְ��жϺ�������Ϣ���ﵽ�û��̣߳����Ҫ�����ж�ͨ�ŵ���Ϣ����
	OSMsg  Msg;
	while(1){//�߳���Ϣѭ������
		while(!IntMsgAcceptFromInt(&Msg) && !AsyMsgAccept(&Msg)){
		//��Ϣ�ǿգ��л����û��߳�ִ��
			IsHasMsg = FALSE;//�߳�û����Ϣ��
			Schedul();
		}
		//ֻ������ThreadProc����������Ϣ����
		ThreadProc(Msg.STaskPtr, Msg.MsgID, Msg.wParam, Msg.lParam);
	}
}
//END
