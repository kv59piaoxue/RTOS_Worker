
#include "config.hpp"

 CIIC0 * CIIC0::m_this = NULL;
void  iic0_interrupter_fun(){
	IOSET = LED1;
	CIIC0*  iic0_this = CIIC0::m_this;
	 iic0_this -> SetRegbit32(&VICIntEnClr,9);//�豸����
	OSCInt::IntEnterInt();
	VICVectAddr = 0;
	OSMsg   Msg(OSCGuardThread::ThreadID,TM_MUTEXPUTMEDI,(WPARAM)&iic0_this -> m_DataMutex);
	switch(I2STAT & 0xF8)
	{
		case 	  0x08://�Ѿ�������ʼ����������ģʽ�����ͺͽ��ܶ��п���
		case   0x10://�Ѿ������ظ���ʼ����������ģʽ�����ͺͽ��ܶ��п���
			iic0_this -> SetReg8(&I2DAT,iic0_this -> Address); 
			//����жϱ�־�Ϳ�ʼ��־
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);
	 		break;
	 	case   0x18://�Ѿ�����SLA + W�ͽ���ACK
	 		iic0_this -> SetReg8(&I2DAT,* iic0_this -> Sendbuf);
	 		iic0_this -> Sendbuf ++;
	 		iic0_this -> Sendbyte --;
	 		iic0_this -> SetRegbit8(&I2CONCLR,SIC);
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);
	 		break;
	 	case   0x28://�Ѿ�����I2DAT�е����ݺͽ���ACK
	 		if(iic0_this -> Sendbyte > 0){
	 			iic0_this -> SetReg8(&I2DAT,* iic0_this -> Sendbuf);
	 			iic0_this -> Sendbuf ++;
	 			iic0_this -> Sendbyte --;
	 			iic0_this -> SetRegbit8(&I2CONCLR,SIC);
				iic0_this -> SetRegbit8(&I2CONCLR,STAC);
			}else{
				iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
				iic0_this -> SetRegbit8(&I2CONCLR,STAC);//�����ʼ��־
				iic0_this -> SetRegbit8(&I2CONSET,STO);//����ֹͣ�ź�
				//������Ϣ�����ѵȴ����ݷ�����ɵ��߳�
				OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
			}
	 		break;
	 	case   0x20://�Ѿ�����SLA + W�ͽ��շ�ACK
	 	case   0x30://�Ѿ�����I2DAT�е����ݺͽ��շ�ACK
	 	case	  0x48://�Ѿ�����SLA+R�ͽ��շ�ACK
	 		iic0_this -> SetRegbit8(&I2CONSET,STO);//����ֹͣ�ź�
	 		iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);//�����ʼ��־
			//������Ϣ�����ѵȴ����ݷ�����ɵ��߳�
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 		 break;
	 	case   0x38://��SLA+R/W���������ֽ��ж�ʧ�ٲ�
	 		iic0_this -> SetRegbit8(&I2CONSET,STO);//����ֹͣ�ź�
	 		iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
	 		//������Ϣ�����ѵȴ����ݷ�����ɵ��߳�
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 		break;
	 	case	  0x40://�Ѿ�����SLA+R�ͽ���ACK
	 		if(iic0_this -> Recbyte <= 1){
	 			iic0_this -> SetRegbit8(&I2CONCLR,AAC);//���ͷ�ACK
	 		}else{
	 			iic0_this -> SetRegbit8(&I2CONSET,AA);//����ACK
	 		}
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);//�����ʼ��־
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
	 		break;
	 	case	  0x50://�Ѿ��������ݲ�����ACK
	 		*iic0_this -> Recbuf =  iic0_this -> GetReg8(&I2DAT);
	 		iic0_this -> Recbuf ++;
	 		iic0_this -> Recbyte --;
	 		if(iic0_this -> Recbyte <= 1){
	 			iic0_this -> SetRegbit8(&I2CONCLR,AAC);//���ͷ�ACK
	 		}else{
	 			iic0_this -> SetRegbit8(&I2CONSET,AA);//����ACK
	 		}
	 		iic0_this -> SetRegbit8(&I2CONCLR,STAC);//�����ʼ��־
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
	 		break;
	 	case	  0x58://�Ѿ��������ݲ����ط�ACK
	 		*iic0_this -> Recbuf =  iic0_this -> GetReg8(&I2DAT);
	 		iic0_this -> Recbyte --;
	 		iic0_this -> SetRegbit8(&I2CONSET,STO);//����ֹͣ�ź�
			//������Ϣ�����ѵȴ����ݷ�����ɵ��߳�
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);//�����ʼ��־
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
	 		break;
	 	default:
	 	  	iic0_this -> SetRegbit8(&I2CONCLR,STAC);//�����ʼ��־
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//����жϱ�־
			iic0_this -> SetRegbit8(&I2CONSET,STO);//����ֹͣ�ź�
			iic0_this -> SetRegbit8(&I2CONSET,STA);
	 		break;
	}		
	OSCInt::IntDisable();
	 iic0_this -> SetRegbit32(&VICIntEnable,9);
	OSCInt::IntExitSwap();	 
}
bool  CIIC0::Vopen(){
	m_IntID = OSCInt::GetIntVector();
	if( m_IntID != -1){
		OSCInt::IntRegister(iic0_interrupter_fun,m_IntID,9);
		CPin_2103  SCL(2,1);
		CPin_2103  SDA(3,1);
		SetReg16(&I2SCLH,(Fpclk/100000 + 1)/2);
		SetReg16(&I2SCLL,(Fpclk/100000 )/2);
		return TRUE;
	}
	return  FALSE;
}
int32 CIIC0::Vwrite(){
	int32 cnt = Sendbyte;
	Address &= 0xfe;//iic
	//SetRegbit8(&I2CONCLR,SIC);//
	//SetRegbit8(&I2CONSET,STA);//
	//SetRegbit8(&I2CONSET,I2EN);//
	//SetRegbit32(&VICIntEnable,9);
	
	I2CONCLR =  AAC|SIC|STAC;//���Ӧ���жϡ���ʼ��־
	I2CONSET =  I2EN|STA;//ʹ�ܲ��������ߣ�ʣ�µ����񽻸��ж�
	VICIntEnable = 1 << 9; 		//ʹ��I2C�ж�
	
	IOSET = LED3;
	if(m_DataMutex.MutexGetSwap(0)){
		return (cnt - Sendbyte);
	}
	m_DataMutex.MutexPutSwap();
	return -1;
}
int32 CIIC0::Vread(){
	int32 cnt =Recbyte;
	Address |= 0x01;//iic
	SetRegbit8(&I2CONCLR,SIC);//
	SetRegbit8(&I2CONSET,STA);//
	SetRegbit8(&I2CONSET,I2EN);//
	SetRegbit32(&VICIntEnable,9);
	if(m_DataMutex.MutexGetSwap(0)){
		return cnt - Recbyte;
	}
	m_DataMutex.MutexPutSwap();
	return -1;
}
bool  CIIC0::Vioctl(uint32 cmd,void *arg){
	uint8 clk = (uint32)arg;
	if(clk < 4)
		clk = 4;
	switch(cmd){
		case IIC_SET_CLH: 
			SetReg16(&I2SCLH,clk);
			break;
		case IIC_SET_CLL: 
			SetReg16(&I2SCLL,clk);
			break;
		default:
			return FALSE;
	}
	return TRUE;
}
bool  CIIC0::Vclose(){
	OSCInt::PutIntVector(m_IntID);
	SetRegbit32(&VICIntEnClr,9);
	CPin_2103  SCL(2,0,OUT);
	CPin_2103  SDA(3,0,OUT);
	return  TRUE;
}
