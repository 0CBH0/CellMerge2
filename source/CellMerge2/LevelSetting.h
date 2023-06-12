#pragma once

#include <QWidget>
#include <qpainter.h>
#include <qevent.h>
#include <qmath.h>
#include <opencv2/opencv.hpp>


class LevelSetting : public QWidget
{
    Q_OBJECT
	
	Q_PROPERTY(int midValue READ getMidValue WRITE setMidValue)
    Q_PROPERTY(int leftValue READ getLeftValue WRITE setLeftValue)
    Q_PROPERTY(int rightValue READ getRightValue WRITE setRightValue)

public:
    explicit LevelSetting(QWidget *parent = 0);
    ~LevelSetting();

	void initial();
	void calcHist(cv::Mat *image);
	double getMidValue();
	int getLeftValue();
	int getRightValue();

	bool enable;

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawBackGround(QPainter *painter);
    void drawImage(QPainter *painter);
    void drawSlider(QPainter *painter);

private:
	size_t hist[256];
	size_t *imgPixel;
	double midValue;
    int leftValue;
    int rightValue;
	int minPixel;
	int maxPixel;

	bool calcPressed;
    bool leftPressed;
    bool rightPressed;
	bool midPressed;

	QRect calcRect;
    QRect leftSliderRect;
    QRect rightSliderRect;
	QRect midSliderRect;

public Q_SLOTS:

    void setCurrentRange(int left, double mid, int right);
	void setMidValue(double mid);
    void setLeftValue(int left);
    void setRightValue(int right);

Q_SIGNALS:
    void valueChanged(int left, double mid, int right);
};
