//л����

#include "config.hpp"

OSCDevBuf::OSCDevBuf():MutexIn(0),MutexOut(0){
	RecApplySize = 0;
	RecIn = RecOut = 0;
	TxIn = TxOut = 0;
	IntID = -1;
}
///////////////////////////////////////////////////////////////////////
bool 	OSCDevBuf::DevOpen(){
	//OSCMutex  Temp(0);
	//MutexIn = Temp;//��յȴ�����
	//MutexOut = Temp;
	RecApplySize = 0;
	RecIn = RecOut = 0;
	TxIn = TxOut = 0;
	IntID = -1;
	//�����жϺţ�ע���жϣ������ж϶�����
	if((IntID = OSCInt::GetIntVector()) == -1)    return  FALSE;//��������жϺ�ʧ��
	IntRegister(IntID);//����ɹ����򽻸���������
	return  BufOpen();
}
////////////////////////////////////////////////////////////////////////
bool		OSCDevBuf::DevClose(){
	OSCInt::PutIntVector(IntID);//����Դ�黹ϵͳ,ж���ж�
	RecApplySize = 0;
	RecIn = RecOut = 0;
	TxIn = TxOut = 0;
	IntID = -1;
	return BufClose();
}

////////////////////////////////////////////////////////////////////////
uint32 	OSCDevBuf::DevWrite(const  char*Buf,uint32 Size){
	uint32  WriteSize = 0;
	while(Size)
	{//������Ҫ���͵����ݣ�������д���������������жϷ���
		while(!IsTxBufFull() && Size){
			PutOneToTxBuf(&Buf[WriteSize]);
			Size = Size - 1;
			WriteSize ++ ;
		}//end
		IntStartUp();//���ú��������ж�,��Ϊ�ж�ÿ����һ�����ݻ��Զ��ر��ж�
		MutexOut.MutexGetSwap(0);//��������ȴ����ݴ������	
		IntShutDown();//���ж�
	}
	return  WriteSize;	
}
////////////////////////////////////////////////////////////////////

uint32  OSCDevBuf::DevRead(char*Buf,uint32 Size){
	uint32  ReadSize = 0;
	while( Size && !IsRecBufEmpty())
	{//����������������ݣ����ȡ������ʣ�µ�����
		Buf[ReadSize]  = GetOneOfRecBuf();
		Size  = Size -1;
		ReadSize ++ ;
	}
	while(Size)
	{//��������,ֱ����ʱ������������
		RecApplySize = Size;//��¼������������ݴ�С
		IntStartUp();//���ú��������ж�
		MutexIn.MutexGetSwap(0);//���������ȴ�����
		IntShutDown();//���豸�ж�
		while( Size && !IsRecBufEmpty())
		{//����������������ݣ����ȡ������ʣ�µ�����
			Buf[ReadSize]  = GetOneOfRecBuf();
			Size  = Size -1;
			ReadSize ++ ;
		}
	}	
	return  ReadSize;			
}
////////////////////////

bool  OSCDevBuf::IntGetTxBufChar(char*Ch){//���ͻ�����ȡһ���ֽ�
	bool  TF = FALSE;
	if(!IsTxBufEmpty()){//����Ч������
		*Ch = GetOneOfTxBuf();
		if(IsTxBufEmpty()){//���������û�������ݣ������������д����
			STMsg  Msg(OSCGuardThread::ThreadID,TM_MUTEXPUTMEDI,(WPARAM)(&(OSCDevBuf::MutexOut)));
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
		}
		TF = TRUE;
	}
	return TF;
}
/////////////
bool  OSCDevBuf::IntPutRecBufChar(char  Ch){//�жϷ���һ���ֽ�
	bool 	TF = FALSE;
	if(!IsRecBufFull()){//�����������Է�����
		PutOneToRecBuf(&Ch);
		RecApplySize = RecApplySize - 1;
		TF = TRUE;
	}
	if( !RecApplySize || IsRecBufFull()){//����������е����ݴﵽ����Ҫ���������
		STMsg  Msg(OSCGuardThread::ThreadID,TM_MUTEXPUTMEDI,(WPARAM)(&OSCDevBuf::MutexIn));
		OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
	}
	return  TF;	
}
char		OSCDevBuf ::GetOneOfRecBuf(){
		char  Ch;
		Ch = RecBuf[RecOut];
		RecOut = (RecOut + 1)%Dev_RecBuf_Size;
		return Ch;
}
bool		OSCDevBuf ::PutOneToRecBuf(const char *Ch){
		RecBuf[RecIn]  = *Ch;
		RecIn = (RecIn +1)%Dev_RecBuf_Size;//����ƶ�ָ�룬��Ч�ʽϵ�,����ࣩ
		return  TRUE;
}
char		OSCDevBuf ::GetOneOfTxBuf(){
		char  Ch ;
		Ch = TxBuf[TxOut];
		TxOut = (TxOut + 1)%Dev_TxBuf_Size ;
		return  Ch;
}
bool		OSCDevBuf ::PutOneToTxBuf(const char *Ch){
		TxBuf[TxIn]  = *Ch;
		TxIn = (TxIn +1)%Dev_TxBuf_Size;//����ƶ�ָ�룬��Ч�ʽϵ�,����ࣩ
		return TRUE;
}

//END
////////////////////////////////////////////////////////////////////////
bool		OSCDevBuf::DevIoctl(uint32  Cmd,const void *Arg){
	switch(Cmd)
	{
		case CMD_FLUSHRECBUF:
				RecIn = 0;
				RecOut = 0;
				break;
		case CMD_FLUSHTXBUF:
				TxIn = 0;
				TxOut = 0;
				break;
		default:		
			return  BufIoctl(Cmd,Arg);
	} 
	return FALSE;
}