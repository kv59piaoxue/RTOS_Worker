#ifndef  __OSCDEVICE_HPP
#define __OSCDEVICE_HPP


class  OSCDevice{
	friend 	class	   OSCDev;
	private:
//�������֡�����������
	OSCDevice  *NextPtr,*PrevPtr;	//�ҵ������ϵ��豸�����У�����˫������
	bool		IsInList;				//��ʶ�豸�Ƿ��Ѿ���������
	char 	DeviceName[30];
//�����������ԵĲ���
	uint32  	PinSel0;//����������Ϣ
	uint32 	Baud;//�����ʣ�Ĭ��9600
	uint32  	Address;//������ַ
	protected:
	OSCDevice();
	//���豸����
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