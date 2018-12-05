

;//ϵͳ����ʱ����ʱ����
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
;//���ܣ�����ж��������л�
;//˵����  
;// //////////////////////////////////////////////////////////////////////////

NoIRQ       	EQU 		0x80                ;// Bit7��Iλ

NoFIQ		EQU	0x40                ;// Bit6��Fλ
                  

USR32Mode   EQU 0x10                             ;// M[4:0]=10000���û�ģʽ 
SVC32Mode   EQU 0x13                             ;// M[4:0]=10011������ģʽ 
SYS32Mode   	EQU 0x1f                             ;// M[4:0]=11111��ϵͳģʽ
IRQ32Mode   	EQU 0x12                             ;// M[4:0]=10010��IRQ�ж�ģʽ
FIQ32Mode   	EQU 0x11                             ;// M[4:0]=10001��FIQ�ж�ģʽ


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
        CMP     R0, #6                          ;// �жϴ������Ĳ����Ƿ����6
        LDRLO   PC, [PC, R0, LSL #2]            ;// С��6��������ȷ�������в��
        LDMFD		SP!,{R0-R3,R12,PC}^ 	;// ���ڻ����6�������������򷵻�

SwiFunction                                    
        DCD             	OSTaskSwap                  ;// 0�ŵ��ã������л�Schedul()
        DCD		OSStartUp			;//1�ŵ��ã�����������StartUp()
        DCD             	OSEnableIRQ                 ;// 2�ŵ��ã����ж�intEnable()
        DCD		OSDisableIRQ		;//3�ŵ���,  ���ж�intDisable()
        DCD		OSEnterCritical		 ;// 4�ŵ��ã����ж�intLock()	OSEnterSum++    
        DCD		OSExitCritical	                ;// 5�ŵ��ã����ж�intUnlock()	OSEnterSum--

OSStartUp				;//����������
		MSR	CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
 		LDR  	R2,=OSTaskCurPtr		;//	
 		LDR		PC,=OSTaskSwap1 
 		    
OSTaskSwap	;//IRQģʽ������
   	  	MRS  		R3,SPSR						;//���������л�ǰCPSR
   	  	LDR 		R2,[SP,#20]  					;//����PCֵ��R2
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
	 	LDR    	R5,=OSTaskNextPtr  		;//ϵͳģʽ������
 		LDR    	R4,[R5]					;//������һ�������SP
 		STR    	R4,[R2]        				;//����һ������ID�ŵ�OSTaskCurID		
 		;//LDR        R4,[R4,#4]  				;//virtual ��������Ķ�ջָ��
 		LDR        R4,[R4]  					;//��������Ķ�ջָ��
		ADD  	SP,R4,#68				;//����ϵͳģʽ�µĶ�ջָ�룬��������CPU�Ĵ�����ָ�����жϵĺ�����
		LDR  	LR,[SP,#-8]				;//���ر��жϵĺ����ķ��ص�ַ���û�ģʽ��LR��
		MSR  	CPSR_c, #(NoIRQ|NoFIQ|SVC32Mode)	;//
		MOV   	SP,R4						;//�ù���ģʽ��SPָ����Ҫ�����е������CPU�Ĵ�����ŵĿ�ʼ��ַ
		LDMFD  	SP!,{R4,R5}				;//��ջR4 = ��OSEnterSum����R5 = ��SPSR�� 
		LDR  	R3,=OSEnterSum 				;//
		STRB	R4,[R3]						;//
		MSR	SPSR_cxsf,R5					;//��ִ������һ��ָ���ʱ�򣬹���ģʽ��SPSR�Ḵ�Ƶ�CPSR�Զ��л����û�ģʽ	
		LDMFD	SP!,{R0-R12,LR,PC}^			;//�Զ��л����û�ģʽ�������߳�
	  					
OSEnableIRQ  
        	MRS   	R0, SPSR                          ;// ��ȡSPSR��ֵ
        	BIC   	R0, R0, #NoIRQ                   ;// ����Iλ�����ÿ�IRQ�ж�               
        	MSR   	SPSR_c, R0                        ;// ��дSPSR 
        	LDMFD		SP!,{R0-R3,R12,PC}^                           ;// ���� 
OSDisableIRQ
		MRS   	R0, SPSR
		ORR  	R0, R0, #NoIRQ 
		MSR   	SPSR_c, R0 
		 LDMFD		SP!,{R0-R3,R12,PC}^                          ;// ���� 
        	        
OSEnterCritical 
        LDR     	R1,=OSEnterSum  			;//OSEnterSum++
        LDRB 	R2,[R1]
        ADD	R2,R2,#1
        STRB	R2,[R1]
        
        MRS     	R0, SPSR                        ;// ��ȡSPSR��ֵ 
        ORR     	R0, R0, #(NoIRQ|NoFIQ)             ;// ��λIλ�����ù�IRQ�ж�
        MSR     	SPSR_c, R0                      ;// ��дSPSR
        LDMFD		SP!,{R0-R3,R12,PC}^                          ;// ���� 

OSExitCritical
         
        LDR     	R1,=OSEnterSum			;//OSEnterSum--
        LDRB 	R2,[R1]
        SUB	R2,R2,#1
        STRB	R2,[R1]
        CMP      R2,#0
                      
        MRSEQ   	R0, SPSR                          ;// ��ȡSPSR��ֵ
        BICEQ   		R0, R0, #NoIRQ                   ;// ����Iλ�����ÿ�IRQ�ж�               
        MSREQ   	SPSR_c, R0                        ;// ��дSPSR 
        LDMFD		SP!,{R0-R3,R12,PC}^                         ;// ���� 
           
    LTORG    
 
SVC_STACK_LEGTH         EQU        10      ;//ʵ����7�͹���
StackSvc           	DCD     SvcStackSpace + (SVC_STACK_LEGTH - 1)* 4   ;// ����ģʽ��ջ           
           
             AREA    MyStacks, DATA, NOINIT, ALIGN=2 ;// MyStacksͨ����ɢ�����ļ���λ
SvcStackSpace      SPACE   SVC_STACK_LEGTH * 4  ;// ����ģʽ��ջ�ռ�
     END
