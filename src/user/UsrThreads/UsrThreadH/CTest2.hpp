#ifndef  __CTEST2_HPP
#define __CTEST2_HPP
#include "devices.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
class    CTest2 :public OSCThread{
private:
	CPin *m_Led8;
	Cdevice *uart0;
 protected:
 	virtual 	bool   ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM  wParam,LPARAM  lParam);
 public:
 	CTest2(Cdevice *dev,CPin *pin,uint32 prio = 3);	
 	static  CTest2 *ThreadID;
 };

#endif