#pragma once

#include <QtWidgets>
#include "ui_CellCounter.h"


class StatForm : public QWidget, public Ui::StatForm
{
	Q_OBJECT

public:
	// function
	StatForm(QWidget *parent = 0);
	void clearForm();

	// variant
	QStandardItemModel *tableData;

private slots:
	void statExportFunc();

};
