#include "CSendMsg.h"
#include "CLiDataStruct.h"
#include "GloabeParams.h"

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CSendMsg::CSendMsg(io_service& io,short port1,QObject *parent)
	: QObject(parent),ios(io),port(port1),acceptor(ios,ip::tcp::endpoint(ip::tcp::v4(),port))
	,flagWrite(true)
{
	ip="";
}

CSendMsg::~CSendMsg()
{

}


void CSendMsg::connect()
{
	try
	{
		ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"),port);
		sock->connect(ep);
	}
	catch(std::exception& e)
	{
		//std::cout<<e.what()<<std::endl;
		//QString content=QString(tr("%1")).arg(e.what());
		QString content=QString::fromLocal8Bit(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
	}
}
			

void CSendMsg::startListen()
{
	while(true)
	{
	try
	{
		sock=sock_pt(new ip::tcp::socket(ios));
		acceptor.accept(*sock);

		ip=sock->remote_endpoint().address().to_string();

		QString content=QString(tr("已连接"));
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		std::string message=ip+" "+numToStr(port)+"已连接";
		writeLog(message,es);

		while(flagWrite)
		{
			LiSendS liSendS;
			liSendS.head=0xaabbccdd;
			liSendS.cmdType=0;
			liSendS.ruleId=0;
	
			//发li_filter
			int lssLen=sizeof(LiSendS);
			char* pSendBuff=new char[lssLen];
			memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
			bool ret=write(pSendBuff,lssLen);

			if(ret)
			{
				//content=QString(tr("心跳"));
				//relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
				//idR->insertLog(relateDevice,content);
			}

			delete [] pSendBuff;

			boost::posix_time::time_duration td(0,0,2,0);
			boost::this_thread::sleep(td);
		}

		content=QString(tr("断开连接"));
		relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		message=ip+" "+numToStr(port)+"断开连接";
		writeLog(message,es);

		flagWrite=true;
	}
	catch(std::exception& e)
	{
		//std::cout<<e.what()<<std::endl;
		//std::string message=numToStr(port)+""+e.what();
		//writeLog(message,es);

		//QString content=QString(tr("%1")).arg(e.what());
		QString content=QString::fromLocal8Bit(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
	}
	}
}

//void CSendMsg::write(std::string& str)
bool CSendMsg::write(char* pSendBuff,int len)
{
	boost::mutex::scoped_lock lock(mtx_buff);
	//std::cout<<"send msg complete."<<std::endl;
	//sock->write_some(buffer("hello Li"));
	try   
	{ 
		//sock->write_some(buffer(str));
		sock->write_some(buffer(pSendBuff,len));

		return true;
	}
	catch (std::exception& e)   
	{   
		//std::cerr << "Exception: " << e.what() << "/n"; 
		//std::string message1=numToStr(port)+" "+e.what();
		//writeLog(message1,es);

		QString content=QString::fromLocal8Bit(e.what());
		//QString content=QString(tr("%1")).arg(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		flagWrite=false;

		return false;
	}   
}

void CSendMsg::read(std::string& str)
{					
	//std::cout<<"send msg complete."<<std::endl;
	//sock->write_some(buffer("hello Li"));
	try		
	{ 
		//sock->read_some(buffer(pSendBuff,len));
	}
	catch (std::exception& e)   
	{   
		std::cerr << "Exception: " << e.what() << "/n"; 
		//std::string message1(e.what());
		//std::string message=""+message1;
		//writeLog(message,es);
	}   
}