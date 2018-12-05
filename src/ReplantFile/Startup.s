
;//ϵͳ����ʱʹ�õ���ʱ����
LED1  		EQU	1<<25
LED2    		EQU  	1<<12
LED3    		EQU  	1<<11
LED4     		EQU 	1<<10
IOSET        EQU	0xE0028004    ;  /* lpc210x only */
IODIR         EQU 	0xE0028008    ;  /* lpc210x only */
IOCLR        EQU 	0xE002800C    ;  /* lpc210x only */
;/////////////////////////////////////////////////////////////////////////////////
;/*�����ջ�Ĵ�С*/
FIQ_STACK_LEGTH         EQU         0
IRQ_STACK_LEGTH         EQU         9*12 ;//ÿ��IRQ�ж���Ҫ��ջ9���Ĵ������������12���ж�Ƕ��
ABT_STACK_LEGTH         EQU         0
UND_STACK_LEGTH         EQU         0

NoIRQ             EQU     0x80                ;// Bit7��Iλ

NoFIQ		EQU	0x40                ;// Bit6��Fλ

USR32Mode   EQU 0x10                ;// M[4:0]=10000���û�ģʽ
SVC32Mode   EQU 0x13                ;// M[4:0]=10011������ģʽ
SYS32Mode   EQU 0x1f                ;// M[4:0]=11111��ϵͳģʽ
IRQ32Mode   EQU 0x12                ;// M[4:0]=10010��IRQ�ж�
FIQ32Mode   EQU 0x11                ;// M[4:0]=10001�������ж�


    IMPORT  __use_no_semihosting_swi
   

;/* ������ⲿ����������� */
    IMPORT    main                  		;// C�������������      
    IMPORT  TargetResetInit         	;// �����������ʼ��     
    IMPORT   SoftwareInterrupt
    IMPORT   IRQ_Handler
;/* ���ⲿʹ�õı���������� */
    EXPORT  	bottom_of_heap
    EXPORT  	StackUsr
    IMPORT	StackSvc
  ;//  EXPORT	StackOfIrq
    
    EXPORT  Reset
    EXPORT __user_initial_stackheap
    

    CODE32

    AREA    vectors,CODE,READONLY
        ENTRY

;/* �ж������� */
Reset
        LDR     PC, ResetAddr                   ;// 0x00000000����λ
        LDR     PC, UndefinedAddr               ;// 0x00000004��δ�����ַ
        LDR     PC, SWI_Addr                    ;// 0x00000008������ж�
        LDR     PC, PrefetchAddr                ;// 0x0000000c��Ԥȡָ��ֹ
        LDR     PC, DataAbortAddr               ;// 0x00000010��������ֹ
        DCD     0xb9205f80                      ;// 0x00000014������  
        LDR     PC, IRQ_Addr               ;// 0x00000018��IRQ�ж�
        LDR     PC, FIQ_Addr                    ;// 0x0000001c��FIQ�ж�

ResetAddr           	DCD     ResetInit           ;// ��λ��ʼ����������ַ
UndefinedAddr       DCD     Undefined           ;// δ����ָ�������ַ
SWI_Addr            	DCD     SoftwareInterrupt   ;// ����жϴ�������ַ
PrefetchAddr        	DCD     PrefetchAbort       ;// Ԥȡָ��ֹ��������ַ
DataAbortAddr       DCD     DataAbort           ;// ������ֹ��������ַ
Nouse               	DCD     0                      ;// δʹ��
IRQ_Addr            	DCD     IRQ_Handler                   ;// IRQ�жϣ�����"LDR PC,[PC,#-0xff0]"�д���
FIQ_Addr            	DCD     FIQ_Handler           ;// FIQ�жϴ�������ַ

;/* δ����ָ�� */
Undefined
	LDR 	R1,=IOSET
	MOV	R2,#LED1
	STR		R2,[R1]
        B       Undefined                       ;// ��ѭ��

;/* ȡָ����ֹ */
PrefetchAbort
	LDR 	R1,=IOSET
	MOV	R2,#LED2
	STR		R2,[R1]
        B      PrefetchAbort                   ;// ��ѭ��

;/* ȡ������ֹ */
DataAbort
	LDR 	R1,=IOSET
	MOV	R2,#LED3
	STR		R2,[R1]
        B       DataAbort                      ;// ��ѭ��

;/* �����ж� */
FIQ_Handler
        SUBS    PC,  LR,  #4                    ;// ���㷵�ص�ַ
      B       FIQ_Handler  			;// ��ѭ��
        

;/*********************************************************************************************************
;* �������ƣ�InitStack
;* ������������ʼ����ջ
;*˵����
;* ��ڲ�������
;* ���ڲ�������
;* ע    �⣺�����г�ʼ����ջʱ��������״̬ΪARM״̬��T=0�Q�ҽ�ֹIRQ��I=1������ֹFIQ��F=1��
;********************************************************************************************************/
InitStack    
        MOV     R0, LR

;/* ���ù���ģʽ��ջ */
      MSR     CPSR_c, #0xd3		
      LDR     SP, =StackSvc
			
;/* �����ж�ģʽ��ջ */
        MSR     CPSR_c, #0xd2
        LDR     SP, StackIrq

;/* ���ÿ����ж�ģʽ��ջ */
        MSR     CPSR_c, #0xd1
        LDR     SP, StackFiq

;/* ������ֹģʽ��ջ */
        MSR     CPSR_c, #0xd7
        LDR     SP, StackAbt

;/* ����δ����ģʽ��ջ */
        MSR     CPSR_c, #0xdb
        LDR     SP, StackUnd

;/* ����ϵͳģʽ��ջ */
        MSR     CPSR_c, #0xdf                  ;// �л���ϵͳģʽ�󣬽���ϵͳģʽ�����У����ǽ���ģʽ�л�
        LDR     SP, =StackUsr

        MOV     PC, R0
        
        
        
        

;/*********************************************************************************************************
;* �������ƣ�ResetInit
;* ������������λ���
;* ��ڲ�������
;* ���ڲ�������
;********************************************************************************************************/
ResetInit

        BL      InitStack                ;// ��ʼ����ջ
  
        BL      TargetResetInit          ;// �����������ʼ��
                                          
        B       main                   ;// ��ת��c�������






;/*********************************************************************************************************
;* �������ƣ�__user_initial_stackheap
;* �����������⺯����ʼ���Ѻ�ջ������ɾ��
;* ��ڲ������ο��⺯���ֲ�
;* ���ڲ������ο��⺯���ֲ�
;********************************************************************************************************/
__user_initial_stackheap    
    LDR   r0,=bottom_of_heap
;// LDR   r1,=StackUsr
;// LDR   r2,=top_of_heap
;// LDR   r3,=bottom_of_Stacks
    MOV   pc,lr


StackIrq           	DCD     IrqStackSpace + (IRQ_STACK_LEGTH - 1)* 4   ;// IRQģʽ��ջ    ָ����䵽��ջ����߶�  ���һ��Ҫʹ��STMDF��ջ
StackFiq           	DCD     FiqStackSpace + (FIQ_STACK_LEGTH - 1)* 4   ;// FIQģʽ��ջ
StackAbt           	DCD     AbtStackSpace + (ABT_STACK_LEGTH - 1)* 4   ;// ��ֹģʽ��ջ
StackUnd          	DCD     UndtStackSpace + (UND_STACK_LEGTH - 1)* 4  ;// δ����ģʽ��ջ




;/*********************************************************************************************************
;* �������ƣ�CrpData
;* ��������������оƬ
;* ��ڲ�������
;* ���ڲ�������
;********************************************************************************************************/
    IF :DEF: EN_CRP
        IF  . >= 0x1fc
        INFO    1,"\nThe data at 0x000001fc must be 0x87654321.\nPlease delete some source before this line."
        ENDIF
CrpData
    WHILE . < 0x1fc             ;// ѭ����NOP��䣬ֱ��0x1FC
    NOP
    WEND
CrpData1
    DCD     0x87654321          ;// ������Ϊ0x87654321ʱ���û����򱻱���
    ENDIF
    
    LTORG        
;/* �����ջ�ռ� */
        AREA    MyStacks, DATA, NOINIT, ALIGN=2 ;// MyStacksͨ����ɢ�����ļ���λ
IrqStackSpace      	SPACE   IRQ_STACK_LEGTH * 4  ;// �ж�ģʽ��ջ�ռ�
FiqStackSpace        	SPACE   FIQ_STACK_LEGTH * 4  ;// �����ж�ģʽ��ջ�ռ�
AbtStackSpace       	SPACE   ABT_STACK_LEGTH * 4  ;// ��ֹ��ģʽ��ջ�ռ�
UndtStackSpace     	SPACE   UND_STACK_LEGTH * 4  ;// δ����ģʽ��ջ

   

        AREA    Heap, DATA, NOINIT              ;// Heapͨ����ɢ�����ļ���λ
bottom_of_heap    SPACE   1

        AREA    Stacks, DATA, NOINIT            ;// Stacksͨ����ɢ�����ļ���λ
StackUsr

    END
