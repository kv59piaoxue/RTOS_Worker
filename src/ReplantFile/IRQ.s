;//系统调试时使用临时定义
LED1  		EQU	1<<25
LED2    		EQU  	1<<12
LED3    		EQU  	1<<11
LED4     		EQU 	1<<10
LED5     		EQU 	1<<24
LED6		EQU	1<<9
LED7		EQU	1<<8
LED8		EQU	1<<7
LED9		EQU	1<<6
IOSET        	EQU	0xE0028004    ;  /* lpc210x only */
IODIR         	EQU 	0xE0028008    ;  /* lpc210x only */
IOCLR        	EQU 	0xE002800C    ;  /* lpc210x only */
;///////////////////////////////////////////////////////////////////////////////////////////
;//*******************************************************************************************************
;//* 文件名：IRQ.s
;//* 功  能：管理中断嵌套
;//*说明：
;//********************************************************************************************************


NoIRQ      		EQU 	0x80                             ;// Bit7，I位     
NoFIQ			EQU	0x40                		;// Bit6，F位
              
USR32Mode  		EQU 0x10                             ;// M[4:0]=10000，用户模式 
SVC32Mode   		EQU 0x13                             ;// M[4:0]=10011，管理模式 
SYS32Mode   		EQU 0x1f                             ;// M[4:0]=11111，系统模式
IRQ32Mode   		EQU 0x12                             ;// M[4:0]=10010，IRQ中断模式
FIQ32Mode   		EQU 0x11                             ;// M[4:0]=10001，FIQ中断模式

	 EXPORT    	IRQ_Handler
	 
 	 IMPORT    	OSIntNesting  
 	 IMPORT    	OSTaskCurPtr
 	 IMPORT    	OSTaskNextPtr
 	 IMPORT    	OSTaskSwap
  
    	CODE32
    	AREA    IRQ,CODE,READONLY

   
IRQ_Handler    
        SUB     		LR, LR, #4                       ;// 计算返回地址
        STMFD   	SP!, {R0-R3, R12, LR}     ;// 保存任务环境	IRQ模式  LR = PC;入栈循序从右到左 
        MRS     		R3, SPSR                         ;// 保存状态
        STMFD   	SP, {R3,SP,LR}^              ;// 保存SPSR和用户状态的SP,注意不能回写,如果回写的是用户的SP，所以后面要调整SP
	 LDR     		R2, =OSIntNesting        ;//OSIntNesting++
	 LDRB     	R1,  [R2]
	 ADD      	R1,R1,#1
	 STRB   		R1, [R2]
        SUB     		SP, SP, #4*3	;//
        MSR     		CPSR_c, #(NoIRQ |NoFIQ | SYS32Mode)     ;// 从IRQ模式切换到系统模式
        ;// 如果是第一次IRQ中断，那么要初始化堆栈指针，
        ;//因为中断服务程序是在系统模式下运行而不是在IRQ模式下运行	
        CMP   		 R1, #1      
        LDREQ   	 SP, Stack       		
        LDR  		 R1,=0xFFFFF030         ;// 调用c语言的IRQ中断处理程序	VICVectAddr 
	 LDR 		 R1,[R1]
	 MOV  		 LR,PC     		 ; //  
	 MOV 		 PC,R1  

        MSR     		CPSR_c, #(NoIRQ |NoFIQ | IRQ32Mode)     ;// 切换回irq模式
        LDMFD   	SP, {R3,SP,LR}^                     ;// 恢复SPSR和用户状态的SP,注意不能回写
                                                 ;// 如果回写的是用户的SP，所以后面要调整SP                                          
        MSR     		SPSR_cxsf, R3
        ADD     		SP, SP, #4*3      
	 LDR     		R2, =OSIntNesting 		;//OSIntNesting--
	 LDRB     	R1,  [R2]
	 SUB	      	R1,R1,#1
	 STRB    		R1, [R2]
 
	 LDR    		R1,=OSTaskCurPtr
	 LDR  		R1,[R1]	
	 LDR    		R0,=OSTaskNextPtr
	 LDR   		R0,[R0]
	 
	 CMP   	R1,R0     ;//如果OSIntNesting 为零，则说明退出中断嵌套了，再判断任务需不需要切换	
        LDMEQFD   SP!, {R0-R3, R12, PC}^   ;//相等不进行任务调度     
	LDR PC,=OSTaskSwap
    		
    	 LTORG
IRQ_NESTING_STACK	EQU		100  
Stack	DCD	StackOfIrq+(IRQ_NESTING_STACK-1)*4    		 
        AREA    IRQ_Stack,DATA, NOINIT, ALIGN=2
StackOfIrq		SPACE      IRQ_NESTING_STACK*4
    
    END


