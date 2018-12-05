
#include "config.hpp"   
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年

   	
extern "C"  int main (void)
{     
    // 用户在下面添加自己的程序代码 
  	OSCTask::SysInit();
  	OSCGuardThread   GuradThread;
   	OSCIdlThread  IdlTask;
   	CPin_2103      Led8(7,0,OUT);
   	CUART0   uart0;
   	CTest		TestApp(0);
   	CTest2		Test2App(&uart0,&Led8,3);
   	CTest3		Test3App(&uart0,&Led8,2);
	OSCMutex   	Mutex(0);
	//OSCSem      Sem(2);	
	//OSCMail     Mail;
   	OSCTask::StartUp();
   	while(1);
   	return 0;
}


