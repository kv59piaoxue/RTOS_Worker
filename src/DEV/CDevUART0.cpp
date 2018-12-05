#include "config.hpp"

CDevUART0 *CDevUART0::DevID  = NULL;
CDevUART0::CDevUART0(const  char *Name){
	 	DevID = this;
	 	SetBusName(Name);
	 	OSCDevMng::DevID -> BusRegister(this);//注册总线
}
bool   CDevUART0::IntStartUp(){//开启中断模式函数
		char  Con;
		if(DevStat & Dev_Stat_RW)	   	Con = 0x03;//允许接收、发送中断
		else  if(DevStat & Dev_Stat_RO)  		Con = 0x01;//允许接收
		else	if(DevStat & Dev_Stat_WO)  	Con = 0x02;//发送中断	
		U0IER  = Con ;//
		if(CDevUART0::DevID -> IntGetTxBufChar(&Con))
	 		U0THR = Con;//
	 	VICIntEnable =  1 << 6;//使能中断
  		return  TRUE;
}
bool	  CDevUART0::IntShutDown(){//关闭中断
		U0IER  &= ~0x03 ;//
		VICIntEnClr =  1 << 6;
 		return  TRUE;
}
bool   CDevUART0::BufOpen(){
		uint32 Fdiv;
		if(CurDevice -> Open()){//打开总线上的设备
			//打开总线功能
			CurDevice -> SetPin(PINSEL0 & 0x0000000F);//保存引脚状态
			PINSEL0	 = ((PINSEL0&0xFFFFFFF0)|0x00000005);//将引脚设置成串口功能
			//设置总线波特率
			U0LCR = 0x83;//DLAB = 1，允许设置波特率
			Fdiv = (Fpclk/16)/CurDevice -> GetBaud();
			U0DLM  = Fdiv >> 8;
			U0DLL = Fdiv & 0xFF;
			U0LCR = 0x03;//DLAB = 1
			return TRUE;
		}
	 	return  FALSE;
}
bool   CDevUART0::BufClose(){
		PINSEL0  =  0;//还原引脚
		CurDevice -> Close();
 		return  TRUE;
}
bool   CDevUART0::IntRegister(int8  IntID){//中断注册函数
	OSCInt::IntRegister(Dri2103_Uart0_IntServe,IntID,6);
 	return  TRUE;
}	

bool   CDevUART0::BufIoctl(uint32 Cmd, const  void *Arg){//设置函数
		switch(Cmd)
		{
			case  CMD_CHANGEBAUD:	
				CurDevice ->SetBaud( (uint32)Arg);	
				break;
			default :		
				return FALSE;
		}
 		return  TRUE;
}
void  Dri2103_Uart0_IntServe()//中断服务函数地址
{
	 	uint8  IIR;
	 	char   Ch;
	 	VICIntEnClr =  1 << 6;//设备保护
	 	VICVectAddr = 0; 	 
	 	OSCInt::IntEnable();
	 	while((0x01&(IIR = U0IIR)) == 0)
	 	{
	 		switch(IIR &0x0e)
	 		{
	 			case 0x02: //THRE
	 					if(CDevUART0::DevID -> IntGetTxBufChar(&Ch))
	 						U0THR = Ch;//
	 				break;
	 			case 0x04://RDA
	 					Ch = U0RBR;//
	 					//CDevUART0::DevID -> IntPutRecBufChar(Ch);
	 				break;
	 			case 0x0C://CTI 超时中断，接收一个
	 					U0IER = U0IER & (~0x01);//禁止接受及字符中断
	 					break;
	 			case  0x06:	//奇偶错误
	 					break;
	 			default: 
	 					break;
	 		}//end  switch
	 	}//end  while	
	 	OSCInt::IntDisable();	
	 	VICIntEnable =  1 << 6;//使能中断
	 	OSCInt::IntExitSwap();	 	
}
//END 

