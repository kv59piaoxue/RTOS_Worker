#include  "config.hpp"


IIC0Device1::IIC0Device1(const  char *Name,const uint32 Baud ,const  uint32 adds)//ע���豸�������ϣ����豸����
{
	//IOSET = PIN2;//�ر��豸1
	SetBaud(Baud);
	SetDeviceName(Name);
	SetAddress(adds);
	CDevI2C0::DevID -> DeviceRegister(this);
}

IIC0Device2::IIC0Device2(const  char *Name,const uint32 Baud,const  uint32 adds )//ע���豸�������ϣ����豸����
{
	//IOSET = PIN3;//�ر��豸2
	SetBaud(Baud);
	SetDeviceName(Name);
	SetAddress(adds);
	CDevI2C0::DevID -> DeviceRegister(this);
}

bool		IIC0Device1::Open()
{
	//IOCLR = PIN2;//���豸1
	return TRUE;
}

bool		IIC0Device1::Close()
{
	//IOSET = PIN2;//�ر��豸1
	return TRUE;
}
bool		IIC0Device2::Open()
{
	//IOCLR = PIN3;//���豸2
	return TRUE;
}

bool		IIC0Device2::Close()
{
	//IOSET = PIN3;//�ر��豸2
	return TRUE;
}