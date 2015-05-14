#ifndef CSENDMSG_H
#define CSENDMSG_H

#include <QObject>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>
#include "eserver.h"

using namespace boost::asio;

class CSendMsg : public QObject
{
	Q_OBJECT

public:
	CSendMsg(io_service& io,short port1,QObject *parent=0);
	~CSendMsg();

private:

public:
	io_service& ios;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt sock;

	std::string ip;
	short port;
	ip::tcp::acceptor acceptor;
	boost::mutex mtx_buff;
	EServer* es;
	bool flagWrite;

	void connect();
	void startListen();
	bool write(char* pSendBuff,int len);
		void read(std::string& str);
	
};

#endif // CSENDMSG_H
