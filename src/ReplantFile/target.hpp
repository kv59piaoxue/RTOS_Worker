
/*******************************************************************************************************

********************************************************************************************************/
#ifndef __TARGET_HPP 
#define __TARGET_HPP 

//OS
extern  "C"    void  MovTargetInit();//进入主函数后的目标板初始化
//堆栈初始化
extern   "C"  OS_STK *MovTaskStkInit (void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop, int PSR);
//中断注册
extern   "C"  void  MovIntRegister(void(*IntServe)(void),int8 Vector,int8 IntChannel);
//定时器启动函数
 extern   "C"  void  MovTimerInit();
//USR
//定时器初始化，为系统提供时钟
extern  "C"    void  MovOnTimeInt();
//ARM VIC 初始化函数
extern  "C"    void  MovVICInit();

#endif
