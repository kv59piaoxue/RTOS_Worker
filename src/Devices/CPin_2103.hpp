#ifndef  __CPIN_2103_HPP
#define __CPIN_2103_HPP

class   CPin_2103: public CPin{
public:
	CPin_2103(uint8 T = 0,char F = 0,bool D = IN);
   	virtual void Clear();
   	virtual void Set();
   	virtual void Init();  //��ʱ������,�����CPU����
   	virtual bool Value();//�������ŵ�ֵ
};


#endif