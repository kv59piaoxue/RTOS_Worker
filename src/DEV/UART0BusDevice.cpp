#include "config.hpp"
#define   PIN2    	1 << 4
#define   PIN3  	1 << 5 

UART0Device1::UART0Device1(const  char *Name,const uint32 Baud)//ע���豸�������ϣ����豸����
{
	IOSET = PIN2;//�ر��豸1
	SetBaud(Baud);
	SetDeviceName(Name);
	CDevUART0::DevID -> DeviceRegister(this);
}

UART0Device2::UART0Device2(const  char *Name,const uint32 Baud)//ע���豸�������ϣ����豸����
{
	IOSET = PIN3;//�ر��豸2
	SetBaud(Baud);
	SetDeviceName(Name);
	CDevUART0::DevID -> DeviceRegister(this);
}

bool		UART0Device1::Open()
{
	IOCLR = PIN2;//���豸1
	return TRUE;
}

bool		UART0Device1::Close()
{
	IOSET = PIN2;//�ر��豸1
	return TRUE;
}
bool		UART0Device2::Open()
{
	IOCLR = PIN3;//���豸2
	return TRUE;
}

bool		UART0Device2::Close()
{
	IOSET = PIN3;//�ر��豸2
	return TRUE;
}
//END