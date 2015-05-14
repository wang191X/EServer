#ifndef GLOABEPARAMS_H
#define GLOABEPARAMS_H

#include <QString>
#include <QTextCodec>
#include "WFCLPAPI.h"
#include "CIdResource.h"
//#include "CLiClientFilter.h"
//#include "CLiClientSave.h"
#include "CSendMsg.h"
#include "CRecvMsg.h"
#include "CSendMsg1.h"
#include "CRecvMsg1.h"
#include "RecvMsgLiFilter.h"
#include "RecvMsgLiSave.h"
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

using namespace boost::asio;

extern CWFCLPAPI *wfclpapi;
//extern CWFCLPAPI *wfaZtg;
extern CIdResource *idR;
extern int g_minACLID;
extern int g_maxACLID;
extern int g_minXACLID;
extern int g_maxXACLID;
extern int g_minPLID;
extern int g_maxPLID;
extern boost::mutex g_mtx_update;
extern boost::mutex g_mtx_log;
extern CSendMsg* sendLiFilter;
extern CRecvMsgLiFilter* recvLiFilter;
extern CSendMsg* sendLiSave;
extern CRecvMsgLiSave* recvLiSave;
extern CSendMsg1* sendGao_200;
extern CRecvMsg1* recvGao_200;
extern CSendMsg1* sendGao_201;
extern CRecvMsg1* recvGao_201;
extern CSendMsg1* sendGao_202;
extern CRecvMsg1* recvGao_202;
extern CSendMsg1* sendGao_203;
extern CRecvMsg1* recvGao_203;
extern CSendMsg1* sendGao_204;
extern CRecvMsg1* recvGao_204;
extern CSendMsg1* sendGao_205;
extern CRecvMsg1* recvGao_205;

#define P_CLIENT_RECV          6688
#define P_LI_FILTER_SEND        6689
#define P_LI_FILTER_RECV        6690
#define P_LI_SAVE_SEND          6691
#define P_LI_SAVE_RECV          6692

#define P_GAO_RECV_200				6693
#define P_GAO_SEND					6694

#define P_GAO_RECV_201				6695

#define P_GAO_RECV_202				6696

#define P_GAO_RECV_203				6697

#define P_GAO_RECV_204				6698

#define P_GAO_RECV_205				6699

#define P_GAO_IP_200				"10.10.88.200"
#define P_GAO_IP_201				"10.10.88.201"
#define P_GAO_IP_202				"10.10.88.202"
#define P_GAO_IP_203				"10.10.88.203"
#define P_GAO_IP_204				"10.10.88.204"
#define P_GAO_IP_205				"10.10.88.205"

#endif //

