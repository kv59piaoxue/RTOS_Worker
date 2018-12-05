#ifndef  __CDEVSPI0_HPP
#define  __CDEVSPI0_HPP
class CDevSPI0 : public OSCDevBuf{
	friend   void  Dri2103_SPI0_IntServe();
private:
	virtual  	bool   IntStartUp();//�����ж�ģʽ����
	virtual 	bool	  IntShutDown();//�ر��ж�
	virtual 	bool   BufOpen();
	virtual 	bool   BufClose();
	virtual  	bool   BufIoctl(uint32 Cmd, const void *Arg);//���ú���
	virtual 	bool   IntRegister(int8  IntID);//�ж�ע�ắ��
public:
	static 	CDevSPI0  * DevID;//ָ���Լ���ָ�룬�����ж�ͨ��
	CDevSPI0(const  char *Name);
};

#endif
//END