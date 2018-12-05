#ifndef  __CIIC0_HPP
#define __CIIC0_HPP

#define  	AA      	2     	//Ӧ���־
#define 	SI	   	3 	//I2C�жϱ�־
#define   	STO		4	//Stop Flag
#define  	STA      	5     	//��ʼ��־λ
#define 	I2EN	6 	//I2C�ӿ�ʹ��
//I2CxCONCLR
#define  	AAC      	2     	//Ӧ���־����
#define 	SIC	   	3 	//I2C�жϱ�־����
#define  	STAC      5     	//��ʼ��־����λ
#define 	I2ENC	6 	//I2C�ӿڽ�ֹ


#define    IIC_SET_CLL      DEV_BASE + 1//���õ͵�ƽʱ��
#define    IIC_SET_CLH     DEV_BASE + 2 //���øߵ�ƽʱ��

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