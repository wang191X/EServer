#ifndef CLICLIENTSAVE_H
#define CLICLIENTSAVE_H

#include <QObject>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>

using namespace boost::asio;

class CLiClientSave : public QObject
{
	Q_OBJECT

public:
	CLiClientSave(io_service& io,QObject *parent=0);
	~CLiClientSave();

public:
	io_service& ios;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt sock;

	void connect();
	void write(std::string& str);
		void read(std::string& str);
};

#endif // CLICLIENTSAVE_H
