#ifndef  __OSCDEVMNG_HPP
#define  __OSCDEVMNG_HPP

class  OSCDevMng{//�����ֻ�ܴ���һ�����󣬲���Ҫ�ڴ�������֮ǰ��������Ϊ�����ڴ�����ʱ���Ҫ��������ע��
	private:
	OSCDev  *BusList;
	uint32 	BusCnt;
	private:
	//��ȡ��������
	bool		GetBusName(char *BusName,const char *DevName);
	//����ָ�����ֵ�����
	OSCDev	*GetBus(const char*BusName);
	public:
	OSCDevMng();
	static  OSCDevMng  *DevID;//
	//����ע�ᡢע������
	bool		BusRegister(OSCDev* Bus);
	bool		BusUnregister(OSCDev*Bus);
	//�豸��������
	OSCDev * open(const  char *DevName,uint32 Propety);
	uint32 	write(OSCDev*DevP,const char*Buf,uint32 Size);
	uint32 	read(OSCDev *DevP,char *Buf,uint32 Size);
	bool  	close(OSCDev *DevP);
	bool		ioctl(OSCDev *DevP,uint32  Cmd,const void *Arg);
};
#endif

//NED 