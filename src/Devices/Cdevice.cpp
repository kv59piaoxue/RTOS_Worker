
#include "config.hpp"


Cdevice::Cdevice():Mutex(1){
	Address = 0;
	Sendbyte = 0;
	Recbyte = 0;
	Recbuf = NULL;
	Sendbuf = NULL;
	IsRW = R;
}
bool	   Cdevice::open(){
	if(Mutex.MutexGetSwap(0)){
		return Vopen();
	}
	return FALSE;
}
int32 Cdevice::write(char *buf,int32 cnt){
	Sendbuf = buf;
	Sendbyte = cnt;
	IsRW = W;
	return Vwrite();
}

int32 Cdevice::read(char *buf,int32 cnt){
	Recbuf = buf;
	Recbyte = cnt;
	IsRW = R;
	return  Vread();
}
bool  Cdevice::ioctl(uint32 cmd,void *arg ){
	switch(cmd){
		case  DEV_SET_ADDR:
			Address = (uint32)arg;
			break;
		default:
			return Vioctl(cmd,arg);
	}
	return FALSE;
}
bool  Cdevice::close(){
	bool TF = Vclose();
	Mutex.MutexPutSwap();
	return  TF;
}
CPin::CPin(uint8 T,char  F,bool D){
	Tag  = T;
	Function = F;
	Direction = D;	
}

void  CRegctl::ClrRegbit8(vbit8*reg,uint8 bit){
	*reg &= ~(1 << bit);
}
void  CRegctl::SetRegbit8(vbit8*reg,uint8 bit){
	*reg |= 1 << bit; 
}
bool  CRegctl::GetRegbit8(vbit8*reg,uint8 bit){
	if(*reg & (1 << bit)){
		return TRUE;
	}
	return FALSE;
}
void  CRegctl::ClrRegbit32(vbit32*reg,uint8 bit){
	*reg &= ~(1 << bit);
}
void  CRegctl::SetRegbit32(vbit32*reg,uint8 bit){
	*reg |= 1 << bit; 
}
bool  CRegctl::GetRegbit32(vbit32*reg,uint8 bit){
	if(*reg & (1 << bit)){
		return TRUE;
	}
	return FALSE;
}
void  CRegctl::SetReg8(vbit8*reg,uint8 data){
	*reg = data;
}
void CRegctl::SetReg32(vbit32*reg,uint32 data){
	*reg = data;
}
void  CRegctl::SetReg16(vbit16*reg,uint16  data){
	*reg = data;
}
uint32 CRegctl::GetReg32(vbit32*reg){
	return (uint32)*reg;
}
uint8   CRegctl::GetReg8(vbit8*reg){
	return (uint8)*reg;
}
uint16 CRegctl::GetReg16(vbit16*reg){
	return (uint16)*reg;
}