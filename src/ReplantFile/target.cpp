/*******************************************************************************************************

********************************************************************************************************/
#include "config.hpp"


//////////////////////
extern  "C"   void  MovOnTimeInt(void)
{//��ʱ�жϺ���
	T0IR = 0x01;
    	VICVectAddr = 0;            //interrupt close ֪ͨ�жϿ������жϽ���
	OSCInt::IntEnterInt();//���ô˺�������ǿ�ƿ��жϣ������ж�Ƕ��
	OSCTimer::OnTimeInt();
	OSCInt::IntExitSwap();
	IOSET = LED8;
}	 
/****************************************************************************
**���ܣ���ʼ��VIC�жϹ�����
**��������
**˵����ϵͳ��ʼ��ʱ����
***************************************************************************/

extern  "C"    void  MovVICInit(void)
  {
    	VICIntEnClr = 0xffffffff;            // ���������ж� 
    	VICVectAddr = 0;
    	VICIntSelect = 0;
  }

/****************************************************************************
**���ܣ���ʼ����ʱ��
**��������
**˵����ϵͳ��ʼ��ʱ����
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
* �������ƣ�TargetInit
* ��    �ܣ��������ʼ��������main()֮��Ŀ������ʼ�������Ա��޸ģ������ܱ�ɾ��
* ��ڲ�������
* ���ڲ�������
*********************************************************************************************************/
 extern "C"  void MovTargetInit(void)
{
    /* ����Լ��Ĵ��� */
     MovVICInit();
}

/*********************************************************************************************************
* �������ƣ�TargetResetInit
* ��    �ܣ�����main()֮ǰ�Ŀ������ʼ�������Ա��޸ģ������ܱ�ɾ��
* ��ڲ�������
* ���ڲ�������
*********************************************************************************************************/
 extern "C"    void TargetResetInit(void)
{   
/* �洢����ӳ�� */
#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;                      // �û�RAMģʽ���ж�������LPC213X�ڲ���̬SRAM����ӳ�䡣
#endif

#ifdef __DEBUG_FLASH    
    MEMMAP = 0x1;                      // �û�Flashģʽ���ж�����������ӳ�䣬��λ��LPC213X�ڲ�FLASH�С�
#endif

#ifdef __IN_CHIP    
    MEMMAP = 0x1;                      // �û�Flashģʽ���ж�����������ӳ�䣬��λ��LPC213X�ڲ�FLASH�С�
#endif


/* ����ϵͳ������ʱ�ӣ���ϵͳӲ������*/
    /* ��������ʱ�ӣ�VPBʱ��pclk����ϵͳʱ�ӣ�cclk���ķ�Ƶ�� */
     
    PLLCON = 1;                        // ���ü��δ����PLL 
#if (Fpclk / (Fcclk / 4)) == 1         // ��ֵ��ϵͳӲ������
    VPBDIV = 0;
#endif
#if (Fpclk / (Fcclk / 4)) == 2         // ��ֵ��ϵͳӲ������
    VPBDIV = 2;
#endif
#if (Fpclk / (Fcclk / 4)) == 4         // ��ֵ��ϵͳӲ������
    VPBDIV = 1;
#endif

    /* �趨PLL�ĳ�����M�ͳ�����P��ֵ */

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

    PLLFEED = 0xaa;                      // ����PLL�������У�ִ���趨PLL�Ķ���  
    PLLFEED = 0x55;
    
    while((PLLSTAT & (1 << 10)) == 0);   // �ȴ�PLL����
    
    PLLCON = 3;                          // ���ü������PLL
    
    PLLFEED = 0xaa;                      // ����PLL�������У�ִ�м�������Ӷ���
    PLLFEED = 0x55;
    
    /* ���ô洢������ģ�� */
   
    MAMCR = 0;                           // ��ֹMAM����
     
#if Fcclk < 20000000                     // ϵͳʱ�ӵ���20MHz����������Ϊ1
    MAMTIM = 1;
#else
#if Fcclk < 40000000                     // ϵͳʱ����20~40MHz֮�䣬��������Ϊ2
    MAMTIM = 2;
#else
#if Fcclk < 60000000                     // ϵͳʱ����40~60MHz֮�䣬��������Ϊ3
    MAMTIM = 3;
#else
#if Fcclk < 80000000                     // ϵͳʱ����60~80MHz֮�䣬��������Ϊ4
    MAMTIM = 4;
#else
#if Fcclk < 100000000                    // ϵͳʱ����80~100MHz֮�䣬��������Ϊ5
    MAMTIM = 5;
#else
#if Fcclk < 120000000                    // ϵͳʱ����100~120MHz֮�䣬��������Ϊ6
    MAMTIM = 6;
#else
    MAMTIM = 7;                          // ϵͳʱ�Ӹ���120MHz����������Ϊ7 
#endif
#endif
#endif
#endif
#endif
#endif
    
    MAMCR = 2;                           // ʹ��MAM
    /* ����Լ��Ĵ��� */
//ϵͳ����ʱ��ӵ���ʱ����
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
{//����ջ��ʼ�����������ݾ���оƬ�ܹ�ʵ��
    OS_STK *stk;
    stk    = Ptop;                          /* ��ȡ��ջָ��                                       */
                                            /* �������񻷾���ADS1.2ʹ�����ݼ���ջ       */
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
    *--stk = (unsigned int) Pdata;          /*  r0,��һ������ʹ��R0����   */
    *--stk = PSR;                   	    /*  spsr������ IRQ, FIQ �ж�   */
    *--stk = 0;                             /*  ���жϼ�����OsEnterSum;    */
    return (stk);
}

extern   "C"  void  MovIntRegister(void(*IntServe)(void)/*�жϷ�����*/,int8 Vector/*�ж�����*/,int8 IntChannel/*ͨ��*/){
//�ж�ע�ắ�������ݲ�ͬ��оƬ�ܹ�ʵ��
	(*((volatile unsigned long *)(0xFFFFF100 + (Vector << 2)))) = (uint32)IntServe; //VICIntAddr_x = IntVector;
	(*((volatile unsigned long *) (0xFFFFF200 + (Vector << 2)))) = (0x20|IntChannel);//�����ж�ģ��
}
/*********************************************************************************************************
*                  ����ΪһЩ��ϵͳ��صĿ⺯����ʵ��
*                  ����������ads�Ĳο���������⺯���ֲ�
*                  �û����Ը����Լ���Ҫ���޸�        
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



