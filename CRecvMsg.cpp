#include "CRecvMsg.h"
#include "WFCLPAPI.h"
#include "GloabeParams.h"
#include "CIdResource.h"

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CRecvMsg::CRecvMsg(io_service& io,short port1,QObject *parent)
:QObject(parent),port(port1),ios(io),acceptor(ios,ip::tcp::endpoint(ip::tcp::v4(),port))
,max_length(1024)
{
	ip="";
	procLen=0;
	tmpLen=0;
	pProcTmp=NULL;
	pRecvBuff=new char[max_length];
}

CRecvMsg::~CRecvMsg()
{

}

void CRecvMsg::start()
{
	try
	{
		sock=sock_pt(new ip::tcp::socket(ios));
		acceptor.async_accept(*sock,bind(&CRecvMsg::accept_handler,this,placeholders::error,sock));
	}
	catch (std::exception& e)   
	{ 
		std::string message=ip+" "+numToStr(port)+" "+e.what();
		writeLog(message,es);
	}
}

void CRecvMsg::fillBuff(char* pSendBuff1,int len)
{
	boost::mutex::scoped_lock lock(mtx_buff);
	pSendBuff=pSendBuff1;
	sendLen=len;
}

void CRecvMsg::accept_handler(const boost::system::error_code& ec,sock_pt sock)
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

	QString content=tr("已连接");
	QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
	idR->insertLog(relateDevice,content);
	
	std::string message=ip+" "+numToStr(port)+"已连接";
	writeLog(message,es);

	try{
		sock->async_read_some(boost::asio::buffer(pRecvBuff, 1024),
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

void CRecvMsg::read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock)
{
	//if (ec)
	//{
	//	//std::string message1="卡1读问题";
	//	//writeLog(message1,es);
	//	//std::string ip=sock->remote_endpoint().address().to_string();
	//	std::string message=numToStr(port)+"读异常";
	//	writeLog(message,es);
	//	return;
	//}

	//int recv_len=bytes_transferred;

	//if(recv_len!=0)
	//{
	//	//liRevS=new LiRevS();
	//	//memcpy_s(liRevS,recv_len,pRecvBuff,recv_len);

	//	//int ruleID=uploadRule(liRevS);
	//	
	//	LiSendS liSendS;
	//	liSendS.head=0xaabbccdd;
	//	liSendS.cmdType=1;
	//	//liSendS.ruleId=ruleID;
	//	int lssLen=sizeof(LiSendS);
	//	char* pSendBuff=new char[lssLen];
	//	memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
	//	
	//	if(port==P_LI_FILTER_RECV)
	//	{
	//		sendLiFilter->write(pSendBuff,lssLen);
	//	}else if(port==P_LI_SAVE_RECV)
	//	{
	//		sendLiSave->write(pSendBuff,lssLen);
	//	}

	//	delete pSendBuff;
	//}

	//try{
	//		sock->async_read_some(boost::asio::buffer(pRecvBuff, 1024),
	//			bind(&CRecvMsg::read_handler, this,
	//			placeholders::error,
	//			placeholders::bytes_transferred,sock));

	//		//boost::posix_time::time_duration td(0,0,2,0);
	//		//boost::this_thread::sleep(td);
	//}
	//catch (std::exception& e)   
	//{   
	//	//std::cerr << "Exception: " << e.what() << "/n";   
	//	std::string message=numToStr(port)+" "+e.what();
	//	writeLog(message,es);
	//}   
}


int CRecvMsg::write(char* pSendBuff,int len)
{
	//std::cout<<"send msg complete."<<std::endl;
	//sock->write_some(buffer("hello Li"));
	try   
	{ 
		//std::string str="hello Li";
		//sock->write_some(buffer(str));
		//sock->write_some(buffer(pSendBuff,len));
		sock->async_write_some(buffer(pSendBuff,len),bind(&CRecvMsg::write_handler,this,placeholders::error,sock));
		return 0;
	}
	catch (std::exception& e)   
	{   
		//std::cout<<e.what()<<std::endl;
		//	std::string message1=e.what();
		//	writeLog(message1,es);
			//flagWrite=false;
			return -1;
	}  

	return 0;
}

void CRecvMsg::write_handler(const boost::system::error_code& error,sock_pt sock)
{
	if (error)
	{
		return;
	}
}

int CRecvMsg::assignACLID()
{
	return 0;
}

unsigned int CRecvMsg::intToNetwork(int param)
{
	return 0;
}

unsigned short CRecvMsg::shortToNetwork(short param)
{
	return 0;
}