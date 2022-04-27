#include "BasicWindow.h"
#include "NotifyManager.h"

#include <QFile>
#include <QStyleOption>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <qdesktopwidget.h>
#include <QSqlQuery>
#include "CommonUtils.h"

extern QString gLoginEmployeeID;//登录者QQ号(员工号)

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	// 获取默认的颜色值
	m_colorBackGround = CommonUtils::getDefaultSkinColor();

	// 先设置窗口风格
	setWindowFlags(Qt::FramelessWindowHint);							// 无边框
	setAttribute(Qt::WA_TranslucentBackground,true);				// 透明效果

	// 发射更改皮肤信号，修改皮肤
	connect(NotifyManager::getInstance(), 
					SIGNAL(signalSkinChanged(const QColor&)),
					this, 
					SLOT(onSignalSkinChange(const QColor&)));
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::loadStyleSheet(const QString& sheetName)
{
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);

	if (file.isOpen())
	{
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());			// 数据内容全部读取出来, 保存到 qstyleSheet字符串里

		// 获取当前用户的皮肤RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());
		
		// titleskin 标题皮肤属性为 true
		// 底部的皮肤 bottomskin 为 true
		// rgba 的 a ,是透明度
		qsstyleSheet += QString("QWidget[titleskin=true]\
												{background-color:rgb(%1,%2,%3);\
												border-top-left-radius:4px;}\
												QWidget[bottomskin=true]\
												{border-top:1px solid rgba(%1,%2,%3,100);\
												background-color:rgba(%1,%2,%3,50);\
												border-bottom-left-radius:4px;\
												border-bottom-right-radius:4px;}")
												.arg(r).arg(g).arg(b);
		// 设置样式表
		setStyleSheet(qsstyleSheet);
	}

	file.close();

}
// src , 来源  ,   mask , 背景, 一个空的圆头像  ,   maskSize, 尺寸大小
QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize)
{
	if (maskSize == QSize(0,0))
	{
		maskSize = mask.size();
	}
	else
	{	
		// Qt::KeepAspectRatio , 缩放时, 尽可能以大的矩形缩放
		// Qt::SmoothTransformation , 平滑方式
		mask = mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}

	// 保存转换后的头像
	// QImage::Format_ARGB32_Premultiplied 格式, 获取的头像会比较清晰
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);

	// CompositionMode , 图片的叠加模式
	QPainter painter(&resultImage);				// 定义一个画家, 画 resultImage
	painter.setCompositionMode(QPainter::CompositionMode_Source);			// 设置图片叠加模式, 将源文件以 复制进行操作 ， Source 将原图片，直接拷贝过来
	painter.fillRect(resultImage.rect(), Qt::transparent);										// 填充矩形, 矩形的大小, 就是图片的大小 , 同时是透明的 transparent
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);	// 用SourceOver方式, 进行叠加
	painter.drawPixmap(0, 0, mask);																		// 对空白的圆形图片 , 进行叠加
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);		// 用SourceIn方式, 进行叠加
	painter.drawPixmap(0, 0, src.scaled(maskSize,Qt::KeepAspectRatio, Qt::SmoothTransformation));		// 在对目标的QQ头像, 进行比例设置 
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::initBackGroundColor()
{
	QStyleOption opt;
	opt.init(this);

	QPainter p(this);						// 用this是 "当前"
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p , this);				// 图源, 风格
}

// 子类化部件时, 需要重写绘图事件 设置背景图
void BasicWindow::paintEvent(QPaintEvent* event)
{
	initBackGroundColor();
	QDialog::paintEvent(event);
}

void BasicWindow::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		// pos() , 事件发生时, 相对于窗口左上角的 偏移坐标
		m_mousePressed = true;
		m_mousePoint = event->globalPos() - pos();		// 减去 坐标值
		event->accept();			// 接受
	}
}

//鼠标移动事件
void BasicWindow::mouseMoveEvent(QMouseEvent* event)
{
	if (m_mousePressed && (event->buttons() && Qt::LeftButton))
	{
		// globalPos() , 获取当前事件发生时, 鼠标全局的坐标值
		move(event->globalPos() - m_mousePoint);
		event->accept();
	}
}

void BasicWindow::mouseReleaseEvent(QMouseEvent* event)
{
	m_mousePressed = false;
}


void BasicWindow::onShowClose(bool)
{
	close();
}

void BasicWindow::onShowMin(bool)
{
	showMinimized();
}

void BasicWindow::onShowHide(bool)
{
	hide();
}

void BasicWindow::onShowNormal(bool)
{
	show();									// 直接显示
	activateWindow();				// 设置为活动的窗口
}

void BasicWindow::onShowQuit(bool)
{
	//更新登录状态为在线
	QString strSqlStatus = QString("UPDATE tab_employees SET online=1 WHERE employeeID= %1").arg(gLoginEmployeeID);
	QSqlQuery sqlStatus(strSqlStatus);
	sqlStatus.exec();

	QApplication::quit();
}

void BasicWindow::onSignalSkinChange(const QColor& color)
{
	// 修改颜色 , 加载样式表, 重新添加
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

void BasicWindow::onButtonMinClicked()
{
	// 判断当前窗体,是不是工具 , 
	// 如果是的话 , 就表示 当前窗体 有工具风格 , 就进行 隐藏
	// 如果不是 ,就 进行最小化
	if (Qt::Tool == (windowFlags() & Qt::Tool))
	{
		hide();
	}
	else
	{
		showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked()
{
	QPoint windowPos;
	QSize windowSize;

	// 获取窗体的位置 , 大小信息
	m_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonMaxClicked()
{
	// 先保存窗体之前的 位置 , 大小高度,宽度
	m_titleBar->saveRestoreInfo(pos(), QSize(width(),height()));

	// desktopRect , 桌面矩形
	QRect desktopRect = QApplication::desktop()->availableGeometry();			// 获取桌面信息

	// factRect , 实际矩形
	QRect factRect = QRect( desktopRect.x() - 3, desktopRect.y() - 3 , 
											desktopRect.width() + 6, desktopRect.height() + 6);
	// 设置矩形
	setGeometry(factRect);
}

void BasicWindow::onButtonCloseClicked()
{
	close();
}

void BasicWindow::initTitleBar(ButtonType buttonType)
{
	m_titleBar = new TitleBar(this);
	m_titleBar->setButtonType(buttonType);
	m_titleBar->move(0, 0);

	connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMinClicked()));
	connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString& icon)
{
	m_titleBar->setTitleIcon(icon);
	m_titleBar->setTitleContent(title);
}
