#include "GloabeParams.h"
#include <QObject>

CWFCLPAPI* wfclpapi=new CWFCLPAPI();
//CWFCLPAPI* wfaZtg=new CWFCLPAPI();
CIdResource *idR=new CIdResource();
 int g_minACLID=253001;
 int g_maxACLID=300000;
 int g_minXACLID=7001;
 int g_maxXACLID=16000;
 int g_minPLID=7001;
 int g_maxPLID=16000;
boost::mutex g_mtx_update;
boost::mutex g_mtx_log;

CSendMsg* sendLiFilter;
CRecvMsgLiFilter* recvLiFilter;
CSendMsg* sendLiSave;
CRecvMsgLiSave* recvLiSave;
CSendMsg1* sendGao_200;
CRecvMsg1* recvGao_200;
CSendMsg1* sendGao_201;
CRecvMsg1* recvGao_201;
CSendMsg1* sendGao_202;
CRecvMsg1* recvGao_202;
CSendMsg1* sendGao_203;
CRecvMsg1* recvGao_203;
CSendMsg1* sendGao_204;
CRecvMsg1* recvGao_204;
CSendMsg1* sendGao_205;
CRecvMsg1* recvGao_205;
