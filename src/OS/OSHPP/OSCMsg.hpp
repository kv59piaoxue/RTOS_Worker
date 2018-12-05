
#ifndef  __OSCMSG_HPP
#define __OSCMSG_HPP

	class  OSMsg{
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
		public :
		uint32	MsgID;
		WPARAM  	wParam;
		LPARAM	lParam;
		OSCTask *STaskPtr;//������Ϣ������ΪNULL��ʾ�жϷ���
		OSMsg(OSCTask*STaskP = NULL,uint32  TMsg = 0,WPARAM wp = 0,LPARAM lp = 0){
			STaskPtr = STaskP;
			MsgID = TMsg;
			wParam = wp;
			lParam = lp;
		}
	};//������Ϣ��С��
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
