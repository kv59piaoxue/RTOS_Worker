#ifndef  __OSCDEVBUF_HPP
#define __OSCDEVBUF_HPP


class OSCDevBuf : public OSCDev{
private:
	OSCMutex    MutexIn;//�������뻺�����ȴ����У����������ж����
	OSCMutex    MutexOut;//��������������ȴ����У���������ж�����
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
 //�����µĺ�������������
	virtual  	bool   IntStartUp() = 0;//�����ж�ģʽ����
	virtual 	bool	  IntShutDown() = 0;//�ر��ж�
	virtual 	bool   BufOpen() = 0;
	virtual 	bool   BufClose() = 0;
	virtual  	bool   BufIoctl(uint32 Cmd, const  void *Arg) = 0;//���ú���
	virtual 	bool   IntRegister(int8  IntID) = 0;//�ж�ע�ắ��	
 protected:	
 	uint32  RecApplySize;//���������(��������ʱ���жϺ�����Ҫ���ʵı���
 //���ؼ̳ж����ĺ���
  	virtual  bool  DevOpen() ;
  	virtual  uint32   DevWrite(const  char*Buf,uint32 Size) ;
  	virtual  uint32   DevRead(char*Buf,uint32 Size) ;
  	virtual  bool    DevIoctl(uint32  Cmd,const void *Arg) ;
  	virtual  bool  DevClose(); 
  	//
  	bool 		IntGetTxBufChar(char *Ch);//�жϺ������ã��ӷ��ͻ�������ȡ��һ�ֽ�
	bool			IntPutRecBufChar(char Ch);//�жϺ������ã���һ�ֽڵ����ջ�����
  public:
	OSCDevBuf();
};

#endif  
//END