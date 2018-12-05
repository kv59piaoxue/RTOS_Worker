#ifndef  __OSCSEM_HPP
#define __OSCSEM_HPP
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
class  OSCSem :public  OSCEvent{//任务级使用的信号量
	private:
	int32   	Cnt;//信号量计数器
	public :
	static   OSCSem *SemID;//调试使用变量
	OSCSem(int32  Sem):OSCEvent(){//
		Cnt = Sem ;
		SemID = this;
	}
		//无等待
		bool	 SemGetMedi();
		bool  SemPutMedi();
		//有等待
		bool  SemGetSwap(uint32 Dly);
		bool  SemPutSwap();
};
#endif
