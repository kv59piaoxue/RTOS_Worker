#ifndef  __UART0_BUS_DEVICE_HPP
#define __UART0_BUS_DEVICE_HPP

class   UART0Device1:public OSCDevice{
	private:
	public:
	UART0Device1(const  char *Name,const uint32 Baud = 9600);
	virtual   	bool		Open();
	virtual 	bool		Close();
};

class   UART0Device2:public OSCDevice{
	private:
	public:
	UART0Device2(const  char *Name,const uint32 Baud = 9600);
	virtual   	bool		Open();
	virtual 	bool		Close();
};
#endif

//END