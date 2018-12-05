//л����

#include "config.hpp"
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��13726903019
	//ʱ�䣺2010��
//������������
bool  OSCPendRdy::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Rdy){//�ھ���������
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_Rdy ;//���������״̬	
	
	++ Count;//��������һ
	if(Count == 1){//������Ϊ��	
		PendPtr = TaskPtr ;//��������
		TaskPtr -> RdyNextPtr = TaskPtr ;//ָ���Լ�
		TaskPtr -> RdyPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> RdyNextPtr = PendPtr ;
		TaskPtr -> RdyPrevPtr = PendPtr -> RdyPrevPtr ;
		PendPtr -> RdyPrevPtr -> RdyNextPtr = TaskPtr ;
		PendPtr -> RdyPrevPtr = TaskPtr ;
	}
	
	OSCTask::TaskUnlock();
	return TRUE;
}
//�������г���
bool  OSCPendRdy::ListOut(OSCTask*TaskPtr){
	bool TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Rdy){//�����ھ���������
		TaskPtr -> TaskStat  &=  ~Task_Stat_Rdy ;
		Count --;
		if(0 == Count){//һ������
			PendPtr = NULL ;
		}else{//�������ϵ�����
			if(PendPtr == TaskPtr){//Ҫȡ�������ǵ�һ������
				PendPtr = TaskPtr -> RdyNextPtr ;
			}
			TaskPtr -> RdyNextPtr -> RdyPrevPtr = TaskPtr -> RdyPrevPtr ;
			TaskPtr -> RdyPrevPtr -> RdyNextPtr = TaskPtr -> RdyNextPtr ;
		}
		
		TaskPtr -> RdyNextPtr = NULL;
		TaskPtr -> RdyPrevPtr = NULL;
		TF = TRUE ;
	}
	OSCTask::TaskUnlock();
	return TF;	
}
//ʱ���������
bool  OSCPendTime::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Time){//��ʱ�������
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_Time ;//���������״̬
	Count ++ ;//��������һ
	if(1 == Count){//������Ϊ��
		PendPtr = TaskPtr ;//��������
		TaskPtr -> TimeNextPtr = TaskPtr ;//ָ���Լ�
		TaskPtr -> TimePrevPtr = TaskPtr;//
	}else{
		TaskPtr -> TimeNextPtr = PendPtr ;
		TaskPtr -> TimePrevPtr = PendPtr -> TimePrevPtr ;
		PendPtr -> TimePrevPtr -> TimeNextPtr = TaskPtr ;
		PendPtr -> TimePrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//ʱ����г���
bool  OSCPendTime::ListOut(OSCTask*TaskPtr){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Time){
	TaskPtr -> TaskStat  &=  ~Task_Stat_Time ;
	Count --;
	if(0 == Count){//һ������
		PendPtr = NULL ;
	}else{//�������ϵ�����
		if(PendPtr == TaskPtr){//Ҫȡ�������ǵ�һ������
			PendPtr = TaskPtr -> TimeNextPtr ;
		}
		TaskPtr -> TimeNextPtr -> TimePrevPtr = TaskPtr -> TimePrevPtr ;
		TaskPtr -> TimePrevPtr -> TimeNextPtr = TaskPtr -> TimeNextPtr ;
	}
	TaskPtr -> TimeNextPtr = NULL;
	TaskPtr -> TimePrevPtr = NULL;
	TF =  TRUE ;
	}
	OSCTask::TaskUnlock();
	return TF;
}
//�¼���������
bool  OSCPendEvent::ListIn(OSCTask*TaskPtr){
	 OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Event){
		OSCTask::TaskUnlock();
	 	return FALSE;
	 }
	TaskPtr -> TaskStat |= Task_Stat_Event ;//���������״̬
	Count ++ ;//��������һ
	if(1 == Count){//������Ϊ��
		PendPtr = TaskPtr ;//��������
		TaskPtr -> EventNextPtr = TaskPtr ;//ָ���Լ�
		TaskPtr -> EventPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> EventNextPtr = PendPtr ;
		TaskPtr -> EventPrevPtr = PendPtr -> EventPrevPtr ;
		PendPtr -> EventPrevPtr -> EventNextPtr = TaskPtr ;
		PendPtr -> EventPrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//�¼����г���
bool  OSCPendEvent::ListOut(OSCTask*TaskPtr){
	bool TF =  FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Event){
	TaskPtr -> TaskStat  &=  ~Task_Stat_Event ;
	Count --;
	if(0 == Count){//һ������
		PendPtr = NULL ;
	}else{//�������ϵ�����
		if(PendPtr == TaskPtr){//Ҫȡ�������ǵ�һ������
			PendPtr = TaskPtr -> EventNextPtr ;
		}
		TaskPtr -> EventNextPtr -> EventPrevPtr = TaskPtr -> EventPrevPtr ;
		TaskPtr -> EventPrevPtr -> EventNextPtr = TaskPtr -> EventNextPtr ;
	}
	
	TaskPtr -> EventNextPtr = NULL;
	TaskPtr -> EventPrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;
}


///PendPd
bool  OSCPendPd::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat &Task_Stat_Pd){
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_Pd ;//���������״̬
	Count ++ ;//��������һ
	if(1 == Count){//������Ϊ��
		PendPtr = TaskPtr ;//��������
		TaskPtr -> PdNextPtr = TaskPtr ;//ָ���Լ�
		TaskPtr -> PdPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> PdNextPtr = PendPtr ;
		TaskPtr -> PdPrevPtr = PendPtr -> PdPrevPtr ;
		PendPtr -> PdPrevPtr -> PdNextPtr = TaskPtr ;
		PendPtr -> PdPrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//������г���
bool  OSCPendPd::ListOut(OSCTask*TaskPtr){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_Pd){
	TaskPtr -> TaskStat  &=  ~Task_Stat_Pd ;
	Count --;
	if(0 == Count){//һ������
		PendPtr = NULL ;
	}else{//�������ϵ�����
		if(PendPtr == TaskPtr){//Ҫȡ�������ǵ�һ������
			PendPtr = TaskPtr -> PdNextPtr ;
		}
		TaskPtr -> PdNextPtr -> PdPrevPtr = TaskPtr -> PdPrevPtr ;
		TaskPtr -> PdPrevPtr -> PdNextPtr = TaskPtr -> PdNextPtr ;
	}
	TaskPtr -> PdNextPtr = NULL;
	TaskPtr -> PdPrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;
}

//
bool  OSCPendThreadTick::ListIn(OSCTask*TaskPtr){
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat &Task_Stat_ThreadTick){
		OSCTask::TaskUnlock();
		return FALSE;
	}
	TaskPtr -> TaskStat |= Task_Stat_ThreadTick ;//���������״̬
	Count ++ ;//��������һ
	if(1 == Count){//������Ϊ��
		PendPtr = TaskPtr ;//��������
		TaskPtr -> ThreadTickNextPtr = TaskPtr ;//ָ���Լ�
		TaskPtr -> ThreadTickPrevPtr = TaskPtr;//
	}else{
		TaskPtr -> ThreadTickNextPtr = PendPtr ;
		TaskPtr -> ThreadTickPrevPtr = PendPtr -> ThreadTickPrevPtr ;
		PendPtr -> ThreadTickPrevPtr -> ThreadTickNextPtr = TaskPtr ;
		PendPtr -> ThreadTickPrevPtr = TaskPtr ;
	}
	OSCTask::TaskUnlock();
	return TRUE;
}
//������г���
bool  OSCPendThreadTick::ListOut(OSCTask*TaskPtr){
	bool  TF = FALSE;
	OSCTask::TaskLock();
	if(TaskPtr -> TaskStat & Task_Stat_ThreadTick){
	TaskPtr -> TaskStat  &=  ~Task_Stat_ThreadTick ;
	Count --;
	if(0 == Count){//һ������
		PendPtr = NULL ;
	}else{//�������ϵ�����
		if(PendPtr == TaskPtr){//Ҫȡ�������ǵ�һ������
			PendPtr = TaskPtr -> ThreadTickNextPtr ;
		}
		TaskPtr -> ThreadTickNextPtr -> ThreadTickPrevPtr = TaskPtr -> ThreadTickPrevPtr ;
		TaskPtr -> ThreadTickPrevPtr -> ThreadTickNextPtr = TaskPtr -> ThreadTickNextPtr ;
	}
	TaskPtr -> ThreadTickNextPtr = NULL;
	TaskPtr -> ThreadTickPrevPtr = NULL;
	TF = TRUE;
	}
	OSCTask::TaskUnlock();
	return TF;
}
//END