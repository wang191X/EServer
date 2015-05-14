#ifndef ESERVER_H
#define ESERVER_H

#include <QtGui/QDialog>
#include "ui_eserver.h"
#include "WFCLPAPI.h"

class EServer : public QDialog
{
	Q_OBJECT

public:
	EServer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~EServer();

public:
	Ui::EServerClass ui;

private slots:

	void fclpSuperUsr();
	void fclpGeneralUsr();
	void beginServer();
	void stopServer();
	void clearLog();
	void initData();
};

#endif // ESERVER_H
