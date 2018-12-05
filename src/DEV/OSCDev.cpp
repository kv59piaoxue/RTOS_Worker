//л����

#include "config.hpp"

OSCDev::OSCDev():BusMutex(1){
	DevStat = 0;  
	//�����ڴ���
	NextPtr = NULL;//
	PrevPtr = NULL;
	IsInList = FALSE;	
	//���߳�ʼ����ʱ���豸����Ϊ��
	DeviceList = NULL;
	DeviceCnt = 0;
	CurDevice = NULL;
}
void 	OSCDev::SetBusName(const char *Name)
{
	strcpy(BusName,Name);
}
//���豸
bool   OSCDev::Open(const char *DevName,uint32 Property){
	bool  Flag = FALSE ;
	OSCDevice  *DeviceP = GetDevice(DevName);
	if(NULL != DeviceP){//�ҵ��������ϵ��豸
		if(BusMutex.MutexGetSwap(0)){//����û�б�����豸ռ��
			DevStat |= Dev_Stat_Open | Property ;//���ô򿪷�ʽ
			Flag = TRUE ;
			CurDevice = DeviceP;
			if(!DevOpen()){//�򿪵ײ��豸
				Flag = FALSE;//��ʧ��
				DevStat  = 0;//
				CurDevice = NULL;
			}
		} 
	}
	return Flag ;
}
//�ر��豸
bool    OSCDev::Close(){
	bool  Flag = FALSE ;
	if(DevClose()){//�رյײ��豸
		Flag = TRUE ;
	}
	DevStat  = 0;//�����豸״̬Ϊ�ر�
	CurDevice = NULL;
	BusMutex.MutexPutSwap();
	return Flag ;
}
//д���豸����,����ʵ��д����ֽ���
uint32  OSCDev::Write(const  char*Buf,uint32 Size){
	uint32 	Count = 0;
	if(DevStat & Dev_Stat_Open){//ȷ���豸�Ѿ���
		if((DevStat & Dev_Stat_RW)  || (DevStat & Dev_Stat_WO)){//���Է����豸
			DevStat |=  Dev_Stat_WO;
			DevStat  &= ~Dev_Stat_RO;
			Count = DevWrite(Buf,Size);
		}
	}
	return Count ;
}
		
//�����豸���ݣ�����ʵ�ʶ������ֽ���
uint32  OSCDev::Read(char*Buf,uint32 Size){
	uint32 	Count = 0;
	if(DevStat & Dev_Stat_Open){//ȷ���豸�Ѿ���
		if((DevStat & Dev_Stat_RW)  || (DevStat & Dev_Stat_RO)){//���Է����豸
			DevStat |=  Dev_Stat_RO;
			DevStat &= ~Dev_Stat_WO;
			Count = DevRead(Buf,Size);
		}
	}
	return Count ;
}		
//����״̬	
bool	  OSCDev::Ioctl(uint32  Cmd,  const void *Arg){

	if(DevStat & Dev_Stat_Open){//�豸��û�д򿪲��ܽ��в�������
		switch(Cmd)
		{
		case CMD_RO:
			DevStat = 0;
			DevStat |= Dev_Stat_Open | Dev_Stat_RO;
			break; 
		case CMD_WO:
			DevStat = 0;
			DevStat |= Dev_Stat_Open | Dev_Stat_WO;
			break; 
		case CMD_RW:
			DevStat = 0;
			DevStat |= Dev_Stat_Open | Dev_Stat_RO| Dev_Stat_WO;
			break; 
		default:
			return  DevIoctl(Cmd,Arg);
		}	
		return  TRUE;
	}
		return FALSE;
}

	//����ָ�����ֵ��豸
OSCDevice	*OSCDev::GetDevice(const char*DevName)
{
	bool		TF = FALSE;
	OSCTask::TaskLock();//��ֹ�ڽ����豸������ʱ�����µ��豸ע�ᣬ��ע����
	uint32  TempCount = DeviceCnt;//�����������豸����
  	OSCDevice  *Temp = DeviceList;//���ص�һ���豸
  	while(TempCount -- ){
  		if(0 == strcmp(DevName,Temp -> DeviceName)){//�ҵ����豸
  			TF = TRUE;
  			break;
 		}
 		Temp = Temp -> NextPtr;//�ƶ�ָ��
 	}//end while
 	OSCTask::TaskUnlock();
 	if(!TF){
 	 	Temp = NULL;
 	 }
 	return Temp;//�����豸������
}

	//���豸ע�ᵽ�����ϵĺ���
bool		OSCDev::DeviceRegister(OSCDevice *Device)
{
	OSCTask::TaskLock();
	if(Device -> IsInList){//�ж������Ƿ��Ѿ��ڶ�����
		OSCTask::TaskUnlock();
	 	return FALSE;
	 }
	Device -> IsInList = TRUE;
	DeviceCnt++ ;//��������һ
	if(1 == DeviceCnt){//������Ϊ��
		DeviceList = Device ;//��������
		Device -> NextPtr = Device ;//ָ���Լ�
		Device -> PrevPtr = Device;//
	}else{
		Device -> NextPtr = DeviceList ;
		Device -> PrevPtr = DeviceList -> PrevPtr ;
		DeviceList -> PrevPtr -> NextPtr = Device ;
		DeviceList -> PrevPtr = Device ;
	}
	OSCTask::TaskUnlockMedi();
	return TRUE;
}
	//���豸��������ж�صĺ���
bool		OSCDev::DeviceUnregister(OSCDevice *Device)
{
	bool TF =  FALSE;
	OSCTask::TaskLock();
	if(Device -> IsInList){
	Device -> IsInList = FALSE;
	DeviceCnt --;
	if(0 == DeviceCnt){//һ������
		DeviceList = NULL ;
	}else{//�������ϵ�����
		if(DeviceList == Device){//Ҫȡ�������ǵ�һ������
			DeviceList = Device -> NextPtr ;
		}
		Device -> NextPtr -> PrevPtr = Device -> PrevPtr ;
		Device -> PrevPtr -> NextPtr = Device -> NextPtr ;
	}
	
	Device -> NextPtr = NULL;
	Device -> PrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlockMedi();
	return TF;
}
//END
	
	
	
	
	