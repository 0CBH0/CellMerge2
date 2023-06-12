#pragma once

#include <QDialog>
#include "ui_ItemSelector.h"


class ItemSelector : public QDialog, public Ui::ItemSelector
{
	Q_OBJECT

public:
	ItemSelector(QWidget *parent = 0);

private slots:

};
