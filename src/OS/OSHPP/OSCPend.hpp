#ifndef   __OSCPEND_HPP
#define   __OSCPEND_HPP
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
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
		uint32     GetCount(){ return Count ;}//��������������ĸ���
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