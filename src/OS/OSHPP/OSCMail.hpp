#ifndef  __OSCMAIL_HPP
#define __OSCMAIL_HPP
	//Worker1.0
	//作者：谢国经
	//QQ：931141594、934854563
	//邮箱：kv59piaoxue@163.com
	//电话：
	//时间：2010年
class  OSCMail :public  OSCEvent{
		private:
		OSCMsg   TaskMsg;//消息队列
		public:
		static   OSCMail *MailID;//调试使用的变量
		OSCMail(){
			MailID = this;
		}
		//有等待
		bool		MsgGetSwap(OSMsg *Msg,uint32 Dly);//
		bool  	MsgPutSwap(OSMsg *Msg);//
		bool		MsgTeleviseSwap(OSMsg*Msg);//在指定的等待消息队列中进行消息广播
		//无等待
		bool		MsgGetMedi(OSMsg *Msg);//
		bool  	MsgPutMedi(OSMsg  *Msg);//
		bool		MsgTeleviseMedi(OSMsg*Msg);//在指定的等待消息队列中进行消息广播
};

#endif
//END
