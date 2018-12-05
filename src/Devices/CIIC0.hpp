#ifndef  __CIIC0_HPP
#define __CIIC0_HPP

#define  	AA      	2     	//应答标志
#define 	SI	   	3 	//I2C中断标志
#define   	STO		4	//Stop Flag
#define  	STA      	5     	//起始标志位
#define 	I2EN	6 	//I2C接口使能
//I2CxCONCLR
#define  	AAC      	2     	//应答标志清零
#define 	SIC	   	3 	//I2C中断标志清零
#define  	STAC      5     	//起始标志清零位
#define 	I2ENC	6 	//I2C接口禁止


#define    IIC_SET_CLL      DEV_BASE + 1//设置低电平时间
#define    IIC_SET_CLH     DEV_BASE + 2 //设置高电平时间

class  CIIC0:public Cdevice{
friend  void  iic0_interrupter_fun();
private:
	OSCMutex  m_DataMutex;
	static CIIC0 *m_this;
protected:
	virtual  bool  Vopen();
	virtual  int32 Vwrite();
	virtual  int32 Vread();
	virtual  bool  Vioctl(uint32 cmd,void *arg = NULL);
	virtual  bool  Vclose();	
public:
	CIIC0():m_DataMutex(0){ m_this = this; }
};

#endif