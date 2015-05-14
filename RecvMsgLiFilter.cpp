#include "RecvMsgLiFilter.h"
#include "WFCLPAPI.h"
#include "GloabeParams.h"
#include "CIdResource.h"

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CRecvMsgLiFilter::CRecvMsgLiFilter(io_service& io,short port1,QObject *parent)
: CRecvMsg(io,port1,parent),recvLen(21)
{
}

CRecvMsgLiFilter::~CRecvMsgLiFilter(void)
{
}

void CRecvMsgLiFilter::procBuff(int recv_len)
{
	procLen=tmpLen+recv_len;
	pProcBuff=new char[procLen];
	memcpy_s(pProcBuff,tmpLen,pProcTmp,tmpLen);
	memcpy_s(pProcBuff+tmpLen,recv_len,pRecvBuff,recv_len);

	int headLen=4;
	unsigned int headflag=0;

	if(pProcTmp)
	{
		delete [] pProcTmp;
		pProcTmp=NULL;
	}

	int leftLen=procLen;

	while(true)
	{
		if(leftLen<headLen)
		{
			pProcTmp=new char[leftLen];
			memcpy_s(pProcTmp,leftLen,pProcBuff+(procLen-leftLen),leftLen);
			tmpLen=leftLen;

			break;
		}

		memcpy_s(&headflag,headLen,pProcBuff+(procLen-leftLen),headLen);

		//if(selfHead->flag!=0xaabbccdd||selfHead->dataLength>256)
		if(headflag!=0xaabbccdd)
		{
			leftLen=leftLen-1;

			//QString content=tr("丢失同步头");
			//QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			//idR->insertLog(relateDevice,content);

			continue;
		}

		leftLen=leftLen-headLen;
		int packetlen=17;

		if((leftLen<packetlen))
		{
			pProcTmp=new char[leftLen+headLen];
			memcpy_s(pProcTmp,leftLen+headLen,pProcBuff+(procLen-leftLen-headLen),leftLen+headLen);
			tmpLen=leftLen+headLen;

			break;
		}

		LiFilterS liFilterS;
		memcpy_s(&liFilterS,packetlen,pProcBuff+(procLen-leftLen),packetlen);
		proData(liFilterS);

		leftLen=leftLen-packetlen;
	}

	delete [] pProcBuff;
}

void CRecvMsgLiFilter::proData(LiFilterS liFilterS)
{
	int lfsLen=sizeof(LiFilterS);
	memcpy_s(&liFilterS,lfsLen,pRecvBuff,lfsLen);

	int ruleID=uploadRuleLiFilter(liFilterS);

	LiSendS liSendS;
	liSendS.head=0xaabbccdd;
	liSendS.cmdType=1;
	liSendS.ruleId=ruleID;
	int lssLen=sizeof(LiSendS);
	char* pSendBuff=new char[lssLen];
	memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
	sendLiFilter->write(pSendBuff,lssLen);
	delete pSendBuff;
}

void CRecvMsgLiFilter::accept_handler(const boost::system::error_code& ec,sock_pt sock)
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

void CRecvMsgLiFilter::read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock)
{
	if (ec)
	{
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

	//ip=sock->remote_endpoint().address().to_string();
	//std::string message=ip+" "+numToStr(port)+"已连接";
	//writeLog(message,es);

	int recv_len=bytes_transferred;

	try{

		if(recv_len!=0)
		{
			procBuff(recv_len);
		}

	sock->async_read_some(boost::asio::buffer(pRecvBuff, recvLen),
			bind(&CRecvMsgLiFilter::read_handler, this,
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

std::string CRecvMsgLiFilter::intToIp(int value)
{

	return message1;
}

int CRecvMsgLiFilter::uploadRuleLiFilter(LiFilterS liFilterS)
{

	SACL sacl;
	sacl.id=assignACLID();

	
	wfclpapi->sacl=sacl;
	int ret=wfclpapi->uploadACL();	

	return ruleId;
}