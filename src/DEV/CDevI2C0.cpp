#include "config.hpp"

CDevI2C0  * CDevI2C0::DevID = NULL;
void  Dri2103_I2C_IntServe();

void  Dri2103_I2C_IntServe(){
	VICVectAddr = 0;
	VICIntEnClr = 1 << 9;//设备保护
	OSCInt::IntEnable();
	char  Ch;
	switch(I2STAT & 0xF8)
	{
		case 	  0x08://已经发送起始条件，主机模式：发送和接受都有可能
		case   0x10://已经发送重复起始条件，主机模式：发送和接受都有可能
	 		if(CDevI2C0::DevID -> DevStat & Dev_Stat_RO){//指定地址读数据
			//向从器件写入地址和读信号
				I2DAT = CDevI2C0::DevID -> CurDevice -> GetAddress()| 0x01; 
			}else  if(CDevI2C0::DevID -> DevStat & Dev_Stat_WO){
			//发送从机地址和写信号
				I2DAT = CDevI2C0::DevID -> CurDevice -> GetAddress() & 0xFE;
			}
			I2CONCLR = SIC|STAC;//清除中断标志和开始标志
	 		break;
	 	case   0x18://已经发送SLA + W和接收ACK
	 		//break;
	 	case   0x20://已经发送SLA + W和接收非ACK
	 	 	//IOSET  = LED7;
	 		//break;
	 	case   0x28://已经发送I2DAT中的数据和接收ACK
	 		//IOSET = LED6;
	 		//break;
	 	case   0x30://已经发送I2DAT中的数据和接收非ACK
	 		if(CDevI2C0::DevID -> IntGetTxBufChar(&Ch)){//还需要发送数据
	 			I2DAT = Ch;//写入数据并发送
	 			I2CONCLR = SIC|STAC;
	 		}else{
	 		 	I2CONCLR = SIC|STAC;
	 		 	I2CONSET = STO;//发送停止条件
	 		 }
	 		 break;
	 	case   0x38://在SLA+R/W或者数据字节中丢失仲裁
	 		I2CONCLR = SIC;
	 		I2CONSET = STA;//发送起始条件（在总线空闲时重新开始）
	 		break;
	 	case	  0x40://已经发生SLA+R和接收ACK
	 		if(CDevI2C0::DevID -> RecApplySize <= 1){
	 			I2CONCLR = SIC|STAC|AAC;
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = AA;
	 		}
	 		break;
	 	case	  0x48://已经发生SLA+R和接收非ACK
	 		if(CDevI2C0::DevID -> RecApplySize >= 1){
	 			I2CONCLR = SIC;
	 			I2CONSET = STA;//发送起始条件
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = STO;
	 		}
	 		break;
	 	case	  0x50://已经接收数据并返回ACK
	 		Ch = I2DAT;
	 		CDevI2C0::DevID -> IntPutRecBufChar(Ch);
	 		if(CDevI2C0::DevID -> RecApplySize <= 1){
	 			I2CONCLR = SIC|STAC|AAC;
	 		}else{
	 			I2CONCLR = SIC|STAC;
	 			I2CONSET = AA;
	 		}
	 		break;
	 	case	  0x58://已经接收数据并返回非ACK
	 		Ch = I2DAT;
	 		CDevI2C0::DevID -> IntPutRecBufChar(Ch);
	 		if(CDevI2C0::DevID -> RecApplySize >= 1){
	 			I2CONCLR = SIC;//发送重复起始条件
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
bool   CDevI2C0::IntStartUp(){//开启中断模式函数
	I2C0CONCLR =  AAC|SIC|STAC;//清除应答、中断、开始标志
	I2C0CONSET =  I2EN|STA;//使能并启动总线，剩下的任务交给中断
	VICIntEnable = 1 << 9; 		//使能I2C中断
	return  TRUE;
}
bool	  CDevI2C0::IntShutDown(){//关闭中断
		VICIntEnClr =  1 << 9;
 		return  TRUE;
}
bool   CDevI2C0::BufOpen(){
		if(CurDevice -> Open()){//打开总线上的设备
			//打开总线功能
			CurDevice -> SetPin(PINSEL0 & 0x000000F0);//保存引脚状态
			PINSEL0	 = ((PINSEL0&0xFFFFFF0F)|0x00000050);//将引脚设置成串口功能
			I2C0CONCLR =  AAC|SIC|STAC|I2ENC;//清除应答、中断、开始标志
			//设置总线波特率,设置为器件的波特率
			I2SCLH = (Fpclk/CurDevice -> GetBaud() + 1)/2;
			I2SCLL = (Fpclk/CurDevice -> GetBaud())/2;
			I2C0CONSET =  I2EN;//使能
			return TRUE;
		}
	 	return  FALSE;
}
bool   CDevI2C0::BufClose(){
		PINSEL0  =  0;//还原引脚
		CurDevice -> Close();
 		return  TRUE;
}
bool   CDevI2C0::BufIoctl(uint32 Cmd, const void *Arg){//设置函数
	
	Cmd = Cmd;
	Arg = Arg;
	return TRUE;
}
bool   CDevI2C0::IntRegister(int8  IntID){//中断注册函数
	OSCInt::IntRegister(Dri2103_I2C_IntServe,IntID,9);
 	return  TRUE;
}

//END