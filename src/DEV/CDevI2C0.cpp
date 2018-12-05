#include "config.hpp"

CDevI2C0  * CDevI2C0::DevID = NULL;
void  Dri2103_I2C_IntServe();

void  Dri2103_I2C_IntServe(){
	VICVectAddr = 0;
	VICIntEnClr = 1 << 9;//�豸����
	OSCInt::IntEnable();
	char  Ch;
	switch(I2STAT & 0xF8)
	{
		case 	  0x08://�Ѿ�������ʼ����������ģʽ�����ͺͽ��ܶ��п���
		case   0x10://�Ѿ������ظ���ʼ����������ģʽ�����ͺͽ��ܶ��п���
	 		if(CDevI2C0::DevID -> DevStat & Dev_Stat_RO){//ָ����ַ������
			//�������д���ַ�Ͷ��ź�
				I2DAT = CDevI2C0::DevID -> CurDevice -> GetAddress()| 0x01; 
			}else  if(CDevI2C0::DevID -> DevStat & Dev_Stat_WO){
			//���ʹӻ���ַ��д�ź�
				I2DAT = CDevI2C0::DevID -> CurDevice -> GetAddress() & 0xFE;
			}
			I2CONCLR = SIC|STAC;//����жϱ�־�Ϳ�ʼ��־
	 		break;
	 	case   0x18://�Ѿ�����SLA + W�ͽ���ACK
	 		//break;
	 	case   0x20://�Ѿ�����SLA + W�ͽ��շ�ACK
	 	 	//IOSET  = LED7;
	 		//break;
	 	case   0x28://�Ѿ�����I2DAT�е����ݺͽ���ACK
	 		//IOSET = LED6;
	 		//break;
	 	case   0x30://�Ѿ�����I2DAT�е����ݺͽ��շ�ACK
	 		if(CDevI2C0::DevID -> IntGetTxBufChar(&Ch)){//����Ҫ��������
	 			I2DAT = Ch;//д�����ݲ�����
	 			I2CONCLR = SIC|STAC;
	 		}else{
	 		 	I2CONCLR = SIC|STAC;
	 		 	I2CONSET = STO;//����ֹͣ����
	 		 }
	 		 break;
	 	case   0x38://��SLA+R/W���������ֽ��ж�ʧ�ٲ�
	 		I2CONCLR = SIC;
	 		I2CONSET = STA;//������ʼ�����������߿���ʱ���¿�ʼ��
	 		break;
	 	case	  0x40://�Ѿ�����SLA+R�ͽ���ACK
	 		if(CDevI2C0::DevID -> RecApplySize <= 1){
	 			I2CONCLR = SIC|STAC|AAC;
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = AA;
	 		}
	 		break;
	 	case	  0x48://�Ѿ�����SLA+R�ͽ��շ�ACK
	 		if(CDevI2C0::DevID -> RecApplySize >= 1){
	 			I2CONCLR = SIC;
	 			I2CONSET = STA;//������ʼ����
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = STO;
	 		}
	 		break;
	 	case	  0x50://�Ѿ��������ݲ�����ACK
	 		Ch = I2DAT;
	 		CDevI2C0::DevID -> IntPutRecBufChar(Ch);
	 		if(CDevI2C0::DevID -> RecApplySize <= 1){
	 			I2CONCLR = SIC|STAC|AAC;
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = AA;
	 		}
	 		break;
	 	case	  0x58://�Ѿ��������ݲ����ط�ACK
	 		Ch = I2DAT;
	 		CDevI2C0::DevID -> IntPutRecBufChar(Ch);
	 		if(CDevI2C0::DevID -> RecApplySize >= 1){
	 			I2CONCLR = SIC;//�����ظ���ʼ����
	 			I2CONSET = STA;
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = STO;
	 		}
	 		break;
	 	default:
	 	  	I2CONCLR = SIC|STAC;
	 		I2CONSET = STO;
	 		break;
	}		
	OSCInt::IntDisable();
	VICIntEnable =  1 << 9; 
	OSCInt::IntExitSwap();	 
}
CDevI2C0::CDevI2C0(const  char *Name){
	 	DevID = this;
	 	SetBusName(Name);
	 	OSCDevMng::DevID -> BusRegister(this);
}
bool   CDevI2C0::IntStartUp(){//�����ж�ģʽ����
	I2C0CONCLR =  AAC|SIC|STAC;//���Ӧ���жϡ���ʼ��־
	I2C0CONSET =  I2EN|STA;//ʹ�ܲ��������ߣ�ʣ�µ����񽻸��ж�
	VICIntEnable = 1 << 9; 		//ʹ��I2C�ж�
	return  TRUE;
}
bool	  CDevI2C0::IntShutDown(){//�ر��ж�
		VICIntEnClr =  1 << 9;
 		return  TRUE;
}
bool   CDevI2C0::BufOpen(){
		if(CurDevice -> Open()){//�������ϵ��豸
			//�����߹���
			CurDevice -> SetPin(PINSEL0 & 0x000000F0);//��������״̬
			PINSEL0	 = ((PINSEL0&0xFFFFFF0F)|0x00000050);//���������óɴ��ڹ���
			I2C0CONCLR =  AAC|SIC|STAC|I2ENC;//���Ӧ���жϡ���ʼ��־
			//�������߲�����,����Ϊ�����Ĳ�����
			I2SCLH = (Fpclk/CurDevice -> GetBaud() + 1)/2;
			I2SCLL = (Fpclk/CurDevice -> GetBaud())/2;
			I2C0CONSET =  I2EN;//ʹ��
			return TRUE;
		}
	 	return  FALSE;
}
bool   CDevI2C0::BufClose(){
		PINSEL0  =  0;//��ԭ����
		CurDevice -> Close();
 		return  TRUE;
}
bool   CDevI2C0::BufIoctl(uint32 Cmd, const void *Arg){//���ú���
	
	Cmd = Cmd;
	Arg = Arg;
	return TRUE;
}
bool   CDevI2C0::IntRegister(int8  IntID){//�ж�ע�ắ��
	OSCInt::IntRegister(Dri2103_I2C_IntServe,IntID,9);
 	return  TRUE;
}

//END