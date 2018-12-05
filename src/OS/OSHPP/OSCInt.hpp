
#ifndef  __OSCINT_HPP
#define __OSCINT_HPP

class OSCInt{
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
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
    	static  	int8   	GetIntVector();//��ȡһ����û�б�ע����ж�������
  	static  	bool		PutIntVector(int8 Vector);//�жϺ�ע��
  	static  	bool		IntRegister(void(*IntServe)(void),int8 Vector,int8 VICChannel);//�жϺ���ע�ắ��
	static   	void  	IntEnterInt();
	static 	void 	IntEnterNoint();
	static  	void  	IntExitSwap();
	static 	void 	IntExitMedi();
  };
#endif
//END
