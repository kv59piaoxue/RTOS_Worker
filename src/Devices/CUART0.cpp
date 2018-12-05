
#include "config.hpp"

 CUART0 * CUART0::m_this = NULL;
void  uart0_interrupter_fun(){
	CUART0*  uart_this = CUART0::m_this;
	 uart_this -> SetRegbit32(&VICIntEnClr,6);//设备保护
	OSCInt::IntEnterInt();
	VICVectAddr = 0;
	uint8  IIR;
	OSMsg   Msg(OSCGuardThread::ThreadID,TM_MUTEXPUTMEDI,(WPARAM)&uart_this -> m_DataMutex);
	while((0x01&(IIR = U0IIR)) == 0)
	 	{
	 		switch(IIR &0x0e)
	 		{
	 			case 0x02: //THRE
	 					if(uart_this -> Sendbyte > 0){
	 						uart_this -> SetReg8(&U0THR ,*uart_this -> Sendbuf);//
	 						uart_this -> Sendbuf ++;
	 						uart_this -> Sendbyte --;
	 					}else{//发送消息，唤醒等待数据发送完成的线程
							OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 						//uart_this -> SetRegbit32(&VICIntEnClr,6);//设备保护
	 						//TF = FALSE;
						}
	 				break;
	 			case 0x04://RDA
	 					if(uart_this -> Recbyte > 0){
	 						*uart_this -> Recbuf = uart_this -> GetReg8(&U0RBR);//
	 						uart_this -> Recbuf ++;
	 						uart_this -> Recbyte --;
	 					}else{//发送消息，唤醒等待数据发送完成的线程
							OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
							//uart_this -> SetRegbit32(&VICIntEnClr,6);//设备保护
	 						//TF = FALSE;
						}
	 				break;
	 			case 0x0C://CTI 超时中断，接收一个
	 					U0IER = U0IER & (~0x01);//禁止接受及字符中断
	 					//发送消息，唤醒等待数据发送完成的线程
						OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
						//uart_this -> SetRegbit32(&VICIntEnClr,6);//设备保护
	 					//TF = FALSE;
	 					break;
	 			case  0x06:	//奇偶错误
	 					//发送消息，唤醒等待数据发送完成的线程
						//OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 					break;
	 			default: 
	 					//发送消息，唤醒等待数据发送完成的线程
						//OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	 					break;
	 		}//end  switch
	 	}//end  while		
	OSCInt::IntDisable();
	 uart_this -> SetRegbit32(&VICIntEnable,6);
	OSCInt::IntExitSwap();	 
}
bool  CUART0::Vopen(){
	m_IntID = OSCInt::GetIntVector();
	if( m_IntID != -1){
		OSCInt::IntRegister(uart0_interrupter_fun,m_IntID,6);
		CPin_2103  SCL(0,1);
		CPin_2103  SDA(1,1);
		//设置总线波特率
		uint32 Fdiv;
		SetReg8(&U0LCR,0x83);//DLAB = 1，允许设置波特率
		Fdiv = (Fpclk/16)/9600;
		SetReg8(&U0DLM,Fdiv >> 8);
		SetReg8(&U0DLL,Fdiv & 0xFF);
		SetReg8(&U0LCR,0x03);//DLAB = 1
		return TRUE;
	}
	return  FALSE;
}
int32 CUART0::Vwrite(){
	int32 cnt = Sendbyte;
	if(Sendbyte > 0){// 发送一个字节
	 	SetReg8(&U0THR,*Sendbuf);//
	 	Sendbuf ++;
	 	Sendbyte --;
	 }
	 SetReg8(&U0IER,0x02);
	 SetRegbit32(&VICIntEnable,6);
	if(m_DataMutex.MutexGetSwap(0)){
		return (cnt - Sendbyte);
	}
	SetRegbit32(&VICIntEnClr,6);
	m_DataMutex.MutexPutSwap();
	return -1;
}
int32 CUART0::Vread(){
	int32  cnt = Recbyte;
	SetReg8(&U0IER,0x01);
	SetRegbit32(&VICIntEnable,6);	
	if(m_DataMutex.MutexGetSwap(0)){
		return cnt - Recbyte;
	}
	SetRegbit32(&VICIntEnClr,6);
	m_DataMutex.MutexPutSwap();
	return -1;
}
bool  CUART0::Vioctl(uint32 cmd,void *arg){
	switch(cmd){
		case UART_SET_BAUND: 
			//设置总线波特率
			uint32 Fdiv;
			SetReg8(&U0LCR,0x83);//DLAB = 1，允许设置波特率
			Fdiv = (Fpclk/16)/(uint32)arg;
			SetReg8(&U0DLM,Fdiv >> 8);
			SetReg8(&U0DLL,Fdiv & 0xFF);
			SetReg8(&U0LCR,0x03);//DLAB = 1
			break;
		case UART_SET_LENGTH: 
			ClrRegbit8(&U0LCR,0);
			ClrRegbit8(&U0LCR,1);
			U0LCR |= (uint8)(uint32)arg - 5;
			break;
		case UART_SET_VERIFY: 
			ClrRegbit8(&U0LCR,3);
			U0LCR |= (uint8)(uint32)arg << 3;
			break;
		default:
			return FALSE;
	}
	return TRUE;
}
bool  CUART0::Vclose(){
	SetRegbit32(&VICIntEnClr,6);
	OSCInt::PutIntVector(m_IntID);
	CPin_2103  SCL(0,0,OUT);
	CPin_2103  SDA(1,0,OUT);
	return  TRUE;
}