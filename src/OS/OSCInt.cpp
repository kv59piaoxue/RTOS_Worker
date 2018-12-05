///
///
#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
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
 		Vector >>= 1;//��ʱ���жϲ�����
 		if((Vector & 0x01) == 0){//���뵽�ж������� 
 			IntVector |= IntTab[i];//ע���ж�
 			OSCTask::TaskUnlock();
 			return  i;
 		}
 		i++;	
 	}
 	OSCTask::TaskUnlock();
 	return  -1;//������ЧID
}
///
bool	 OSCInt::PutIntVector(int8 Vector){
	if(Vector < 1 || Vector > 15)   return FALSE;
	IntLock();
	IntVector &= ~IntTab[Vector];//����ע��
	IntUnlock();
	return TRUE;
}
////
bool	 OSCInt::IntRegister(void(*IntServe)(void),int8 Vector,int8 IntChannel){
	if(OSIntNesting > 0) return FALSE;
	if(IntServe == NULL)  return  FALSE ; 
	if(Vector == 0)  return  FALSE ;//�ж�0����ϵͳʱ�䰴����
	MovIntRegister(IntServe,Vector,IntChannel);
	return TRUE ;
}

void  OSCInt::IntEnterInt(){
	IntEnable();//�����жϺ������жϣ������ж�Ƕ��
	OSEnterSum = 0;//
}
void 	OSCInt::IntEnterNoint(){
		IntDisable(); 
 		OSEnterSum = 1;
 }
 
void  OSCInt::IntExitSwap(){  
	if(OSCTask::LockCount > 0){
		OSTaskNextPtr = OSTaskCurPtr;
	}else if(OSCGuardThread::ThreadID -> IsHasMsg){//���������û�б�����ϵͳ�ػ��߳�����Ϣ
		OSTaskCurPtr -> RemainTick = OSCTimer::CurTick;//����������ռʱ����¼����ʣ�µ�ʱ��
		OSTaskNextPtr  = OSCGuardThread::ThreadID;//�л���ϵͳ�ػ��̣߳�������Ϣ
		OSCTimer::CurTick =0;
	}else{
		OSTaskNextPtr = OSTaskCurPtr ;//�����е��ȣ�ֻҪ���������
	}	
	OSEnterSum = 0;//�����ж�
}

void   OSCInt::IntExitMedi(){
	IntDisable(); 
	OSTaskNextPtr = OSTaskCurPtr; 
	OSEnterSum = 0;
}

//END