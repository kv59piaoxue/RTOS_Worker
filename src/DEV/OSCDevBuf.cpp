//谢国经

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
	//MutexIn = Temp;//清空等待队列
	//MutexOut = Temp;
	RecApplySize = 0;
	RecIn = RecOut = 0;
	TxIn = TxOut = 0;
	IntID = -1;
	//申请中断号，注册中断，开启中断读数据
	if((IntID = OSCInt::GetIntVector()) == -1)    return  FALSE;//如果申请中断号失败
	IntRegister(IntID);//申请成功，则交给驱动处理
	return  BufOpen();
}
////////////////////////////////////////////////////////////////////////
bool		OSCDevBuf::DevClose(){
	OSCInt::PutIntVector(IntID);//把资源归还系统,卸载中断
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
	{//当还有要发送的数据，把数据写进缓冲区，开启中断发送
		while(!IsTxBufFull() && Size){
			PutOneToTxBuf(&Buf[WriteSize]);
			Size = Size - 1;
			WriteSize ++ ;
		}//end
		IntStartUp();//调用函数开启中断,因为中断每接收一次数据会自动关闭中断
		MutexOut.MutexGetSwap(0);//挂起任务等待数据传送完毕	
		IntShutDown();//关中断
	}
	return  WriteSize;	
}
////////////////////////////////////////////////////////////////////

uint32  OSCDevBuf::DevRead(char*Buf,uint32 Size){
	uint32  ReadSize = 0;
	while( Size && !IsRecBufEmpty())
	{//如果缓冲区还有数据，则读取缓冲区剩下的数据
		Buf[ReadSize]  = GetOneOfRecBuf();
		Size  = Size -1;
		ReadSize ++ ;
	}
	while(Size)
	{//挂起自身,直到超时或者数据收满
		RecApplySize = Size;//记录任务请求的数据大小
		IntStartUp();//调用函数开启中断
		MutexIn.MutexGetSwap(0);//挂起自身，等待数据
		IntShutDown();//关设备中断
		while( Size && !IsRecBufEmpty())
		{//如果缓冲区还有数据，则读取缓冲区剩下的数据
			Buf[ReadSize]  = GetOneOfRecBuf();
			Size  = Size -1;
			ReadSize ++ ;
		}
	}	
	return  ReadSize;			
}
////////////////////////

bool  OSCDevBuf::IntGetTxBufChar(char*Ch){//发送缓冲区取一个字节
	bool  TF = FALSE;
	if(!IsTxBufEmpty()){//有有效的数据
		*Ch = GetOneOfTxBuf();
		if(IsTxBufEmpty()){//如果缓冲区没有了数据，唤醒任务继续写数据
			STMsg  Msg(OSCGuardThread::ThreadID,TM_MUTEXPUTMEDI,(WPARAM)(&(OSCDevBuf::MutexOut)));
			OSCGuardThread::ThreadID -> IntMsgSendToTask(&Msg);
		}
		TF = TRUE;
	}
	return TF;
}
/////////////
bool  OSCDevBuf::IntPutRecBufChar(char  Ch){//中断发送一个字节
	bool 	TF = FALSE;
	if(!IsRecBufFull()){//缓冲区还可以放数据
		PutOneToRecBuf(&Ch);
		RecApplySize = RecApplySize - 1;
		TF = TRUE;
	}
	if( !RecApplySize || IsRecBufFull()){//如果缓冲区中的数据达到任务要求或者满了
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
		RecIn = (RecIn +1)%Dev_RecBuf_Size;//向后移动指针，（效率较低,但简洁）
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
		TxIn = (TxIn +1)%Dev_TxBuf_Size;//向后移动指针，（效率较低,但简洁）
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