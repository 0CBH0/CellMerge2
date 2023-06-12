#include "CellCounter.h"

StatForm::StatForm(QWidget *parent)
	:QWidget(parent)
{
	setupUi(this);

	table->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
	table->verticalHeader()->setDefaultAlignment(Qt::AlignVCenter);
	table->setSortingEnabled(true);

	tableData = new QStandardItemModel;
	tableData->setColumnCount(6);
	tableData->setHeaderData(0, Qt::Horizontal, "ID");
	tableData->setHeaderData(1, Qt::Horizontal, "Type");
	tableData->setHeaderData(2, Qt::Horizontal, "Degree");
	tableData->setHeaderData(3, Qt::Horizontal, "Count");
	tableData->setHeaderData(4, Qt::Horizontal, "Area");
	tableData->setHeaderData(5, Qt::Horizontal, "Region");
	table->setModel(tableData);

	connect(exportButton, SIGNAL(clicked()), this, SLOT(statExportFunc()));
}

void StatForm::clearForm()
{
	tableData->removeRows(0, tableData->rowCount());
}

void StatForm::statExportFunc()
{
	if (tableData->rowCount() == 0) return;
	QString fileName = QFileDialog::getSaveFileName(this, QStringLiteral("导出当前结果为..."), ".", QStringLiteral("表格文件 (*.csv)"));
	if (fileName == "") return;
	QFile file(fileName);
	QAbstractItemModel *model = table->model();
	if (!file.open(QFile::WriteOnly | QFile::Truncate)) return;
	QTextStream data(&file);
	QStringList strList;
	for (int i = 0; i < model->columnCount(); i++)
	{
		QString item = model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString();
		if (item.indexOf(",") >= 0) item = "\"" + item + "\"";
		strList.append(item);
	}
	data << strList.join(",") << "\n";
	for (int i = 0; i < model->rowCount(); i++)
	{
		strList.clear();
		for (int j = 0; j < model->columnCount(); j++)
		{
			QString item = model->data(model->index(i, j)).toString();
			if (item.indexOf(",") >= 0) item = "\"" + item + "\"";
			strList.append(item);
		}
		data << strList.join(",") + "\n";
	}
	file.close();
}
