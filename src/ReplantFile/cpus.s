

;//系统调试时的临时定义
LED1  		EQU	1<<25
LED2    		EQU  	1<<12
LED3    		EQU  	1<<11
LED4     		EQU 	1<<10
LED5     EQU 	1<<24
LED6	EQU	1<<9
LED7	EQU	1<<8
LED8	EQU	1<<7
LED9	EQU	1<<6
IOSET        EQU	0xE0028004    ;  /* lpc210x only */
IODIR         EQU 	0xE0028008    ;  /* lpc210x only */
IOCLR        EQU 	0xE002800C    ;  /* lpc210x only */
;//////////////////////////////////////////////////////////////////////////////
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


   EXPORT    SoftwareInterrupt
   EXPORT	OSTaskSwap
   
   IMPORT     OSIntNesting   
   IMPORT     OSEnterSum  
   IMPORT     OSTaskCurPtr
   IMPORT     OSTaskNextPtr   
   EXPORT    StackSvc
 	CODE32  
   AREA    SoftInt  ,CODE,READONLY
 
SoftwareInterrupt	
       	LDR   		SP, StackSvc;//
	STMFD   	SP!, {R0-R3, R12, LR}               
        CMP     R0, #6                          ;// 判断传过来的参数是否大于6
        LDRLO   PC, [PC, R0, LSL #2]            ;// 小于6（参数正确），进行查表
        LDMFD		SP!,{R0-R3,R12,PC}^ 	;// 大于或等于6（参数出错），则返回

SwiFunction                                    
        DCD             	OSTaskSwap                  ;// 0号调用，任务切换Schedul()
        DCD		OSStartUp			;//1号调用，开启多任务StartUp()
        DCD             	OSEnableIRQ                 ;// 2号调用，开中断intEnable()
        DCD		OSDisableIRQ		;//3号调用,  关中断intDisable()
        DCD		OSEnterCritical		 ;// 4号调用，关中断intLock()	OSEnterSum++    
        DCD		OSExitCritical	                ;// 5号调用，开中断intUnlock()	OSEnterSum--

OSStartUp				;//启动多任务
		MSR	CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
 		LDR  	R2,=OSTaskCurPtr		;//	
 		LDR		PC,=OSTaskSwap1 
 		    
OSTaskSwap	;//IRQ模式下运行
   	  	MRS  		R3,SPSR						;//保存任务切换前CPSR
   	  	LDR 		R2,[SP,#20]  					;//保存PC值到R2
   	  	LDR 		R12,[SP,#16]					;//SAVE  R12
   	  	   	  
   	  	MRS		R0,CPSR
   	  
   	  	MSR		CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
   	  	MOV		R1,LR  ;//SAVE   	LR
   	  	STMFD		SP!,{R1-R2};//SAVE   LR,PC
   	  	STMFD		SP!,{R4-R12};//SAVE  R4 - R12
   	  
   	  	MSR 		CPSR_c,R0;//CHANGE  IRQ

   	  	LDMFD	SP!,{R4-R7} ;//OUT  R0--R3
   	  	ADD		SP,SP,#8 ;//OUT R12,PC 
   	  	MSR		CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
   	  	STMFD		SP!,{R4-R7};// IN   R0-R3 
   	  	LDR	 	R1,=OSEnterSum
   	  	LDRB		R2,[R1]
   	  	STMFD		SP!,{R2-R3};//OSEnterSum ,CPSR
   	  
	  	LDR  		R2,=OSTaskCurPtr		;//
 	 	LDR 		R3,[R2]
 	  	;//STR			SP,[R3,#4] ;//virtual 	
 	  	STR			SP,[R3] ;//
OSTaskSwap1	 
	 	LDR    	R5,=OSTaskNextPtr  		;//系统模式下运行
 		LDR    	R4,[R5]					;//读出下一个任务的SP
 		STR    	R4,[R2]        				;//把下一个任务ID放到OSTaskCurID		
 		;//LDR        R4,[R4,#4]  				;//virtual 读出任务的堆栈指针
 		LDR        R4,[R4]  					;//读出任务的堆栈指针
		ADD  	SP,R4,#68				;//调整系统模式下的堆栈指针，让他跳过CPU寄存器，指到被中断的函数处
		LDR  	LR,[SP,#-8]				;//读回被中断的函数的返回地址（用户模式的LR）
		MSR  	CPSR_c, #(NoIRQ|NoFIQ|SVC32Mode)	;//
		MOV   	SP,R4						;//让管理模式的SP指到将要被运行的任务的CPU寄存器存放的开始地址
		LDMFD  	SP!,{R4,R5}				;//出栈R4 = （OSEnterSum），R5 = （SPSR） 
		LDR  	R3,=OSEnterSum 				;//
		STRB	R4,[R3]						;//
		MSR	SPSR_cxsf,R5					;//在执行下面一条指令的时候，管理模式的SPSR会复制到CPSR自动切换到用户模式	
		LDMFD	SP!,{R0-R12,LR,PC}^			;//自动切换到用户模式，运行线程
	  					
OSEnableIRQ  
        	MRS   	R0, SPSR                          ;// 读取SPSR的值
        	BIC   	R0, R0, #NoIRQ                   ;// 清零I位，设置开IRQ中断               
        	MSR   	SPSR_c, R0                        ;// 回写SPSR 
        	LDMFD		SP!,{R0-R3,R12,PC}^                           ;// 返回 
OSDisableIRQ
		MRS   	R0, SPSR
		ORR  	R0, R0, #NoIRQ 
		MSR   	SPSR_c, R0 
		 LDMFD		SP!,{R0-R3,R12,PC}^                          ;// 返回 
        	        
OSEnterCritical 
        LDR     	R1,=OSEnterSum  			;//OSEnterSum++
        LDRB 	R2,[R1]
        ADD	R2,R2,#1
        STRB	R2,[R1]
        
        MRS     	R0, SPSR                        ;// 读取SPSR的值 
        ORR     	R0, R0, #(NoIRQ|NoFIQ)             ;// 置位I位，设置关IRQ中断
        MSR     	SPSR_c, R0                      ;// 回写SPSR
        LDMFD		SP!,{R0-R3,R12,PC}^                          ;// 返回 

OSExitCritical
         
        LDR     	R1,=OSEnterSum			;//OSEnterSum--
        LDRB 	R2,[R1]
        SUB	R2,R2,#1
        STRB	R2,[R1]
        CMP      R2,#0
                      
        MRSEQ   	R0, SPSR                          ;// 读取SPSR的值
        BICEQ   		R0, R0, #NoIRQ                   ;// 清零I位，设置开IRQ中断               
        MSREQ   	SPSR_c, R0                        ;// 回写SPSR 
        LDMFD		SP!,{R0-R3,R12,PC}^                         ;// 返回 
           
    LTORG    
 
SVC_STACK_LEGTH         EQU        10      ;//实际上7就够了
StackSvc           	DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4   ;// 管理模式堆栈           
           
             AREA    MyStacks, DATA, NOINIT, ALIGN=2 ;// MyStacks通过分散加载文件定位
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;// 管理模式堆栈空间
     END
