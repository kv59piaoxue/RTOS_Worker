#include "config.hpp"

CDevUART0 *CDevUART0::DevID  = NULL;
CDevUART0::CDevUART0(const  char *Name){
	 	DevID = this;
	 	SetBusName(Name);
	 	OSCDevMng::DevID -> BusRegister(this);//ע������
}
bool   CDevUART0::IntStartUp(){//�����ж�ģʽ����
		char  Con;
		if(DevStat & Dev_Stat_RW)	   	Con = 0x03;//������ա������ж�
		else  if(DevStat & Dev_Stat_RO)  		Con = 0x01;//�������
		else	if(DevStat & Dev_Stat_WO)  	Con = 0x02;//�����ж�	
		U0IER  = Con ;//
		if(CDevUART0::DevID -> IntGetTxBufChar(&Con))
	 		U0THR = Con;//
	 	VICIntEnable =  1 << 6;//ʹ���ж�
  		return  TRUE;
}
bool	  CDevUART0::IntShutDown(){//�ر��ж�
		U0IER  &= ~0x03 ;//
		VICIntEnClr =  1 << 6;
 		return  TRUE;
}
bool   CDevUART0::BufOpen(){
		uint32 Fdiv;
		if(CurDevice -> Open()){//�������ϵ��豸
			//�����߹���
			CurDevice -> SetPin(PINSEL0 & 0x0000000F);//��������״̬
			PINSEL0	 = ((PINSEL0&0xFFFFFFF0)|0x00000005);//���������óɴ��ڹ���
			//�������߲�����
			U0LCR = 0x83;//DLAB = 1���������ò�����
			Fdiv = (Fpclk/16)/CurDevice -> GetBaud();
			U0DLM  = Fdiv >> 8;
			U0DLL = Fdiv & 0xFF;
			U0LCR = 0x03;//DLAB = 1
			return TRUE;
		}
	 	return  FALSE;
}
bool   CDevUART0::BufClose(){
		PINSEL0  =  0;//��ԭ����
		CurDevice -> Close();
 		return  TRUE;
}
bool   CDevUART0::IntRegister(int8  IntID){//�ж�ע�ắ��
	OSCInt::IntRegister(Dri2103_Uart0_IntServe,IntID,6);
 	return  TRUE;
}	

bool   CDevUART0::BufIoctl(uint32 Cmd, const  void *Arg){//���ú���
		switch(Cmd)
		{
			case  CMD_CHANGEBAUD:	
				CurDevice ->SetBaud( (uint32)Arg);	
				break;
			default :		
				return FALSE;
		}
 		return  TRUE;
}
void  Dri2103_Uart0_IntServe()//�жϷ�������ַ
{
	 	uint8  IIR;
	 	char   Ch;
	 	VICIntEnClr =  1 << 6;//�豸����
	 	VICVectAddr = 0; 	 
	 	OSCInt::IntEnable();
	 	while((0x01&(IIR = U0IIR)) == 0)
	 	{
	 		switch(IIR &0x0e)
	 		{
	 			case 0x02: //THRE
	 					if(CDevUART0::DevID -> IntGetTxBufChar(&Ch))
	 						U0THR = Ch;//
	 				break;
	 			case 0x04://RDA
	 					Ch = U0RBR;//
	 					//CDevUART0::DevID -> IntPutRecBufChar(Ch);
	 				break;
	 			case 0x0C://CTI ��ʱ�жϣ�����һ��
	 					U0IER = U0IER & (~0x01);//��ֹ���ܼ��ַ��ж�
	 					break;
	 			case  0x06:	//��ż����
	 					break;
	 			default: 
	 					break;
	 		}//end  switch
	 	}//end  while	
	 	OSCInt::IntDisable();	
	 	VICIntEnable =  1 << 6;//ʹ���ж�
	 	OSCInt::IntExitSwap();	 	
}
//END 

