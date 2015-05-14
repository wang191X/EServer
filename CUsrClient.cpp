#include "CUsrClient.h"
#include "WFCLPAPI.h"
#include "GloabeParams.h"
#include "CLiDataStruct.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>

void writeLog(std::string& message,EServer* es);
std::string numToStr(int id);

CUsrClient::CUsrClient(io_service& io,QObject *parent)
: QObject(parent)
,ios(io)
,port(P_CLIENT_RECV)
,acceptor(ios,ip::tcp::endpoint(ip::tcp::v4(),port))
{
	ip="";
}

CUsrClient::~CUsrClient()
{
	delete [] rules;
	delete [] rule;
	delete [] pRecvBuff;
}

void CUsrClient::start()
{
	initDevCon();
	initData();
	//serverCard1.start();
	//serverCard2.start();
	start1();
}

void CUsrClient::start1()
{
	sock_pt sock(new ip::tcp::socket(ios));
	acceptor.async_accept(*sock,bind(&CUsrClient::accept_handler,this,placeholders::error,sock));
}

void CUsrClient::initDevCon()
{
//{
//	int ret=wfclpapi->connect(sSession);
//
//	if(ret==-1)
//	{
//		return;
//	}
//}
//
//	std::string message="设备已连接";
//	writeLog(message,es);
}

void CUsrClient::initData()
{
	max_length=sizeof(ClientRule);
	crLen=max_length;
	rules=new ClientRule[200];
	rule=new ClientRule();
	pktNum=0;
	pRecvBuff=new char[max_length];
}


void CUsrClient::accept_handler(const boost::system::error_code& ec,sock_pt sock)
{
	if(ec)
	{
		//std::string message1="未知";
		//writeLog(message1,es);

		ip=sock->remote_endpoint().address().to_string();
		QString content=QString(tr("连接接收错误"));
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return;
	}

	start1();

	ip=sock->remote_endpoint().address().to_string();
	std::string message=ip+" "+numToStr(port)+"客户端已连接";
	writeLog(message,es);

	async_read(*sock,boost::asio::buffer(pRecvBuff,max_length),
		bind(&CUsrClient::read_handler, this,
		placeholders::error,
		placeholders::bytes_transferred,sock));

	/*sock->async_read_some(boost::asio::buffer(pRecvBuff,max_length),
		bind(&CUsrClient::read_handler, this,
		placeholders::error,
		placeholders::bytes_transferred,sock));*/
}

void CUsrClient::initACLObj()
{
	
}

void CUsrClient::initXACLObj()
{                
	
}
void CUsrClient::initPLObj()
{
	
}
bool CUsrClient::uploadacl()
{
	initACLObj();
	
	int ret=wfclpapi->uploadACL();

	if(ret==FCLPAPI_RULE_EXIST)
	{
		rule->cmdType=RULE_LOAD_SUCCESS;
		//std::string message1="失败,命令类型：下发acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("重复,命令类型：下发acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return true;
	}

	if(ret!=FCLPAPI_OK)
	{
		rule->cmdType=RULE_LOAD_FAILED;
		//std::string message1="失败,命令类型：下发acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：下发acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		
		return false;
	}

	if(ret==FCLPAPI_OK)
	{
		rule->cmdType=RULE_LOAD_SUCCESS;
		//std::string message1="成功,命令类型：下发acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：下发acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return true;
	}

	return true;
}

bool CUsrClient::uploadDeny()
{
	initACLObj();
	
	int ret=wfclpapi->uploadDeny();

	if(ret==-1)
	{
		rule->cmdType=RULE_LOAD_FAILED;
		//std::string message1="失败,命令类型：下发acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：下发acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		
		return false;
	}
	else
	{
		rule->cmdType=RULE_LOAD_SUCCESS;
		//std::string message1="成功,命令类型：下发acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：下发acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return true;
	}
}

bool CUsrClient::uploadxacl()
{
	initXACLObj();
	
	int ret=wfclpapi->uploadXACL();

	if(ret==-1)
	{
		rule->cmdType=RULE_LOAD_FAILED;
		//std::string message1="失败,命令类型：下发xacl，id："+numToStr(rule->xaclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：下发xacl，id：%1")).arg(rule->xaclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return false;
	}
	else
	{
		rule->cmdType=RULE_LOAD_SUCCESS;
		//std::string message1="成功,命令类型：下发xacl，id："+numToStr(rule->xaclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：下发xacl，id：%1")).arg(rule->xaclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return true;
	}
}
bool CUsrClient::uploadpl()
{
	initPLObj();
	
	int ret=wfclpapi->uploadPL();

	if(ret==-1)
	{
		rule->cmdType=RULE_LOAD_FAILED;
		//std::string message1="失败,命令类型：下发pl，id："+numToStr(rule->plid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：下发pl，id：%1")).arg(rule->plid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return false;
	}
	else
	{
		rule->cmdType=RULE_LOAD_SUCCESS;
		//std::string message1="成功,命令类型：下发pl，id："+numToStr(rule->plid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：下发pl，id：%1")).arg(rule->plid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return true;
	}
}
bool CUsrClient::downloadacl()
{
	
	int ret=wfclpapi->removeACL(rule->aclid);

	if(ret==-1)
	{
		rule->cmdType=RULE_UNLOAD_FAILED;
		//std::string message1="失败,命令类型：删除acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：删除acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return false;
	}
	else
	{
		rule->cmdType=RULE_UNLOAD_SUCCESS;
		//std::string message1="成功,命令类型：删除acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：删除acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return true;
	}	
}

bool CUsrClient::downloadDeny()
{
	
	int ret=wfclpapi->removeDeny(rule->aclid);

	if(ret==-1)
	{
		rule->cmdType=RULE_UNLOAD_FAILED;
		//std::string message1="失败,命令类型：删除acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：删除acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return false;
	}
	else
	{
		rule->cmdType=RULE_UNLOAD_SUCCESS;
		//std::string message1="成功,命令类型：删除acl，id："+numToStr(rule->aclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：删除acl，id：%1")).arg(rule->aclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		
		return true;
	}	
}

bool CUsrClient::downloadxacl()
{
	int ret=wfclpapi->removeXACL(rule->xaclid);

	if(ret==-1)
	{
		rule->cmdType=RULE_UNLOAD_FAILED;
		//std::string message1="失败,命令类型：删除xacl，id："+numToStr(rule->xaclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：删除xacl，id：%1")).arg(rule->xaclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return false;
	}
	else
	{
		rule->cmdType=RULE_UNLOAD_SUCCESS;
		//std::string message1="成功,命令类型：删除xacl，id："+numToStr(rule->xaclid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：删除xacl，id：%1")).arg(rule->xaclid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return true;
	}	
}
bool CUsrClient::downloadpl()
{
	int ret=wfclpapi->removePL(rule->plid);

	if(ret==-1)
	{
		rule->cmdType=RULE_UNLOAD_FAILED;
		//std::string message1="失败,命令类型：删除pl，id："+numToStr(rule->plid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("失败,命令类型：删除pl，id：%1")).arg(rule->plid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return false;
	}
	else
	{
		rule->cmdType=RULE_UNLOAD_SUCCESS;
		//std::string message1="成功,命令类型：删除pl，id："+numToStr(rule->plid)+","+ip;
		//writeLog(message1,es);

		QString content=QString(tr("成功,命令类型：删除pl，id：%1")).arg(rule->plid);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return true;
	}	
}

bool CUsrClient::tellLiFilter(int cmdType)
{
	LiSendS liSendS;
	liSendS.head=0xaabbccdd;
	
	liSendS.cmdType=cmdType;
	liSendS.ruleId=rule->ruleId;
	
	//发li_filter
	int lssLen=sizeof(LiSendS);
	char* pSendBuff=new char[lssLen];
	memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
	bool ret=sendLiFilter->write(pSendBuff,lssLen);

	if(ret)
	{
		QString content=QString(tr("ruleid:%1已发送至lifilter")).arg(rule->ruleId);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);
		return true;
	}
	
	delete pSendBuff;
	return false;
}

bool CUsrClient::tellLiSave(int cmdType)
{
	LiSendS liSendS;
	liSendS.head=0xaabbccdd;
	
	liSendS.cmdType=cmdType;
	liSendS.ruleId=rule->ruleId;
	
	//发li_filter
	int lssLen=sizeof(LiSendS);
	char* pSendBuff=new char[lssLen];
	memcpy_s(pSendBuff, lssLen, (char*)&liSendS, lssLen );
	bool ret=sendLiSave->write(pSendBuff,lssLen);

	if(ret)
	{
		QString content=QString(tr("ruleid:%1已发送至liSave")).arg(rule->ruleId);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return true;
	}
	delete pSendBuff;

	return false;
}

bool CUsrClient::tellGao()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	
	io_service ios_200;
	sendGao_200=new CSendMsg1(ios_200,P_GAO_IP_200,P_GAO_SEND);
	sendGao_200->connect();
	bool ret_w_200=sendGao_200->write(out);
	
	io_service ios_201;
	sendGao_201=new CSendMsg1(ios_201,P_GAO_IP_201,P_GAO_SEND);
	sendGao_201->connect();
	bool ret_w_201=sendGao_201->write(out);

	io_service ios_202;
	sendGao_202=new CSendMsg1(ios_202,P_GAO_IP_202,P_GAO_SEND);
	sendGao_202->connect();
	bool ret_w_202=sendGao_202->write(out);
	
	io_service ios_203;
	sendGao_203=new CSendMsg1(ios_203,P_GAO_IP_203,P_GAO_SEND);
	sendGao_203->connect();
	bool ret_w_203=sendGao_203->write(out);
	
	io_service ios_204;
	sendGao_204=new CSendMsg1(ios_204,P_GAO_IP_204,P_GAO_SEND);
	sendGao_204->connect();
	bool ret_w_204=sendGao_204->write(out);
	
	io_service ios_205;
	sendGao_205=new CSendMsg1(ios_205,P_GAO_IP_205,P_GAO_SEND);
	sendGao_205->connect();
	bool ret_w_205=sendGao_205->write(out);
	
	//bool ret0=sendGao_200->read();
	sendGao_200->disConn();
	
	//bool ret1=sendGao_201->read();
	sendGao_201->disConn();
	
	//bool ret2=sendGao_202->read();
	sendGao_202->disConn();
	
	//bool ret3=sendGao_203->read();
	sendGao_203->disConn();
	
	//bool ret4=sendGao_204->read();
	sendGao_204->disConn();
	
	//bool ret5=sendGao_205->read();
	sendGao_205->disConn();
	
	if(ret_w_200&&ret_w_201&&ret_w_202&&ret_w_203&&ret_w_204&&ret_w_205)
		return true;
		
	return false;
}

bool CUsrClient::tellGao_200()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	io_service ios;
	sendGao_200=new CSendMsg1(ios,P_GAO_IP_200,P_GAO_SEND);
	sendGao_200->connect();
	sendGao_200->write(out);
	
	// bool ret=sendGao_200->read();
	// sendGao_200->disConn();

	return true;
}

bool CUsrClient::recvGao_200()
{
	bool ret=sendGao_200->read();
	sendGao_200->disConn();

	return ret;
}

bool CUsrClient::tellGao_201()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	io_service ios;
	sendGao_201=new CSendMsg1(ios,P_GAO_IP_201,P_GAO_SEND);
	sendGao_201->connect();
	sendGao_201->write(out);
	// bool ret=sendGao_201->read();
	// sendGao_201->disConn();

	return true;
}

bool CUsrClient::recvGao_201()
{
	bool ret=sendGao_201->read();
	sendGao_201->disConn();

	return ret;
}

bool CUsrClient::tellGao_202()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	io_service ios;
	sendGao_202=new CSendMsg1(ios,P_GAO_IP_202,P_GAO_SEND);
	sendGao_202->connect();
	sendGao_202->write(out);
	// bool ret=sendGao_202->read();
	// sendGao_202->disConn();

	return true;
}

bool CUsrClient::recvGao_202()
{
	bool ret=sendGao_202->read();
	sendGao_202->disConn();

	return ret;
}

bool CUsrClient::tellGao_203()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	io_service ios;
	sendGao_203=new CSendMsg1(ios,P_GAO_IP_203,P_GAO_SEND);
	sendGao_203->connect();
	sendGao_203->write(out);
	// bool ret=sendGao_203->read();
	// sendGao_203->disConn();

	return true;
}

bool CUsrClient::recvGao_203()
{
	bool ret=sendGao_203->read();
	sendGao_203->disConn();

	return ret;
}

bool CUsrClient::tellGao_204()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	io_service ios;
	sendGao_204=new CSendMsg1(ios,P_GAO_IP_204,P_GAO_SEND);
	sendGao_204->connect();
	sendGao_204->write(out);
	// bool ret=sendGao_204->read();
	// sendGao_204->disConn();

	return true;
}

bool CUsrClient::recvGao_204()
{
	bool ret=sendGao_204->read();
	sendGao_204->disConn();

	return ret;
}

bool CUsrClient::tellGao_205()
{
	Json::Value root;
	root["Method"] = "ReloadNic";
	//root["CmdType"]=cmdType;
	//root["RuleId"]=rule->ruleId;
	std::string out = root.toStyledString();
	io_service ios;
	sendGao_205=new CSendMsg1(ios,P_GAO_IP_205,P_GAO_SEND);
	sendGao_205->connect();
	sendGao_205->write(out);
	// bool ret=sendGao_205->read();
	// sendGao_205->disConn();

	return true;
}

bool CUsrClient::recvGao_205()
{
	bool ret=sendGao_205->read();
	sendGao_205->disConn();

	return ret;
}

bool CUsrClient::updateCMD(int ruleId,int cmdType)
{
	QSqlQuery q;
	QString querySql;
	querySql=QString("update rule set cmdtype=%1 where ruleid=%2").arg(cmdType).arg(ruleId);
	return q.exec(querySql);
}

void CUsrClient::ruleBatchProc()
{
	std::vector<int> vecGao;
	std::vector<int> vecLiZ;
	std::vector<int> vecLiF;

	for(int i=0;i!=pktNum;++i)
	{
		*rule=rules[i];

		if(rules[i].RULETYPE==1&&rules[i].lianjie==1)
		{
			vecGao.push_back(i);

			if(rules[i].cmdType==RULE_LOAD)
			{
				updateCMD(rules[i].ruleId,RULE_LOAD_SUCCESS);
				rules[i].cmdType=RULE_LOAD_SUCCESS;
			}
			else if(rules[i].cmdType==RULE_UNLOAD)
			{
				updateCMD(rules[i].ruleId,RULE_UNLOAD_SUCCESS);
				rules[i].cmdType=RULE_UNLOAD_SUCCESS;
			}
			
			continue;
		}

		if(rules[i].RULETYPE==2)
		{
			if(rules[i].lianjie==0&&rules[i].FIVEITEM==1)
			{
				//发fclp
				//发li_save
				if(rules[i].cmdType==RULE_LOAD)
				{
					if(!uploadDeny())
					{
						updateCMD(rules[i].ruleId,RULE_LOAD_FAILED);
						rules[i].cmdType=RULE_LOAD_FAILED;

						QString content=QString(tr("aclid:%1,deny规则上载失败")).arg(rules[i].aclid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					/*if(!tellLiSave(1))
					{
					rules[i].cmdType=RULE_LOAD_FAILED;
					continue;
					}*/

					vecLiF.push_back(i);
					updateCMD(rules[i].ruleId,RULE_LOAD_SUCCESS);
					rules[i].cmdType=RULE_LOAD_SUCCESS;
					continue;

				}else if(rules[i].cmdType==RULE_UNLOAD)
				{
					if(!downloadDeny())
					{
						//rules[i].cmdType=RULE_UNLOAD_FAILED;
						continue;
					}

					/*if(!tellLiSave(2))
					{
					rules[i].cmdType=RULE_UNLOAD_FAILED;
					continue;
					}*/

					vecLiF.push_back(i);
					updateCMD(rules[i].ruleId,RULE_UNLOAD_SUCCESS);
					rules[i].cmdType=RULE_UNLOAD_SUCCESS;

					continue;
				}
			}

			//发li_save
			if(rules[i].cmdType==RULE_LOAD)
			{
				/*if(!tellLiSave(1))
				{
				rules[i].cmdType=RULE_LOAD_FAILED;
				continue;
				}*/

				vecLiF.push_back(i);
				updateCMD(rules[i].ruleId,RULE_LOAD_SUCCESS);
				rules[i].cmdType=RULE_LOAD_SUCCESS;

				continue;
			}
			else if(rules[i].cmdType==RULE_UNLOAD)
			{
				/*if(!tellLiSave(2))
				{
				rules[i].cmdType=RULE_UNLOAD_FAILED;
				continue;
				}*/

				vecLiF.push_back(i);
				updateCMD(rules[i].ruleId,RULE_UNLOAD_SUCCESS);
				rules[i].cmdType=RULE_UNLOAD_SUCCESS;
				continue;
			}
		}

		if(rules[i].RULETYPE==1&&rules[i].lianjie==0)
		{
			if(rules[i].solidExist==1)
			{
				//发fclp_pl
				//发fclp_xacl
				//发li_filter
				if(rules[i].cmdType==RULE_LOAD)
				{
					if(!uploadpl())
					{
						updateCMD(rules[i].ruleId,RULE_LOAD_FAILED);
						rules[i].cmdType=RULE_LOAD_FAILED;

						QString content=QString(tr("plid:%1,pl规则上载成功")).arg(rules[i].plid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					if(!uploadxacl())
					{
						updateCMD(rules[i].ruleId,RULE_LOAD_FAILED);
						rules[i].cmdType=RULE_LOAD_FAILED;

						QString content=QString(tr("xaclid:%1,xacl规则上载成功")).arg(rules[i].xaclid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					/*if(!tellLiSave(3))
					{
					rules[i].cmdType=RULE_LOAD_FAILED;
					continue;
					}

					if(!tellLiFilter(1))
					{
					rules[i].cmdType=RULE_LOAD_FAILED;
					continue;
					}*/

					vecLiZ.push_back(i);
					updateCMD(rules[i].ruleId,RULE_LOAD_SUCCESS);
					rules[i].cmdType=RULE_LOAD_SUCCESS;
					continue;
				}
				else if(rules[i].cmdType==RULE_UNLOAD)
				{
					if(!downloadxacl())
					{
						//rules[i].cmdType=RULE_UNLOAD_FAILED;

						QString content=QString(tr("xaclid:%1,xacl规则卸载失败")).arg(rules[i].xaclid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					if(!downloadpl())
					{
						//rules[i].cmdType=RULE_UNLOAD_FAILED;

						QString content=QString(tr("plid:%1,pl规则卸载失败")).arg(rules[i].plid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					/*if(!tellLiFilter(2))
					{
					rules[i].cmdType=RULE_UNLOAD_FAILED;
					continue;
					}*/

					QString content=QString(tr("ruleid:%1,规则卸载成功")).arg(rules[i].ruleId);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

					vecLiZ.push_back(i);
					updateCMD(rules[i].ruleId,RULE_UNLOAD_SUCCESS);
					rules[i].cmdType=RULE_UNLOAD_SUCCESS;

					continue;
				}
			}

			if(rules[i].solidExist==0&&rules[i].FIVEITEM==1)
			{
				//发fclp_acl
				if(rules[i].cmdType==RULE_LOAD)
				{
					if(!uploadacl())
					{
						updateCMD(rules[i].ruleId,RULE_LOAD_FAILED);
						rules[i].cmdType=RULE_LOAD_FAILED;

						QString content=QString(tr("aclid:%1,规则上载失败")).arg(rules[i].aclid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					/*if(!tellLiSave(3))
					{
					rules[i].cmdType=RULE_LOAD_FAILED;
					continue;
					}

					if(!tellLiFilter(1))
					{
					rules[i].cmdType=RULE_LOAD_FAILED;
					continue;
					}*/

					vecLiZ.push_back(i);
					updateCMD(rules[i].ruleId,RULE_LOAD_SUCCESS);
					rules[i].cmdType=RULE_LOAD_SUCCESS;
					continue;

				}
				else if(rules[i].cmdType==RULE_UNLOAD)
				{
					if(!downloadacl())
					{
						//rules[i].cmdType=RULE_UNLOAD_FAILED;

						QString content=QString(tr("aclid:%1,规则卸载失败")).arg(rules[i].aclid);
						QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
						idR->insertLog(relateDevice,content);

						continue;
					}

					/*if(!tellLiFilter(2))
					{
					rules[i].cmdType=RULE_UNLOAD_FAILED;
					continue;
					}*/

					QString content=QString(tr("aclid:%1,规则卸载成功")).arg(rules[i].aclid);
					QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
					idR->insertLog(relateDevice,content);

					vecLiZ.push_back(i);
					updateCMD(rules[i].ruleId,RULE_UNLOAD_SUCCESS);
					rules[i].cmdType=RULE_UNLOAD_SUCCESS;

					continue;
				}
			}
		}
	}

	if(vecLiF.size()!=0||vecLiZ.size()!=0||vecGao.size()!=0)
	{
		bool ret=tellLiSave(4);

		if(ret)
		{
			QString content=QString(tr("发送规则更新命令4至lisave成功"));
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);
		}
		else
		{
			QString content=QString(tr("发送规则更新命令4至lisave失败"));
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);
		}
	}

	if(vecLiF.size()!=0||vecLiZ.size()!=0)
	{
		bool ret=tellLiFilter(4);
		
		if(ret)
		{
			QString content=QString(tr("发送规则更新命令4至lifilter成功"));
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);
		}
		else
		{
			QString content=QString(tr("发送规则更新命令4至lifilter失败"));
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);
		}
	}

	if(vecGao.size()!=0)
	{
		int ret=tellGao();

		if(ret)
		{
			QString content=QString(tr("通知gao_ReloadNic成功"));
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);
		}
		else
		{
			QString content=QString(tr("通知gao_ReloadNic部分失败"));
			QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
			idR->insertLog(relateDevice,content);
		}
	}


	//for(int j=0;j!=vecGao.size();++j)
	//{
	//	int index=vecGao[j];

	//	if(rules[index].cmdType==RULE_LOAD)
	//	{
	//		updateCMD(rules[index].ruleId,GAO_NEW_RULE);
	//	}
	//	else if(rules[index].cmdType==RULE_UNLOAD)
	//	{
	//		updateCMD(rules[index].ruleId,GAO_DELETE_RULE);
	//	}
	//}

	//int retGao=0;

	//if(vecGao.size()!=0)
	//{
	//	retGao=tellGao();
	//}

	//for(int j=0;j!=vecGao.size();++j)
	//{
	//	int index=vecGao[j];

	//	if(retGao)
	//	{
	//		QString content=QString(tr("通知gao_ReloadNic成功"));
	//		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
	//		idR->insertLog(relateDevice,content);

	//		if(rules[index].cmdType==RULE_LOAD)
	//		{
	//			rules[index].cmdType=RULE_LOAD_SUCCESS;
	//		}
	//		else if(rules[index].cmdType==RULE_UNLOAD)
	//		{
	//			rules[index].cmdType=RULE_UNLOAD_SUCCESS;
	//		}

	//	}
	//	else
	//	{
	//		if(rules[index].cmdType==RULE_LOAD)
	//		{
	//			rules[index].cmdType=RULE_LOAD_FAILED;
	//		}
	//		else if(rules[index].cmdType==RULE_UNLOAD)
	//		{
	//			rules[index].cmdType=RULE_UNLOAD_FAILED;
	//		}

	//	}
	//}

	vecGao.clear();
	vecLiZ.clear();
	vecLiF.clear();
}

void CUsrClient::read_handler(const boost::system::error_code& ec,size_t bytes_transferred,sock_pt sock)
{
	if (ec)
	{
	
		//ip=sock->remote_endpoint().address().to_string();
		//QString content=QString(tr("读问题"));
		//QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		//idR->insertLog(relateDevice,content);

		//std::string message=ip+" "+numToStr(port)+"读问题";
		//writeLog(message,es);

		return;
	}
	
	ip=sock->remote_endpoint().address().to_string();
	int recv_len=bytes_transferred;
	memcpy_s(rule,crLen,pRecvBuff,crLen);

	if(rule->tailFlag!=1)
	{
		rules[pktNum]=*rule;
		pktNum++;

		async_read(*sock,boost::asio::buffer(pRecvBuff,max_length),
		bind(&CUsrClient::read_handler, this,
		placeholders::error,
		placeholders::bytes_transferred,sock));

		return;
	}

	//std::string message1="客户端发送规则条数:"+numToStr(pktNum);
	//writeLog(message1,es);
	QString content=QString(tr("客户端发送规则条数:%1")).arg(pktNum);
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

	ruleBatchProc();
	char* pSendBuff=new char[crLen];

	for(int i=0;i!=pktNum;++i)
	{
		memcpy_s(pSendBuff, crLen, (char*)(&(rules[i])), crLen );
		sock->async_write_some(buffer(pSendBuff,crLen),bind(&CUsrClient::write_handler,this,placeholders::error,sock));
	}
	
	delete [] pSendBuff;

	pktNum=0;

	async_read(*sock,boost::asio::buffer(pRecvBuff,max_length),
		bind(&CUsrClient::read_handler, this,
		placeholders::error,
		placeholders::bytes_transferred,sock));

	/*content=QString(tr("线程状态正常"));
	relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
	idR->insertLog(relateDevice,content);*/
}

void CUsrClient::write_handler(const boost::system::error_code& error,sock_pt sock)
{
	if (error)
	{
		//std::string message1="客户端写问题";
		//writeLog(message1,es);
		QString content=QString(tr("写问题"));
		QString relateDevice=QString("%1 %2").arg(ip.c_str()).arg(port);
		idR->insertLog(relateDevice,content);

		return;
	}
	//std::cout<<"send msg complete."<<std::endl;

	/*sock->async_read_some(boost::asio::buffer(pRecvBuff,max_length),
		bind(&CUsrClient::read_handler, this,
		placeholders::error,
		placeholders::bytes_transferred,sock));*/
	
}
