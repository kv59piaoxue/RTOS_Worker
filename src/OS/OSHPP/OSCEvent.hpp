#ifndef  __OSCEVENT_HPP
#define __OSCEVENT_HPP

	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
class OSCEvent{	
	private:
	OSCPendEvent   ArryEvent[DF_MINPRIO];//
	uint32    PrioIndex;
	protected:
	OSCEvent(){
		PrioIndex = 0;
	}	
	bool		EventPend(OSCTask*TaskP,uint32  Dly,uint32  TaskInfo);//任务等待
	OSCTask*EventResume(uint32  TaskInfo);//等待队列恢复一个任务
	bool		EventListOut(OSCTask*TaskP);//任务任务超时
};

#endif
//END 
