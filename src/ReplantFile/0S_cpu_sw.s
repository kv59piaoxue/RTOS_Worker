;//////////////////////////////////////////////////////////////////////////////
;//功能：软件中断与任务切换
;//说明：  
;// //////////////////////////////////////////////////////////////////////////

NoIRQ       	EQU 		0x80                ;// Bit7，I位

NoFIQ		EQU	0x40                ;// Bit6，F位
                  

USR32Mode   EQU 0x10                             ;// M[4:0]=10000，用户模式 
SVC32Mode   EQU 0x13                             ;// M[4:0]=10011，管理模式 
SYS32Mode   	EQU 0x1f                             ;// M[4:0]=11111，系统模式
IRQ32Mode   	EQU 0x12                             ;// M[4:0]=10010，IRQ中断模式
FIQ32Mode   	EQU 0x11                             ;// M[4:0]=10001，FIQ中断模式


LED1  		EQU	1<<25
LED2    		EQU  	1<<12
LED3    		EQU  	1<<11
LED4     		EQU 	1<<10

IOSET        EQU	0xE0028004    ;  /* lpc210x only */
IODIR         EQU 	0xE0028008    ;  /* lpc210x only */
IOCLR        EQU 	0xE002800C    ;  /* lpc210x only */

   EXPORT    SoftwareInterrupt
   
   EXPORT   OSTaskSw_0
   
   IMPORT     OSIntNesting   
   IMPORT     OSEnterSum  
   IMPORT     OSTaskCurPtr
   IMPORT     OSTaskNextPtr   
   
 	CODE32  

   AREA    SoftInt  ,CODE,READONLY



SoftwareInterrupt			
                             
        CMP     R0, #5                          ;// 判断传过来的参数是否大于5
        LDRLO   PC, [PC, R0, LSL #2]            ;// 小于5（参数正确），进行查表
        MOVS    PC, LR                          ;// 大于或等于5（参数出错），则返回

SwiFunction                                    
        DCD             	OSTaskSw                      ;// 0号调用，任务调度
        DCD             	OSEnIRQ                     ;// 1号调用，中断函数里调用，开IRQ中断
        DCD		OSStartTaskSw			;//2号调用，开启多任务
        DCD		OSEntryCritical		                ;// 3号调用，禁止IRQ中断       
        DCD		OSExitCritical	                ;// 4号调用，使能IRQ中断

OSTaskSw 	
   	  MOV 		R2,LR   						;//
   	  MRS  		R3,SPSR						;//
   	  MSR		CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
   	  STMFD		SP!,{R2}						;//压栈pc
   	  STMFD 	SP!,{R0-R12,LR}				;//
	  LDR     		PC,=OSTaskSw_1 				;//
   	 
OSStartTaskSw				;//启动多任务
		MSR	CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
 		LDR  	R2,=OSTaskCurPtr		;//
 		LDR		PC,=OSTaskSw_2 			
        
OSTaskSw_0
 		LDR  R2,[SP,#20]				;//获取PC
 		LDR  R12,[SP,#16]				;//获取R12
 		MRS  R0 ,CPSR				;//暂时保存cpsr
 											
 		MSR    CPSR_c, #(NoIRQ |NoFIQ| SYS32Mode)     ;// 
 						
 		MOV  R1 ,LR				;//保存lr  函数返回地址
 		STMFD  SP!, {R1,R2}		;// 压栈 lr  pc
 		STMFD  SP!,{R4-R12}		;//压栈r4-----r12
 		
 		MSR CPSR_c,R0			;//恢复cpsr 变回IRQ中断模式
 		
 		LDMFD  SP!,{R4-R7}		;//获得r0--r3
 		ADD   SP,SP,#8			;//出栈r12，pc
 							
 		MSR    CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)		;// 变回系统模式
 		
 		STMFD  SP!,{R4-R7}				;//压栈r0---r3
OSTaskSw_1 		
 		LDR  	R1,=OSEnterSum				;//
 		LDRB  	R2,[R1]						;//		
 	       STMFD 	SP!,{R2,R3}				;// 压栈OSEnterSum	，spsr
 		LDR  	R2,=OSTaskCurPtr		;//
 		LDR 	R3,[R2]
 		STR		SP,[R3]					;// 		
 				
OSTaskSw_2	
	 	LDR    	R5,=OSTaskNextPtr  		;//
 		LDR    	R4,[R5]					;//读出下一个任务的SP
 		STR    	R4,[R2]        				;//把下一个任务ID放到OSTaskCurID
 		LDR        R4,[R4]
		ADD  	SP,R4,#68				;//
		LDR  	LR,[SP,#-8]				;//
		MSR  	CPSR_c, #(NoIRQ|NoFIQ|SVC32Mode)	;//
		MOV   	SP,R4						;//
		LDMFD  	SP!,{R4,R5}					;//
		LDR  	R3,=OSEnterSum 				;//
		STRB	R4,[R3]						;//
		MSR	SPSR_cxsf,R5					;//	
		LDMFD	SP!,{R0-R12,LR,PC}^				;//
				
OSEnIRQ  
        	MRS   	R0, SPSR                          ;// 读取SPSR的值
        	BIC   	R0, R0, #NoIRQ                   ;// 清零I位，设置开IRQ中断               
        	MSR   	SPSR_c, R0                        ;// 回写SPSR 
        	MOVS    	PC, LR                          ;// 返回 
        	        
OSEntryCritical 
        LDR     	R1,=OSEnterSum  			;//OSEnterSum++
        LDRB 	R2,[R1]
        ADD	R2,R2,#1
        STRB	R2,[R1]
        CMP      R2,#1
        
        MRSEQ     	R0, SPSR                        ;// 读取SPSR的值 
        ORREQ     	R0, R0, #(NoIRQ|NoFIQ)             ;// 置位I位，设置关IRQ中断
        MSREQ     	SPSR_c, R0                      ;// 回写SPSR
        MOVS    	PC, LR                          ;// 返回 

OSExitCritical
         
        LDR     	R1,=OSEnterSum			;//OSEnterSum--
        LDRB 	R2,[R1]
        SUB	R2,R2,#1
        STRB	R2,[R1]
        CMP      R2,#0
                      
        MRSEQ   	R0, SPSR                          ;// 读取SPSR的值
        BICEQ   		R0, R0, #NoIRQ                   ;// 清零I位，设置开IRQ中断               
        MSREQ   	SPSR_c, R0                        ;// 回写SPSR 
        MOVS    	PC, LR                          ;// 返回 

     END
