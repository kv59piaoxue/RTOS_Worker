/*******************************************************************************************************

********************************************************************************************************/
#include "config.hpp"


//////////////////////
extern  "C"   void  MovOnTimeInt(void)
{//定时中断函数
	T0IR = 0x01;
    	VICVectAddr = 0;            //interrupt close 通知中断控制器中断结束
	OSCInt::IntEnterInt();//调用此函数，会强制开中断，允许中断嵌套
	OSCTimer::OnTimeInt();
	OSCInt::IntExitSwap();
	IOSET = LED8;
}	 
/****************************************************************************
**功能：初始化VIC中断管理器
**参数：无
**说明：系统初始化时调用
***************************************************************************/

extern  "C"    void  MovVICInit(void)
  {
    	VICIntEnClr = 0xffffffff;            // 清零所有中断 
    	VICVectAddr = 0;
    	VICIntSelect = 0;
  }

/****************************************************************************
**功能：初始化定时器
**参数：无
**说明：系统初始化时调用
***************************************************************************/


  extern   "C" void  MovTimerInit(void)
  {
    T0IR = 0xff;
    T0TC = 0;
    T0PR = 0;
    T0TCR = 0x01;
    T0MCR = 0x03;
    T0MR0 = (Fpclk / DF_TICKS_PSEC);
    VICVectAddr0 = (uint32)(MovOnTimeInt);
    VICVectCntl0 = (0x20 | 0x04);
    VICIntEnable = 1 << 4;
 }

/*********************************************************************************************************
* 函数名称：TargetInit
* 功    能：开发板初始化，进入main()之后的开发板初始化，可以被修改，但不能被删除
* 入口参数：无
* 出口参数：无
*********************************************************************************************************/
 extern "C"  void MovTargetInit(void)
{
    /* 添加自己的代码 */
     MovVICInit();
}

/*********************************************************************************************************
* 函数名称：TargetResetInit
* 功    能：进入main()之前的开发板初始化，可以被修改，但不能被删除
* 入口参数：无
* 出口参数：无
*********************************************************************************************************/
 extern "C"    void TargetResetInit(void)
{   
/* 存储器重映射 */
#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;                      // 用户RAM模式。中断向量从LPC213X内部静态SRAM重新映射。
#endif

#ifdef __DEBUG_FLASH    
    MEMMAP = 0x1;                      // 用户Flash模式。中断向量不重新映射，它位于LPC213X内部FLASH中。
#endif

#ifdef __IN_CHIP    
    MEMMAP = 0x1;                      // 用户Flash模式。中断向量不重新映射，它位于LPC213X内部FLASH中。
#endif


/* 设置系统各部分时钟，由系统硬件决定*/
    /* 设置外设时钟（VPB时钟pclk）与系统时钟（cclk）的分频比 */
     
    PLLCON = 1;                        // 设置激活但未连接PLL 
#if (Fpclk / (Fcclk / 4)) == 1         // 此值由系统硬件决定
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2         // 此值由系统硬件决定
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4         // 此值由系统硬件决定
    VPBDIV = 1;
#endif

    /* 设定PLL的乘因子M和除因子P的值 */

#if (Fcco / Fcclk) == 2
    PLLCFG = ((Fcclk / Fosc) - 1) | (0 << 5);
#endif
#if (Fcco / Fcclk) == 4
    PLLCFG = ((Fcclk / Fosc) - 1) | (1 << 5);
#endif
#if (Fcco / Fcclk) == 8
    PLLCFG = ((Fcclk / Fosc) - 1) | (2 << 5);
#endif
#if (Fcco / Fcclk) == 16
    PLLCFG = ((Fcclk / Fosc) - 1) | (3 << 5);
#endif

    PLLFEED = 0xaa;                      // 发送PLL馈送序列，执行设定PLL的动作  
    PLLFEED = 0x55;
    
    while((PLLSTAT & (1 << 10)) == 0);   // 等待PLL锁定
    
    PLLCON = 3;                          // 设置激活并连接PLL
    
    PLLFEED = 0xaa;                      // 发送PLL馈送序列，执行激活和连接动作
    PLLFEED = 0x55;
    
    /* 设置存储器加速模块 */
   
    MAMCR = 0;                           // 禁止MAM功能
     
#if Fcclk < 20000000                     // 系统时钟低于20MHz，建议设置为1
    MAMTIM = 1;
#else
#if Fcclk < 40000000                     // 系统时钟在20~40MHz之间，建议设置为2
    MAMTIM = 2;
#else
#if Fcclk < 60000000                     // 系统时钟在40~60MHz之间，建议设置为3
    MAMTIM = 3;
#else
#if Fcclk < 80000000                     // 系统时钟在60~80MHz之间，建议设置为4
    MAMTIM = 4;
#else
#if Fcclk < 100000000                    // 系统时钟在80~100MHz之间，建议设置为5
    MAMTIM = 5;
#else
#if Fcclk < 120000000                    // 系统时钟在100~120MHz之间，建议设置为6
    MAMTIM = 6;
#else
    MAMTIM = 7;                          // 系统时钟高于120MHz，建议设置为7 
#endif
#endif
#endif
#endif
#endif
#endif
    
    MAMCR = 2;                           // 使能MAM
    /* 添加自己的代码 */
//系统调试时添加的临时代码
  // IODIR |= LED1| LED2 |LED3|LED4|LED5|LED6|LED7|LED8|LED9;
    IODIR = 0xffffffff;
    IOCLR |= LED1| LED2 |LED3|LED4|LED5|LED6|LED7|LED8|LED9;
	for(uint32 i = 0;i < 2;i++){ 
		uint32  Count;
		for(Count = 0; Count < 400000;Count++);
		IOSET |= LED1| LED2 |LED3|LED4|LED5|LED6|LED7|LED8|LED9;
	 	for(Count = 0; Count < 400000;Count++);
 		IOCLR |= LED1| LED2 |LED3|LED4|LED5|LED6|LED7|LED8|LED9;
 	}
}
 
 extern   "C" OS_STK *MovTaskStkInit (void (*Task)(void *Pd), void *Pdata, OS_STK *Ptop, int PSR)
{//任务栈初始化函数，根据具体芯片架构实现
    OS_STK *stk;
    stk    = Ptop;                          /* 获取堆栈指针                                       */
                                            /* 建立任务环境，ADS1.2使用满递减堆栈       */
    *stk = (OS_STK) Task;                   /*  pc  */
    *--stk = (OS_STK) Task;                 /*  lr  */
    *--stk = 0;                             /*  r12  */
    *--stk = 0;                             /*  r11  */
    *--stk = 0;                             /*  r10  */
    *--stk = 0;                             /*  r9   */
    *--stk = 0;                             /*  r8   */
    *--stk = 0;                             /*  r7   */
    *--stk = 0;                             /*  r6   */
    *--stk = 0;                             /*  r5   */
    *--stk = 0;                             /*  r4   */
    *--stk = 0;                             /*  r3   */
    *--stk = 0;                             /*  r2   */
    *--stk = 0;                             /*  r1   */
    *--stk = (unsigned int) Pdata;          /*  r0,第一个参数使用R0传递   */
    *--stk = PSR;                   	    /*  spsr，允许 IRQ, FIQ 中断   */
    *--stk = 0;                             /*  关中断计数器OsEnterSum;    */
    return (stk);
}

extern   "C"  void  MovIntRegister(void(*IntServe)(void)/*中断服务函数*/,int8 Vector/*中断向量*/,int8 IntChannel/*通道*/){
//中断注册函数，根据不同的芯片架构实现
	(*((volatile unsigned long *)(0xFFFFF100 + (Vector << 2)))) = (uint32)IntServe; //VICIntAddr_x = IntVector;
	(*((volatile unsigned long *) (0xFFFFF200 + (Vector << 2)))) = (0x20|IntChannel);//连接中断模块
}
/*********************************************************************************************************
*                  以下为一些与系统相关的库函数的实现
*                  具体作用请ads的参考编译器与库函数手册
*                  用户可以根据自己的要求修改        
********************************************************************************************************/
#include "rt_sys.h"
#include "stdio.h"

#pragma import(__use_no_semihosting_swi)
#pragma import(__use_two_region_memory)

    extern "C"         int __rt_div0(int a)
{
    a = a;
    return 0;
}

  extern "C"              int fputc(int ch,FILE *f)
{
    ch = ch;
    f = f;
    return 0;
}

  extern "C"       int fgetc(FILE *f)
{
    f = f;
    return 0;
}


 extern "C"       int _sys_close(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

  extern "C"           int _sys_write(FILEHANDLE fh, const unsigned char * buf,
                      unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    return 0;
}
    extern "C"         int _sys_read(FILEHANDLE fh, unsigned char * buf,
                     unsigned len, int mode)
{
    fh = fh;
    buf = buf;
    len =len;
    mode = mode;
    
    return 0;
}

 extern "C"           void _ttywrch(int ch)
{
    ch = ch;
}

  extern "C"           int _sys_istty(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
   extern "C"          int _sys_seek(FILEHANDLE fh, long pos)
{
    fh = fh;
    pos = pos;
    return 0;
}
   extern "C"          int _sys_ensure(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}

   extern "C"          long _sys_flen(FILEHANDLE fh)
{
    fh = fh;
    return 0;
}
  extern "C"          int _sys_tmpnam(char * name, int sig, unsigned maxlen)
{
    name = name;
    sig = sig;
    maxlen = maxlen;
    return 0;
}

    extern "C"         void _sys_exit(int returncode)
{
    returncode = returncode;
}

 extern "C"            char *_sys_command_string(char * cmd, int len)
{
    cmd = cmd;
    len = len;
    return 0;
}



