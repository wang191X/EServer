#ifndef CRECVMSG1_H
#define CRECVMSG1_H

#include "CRecvMsg.h"
#include "json/json.h"

class CRecvMsg1 : public CRecvMsg
{
	Q_OBJECT

public:
	CRecvMsg1(io_service& io,short port1,QObject *parent=0);
	~CRecvMsg1();

	void read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock);
	int uploadRuleG(Json::Value& value);
	void jsonProc(char* pJsonBuff);
	void ProcBuff(int recv_len);
private:
	
};

#endif // CRECVMSG1_H
