//谢国经

#include "config.hpp"

OSCDev::OSCDev():BusMutex(1){
	DevStat = 0;  
	//总线在创建
	NextPtr = NULL;//
	PrevPtr = NULL;
	IsInList = FALSE;	
	//总线初始化的时候，设备链表为空
	DeviceList = NULL;
	DeviceCnt = 0;
	CurDevice = NULL;
}
void 	OSCDev::SetBusName(const char *Name)
{
	strcpy(BusName,Name);
}
//打开设备
bool   OSCDev::Open(const char *DevName,uint32 Property){
	bool  Flag = FALSE ;
	OSCDevice  *DeviceP = GetDevice(DevName);
	if(NULL != DeviceP){//找到在总线上的设备
		if(BusMutex.MutexGetSwap(0)){//总线没有被别的设备占用
			DevStat |= Dev_Stat_Open | Property ;//设置打开方式
			Flag = TRUE ;
			CurDevice = DeviceP;
			if(!DevOpen()){//打开底层设备
				Flag = FALSE;//打开失败
				DevStat  = 0;//
				CurDevice = NULL;
			}
		} 
	}
	return Flag ;
}
//关闭设备
bool    OSCDev::Close(){
	bool  Flag = FALSE ;
	if(DevClose()){//关闭底层设备
		Flag = TRUE ;
	}
	DevStat  = 0;//设置设备状态为关闭
	CurDevice = NULL;
	BusMutex.MutexPutSwap();
	return Flag ;
}
//写入设备数据,返回实际写入的字节数
uint32  OSCDev::Write(const  char*Buf,uint32 Size){
	uint32 	Count = 0;
	if(DevStat & Dev_Stat_Open){//确认设备已经打开
		if((DevStat & Dev_Stat_RW)  || (DevStat & Dev_Stat_WO)){//可以访问设备
			DevStat |=  Dev_Stat_WO;
			DevStat  &= ~Dev_Stat_RO;
			Count = DevWrite(Buf,Size);
		}
	}
	return Count ;
}
		
//读出设备数据，返回实际读出的字节数
uint32  OSCDev::Read(char*Buf,uint32 Size){
	uint32 	Count = 0;
	if(DevStat & Dev_Stat_Open){//确认设备已经打开
		if((DevStat & Dev_Stat_RW)  || (DevStat & Dev_Stat_RO)){//可以访问设备
			DevStat |=  Dev_Stat_RO;
			DevStat &= ~Dev_Stat_WO;
			Count = DevRead(Buf,Size);
		}
	}
	return Count ;
}		
//设置状态	
bool	  OSCDev::Ioctl(uint32  Cmd,  const void *Arg){

	if(DevStat & Dev_Stat_Open){//设备还没有打开不能进行参数设置
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

	//查找指定名字的设备
OSCDevice	*OSCDev::GetDevice(const char*DevName)
{
	bool		TF = FALSE;
	OSCTask::TaskLock();//防止在进行设备收索的时候有新的设备注册，或注销。
	uint32  TempCount = DeviceCnt;//返回总线中设备条数
  	OSCDevice  *Temp = DeviceList;//返回第一个设备
  	while(TempCount -- ){
  		if(0 == strcmp(DevName,Temp -> DeviceName)){//找到了设备
  			TF = TRUE;
  			break;
 		}
 		Temp = Temp -> NextPtr;//移动指针
 	}//end while
 	OSCTask::TaskUnlock();
 	if(!TF){
 	 	Temp = NULL;
 	 }
 	return Temp;//返回设备描述符
}

	//将设备注册到总线上的函数
bool		OSCDev::DeviceRegister(OSCDevice *Device)
{
	OSCTask::TaskLock();
	if(Device -> IsInList){//判断任务是否已经在队列中
		OSCTask::TaskUnlock();
	 	return FALSE;
	 }
	Device -> IsInList = TRUE;
	DeviceCnt++ ;//任务数加一
	if(1 == DeviceCnt){//任务数为空
		DeviceList = Device ;//挂起任务
		Device -> NextPtr = Device ;//指向自己
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
	//将设备从总线上卸载的函数
bool		OSCDev::DeviceUnregister(OSCDevice *Device)
{
	bool TF =  FALSE;
	OSCTask::TaskLock();
	if(Device -> IsInList){
	Device -> IsInList = FALSE;
	DeviceCnt --;
	if(0 == DeviceCnt){//一个任务
		DeviceList = NULL ;
	}else{//两个以上的任务
		if(DeviceList == Device){//要取的任务是第一个任务
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
	
	
	
	
	