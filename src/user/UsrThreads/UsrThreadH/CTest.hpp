
#ifndef   __CTEST_HPP
#define 	__CTEST_HPP
#include "devices.hpp"
class    CTest :public OSCThread{
private:
	CPin_2103  m_Led4;
	CPin_2103  m_Led5;
	CPin_2103  m_Led2;
 protected:
 	virtual 	bool   ThreadProc(OSCTask*STaskP,uint32 mParam,WPARAM wParam,LPARAM lParam);
 public:
 	CTest(uint32 prio = 0);
 	static   CTest *ThreadID;  
 };
 #endif