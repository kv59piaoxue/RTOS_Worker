#ifndef   __OSCPEND_HPP
#define   __OSCPEND_HPP
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
class OSCTask;
class  OSCPend{
	protected:
		int32   Count;
		OSCTask  *PendPtr;
		OSCPend(){
			Count = 0 ;
			PendPtr = NULL ;
		}//
	public :
		bool   	IsEmpty(){return  0 == Count;}
		uint32     GetCount(){ return Count ;}//返回链条中任务的个数
		OSCTask*GetFirst(){return PendPtr ;}
};
class OSCPendThreadTick : public OSCPend{
	public:
		OSCPendThreadTick ():OSCPend(){
		}
		bool 	ListIn(OSCTask *TaskPtr) ;//
		bool		ListOut(OSCTask *TaskPtr) ;//
};

class OSCPendRdy : public OSCPend{
	public:
		OSCPendRdy():OSCPend(){
		}
		bool 	ListIn(OSCTask *TaskPtr) ;//
		bool		ListOut(OSCTask *TaskPtr) ;//
};

class OSCPendTime : public OSCPend{
	public:
		OSCPendTime():OSCPend(){}
		bool 	ListIn(OSCTask *TaskPtr) ;//
		bool		ListOut(OSCTask *TaskPtr) ;//
};

class OSCPendEvent: public OSCPend{
	public:
		OSCPendEvent():OSCPend(){}
		bool 	ListIn(OSCTask *TaskPtr) ;//
		bool		ListOut(OSCTask *TaskPtr) ;//
};

class		OSCPendPd : public	OSCPend{
	public:
		OSCPendPd():OSCPend(){
		}
		bool 	ListIn(OSCTask *TaskPtr) ;//
		bool		ListOut(OSCTask *TaskPtr) ;//
};

#endif
//END