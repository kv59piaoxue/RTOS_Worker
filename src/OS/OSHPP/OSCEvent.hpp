#ifndef  __OSCEVENT_HPP
#define __OSCEVENT_HPP

	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
class OSCEvent{	
	private:
	OSCPendEvent   ArryEvent[DF_MINPRIO];//
	uint32    PrioIndex;
	protected:
	OSCEvent(){
		PrioIndex = 0;
	}	
	bool		EventPend(OSCTask*TaskP,uint32  Dly,uint32  TaskInfo);//����ȴ�
	OSCTask*EventResume(uint32  TaskInfo);//�ȴ����лָ�һ������
	bool		EventListOut(OSCTask*TaskP);//��������ʱ
};

#endif
//END 
