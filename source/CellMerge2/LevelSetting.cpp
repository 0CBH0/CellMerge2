#include "LevelSetting.h"

using namespace cv;

LevelSetting::LevelSetting(QWidget *parent) : QWidget(parent)
{
	imgPixel = NULL;
	initial();
	enable = false;
	calcPressed = false;
	midPressed = false;
    leftPressed = false;
    rightPressed = false;
}

LevelSetting::~LevelSetting()
{
	delete[]imgPixel;
}

void LevelSetting::initial()
{
	if (imgPixel != NULL) delete[]imgPixel;
	imgPixel = NULL;
	memset(hist, 0, sizeof(size_t) * 256);
	midValue = 1.0;
	leftValue = 0;
	rightValue = 255;
	minPixel = 0;
	maxPixel = 0;
	update();
}

void LevelSetting::mousePressEvent(QMouseEvent *e)
{
    if (e->button() & Qt::LeftButton) {
        if (leftSliderRect.contains(e->pos())) {
            leftPressed = true;
            update();
        } else if (rightSliderRect.contains(e->pos())) {
            rightPressed = true;
            update();
        }
		else if (midSliderRect.contains(e->pos())) {
			midPressed = true;
			update();
		}
		else if (calcRect.contains(e->pos())) {
			calcPressed = true;
		}
    }
}

void LevelSetting::mouseReleaseEvent(QMouseEvent *)
{
    leftPressed = false;
    rightPressed = false;
	midPressed = false;
	if (calcPressed == true) calcPressed = false;
    update();
}

void LevelSetting::mouseMoveEvent(QMouseEvent *e)
{
    if (leftPressed)
	{
		int valueOri = leftValue;
		leftValue = min(rightValue - 5, max(0, (e->pos().x() - 6) * 255 / (this->width() - 12)));
		emit valueChanged(leftValue, midValue, rightValue);
		update();
    }
	else if (rightPressed)
	{
		int valueOri = rightValue;
		rightValue = min(255, max(leftValue + 5, (e->pos().x() - 6) * 255 / (this->width() - 12)));
		emit valueChanged(leftValue, midValue, rightValue);
		update();
    }
	else if (midPressed)
	{
		midValue = pow((min(rightValue - 5.0, max(leftValue + 5.0, 255.0 * (e->pos().x() - 6.0) / (this->width() - 12))) - leftValue) * 2 / (rightValue - leftValue), 3.0);
		emit valueChanged(leftValue, midValue, rightValue);
		update();
	}
}

void LevelSetting::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    drawBackGround(&painter);
	drawImage(&painter);
	drawSlider(&painter);
}

void LevelSetting::drawBackGround(QPainter *painter)
{
    painter->save();
    int width = this->width();
    int height = this->height();
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Qt::black);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
	calcRect = QRect(6, 6, width - 12, height - 30);
	QPointF points[5] = {
		QPointF(4.0, 4.0),
		QPointF(width - 4.0, 4.0),
		QPointF(width - 4.0, height - 15.0),
		QPointF(4.0, height - 15.0),
		QPointF(4.0, 4.0)
	};
	painter->drawPolyline(points, 5);
    painter->restore();
}

void LevelSetting::drawImage(QPainter *painter)
{
	painter->save();
	int width = this->width();
	int height = this->height();
	QPen pen;
	pen.setWidth(1);
	pen.setColor(Qt::black);
	pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);
	size_t rangeY = maxPixel - minPixel;
	int y2 = height - 15;
	if (imgPixel != NULL & rangeY > 0) for (int i = 5; i < width - 5; i++) painter->drawLine(i, imgPixel[i - 5], i, y2);
	painter->restore();
}

void LevelSetting::drawSlider(QPainter *painter)
{
	painter->save();
	int width = this->width();
	int height = this->height();
	QPen pen;
	pen.setWidth(1);
	pen.setColor(Qt::black);
	pen.setCapStyle(Qt::RoundCap);
	painter->setPen(pen);
	QBrush sliderBrush[3] = { Qt::white, Qt::lightGray, Qt::black };
	QPointF sliderPos[3] = {
		QPointF(leftValue * (width - 12) / 255 + 6, height - 10),
		QPointF(0, height - 10),
		QPointF(rightValue * (width - 12) / 255 + 6, height - 10)
	};
	sliderPos[1].setX(pow(midValue, 1.0 / 3)*(sliderPos[2].x() - sliderPos[0].x()) / 2 + sliderPos[0].x());
	leftSliderRect = QRect(sliderPos[0].x() - 5, sliderPos[0].y() - 5, 10, 10);
	midSliderRect = QRect(sliderPos[1].x() - 5, sliderPos[1].y() - 5, 10, 10);
	rightSliderRect = QRect(sliderPos[2].x() - 5, sliderPos[2].y() - 5, 10, 10);
	for (int i = 0; i < 3; i++)
	{
		QPointF points[6] = {
			QPointF(0.0 + sliderPos[i].x(), -10.0 + sliderPos[i].y()),
			QPointF(5.0 + sliderPos[i].x(), -5.0 + sliderPos[i].y()),
			QPointF(5.0 + sliderPos[i].x(), 5.0 + sliderPos[i].y()),
			QPointF(-5.0 + sliderPos[i].x(), 5 + sliderPos[i].y()),
			QPointF(-5.0 + sliderPos[i].x(), -5.0 + sliderPos[i].y()),
			QPointF(0.0 + sliderPos[i].x(), -10.0 + sliderPos[i].y())
		};
		painter->setBrush(sliderBrush[i]);
		painter->drawPolygon(points, 6);
	}
	painter->restore();
}

int LevelSetting::getLeftValue()
{
    return this->leftValue;
}

int LevelSetting::getRightValue()
{
    return this->rightValue;
}

double LevelSetting::getMidValue()
{
	return this->midValue;
}

void LevelSetting::setCurrentRange(int left, double mid, int right)
{
	if (left > right - 5 || right < left + 5 || mid <= 0.0 || mid >= 8.0 || right > 255 || left < 0) return;
	this->midValue = mid;
    this->leftValue = left;
    this->rightValue = right;
    emit valueChanged(left, mid, right);
    update();
}

void LevelSetting::setMidValue(double mid)
{
	setCurrentRange(this->leftValue, mid, this->rightValue);
}

void LevelSetting::setLeftValue(int left)
{
    setCurrentRange(left, this->midValue, this->rightValue);
}

void LevelSetting::setRightValue(int right)
{
    setCurrentRange(this->leftValue, this->midValue, right);
}

void LevelSetting::calcHist(Mat *image)
{
	if (imgPixel != NULL) delete[]imgPixel;
	imgPixel = new size_t[this->width() - 10];
	memset(hist, 0, sizeof(size_t) * 256);
	for (int i = 0; i < image->rows; i++) for (int j = 0; j < image->cols; j++) hist[image->at<uchar>(i, j)]++;
	hist[0] = 0;
	int rangeX = this->width() - 10;
	if (rangeX >= 256) for (int i = 0; i < rangeX; i++) imgPixel[i] = hist[256 * i / rangeX];
	else
	{
		for (int i = 0; i < rangeX - 1; i++)
		{
			imgPixel[i] = 0;
			for (int j = 256 * i / rangeX; j < 256 * (i + 1) / rangeX; j++) imgPixel[i] += hist[j];
		}
		imgPixel[rangeX - 1] = hist[255];
	}
	minPixel = imgPixel[0];
	maxPixel = imgPixel[0];
	for (int i = 0; i < rangeX; i++)
	{
		if (imgPixel[i] < minPixel) minPixel = imgPixel[i];
		if (imgPixel[i] > maxPixel) maxPixel = imgPixel[i];
	}
	size_t rangeY = maxPixel - minPixel;
	int imageHeight = this->height() - 30;
	if (rangeY > 0) for (int i = 0; i < rangeX; i++) imgPixel[i] = this->height() - imgPixel[i] * imageHeight / rangeY - 15;
	update();
}
