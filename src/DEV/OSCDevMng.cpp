#include "config.hpp"

OSCDevMng  *OSCDevMng::DevID = NULL;
OSCDevMng::OSCDevMng(){
	BusList = NULL;
	BusCnt = 0;
	DevID = this;
}
//从设备名字中分离出总线的名字
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
	OSCTask::TaskLock();//防止在进行总线收索的时候有新的总线注册，或注销。
	uint32  TempCount = BusCnt;//返回链表中总线条数
  	OSCDev  *Temp = BusList;//返回第一条总线
  	while(TempCount -- ){
  		if(0 == strcmp(BusName,Temp -> BusName)){//找到了总线
  			TF = TRUE;
  			break;
 		}
 		Temp = Temp -> NextPtr;//移动指针
 	}//end while
 	OSCTask::TaskUnlock();
 	if(!TF){
 	 	Temp = NULL;
 	 }
 	return Temp;//返回总线描述符
} 
//
bool		OSCDevMng::BusRegister(OSCDev* Bus)
{
	OSCTask::TaskLock();
	if(Bus -> IsInList){//判断任务是否已经在队列中
		OSCTask::TaskUnlock();
	 	return FALSE;
	 }
	Bus -> IsInList = TRUE;
	BusCnt++ ;//任务数加一
	if(1 == BusCnt){//任务数为空
		BusList = Bus ;//挂起任务
		Bus -> NextPtr = Bus ;//指向自己
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
	if(0 == BusCnt){//一个任务
		BusList = NULL ;
	}else{//两个以上的任务
		if(BusList == Bus){//要取的任务是第一个任务
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
	GetBusName(BusName,DevName);//分离总线名字
	DevP =  GetBus(BusName);
	if(NULL != DevP){//有对应的总线
		if(!DevP -> Open(DevName,Propety)){//如果设备没有打开成功
			
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