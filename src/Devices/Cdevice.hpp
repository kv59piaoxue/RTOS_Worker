
#ifndef __CDEVICE__HPP
#define __CDEVICE__HPP

#define  R  	TRUE
#define  W  	FALSE
#define  IN     FALSE
#define  OUT  TRUE

#define  DEV_SET_ADDR    0
#define   DEV_BASE         100
class CRegctl{	
protected:
	CRegctl(){}
	static  void  	ClrRegbit32(vbit32*reg,uint8 bit);
	static  void  	SetRegbit32(vbit32*reg,uint8 bit);
	static  bool  	GetRegbit32(vbit32*reg,uint8 bit);
	static  void  	ClrRegbit8(vbit8*reg,uint8 bit);
	static  void  	SetRegbit8(vbit8*reg,uint8 bit);
	static  bool  	GetRegbit8(vbit8*reg,uint8 bit);
	static  void  	SetReg8(vbit8*reg,uint8  data);
	static  void 	SetReg32(vbit32*reg,uint32 data);
	static  uint32 	GetReg32(vbit32*reg);
	static  uint8   	GetReg8(vbit8*reg);
	static  void  	SetReg16(vbit16*reg,uint16 data);
	static  uint16 	GetReg16(vbit16*reg);
};

class  Cdevice:public CRegctl{
private:
	OSCMutex   Mutex;
protected:
//��Ϊ��Щ��������Ҫ���жϷ�����ʹ�ã�
//�������Ϊ����
	uint8 Address;//�������ݵ�������ַ
	int32  Sendbyte;//���͵��ֽ���
	int32  Recbyte;//���յ��ֽ���
	char *Recbuf;//���ջ�������ַ
	char *Sendbuf;//���ͻ�������ַ
	bool  IsRW;
	int8 m_IntID;
	Cdevice();
	//Vfunction
	virtual  bool  Vopen() = 0;
	virtual  int32 Vwrite() = 0;
	virtual  int32 Vread() = 0;
	virtual  bool  Vioctl(uint32 cmd,void *arg ) = 0;
	virtual  bool  Vclose() = 0;	
public:
	bool  open();
	int32 write(char *buf,int32 cnt);
	int32 read(char *buf,int32 cnt);
	bool  ioctl(uint32 cmd,void *arg = NULL);
	bool  close();	
};

class  CPin:public CRegctl{
public:
   	uint8 Tag;//���ű�ʶ,��P20 Ϊ20
   	char   Function;//���ŵĹ��ܺţ�0�Ź���һ��ΪGPIO
   	bool   Direction;//�������ΪGPIO,�ó�Ա������ʾ���ŷ���
   	CPin(uint8 T = 0,char F = 0,bool D = 0);
   	virtual void Clear() = 0;
   	virtual void Set() = 0 ;
   	virtual void Init() = 0;  //��ʱ������,�����CPU����
   	virtual bool Value() = 0;//�������ŵ�ֵ
};
#endif
