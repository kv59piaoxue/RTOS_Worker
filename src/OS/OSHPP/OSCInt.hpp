
#ifndef  __OSCINT_HPP
#define __OSCINT_HPP

class OSCInt{
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
	friend class  OSCTask;
	friend class OSCIntMsg;
	friend class OSCTimer;
	friend class OSCPrioMng;
private:
	static  	uint32  IntVector;
	static  const  uint32  IntTab[16];
	OSCInt(){}	
	static  	void  	Init(){ IntVector = 0;}
	static  	void 	IntLock(){ OS_ENTER_CRITICAL();}
	static  	void 	IntUnlock(){ OS_EXIT_CRITICAL();}
  public:
  	static  	void 	IntEnable(){  OS_EN_IRQ(); }
	static  	void		IntDisable(){  OS_DIS_IRQ(); }
    	static  	int8   	GetIntVector();//获取一个还没有被注册的中断向量号
  	static  	bool		PutIntVector(int8 Vector);//中断号注销
  	static  	bool		IntRegister(void(*IntServe)(void),int8 Vector,int8 VICChannel);//中断函数注册函数
	static   	void  	IntEnterInt();
	static 	void 	IntEnterNoint();
	static  	void  	IntExitSwap();
	static 	void 	IntExitMedi();
  };
#endif
//END
