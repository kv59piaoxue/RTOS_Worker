#ifndef  __CDEVUART0_HPP
#define __CDEVUART0_HPP

class  CDevUART0 : public OSCDevBuf{
	friend   void  Dri2103_Uart0_IntServe();
private:
	virtual  	bool   IntStartUp();//�����ж�ģʽ����
	virtual 	bool	  IntShutDown();//�ر��ж�
	virtual 	bool   BufOpen();
	virtual 	bool   BufClose();
	virtual  	bool   BufIoctl(uint32 Cmd, const void *Arg);//���ú���
	virtual 	bool   IntRegister(int8  IntID);//�ж�ע�ắ��
public:
	static  CDevUART0 *DevID;//�������жϷ���������ͨ��
	CDevUART0(const char *Name);
};
   
#endif
//END