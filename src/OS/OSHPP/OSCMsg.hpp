
#ifndef  __OSCMSG_HPP
#define __OSCMSG_HPP

	class  OSMsg{
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
		public :
		uint32	MsgID;
		WPARAM  	wParam;
		LPARAM	lParam;
		OSCTask *STaskPtr;//发送消息的任务，为NULL表示中断发送
		OSMsg(OSCTask*STaskP = NULL,uint32  TMsg = 0,WPARAM wp = 0,LPARAM lp = 0){
			STaskPtr = STaskP;
			MsgID = TMsg;
			wParam = wp;
			lParam = lp;
		}
	};//承载消息的小车
class OSCMsg{
	protected:
		OSMsg	MsgArry[DF_MsgLength];
		uint32 		Front,Rear;
	public:
		bool 	IsEmpty(){ return  Front == Rear;}
		bool		IsFull(){ return Front == (Rear + 1) % DF_MsgLength;}
		bool		Add(OSMsg  *TMsg);
		bool 	Delete(OSMsg * TMsg);	
		OSCMsg();//
};

class      OSCIntMsg:public  OSCMsg{
	public:
		bool		Add(OSMsg  *TMsg);
		bool 	Delete(OSMsg * TMsg);	
		OSCIntMsg(){};
};
#endif
//END
