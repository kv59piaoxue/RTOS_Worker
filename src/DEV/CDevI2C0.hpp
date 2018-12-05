#ifndef  __CDEVI2C0_HPP
#define  __CDEVI2C0_HPP
//关于I2C总线的宏定义
//I2CxCONSET
#define  	AA      	1<<2     	//应答标志
#define 	SI	   	1<<3 	//I2C中断标志
#define   	STO		1<<4	//Stop Flag
#define  	STA      	1<<5     	//起始标志位
#define 	I2EN	1<<6 	//I2C接口使能
//I2CxCONCLR
#define  	AAC      	1<<2     	//应答标志清零
#define 	SIC	   	1<<3 	//I2C中断标志清零
#define  	STAC      1<<5     	//起始标志清零位
#define 	I2ENC	1<<6 	//I2C接口禁止

class CDevI2C0 : public OSCDevBuf{
	friend   void  Dri2103_I2C_IntServe();
private:
	virtual  	bool   IntStartUp();//开启中断模式函数
	virtual 	bool	  IntShutDown();//关闭中断
	virtual 	bool   BufOpen();
	virtual 	bool   BufClose();
	virtual  	bool   BufIoctl(uint32 Cmd, const void *Arg);//设置函数
	virtual 	bool   IntRegister(int8  IntID);//中断注册函数
public:
	static 	CDevI2C0  * DevID;//指向自己的指针，用于中断通信
	CDevI2C0(const  char *Name);
};



#endif
//END