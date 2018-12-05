#ifndef  __CDEVI2C0_HPP
#define  __CDEVI2C0_HPP
//����I2C���ߵĺ궨��
//I2CxCONSET
#define  	AA      	1<<2     	//Ӧ���־
#define 	SI	   	1<<3 	//I2C�жϱ�־
#define   	STO		1<<4	//Stop Flag
#define  	STA      	1<<5     	//��ʼ��־λ
#define 	I2EN	1<<6 	//I2C�ӿ�ʹ��
//I2CxCONCLR
#define  	AAC      	1<<2     	//Ӧ���־����
#define 	SIC	   	1<<3 	//I2C�жϱ�־����
#define  	STAC      1<<5     	//��ʼ��־����λ
#define 	I2ENC	1<<6 	//I2C�ӿڽ�ֹ

class CDevI2C0 : public OSCDevBuf{
	friend   void  Dri2103_I2C_IntServe();
private:
	virtual  	bool   IntStartUp();//�����ж�ģʽ����
	virtual 	bool	  IntShutDown();//�ر��ж�
	virtual 	bool   BufOpen();
	virtual 	bool   BufClose();
	virtual  	bool   BufIoctl(uint32 Cmd, const void *Arg);//���ú���
	virtual 	bool   IntRegister(int8  IntID);//�ж�ע�ắ��
public:
	static 	CDevI2C0  * DevID;//ָ���Լ���ָ�룬�����ж�ͨ��
	CDevI2C0(const  char *Name);
};



#endif
//END