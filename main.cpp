#include "eserver.h"
#include <QtGui/QApplication>
 #include <QTextCodec>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QString>

void writeLog(std::string& message,EServer* es);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
	//QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	
	//const QString g_ip= "11.39.109.249";
	//const QString g_dbName= "rulesdb";
	//const QString g_usr="ruleadmin";
	//const QString g_passwd="123456";

	const QString g_ip= "10.10.88.209";
	const QString g_dbName= "orcl";
	const QString g_usr="eightyg";
	const QString g_passwd="123456";
	
	QSqlDatabase db=QSqlDatabase::addDatabase("QOCI");
	db.setHostName(g_ip);
	db.setDatabaseName(g_dbName);
	db.setUserName(g_usr);
	db.setPassword(g_passwd);

	EServer w;

	if (!db.open())
	{
		QString str=db.lastError().text();
		std::string message="数据库连接异常";
		writeLog(message,&w);
	}
	else
	{
		std::string message="数据库连接";
		writeLog(message,&w);
	}

	w.show();
	return a.exec();
}
