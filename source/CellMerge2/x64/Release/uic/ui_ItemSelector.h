/********************************************************************************
** Form generated from reading UI file 'ItemSelector.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ITEMSELECTOR_H
#define UI_ITEMSELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ItemSelector
{
public:
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QComboBox *itemBox;
    QSpacerItem *verticalSpacer;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ItemSelector)
    {
        if (ItemSelector->objectName().isEmpty())
            ItemSelector->setObjectName(QStringLiteral("ItemSelector"));
        ItemSelector->resize(336, 103);
        gridLayout_2 = new QGridLayout(ItemSelector);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        itemBox = new QComboBox(ItemSelector);
        itemBox->setObjectName(QStringLiteral("itemBox"));

        gridLayout->addWidget(itemBox, 0, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(ItemSelector);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Vertical);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 0, 1, 1, 1);


        retranslateUi(ItemSelector);
        QObject::connect(buttonBox, SIGNAL(accepted()), ItemSelector, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ItemSelector, SLOT(reject()));

        QMetaObject::connectSlotsByName(ItemSelector);
    } // setupUi

    void retranslateUi(QDialog *ItemSelector)
    {
        ItemSelector->setWindowTitle(QApplication::translate("ItemSelector", "\351\200\211\346\213\251\350\246\201\345\212\240\350\275\275\347\232\204\345\233\276\345\203\217", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ItemSelector: public Ui_ItemSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ITEMSELECTOR_H
