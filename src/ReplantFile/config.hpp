
/*******************************************************************************************************

********************************************************************************************************/
#ifndef __CONFIG_HPP 
#define __CONFIG_HPP
//����ʹ�õ���ʱ�궨��
#define   	LED1      1<<25
#define  	LED2      1<<12
#define 	LED3      1<<11
#define   	LED4      1<<10
#define    LED5     1<<24
#define    LED6	1<<9
#define    LED7	1<<8
#define 	LED8	1<<7
#define    LED9	1<<6

//�ɸ��ݾ����оƬ�ͱ���������ʵ�ָ�����������

typedef unsigned char  uint8;                   /* �޷���8λ���ͱ���  */
typedef signed   char  int8;                    /* �з���8λ���ͱ���  */
typedef unsigned short uint16;                  /* �޷���16λ���ͱ��� */
typedef signed   short int16;                   /* �з���16λ���ͱ��� */
typedef unsigned int   uint32;                  /* �޷���32λ���ͱ��� */
typedef signed   int   int32;                   /* �з���32λ���ͱ��� */
typedef float          fp32;                    /* �����ȸ�������32λ���ȣ� */
typedef double         fp64;                    /* ˫���ȸ�������64λ���ȣ� */

		__swi(0x00) void SwiHandle( int handle);
//���Ը��ݾ����оƬ����ʵ�ָ��ֺ깦��
#define  OS_TASK_SW()                  	SwiHandle(0)//�����л�Schedul()
#define  OS_SYS_STARTUP()	   	SwiHandle(1)//����������StartUp()

#define  OS_EN_IRQ()           		SwiHandle(2)  //���ж�intEnable()
#define  OS_DIS_IRQ()   			SwiHandle(3)//���ж�intDisable()

#define  OS_ENTER_CRITICAL()   	SwiHandle(4)//���ж�intLock()	OSEnterSum++
#define  OS_EXIT_CRITICAL()       	SwiHandle(5)//���ж�intUnlock()	OSEnterSum--


/********************************/
/*      ARM���������           */
/********************************/
//��һ������Ķ�
#include    "LPC2294.hpp"
#include   "OS.hpp"
#include    "devices.hpp"
//#include  <string.h>
/********************************/
/*     Ӧ�ó�������             */
/********************************/
//���¸�����Ҫ�Ķ�

/********************************/
/*Configuration of the example */
/*     ������Ĭ������             */
/********************************/
/* ϵͳ����, Fosc��Fcclk��Fcco��Fpclk���붨��*/
#define Fosc            11059200         // ����Ƶ�ʣ�10~25MHz��Ӧ����ʵ��ʹ��һ��
#define Fcclk           (Fosc * 4)       // ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ
#define Fcco            (Fcclk * 4)      // CCOƵ�ʣ�����ΪFcclk��2��4��8��16������ΧΪ156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1  // VPBʱ��Ƶ�ʣ�ֻ��Ϊ(Fcclk / 4)��1��2��4��

#include    "target.hpp"                   // �û�����ɾ���þ� 


#endif
