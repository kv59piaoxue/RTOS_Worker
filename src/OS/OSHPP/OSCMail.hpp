#ifndef  __OSCMAIL_HPP
#define __OSCMAIL_HPP
	//Worker1.0
	//���ߣ�л����
	//QQ��931141594��934854563
	//���䣺kv59piaoxue@163.com
	//�绰��
	//ʱ�䣺2010��
class  OSCMail :public  OSCEvent{
		private:
		OSCMsg   TaskMsg;//��Ϣ����
		public:
		static   OSCMail *MailID;//����ʹ�õı���
		OSCMail(){
			MailID = this;
		}
		//�еȴ�
		bool		MsgGetSwap(OSMsg *Msg,uint32 Dly);//
		bool  	MsgPutSwap(OSMsg *Msg);//
		bool		MsgTeleviseSwap(OSMsg*Msg);//��ָ���ĵȴ���Ϣ�����н�����Ϣ�㲥
		//�޵ȴ�
		bool		MsgGetMedi(OSMsg *Msg);//
		bool  	MsgPutMedi(OSMsg  *Msg);//
		bool		MsgTeleviseMedi(OSMsg*Msg);//��ָ���ĵȴ���Ϣ�����н�����Ϣ�㲥
};

#endif
//END
