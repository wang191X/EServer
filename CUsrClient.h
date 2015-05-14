#ifndef CUSRCLIENT_H
#define CUSRCLIENT_H

#include <QObject>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>
#include "WFCLPAPI.h"
#include "eserver.h"

using namespace boost::asio;

class CUsrClient : public QObject
{
	Q_OBJECT

public:
	CUsrClient(io_service& io,QObject *parent=0);
	~CUsrClient();

	io_service &ios;
	short port;
	ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	
	int max_length;
	char* pRecvBuff;
	int logCount;
	std::string ip;
	ClientRule* rules;
	ClientRule* rule;
	int crLen;
	int pktNum;

	void start();
	void start1();
	void initDevCon();
	void initData();
void initACLObj();
void initXACLObj();
void initPLObj();
bool uploadacl();
bool uploadxacl();
bool uploadpl();
bool uploadDeny();
bool downloadacl();
bool downloadDeny();
bool downloadxacl();
bool downloadpl();
bool tellLiFilter(int cmdType);
bool tellLiSave(int cmdType);
bool tellGao();
bool tellGao_200();
bool tellGao_201();
bool tellGao_202();
bool tellGao_203();
bool tellGao_204();
bool tellGao_205();

bool recvGao_200();
bool recvGao_201();
bool recvGao_202();
bool recvGao_203();
bool recvGao_204();
bool recvGao_205();

void ruleBatchProc();
bool updateCMD(int ruleId,int cmdType);
	void accept_handler(const boost::system::error_code& ec,sock_pt sock);
	void read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock);
	void write_handler(const boost::system::error_code& error,sock_pt sock);

public:
	EServer* es;
};

#endif // CUSRCLIENT_H
