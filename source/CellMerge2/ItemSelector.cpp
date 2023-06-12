#include "ItemSelector.h"

ItemSelector::ItemSelector(QWidget *parent)
	:QDialog(parent)
{
	setupUi(this);
	setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}
