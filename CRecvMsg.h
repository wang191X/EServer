#ifndef CRECVMSG_H
#define CRECVMSG_H

#include <QObject>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>
#include "eserver.h"
#include "CLiDataStruct.h"

using namespace boost::asio;

class CRecvMsg : public QObject
{
	Q_OBJECT

public:
	CRecvMsg(io_service& io,short port1,QObject *parent=0);
	virtual ~CRecvMsg();

private:
	

public:
	std::string ip;
	short port;
	io_service &ios;
	ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt sock;
	char* pRecvBuff;
	char* pProcBuff;
	char* pProcTmp;
	int procLen;
	int tmpLen;
	int max_length;
	char* pSendBuff;
	int sendLen;
	boost::mutex mtx_buff;
	//LiFilterS* liFilterS;
	//LiSaveS* liSaveS;
	
public:
	EServer* es;

	void start();
	virtual void accept_handler(const boost::system::error_code& ec,sock_pt sock);
	virtual void read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock);
	void write_handler(const boost::system::error_code& error,sock_pt sock);
	//void startListen();
	int write(char* pSendBuff,int len);
	void fillBuff(char* pSendBuff1,int len);
	//void startSyn();
	//int uploadRule(LiRevS* liRevS);
	int assignACLID();
};

#endif // CRECVMSG_H
