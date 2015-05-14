#include "CRecvMsg1.h"
#include "WFCLPAPI.h"
#include "GloabeParams.h"
#include "CIdResource.h"
#include "CGaoDataStruct.h"
#include <QString>
#include "fclpapi.h"

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CRecvMsg1::CRecvMsg1(io_service& io,short port1,QObject *parent)
	: CRecvMsg(io,port1,parent)
{

}

CRecvMsg1::~CRecvMsg1()
{

}

void CRecvMsg1::jsonProc(char* pJsonBuff)
{
	Json::Reader reader;
	Json::Value value;

	if (reader.parse(pJsonBuff, value))
	{
		if(value["Method"].asString()=="SelfStudy")
		{
			int ruleID=uploadRuleG(value);
			//int ruleID=100;

			QString content=QString(tr("id:%1，准备发送至lifilter")).arg(ruleID);
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);

			LiSendS liSendS;
			liSendS.head=0xaabbccdd;
			liSendS.cmdType=1;
			liSendS.ruleId=ruleID;
			int lssLen=sizeof(LiSendS);
			char* pSendBuff=new char[lssLen];
			memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
			bool ret=sendLiFilter->write(pSendBuff,lssLen);

			if(ret)
			{
				content=QString(tr("id:%1，成功发送至lifilter")).arg(ruleID);
				relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
				idR->insertLog(relateDevice,content);
			}

			delete pSendBuff;
		}
	}
}

void CRecvMsg1::ProcBuff(int recv_len)
{
	procLen=tmpLen+recv_len;
	pProcBuff=new char[procLen];
	memcpy_s(pProcBuff,tmpLen,pProcTmp,tmpLen);
	memcpy_s(pProcBuff+tmpLen,recv_len,pRecvBuff,recv_len);

	int headLen=sizeof(SelfStudyHead);
	SelfStudyHead* selfHead=new SelfStudyHead();

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

		memcpy_s(selfHead,headLen,pProcBuff+(procLen-leftLen),headLen);

		if(selfHead->flag!=0xaabbccdd||selfHead->dataLength>256)
		{
			leftLen=leftLen-1;

			//QString content=tr("丢失同步头");
			//QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			//idR->insertLog(relateDevice,content);

			continue;
		}

		leftLen=leftLen-headLen;
		int packetlen=selfHead->dataLength;

		if((leftLen<packetlen))
		{
			pProcTmp=new char[leftLen+headLen];
			memcpy_s(pProcTmp,leftLen+headLen,pProcBuff+(procLen-leftLen-headLen),leftLen+headLen);
			tmpLen=leftLen+headLen;

			break;
		}

		char* pTmp1=new char[packetlen];
		memcpy_s(pTmp1,packetlen,pProcBuff+(procLen-leftLen),packetlen);

		jsonProc(pTmp1);

		leftLen=leftLen-packetlen;
		delete [] pTmp1;
	}

	delete selfHead;
	delete [] pProcBuff;
}

void CRecvMsg1::read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock)
{
	if (ec)
	{
		ip=sock->remote_endpoint().address().to_string();
		std::string message=ip+" "+numToStr(port)+"读异常";
		writeLog(message,es);

		QString content=tr("读异常");
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return;
	}

	int recv_len=bytes_transferred;

	ip=sock->remote_endpoint().address().to_string();
	QString content=tr("接收字节数：%1").arg(recv_len);
	QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
	idR->insertLog(relateDevice,content);
	
	if(recv_len!=0)
	{
		ProcBuff(recv_len);
	}

	/*if(recv_len!=0)
	{
		Json::Reader reader;
		Json::Value value;

		if (reader.parse(pRecvBuff, value))
		{
			if(value["Method"].asString()=="SelfStudy")
			{
				int ruleID=uploadRuleG(value);

				QString content=QString(tr("id:%1，准备发送至lifilter"));
				QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
				idR->insertLog(relateDevice,content);

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
		}
	}*/

	try{
			sock->async_read_some(boost::asio::buffer(pRecvBuff, 1024),
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

int CRecvMsg1::uploadRuleG(Json::Value& value)
{
	return 0;
}

