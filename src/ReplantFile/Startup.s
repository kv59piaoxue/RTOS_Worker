
;//系统调试时使用的临时定义
LED1  		EQU	1<<25
LED2    		EQU  	1<<12
LED3    		EQU  	1<<11
LED4     		EQU 	1<<10
IOSET        EQU	0xE0028004    ;  /* lpc210x only */
IODIR         EQU 	0xE0028008    ;  /* lpc210x only */
IOCLR        EQU 	0xE002800C    ;  /* lpc210x only */
;/////////////////////////////////////////////////////////////////////////////////
;/*定义堆栈的大小*/
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         9*12 ;//每次IRQ中断需要入栈9个寄存器，因此允许12层中断嵌套
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0

NoIRQ             EQU     0x80                ;// Bit7，I位

NoFIQ		EQU	0x40                ;// Bit6，F位

USR32Mode   EQU 0x10                ;// M[4:0]=10000，用户模式
SVC32Mode   EQU 0x13                ;// M[4:0]=10011，管理模式
SYS32Mode   EQU 0x1f                ;// M[4:0]=11111，系统模式
IRQ32Mode   EQU 0x12                ;// M[4:0]=10010，IRQ中断
FIQ32Mode   EQU 0x11                ;// M[4:0]=10001，快速中断


    IMPORT  __use_no_semihosting_swi
   

;/* 引入的外部标号在这声明 */
    IMPORT    main                  		;// C语言主程序入口      
    IMPORT  TargetResetInit         	;// 开发板基本初始化     
    IMPORT   SoftwareInterrupt
    IMPORT   IRQ_Handler
;/* 给外部使用的标号在这声明 */
    EXPORT  	bottom_of_heap
    EXPORT  	StackUsr
    IMPORT	StackSvc
  ;//  EXPORT	StackOfIrq
    
    EXPORT  Reset
    EXPORT __user_initial_stackheap
    

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY

;/* 中断向量表 */
Reset
        LDR     PC, ResetAddr                   ;// 0x00000000，复位
        LDR     PC, UndefinedAddr               ;// 0x00000004，未定义地址
        LDR     PC, SWI_Addr                    ;// 0x00000008，软件中断
        LDR     PC, PrefetchAddr                ;// 0x0000000c，预取指中止
        LDR     PC, DataAbortAddr               ;// 0x00000010，数据中止
        DCD     0xb9205f80                      ;// 0x00000014，保留  
        LDR     PC, IRQ_Addr               ;// 0x00000018，IRQ中断
        LDR     PC, FIQ_Addr                    ;// 0x0000001c，FIQ中断

ResetAddr           	DCD     ResetInit           ;// 复位初始化处理程序地址
UndefinedAddr       DCD     Undefined           ;// 未定义指令处理程序地址
SWI_Addr            	DCD     SoftwareInterrupt   ;// 软件中断处理程序地址
PrefetchAddr        	DCD     PrefetchAbort       ;// 预取指中止处理程序地址
DataAbortAddr       DCD     DataAbort           ;// 数据中止处理程序地址
Nouse               	DCD     0                      ;// 未使用
IRQ_Addr            	DCD     IRQ_Handler                   ;// IRQ中断，已在"LDR PC,[PC,#-0xff0]"中处理
FIQ_Addr            	DCD     FIQ_Handler           ;// FIQ中断处理程序地址

;/* 未定义指令 */
Undefined
	LDR 	R1,=IOSET
	MOV	R2,#LED1
	STR		R2,[R1]
        B       Undefined                       ;// 死循环

;/* 取指令中止 */
PrefetchAbort
	LDR 	R1,=IOSET
	MOV	R2,#LED2
	STR		R2,[R1]
        B      PrefetchAbort                   ;// 死循环

;/* 取数据中止 */
DataAbort
	LDR 	R1,=IOSET
	MOV	R2,#LED3
	STR		R2,[R1]
        B       DataAbort                      ;// 死循环

;/* 快速中断 */
FIQ_Handler
        SUBS    PC,  LR,  #4                    ;// 计算返回地址
      B       FIQ_Handler  			;// 死循环
        

;/*********************************************************************************************************
;* 函数名称：InitStack
;* 功能描述：初始化堆栈
;*说明：
;* 入口参数：无
;* 出口参数：无
;* 注    意：当进行初始化堆栈时，处理器状态为ARM状态（T=0且禁止IRQ（I=1）及禁止FIQ（F=1）
;********************************************************************************************************/
InitStack    
        MOV     R0, LR

;/* 设置管理模式堆栈 */
      MSR     CPSR_c, #0xd3		
      LDR     SP, =StackSvc
			
;/* 设置中断模式堆栈 */
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq

;/* 设置快速中断模式堆栈 */
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq

;/* 设置中止模式堆栈 */
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt

;/* 设置未定义模式堆栈 */
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd

;/* 设置系统模式堆栈 */
        MSR     CPSR_c, #0xdf                  ;// 切换到系统模式后，将在系统模式下运行，除非进行模式切换
        LDR     SP, =StackUsr

        MOV     PC, R0
        
        
        
        

;/*********************************************************************************************************
;* 函数名称：ResetInit
;* 功能描述：复位入口
;* 入口参数：无
;* 出口参数：无
;********************************************************************************************************/
ResetInit

        BL      InitStack                ;// 初始化堆栈
  
        BL      TargetResetInit          ;// 开发板基本初始化
                                          
        B       main                   ;// 跳转到c语言入口






;/*********************************************************************************************************
;* 函数名称：__user_initial_stackheap
;* 功能描述：库函数初始化堆和栈，不能删除
;* 入口参数：参考库函数手册
;* 出口参数：参考库函数手册
;********************************************************************************************************/
__user_initial_stackheap    
    LDR   r0,=bottom_of_heap
;// LDR   r1,=StackUsr
;// LDR   r2,=top_of_heap
;// LDR   r3,=bottom_of_Stacks
    MOV   pc,lr


StackIrq           	DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4   ;// IRQ模式堆栈    指向分配到堆栈的最高端  因此一定要使用STMDF入栈
StackFiq           	DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4   ;// FIQ模式堆栈
StackAbt           	DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4   ;// 中止模式堆栈
StackUnd          	DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4  ;// 未定义模式堆栈




;/*********************************************************************************************************
;* 函数名称：CrpData
;* 功能描述：加密芯片
;* 入口参数：无
;* 出口参数：无
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc             ;// 循环用NOP填充，直到0x1FC
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;// 当此数为0x87654321时，用户程序被保护
    ENDIF
    
    LTORG        
;/* 分配堆栈空间 */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2 ;// MyStacks通过分散加载文件定位
IrqStackSpace      	SPACE   IRQ_STACK_LEGTH * 4  ;// 中断模式堆栈空间
FiqStackSpace        	SPACE   FIQ_STACK_LEGTH * 4  ;// 快速中断模式堆栈空间
AbtStackSpace       	SPACE   ABT_STACK_LEGTH * 4  ;// 中止义模式堆栈空间
UndtStackSpace     	SPACE   UND_STACK_LEGTH * 4  ;// 未定义模式堆栈

   

        AREA    Heap, DATA, NOINIT              ;// Heap通过分散加载文件定位
bottom_of_heap    SPACE   1

        AREA    Stacks, DATA, NOINIT            ;// Stacks通过分散加载文件定位
StackUsr

    END
