#pragma once

#include <QPixmap>
#include <QSize>
#include <QProxyStyle>


// 改变 默认的部件风格
class CustomProxyStyle : public QProxyStyle
{
public:
	CustomProxyStyle(QObject* parent)
	{
		setParent(parent);
	}

	virtual void drawPrimitive(PrimitiveElement element, const QStyleOption* option,
		QPainter* painter, const QWidget* widget = nullptr)const
	{
		if (PE_FrameFocusRect == element)
		{
			// 去掉Windows中部件默认的边框, 或 虚线框
			// 部件获取焦点时, 直接返回
			return;
		}
		else
		{
			QProxyStyle::drawPrimitive(element, option, painter, widget);
		}
	}

public:

};



// =====================================
class CommonUtils
{
public:
	CommonUtils();

public:
	// 获取图片资源
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0,0));
	
	// 加载皮肤
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);

	// 默认的颜色
	static void setDefaultSkinColor(const QColor& color);

	// 获取颜色
	static QColor getDefaultSkinColor();





};

