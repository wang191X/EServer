#include "RecvMsgLiSave.h"
#include "WFCLPAPI.h"
#include "GloabeParams.h"
#include "CIdResource.h"

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CRecvMsgLiSave::CRecvMsgLiSave(io_service& io,short port1,QObject *parent)
: CRecvMsg(io,port1,parent)
{
	recvLen=sizeof(LiSaveS);
}

CRecvMsgLiSave::~CRecvMsgLiSave(void)
{
}

void CRecvMsgLiSave::accept_handler(const boost::system::error_code& ec,sock_pt sock)
{
	if(ec)
	{
		ip=sock->remote_endpoint().address().to_string();
		std::string message=ip+" "+numToStr(port)+"连接异常";
		writeLog(message,es);
		return;
	}

	start();

	ip=sock->remote_endpoint().address().to_string();
	QString content=QString(tr("已连接"));
	QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
	idR->insertLog(relateDevice,content);

	//ip=sock->remote_endpoint().address().to_string();
	std::string message=ip+" "+numToStr(port)+"已连接";
	writeLog(message,es);

	try{
		sock->async_read_some(boost::asio::buffer(pRecvBuff, recvLen),
			bind(&CRecvMsg::read_handler, this,
			placeholders::error,
			placeholders::bytes_transferred,sock));

		//boost::posix_time::time_duration td(0,0,2,0);
		//boost::this_thread::sleep(td);
	}
	catch (std::exception& e)   
	{   
		//std::cerr << "Exception: " << e.what() << "/n";   
		std::string message=ip+" "+numToStr(port)+" "+e.what();
		writeLog(message,es);
	}
}

void CRecvMsgLiSave::read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock)
{
	if (ec)
	{
		//std::string message1="卡1读问题";
		//writeLog(message1,es);
		//std::string ip=sock->remote_endpoint().address().to_string();
		//std::string message=numToStr(port)+"读异常";
		//writeLog(message,es);

		ip=sock->remote_endpoint().address().to_string();
		QString content=QString(tr("读异常"));
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		std::string message=ip+" "+numToStr(port)+"读异常";
		writeLog(message,es);

		return;
	}

	/*ip=sock->remote_endpoint().address().to_string();
	std::string message=ip+" "+numToStr(port)+"已连接";
	writeLog(message,es);*/

	int recv_len=bytes_transferred;

	if(recv_len!=0)
	{
		liSaveS=new LiSaveS();
		int ssLen=sizeof(LiSaveS);
		memcpy_s(liSaveS,ssLen,pRecvBuff,ssLen);

		int ruleID=uploadRuleLiSave(liSaveS);
		
		LiSendS liSendS;
		liSendS.head=0xaabbccdd;
		liSendS.cmdType=1;
		liSendS.ruleId=ruleID;
		int lssLen=sizeof(LiSendS);
		char* pSendBuff=new char[lssLen];
		memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
		sendLiSave->write(pSendBuff,lssLen);
		delete pSendBuff;
	}

	try{
			sock->async_read_some(boost::asio::buffer(pRecvBuff, recvLen),
				bind(&CRecvMsg::read_handler, this,
				placeholders::error,
				placeholders::bytes_transferred,sock));

			/*QString content=QString(tr("线程状态正常"));
	QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
	idR->insertLog(relateDevice,content);*/

			//boost::posix_time::time_duration td(0,0,2,0);
			//boost::this_thread::sleep(td);
	}
	catch (std::exception& e)   
	{   
		//std::cerr << "Exception: " << e.what() << "/n";   
		//std::string message=numToStr(port)+" "+e.what();
		//writeLog(message,es);

		QString content=QString(tr("%1")).arg(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
	}   
}

int CRecvMsgLiSave::uploadRuleLiSave(LiSaveS* liSaveS)
{
	
}