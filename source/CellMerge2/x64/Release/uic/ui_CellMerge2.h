/********************************************************************************
** Form generated from reading UI file 'CellMerge2.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CELLMERGE2_H
#define UI_CELLMERGE2_H

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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "clickqlabel.h"
#include "levelsetting.h"

QT_BEGIN_NAMESPACE

class Ui_CellMerge2Class
{
public:
    QAction *actionHelp;
    QAction *actionFile;
    QAction *actionSave;
    QAction *actionImport;
    QAction *actionAbout;
    QAction *actionAdd_Layer;
    QAction *actionRemove_Layer;
    QAction *actionReset_Layer;
    QAction *actionExport;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    ClickQLabel *mainForm;
    QVBoxLayout *verticalLayout;
    QLabel *label_3;
    QLineEdit *dpMinBox;
    QSlider *depthSlider;
    QLineEdit *dpMaxBox;
    QFrame *line;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *roiEdit;
    QPushButton *roiButton;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *nameEdit;
    ClickQLabel *layerColor;
    LevelSetting *levelSetting;
    QHBoxLayout *horizontalLayout_4;
    QLabel *setLabel;
    QSlider *weightSlider;
    QCheckBox *displayBox;
    QPushButton *resetButton;
    QLabel *infoLabel;
    QComboBox *selectBox;
    QSpacerItem *verticalSpacer;
    QPushButton *editButton;
    QPushButton *statButton;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CellMerge2Class)
    {
        if (CellMerge2Class->objectName().isEmpty())
            CellMerge2Class->setObjectName(QStringLiteral("CellMerge2Class"));
        CellMerge2Class->resize(1024, 768);
        CellMerge2Class->setMouseTracking(true);
        CellMerge2Class->setFocusPolicy(Qt::ClickFocus);
        actionHelp = new QAction(CellMerge2Class);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionFile = new QAction(CellMerge2Class);
        actionFile->setObjectName(QStringLiteral("actionFile"));
        actionSave = new QAction(CellMerge2Class);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionImport = new QAction(CellMerge2Class);
        actionImport->setObjectName(QStringLiteral("actionImport"));
        actionAbout = new QAction(CellMerge2Class);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAdd_Layer = new QAction(CellMerge2Class);
        actionAdd_Layer->setObjectName(QStringLiteral("actionAdd_Layer"));
        actionRemove_Layer = new QAction(CellMerge2Class);
        actionRemove_Layer->setObjectName(QStringLiteral("actionRemove_Layer"));
        actionReset_Layer = new QAction(CellMerge2Class);
        actionReset_Layer->setObjectName(QStringLiteral("actionReset_Layer"));
        actionExport = new QAction(CellMerge2Class);
        actionExport->setObjectName(QStringLiteral("actionExport"));
        centralWidget = new QWidget(CellMerge2Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMouseTracking(true);
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        mainForm = new ClickQLabel(centralWidget);
        mainForm->setObjectName(QStringLiteral("mainForm"));
        mainForm->setMinimumSize(QSize(700, 600));
        mainForm->setMouseTracking(true);
        mainForm->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(mainForm);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(28, 20));
        label_3->setMaximumSize(QSize(28, 20));
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label_3);

        dpMinBox = new QLineEdit(centralWidget);
        dpMinBox->setObjectName(QStringLiteral("dpMinBox"));
        dpMinBox->setMinimumSize(QSize(28, 20));
        dpMinBox->setMaximumSize(QSize(28, 20));
        dpMinBox->setMaxLength(32767);
        dpMinBox->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(dpMinBox);

        depthSlider = new QSlider(centralWidget);
        depthSlider->setObjectName(QStringLiteral("depthSlider"));
        depthSlider->setMinimumSize(QSize(28, 0));
        depthSlider->setMaximumSize(QSize(28, 16777215));
        depthSlider->setPageStep(1);
        depthSlider->setValue(0);
        depthSlider->setOrientation(Qt::Vertical);
        depthSlider->setInvertedAppearance(true);
        depthSlider->setInvertedControls(true);

        verticalLayout->addWidget(depthSlider);

        dpMaxBox = new QLineEdit(centralWidget);
        dpMaxBox->setObjectName(QStringLiteral("dpMaxBox"));
        dpMaxBox->setMinimumSize(QSize(28, 20));
        dpMaxBox->setMaximumSize(QSize(28, 20));
        dpMaxBox->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(dpMaxBox);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 1);
        verticalLayout->setStretch(2, 500);
        verticalLayout->setStretch(3, 1);

        horizontalLayout->addLayout(verticalLayout);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout->addWidget(line);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(label);

        roiEdit = new QLineEdit(centralWidget);
        roiEdit->setObjectName(QStringLiteral("roiEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(roiEdit->sizePolicy().hasHeightForWidth());
        roiEdit->setSizePolicy(sizePolicy);
        roiEdit->setMinimumSize(QSize(0, 25));

        horizontalLayout_2->addWidget(roiEdit);

        roiButton = new QPushButton(centralWidget);
        roiButton->setObjectName(QStringLiteral("roiButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(roiButton->sizePolicy().hasHeightForWidth());
        roiButton->setSizePolicy(sizePolicy1);
        roiButton->setMinimumSize(QSize(50, 25));
        roiButton->setMaximumSize(QSize(50, 16777215));

        horizontalLayout_2->addWidget(roiButton);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 200);
        horizontalLayout_2->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        nameEdit = new QLineEdit(centralWidget);
        nameEdit->setObjectName(QStringLiteral("nameEdit"));
        nameEdit->setMinimumSize(QSize(0, 25));
        nameEdit->setMaximumSize(QSize(16777215, 16777215));
        nameEdit->setFocusPolicy(Qt::ClickFocus);
        nameEdit->setMaxLength(60);

        horizontalLayout_3->addWidget(nameEdit);

        layerColor = new ClickQLabel(centralWidget);
        layerColor->setObjectName(QStringLiteral("layerColor"));
        sizePolicy1.setHeightForWidth(layerColor->sizePolicy().hasHeightForWidth());
        layerColor->setSizePolicy(sizePolicy1);
        layerColor->setMinimumSize(QSize(50, 25));
        layerColor->setMaximumSize(QSize(50, 16777215));
        layerColor->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(layerColor);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 200);
        horizontalLayout_3->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);

        levelSetting = new LevelSetting(centralWidget);
        levelSetting->setObjectName(QStringLiteral("levelSetting"));
        levelSetting->setMinimumSize(QSize(220, 110));
        levelSetting->setMaximumSize(QSize(16777215, 16777215));

        verticalLayout_2->addWidget(levelSetting);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        setLabel = new QLabel(centralWidget);
        setLabel->setObjectName(QStringLiteral("setLabel"));
        setLabel->setMinimumSize(QSize(0, 25));

        horizontalLayout_4->addWidget(setLabel);

        weightSlider = new QSlider(centralWidget);
        weightSlider->setObjectName(QStringLiteral("weightSlider"));
        weightSlider->setMinimumSize(QSize(0, 25));
        weightSlider->setMaximum(100);
        weightSlider->setPageStep(2);
        weightSlider->setValue(100);
        weightSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(weightSlider);

        displayBox = new QCheckBox(centralWidget);
        displayBox->setObjectName(QStringLiteral("displayBox"));
        sizePolicy1.setHeightForWidth(displayBox->sizePolicy().hasHeightForWidth());
        displayBox->setSizePolicy(sizePolicy1);
        displayBox->setMinimumSize(QSize(0, 25));
        displayBox->setChecked(false);
        displayBox->setTristate(true);

        horizontalLayout_4->addWidget(displayBox);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 200);
        horizontalLayout_4->setStretch(2, 1);

        verticalLayout_2->addLayout(horizontalLayout_4);

        resetButton = new QPushButton(centralWidget);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        resetButton->setMinimumSize(QSize(220, 25));

        verticalLayout_2->addWidget(resetButton);

        infoLabel = new QLabel(centralWidget);
        infoLabel->setObjectName(QStringLiteral("infoLabel"));
        infoLabel->setMinimumSize(QSize(0, 25));

        verticalLayout_2->addWidget(infoLabel);

        selectBox = new QComboBox(centralWidget);
        selectBox->setObjectName(QStringLiteral("selectBox"));
        selectBox->setMinimumSize(QSize(220, 25));

        verticalLayout_2->addWidget(selectBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        editButton = new QPushButton(centralWidget);
        editButton->setObjectName(QStringLiteral("editButton"));
        editButton->setMinimumSize(QSize(220, 25));

        verticalLayout_2->addWidget(editButton);

        statButton = new QPushButton(centralWidget);
        statButton->setObjectName(QStringLiteral("statButton"));
        statButton->setMinimumSize(QSize(220, 25));

        verticalLayout_2->addWidget(statButton);

        verticalLayout_2->setStretch(0, 1);
        verticalLayout_2->setStretch(1, 1);
        verticalLayout_2->setStretch(2, 5);
        verticalLayout_2->setStretch(3, 1);
        verticalLayout_2->setStretch(4, 1);
        verticalLayout_2->setStretch(5, 1);
        verticalLayout_2->setStretch(6, 1);
        verticalLayout_2->setStretch(7, 12);
        verticalLayout_2->setStretch(8, 1);
        verticalLayout_2->setStretch(9, 1);

        horizontalLayout->addLayout(verticalLayout_2);

        horizontalLayout->setStretch(0, 200);
        horizontalLayout->setStretch(2, 1);
        horizontalLayout->setStretch(3, 2);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        CellMerge2Class->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(CellMerge2Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 26));
        CellMerge2Class->setMenuBar(menuBar);
        statusBar = new QStatusBar(CellMerge2Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CellMerge2Class->setStatusBar(statusBar);

        retranslateUi(CellMerge2Class);

        QMetaObject::connectSlotsByName(CellMerge2Class);
    } // setupUi

    void retranslateUi(QMainWindow *CellMerge2Class)
    {
        CellMerge2Class->setWindowTitle(QApplication::translate("CellMerge2Class", "CellMerge2", Q_NULLPTR));
        actionHelp->setText(QApplication::translate("CellMerge2Class", "\345\205\263\344\272\216", Q_NULLPTR));
        actionFile->setText(QApplication::translate("CellMerge2Class", "\346\211\223\345\274\200", Q_NULLPTR));
        actionSave->setText(QApplication::translate("CellMerge2Class", "\344\277\235\345\255\230", Q_NULLPTR));
        actionImport->setText(QApplication::translate("CellMerge2Class", "\345\257\274\345\205\245", Q_NULLPTR));
        actionAbout->setText(QApplication::translate("CellMerge2Class", "\345\205\263\344\272\216", Q_NULLPTR));
        actionAdd_Layer->setText(QApplication::translate("CellMerge2Class", "\346\267\273\345\212\240\345\261\202", Q_NULLPTR));
        actionRemove_Layer->setText(QApplication::translate("CellMerge2Class", "\345\210\240\351\231\244\345\261\202", Q_NULLPTR));
        actionReset_Layer->setText(QApplication::translate("CellMerge2Class", "\350\277\230\345\216\237\345\261\202", Q_NULLPTR));
        actionExport->setText(QApplication::translate("CellMerge2Class", "\345\257\274\345\207\272", Q_NULLPTR));
        mainForm->setText(QString());
        label_3->setText(QApplication::translate("CellMerge2Class", "\345\261\202", Q_NULLPTR));
        dpMinBox->setInputMask(QString());
        dpMinBox->setText(QApplication::translate("CellMerge2Class", "0", Q_NULLPTR));
        dpMaxBox->setInputMask(QString());
        dpMaxBox->setText(QApplication::translate("CellMerge2Class", "0", Q_NULLPTR));
        label->setText(QApplication::translate("CellMerge2Class", "\345\214\272\345\237\237", Q_NULLPTR));
        roiButton->setText(QApplication::translate("CellMerge2Class", "\345\244\215\344\275\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("CellMerge2Class", "\351\200\232\351\201\223", Q_NULLPTR));
        layerColor->setText(QString());
        setLabel->setText(QApplication::translate("CellMerge2Class", "\346\235\203\351\207\215", Q_NULLPTR));
        displayBox->setText(QApplication::translate("CellMerge2Class", "\346\230\276\347\244\272", Q_NULLPTR));
        resetButton->setText(QApplication::translate("CellMerge2Class", "\345\217\202\346\225\260\350\277\230\345\216\237", Q_NULLPTR));
        infoLabel->setText(QApplication::translate("CellMerge2Class", "\345\275\223\345\211\215\351\200\232\351\201\223", Q_NULLPTR));
        editButton->setText(QApplication::translate("CellMerge2Class", "\346\230\276\347\244\272\345\214\272\345\237\237", Q_NULLPTR));
        statButton->setText(QApplication::translate("CellMerge2Class", "\347\273\237\350\256\241\345\214\272\345\237\237", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CellMerge2Class: public Ui_CellMerge2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CELLMERGE2_H
