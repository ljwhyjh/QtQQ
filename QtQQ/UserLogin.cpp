#include "UserLogin.h"
#include "CCMainWindow.h"

#include <QMessageBox>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <qDebug>

QString gLoginEmployeeID;//��¼��QQ��(Ա����)

UserLogin::UserLogin(QWidget *parent)
	: BasicWindow(parent)
{
	ui.setupUi(this);

	// �����캯������һ��
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
	headLabel->setPixmap(getRoundImage(QPixmap(":/Resources/MainWindow/girl.png"),pix,headLabel->size()));
	headLabel->move(width() / 2 - 34, ui.titleWidget->height() - 34);

	// ��Ϊ�ǿյ�, ���Բ���ֱ������,Ҫ�ȵ��ú��� ת��
	connect(ui.loginBtn, &QPushButton::clicked, this, &UserLogin::onLoginBtnClicked);

	if (!connectMySql())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("�������ݿ�ʧ�ܣ�"));
		close();
	}
}

bool UserLogin::connectMySql()
{

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");      //�������ݿ���������������Ҫע�⣨�����Ϊ��127.0.0.1�������ֲ������ӣ����Ϊlocalhost)
	db.setPort(3306);                 //�������ݿ�˿ںţ�������һ��
	db.setDatabaseName("qtqq");      //�������ݿ�����������һ��
	db.setUserName("root");          //���ݿ��û�����������һ��
	db.setPassword("root");    //���ݿ����룬������һ��
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

bool UserLogin::veryfyAccountCode()
{
	QString strAccountInput = ui.editUserAccount->text();
	QString strCodeInput = ui.editPassword->text();

	//����Ա���ţ�QQ�ŵ�½��
	QString strSqlCode = QString("SELECT code FROM tab_accounts WHERE employeeID = %1").arg(strAccountInput);
	QSqlQuery queryEmpoyeeID(strSqlCode);
	queryEmpoyeeID.exec();

	if (queryEmpoyeeID.first())//ָ�������ĵ�һ��
	{
		//���ݿ���qq�Ŷ�Ӧ������
		QString strCode=queryEmpoyeeID.value(0).toString();

		if (strCode == strCodeInput)
		{
			gLoginEmployeeID = strCodeInput;
			return true;
		}
		else
		{
			return false;
		}
	}

	//�˺ŵ�¼
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
	if (!veryfyAccountCode())
	{
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("��������˺Ż������������������룡"));
		//ui.editUserAccount->setText("");
		//ui.editPassword->setText("");
		return;
	}

	close();
	CCMainWindow* mainWindow = new CCMainWindow;
	mainWindow->show();
}
