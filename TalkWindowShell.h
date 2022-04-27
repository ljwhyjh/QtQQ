#pragma once

#include "BasicWindow.h"
#include "ui_TalkWindowShell.h"

#include "EmotionWindow.h"
#include "TalkWindow.h"
#include "TalkWindowItem.h"
#include <qmap.h>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QString>

class TalkWindow;
class TalkWindowItem;
class QListWidgetItem;
class EmotionWindow;

const int gtcpPort = 8888;

enum GroupType{
	COMPANY = 0, //��˾Ⱥ
	PERSONELGROUP, //���²�
	DEVELOPMENTGROUP, //�з���
	MARKETGROUP,      //�г���
	PTOP             //ͬ��֮�䵥������
};

class TalkWindowShell : public BasicWindow
{
	Q_OBJECT

public:
	TalkWindowShell(QWidget *parent = Q_NULLPTR);
	~TalkWindowShell();

public:
	//����µ����촰�ڣ�
	void addTalkWindow(TalkWindow* talkWindow, TalkWindowItem* talkWindowItem,const QString uid/*,GroupType grouptype*/);

	//���õ�ǰ���촰��
	void setCurrentWidget(QWidget* widget);

	const QMap<QListWidgetItem*, QWidget*>& getTalkWindowItemMap() const;

private:
	void initControl();//ȹʼ���ؼ�
	void initTcpSocket(); //��ʼ��TCP
	void initUdpSocket();//��ʼ��UDP
	void getEmployeesID(QStringList &employeesList);// ��ȡ����Ա��QQ��
	bool createJSFile(QStringList &employeesList);
	void handleReceiveMsg(int  senderEmployeeID, int msgType, QString strMsg);

public slots:
	void onEmotionBtnClicked(bool);//���鰴ť�����ִ�к�Ĳۺ���

	//�ͻ��˷���tcp����(���ݣ��������ͣ��ļ�)
	void updateSendTcpMsg(QString& strData, int &msgType, QString fileName = "");

private slots:
	void onTalkWindowItemClicked(QListWidgetItem* item);//����б�����ִ�еĲۺ���
	void onEmotionItemClicked(int emotionNum);//���鱻ѡ��
	void processPeningData();//����UDP�㲥������

private:
	Ui::TalkWindowClass ui;
	QMap<QListWidgetItem*, QWidget*> m_talkwindowItemMap;//�򿪵����촰��
	EmotionWindow* m_emotionWindow;  //���鴰��

private:
	QTcpSocket *m_tcpClientSocket;  //tcp�ͻ���
	QUdpSocket *m_udpReceiver;     //udp���ն�
};
