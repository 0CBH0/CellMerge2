#include "CellMerge2.h"

using namespace std;
using namespace cv;

CellMerge2::CellMerge2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	version = QStringLiteral("1.9");
	setWindowTitle(QStringLiteral("CellMerge2  v%1").arg(version));
	setContextMenuPolicy(Qt::NoContextMenu);
	setMouseTracking(true);
	createActions();
	createMenus();
	colorDialog = new QColorDialog;
	statusMsgLabel = new QLabel;
	ui.statusBar->addWidget(statusMsgLabel);

	connect(ui.mainForm, SIGNAL(scrolled(int, int, int)), this, SLOT(scrollFunc(int, int, int)));
	connect(ui.mainForm, SIGNAL(moved(int, int, int, int)), this, SLOT(moveFunc(int, int, int, int)));
	connect(ui.mainForm, SIGNAL(selecting(int, int, int, int)), this, SLOT(drawRoiFunc(int, int, int, int)));
	connect(ui.mainForm, SIGNAL(selected(int, int, int, int)), this, SLOT(selRoiFunc(int, int, int, int)));
	connect(ui.depthSlider, SIGNAL(valueChanged(int)), this, SLOT(selDepthFunc(int)));
	connect(ui.roiEdit, SIGNAL(editingFinished()), this, SLOT(setRoiFunc()));
	connect(ui.roiButton, SIGNAL(clicked()), this, SLOT(resetRoiFunc()));
	connect(ui.nameEdit, SIGNAL(editingFinished()), this, SLOT(setNameFunc()));
	connect(ui.layerColor, SIGNAL(clicked()), this, SLOT(setColorFunc()));
	connect(ui.levelSetting, SIGNAL(valueChanged(int, double, int)), this, SLOT(setLimitValue(int, double, int)));
	connect(ui.weightSlider, SIGNAL(valueChanged(int)), this, SLOT(setWeightFunc(int)));
	connect(ui.displayBox, SIGNAL(stateChanged(int)), this, SLOT(displayFunc(int)));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(resetLayerFunc()));
	connect(ui.selectBox, SIGNAL(currentIndexChanged(int)), this, SLOT(selLayerFunc(int)));
	connect(ui.editButton, SIGNAL(clicked()), this, SLOT(modeChange()));
	connect(ui.statButton, SIGNAL(clicked()), this, SLOT(statShowFunc()));
	connect(statForm.hideButton, SIGNAL(clicked()), this, SLOT(statHideFunc()));
	connect(statForm.calcButton, SIGNAL(clicked()), this, SLOT(statCalcFunc()));

	initial();
}

CellMerge2::~CellMerge2()
{
	release();
}

void CellMerge2::initial()
{
	release();
	projectName = "";
	roi = Rect(0, 0, 0, 0);
	imageData.initial();
	ui.depthSlider->setRange(0, 0);
	ui.depthSlider->setValue(0);
	ui.weightSlider->setRange(0, 100);
	ui.weightSlider->setValue(100);
	ui.displayBox->setChecked(false);
	ui.roiEdit->setText("");
	ui.nameEdit->setText("");
	ui.layerColor->setStyleSheet("background-color:rgb(240,240,240)");
	ui.mainForm->setPixmap(QPixmap());
	ui.levelSetting->setCurrentRange(1, 1.0, 255);
	setMode(0);
	layerRec = 0;
	calcHist();
}

void CellMerge2::setMode(int value)
{
	widgetMode = value;
	if (widgetMode == 0)
	{
		ui.setLabel->setText(QStringLiteral("权重"));
		ui.infoLabel->setText(QStringLiteral("当前通道"));
		ui.editButton->setText(QStringLiteral("显示区域"));
	}
	else
	{
		ui.setLabel->setText(QStringLiteral("尺度"));
		ui.infoLabel->setText(QStringLiteral("参考通道"));
		ui.editButton->setText(QStringLiteral("显示通道"));
	}
}

void CellMerge2::modeChange()
{
	if (imageData.layerList.size() == 0) return;
	if (widgetMode == 0)
	{
		setMode(1);
		layerRec = ui.selectBox->currentIndex();
		imageData.nucId = layerRec;
		ui.selectBox->setCurrentIndex(imageData.nucId);
		ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[imageData.nucId].name));
		setWidget();
	}
	else
	{
		setMode(0);
		layerRec = ui.selectBox->currentIndex();
		ui.selectBox->setCurrentIndex(layerRec);
		ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[layerRec].name));
		setWidget(layerRec);
	}
	calcHist();
	refreshView(true);
}

void CellMerge2::setWidget(int id)
{
	if (id >= imageData.layerList.size()) return;
	if (widgetMode == 0)
	{
		ui.nameEdit->setText(QString::fromStdString(imageData.layerList[id].name));
		Vec3b color = imageData.imgMode == 1 ? Vec3b(255, 255, 255) : imageData.layerList[id].color;
		ui.layerColor->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(color[0]).arg(color[1]).arg(color[2]));
		ui.weightSlider->setRange(0, 100);
		ui.weightSlider->setValue((int)round(imageData.layerList[id].weight * 100));
		ui.displayBox->setChecked(imageData.layerList[id].display);
		ui.levelSetting->setCurrentRange(imageData.layerList[id].minVal, imageData.layerList[id].percent, imageData.layerList[id].maxVal);
	}
	else
	{
		ui.nameEdit->setText(QString::fromStdString(imageData.cellLayer.name));
		Vec3b color = imageData.cellLayer.color;
		ui.layerColor->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(color[0]).arg(color[1]).arg(color[2]));
		ui.weightSlider->setValue(imageData.layerList[imageData.nucId].ks);
		ui.weightSlider->setRange(1, 40);
		ui.displayBox->setChecked(imageData.cellLayer.display);
		ui.levelSetting->setCurrentRange(imageData.layerList[imageData.nucId].minVal, imageData.layerList[imageData.nucId].percent, imageData.layerList[imageData.nucId].maxVal);
	}
}

void CellMerge2::disableWidget(bool stat)
{
	ui.menuBar->setDisabled(stat);
	ui.mainForm->setDisabled(stat);
	ui.depthSlider->setDisabled(stat);
	ui.nameEdit->setDisabled(stat);
	ui.layerColor->setDisabled(stat);
	ui.levelSetting->setDisabled(stat);
	ui.weightSlider->setDisabled(stat);
	ui.displayBox->setDisabled(stat);
	ui.resetButton->setDisabled(stat);
	ui.selectBox->setDisabled(stat);
	ui.editButton->setDisabled(stat);
	ui.statButton->setDisabled(stat);
	statForm.table->setDisabled(stat);
	statForm.layerBox->setDisabled(stat);
	statForm.modeBox->setDisabled(stat);
	statForm.calcButton->setDisabled(stat);
	statForm.exportButton->setDisabled(stat);
	statForm.hideButton->setDisabled(stat);
}

void CellMerge2::release()
{
	imageMat.release();
	roiMat.release();
	imageData.release();
}

void CellMerge2::closeEvent(QCloseEvent *event)
{
	statForm.close();
	release();
}

void CellMerge2::aboutFunc()
{
	QMessageBox::about(this, QStringLiteral("使用说明"),
		QStringLiteral("<h2>CellMerge2</h2>"
			"<p>Version：%1  CBH 制"
			"<p><a href='https://github.com/0CBH0/CellMerge2/wiki'>使用教程Wiki</a>").arg(version));
}

void CellMerge2::createActions()
{
	openAction = new QAction(QStringLiteral("打开"), this);
	openAction->setIcon(QIcon(":/CellMerge2/Resources/open.png"));
	openAction->setShortcut(QKeySequence::Open);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFunc()));
	saveAction = new QAction(QStringLiteral("保存"), this);
	saveAction->setIcon(QIcon(":/CellMerge2/Resources/save.png"));
	saveAction->setShortcut(QKeySequence::Save);
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFunc()));
	importAction = new QAction(QStringLiteral("导入"), this);
	importAction->setIcon(QIcon(":/CellMerge2/Resources/import.png"));
	connect(importAction, SIGNAL(triggered()), this, SLOT(importFunc()));
	exportAction = new QAction(QStringLiteral("导出"), this);
	exportAction->setIcon(QIcon(":/CellMerge2/Resources/export.png"));
	connect(exportAction, SIGNAL(triggered()), this, SLOT(exportFunc()));
	aboutAction = new QAction(QStringLiteral("关于"), this);
	aboutAction->setIcon(QIcon(":/CellMerge2/Resources/help.png"));
	aboutAction->setShortcut(QKeySequence::HelpContents);
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutFunc()));
}

void CellMerge2::createMenus()
{
	itemMenu = menuBar()->addMenu(QStringLiteral("项目"));
	itemMenu->addAction(openAction);
	itemMenu->addAction(saveAction);
	itemMenu->addAction(importAction);
	itemMenu->addAction(exportAction);
	helpMenu = menuBar()->addMenu(QStringLiteral("帮助"));
	helpMenu->addAction(aboutAction);
}

void CellMerge2::openFunc()
{
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("选择项目文件..."), ".", QStringLiteral("项目文件 (*.cmr)"));
	if (fileName == "") return;
	disableWidget(true);
	int res = loadFile(fileName, "cmr");
	disableWidget(false);
	if (res == -1 || imageData.layerList.size() == 0)
	{
		projectName = "";
		QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("读取失败!"));
		return;
	}
	projectName = fileName;
}

void CellMerge2::saveFunc()
{
	if (imageData.layerList.size() == 0) return;
	if (projectName == "") projectName = QFileDialog::getSaveFileName(this, QStringLiteral("保存当前项目为..."), ".", QStringLiteral("项目文件 (*.cmr)"));
	if (projectName == "") return;
	disableWidget(true);
	int res = writeCMR(projectName);
	disableWidget(false);
}

void CellMerge2::importFunc()
{
	QStringList fileNames = QFileDialog::getOpenFileNames(this, QStringLiteral("选择图像文件..."), ".", QStringLiteral("图像文件 (*.*)"));
	if (fileNames.size() == 0 || fileNames[0] == "") return;
	disableWidget(true);
	if (fileNames.size() == 1)
	{
		QStringList info = fileNames[0].split(".");
		int res = loadFile(fileNames[0], info[info.size() - 1]);
		if (res == -1 || imageData.layerList.size() == 0) QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("读取失败!"));
		else projectName = info[info.size() - 1] == "cmr" ? fileNames[0] : "";
	}
	else
	{
		int res = loadSeries(fileNames);
		if (res == -1 || imageData.layerList.size() == 0) QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("读取失败!"));
		else projectName = "";
	}
	disableWidget(false);
}

void CellMerge2::exportFunc()
{
	if (imageData.layerList.size() == 0) return;
	QString dirName = QFileDialog::getExistingDirectory(this, QStringLiteral("选择导出图像位置..."));
	if (dirName == "") return;
	disableWidget(true);
	QFileInfo fileInfo(projectName);
	QString baseName = fileInfo.completeBaseName();
	Mat res = Mat::zeros(imageData.size, CV_8UC3);
	for (int i = 0; i < imageData.depth; i++)
	{
		Mat img = widgetMode == 0 ? imageData.getImage(i) : imageData.getCell(i);
		roiMat = img(roi);
		cvtColor(roiMat, res, COLOR_BGR2RGB);
		QString pathName = QDir::cleanPath(dirName + QDir::separator() + baseName + QString("_%1.png").arg(i, 2, 10, QLatin1Char('0')));
		QTextCodec *code = QTextCodec::codecForName("GB2312");
		string pathConv = code->fromUnicode(pathName).data();
		imwrite(pathConv, res);
	}
	res.release();
	disableWidget(false);
}

int CellMerge2::loadFile(QString fileName, QString type)
{
	int res = 0;
	if (type == "cmr") res = readCMR(fileName);
	else if (type == "lif") res = readLIF(fileName);
	else res = readIMG(fileName);
	if (res == -1 || imageData.layerList.size() == 0 || imageData.depth == 0) return res;
	imageData.calcMode();
	Size labelSize = Size(ui.mainForm->size().width(), ui.mainForm->size().height());
	roi = Rect(0, 0, imageData.size.width, imageData.size.height);
	setMode(0);
	disableWidget(false);
	ui.depthSlider->setRange(0, imageData.depth - 1);
	ui.depthSlider->setValue(0);
	ui.selectBox->clear();
	if (imageData.imgMode == 1)
	{
		imageData.nucId = 0;
		imageData.layerList[0].name = "BF_Low";
		imageData.layerList[1].name = "BF_High";
		imageData.layerList[0].minVal = 128;
		imageData.layerList[1].maxVal = 128;
		ui.selectBox->addItem(QString::fromStdString(imageData.layerList[0].name));
		ui.selectBox->addItem(QString::fromStdString(imageData.layerList[1].name));
		ui.selectBox->setCurrentIndex(0);
		ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[0].name));
		imageData.cellLayer.ori = Vec3b(255, 0, 0);
		imageData.cellLayer.color = Vec3b(255, 0, 0);
	}
	else
	{
		for (size_t i = 0; i < imageData.layerList.size(); i++) ui.selectBox->addItem(QString::fromStdString(imageData.layerList[i].name));
		ui.selectBox->setCurrentIndex(0);
		ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[0].name));
	}
	setWidget(0);
	calcHist();
	refreshView(true);
	return res;
}

int CellMerge2::loadSeries(QStringList fileNames)
{
	if (fileNames.size() == 0) return -1;
	fileNames.sort();
	QTextCodec *code = QTextCodec::codecForName("GB2312");
	string fileNameConv = code->fromUnicode(fileNames[0]).data();
	Mat img = imread(fileNameConv);
	if (!img.data) return -1;
	imageData.release();
	imageData.depth = fileNames.size();
	imageData.size = Size(img.cols, img.rows);
	imageData.addLayer("Blue", Vec3b(0, 0, 255));
	imageData.addLayer("Green", Vec3b(0, 255, 0));
	imageData.addLayer("Red", Vec3b(255, 0, 0));
	for (int i = 0; i < fileNames.size(); i++)
	{
		fileNameConv = code->fromUnicode(fileNames[i]).data();
		img = imread(fileNameConv);
		if (!img.data || imageData.size != Size(img.cols, img.rows))
		{
			imageData.release();
			return -1;
		}
		vector<Mat> chs;
		split(img, chs);
		imageData.layerList[0].matList[i] = chs[0].clone();
		imageData.normalize(0, i);
		imageData.layerList[1].matList[i] = chs[1].clone();
		imageData.normalize(1, i);
		imageData.layerList[2].matList[i] = chs[2].clone();
		imageData.normalize(2, i);
		imageData.nucId = 0;
		for (size_t j = 0; j < chs.size(); j++) chs[j].release();
		vector<Mat>().swap(chs);
	}
	img.release();
	imageData.calcMode();
	Size labelSize = Size(ui.mainForm->size().width(), ui.mainForm->size().height());
	roi = Rect(0, 0, imageData.size.width, imageData.size.height);
	setMode(0);
	disableWidget(false);
	ui.depthSlider->setRange(0, imageData.depth - 1);
	ui.depthSlider->setValue(0);
	ui.selectBox->clear();
	if (imageData.imgMode == 1)
	{
		imageData.nucId = 0;
		imageData.layerList[0].name = "BF_low";
		imageData.layerList[1].name = "BF_High";
		imageData.layerList[0].minVal = 128;
		imageData.layerList[1].maxVal = 128;
		ui.selectBox->addItem(QString::fromStdString(imageData.layerList[0].name));
		ui.selectBox->addItem(QString::fromStdString(imageData.layerList[1].name));
		ui.selectBox->setCurrentIndex(0);
		ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[0].name));
		imageData.cellLayer.ori = Vec3b(255, 0, 0);
		imageData.cellLayer.color = Vec3b(255, 0, 0);
	}
	else
	{
		for (size_t i = 0; i < imageData.layerList.size(); i++) ui.selectBox->addItem(QString::fromStdString(imageData.layerList[i].name));
		ui.selectBox->setCurrentIndex(0);
		ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[0].name));
	}
	setWidget(0);
	calcHist();
	refreshView(true);
	return 0;
}

int CellMerge2::readCMR(QString fileName)
{
	QTextCodec *code = QTextCodec::codecForName("GB2312");
	string fileNameConv = code->fromUnicode(fileName).data();
	FILE *cmr = fopen(fileNameConv.c_str(), "rb");
	if (cmr == NULL) return -1;
	fseek(cmr, 0, 2);
	if (ftell(cmr) < 0x20)
	{
		fclose(cmr);
		return -1;
	}
	rewind(cmr);
	uint ia;
	fread(&ia, 4, 1, cmr);
	if (ia != 0x43524D30)
	{
		fclose(cmr);
		return -1;
	}
	imageData.release();
	fread(&ia, 4, 1, cmr);
	imageData.size.width = ia;
	fread(&ia, 4, 1, cmr);
	imageData.size.height = ia;
	fread(&ia, 4, 1, cmr);
	imageData.depth = ia;
	fread(&ia, 4, 1, cmr);
	imageData.nucId = ia;
	fread(&ia, 4, 1, cmr);
	char name[0x40];
	fread(&name, 0x40, 1, cmr);
	imageData.cellLayer.name = string(name);
	imageData.cellLayer.color[0] = getc(cmr);
	imageData.cellLayer.color[1] = getc(cmr);
	imageData.cellLayer.color[2] = getc(cmr);
	imageData.cellLayer.ori[0] = getc(cmr);
	imageData.cellLayer.ori[1] = getc(cmr);
	imageData.cellLayer.ori[2] = getc(cmr);
	imageData.cellLayer.display = getc(cmr);
	imageData.cellLayer.type = getc(cmr);
	imageData.cellLayer.ks = getc(cmr);
	imageData.cellLayer.maxVal = getc(cmr);
	imageData.cellLayer.minVal = getc(cmr);
	imageData.cellLayer.percent = getc(cmr) / 100.0;
	imageData.cellLayer.weight = getc(cmr) / 100.0;
	for (uint i = 0; i < ia; i++)
	{
		fread(&name, 0x40, 1, cmr);
		imageData.addLayer(string(name), Vec3b(0, 0, 0));
		imageData.layerList[i].color[0] = getc(cmr);
		imageData.layerList[i].color[1] = getc(cmr);
		imageData.layerList[i].color[2] = getc(cmr);
		imageData.layerList[i].ori[0] = getc(cmr);
		imageData.layerList[i].ori[1] = getc(cmr);
		imageData.layerList[i].ori[2] = getc(cmr);
		imageData.layerList[i].display = getc(cmr);
		imageData.layerList[i].type = getc(cmr);
		imageData.layerList[i].ks = getc(cmr);
		imageData.layerList[i].maxVal = getc(cmr);
		imageData.layerList[i].minVal = getc(cmr);
		imageData.layerList[i].percent = getc(cmr) / 100.0;
		imageData.layerList[i].weight = getc(cmr) / 100.0;
	}
	int size = imageData.size.area();
	for (size_t d = 0; d < imageData.depth; d++) for (size_t i = 0; i < imageData.layerList.size(); i++)
	{
		if (imageData.layerList[i].type > 0) continue;
		fread(imageData.layerList[i].matList[d].data, 1, size, cmr);
	}
	fclose(cmr);
	return 0;
}

int CellMerge2::writeCMR(QString fileName)
{
	QTextCodec *code = QTextCodec::codecForName("GB2312");
	string fileNameConv = code->fromUnicode(fileName).data();
	FILE *cmr = fopen(fileNameConv.c_str(), "wb");
	uint ia = 0x43524D30;
	fwrite(&ia, 4, 1, cmr);
	ia = imageData.size.width;
	fwrite(&ia, 4, 1, cmr);
	ia = imageData.size.height;
	fwrite(&ia, 4, 1, cmr);
	ia = imageData.depth;
	fwrite(&ia, 4, 1, cmr);
	ia = imageData.nucId;
	fwrite(&ia, 4, 1, cmr);
	ia = imageData.layerList.size();
	fwrite(&ia, 4, 1, cmr);
	char name[0x40];
	sprintf(name, "%s", imageData.cellLayer.name.c_str());
	fwrite(&name, 0x40, 1, cmr);
	putc(imageData.cellLayer.color[0], cmr);
	putc(imageData.cellLayer.color[1], cmr);
	putc(imageData.cellLayer.color[2], cmr);
	putc(imageData.cellLayer.ori[0], cmr);
	putc(imageData.cellLayer.ori[1], cmr);
	putc(imageData.cellLayer.ori[2], cmr);
	putc(imageData.cellLayer.display, cmr);
	putc(imageData.cellLayer.type, cmr);
	putc(imageData.cellLayer.ks, cmr);
	putc(imageData.cellLayer.maxVal, cmr);
	putc(imageData.cellLayer.minVal, cmr);
	putc((uchar)round(imageData.cellLayer.percent * 100), cmr);
	putc((uchar)round(imageData.cellLayer.weight * 100), cmr);
	for (size_t i = 0; i < imageData.layerList.size(); i++)
	{
		sprintf(name, "%s", imageData.layerList[i].name.c_str());
		fwrite(&name, 0x40, 1, cmr);
		putc(imageData.layerList[i].color[0], cmr);
		putc(imageData.layerList[i].color[1], cmr);
		putc(imageData.layerList[i].color[2], cmr);
		putc(imageData.layerList[i].ori[0], cmr);
		putc(imageData.layerList[i].ori[1], cmr);
		putc(imageData.layerList[i].ori[2], cmr);
		putc(imageData.layerList[i].display, cmr);
		putc(imageData.layerList[i].type, cmr);
		putc(imageData.layerList[i].ks, cmr);
		putc(imageData.layerList[i].maxVal, cmr);
		putc(imageData.layerList[i].minVal, cmr);
		putc((uchar)round(imageData.layerList[i].percent * 100), cmr);
		putc((uchar)round(imageData.layerList[i].weight * 100), cmr);
	}
	int size = imageData.size.area();
	for (size_t d = 0; d < imageData.depth; d++) for (size_t i = 0; i < imageData.layerList.size(); i++)
	{
		if (imageData.layerList[i].type > 0) continue;
		fwrite(imageData.layerList[i].matList[d].data, 1, size, cmr);
	}
	fclose(cmr);
	return 0;
}

int CellMerge2::readLIF(QString fileName)
{
	FILE *lif;
	xmlDocPtr doc;
	xmlNodePtr curNode;
	uint ia, ib;

	QTextCodec *code = QTextCodec::codecForName("GB2312");
	string fileNameConv = code->fromUnicode(fileName).data();
	lif = fopen(fileNameConv.c_str(), "rb");
	if (lif == NULL) return -1;
	fseek(lif, 0, 2);
	size_t fileSize = ftell(lif);
	rewind(lif);
	if (fileSize < 0x20)
	{
		fclose(lif);
		return -1;
	}
	fread(&ia, 4, 1, lif);
	if (ia != 0x70)
	{
		fclose(lif);
		return -1;
	}
	fread(&ia, 4, 1, lif);
	getc(lif);
	fread(&ia, 4, 1, lif);
	ia *= 2;
	char *metadata = new char[ia];
	memset(metadata, 0, ia);
	fread(metadata, 1, ia, lif);
	doc = xmlReadMemory(metadata, ia, NULL, "UTF-16", XML_PARSE_RECOVER);
	delete[]metadata;
	if (doc == NULL) return -1;

	vector<vector<uint>> sectionInfo;
	while (ftell(lif) < fileSize)
	{
		fread(&ia, 4, 1, lif);
		fread(&ib, 4, 1, lif);
		getc(lif);
		fread(&ia, 4, 1, lif);
		fread(&ib, 4, 1, lif);
		getc(lif);
		fread(&ib, 4, 1, lif);
		fseek(lif, ib * 2, 1);
		if (ia > 0)
		{
			sectionInfo.push_back(vector<uint>(2));
			sectionInfo[sectionInfo.size() - 1][0] = (uint)ftell(lif);
			sectionInfo[sectionInfo.size() - 1][1] = ia;
		}
		fseek(lif, ia, 1);
	}
	fclose(lif);

	ItemSelector itemSelector(this);
	itemSelector.setWindowTitle(QStringLiteral("选择图像序列..."));
	vector<vector<string>> channelList;
	vector<vector<uint>> dimList;
	curNode = xmlDocGetRootElement(doc);
	if (curNode == NULL || xmlStrcmp(curNode->name, BAD_CAST"LMSDataContainerHeader"))
	{
		xmlFreeDoc(doc);
		return -1;
	}
	curNode = curNode->xmlChildrenNode;
	if (curNode == NULL || xmlStrcmp(curNode->name, BAD_CAST"Element"))
	{
		xmlFreeDoc(doc);
		return -1;
	}
	curNode = curNode->xmlChildrenNode;
	while (curNode != NULL)
	{
		if (!xmlStrcmp(curNode->name, BAD_CAST"Children")) break;
		curNode = curNode->next;
	}
	if (curNode == NULL || xmlStrcmp(curNode->name, BAD_CAST"Children"))
	{
		xmlFreeDoc(doc);
		return -1;
	}
	curNode = curNode->xmlChildrenNode;
	while (curNode != NULL)
	{
		if (!xmlStrcmp(curNode->name, BAD_CAST"Element"))
		{
			xmlChar *elementName = xmlGetProp(curNode, BAD_CAST"Name");
			xmlNodePtr propNode = curNode;
			propNode = propNode->xmlChildrenNode;
			if (propNode == NULL || xmlStrcmp(propNode->name, BAD_CAST"Data"))
			{
				curNode = curNode->next;
				continue;
			}
			propNode = propNode->xmlChildrenNode;
			if (propNode == NULL || xmlStrcmp(propNode->name, BAD_CAST"Image"))
			{
				curNode = curNode->next;
				continue;
			}
			propNode = propNode->xmlChildrenNode;
			while (propNode != NULL)
			{
				if (!xmlStrcmp(propNode->name, BAD_CAST"ImageDescription")) break;
				propNode = propNode->next;
			}
			if (propNode == NULL || xmlStrcmp(propNode->name, BAD_CAST"ImageDescription"))
			{
				curNode = curNode->next;
				continue;
			}
			itemSelector.itemBox->addItem(QString::fromUtf8((char*)elementName));
			channelList.push_back(vector<string>());
			dimList.push_back(vector<uint>());
			propNode = propNode->xmlChildrenNode;
			while (propNode != NULL)
			{
				if (!xmlStrcmp(propNode->name, BAD_CAST"Channels"))
				{
					xmlNodePtr attrNode = propNode->xmlChildrenNode;
					while (attrNode != NULL)
					{
						if (!xmlStrcmp(attrNode->name, BAD_CAST"ChannelDescription"))
						{
							channelList[channelList.size() - 1].push_back(string((char*)xmlGetProp(attrNode, BAD_CAST"LUTName")));
						}
						attrNode = attrNode->next;
					}
				}
				if (!xmlStrcmp(propNode->name, BAD_CAST"Dimensions"))
				{
					xmlNodePtr attrNode = propNode->xmlChildrenNode;
					while (attrNode != NULL)
					{
						if (!xmlStrcmp(attrNode->name, BAD_CAST"DimensionDescription"))
						{
							dimList[dimList.size() - 1].push_back(atoi((char*)xmlGetProp(attrNode, BAD_CAST"NumberOfElements")));
						}
						attrNode = attrNode->next;
					}
				}
				propNode = propNode->next;
			}
		}
		curNode = curNode->next;
	}
	xmlFreeDoc(doc);
	if (itemSelector.itemBox->count() != sectionInfo.size()) return -1;

	int result = itemSelector.exec();
	if (result == QDialog::Accepted)
	{
		imageData.release();
		int id = itemSelector.itemBox->currentIndex();
		string name = itemSelector.itemBox->currentText().toStdString() + "_";
		imageData.size = Size(dimList[id][0], dimList[id][1]);
		imageData.depth = dimList[id][2];
		for (size_t i = 0; i < channelList[id].size(); i++)
		{
			if (channelList[id][i] == "Blue") imageData.nucId = i;
			imageData.addLayer(name + channelList[id][i], imageData.name2col(channelList[id][i]));
		}
		int size = imageData.size.area();
		lif = fopen(fileNameConv.c_str(), "rb");
		fseek(lif, sectionInfo[id][0], 0);
		for (size_t d = 0; d < imageData.depth; d++)
		{
			for (size_t i = 0; i < imageData.layerList.size(); i++)
			{
				fread(imageData.layerList[i].matList[d].data, 1, size, lif);
				imageData.normalize(i, d);
			}
		}
		fclose(lif);
	}
	return 0;
}

int CellMerge2::readIMG(QString fileName)
{
	QTextCodec *code = QTextCodec::codecForName("GB2312");
	string fileNameConv = code->fromUnicode(fileName).data();
	Mat img = imread(fileNameConv);
	if (!img.data) return -1;
	vector<Mat> chs;
	split(img, chs);
	imageData.release();
	imageData.size = Size(img.cols, img.rows);
	imageData.depth = 1;
	imageData.addLayer("Blue", Vec3b(0, 0, 255));
	imageData.layerList[0].matList[0] = chs[0].clone();
	imageData.normalize(0, 0);
	imageData.addLayer("Green", Vec3b(0, 255, 0));
	imageData.layerList[1].matList[0] = chs[1].clone();
	imageData.normalize(1, 0);
	imageData.addLayer("Red", Vec3b(255, 0, 0));
	imageData.layerList[2].matList[0] = chs[2].clone();
	imageData.normalize(2, 0);
	imageData.nucId = 0;
	for (size_t i = 0; i < chs.size(); i++) chs[i].release();
	vector<Mat>().swap(chs);
	img.release();
	return 0;
}

void CellMerge2::calcHist()
{
	if (ui.selectBox->count() > 0 && ui.selectBox->currentIndex() < imageData.layerList.size() && ui.depthSlider->value() < imageData.depth)
	{
		if (imageData.imgMode == 1)
		{
			Mat img = Mat::zeros(imageData.size, CV_8UC3);
			int d = ui.depthSlider->value();
			for (int x = 0; x < imageData.size.height; x++) for (int y = 0; y < imageData.size.width; y++)
			{
				img.at<Vec3b>(x, y)[0] = imageData.layerList[2].matList[d].at<uchar>(x, y);
				img.at<Vec3b>(x, y)[1] = imageData.layerList[1].matList[d].at<uchar>(x, y);
				img.at<Vec3b>(x, y)[2] = imageData.layerList[0].matList[d].at<uchar>(x, y);
			}
			Mat temp;
			cvtColor(img, temp, COLOR_RGB2GRAY);
			ui.levelSetting->calcHist(&temp);
			img.release();
			temp.release();
		}
		else ui.levelSetting->calcHist(&imageData.layerList[ui.selectBox->currentIndex()].matList[ui.depthSlider->value()]);
	}
	else ui.levelSetting->initial();
}

Rect CellMerge2::roiFix(Rect term)
{
	if (term.width == 0) term.width = 1;
	if (term.height == 0) term.height = 1;
	if (term.width < imageData.size.width)
	{
		if (term.x < 0) term.x = 0; else if (term.x + term.width > imageData.size.width) term.x = imageData.size.width - term.width;
	}
	if (term.height < imageData.size.height)
	{
		if (term.y < 0) term.y = 0; else if (term.y + term.height > imageData.size.height) term.y = imageData.size.height - term.height;
	}
	if (term.width >= imageData.size.width)
	{
		term.x = 0;
		term.width = imageData.size.width;
	}
	if (term.height >= imageData.size.height)
	{
		term.y = 0;
		term.height = imageData.size.height;
	}
	return term;
}

void CellMerge2::refreshView(bool re)
{
	if (ui.mainForm->size().width() < 20 || ui.mainForm->size().height() < 20 || roi.area() == 0 || imageData.depth <= ui.depthSlider->value()) return;
	if (re) imageMat = widgetMode == 0 ? imageData.getImage(ui.depthSlider->value()) : imageData.getCell(ui.depthSlider->value());
	ui.roiEdit->setText(QString("%1*%2").arg(roi.width).arg(roi.height));
	roiMat = imageMat(roi);
	img = QImage((const uchar*)(roiMat.data), roiMat.cols, roiMat.rows, roiMat.step, QImage::Format_RGB888);
	ui.mainForm->setPixmap(QPixmap::fromImage(img).scaled(ui.mainForm->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void CellMerge2::resizeEvent(QResizeEvent *event)
{
	if (imageData.size.area() == 0) return;
	roi = roiFix(roi);
	calcHist();
	refreshView();
}

void CellMerge2::scrollFunc(int x, int y, int val)
{
	if (imageData.size.area() == 0) return;
	if (roi.width == 0) roi.width = 1;
	if (roi.height == 0) roi.height = 1;
	double scaleRate = 0.2;
	double scale = val > 0 ? 1.0 - scaleRate : 1.0 + scaleRate;
	double sizeLimit = 10.0 / scale;
	Size labelSize = Size(ui.mainForm->size().width(), ui.mainForm->size().height());
	Point pr(x, y);
	if ((double)roiMat.cols / roiMat.rows < (double)labelSize.width / labelSize.height)
	{
		pr.x = (int)round((pr.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pr.y = (int)round((double)pr.y * roiMat.rows / labelSize.height);
	}
	else if ((double)roiMat.cols / roiMat.rows > (double)labelSize.width / labelSize.height)
	{
		pr.y = (int)round((pr.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pr.x = (int)round((double)pr.x * roiMat.cols / labelSize.width);
	}
	else pr = Point((int)round((double)pr.x * roiMat.cols / labelSize.width), (int)round((double)pr.y * roiMat.rows / labelSize.height));
	if (pr.x < 0 || pr.x > roiMat.cols || pr.y < 0 || pr.y > roiMat.rows) return;
	double lr = (double)ui.mainForm->size().width() / ui.mainForm->size().height();
	double rr = (double)roi.width / roi.height;
	if (scale > 1 || lr == rr || (lr > rr && roi.height > sizeLimit) || (lr < rr && roi.width > sizeLimit))
	{
		double xs = scale;
		double ys = scale;
		if (scale < 1 && lr > rr) xs = min(1.0, ys * lr / rr);
		else if (scale < 1 && lr < rr) ys = min(1.0, xs * rr / lr);
		roi = Rect(roi.x + (1.0 - xs) * pr.x, roi.y + (1.0 - ys) * pr.y, roi.width * xs, roi.height * ys);
		rr = (double)roi.width / roi.height;
	}
	if (lr > rr)
	{
		scale = min(1.0 + scaleRate, lr / rr);
		roi.x = (int)round(roi.x + (1.0 - scale) * roi.width / 2.0);
		roi.width = (int)round(roi.width * scale);
	}
	else if (lr < rr)
	{
		scale = min(1.0 + scaleRate, rr / lr);
		roi.y = (int)round(roi.y + (1.0 - scale) * roi.height / 2.0);
		roi.height = (int)round(roi.height * scale);
	}
	roi = roiFix(roi);
	refreshView();
}

void CellMerge2::moveFunc(int xa, int ya, int xb, int yb)
{
	if (imageData.size.area() == 0 || (xa == xb && ya == yb)) return;
	Size labelSize = Size(ui.mainForm->size().width(), ui.mainForm->size().height());
	Point pa(xa, ya), pb(xb, yb);
	if ((double)roiMat.cols / roiMat.rows < (double)labelSize.width / labelSize.height)
	{
		pa.x = (int)round((pa.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pa.y = (int)round((double)pa.y * roiMat.rows / labelSize.height);
		pb.x = (int)round((pb.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pb.y = (int)round((double)pb.y * roiMat.rows / labelSize.height);
	}
	else if ((double)roiMat.cols / roiMat.rows > (double)labelSize.width / labelSize.height)
	{
		pa.y = (int)round((pa.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pa.x = (int)round((double)pa.x * roiMat.cols / labelSize.width);
		pb.y = (int)round((pb.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pb.x = (int)round((double)pb.x * roiMat.cols / labelSize.width);
	}
	else
	{
		pa = Point((int)round((double)pa.x * roiMat.cols / labelSize.width), (int)round((double)pa.y * roiMat.rows / labelSize.height));
		pb = Point((int)round((double)pb.x * roiMat.cols / labelSize.width), (int)round((double)pb.y * roiMat.rows / labelSize.height));
	}
	if (pa.x < 0 || pa.x > roiMat.cols || pa.y < 0 || pa.y > roiMat.rows || pb.x < 0 || pb.x > roiMat.cols || pb.y < 0 || pb.y > roiMat.rows) return;
	roi.x += pa.x - pb.x;
	roi.y += pa.y - pb.y;
	roi = roiFix(roi);
	refreshView();
}

void CellMerge2::drawRoiFunc(int xa, int ya, int xb, int yb)
{
	if (ui.mainForm->size().width() < 20 || ui.mainForm->size().height() < 20 || imageData.size.area() == 0 ||
		imageData.depth <= ui.depthSlider->value() || xa == xb || ya == yb) return;
	double sizeLimit = 10.0;
	Size labelSize = Size(ui.mainForm->size().width(), ui.mainForm->size().height());
	Point pa(min(xa, xb), min(ya, yb)), pb(max(xa, xb), max(ya, yb));
	double rr = (double)roiMat.cols / roiMat.rows;
	double lr = (double)labelSize.width / labelSize.height;
	if (rr < lr)
	{
		pa.x = (int)round((pa.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pa.y = (int)round((double)pa.y * roiMat.rows / labelSize.height);
		pb.x = (int)round((pb.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pb.y = (int)round((double)pb.y * roiMat.rows / labelSize.height);
	}
	else if (rr > lr)
	{
		pa.y = (int)round((pa.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pa.x = (int)round((double)pa.x * roiMat.cols / labelSize.width);
		pb.y = (int)round((pb.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pb.x = (int)round((double)pb.x * roiMat.cols / labelSize.width);
	}
	else
	{
		pa = Point((int)round((double)pa.x * roiMat.cols / labelSize.width), (int)round((double)pa.y * roiMat.rows / labelSize.height));
		pb = Point((int)round((double)pb.x * roiMat.cols / labelSize.width), (int)round((double)pb.y * roiMat.rows / labelSize.height));
	}
	if (pa.x < 0) pa.x = 0;
	if (pa.y < 0) pa.y = 0;
	if (pb.x > roiMat.cols) pb.x = roiMat.cols;
	if (pb.y > roiMat.rows) pb.y = roiMat.rows;
	if (pa.x == pb.x || pa.y == pb.y) return;
	ui.roiEdit->setText(QString("%1*%2").arg(pb.x - pa.x).arg(pb.y - pa.y));
	pa = Point(min(xa, xb), min(ya, yb));
	pb = Point(max(xa, xb), max(ya, yb));
	if (rr < lr)
	{
		int md = (int)round((labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0);
		pa.x = max(pa.x - md, 0);
		pb.x = min(pb.x - md, (int)round((double)roiMat.cols * labelSize.height / roiMat.rows));
	}
	if (rr > lr)
	{
		int md = (int)round((labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0);
		pa.y = max(pa.y - md, 0);
		pb.y = min(pb.y - md, (int)round((double)roiMat.rows * labelSize.width / roiMat.cols));
	}
	QPixmap pix = QPixmap::fromImage(img).scaled(ui.mainForm->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QPainter paint(&pix);
	if (imageData.imgMode == 1) paint.setPen(QPen(Qt::black, 4, Qt::SolidLine, Qt::RoundCap));
	else paint.setPen(QPen(Qt::white, 4, Qt::SolidLine, Qt::RoundCap));
	paint.drawRect(QRect(QPoint(pa.x, pa.y), QPoint(pb.x, pb.y)));
	ui.mainForm->setPixmap(pix);
}

void CellMerge2::selRoiFunc(int xa, int ya, int xb, int yb)
{
	if (imageData.size.area() == 0 || xa == xb || ya == yb) return;
	double sizeLimit = 10.0;
	Size labelSize = Size(ui.mainForm->size().width(), ui.mainForm->size().height());
	Point pa(min(xa, xb), min(ya, yb)), pb(max(xa, xb), max(ya, yb));
	if ((double)roiMat.cols / roiMat.rows < (double)labelSize.width / labelSize.height)
	{
		pa.x = (int)round((pa.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pa.y = (int)round((double)pa.y * roiMat.rows / labelSize.height);
		pb.x = (int)round((pb.x - (labelSize.width - roiMat.cols * labelSize.height / roiMat.rows) / 2.0) * roiMat.rows / labelSize.height);
		pb.y = (int)round((double)pb.y * roiMat.rows / labelSize.height);
	}
	else if ((double)roiMat.cols / roiMat.rows > (double)labelSize.width / labelSize.height)
	{
		pa.y = (int)round((pa.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pa.x = (int)round((double)pa.x * roiMat.cols / labelSize.width);
		pb.y = (int)round((pb.y - (labelSize.height - roiMat.rows * labelSize.width / roiMat.cols) / 2.0) * roiMat.cols / labelSize.width);
		pb.x = (int)round((double)pb.x * roiMat.cols / labelSize.width);
	}
	else
	{
		pa = Point((int)round((double)pa.x * roiMat.cols / labelSize.width), (int)round((double)pa.y * roiMat.rows / labelSize.height));
		pb = Point((int)round((double)pb.x * roiMat.cols / labelSize.width), (int)round((double)pb.y * roiMat.rows / labelSize.height));
	}
	if (pa.x < 0) pa.x = 0;
	if (pa.y < 0) pa.y = 0;
	if (pb.x > roiMat.cols) pb.x = roiMat.cols;
	if (pb.y > roiMat.rows) pb.y = roiMat.rows;
	if (pa.x != pb.x && pa.y != pb.y && pb.x - pa.x > sizeLimit && pb.y - pa.y > sizeLimit) roi = Rect(pa + Point(roi.x, roi.y), pb + Point(roi.x, roi.y));
	roi = roiFix(roi);
	refreshView();
}

void CellMerge2::selDepthFunc(int value)
{
	if (imageData.depth <= value) return;
	calcHist();
	refreshView(true);
}

void CellMerge2::setRoiFunc()
{
	if (imageData.size.area() == 0)
	{
		ui.nameEdit->setText(QString(""));
		return;
	}
	if (QString("%1*%2").arg(roi.width).arg(roi.height) == ui.roiEdit->text()) return;
	QRegularExpression re("^([0-9]+)[|,;\\*]([0-9]+)$");
	QRegularExpressionMatch match = re.match(ui.roiEdit->text());
	if (!match.hasMatch() || match.captured(1).toInt() == 0 || match.captured(2).toInt() == 0)
	{
		ui.roiEdit->setText(QString("%1*%2").arg(roi.width).arg(roi.height));
		return;
	}
	int w = match.captured(1).toInt();
	int h = match.captured(2).toInt();
	roi = roiFix(Rect((int)round(roi.x + roi.width / 2.0 - w / 2.0), (int)round(roi.y + roi.height / 2.0 - h / 2.0), w, h));
	refreshView();
}

void CellMerge2::resetRoiFunc()
{
	if (imageData.size.area() == 0)
	{
		ui.nameEdit->setText(QString(""));
		return;
	}
	roi = Rect(0, 0, imageData.size.width, imageData.size.height);
	refreshView();
}

void CellMerge2::setNameFunc()
{
	if (imageData.layerList.size() == 0)
	{
		ui.nameEdit->setText(QString(""));
		return;
	}
	if (widgetMode == 0)
	{
		int id = ui.selectBox->currentIndex();
		if (ui.nameEdit->text().size() >= 0x40) ui.nameEdit->setText(QString::fromStdString(imageData.layerList[id].name));
		else
		{
			imageData.layerList[id].name = ui.nameEdit->text().toStdString();
			ui.selectBox->setItemText(id, ui.nameEdit->text());
			ui.selectBox->setToolTip(ui.nameEdit->text());
		}
	}
	else
	{
		if (ui.nameEdit->text().size() >= 0x40) ui.nameEdit->setText(QString::fromStdString(imageData.cellLayer.name));
		else imageData.cellLayer.name = ui.nameEdit->text().toStdString();
	}
}

void CellMerge2::setColorFunc()
{
	if (imageData.layerList.size() == 0) return;
	if (widgetMode == 0)
	{
		if (imageData.imgMode == 1) return;
		int id = ui.selectBox->currentIndex();
		QColor color = QColorDialog::getColor(QColor(imageData.layerList[id].color[0], imageData.layerList[id].color[1], imageData.layerList[id].color[2]), 
			this, QStringLiteral("选择层颜色..."));
		if (!color.isValid()) return;
		imageData.layerList[id].color = Vec3b(color.red(), color.green(), color.blue());
		ui.layerColor->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()));
	}
	else
	{
		QColor color = QColorDialog::getColor(QColor(imageData.cellLayer.color[0], imageData.cellLayer.color[1], imageData.cellLayer.color[2]), 
			this, QStringLiteral("选择层颜色..."));
		if (!color.isValid()) return;
		imageData.cellLayer.color = Vec3b(color.red(), color.green(), color.blue());
		ui.layerColor->setStyleSheet(QString("background-color:rgb(%1,%2,%3)").arg(color.red()).arg(color.green()).arg(color.blue()));
	}
	refreshView(true);
}

void CellMerge2::setLimitValue(int minValue, double midValue, int maxValue)
{
	if (imageData.layerList.size() == 0) return;
	if (widgetMode == 0)
	{
		imageData.layerList[ui.selectBox->currentIndex()].minVal = minValue;
		imageData.layerList[ui.selectBox->currentIndex()].maxVal = maxValue;
		imageData.layerList[ui.selectBox->currentIndex()].percent = midValue;
	}
	else
	{
		imageData.layerList[imageData.nucId].minVal = minValue;
		imageData.layerList[imageData.nucId].maxVal = maxValue;
		imageData.layerList[imageData.nucId].percent = midValue;
	}
	calcHist();
	refreshView(true);
}

void CellMerge2::setWeightFunc(int value)
{
	if (imageData.layerList.size() == 0) return;
	if (widgetMode == 0) imageData.layerList[ui.selectBox->currentIndex()].weight = value / 100.0;
	else imageData.layerList[imageData.nucId].ks = value;
	refreshView(true);
}

void CellMerge2::displayFunc(int value)
{
	if (imageData.layerList.size() == 0) return;
	if (widgetMode == 0) imageData.layerList[ui.selectBox->currentIndex()].display = ui.displayBox->isChecked();
	else imageData.cellLayer.display = ui.displayBox->isChecked();
	refreshView(true);
}

void CellMerge2::selLayerFunc(int value)
{
	if (imageData.layerList.size() == 0 || value < 0 || value >= ui.selectBox->count()) return;
	ui.selectBox->setToolTip(QString::fromStdString(imageData.layerList[value].name));
	if (widgetMode == 0) setWidget(value);
	else
	{
		imageData.nucId = value;
		if (imageData.nucId != value)
		{
			imageData.layerList[imageData.nucId].maxVal = 255;
			imageData.layerList[imageData.nucId].minVal = 1;
			imageData.layerList[imageData.nucId].percent = 1.0;
			imageData.nucId = value;
		}
		setWidget();
	}
	calcHist();
	refreshView(true);
}

void CellMerge2::resetLayerFunc()
{
	if (imageData.layerList.size() == 0) return;
	if (widgetMode == 0)
	{
		imageData.resetLayer(ui.selectBox->currentIndex());
		setWidget(ui.selectBox->currentIndex());
	}
	else
	{
		imageData.resetCellLayer();
		setWidget();
	}
	calcHist();
	refreshView(true);
}

void CellMerge2::statShowFunc()
{
	if (imageData.layerList.size() == 0 || ui.selectBox->count() == 0) return;
	statForm.close();
	statForm.layerBox->clear();
	if (imageData.imgMode == 1)
	{
		statForm.layerBox->addItem(QString::fromStdString(imageData.layerList[0].name));
		statForm.layerBox->addItem(QString::fromStdString(imageData.layerList[1].name));
	}
	else for (size_t i = 0; i < imageData.layerList.size(); i++) statForm.layerBox->addItem(QString::fromStdString(imageData.layerList[i].name));
	statForm.layerBox->setCurrentIndex(imageData.nucId);
	statForm.layerBox->setToolTip(QString::fromStdString(imageData.layerList[imageData.nucId].name));
	statForm.show();
}

void CellMerge2::statHideFunc()
{
	statForm.close();
	if (!this->isVisible()) this->show();
}

void CellMerge2::statCalcFunc()
{
	if (imageData.layerList.size() == 0 || statForm.layerBox->count() == 0) return;
	disableWidget(true);
	Rect relRoi = Rect(0, 0, imageData.size.width, imageData.size.height);
	if (statForm.roiBox->isChecked())
	{
		relRoi = roi;
		if (relRoi.width > imageData.size.width)
		{
			relRoi.x = 0;
			relRoi.width = imageData.size.width;
		}
		if (relRoi.height > imageData.size.height)
		{
			relRoi.y = 0;
			relRoi.height = imageData.size.height;
		}
	}
	uint ref = statForm.layerBox->currentIndex();
	for (int i = 0; i < imageData.layerList.size(); i++)
	{
		if (i != ref && imageData.layerList[i].display == 0) imageData.layerList[i].resetCluster(); else imageData.calcCluster(i);
	}
	imageData.calcType(ref, statForm.modeBox->currentIndex());
	vector<TypeInfo> infoList = imageData.calcTypeInfo(ref, relRoi);
	statForm.clearForm();
	for (size_t i = 0; i < infoList.size(); i++)
	{
		statForm.tableData->setItem(i, 0, new QStandardItem(QString::fromStdString(infoList[i].type)));
		statForm.tableData->setItem(i, 1, new QStandardItem(QString::fromStdString(infoList[i].name)));
		statForm.tableData->setItem(i, 2, new QStandardItem(QString::number(infoList[i].degree)));
		statForm.tableData->setItem(i, 3, new QStandardItem(QString::number(infoList[i].count)));
		statForm.tableData->setItem(i, 4, new QStandardItem(QString::number(max(1.0, round(infoList[i].area)))));
		statForm.tableData->setItem(i, 5, new QStandardItem(QString("%1*%2").arg(relRoi.width).arg(relRoi.height)));
	}
	disableWidget(false);
}
