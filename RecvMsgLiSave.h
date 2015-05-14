#ifndef RECVMSGLISAVE_H
#define RECVMSGLISAVE_H

#include  "CRecvMsg.h"

class CRecvMsgLiSave :
	public CRecvMsg
{
	Q_OBJECT
public:
	CRecvMsgLiSave(io_service& io,short port1,QObject *parent=0);
	~CRecvMsgLiSave(void);

	int recvLen;
	
	LiSaveS* liSaveS;

	void accept_handler(const boost::system::error_code& ec,sock_pt sock);
	virtual void read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock);
	int uploadRuleLiSave(LiSaveS* liSaveS);
};

#endif // RECVMSGLISAVE_H
