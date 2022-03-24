#pragma once

#include <QDialog>
#include "TitleBar.h"

class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

public:
	// 加载样式表
	void loadStyleSheet(const QString& sheetName);

	// 获取圆头像
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0, 0));


private:
	void initBackGroundColor();													// 初始化背景 , 颜色


protected:
	void paintEvent(QPaintEvent* event);										// 绘制事件
	void mousePressEvent(QMouseEvent* event);						// 鼠标事件
	void mouseMoveEvent(QMouseEvent* event);						// 鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event);					// 鼠标松开事件 , 就是 鼠标释放

protected:
	void initTitleBar(ButtonType buttonType = MIN_BUTTON);							// 初始化标题栏
	void setTitleBarTitle(const QString& title, const QString& icon = "");			// 标题栏内容


public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onSignalSkinChange(const QColor &color);					// 皮肤改变

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	QPoint m_mousePoint;								// 鼠标位置
	bool m_mousePressed;								// 鼠标是否按下
	QColor m_colorBackGround;						// 背景色
	QString m_styleName;								// 样式文件名称
	TitleBar* m_titleBar;										// 标题栏
};
