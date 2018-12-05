#ifndef	__OSDEFINE_HPP
#define  __OSDEFINE_HPP
          //Worker1.0
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
#define  	NULL  			0
#define    EnIRQ    			2
#define    EnFIQ   			1
#define  	DF_ThreadStackDeep	300
#define   	DF_MINPRIO   		4
#define     DF_TICKS_PSEC    		100		//设置定时器，中断频率 T0MR0 = (Fpclk / DF_TICKS_PSEC);
#define     DF_MAXRUNTIME		DF_TICKS_PSEC*100/1000		//100ms
#define 	DF_MAXWAITTIME		DF_TICKS_PSEC*500/1000		//500ms
#define  	DF_MsgLength         		5

//Task Stat  Define
#define Task_Stat_Rdy   	 	0x00000001
#define Task_Stat_Time  	 	0x00000002
#define Task_Stat_Event 	 	0x00000004
#define Task_Stat_ThreadTick  	0x00000008
#define Task_Stat_Pd	 	 	0x00000010
#define  Task_Stat_Exit			0x00000020
#define Task_Info_TimeOut  	0x00000001
#define  Task_Info_PendMailQ	0x00000002
#define  Task_Info_PendMutex  	0x00000004
#define  Task_Info_MsgSend	0x00000008
#define  Task_Info_MutexGet	0x00000010
#define  Task_Info_PendMsg	0x00000020
#define  Task_Info_MailQGet	0x00000040
#define  Task_Info_PendSem  	0x00000080
#define  Task_Info_SemGet		0x00000100
//Thread Message
#define    TM_TM 			  				1
#define  	TM_QUIT  					TM_TM + 1	 
#define  	TM_CREATE					TM_TM + 2	
#define 	TM_TIMERINT				TM_TM+3
#define 	TM_THREADONTIME  		TM_TM+4
#define  	TM_MUTEXPUTMEDI		TM_TM+5
#define  	TM_MSGFLUSHMEDI		        TM_TM+6
#define  	TM_SEMXPUTMEDI			TM_TM+7

#define    TM_SYSMESSAGETOP		TM_TM +1024
#define   	TM_USEMESSAGEBASE		TM_TM +1025


#define  	ONLED5		TM_TM+300
#define  	OFFLED5	TM_TM+301
#define  	ONLED7		TM_TM+302
#define  	OFFLED7	TM_TM+303
#define  	ONLED8		TM_TM+304
#define  	OFFLED8	TM_TM+305
#define  	ONLED4		TM_TM+306
#define  	OFFLED4	TM_TM+307



#endif
