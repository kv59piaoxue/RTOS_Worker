#include  "config.hpp"

OSCDevice::OSCDevice(){
	NextPtr = NULL;
	PrevPtr = NULL;
	IsInList  = FALSE;			//��ʶû���ڶ�����
}

void 	OSCDevice::SetDeviceName(const char *Name)
{//
	strcpy(DeviceName,Name);
}
//END