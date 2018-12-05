
#include "config.hpp"

 CIIC0 * CIIC0::m_this = NULL;
void  iic0_interrupter_fun(){
	IOSET = LED1;
	CIIC0*  iic0_this = CIIC0::m_this;
	 iic0_this -> SetRegbit32(&VICIntEnClr,9);//设备保护
	OSCInt::IntEnterInt();
	VICVectAddr = 0;
	OSMsg   Msg(OSCGuardThread::ThreadID,TM_MUTEXPUTMEDI,(WPARAM)&iic0_this -> m_DataMutex);
	switch(I2STAT & 0xF8)
	{
		case 	  0x08://已经发送起始条件，主机模式：发送和接受都有可能
		case   0x10://已经发送重复起始条件，主机模式：发送和接受都有可能
			iic0_this -> SetReg8(&I2DAT,iic0_this -> Address); 
			//清除中断标志和开始标志
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);
	 		break;
	 	case   0x18://已经发送SLA + W和接收ACK
	 		iic0_this -> SetReg8(&I2DAT,* iic0_this -> Sendbuf);
	 		iic0_this -> Sendbuf ++;
	 		iic0_this -> Sendbyte --;
	 		iic0_this -> SetRegbit8(&I2CONCLR,SIC);
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);
	 		break;
	 	case   0x28://已经发送I2DAT中的数据和接收ACK
	 		if(iic0_this -> Sendbyte > 0){
	 			iic0_this -> SetReg8(&I2DAT,* iic0_this -> Sendbuf);
	 			iic0_this -> Sendbuf ++;
	 			iic0_this -> Sendbyte --;
	 			iic0_this -> SetRegbit8(&I2CONCLR,SIC);
				iic0_this -> SetRegbit8(&I2CONCLR,STAC);
			}else{
				iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
				iic0_this -> SetRegbit8(&I2CONCLR,STAC);//清除开始标志
				iic0_this -> SetRegbit8(&I2CONSET,STO);//发送停止信号
				//发送消息，唤醒等待数据发送完成的线程
				OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
			}
	 		break;
	 	case   0x20://已经发送SLA + W和接收非ACK
	 	case   0x30://已经发送I2DAT中的数据和接收非ACK
	 	case	  0x48://已经发生SLA+R和接收非ACK
	 		iic0_this -> SetRegbit8(&I2CONSET,STO);//发送停止信号
	 		iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);//清除开始标志
			//发送消息，唤醒等待数据发送完成的线程
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 		 break;
	 	case   0x38://在SLA+R/W或者数据字节中丢失仲裁
	 		iic0_this -> SetRegbit8(&I2CONSET,STO);//发送停止信号
	 		iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
	 		//发送消息，唤醒等待数据发送完成的线程
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 		break;
	 	case	  0x40://已经发生SLA+R和接收ACK
	 		if(iic0_this -> Recbyte <= 1){
	 			iic0_this -> SetRegbit8(&I2CONCLR,AAC);//发送非ACK
	 		}else{
	 			iic0_this -> SetRegbit8(&I2CONSET,AA);//发送ACK
	 		}
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);//清除开始标志
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
	 		break;
	 	case	  0x50://已经接收数据并返回ACK
	 		*iic0_this -> Recbuf =  iic0_this -> GetReg8(&I2DAT);
	 		iic0_this -> Recbuf ++;
	 		iic0_this -> Recbyte --;
	 		if(iic0_this -> Recbyte <= 1){
	 			iic0_this -> SetRegbit8(&I2CONCLR,AAC);//发送非ACK
	 		}else{
	 			iic0_this -> SetRegbit8(&I2CONSET,AA);//发送ACK
	 		}
	 		iic0_this -> SetRegbit8(&I2CONCLR,STAC);//清除开始标志
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
	 		break;
	 	case	  0x58://已经接收数据并返回非ACK
	 		*iic0_this -> Recbuf =  iic0_this -> GetReg8(&I2DAT);
	 		iic0_this -> Recbyte --;
	 		iic0_this -> SetRegbit8(&I2CONSET,STO);//发送停止信号
			//发送消息，唤醒等待数据发送完成的线程
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
			iic0_this -> SetRegbit8(&I2CONCLR,STAC);//清除开始标志
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
	 		break;
	 	default:
	 	  	iic0_this -> SetRegbit8(&I2CONCLR,STAC);//清除开始标志
			iic0_this -> SetRegbit8(&I2CONCLR,SIC);//清除中断标志
			iic0_this -> SetRegbit8(&I2CONSET,STO);//发送停止信号
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
	
	I2CONCLR =  AAC|SIC|STAC;//清除应答、中断、开始标志
	I2CONSET =  I2EN|STA;//使能并启动总线，剩下的任务交给中断
	VICIntEnable = 1 << 9; 		//使能I2C中断
	
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
