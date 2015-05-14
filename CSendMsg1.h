#ifndef CSendMsg1_H
#define CSendMsg1_H

#include <QObject>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>
#include "eserver.h"
#include "json/json.h"

using namespace boost::asio;

class CSendMsg1 : public QObject
{
	Q_OBJECT

public:
	CSendMsg1(io_service& io,std::string ip1,short port1,QObject *parent=0);
	~CSendMsg1();

private:

public:
	io_service& ios;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt sock;

	std::string ip;
	short port;
	boost::mutex mtx_buff;
	EServer* es;

	void connect();
	bool write(std::string& str);
	bool read();
	void disConn();
};

#endif // CSendMsg1_H
