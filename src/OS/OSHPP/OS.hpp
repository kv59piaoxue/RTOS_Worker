
#ifndef  __OS_HPP
#define __OS_HPP
         //Worker1.0
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef  uint32  OS_STK  ;
typedef  uint32   WPARAM;
typedef  uint32   LPARAM;

#include   "OSdefine.hpp"

#include  "OSCPend.hpp"
#include  "OSCMsg.hpp"
#include  "OSCTask.hpp"
#include "OSCThread.hpp"
#include 	"OSCTimer.hpp"
#include   "OSCInt.hpp"
#include  "OSCPrioMng.hpp"
#include  "OSCEvent.hpp"
#include "OSCMutex.hpp"
#include "OSCSem.hpp"
#include  "OSCMail.hpp"
#include "OSCGuardThread.hpp"
#include  "OSCIdlThread.hpp"
#include  "UsrThread.hpp"

//请用户不要修改
extern    	int8  OSIntNesting;
extern   	int8  OSEnterSum;
extern   	OSCTask  *OSTaskCurPtr;
extern  	OSCTask   *OSTaskNextPtr;

#endif 
//END 