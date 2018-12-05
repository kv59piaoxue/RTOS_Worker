#include "config.hpp"

CSPI0 *CSPI0::m_this = NULL;

void  spi0_interrupter_fun(){
	IOSET = LED1;
}
bool  CSPI0::Vopen(){
	m_IntID = OSCInt::GetIntVector();
	if( m_IntID != -1){
		OSCInt::IntRegister(spi0_interrupter_fun,m_IntID,10);
		//初始化SPI引脚
		CPin_2103  SCK(4,1);
		CPin_2103  MISO(5,1);
		CPin_2103  MOSI(6,1);
		CPin_2103  SSEL(7,1);
		//设置总线波特率
		uint32 clk;
		clk = Fpclk / 10000;
		if(clk < 8)
			clk = 8;
		SetReg8(&SPI_SPCCR,clk);
		return TRUE;
	}
	return  FALSE;
}
int32 CSPI0::Vwrite(){
	int32 cnt = Sendbyte;
	if(Sendbyte > 0){// 发送一个字节
	 	SetReg8(&SPI_SPDR,*Sendbuf);//
	 	Sendbuf ++;
	 	Sendbyte --;
	 }
	 SetRegbit8(&SPI_SPCR,7);
	 SetRegbit32(&VICIntEnable,10);
	if(m_DataMutex.MutexGetSwap(0)){
		return (cnt - Sendbyte);
	}
	SetRegbit32(&VICIntEnClr,10);
	m_DataMutex.MutexPutSwap();
	return -1;
}
int32 CSPI0::Vread(){
	int32 cnt = Recbyte;
	 SetReg8(&SPI_SPDR,*Sendbuf);//触发中断
	 SetRegbit8(&SPI_SPCR,7);
	 SetRegbit32(&VICIntEnable,10);
	if(m_DataMutex.MutexGetSwap(0)){
		return (cnt - Recbyte);
	}
	SetRegbit32(&VICIntEnClr,10);
	m_DataMutex.MutexPutSwap();
	return -1;
}
bool  CSPI0::Vioctl(uint32 cmd,void *arg){
	switch(cmd){
		case SPI_SET_CLK: 
			uint8 clk;
			clk = Fpclk / (uint32)arg;
			if(clk < 8)
				clk = 8;
			SetReg8(&SPI_SPCCR,clk);
			break;
		case SPI_SET_CPHA_FIRST: 
			ClrRegbit8(&SPI_SPCR,3);
			break;
		case SPI_SET_CPHA_SECOND: 
			SetRegbit8(&SPI_SPCR,3);
			break;
		case SPI_SET_CPOL_LOW: 
			SetRegbit8(&SPI_SPCR,4);
			break;
		case SPI_SET_CPOL_HIGHT: 
			ClrRegbit8(&SPI_SPCR,4);
			break;
		case SPI_SET_LSB_BIT0: 
			SetRegbit8(&SPI_SPCR,6);
			break;
		case SPI_SET_LSB_BIT7: 
			ClrRegbit8(&SPI_SPCR,6);
			break;
		case SPI_MASTER: 
			SetRegbit8(&SPI_SPCR,5);
			break;
		case SPI_SLAVE: 
			ClrRegbit8(&SPI_SPCR,5);
			break;
		case SPI_RW_BYTE: 
			SPI_SPDR= *(uint8*)arg;
      			while( (SPI_SPSR & 0x80) == 0 );       // 等待bsy置位，即发送FIFO空  
      			*(uint8*)arg = SPI_SPDR;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}
bool  CSPI0::Vclose(){
	CPin_2103  SCK(4,0,OUT);
	CPin_2103  MISO(5,0,OUT);
	CPin_2103  MOSI(6,0,OUT);
	CPin_2103  SSEL(7,0,OUT);
	return  TRUE;
}

//END