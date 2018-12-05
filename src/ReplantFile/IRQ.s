;//ϵͳ����ʱʹ����ʱ����
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
;//* �ļ�����IRQ.s
;//* ��  �ܣ������ж�Ƕ��
;//*˵����
;//********************************************************************************************************


NoIRQ      		EQU 	0x80                             ;// Bit7��Iλ     
NoFIQ			EQU	0x40                		;// Bit6��Fλ
              
USR32Mode  		EQU 0x10                             ;// M[4:0]=10000���û�ģʽ 
SVC32Mode   		EQU 0x13                             ;// M[4:0]=10011������ģʽ 
SYS32Mode   		EQU 0x1f                             ;// M[4:0]=11111��ϵͳģʽ
IRQ32Mode   		EQU 0x12                             ;// M[4:0]=10010��IRQ�ж�ģʽ
FIQ32Mode   		EQU 0x11                             ;// M[4:0]=10001��FIQ�ж�ģʽ

	 EXPORT    	IRQ_Handler
	 
 	 IMPORT    	OSIntNesting  
 	 IMPORT    	OSTaskCurPtr
 	 IMPORT    	OSTaskNextPtr
 	 IMPORT    	OSTaskSwap
  
    	CODE32
    	AREA    IRQ,CODE,READONLY

   
IRQ_Handler    
        SUB     		LR, LR, #4                       ;// ���㷵�ص�ַ
        STMFD   	SP!, {R0-R3, R12, LR}     ;// �������񻷾�	IRQģʽ  LR = PC;��ջѭ����ҵ��� 
        MRS     		R3, SPSR                         ;// ����״̬
        STMFD   	SP, {R3,SP,LR}^              ;// ����SPSR���û�״̬��SP,ע�ⲻ�ܻ�д,�����д�����û���SP�����Ժ���Ҫ����SP
	 LDR     		R2, =OSIntNesting        ;//OSIntNesting++
	 LDRB     	R1,  [R2]
	 ADD      	R1,R1,#1
	 STRB   		R1, [R2]
        SUB     		SP, SP, #4*3	;//
        MSR     		CPSR_c, #(NoIRQ |NoFIQ | SYS32Mode)     ;// ��IRQģʽ�л���ϵͳģʽ
        ;// ����ǵ�һ��IRQ�жϣ���ôҪ��ʼ����ջָ�룬
        ;//��Ϊ�жϷ����������ϵͳģʽ�����ж�������IRQģʽ������	
        CMP   		 R1, #1      
        LDREQ   	 SP, Stack       		
        LDR  		 R1,=0xFFFFF030         ;// ����c���Ե�IRQ�жϴ������	VICVectAddr 
	 LDR 		 R1,[R1]
	 MOV  		 LR,PC     		 ; //  
	 MOV 		 PC,R1  

        MSR     		CPSR_c, #(NoIRQ |NoFIQ | IRQ32Mode)     ;// �л���irqģʽ
        LDMFD   	SP, {R3,SP,LR}^                     ;// �ָ�SPSR���û�״̬��SP,ע�ⲻ�ܻ�д
                                                 ;// �����д�����û���SP�����Ժ���Ҫ����SP                                          
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
	 
	 CMP   	R1,R0     ;//���OSIntNesting Ϊ�㣬��˵���˳��ж�Ƕ���ˣ����ж������費��Ҫ�л�	
        LDMEQFD   SP!, {R0-R3, R12, PC}^   ;//��Ȳ������������     
	LDR PC,=OSTaskSwap
    		
    	 LTORG
IRQ_NESTING_STACK	EQU		100  
Stack	DCD	StackOfIrq+(IRQ_NESTING_STACK-1)*4    		 
        AREA    IRQ_Stack,DATA, NOINIT, ALIGN=2
StackOfIrq		SPACE      IRQ_NESTING_STACK*4
    
    END


