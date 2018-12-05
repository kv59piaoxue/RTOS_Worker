#ifndef  __CPIN_2103_HPP
#define __CPIN_2103_HPP

class   CPin_2103: public CPin{
public:
	CPin_2103(uint8 T = 0,char F = 0,bool D = IN);
   	virtual void Clear();
   	virtual void Set();
   	virtual void Init();  //初时化配置,与具体CPU关联
   	virtual bool Value();//输入引脚的值
};


#endif