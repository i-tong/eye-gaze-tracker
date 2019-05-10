/*
GuiToolbar.cpp

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2018 University of British Columbia

--- begin license - do not edit ---

    This file is part of CGaze UI. 
   
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

#include "GuiToolbar.h"

GazeToolbar::GazeToolbar(void) {
	initLayout();
}

GazeToolbar::~GazeToolbar(void) {
}

void GazeToolbar::initLayout() {
	// Initialize widgets
	pushButton_Start = new QPushButton;
	pushButton_Calibrate = new QPushButton;
    pushButton_ShowGaze = new QPushButton;
    comboBox_trackerInputBoth = new QComboBox;
    comboBox_calibNumber = new QComboBox;
	comboBox_calibMonitor = new  QComboBox;
    comboBox_calibEye = new QComboBox;
    pushButton_Log = new QPushButton;
    pushButton_ManualGlints= new QPushButton;
    pushButton_Head = new QPushButton;

    // Set flags
    _start_button_on = false;
    _calibrate_button_on = false;
    _log_button_on = false;
    _manual_glints_open= false;

    // Configure widgets
    QStringList iconFileList;
    iconFileList << ":/Icons/Resources/play_icon_light.png";
    iconFileList << ":/Icons/Resources/calib_icon_light.png";
    iconFileList << ":/Icons/Resources/log_icon_grey.png";
    std::vector<QPushButton*> buttonList;
    buttonList.push_back(pushButton_Start);
    buttonList.push_back(pushButton_Calibrate);
    buttonList.push_back(pushButton_Log);

    for (int i = 0 ; i < iconFileList.size() ; i++ ) {
        QPixmap pix(iconFileList.at(i));
        QIcon icon(pix);
        buttonList.at(i)->setIcon(icon);
        buttonList.at(i)->setIconSize(pix.size()*0.5);
    }


    pushButton_ManualGlints->setText("Glints");
    pushButton_ManualGlints->setMinimumSize(60,60);
    pushButton_ManualGlints->setMaximumSize(60,60);
    pushButton_Head->setText("HEAD");
    pushButton_Head->setMinimumSize(60,60);
    pushButton_Head->setMaximumSize(60,60);
    pushButton_ShowGaze->setText("Show\nGaze");
    pushButton_ShowGaze->setMinimumSize(60,60);
    pushButton_ShowGaze->setMaximumSize(60,60);

	//  Set tracker input options
	std::vector<int> camIndList;

    comboBox_trackerInputBoth->addItem("None");
    comboBox_trackerInputBoth->addItem("Video");

    for ( int i = 0 ; i < 10/*camIndList.size()*/ ; i++ ) {
        comboBox_trackerInputBoth->addItem(QString("Camera %1").arg(i));
	}

	comboBox_calibNumber->addItem("5 point");
	comboBox_calibNumber->addItem("9 point");
	comboBox_calibNumber->addItem("16 point");
    comboBox_calibEye->addItem("Both Eyes");
    comboBox_calibEye->addItem("Right Eye");
    comboBox_calibEye->addItem("Left Eye");

	QDesktopWidget* deskWin = QApplication::desktop();
	int numScreens = deskWin->screenCount();
    for (int j = 0 ; j < numScreens ; j++ ) {
		comboBox_calibMonitor->addItem(QString("Monitor %1").arg(j));
    }
    comboBox_calibMonitor->setCurrentIndex(numScreens-1);
	// Layout management
	QGridLayout* layout1 = new QGridLayout;

    QLabel* label_leftEye =  new QLabel("Input: ");
    layout1->addWidget(label_leftEye,0,0);
    layout1->addWidget(comboBox_trackerInputBoth,0,1);

	QWidget* sourceSection = new QWidget;
	sourceSection->setLayout(layout1);

    QGridLayout* layout2 = new QGridLayout;

    layout2->addWidget(comboBox_calibNumber,0,0);
    layout2->addWidget(comboBox_calibEye,1,0);
    layout2->addWidget(comboBox_calibMonitor,0,1);
    QWidget* calib2Dsection = new QWidget;
    calib2Dsection->setLayout(layout2);

	// Add widgets to toolbar
	this->addWidget(pushButton_Start);
	this->addSeparator();
	this->addWidget(sourceSection);
	this->addSeparator();
	this->addWidget(pushButton_Calibrate);
    this->addWidget(calib2Dsection);
    this->addWidget(pushButton_ShowGaze);
	this->addSeparator();

    this->addWidget(pushButton_ManualGlints);
    this->addWidget(pushButton_Head);
    this->addSeparator();
    this->addWidget(pushButton_Log);
    this->addSeparator();
	// Signals and slots
	connect(pushButton_Start,SIGNAL(clicked()),this,SLOT(onButtonClickStart()));
    connect(pushButton_Log, SIGNAL(clicked()),this,SLOT(onButtonClickLog()));
    connect(pushButton_ShowGaze,SIGNAL(clicked()), this, SLOT(onButtonClick_ShowGaze()));
}


void GazeToolbar::onButtonClickLog(){


    if (_log_button_on == true) {
        QPixmap pix(":/Icons/Resources/log_icon_grey");
        QIcon icon(pix);
        pushButton_Log->setIcon(icon);
        pushButton_Log->setIconSize(pix.size()*0.5);

    } else {
        QPixmap pix(":/Icons/Resources/log_icon_red");
        QIcon icon(pix);
        pushButton_Log->setIcon(icon);
        pushButton_Log->setIconSize(pix.size()*0.5);

    }
    _log_button_on = !_log_button_on;
}

void GazeToolbar::onButtonClickStart() {

    if (_start_button_on == true) {
        QPixmap pix(":/Icons/Resources/play_icon_light.png");// /Resources/play_icon_light.png
		QIcon icon(pix);
		pushButton_Start->setIcon(icon);
		pushButton_Start->setIconSize(pix.size()*0.5);
	} else {
        QPixmap pix(":/Icons/Resources/stop_icon_light.png");// /Resources/stop_icon_light.png
		QIcon icon(pix);
		pushButton_Start->setIcon(icon);
		pushButton_Start->setIconSize(pix.size()*0.5);
	}
    _start_button_on = !_start_button_on;

}

void GazeToolbar::onButtonClick_ShowGaze() {

}

void GazeToolbar::setStatus_pushButton_manualGlint(bool status)
{
    if (status == true) {
        pushButton_ManualGlints->setText("STOP");
        _manual_glints_open = true;
    } else {
        pushButton_ManualGlints->setText("Glint");
        _manual_glints_open= false;
    }
}

bool GazeToolbar::getStatus_pushButton_manualGlint()
{
    return _manual_glints_open;
}
