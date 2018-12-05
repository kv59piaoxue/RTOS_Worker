#ifndef    __IIC_BUS_DEVICE_HPP
#define    __IIC_BUS_DEVICE_HPP

class   IIC0Device1:public OSCDevice{
	private:
	public:
	IIC0Device1(const  char *Name,const uint32 Baud = 9600,const uint32 adds = 0);
	virtual   	bool		Open();
	virtual 	bool		Close();
};

class   IIC0Device2:public OSCDevice{
	private:
	public:
	IIC0Device2(const  char *Name,const uint32 Baud = 9600,const uint32 adds = 0);
	virtual   	bool		Open();
	virtual 	bool		Close();
};



#endif