#ifndef CLICLIENTFILTER_H
#define CLICLIENTFILTER_H

#include <QObject>
#include <iostream>
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

class CLiClientFilter : public QObject
{
	Q_OBJECT

public:
	CLiClientFilter(io_service& io,QObject *parent=0);
	~CLiClientFilter();

public:
	io_service &ios;
	ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt sock;
	char* pRecvBuff;
	int max_length;
	
public:
	EServer* es;

	void start();
	void start1();
	void startSyn();
	void initData();
	void accept_handler(const boost::system::error_code& ec,sock_pt sock);
	void read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock);
	int uploadRule(LiRevS* liRevS);
	void write_handler(const boost::system::error_code& error,sock_pt sock);
	void write(std::string& str);
};

#endif // CLICLIENTFILTER_H
