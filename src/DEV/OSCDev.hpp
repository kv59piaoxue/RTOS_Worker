#ifndef  __OSCDEV_HPP
#define __OSCDEV_HPP

class OSCDev{
	friend  class	OSCDevMng;
private:
	//ע���˵����߾���������
	OSCDev	*NextPtr,*PrevPtr;
	bool		IsInList;//�Ƿ���������
	char  	BusName[30];//��������
	//�ҽ��������ϵ��豸
	OSCDevice	*DeviceList;
	uint32 		DeviceCnt;
	//�����ź���
	OSCMutex  BusMutex;//Dev Sourse
	//����ָ�����ֵ��豸
	OSCDevice	*GetDevice(const char*DevName);
		//���豸�����Ľӿں���
	bool  	Open(const char *DevName,uint32 Property);//Dev_Stat_Open;Dev_Stat_RW;Dev_Stat_RO;Dev_Stat_WO
	bool		Close();//
	uint32 	Write(const char*Buf,uint32 Size);
	uint32 	Read(char*Buf,uint32 Size);
	bool		Ioctl(uint32  Cmd,const void *Arg);
protected:
	uint32  DevStat;//Dev_Stat_Open;Dev_Stat_RW;Dev_Stat_RO;Dev_Stat_WO
	OSCDevice	*CurDevice;//��ǰ��ռ�����ߵ��豸
	//�������ص��豸�ײ��������
  	virtual  bool  DevOpen() = 0;
  	virtual  uint32   DevWrite(const char*Buf,uint32 Size) = 0  ;
  	virtual  uint32   DevRead(char*Buf,uint32 Size) = 0;
  	virtual  bool    DevIoctl(uint32  Cmd,const void *Arg) = 0;
  	virtual  bool  DevClose() = 0; 
  	//
  	OSCDev();
  	//������������
  	void 	SetBusName(const char *Name);
public:
	//���豸ע�ᵽ�����ϵĺ���
	bool		DeviceRegister(OSCDevice *Device);
	//���豸��������ж�صĺ���
	bool		DeviceUnregister(OSCDevice *Device);
}; 
#endif
//END