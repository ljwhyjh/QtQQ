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

private slots:
	void onSendBtnClicked(bool);
	void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
	void initControl();
	void initConpanyTalk();//初始化公司聊天
	void initPerisonnalTalk();
	void initMarketTalk();
	void initDevelopTalk();
	void initPtoPTalk();
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem);
private:
	Ui::TalkWindow ui;
	QString m_talkId;
//	GroupType m_groupType;
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap; //所有分组联系人姓名
};
