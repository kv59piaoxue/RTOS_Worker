

#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年

CTest2 *CTest2::ThreadID = NULL;
 CTest2::CTest2(Cdevice *dev ,CPin *pin,uint32 prio ):OSCThread(prio)
 {
 	ThreadID = this;//
 	m_Led8 = pin;
 	uart0 = dev;
 }
 
  bool	CTest2::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM  lParam)
 {
 	OSMsg  Msg;
 	static  uint32   Cnt = 0;
 	//CPin_2103   Led7(8,0,OUT);
  	char  buf[] = "ABCDEFG";
  	switch(mParam)
 	{
 		case TM_CREATE:			
       				//Msg.Msg = TM_CREATE;
       				//MsgSend(&Msg);
       				Sleep(10);
 				//CTest::ThreadID -> MsgPost(&Msg);
 				//OSCMutex::MutexID -> MutexGetSwap(0);
 				//OSCMutex::MutexID -> MutexPutSwap();
 				//OSCMutex::MutexID -> MutexPutSwap();
 				OSCMutex::MutexID -> MutexPutMedi();
 				//OSCSem::SemID -> SemPutMedi();
 				//OSCSem::SemID -> SemPutMedi();
 				//OSCSem::SemID -> SemPutSwap();
 				//OSCSem::SemID -> SemPutSwap();
 				//Msg.MsgID = 9;
 				//OSCMail::MailID -> MsgPutSwap(&Msg);
 				//OSCMail::MailID -> MsgPutMedi(&Msg);
 				//OSCMail::MailID -> MsgTeleviseSwap(&Msg);
 				//OSCMail::MailID -> MsgTeleviseMedi(&Msg);
 				uart0 -> open();
 				int j ;
 				j = 20;
 				while(j--){
 					uart0 -> write(buf,sizeof(buf));
				}
 				uart0 -> close();
 				OSCTimer::TimeServerRegister(this,10);
       				break;
       		case  TM_THREADONTIME:
       				m_Led8 -> Clear();
       				if(Cnt % 2){     		
       					Msg.MsgID = ONLED4;
       					CTest::ThreadID -> AsyMsgSend(&Msg);
       					//Led7.Clear();
 				}else {
 					Msg.MsgID = OFFLED4;
       					CTest::ThreadID -> AsyMsgSend(&Msg);
       					//Led7.Set();
 				}
 				Cnt ++;
 				
       				break;
       		default:   
       		 	return 	OSCThread::ThreadProc(STaskP,mParam,wParam,lParam);
       	}
 	return  TRUE;
 }
 //END 