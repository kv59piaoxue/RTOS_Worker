#include "config.hpp"

OSCDevMng  *OSCDevMng::DevID = NULL;
OSCDevMng::OSCDevMng(){
	BusList = NULL;
	BusCnt = 0;
	DevID = this;
}
//���豸�����з�������ߵ�����
bool		OSCDevMng::GetBusName(char *BusName,const char *DevName)
{
	uint8  i = 0;
	while(DevName[i] != '/'){
		i ++;
	} 
	strncpy(BusName,DevName,i);//copy  BusName  from DevaName
	BusName[i] = '\0';
	return TRUE;
}
OSCDev	*OSCDevMng::GetBus(const char*BusName)
{	
	bool		TF = FALSE;
	OSCTask::TaskLock();//��ֹ�ڽ�������������ʱ�����µ�����ע�ᣬ��ע����
	uint32  TempCount = BusCnt;//������������������
  	OSCDev  *Temp = BusList;//���ص�һ������
  	while(TempCount -- ){
  		if(0 == strcmp(BusName,Temp -> BusName)){//�ҵ�������
  			TF = TRUE;
  			break;
 		}
 		Temp = Temp -> NextPtr;//�ƶ�ָ��
 	}//end while
 	OSCTask::TaskUnlock();
 	if(!TF){
 	 	Temp = NULL;
 	 }
 	return Temp;//��������������
} 
//
bool		OSCDevMng::BusRegister(OSCDev* Bus)
{
	OSCTask::TaskLock();
	if(Bus -> IsInList){//�ж������Ƿ��Ѿ��ڶ�����
		OSCTask::TaskUnlock();
	 	return FALSE;
	 }
	Bus -> IsInList = TRUE;
	BusCnt++ ;//��������һ
	if(1 == BusCnt){//������Ϊ��
		BusList = Bus ;//��������
		Bus -> NextPtr = Bus ;//ָ���Լ�
		Bus -> PrevPtr = Bus;//
	}else{
		Bus -> NextPtr = BusList ;
		Bus -> PrevPtr = BusList -> PrevPtr ;
		BusList -> PrevPtr -> NextPtr = Bus ;
		BusList -> PrevPtr = Bus ;
	}
	OSCTask::TaskUnlock();
	return TRUE;

}
bool		OSCDevMng::BusUnregister(OSCDev*Bus)
{
	bool TF =  FALSE;
	OSCTask::TaskLock();
	if(Bus -> IsInList){
	Bus -> IsInList = FALSE;
	BusCnt --;
	if(0 == BusCnt){//һ������
		BusList = NULL ;
	}else{//�������ϵ�����
		if(BusList == Bus){//Ҫȡ�������ǵ�һ������
			BusList = Bus -> NextPtr ;
		}
		Bus -> NextPtr -> PrevPtr = Bus -> PrevPtr ;
		Bus -> PrevPtr -> NextPtr = Bus -> NextPtr ;
	}
	
	Bus -> NextPtr = NULL;
	Bus -> PrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;

}
	
OSCDev * OSCDevMng::open(const  char *DevName,uint32 Propety)
{
	char BusName[20];
	OSCDev  *DevP = NULL;
	GetBusName(BusName,DevName);//������������
	DevP =  GetBus(BusName);
	if(NULL != DevP){//�ж�Ӧ������
		if(!DevP -> Open(DevName,Propety)){//����豸û�д򿪳ɹ�
			
			DevP = NULL;
		}
	}
	return DevP;	
}
uint32 	OSCDevMng::write(OSCDev*DevP,const char*Buf,uint32 Size)
{
	return DevP -> Write(Buf,Size);
}
uint32 	OSCDevMng::read(OSCDev *DevP,char *Buf,uint32 Size)
{
	return	DevP -> Read(Buf,Size); 
}
bool  	OSCDevMng::close(OSCDev *DevP)
{
	return DevP -> Close();
}
bool		OSCDevMng::ioctl(OSCDev *DevP,uint32  Cmd,const void *Arg)
{
	return  DevP -> Ioctl(Cmd,Arg);
}
//END