#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
OSCIdlThread	*OSCIdlThread::ThreadID = NULL;
 OSCIdlThread::OSCIdlThread():OSCThread()
 {
 	ThreadID = this;//��ʼ������ָ��
 }
   bool	OSCIdlThread::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM  lParam)
 {
 	OSMsg  Msg(STaskP,TM_CREATE);
  	switch(mParam)
 	{
 		case TM_CREATE:  
 				 OnThreadCreate();
 				 AsyMsgSend(&Msg); 
 				 break;
 		default:
 				return  OSCThread::ThreadProc(STaskP, mParam, wParam, lParam);
 	}
 	return  TRUE;
 }
 void  OSCIdlThread::OnThreadCreate(){
    	uint32  Count;   
    	for(Count = 0; Count < 4000;Count++);
	IOSET = LED9;
	 for(Count = 0; Count < 4000;Count++);
 	IOCLR  = LED9;
 }
/* 
  void  OSCIdlThread::ThreadMain(){
      	while(1){
      		uint32  Count;   
    		for(Count = 0; Count < 4000;Count++);
		IOSET = LED9;
		for(Count = 0; Count < 4000;Count++);
 		IOCLR  = LED9;
 	}
  }
  */
 //END 
