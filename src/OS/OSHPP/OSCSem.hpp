#ifndef  __OSCSEM_HPP
#define __OSCSEM_HPP
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
class  OSCSem :public  OSCEvent{//����ʹ�õ��ź���
	private:
	int32   	Cnt;//�ź���������
	public :
	static   OSCSem *SemID;//����ʹ�ñ���
	OSCSem(int32  Sem):OSCEvent(){//
		Cnt = Sem ;
		SemID = this;
	}
		//�޵ȴ�
		bool	 SemGetMedi();
		bool  SemPutMedi();
		//�еȴ�
		bool  SemGetSwap(uint32 Dly);
		bool  SemPutSwap();
};
#endif
