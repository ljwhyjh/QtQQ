#include "QClickLabel.h"

#include <QMouseevent>

QClickLabel::QClickLabel(QWidget *parent)
	: QLabel(parent)
{
}

QClickLabel::~QClickLabel()
{
}

void QClickLabel::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked();
	}
}
