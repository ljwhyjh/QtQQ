#include "UserLogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qDebug>

QString gLoginEmployeeID;//登录者QQ号(员工号)

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);

	// 将构造函数完善一下
	setAttribute(Qt::WA_DeleteOnClose);
	initTitleBar();
	setTitleBarTitle("", ":/Resources/MainWindow/qqlogoclassic.png");

	loadStyleSheet("UserLogin");
	initControl();
}

UserLogin::~UserLogin()
{
}

void UserLogin::initControl()
{
	QLabel* headLabel = new QLabel(this);
	headLabel->setFixedSize(68, 68);

	QPixmap pix(":/Resources/MainWindow/head_mask.png");
	headLabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/app/logo.ico"),pix,headLabel->size()));
	headLabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);

	// 因为是空的, 所以不能直接设置,要先调用函数 转换
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);

	if (!connectMySql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("连接数据库失败！"));
		close();
	}
}

bool UserLogin::connectMySql()
{

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");      //连接数据库主机名，这里需要注意（若填的为”127.0.0.1“，出现不能连接，则改为localhost)
	db.setPort(3306);                 //连接数据库端口号，与设置一致
	db.setDatabaseName("qtqq");      //连接数据库名，与设置一致
	db.setUserName("root");          //数据库用户名，与设置一致
	db.setPassword("root");    //数据库密码，与设置一致
	db.open();

	qDebug() << QSqlDatabase::drivers() << endl;
	if (!db.open())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool UserLogin::veryfyAccountCode(bool &isAccountLogin, QString &strAccount)
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();

	//输入员工号（QQ号登陆）
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);
	QSqlQuery queryEmpoyeeID(strSqlCode);
	queryEmpoyeeID.exec();

	if (queryEmpoyeeID.first())//指向结果集的第一条
	{
		//数据库中qq号对应的密码
		QString strCode=queryEmpoyeeID.value(0).toString();

		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = strAccountInput;

			isAccountLogin = false;
			strAccount = strAccountInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	//账号登录
	strSqlCode = QString("SELECT code,employeeID FROM tab_accounts WHERE account ='%1'")
		           .arg(strAccountInput);

	QSqlQuery queryAccount(strSqlCode);
	queryAccount.exec();
	if (queryAccount.first())
	{
		QString strCode = queryAccount.value(0).toString();

		if (strCode == strCodeInput)
		{
			gLoginEmployeeID= queryAccount.value(1).toString();

			isAccountLogin = true;
			strAccount = strAccountInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void UserLogin::onLoginBtnClicked()
{
	bool isAccountLogin;
	QString strAccount; //账号或QQ号

	if (!veryfyAccountCode(isAccountLogin,strAccount))
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("您输入的账号或密码有误，请重新输入！"));
		//ui.editUserAccount->setText("");
		//ui.editPassword->setText("");
		return;
	}

	//更新登录状态为在线
	QString strSqlStatus = QString("UPDATE tab_employees SET online=2 WHERE employeeID= %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	close();
	CCMainWindow* mainWindow = new CCMainWindow(strAccount,isAccountLogin);
	mainWindow->show();
}
