
#include "config.hpp"   
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��

   	
extern "C"  int main (void)
{     
    // �û�����������Լ��ĳ������ 
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


