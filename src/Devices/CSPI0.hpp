#ifndef  __CSPI0__HPP
#define  __CSPI0__HPP

#define  SPI_SET_CPHA_FIRST   	DEV_BASE + 1
#define  SPI_SET_CPHA_SECOND	DEV_BASE + 2
#define  SPI_SET_CPOL_LOW  		DEV_BASE + 3
#define SPI_SET_CPOL_HIGHT		DEV_BASE + 4
#define SPI_SET_LSB_BIT0			DEV_BASE + 5
#define SPI_SET_LSB_BIT7			DEV_BASE + 6
#define SPI_MASTER				DEV_BASE + 7
#define SPI_SLAVE				DEV_BASE + 8
#define  SPI_RW_BYTE			DEV_BASE + 9
#define  SPI_SET_CLK        			DEV_BASE + 10

class  CSPI0 : public Cdevice{
friend  void  spi0_interrupter_fun();
private:
	OSCMutex   m_DataMutex;
protected:
	virtual  bool  Vopen();
	virtual  int32 Vwrite();
	virtual  int32 Vread();
	virtual  bool  Vioctl(uint32 cmd,void *arg = NULL);
	virtual  bool  Vclose();	
public:
	static  CSPI0 *m_this;
	CSPI0():m_DataMutex(0){
		m_this = this;
	}
};


#endif 