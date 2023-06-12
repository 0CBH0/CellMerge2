#pragma once

#include <QLabel>
#include <QWidget>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <Qt>


class ClickQLabel : public QLabel { 
    Q_OBJECT 

public:
    explicit ClickQLabel(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

Q_SIGNALS:
    void clicked();
	void scrolled(int x, int y, int val);
	void moved(int xa, int ya, int xb, int yb);
	void selecting(int xa, int ya, int xb, int yb);
	void selected(int xa, int ya, int xb, int yb);

protected:
    void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
	int mouseStatus;
	QPoint oldPos;

};
