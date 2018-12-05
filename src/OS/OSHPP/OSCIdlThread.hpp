#ifndef __OSCIdlThread_HPP
#define __OSCIdlThread_HPP
  	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
class    OSCIdlThread:public OSCThread{
	private:
	static      	void  OnThreadCreate();
	//virtual	void ThreadMain();
 	virtual	bool	  ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM lParam);
 	public:
 	OSCIdlThread();
 	static   OSCIdlThread *ThreadID;
 };
 
#endif
//END
