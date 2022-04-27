#pragma once

#include <QPixmap>
#include <QSize>
#include <QProxyStyle>


// �ı� Ĭ�ϵĲ������
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
			// ȥ��Windows�в���Ĭ�ϵı߿�, �� ���߿�
			// ������ȡ����ʱ, ֱ�ӷ���
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
	// ��ȡͼƬ��Դ
	static QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize = QSize(0,0));
	
	// ����Ƥ��
	static void loadStyleSheet(QWidget* widget, const QString& sheetName);

	// Ĭ�ϵ���ɫ
	static void setDefaultSkinColor(const QColor& color);

	// ��ȡ��ɫ
	static QColor getDefaultSkinColor();





};

