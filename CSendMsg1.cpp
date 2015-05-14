#include "CSendMsg1.h"
#include "CLiDataStruct.h"
#include "GloabeParams.h"
#include <QObject>

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CSendMsg1::CSendMsg1(io_service& io,std::string ip1,short port1,QObject *parent)
	: QObject(parent),ios(io),ip(ip1),port(port1),sock(new ip::tcp::socket(ios))
{

}

CSendMsg1::~CSendMsg1()
{

}

void timeoutAction(CSendMsg1* sendMsg1)
{
	try
	{
		//boost::asio::io_service io1;
		//boost::asio::deadline_timer t(io1, boost::posix_time::seconds(120));
		//t.wait();
		boost::this_thread::sleep( boost::posix_time::seconds(120));
		sendMsg1->disConn();

		QString content=QString(QObject::tr("发送接收gao消息超时"));
		QString relateDevice=QString("%1 %2").arg(sendMsg1->ip.c_str()).arg(sendMsg1->port);
		idR->insertLog(relateDevice,content);
	}
	catch(boost::thread_interrupted&)
	{

	}
}

void CSendMsg1::connect()
{

	try
	{
		
		ip::tcp::endpoint ep(ip::address::from_string(ip),port);
		sock->connect(ep);
	}
	catch(std::exception& e)
	{
		//std::cout<<e.what()<<std::endl;
		std::string str=e.what();
		QString content=QString::fromLocal8Bit(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
	}

}

void CSendMsg1::disConn()
{
	try
	{
		
		sock->close();
	}
	catch(std::exception& e)
	{
		//std::cout<<e.what()<<std::endl;
		QString content=QString::fromLocal8Bit(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
	}
}
			

bool CSendMsg1::write(std::string& str)
{
	//boost::thread thread1(timeoutAction,this);

	try   
	{ 
		sock->write_some(buffer(str));
	}
	catch (std::exception& e)   
	{   
		//std::cerr << "Exception: " << e.what() << "/n"; 
		//std::string message1=numToStr(port)+" "+e.what();
		//writeLog(message1,es);
		QString content=QString::fromLocal8Bit(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		return false;
	}   

	//thread1.interrupt();
	return true;
}



bool CSendMsg1::read()
{		
	int len=1024;
	char* pRecvBuff=new char[len];
	bool ret=false;
	Json::Reader reader;
	Json::Value value;
	boost::thread thread1(timeoutAction,this);

	try		
	{ 
		sock->read_some(buffer(pRecvBuff,len));

		if (reader.parse(pRecvBuff, value))
		{
			if(value["Status"].asString()=="OK")
			{
				ret= true;
			}
			else
			{
				ret= false;
			}
		}
	}
	catch (std::exception& e)   
	{   
		//std::cerr << "Exception: " << e.what() << "/n"; 
		//std::string message1=numToStr(port)+" "+e.what();
		//writeLog(message1,es);
		QString content=QString::fromLocal8Bit(e.what());
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		ret= false;
	} 

	delete [] pRecvBuff;

	thread1.interrupt();

	return ret;
}