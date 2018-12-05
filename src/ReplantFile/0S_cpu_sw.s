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
                             
        CMP     R0, #5                          ;// �жϴ������Ĳ����Ƿ����5
        LDRLO   PC, [PC, R0, LSL #2]            ;// С��5��������ȷ�������в��
        MOVS    PC, LR                          ;// ���ڻ����5�������������򷵻�

SwiFunction                                    
        DCD             	OSTaskSw                      ;// 0�ŵ��ã��������
        DCD             	OSEnIRQ                     ;// 1�ŵ��ã��жϺ�������ã���IRQ�ж�
        DCD		OSStartTaskSw			;//2�ŵ��ã�����������
        DCD		OSEntryCritical		                ;// 3�ŵ��ã���ֹIRQ�ж�       
        DCD		OSExitCritical	                ;// 4�ŵ��ã�ʹ��IRQ�ж�

OSTaskSw 	
   	  MOV 		R2,LR   						;//
   	  MRS  		R3,SPSR						;//
   	  MSR		CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
   	  STMFD		SP!,{R2}						;//ѹջpc
   	  STMFD 	SP!,{R0-R12,LR}				;//
	  LDR     		PC,=OSTaskSw_1 				;//
   	 
OSStartTaskSw				;//����������
		MSR	CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)
 		LDR  	R2,=OSTaskCurPtr		;//
 		LDR		PC,=OSTaskSw_2 			
        
OSTaskSw_0
 		LDR  R2,[SP,#20]				;//��ȡPC
 		LDR  R12,[SP,#16]				;//��ȡR12
 		MRS  R0 ,CPSR				;//��ʱ����cpsr
 											
 		MSR    CPSR_c, #(NoIRQ |NoFIQ| SYS32Mode)     ;// 
 						
 		MOV  R1 ,LR				;//����lr  �������ص�ַ
 		STMFD  SP!, {R1,R2}		;// ѹջ lr  pc
 		STMFD  SP!,{R4-R12}		;//ѹջr4-----r12
 		
 		MSR CPSR_c,R0			;//�ָ�cpsr ���IRQ�ж�ģʽ
 		
 		LDMFD  SP!,{R4-R7}		;//���r0--r3
 		ADD   SP,SP,#8			;//��ջr12��pc
 							
 		MSR    CPSR_c,#(NoIRQ|NoFIQ|SYS32Mode)		;// ���ϵͳģʽ
 		
 		STMFD  SP!,{R4-R7}				;//ѹջr0---r3
OSTaskSw_1 		
 		LDR  	R1,=OSEnterSum				;//
 		LDRB  	R2,[R1]						;//		
 	       STMFD 	SP!,{R2,R3}				;// ѹջOSEnterSum	��spsr
 		LDR  	R2,=OSTaskCurPtr		;//
 		LDR 	R3,[R2]
 		STR		SP,[R3]					;// 		
 				
OSTaskSw_2	
	 	LDR    	R5,=OSTaskNextPtr  		;//
 		LDR    	R4,[R5]					;//������һ�������SP
 		STR    	R4,[R2]        				;//����һ������ID�ŵ�OSTaskCurID
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
        	MRS   	R0, SPSR                          ;// ��ȡSPSR��ֵ
        	BIC   	R0, R0, #NoIRQ                   ;// ����Iλ�����ÿ�IRQ�ж�               
        	MSR   	SPSR_c, R0                        ;// ��дSPSR 
        	MOVS    	PC, LR                          ;// ���� 
        	        
OSEntryCritical 
        LDR     	R1,=OSEnterSum  			;//OSEnterSum++
        LDRB 	R2,[R1]
        ADD	R2,R2,#1
        STRB	R2,[R1]
        CMP      R2,#1
        
        MRSEQ     	R0, SPSR                        ;// ��ȡSPSR��ֵ 
        ORREQ     	R0, R0, #(NoIRQ|NoFIQ)             ;// ��λIλ�����ù�IRQ�ж�
        MSREQ     	SPSR_c, R0                      ;// ��дSPSR
        MOVS    	PC, LR                          ;// ���� 

OSExitCritical
         
        LDR     	R1,=OSEnterSum			;//OSEnterSum--
        LDRB 	R2,[R1]
        SUB	R2,R2,#1
        STRB	R2,[R1]
        CMP      R2,#0
                      
        MRSEQ   	R0, SPSR                          ;// ��ȡSPSR��ֵ
        BICEQ   		R0, R0, #NoIRQ                   ;// ����Iλ�����ÿ�IRQ�ж�               
        MSREQ   	SPSR_c, R0                        ;// ��дSPSR 
        MOVS    	PC, LR                          ;// ���� 

     END
