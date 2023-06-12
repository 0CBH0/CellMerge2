#include "ClickQLabel.h"

ClickQLabel::ClickQLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent)
{
	setMouseTracking(true);
	oldPos = QPoint(0, 0);
	mouseStatus = 0;
}

void ClickQLabel::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		emit clicked();
	}
	if (event->button() == Qt::LeftButton && mouseStatus == 0)
	{
		mouseStatus = 1;
		oldPos = event->pos();
	}
	else if (event->button() == Qt::RightButton && mouseStatus == 0)
	{
		mouseStatus = 2;
		oldPos = event->pos();
	}
}

void ClickQLabel::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && mouseStatus == 1)
	{
		mouseStatus = 0;
		emit moved(oldPos.x(), oldPos.y(), event->pos().x(), event->pos().y());
	}
	else if (event->button() == Qt::RightButton && mouseStatus == 2)
	{
		mouseStatus = 0;
		emit selected(oldPos.x(), oldPos.y(), event->pos().x(), event->pos().y());
	}
}

void ClickQLabel::mouseMoveEvent(QMouseEvent* event)
{
	if (mouseStatus == 2)
	{
		emit selecting(oldPos.x(), oldPos.y(), event->pos().x(), event->pos().y());
	}
}

void ClickQLabel::wheelEvent(QWheelEvent* event)
{
	emit scrolled(event->pos().x(), event->pos().y(), event->delta());
}
