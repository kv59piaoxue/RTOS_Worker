///
///
#include "config.hpp"
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：13726903019
	//时间：2010年
 const  uint32 OSCInt::IntTab[16] = {1<<0,1<<1,1<<2,1<<3,1<<4,1<<5,1<<6,1<<7,1<<8,1<<9,1<<10,1<<11,
  								1<<12,1<<13,1<<14,1<<15};
uint32   OSCInt::IntVector = 0;

int8 	OSCInt::GetIntVector(){
	if(OSIntNesting > 0) return -1;
	 int8  i;
 	uint32  Vector;
 	OSCTask::TaskLock();
 	Vector = IntVector;
 	for(i = 1; i < 16; i++){
 		Vector >>= 1;//定时器中断不能用
 		if((Vector & 0x01) == 0){//申请到中断向量号 
 			IntVector |= IntTab[i];//注册中断
 			OSCTask::TaskUnlock();
 			return  i;
 		}
 		i++;	
 	}
 	OSCTask::TaskUnlock();
 	return  -1;//返回无效ID
}
///
bool	 OSCInt::PutIntVector(int8 Vector){
	if(Vector < 1 || Vector > 15)   return FALSE;
	IntLock();
	IntVector &= ~IntTab[Vector];//撤销注册
	IntUnlock();
	return TRUE;
}
////
bool	 OSCInt::IntRegister(void(*IntServe)(void),int8 Vector,int8 IntChannel){
	if(OSIntNesting > 0) return FALSE;
	if(IntServe == NULL)  return  FALSE ; 
	if(Vector == 0)  return  FALSE ;//中断0留着系统时间按服务
	MovIntRegister(IntServe,Vector,IntChannel);
	return TRUE ;
}

void  OSCInt::IntEnterInt(){
	IntEnable();//进入中断后腰开中断，允许中断嵌套
	OSEnterSum = 0;//
}
void 	OSCInt::IntEnterNoint(){
		IntDisable(); 
 		OSEnterSum = 1;
 }
 
void  OSCInt::IntExitSwap(){  
	if(OSCTask::LockCount > 0){
		OSTaskNextPtr = OSTaskCurPtr;
	}else if(OSCGuardThread::ThreadID -> IsHasMsg){//任务调度器没有被锁、系统守护线程有消息
		OSTaskCurPtr -> RemainTick = OSCTimer::CurTick;//在任务发生抢占时，记录任务剩下的时间
		OSTaskNextPtr  = OSCGuardThread::ThreadID;//切换到系统守护线程，处理消息
		OSCTimer::CurTick =0;
	}else{
		OSTaskNextPtr = OSTaskCurPtr ;//不进行调度，只要令他们相等
	}	
	OSEnterSum = 0;//允许中断
}

void   OSCInt::IntExitMedi(){
	IntDisable(); 
	OSTaskNextPtr = OSTaskCurPtr; 
	OSEnterSum = 0;
}

//END