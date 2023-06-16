#pragma once

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include <QTextCodec>
#include <libxml/parser.h>
#include "ui_CellMerge2.h"
#include "ImageHandle.h"
#include "ItemSelector.h"
#include "CellCounter.h"


class CellMerge2 : public QMainWindow
{
    Q_OBJECT

public:
	// function
    CellMerge2(QWidget *parent = nullptr);
    ~CellMerge2();
	void calcHist();

	// variant
	StatForm statForm;
	ImageData imageData;
	QString projectName;

protected:
	virtual void resizeEvent(QResizeEvent *event) override;
	virtual void closeEvent(QCloseEvent *event) override;

private:
	// class
    Ui::CellMerge2Class ui;

	// function
	void createActions();
	void createMenus();
	int loadFile(QString fileName, QString type);
	int loadSeries(QStringList fileNames);
	int readCMR(QString fileName);
	int writeCMR(QString fileName);
	int readLIF(QString fileName);
	int readIMG(QString fileName);
	void setWidget(int id = 0);
	void setMode(int value);
	void disableWidget(bool stat);
	cv::Rect roiFix(cv::Rect term);

	// variant
	QString version;
	QLabel *statusMsgLabel;
	QColorDialog *colorDialog;

	cv::Mat imageMat;
	cv::Mat roiMat;
	QImage img;
	cv::Rect roi;
	int widgetMode;
	int layerRec;

	// menu
	QMenu *itemMenu;
	QMenu *helpMenu;

	// action
	QAction *openAction;
	QAction *saveAction;
	QAction *importAction;
	QAction *exportAction;
	QAction *aboutAction;

private slots:
	void initial();
	void release();
	void refreshView(bool re = false);

	void openFunc();
	void saveFunc();
	void importFunc();
	void exportFunc();
	void aboutFunc();

	void scrollFunc(int x, int y, int val);
	void moveFunc(int xa, int ya, int xb, int yb);
	void drawRoiFunc(int xa, int ya, int xb, int yb);
	void selRoiFunc(int xa, int ya, int xb, int yb);
	void selDepthFunc(int value);
	void setRoiFunc();
	void resetRoiFunc();
	void dpMinFunc();
	void dpMaxFunc();
	void setNameFunc();
	void setColorFunc();
	void setLimitValue(int minValue, double midValue, int maxValue);
	void setWeightFunc(int value);
	void displayFunc(int value);
	void selLayerFunc(int value);
	void modeChange();
	void resetLayerFunc();
	void statShowFunc();
	void statHideFunc();
	void statCalcFunc();

};
