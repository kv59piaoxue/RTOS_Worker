
//л����
#include "config.hpp"

	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
	
//��ǰ����ȴ��¼�
bool  OSCEvent :: EventPend(OSCTask*TaskP,uint32 Dly,uint32 TaskInfo){
	if(PrioIndex > TaskP -> TaskPrio){
		PrioIndex = TaskP -> TaskPrio;
	}
	ArryEvent[TaskP -> TaskPrio].ListIn(TaskP);
	TaskP -> EventList = ArryEvent;//��¼��������Ķ���
	if(Dly > 0){
		OSCTimer::TimeWait(TaskP,Dly);//ʱ����еȴ�
	}
	TaskP -> TaskInfo |= TaskInfo;
	OSCPrioMng::Leave(TaskP);// ��������ȡ������
	return TRUE ;
}
// ��ǰ�������¼�
OSCTask*  OSCEvent :: EventResume(uint32 TaskInfo){
	OSCTask *TaskPtr =NULL ;
	while(PrioIndex < DF_MINPRIO){
		if(!ArryEvent[PrioIndex].IsEmpty()){//�ȴ����в��ǿ�
			OSCTask::TaskLock();
			TaskPtr = ArryEvent[PrioIndex].GetFirst();
			if(NULL != TaskPtr){
				if(ArryEvent[PrioIndex].ListOut(TaskPtr)){
					if( TaskPtr -> TaskStat & Task_Stat_Time){
						OSCTimer::TimeLeave(TaskPtr);
					}
					TaskPtr -> TaskInfo |= TaskInfo;
					TaskPtr -> EventList = NULL;//��¼��������Ķ���
					OSCPrioMng::Wait(TaskPtr);
				}
			}
			OSCTask::TaskUnlock();
			return TaskPtr;
		}
		PrioIndex ++;
	}
	return  NULL;
}

///
bool  OSCEvent :: EventListOut(OSCTask *TaskP){
	return  ArryEvent[TaskP -> TaskPrio].ListOut(TaskP);//�ȴ����г���
}
		