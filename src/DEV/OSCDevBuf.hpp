#ifndef  __OSCDEVBUF_HPP
#define __OSCDEVBUF_HPP


class OSCDevBuf : public OSCDev{
private:
	OSCMutex    MutexIn;//数据输入缓冲区等待队列，任务输入中断输出
	OSCMutex    MutexOut;//数据输出缓冲区等待队列，任务输出中断输入
	int8	  IntID;
	int32  RecIn,RecOut;
	char  RecBuf[Dev_RecBuf_Size];
	int32  TxIn,TxOut;
	char  TxBuf[Dev_TxBuf_Size];

	bool		IsRecBufEmpty(){ return  RecIn == RecOut;}
	bool		IsRecBufFull(){return RecOut == (RecIn + 1)%Dev_RecBuf_Size;}
	bool		IsTxBufEmpty() { return  TxIn == TxOut ;}
	bool		IsTxBufFull(){return TxOut == (TxIn + 1)%Dev_TxBuf_Size;}	
	char		GetOneOfRecBuf();
	bool		PutOneToRecBuf(const char *Ch);
	char		GetOneOfTxBuf();
	bool		PutOneToTxBuf(const char *Ch);
 protected:
 //定义新的函数，必须重载
	virtual  	bool   IntStartUp() = 0;//开启中断模式函数
	virtual 	bool	  IntShutDown() = 0;//关闭中断
	virtual 	bool   BufOpen() = 0;
	virtual 	bool   BufClose() = 0;
	virtual  	bool   BufIoctl(uint32 Cmd, const  void *Arg) = 0;//设置函数
	virtual 	bool   IntRegister(int8  IntID) = 0;//中断注册函数	
 protected:	
 	uint32  RecApplySize;//申请的数量(接收数据时，中断函数需要访问的变量
 //重载继承而来的函数
  	virtual  bool  DevOpen() ;
  	virtual  uint32   DevWrite(const  char*Buf,uint32 Size) ;
  	virtual  uint32   DevRead(char*Buf,uint32 Size) ;
  	virtual  bool    DevIoctl(uint32  Cmd,const void *Arg) ;
  	virtual  bool  DevClose(); 
  	//
  	bool 		IntGetTxBufChar(char *Ch);//中断函数调用，从发送缓冲区中取出一字节
	bool			IntPutRecBufChar(char Ch);//中断函数调用，存一字节到接收缓冲区
  public:
	OSCDevBuf();
};

#endif  
//END