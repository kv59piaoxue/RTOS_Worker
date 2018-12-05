#include "config.hpp"
#define   PIN2    	1 << 4
#define   PIN3  	1 << 5 

UART0Device1::UART0Device1(const  char *Name,const uint32 Baud)//注册设备到总线上，给设备命名
{
	IOSET = PIN2;//关闭设备1
	SetBaud(Baud);
	SetDeviceName(Name);
	CDevUART0::DevID -> DeviceRegister(this);
}

UART0Device2::UART0Device2(const  char *Name,const uint32 Baud)//注册设备到总线上，给设备命名
{
	IOSET = PIN3;//关闭设备2
	SetBaud(Baud);
	SetDeviceName(Name);
	CDevUART0::DevID -> DeviceRegister(this);
}

bool		UART0Device1::Open()
{
	IOCLR = PIN2;//打开设备1
	return TRUE;
}

bool		UART0Device1::Close()
{
	IOSET = PIN2;//关闭设备1
	return TRUE;
}
bool		UART0Device2::Open()
{
	IOCLR = PIN3;//打开设备2
	return TRUE;
}

bool		UART0Device2::Close()
{
	IOSET = PIN3;//关闭设备2
	return TRUE;
}
//END