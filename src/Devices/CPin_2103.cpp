

#include "config.hpp"
CPin_2103::CPin_2103(uint8 T,char F ,bool D ):CPin(T,F,D){
	Init();
}
void CPin_2103::Init(){//初时化配置,与具体CPU关联
	//功能选择
	uint8  temp;
	vbit32 *sel;
	if(Tag >= 16){
		temp = (Tag - 16) << 2;
		sel = &PINSEL1;
	}else{
		temp = Tag << 2;
		sel = &PINSEL0;
	}
	ClrRegbit32(sel,temp);
	ClrRegbit32(sel,temp + 1);
	switch(Function){
		case  0:  break;
		case  3 : SetRegbit32(sel,temp + 1); 
		case  1:  SetRegbit32(sel,temp); break;
		case  2 :  SetRegbit32(sel,temp + 1); break;
		default : break;
	}	
	//方向选择
	if(OUT == Direction){//设置为输出
		SetRegbit32(&IODIR,Tag);
	}else{//设置为输入
		ClrRegbit32(&IODIR,Tag);
	}
}

void CPin_2103::Clear(){
	SetRegbit32(&IOCLR,Tag);
}

void CPin_2103::Set(){
	SetRegbit32(&IOSET,Tag);
}

bool CPin_2103::Value(){ //输入引脚的值
	return  GetRegbit32(&IOPIN,Tag);
}