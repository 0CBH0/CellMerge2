/********************************************************************************
** Form generated from reading UI file 'CellCounter.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CELLCOUNTER_H
#define UI_CELLCOUNTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatForm
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QTableView *table;
    QFrame *line;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QComboBox *layerBox;
    QLabel *label_2;
    QComboBox *modeBox;
    QCheckBox *roiBox;
    QSpacerItem *verticalSpacer_2;
    QPushButton *calcButton;
    QPushButton *exportButton;
    QPushButton *hideButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *StatForm)
    {
        if (StatForm->objectName().isEmpty())
            StatForm->setObjectName(QStringLiteral("StatForm"));
        StatForm->resize(1024, 768);
        gridLayout = new QGridLayout(StatForm);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        table = new QTableView(StatForm);
        table->setObjectName(QStringLiteral("table"));
        table->setMinimumSize(QSize(800, 700));

        horizontalLayout->addWidget(table);

        line = new QFrame(StatForm);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(StatForm);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(label);

        layerBox = new QComboBox(StatForm);
        layerBox->setObjectName(QStringLiteral("layerBox"));
        layerBox->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(layerBox);

        label_2 = new QLabel(StatForm);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(label_2);

        modeBox = new QComboBox(StatForm);
        modeBox->setObjectName(QStringLiteral("modeBox"));
        modeBox->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(modeBox);

        roiBox = new QCheckBox(StatForm);
        roiBox->setObjectName(QStringLiteral("roiBox"));
        roiBox->setMinimumSize(QSize(150, 25));
        roiBox->setChecked(true);

        verticalLayout->addWidget(roiBox);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        calcButton = new QPushButton(StatForm);
        calcButton->setObjectName(QStringLiteral("calcButton"));
        calcButton->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(calcButton);

        exportButton = new QPushButton(StatForm);
        exportButton->setObjectName(QStringLiteral("exportButton"));
        exportButton->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(exportButton);

        hideButton = new QPushButton(StatForm);
        hideButton->setObjectName(QStringLiteral("hideButton"));
        hideButton->setMinimumSize(QSize(150, 25));

        verticalLayout->addWidget(hideButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        verticalLayout->setStretch(9, 5);

        horizontalLayout->addLayout(verticalLayout);

        horizontalLayout->setStretch(0, 40);
        horizontalLayout->setStretch(1, 1);
        horizontalLayout->setStretch(2, 4);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);


        retranslateUi(StatForm);

        QMetaObject::connectSlotsByName(StatForm);
    } // setupUi

    void retranslateUi(QWidget *StatForm)
    {
        StatForm->setWindowTitle(QApplication::translate("StatForm", "\345\214\272\345\237\237\347\273\237\350\256\241", Q_NULLPTR));
        label->setText(QApplication::translate("StatForm", "\345\217\202\350\200\203\351\200\232\351\201\223", Q_NULLPTR));
        label_2->setText(QApplication::translate("StatForm", "\350\256\241\347\256\227\346\250\241\345\274\217", Q_NULLPTR));
        modeBox->clear();
        modeBox->insertItems(0, QStringList()
         << QApplication::translate("StatForm", "\345\214\205\345\220\253", Q_NULLPTR)
         << QApplication::translate("StatForm", "2.0\345\200\215\345\215\212\345\276\204", Q_NULLPTR)
         << QApplication::translate("StatForm", "1.5\345\200\215\345\215\212\345\276\204", Q_NULLPTR)
         << QApplication::translate("StatForm", "1.0\345\200\215\345\215\212\345\276\204", Q_NULLPTR)
         << QApplication::translate("StatForm", "0.5\345\200\215\350\267\235\347\246\273", Q_NULLPTR)
        );
        roiBox->setText(QApplication::translate("StatForm", "\345\275\223\345\211\215\345\214\272\345\237\237", Q_NULLPTR));
        calcButton->setText(QApplication::translate("StatForm", "\350\256\241\347\256\227", Q_NULLPTR));
        exportButton->setText(QApplication::translate("StatForm", "\345\257\274\345\207\272", Q_NULLPTR));
        hideButton->setText(QApplication::translate("StatForm", "\350\277\224\345\233\236", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class StatForm: public Ui_StatForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CELLCOUNTER_H
