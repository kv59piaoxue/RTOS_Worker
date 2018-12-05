//
#include "config.hpp"
#include "CTest.hpp"
	//Worker1.0
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
CTest * CTest::ThreadID = NULL;  
 CTest::CTest(uint32 prio):OSCThread(prio),m_Led4(10,0,OUT),m_Led5(24,0,OUT),m_Led2(12,0,OUT)
{
 	ThreadID = this;//
 }
 bool	CTest::ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM  lParam)
 {
 	static  uint32   Cnt = 0;
 	OSMsg   Msg;
 	//char  buf[] = "1+2+3+4+5+6+7+8+9+";
 	//CSPI0   spi0;
 	//CIIC0   iic0;
 	//CUART0  uart0;
  	switch(mParam)
 	{	
 		case TM_CREATE:
 				 OSCMutex::MutexID -> MutexGetSwap(0);
 				//OSCMutex::MutexID -> MutexGetSwap(100);
 				//OSCSem::SemID -> SemGetSwap(0);
 				//OSCSem::SemID -> SemGetSwap(0);
 				//OSCSem::SemID -> SemGetSwap(0);
 				//OSCSem::SemID -> SemGetSwap(0);
 				//OSCMail::MailID -> MsgGetSwap(0);
 				//OSCMail::MailID -> MsgGetSwap(&Msg,0);
 				//if(Msg.MsgID == 9){ //IOSET = LED1;
 				//	CPin_2103  led1(25,0,1);
 				//	led1.Set();
 				//}

 				//iic0.open();
 				//iic0.write(buf,sizeof(buf));
 				//iic0.close();
 				//uart0.open();
 				//uart0.write(buf,sizeof(buf));
 				//uart0.write(buf,sizeof(buf));
 				//uart0.close();
 				/*uart0.open();
 				int j ;
 				 j = 30;
 				while(j--){
 					uart0.write(buf,sizeof(buf));
				}
 				uart0.close();*/
 				/*spi0.open();
 				spi0.ioctl(SPI_MASTER);
 				spi0.write(buf,sizeof(buf));
 				spi0.ioctl(SPI_SLAVE);
 				spi0.close();*/
 				 OSCTimer::TimeServerRegister(this,9);
 				break;
 		case TM_THREADONTIME:
 				if(Cnt % 2){ 
 					 m_Led2.Clear();   
 				}else {
 					m_Led2.Set();	
 				}
 				 
 				Cnt ++;
 				//OSCMutex::MutexID -> MutexGetSwap(0);
 				//SynMsgWait(&Msg);
 				//if(Msg.MsgID == 1000)  IOSET  = LED6;
 				break;
		case ONLED4: 	m_Led4.Clear(); break;
		case OFFLED4: 	m_Led4.Set(); break;
		case ONLED5: 	m_Led5.Clear(); break;
		case OFFLED5: 	m_Led5.Set(); break;
		default: 
			return 	OSCThread::ThreadProc(STaskP,mParam,wParam,lParam);
 	}
 	return  TRUE;
 }
 //END 
