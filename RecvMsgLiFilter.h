#ifndef RECVMSGLIFILTER_H
#define RECVMSGLIFILTER_H

#include "CRecvMsg.h"

class CRecvMsgLiFilter :
	public CRecvMsg
{
	Q_OBJECT

public:
	CRecvMsgLiFilter(io_service& io,short port1,QObject *parent=0);
	~CRecvMsgLiFilter(void);

	int recvLen;

	void accept_handler(const boost::system::error_code& ec,sock_pt sock);
	void read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock);
	int uploadRuleLiFilter(LiFilterS liFilterS);
	void procBuff(int recv_len);
	void proData(LiFilterS liFilterS);
	std::string intToIp(int value);
};

#endif // RECVMSGLIFILTER_H
