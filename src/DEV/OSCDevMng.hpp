#ifndef  __OSCDEVMNG_HPP
#define  __OSCDEVMNG_HPP

class  OSCDevMng{//这个类只能创建一个对象，并且要在创建总线之前创建，因为总线在创建的时候就要进行自我注册
	private:
	OSCDev  *BusList;
	uint32 	BusCnt;
	private:
	//提取总线名字
	bool		GetBusName(char *BusName,const char *DevName);
	//查找指定名字的总线
	OSCDev	*GetBus(const char*BusName);
	public:
	OSCDevMng();
	static  OSCDevMng  *DevID;//
	//总线注册、注销函数
	bool		BusRegister(OSCDev* Bus);
	bool		BusUnregister(OSCDev*Bus);
	//设备操作函数
	OSCDev * open(const  char *DevName,uint32 Propety);
	uint32 	write(OSCDev*DevP,const char*Buf,uint32 Size);
	uint32 	read(OSCDev *DevP,char *Buf,uint32 Size);
	bool  	close(OSCDev *DevP);
	bool		ioctl(OSCDev *DevP,uint32  Cmd,const void *Arg);
};
#endif

//NED 