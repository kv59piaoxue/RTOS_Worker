#ifndef  __OSCDEV_HPP
#define __OSCDEV_HPP

class OSCDev{
	friend  class	OSCDevMng;
private:
	//注册了的总线就在链表中
	OSCDev	*NextPtr,*PrevPtr;
	bool		IsInList;//是否在链表中
	char  	BusName[30];//总线名字
	//挂接在总线上的设备
	OSCDevice	*DeviceList;
	uint32 		DeviceCnt;
	//总线信号量
	OSCMutex  BusMutex;//Dev Sourse
	//查找指定名字的设备
	OSCDevice	*GetDevice(const char*DevName);
		//对设备操作的接口函数
	bool  	Open(const char *DevName,uint32 Property);//Dev_Stat_Open;Dev_Stat_RW;Dev_Stat_RO;Dev_Stat_WO
	bool		Close();//
	uint32 	Write(const char*Buf,uint32 Size);
	uint32 	Read(char*Buf,uint32 Size);
	bool		Ioctl(uint32  Cmd,const void *Arg);
protected:
	uint32  DevStat;//Dev_Stat_Open;Dev_Stat_RW;Dev_Stat_RO;Dev_Stat_WO
	OSCDevice	*CurDevice;//当前打占用总线的设备
	//必须重载的设备底层操作函数
  	virtual  bool  DevOpen() = 0;
  	virtual  uint32   DevWrite(const char*Buf,uint32 Size) = 0  ;
  	virtual  uint32   DevRead(char*Buf,uint32 Size) = 0;
  	virtual  bool    DevIoctl(uint32  Cmd,const void *Arg) = 0;
  	virtual  bool  DevClose() = 0; 
  	//
  	OSCDev();
  	//设置总线名字
  	void 	SetBusName(const char *Name);
public:
	//将设备注册到总线上的函数
	bool		DeviceRegister(OSCDevice *Device);
	//将设备从总线上卸载的函数
	bool		DeviceUnregister(OSCDevice *Device);
}; 
#endif
//END