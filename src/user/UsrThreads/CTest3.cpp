#include "config.hpp"
#include "CTest3.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
CTest3 *CTest3::ThreadID = NULL;
  CTest3::CTest3(Cdevice *dev ,CPin *pin,uint32 prio):OSCThread(prio)
 {
 	ThreadID = this;//
 	m_Led8 = pin;
 	uart0 = dev;
 }

  bool	CTest3::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM lParam)
 {
 	static  uint32 Cnt = 0;
 	OSMsg   Msg;
  	//CPin_2103   Led6(9,0,OUT);
  	char  buf[] = "1+2+3+4+5+6+7+8+9+";
  	//CUART0  uart0;
  	switch(mParam)
 	{
 	case	TM_CREATE:
       			//Msg.Msg = TM_CREATE;
       			//CTest::ThreadID -> MsgPost(&Msg);
       			//MsgSend(&Msg);
 			//OSCMutex::MutexID -> MutexPutMedi();
 			//OSCMutex::MutexID -> MutexPutSwap();
 			//Sleep(10);
 			//OSCMutex::MutexID -> MutexGetSwap(30);
 			//OSCMutex::MutexID -> MutexPutMedi();
 			//OSCMutex::MutexID -> MutexPutSwap();
			//OSCGuardThread::ThreadID -> AsyMsgSend(&AsyMsg);
			//OSCSem::SemID -> SemGetSwap(0);
			//OSCMail::MailID -> MsgGetSwap(&Msg,0);
			//if(Msg.MsgID == 9) IOSET = LED6;
			uart0 -> open();
 			int j ;
 			 j = 10;
 			while(j--){
 				uart0 -> write(buf,sizeof(buf));
			}
 			uart0 -> close();
			OSCTimer::TimeServerRegister(this,2);
       		break;
       	case TM_THREADONTIME:
       		//OSCMutex::MutexID -> MutexPutMedi();
       		m_Led8 -> Clear();
       		if(Cnt % 2){     		
       			Msg.MsgID = ONLED5;
       			CTest::ThreadID -> AsyMsgPost(&Msg);
       			//Led6.Clear();
 		}else {
  			Msg.MsgID = OFFLED5;
       			CTest::ThreadID -> AsyMsgPost(&Msg);
       			//Led6.Set();
 		}
 			Cnt ++;
 		 Msg.MsgID = 1000;
 		//MsgTelevise(&Msg);//
 		//SynMsgPost(CTest::ThreadID,&Msg);
       		break;
       	default:
       		return 	OSCThread::ThreadProc(STaskP,mParam,wParam,lParam);		
 	}
 	return  TRUE;
 }
 
 //END 
