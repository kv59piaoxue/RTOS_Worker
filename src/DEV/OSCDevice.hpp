#ifndef  __OSCDEVICE_HPP
#define __OSCDEVICE_HPP


class  OSCDevice{
	friend 	class	   OSCDev;
	private:
//器件名字······
	OSCDevice  *NextPtr,*PrevPtr;	//挂到总线上的设备链表中，构成双向链表
	bool		IsInList;				//标识设备是否已经在链表中
	char 	DeviceName[30];
//关于器件特性的参数
	uint32  	PinSel0;//保存引脚信息
	uint32 	Baud;//波特率，默认9600
	uint32  	Address;//器件地址
	protected:
	OSCDevice();
	//给设备命名
	void 	SetDeviceName(const char *Name);//
	public:
	void		SetPin(const uint32 Pin){ PinSel0 = Pin;}
	uint32 	GetPin(){return PinSel0;}
	void 	SetBaud(const uint32 B){Baud = B;}
	uint32     GetBaud(){ return Baud;}
	uint32	GetAddress(){return Address;}
	void 	SetAddress(const  uint32  Adds){ Address = Adds;}
	//
	virtual  		bool		Open() = 0;				//
	virtual  		bool		Close() = 0;				//
};
#endif