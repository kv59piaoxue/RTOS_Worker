#include "config.hpp"

CDevSPI0  * CDevSPI0::DevID = NULL;
void  Dri2103_SPI0_IntServe();

void  Dri2103_SPI0_IntServe(){
	VICVectAddr = 0;
	VICIntEnClr = 1 << 10;//�豸����
	OSCInt::IntEnable();
	char  Ch;
	//switch()
	//{
	//}		
	OSCInt::IntDisable();
	VICIntEnable =  1 << 9; 
	SPINT  = 0x01;//����жϱ�־
	OSCInt::IntExitSwap();	 
}
CDevSPI0::CDevSPI0(const  char *Name){
	 	DevID = this;
	 	SetBusName(Name);
	 	OSCDevMng::DevID -> BusRegister(this);
}
bool   CDevSPI0::IntStartUp(){//�����ж�ģʽ����

	VICIntEnable = 1 << 10; 		//ʹ��I2C�ж�
	return  TRUE;
}
bool	  CDevSPI0::IntShutDown(){//�ر��ж�
		VICIntEnClr =  1 << 10;
 		return  TRUE;
}
bool   CDevSPI0::BufOpen(){
		if(CurDevice -> Open()){//�������ϵ��豸
			//�����߹���
			CurDevice -> SetPin(PINSEL0 & (0xFF << 8));//��������״̬
			PINSEL0	 = ((PINSEL0&(~(0xFF << 8)))|(0x55 << 8));//���������óɴ��ڹ���
			//�������߲�����,����Ϊ�����Ĳ�����
			SPCCR = Fpclk/CurDevice -> GetBaud();
			return TRUE;
		}
	 	return  FALSE;
}
bool   CDevSPI0::BufClose(){
		PINSEL0  =  0;//��ԭ����
		CurDevice -> Close();
 		return  TRUE;
}
bool   CDevSPI0::BufIoctl(uint32 Cmd, const void *Arg){//���ú���
	
	Cmd = Cmd;
	Arg = Arg;
	return TRUE;
}
bool   CDevSPI0::IntRegister(int8  IntID){//�ж�ע�ắ��
	OSCInt::IntRegister(Dri2103_SPI0_IntServe,IntID,9);
 	return  TRUE;
}

//END