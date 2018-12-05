#ifndef  __CUART0_HPP
#define __CUART0_HPP
 

#define   UART_SET_LENGTH  DEV_BASE + 1 //设置字长
#define  UART_SET_VERIFY     DEV_BASE + 2//奇偶校验
#define  UART_SET_BAUND    DEV_BASE + 3//设置波特率

class  CUART0:public Cdevice{
friend  void  uart0_interrupter_fun();
private:
	OSCMutex  m_DataMutex;
	static CUART0 *m_this;
//Vfunction
	virtual  bool  Vopen();
	virtual  int32 Vwrite();
	virtual  int32 Vread();
	virtual  bool  Vioctl(uint32 cmd,void *arg = NULL);
	virtual  bool  Vclose();	
public:
	CUART0():m_DataMutex(0){ m_this = this; }
};

#endif