#pragma once

#include <QWebEngineView>
#include <QDomNode>

class MsgHtmlObj: public QObject
{
	Q_OBJECT
	
	Q_PROPERTY(QString msgLHtmlTmpl MEMBER m_msgLHtmlTmpl NOTIFY signalMsgHtml) //��������Ϣ	
	Q_PROPERTY(QString msgRHtmlTmpl MEMBER m_msgRHtmlTmpl NOTIFY signalMsgHtml) //�ҷ�����Ϣ

public:
	MsgHtmlObj(QObject* parent);

signals:
	void signalMsgHtml(const QString& html);


private:
	void initHtmlTmpl();  // ��ʼ��������ҳ
	QString getMsgTmplHtml(const QString& code);

private:
	QString m_msgLHtmlTmpl;//���˷�������Ϣ
	QString m_msgRHtmlTmpl;//�ҷ�����Ϣ
};

class MsgWebPage : public QWebEnginePage
{
	Q_OBJECT
public:
	MsgWebPage(QObject* parent = nullptr) :QWebEnginePage(parent)
	{

	}
protected:
	bool acceptNavigationRequest(const QUrl& url, NavigationType type, bool isMainFrame);

};

//QWebEngineView��ͼ������ʾ��ҳ����
class MsgWebView : public QWebEngineView
{
	Q_OBJECT

public:
	MsgWebView(QWidget *parent);
	~MsgWebView();
	void appendMsg(const QString& html);

private:
	QList<QStringList> parseHtml(const QString& html); //����html
	//Qt������DOM��㣨���ԡ�˵�����ı��ȣ�������ʹ��QDomNode���б�ʾ
	QList<QStringList> parseDocNode(const QDomNode& node);//�������

private:
	MsgHtmlObj* m_msgHtmlObj;
};
