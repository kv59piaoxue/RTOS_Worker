#include  "config.hpp"

OSCDevice::OSCDevice(){
	NextPtr = NULL;
	PrevPtr = NULL;
	IsInList  = FALSE;			//标识没有在队列中
}

void 	OSCDevice::SetDeviceName(const char *Name)
{//
	strcpy(DeviceName,Name);
}
//END