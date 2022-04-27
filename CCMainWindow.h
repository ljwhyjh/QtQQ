			#pragma once

#include "BasicWindow.h"
#include "ui_CCMainWindow.h"

class QTreeWidgetItem;

class CCMainWindow : public BasicWindow
{
	Q_OBJECT

public:
	CCMainWindow(QString account,bool isAccountLogin,QWidget *parent = Q_NULLPTR);
	~CCMainWindow();

public:
	
	void setUserName(const QString& username);  //�����û���
	void setUserSign(const QString& usersign); //�����û�ǩ��
	void setLevelPixmap(int level);             //���ö���
	void setHeadPixmap(const QString& headPath);//����ͷ��
	void setStatusMenuIcon(const QString& statusPath); //����״̬
	
	//���Ӧ�ò���(appͼƬ·����app����������)
	QWidget* addOtherAppExtension(const QString& appPath,const QString& appName);
	void initContactTree();
	void initFriendTree();

private:
	void initTimer();   //��ʼ����ʱ��
	void initControl();
	QString getHeadPicturePath();
	void getUsername(QString &strName, QString &strSign);
	void updateSeachStyle();//����������ʽ
	//void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, const QString& sDeps);
	void addCompanyDeps(QTreeWidgetItem* pRootGroupItem, int DepID);
	void addPeopInfo(QTreeWidgetItem* pRootGroupItem, int employeeID);

private:
	void resizeEvent(QResizeEvent* event);
	bool eventFilter(QObject* obj, QEvent* event);
	void mousePressEvent(QMouseEvent* event);

private slots:
	void onItemClicked(QTreeWidgetItem *item, int column);
	void onItemExpanded(QTreeWidgetItem *item);
	void onItemCollapsed(QTreeWidgetItem *item);
	void onItemDoubleClicked(QTreeWidgetItem *item, int column);
	void onAppIconClicked();
	void on_friendBtn_clicked();
	void on_contactBtn_clicked();
	void on_msgBtn_clicked();

private:
	Ui::CCMainWindowClass ui;
	bool m_isAccountLogin;
	QString m_account; //��¼���˺Ż�QQ��
	QMap<QTreeWidgetItem*, QString> m_groupPeopleMap; //���з�����ϵ������
	//QMap<QTreeWidgetItem*, QString> m_groupMap; //���з���ķ�����
};
