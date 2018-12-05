#include "config.hpp"

CDevSPI0  * CDevSPI0::DevID = NULL;
void  Dri2103_SPI0_IntServe();

void  Dri2103_SPI0_IntServe(){
	VICVectAddr = 0;
	VICIntEnClr = 1 << 10;//设备保护
	OSCInt::IntEnable();
	char  Ch;
	//switch()
	//{
	//}		
	OSCInt::IntDisable();
	VICIntEnable =  1 << 9; 
	SPINT  = 0x01;//清除中断标志
	OSCInt::IntExitSwap();	 
}
CDevSPI0::CDevSPI0(const  char *Name){
	 	DevID = this;
	 	SetBusName(Name);
	 	OSCDevMng::DevID -> BusRegister(this);
}
bool   CDevSPI0::IntStartUp(){//开启中断模式函数

	VICIntEnable = 1 << 10; 		//使能I2C中断
	return  TRUE;
}
bool	  CDevSPI0::IntShutDown(){//关闭中断
		VICIntEnClr =  1 << 10;
 		return  TRUE;
}
bool   CDevSPI0::BufOpen(){
		if(CurDevice -> Open()){//打开总线上的设备
			//打开总线功能
			CurDevice -> SetPin(PINSEL0 & (0xFF << 8));//保存引脚状态
			PINSEL0	 = ((PINSEL0&(~(0xFF << 8)))|(0x55 << 8));//将引脚设置成串口功能
			//设置总线波特率,设置为器件的波特率
			SPCCR = Fpclk/CurDevice -> GetBaud();
			return TRUE;
		}
	 	return  FALSE;
}
bool   CDevSPI0::BufClose(){
		PINSEL0  =  0;//还原引脚
		CurDevice -> Close();
 		return  TRUE;
}
bool   CDevSPI0::BufIoctl(uint32 Cmd, const void *Arg){//设置函数
	
	Cmd = Cmd;
	Arg = Arg;
	return TRUE;
}
bool   CDevSPI0::IntRegister(int8  IntID){//中断注册函数
	OSCInt::IntRegister(Dri2103_SPI0_IntServe,IntID,9);
 	return  TRUE;
}

//END