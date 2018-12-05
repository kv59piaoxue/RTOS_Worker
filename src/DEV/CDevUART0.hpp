#ifndef  __CDEVUART0_HPP
#define __CDEVUART0_HPP

class  CDevUART0 : public OSCDevBuf{
	friend   void  Dri2103_Uart0_IntServe();
private:
	virtual  	bool   IntStartUp();//开启中断模式函数
	virtual 	bool	  IntShutDown();//关闭中断
	virtual 	bool   BufOpen();
	virtual 	bool   BufClose();
	virtual  	bool   BufIoctl(uint32 Cmd, const void *Arg);//设置函数
	virtual 	bool   IntRegister(int8  IntID);//中断注册函数
public:
	static  CDevUART0 *DevID;//用于与中断服务函数进行通信
	CDevUART0(const char *Name);
};
   
#endif
//END