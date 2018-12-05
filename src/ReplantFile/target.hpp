
/*******************************************************************************************************

********************************************************************************************************/
#ifndef __TARGET_HPP 
#define __TARGET_HPP 

//OS
extern  "C"    void  MovTargetInit();//�������������Ŀ����ʼ��
//��ջ��ʼ��
extern   "C"  OS_STK *MovTaskStkInit (void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop, int PSR);
//�ж�ע��
extern   "C"  void  MovIntRegister(void(*IntServe)(void),int8 Vector,int8 IntChannel);
//��ʱ����������
 extern   "C"  void  MovTimerInit();
//USR
//��ʱ����ʼ����Ϊϵͳ�ṩʱ��
extern  "C"    void  MovOnTimeInt();
//ARM VIC ��ʼ������
extern  "C"    void  MovVICInit();

#endif
