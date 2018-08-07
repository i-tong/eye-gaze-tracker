/********************************************************************************
** Form generated from reading UI file 'gazetrackgui.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAZETRACKGUI_H
#define UI_GAZETRACKGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include "GuiDisplayWidget.h"

QT_BEGIN_NAMESPACE

class Ui_gazeTrackGUIClass
{
public:
    QAction *temp;
    QAction *actionStart;
    QAction *actionRight_Video_Source;
    QAction *actionLeft_Video_Source;
    QAction *actionStart_Recording;
    QAction *actionStop_Recording;
    QAction *actionCapture_Settings;
    QAction *actionLeft_Right_Video_Source;
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    GazeDisplayWidget *widget;
    QSpacerItem *verticalSpacer;
    QMenuBar *menuBar;
    QMenu *menuSettings;
    QMenu *menuCapture;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QLabel *statusLabelFPS;
    QLabel *statusLabelCamera;
    QLabel *statusLabelCalibration;
    void setupUi(QMainWindow *gazeTrackGUIClass)
    {
        if (gazeTrackGUIClass->objectName().isEmpty())
            gazeTrackGUIClass->setObjectName(QStringLiteral("gazeTrackGUIClass"));
        gazeTrackGUIClass->resize(1338, 576);
        actionStart = new QAction(gazeTrackGUIClass);
        actionStart->setObjectName(QStringLiteral("actionStart"));
        actionRight_Video_Source = new QAction(gazeTrackGUIClass);
        actionRight_Video_Source->setObjectName(QStringLiteral("actionRight_Video_Source"));
        actionLeft_Video_Source = new QAction(gazeTrackGUIClass);
        actionLeft_Video_Source->setObjectName(QStringLiteral("actionLeft_Video_Source"));
        actionLeft_Right_Video_Source = new QAction(gazeTrackGUIClass);
        actionLeft_Right_Video_Source->setObjectName(QStringLiteral("actionLeft_Right_Video_Source"));
        actionLeft_Right_Video_Source->setCheckable(true);
        actionLeft_Right_Video_Source->setChecked(true);


        actionStart_Recording = new QAction(gazeTrackGUIClass);
        actionStart_Recording->setObjectName(QStringLiteral("actionStart_Recording"));
        actionStart_Recording->setCheckable(false);
        actionStop_Recording = new QAction(gazeTrackGUIClass);
        actionStop_Recording->setObjectName(QStringLiteral("actionStop_Recording"));
        actionStop_Recording->setCheckable(false);
        actionCapture_Settings = new QAction(gazeTrackGUIClass);
        actionCapture_Settings->setObjectName(QStringLiteral("actionCapture_Settings"));

        centralWidget = new QWidget(gazeTrackGUIClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 0, 1301, 521));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new GazeDisplayWidget(verticalLayoutWidget);
        widget->setObjectName(QStringLiteral("widget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(640, 480));

        verticalLayout->addWidget(widget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Preferred);

        verticalLayout->addItem(verticalSpacer);

        gazeTrackGUIClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(gazeTrackGUIClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1338, 21));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuCapture = new QMenu(menuBar);
        menuCapture->setObjectName(QStringLiteral("menuCapture"));
        gazeTrackGUIClass->setMenuBar(menuBar);
        //mainToolBar = new QToolBar(gazeTrackGUIClass);
        //mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        //mainToolBar->setMinimumSize(QSize(50, 0));
        //gazeTrackGUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(gazeTrackGUIClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusLabelFPS = new QLabel(gazeTrackGUIClass);
        statusLabelFPS->setObjectName(QStringLiteral("statusLabelFPS"));
        statusLabelCamera = new QLabel(gazeTrackGUIClass);
        statusLabelCamera->setObjectName(QStringLiteral("statusLabelCamera"));
        statusLabelCalibration = new QLabel(gazeTrackGUIClass);
        statusLabelCalibration->setObjectName(QStringLiteral("statusLabelCalibration"));

        statusLabelCamera->setMinimumWidth(100);
        statusLabelFPS->setMinimumWidth(200);
        statusLabelCalibration->setMinimumWidth(400);

        statusBar->addPermanentWidget(statusLabelCamera,1);
        statusBar->addPermanentWidget(statusLabelCalibration,0);
        statusBar->addPermanentWidget(statusLabelFPS,0);

        gazeTrackGUIClass->setStatusBar(statusBar);

        menuBar->addAction(menuSettings->menuAction());
        menuBar->addAction(menuCapture->menuAction());
        menuSettings->addAction(actionRight_Video_Source);
        menuSettings->addAction(actionLeft_Video_Source);
        menuSettings->addSeparator();
        menuSettings->addAction(actionLeft_Right_Video_Source);

        menuCapture->addAction(actionStart_Recording);
        menuCapture->addAction(actionStop_Recording);
        menuCapture->addAction(actionCapture_Settings);

        retranslateUi(gazeTrackGUIClass);

        QMetaObject::connectSlotsByName(gazeTrackGUIClass);
    } // setupUi

    void retranslateUi(QMainWindow *gazeTrackGUIClass)
    {
        gazeTrackGUIClass->setWindowTitle(QApplication::translate("gazeTrackGUIClass", "gazeTrackGUI", 0));
        actionStart->setText(QApplication::translate("gazeTrackGUIClass", "Start", 0));
#ifndef QT_NO_TOOLTIP
        actionStart->setToolTip(QApplication::translate("gazeTrackGUIClass", "<html><head/><body><p><img src=\":/gazeTrackGUI/Resources/play_icon.png\"/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        actionRight_Video_Source->setText(QApplication::translate("gazeTrackGUIClass", "Right Video Source", 0));
        actionLeft_Video_Source->setText(QApplication::translate("gazeTrackGUIClass", "Left Video Source", 0));
        actionStart_Recording->setText(QApplication::translate("gazeTrackGUIClass", "Start Recording", 0));
        actionStop_Recording->setText(QApplication::translate("gazeTrackGUIClass", "Stop Recording", 0));
        actionCapture_Settings->setText(QApplication::translate("gazeTrackGUIClass", "Capture Settings", 0));
        actionLeft_Right_Video_Source->setText(QApplication::translate("gazeTrackGUIClass", "Same left/right videos", 0));
        menuSettings->setTitle(QApplication::translate("gazeTrackGUIClass", "Settings", 0));
        menuCapture->setTitle(QApplication::translate("gazeTrackGUIClass", "Capture", 0));
    } // retranslateUi

};

namespace Ui {
    class gazeTrackGUIClass: public Ui_gazeTrackGUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAZETRACKGUI_H
/*
ui_gazetrackgui.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2017-2018 University of British Columbia

--- begin license - do not edit ---
    This file is a part of CGaze UI.
    
    CGaze UI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cgaze UI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cgaze UI.  If not, see <https://www.gnu.org/licenses/>.
--- end license ---
*/

