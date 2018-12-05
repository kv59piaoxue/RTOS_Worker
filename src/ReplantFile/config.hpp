
/*******************************************************************************************************

********************************************************************************************************/
#ifndef __CONFIG_HPP 
#define __CONFIG_HPP
//调试使用的临时宏定义
#define   	LED1      1<<25
#define  	LED2      1<<12
#define 	LED3      1<<11
#define   	LED4      1<<10
#define    LED5     1<<24
#define    LED6	1<<9
#define    LED7	1<<8
#define 	LED8	1<<7
#define    LED9	1<<6

//可根据具体的芯片和编译器特性实现各种数据类型

typedef unsigned char  uint8;                   /* 无符号8位整型变量  */
typedef signed   char  int8;                    /* 有符号8位整型变量  */
typedef unsigned short uint16;                  /* 无符号16位整型变量 */
typedef signed   short int16;                   /* 有符号16位整型变量 */
typedef unsigned int   uint32;                  /* 无符号32位整型变量 */
typedef signed   int   int32;                   /* 有符号32位整型变量 */
typedef float          fp32;                    /* 单精度浮点数（32位长度） */
typedef double         fp64;                    /* 双精度浮点数（64位长度） */

		__swi(0x00) void SwiHandle( int handle);
//可以根据具体的芯片特性实现各种宏功能
#define  OS_TASK_SW()                  	SwiHandle(0)//任务切换Schedul()
#define  OS_SYS_STARTUP()	   	SwiHandle(1)//开启多任务StartUp()

#define  OS_EN_IRQ()           		SwiHandle(2)  //开中断intEnable()
#define  OS_DIS_IRQ()   			SwiHandle(3)//关中断intDisable()

#define  OS_ENTER_CRITICAL()   	SwiHandle(4)//关中断intLock()	OSEnterSum++
#define  OS_EXIT_CRITICAL()       	SwiHandle(5)//开中断intUnlock()	OSEnterSum--


/********************************/
/*      ARM的特殊代码           */
/********************************/
//这一段无需改动
#include    "LPC2294.hpp"
#include   "OS.hpp"
#include    "devices.hpp"
//#include  <string.h>
/********************************/
/*     应用程序配置             */
/********************************/
//以下根据需要改动

/********************************/
/*Configuration of the example */
/*     开发板默认配置             */
/********************************/
/* 系统设置, Fosc、Fcclk、Fcco、Fpclk必须定义*/
#define Fosc            11059200         // 晶振频率，10~25MHz，应当与实际使用一致
#define Fcclk           (Fosc * 4)       // 系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ
#define Fcco            (Fcclk * 4)      // CCO频率，必须为Fcclk的2、4、8、16倍，范围为156MHz~320MHz
#define Fpclk           (Fcclk / 4) * 1  // VPB时钟频率，只能为(Fcclk / 4)的1、2、4倍

#include    "target.hpp"                   // 用户不能删除该句 


#endif
