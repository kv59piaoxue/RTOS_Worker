#ifndef __OSCIdlThread_HPP
#define __OSCIdlThread_HPP
  	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
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
