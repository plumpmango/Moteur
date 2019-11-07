/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Version_OpenGL;
    QAction *actionHello_tensorprod;
    QAction *actionHello_camera;
    QAction *actionHello_bspline;
    QAction *actionHello_nurbs;
    QAction *actionHello_envmap;
    QAction *actionHello_clear;
    QMenuBar *menuBar;
    QMenu *menuInfo;
    QMenu *menu_Demo;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(512, 512);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        action_Version_OpenGL = new QAction(MainWindow);
        action_Version_OpenGL->setObjectName(QStringLiteral("action_Version_OpenGL"));
        actionHello_tensorprod = new QAction(MainWindow);
        actionHello_tensorprod->setObjectName(QStringLiteral("actionHello_tensorprod"));
        actionHello_camera = new QAction(MainWindow);
        actionHello_camera->setObjectName(QStringLiteral("actionHello_camera"));
        actionHello_bspline = new QAction(MainWindow);
        actionHello_bspline->setObjectName(QStringLiteral("actionHello_bspline"));
        actionHello_nurbs = new QAction(MainWindow);
        actionHello_nurbs->setObjectName(QStringLiteral("actionHello_nurbs"));
        actionHello_envmap = new QAction(MainWindow);
        actionHello_envmap->setObjectName(QStringLiteral("actionHello_envmap"));
        actionHello_clear = new QAction(MainWindow);
        actionHello_clear->setObjectName(QStringLiteral("actionHello_clear"));
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 512, 22));
        menuInfo = new QMenu(menuBar);
        menuInfo->setObjectName(QStringLiteral("menuInfo"));
        menu_Demo = new QMenu(menuBar);
        menu_Demo->setObjectName(QStringLiteral("menu_Demo"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_Demo->menuAction());
        menuBar->addAction(menuInfo->menuAction());
        menuInfo->addAction(action_Version_OpenGL);
        menu_Demo->addAction(actionHello_clear);
        menu_Demo->addAction(actionHello_tensorprod);
        menu_Demo->addAction(actionHello_camera);
        menu_Demo->addAction(actionHello_bspline);
        menu_Demo->addAction(actionHello_nurbs);
        menu_Demo->addAction(actionHello_envmap);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Hello OpenGL ...", 0));
        action_Version_OpenGL->setText(QApplication::translate("MainWindow", "&Version OpenGL", 0));
        actionHello_tensorprod->setText(QApplication::translate("MainWindow", "Hello tensorprod ...", 0));
        actionHello_camera->setText(QApplication::translate("MainWindow", "Hello camera ...", 0));
        actionHello_bspline->setText(QApplication::translate("MainWindow", "Hello bspline ...", 0));
        actionHello_nurbs->setText(QApplication::translate("MainWindow", "Hello nurbs ...", 0));
        actionHello_envmap->setText(QApplication::translate("MainWindow", "Hello shadowmap ...", 0));
        actionHello_clear->setText(QApplication::translate("MainWindow", "Hello clear ...", 0));
        menuInfo->setTitle(QApplication::translate("MainWindow", "Info", 0));
        menu_Demo->setTitle(QApplication::translate("MainWindow", "&Demo", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
