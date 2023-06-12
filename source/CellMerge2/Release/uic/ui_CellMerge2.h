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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CellMerge2Class
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CellMerge2Class)
    {
        if (CellMerge2Class->objectName().isEmpty())
            CellMerge2Class->setObjectName(QStringLiteral("CellMerge2Class"));
        CellMerge2Class->resize(600, 400);
        menuBar = new QMenuBar(CellMerge2Class);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CellMerge2Class->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CellMerge2Class);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CellMerge2Class->addToolBar(mainToolBar);
        centralWidget = new QWidget(CellMerge2Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CellMerge2Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CellMerge2Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CellMerge2Class->setStatusBar(statusBar);

        retranslateUi(CellMerge2Class);

        QMetaObject::connectSlotsByName(CellMerge2Class);
    } // setupUi

    void retranslateUi(QMainWindow *CellMerge2Class)
    {
        CellMerge2Class->setWindowTitle(QApplication::translate("CellMerge2Class", "CellMerge2", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CellMerge2Class: public Ui_CellMerge2Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CELLMERGE2_H
