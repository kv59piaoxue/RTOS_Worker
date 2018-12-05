#include  "config.hpp"


IIC0Device1::IIC0Device1(const  char *Name,const uint32 Baud ,const  uint32 adds)//注册设备到总线上，给设备命名
{
	//IOSET = PIN2;//关闭设备1
	SetBaud(Baud);
	SetDeviceName(Name);
	SetAddress(adds);
	CDevI2C0::DevID -> DeviceRegister(this);
}

IIC0Device2::IIC0Device2(const  char *Name,const uint32 Baud,const  uint32 adds )//注册设备到总线上，给设备命名
{
	//IOSET = PIN3;//关闭设备2
	SetBaud(Baud);
	SetDeviceName(Name);
	SetAddress(adds);
	CDevI2C0::DevID -> DeviceRegister(this);
}

bool		IIC0Device1::Open()
{
	//IOCLR = PIN2;//打开设备1
	return TRUE;
}

bool		IIC0Device1::Close()
{
	//IOSET = PIN2;//关闭设备1
	return TRUE;
}
bool		IIC0Device2::Open()
{
	//IOCLR = PIN3;//打开设备2
	return TRUE;
}

bool		IIC0Device2::Close()
{
	//IOSET = PIN3;//关闭设备2
	return TRUE;
}