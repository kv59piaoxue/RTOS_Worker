#ifndef  __CDEVSPI0_HPP
#define  __CDEVSPI0_HPP
class CDevSPI0 : public OSCDevBuf{
	friend   void  Dri2103_SPI0_IntServe();
private:
	virtual  	bool   IntStartUp();//开启中断模式函数
	virtual 	bool	  IntShutDown();//关闭中断
	virtual 	bool   BufOpen();
	virtual 	bool   BufClose();
	virtual  	bool   BufIoctl(uint32 Cmd, const void *Arg);//设置函数
	virtual 	bool   IntRegister(int8  IntID);//中断注册函数
public:
	static 	CDevSPI0  * DevID;//指向自己的指针，用于中断通信
	CDevSPI0(const  char *Name);
};

#endif
//END