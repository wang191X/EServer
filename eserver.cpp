#include "eserver.h"
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>
#include "CUsrClient.h"
#include "GloabeParams.h"
//#include "CLiClientSave.h"
#include "CUpdateIdPool.h"
#include "CSendMsg.h"
#include "WFCLPAPI.h"

using namespace boost::asio;

//boost::mutex mtx;

std::string get_sysdate(void)
{
	struct tm today;
	time_t ltime;
	time( &ltime );
	char tmpbuf[50];
	_localtime64_s( &today, &ltime );
	strftime( tmpbuf, 50,"%Y年%m月%d日 %H:%M:%S", &today );
	std::string localtime(tmpbuf);
	return localtime;
}

void writeLog(std::string& message,EServer* es)
{
	using namespace std;
	boost::mutex::scoped_lock lock(g_mtx_log);

	message=get_sysdate()+" "+message;
	/*ofstream ofile;
	ofile.open("ccol_log.txt",ios_base::app);
	ofile<<message<<endl;
	ofile.close();*/

	es->ui.listLog->addItem (QString::fromLocal8Bit(message.c_str()));
}	

std::string numToStr(int id)
{
	using namespace std;

	stringstream strStream;
	strStream<<id;
	string message1 = strStream.str();

	return message1;
}

int strToNum(std::string str)
{
	using namespace std;

	stringstream strStream;
	strStream<<str;
	int num;
	strStream>>num;

	return num;
}

void recvClientTrd(EServer* es)
{
	io_service ios;
	CUsrClient server(ios);
	server.es=es;
	server.start();

	ios.run();
}

void recvLiFilterTrd(EServer* es)
{
	//io_service ios;
	//serveLiFilter=new CLiClientFilter(ios);
	//serveLiFilter->es=es;
	//serveLiFilter->startSyn();

	////serveLiFilter->start1();
	////ios.run();

	io_service ios;
	recvLiFilter=new CRecvMsgLiFilter(ios,P_LI_FILTER_RECV);
	recvLiFilter->es=es;
	recvLiFilter->start();
	ios.run();
}

void sendLiFilterTrd(EServer* es)
{
	io_service ios;
	sendLiFilter=new CSendMsg(ios,P_LI_FILTER_SEND);
	sendLiFilter->es=es;
	sendLiFilter->startListen();
	//ios.run();
}

void recvLiSaveTrd(EServer* es)
{
	io_service ios;
	recvLiSave=new CRecvMsgLiSave(ios,P_LI_SAVE_RECV);
	recvLiSave->es=es;
	recvLiSave->start();
	ios.run();
}

void sendLiSaveTrd(EServer* es)
{
	io_service ios;
	sendLiSave=new CSendMsg(ios,P_LI_SAVE_SEND);
	sendLiSave->es=es;
	sendLiSave->startListen();
	//ios.run();
}

void recvGaoTrd_200(EServer* es)
{
	io_service ios;
	recvGao_200=new CRecvMsg1(ios,P_GAO_RECV_200);
	recvGao_200->es=es;
	recvGao_200->start();
	ios.run();
}

void recvGaoTrd_201(EServer* es)
{
	io_service ios;
	recvGao_201=new CRecvMsg1(ios,P_GAO_RECV_201);
	recvGao_201->es=es;
	recvGao_201->start();
	ios.run();
}

void recvGaoTrd_202(EServer* es)
{
	io_service ios;
	recvGao_202=new CRecvMsg1(ios,P_GAO_RECV_202);
	recvGao_202->es=es;
	recvGao_202->start();
	ios.run();
}

void recvGaoTrd_203(EServer* es)
{
	io_service ios;
	recvGao_203=new CRecvMsg1(ios,P_GAO_RECV_203);
	recvGao_203->es=es;
	recvGao_203->start();
	ios.run();
}

void recvGaoTrd_204(EServer* es)
{
	io_service ios;
	recvGao_204=new CRecvMsg1(ios,P_GAO_RECV_204);
	recvGao_204->es=es;
	recvGao_204->start();
	ios.run();
}

void recvGaoTrd_205(EServer* es)
{
	io_service ios;
	recvGao_205=new CRecvMsg1(ios,P_GAO_RECV_205);
	recvGao_205->es=es;
	recvGao_205->start();
	ios.run();
}

void comTrd(EServer* es)
{
	io_service ios;
	CUpdateIdPool* updateIdPool=new CUpdateIdPool(ios);
	ios.run();
}


EServer::EServer(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	ui.setupUi(this);

	flags=Qt::Dialog;

	flags|=Qt::WindowMinMaxButtonsHint;
	setWindowFlags(flags);

	connect(ui.pbBegin, SIGNAL( clicked() ), this, SLOT(beginServer()));        
	connect(ui.pbStop, SIGNAL( clicked() ), this, SLOT(stopServer()));       
	connect(ui.pbClear, SIGNAL( clicked() ), this, SLOT(clearLog()));
	/*connect(ui.pbStatus, SIGNAL( clicked() ), this, SLOT(queryStatus()));   
	connect(ui.pbClient, SIGNAL( clicked() ), this, SLOT(beginClientThread())); 
	connect(ui.pbTimer, SIGNAL( clicked() ), this, SLOT(beginTimerThread()));
	
	connect(ui.pbG0, SIGNAL( clicked() ), this, SLOT(beginG0Thread()));       
	connect(ui.pbG1, SIGNAL( clicked() ), this, SLOT(beginG1Thread()));
	connect(ui.pbG2, SIGNAL( clicked() ), this, SLOT(beginG2Thread()));        
	connect(ui.pbG3, SIGNAL( clicked() ), this, SLOT(beginG3Thread()));       
	connect(ui.pbG4, SIGNAL( clicked() ), this, SLOT(beginG4Thread()));
	connect(ui.pbG5, SIGNAL( clicked() ), this, SLOT(beginG5Thread()));        
	      
	connect(ui.pbLFR, SIGNAL( clicked() ), this, SLOT(beginLFRThread()));
	connect(ui.pbLFS, SIGNAL( clicked() ), this, SLOT(beginLFSThread()));
	connect(ui.pbLSR, SIGNAL( clicked() ), this, SLOT(beginLSRThread()));
	connect(ui.pbLSS, SIGNAL( clicked() ), this, SLOT(beginLSSThread()));*/
	
}

EServer::~EServer()
{

}


void EServer::fclpGeneralUsr()
{

}

void EServer::fclpSuperUsr()
{

}

void EServer::initData()
{
	int headRuleId=0;
	int headACLID=0;
	idR->initPool(headRuleId,headACLID);
}

void EServer::beginServer()
{
	ui.pbBegin->setDisabled(true);
	ui.pbStop->setDisabled(false);
	initData();
	fclpGeneralUsr();
	
	boost::thread t4(sendLiFilterTrd,this);

	boost::thread t5(sendLiSaveTrd,this);

	boost::thread t1(recvClientTrd,this);

	boost::thread t2(recvLiFilterTrd,this);

	boost::thread t3(recvLiSaveTrd,this);

	boost::thread t6(recvGaoTrd_200,this);

	boost::thread t7(recvGaoTrd_201,this);

	boost::thread t8(recvGaoTrd_202,this);
	
	boost::thread t9(recvGaoTrd_203,this);
	
	boost::thread t10(recvGaoTrd_204,this);
	
	boost::thread t11(recvGaoTrd_205,this);

	boost::thread t12(comTrd,this);

	boost::posix_time::time_duration td(0,0,3,0);
	boost::this_thread::sleep(td);
	
	std::string message="服务已启动";
	writeLog(message,this);

}

void EServer::stopServer()
{
		ui.pbBegin->setDisabled(false);
	ui.pbStop->setDisabled(true);
}

void EServer::clearLog()
{
	ui.listLog->clear();
}


//void EServer::queryStatus(){
//
//
//}
//void EServer::beginClientThread(){}
//void EServer::beginTimerThread(){}	
//void EServer::beginG0Thread{}       
//void EServer::beginG1Thread{}
//void EServer::beginG2Thread{}        
//void EServer::beginG3Thread{}       
//void EServer::beginG4Thread{}
//void EServer::beginG5Thread{}        
//void EServer::beginLFRThread()
//void EServer::beginLFSThread()
//void EServer::beginLSRThread()
//void EServer::beginLSSThread()
