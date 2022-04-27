#pragma once

#include <QWidget>
#include "TalkWindowShell.h"
#include "ui_TalkWindow.h"

enum GroupType;

class TalkWindow : public QWidget
{
	Q_OBJECT

public:
	TalkWindow(QWidget *parent,const QString& uid/*,GroupType groupType*/);
	~TalkWindow();

public:
	void addEmotionImage(int emotionNum);
	void setWindowName(const QString& name);
	QString getTalkId();

private slots:
	void onFileOpenBtnClicked(bool);
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	void initControl();
	void initGroupTalkStatus();
	int getCompDepID();

	//void initConpanyTalk();//��ʼ����˾����
	//void initPerisonnalTalk();
	//void initMarketTalk();
	//void initDevelopTalk();

	void initTalkWindow(); //��ʼ��Ⱥ��
	void initPtoPTalk();   //��ʼ������
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem, int employeeID);
private:
	Ui::TalkWindow ui;
	QString m_talkId;
	bool m_isGroupTalk;  //�Ƿ�Ⱥ��
//	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap; //���з�����ϵ������
	friend class TalkWindowShell;
};

