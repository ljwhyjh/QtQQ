#pragma once

#include <QObject>

class NotifyManager : public QObject
{
	Q_OBJECT

public:
	NotifyManager();
	~NotifyManager();

signals:
	void signalSkinChanged(const QColor& color);			// 改变颜色，信号

public:
	static NotifyManager* getInstance();			// 获取操作实例

	void notifyOtherWindowChangeSkin(const QColor& color);


private:
	static NotifyManager* instance;


};
