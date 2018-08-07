/*
GuiToolbar.cpp

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
    //comboBox_trackerInputRight = new QComboBox;
    //comboBox_trackerInputLeft = new QComboBox;
    comboBox_trackerInputBoth = new QComboBox;
    comboBox_calibNumber = new QComboBox;
	comboBox_calibMonitor = new  QComboBox;
    comboBox_calibEye = new QComboBox;
    pushButton_Log = new QPushButton;
    //pushButton_Calibrate3D = new QPushButton;
    pushButton_streamGazePosition = new QPushButton;
    pushButton_ManualGlints = new QPushButton;
    pushButton_Head = new QPushButton;
    //pushButton_HeadRepeat = new QPushButton;
	startButtonOn = false;
	calibrateButtonOn = false;
    logButtonOn = false;
    streamGazeButtonOn = false;
    manualGlints = false;
    // Configure widgets
    QStringList iconFileList;
    iconFileList << ":/Icons/Resources/play_icon_light.png";
    iconFileList << ":/Icons/Resources/calib_icon_light.png";
    //iconFileList << ":/Icons/Resources/calib_icon_3D.png";
    iconFileList << ":/Icons/Resources/log_icon_grey.png";
    std::vector<QPushButton*> buttonList;
    buttonList.push_back(pushButton_Start);
    buttonList.push_back(pushButton_Calibrate);
    //buttonList.push_back(pushButton_Calibrate3D);
    buttonList.push_back(pushButton_Log);

    for (int i = 0 ; i < iconFileList.size() ; i++ ) {
        QPixmap pix(iconFileList.at(i));
        QIcon icon(pix);
        buttonList.at(i)->setIcon(icon);
        buttonList.at(i)->setIconSize(pix.size()*0.5);
    }


    pushButton_streamGazePosition->setText("ROS");
    pushButton_streamGazePosition->setMinimumSize(60,60);
    pushButton_streamGazePosition->setMaximumSize(60,60);
    pushButton_ManualGlints->setText("Glints");
    pushButton_ManualGlints->setMinimumSize(60,60);
    pushButton_ManualGlints->setMaximumSize(60,60);
    pushButton_Head->setText("HEAD");
    pushButton_Head->setMinimumSize(60,60);
    pushButton_Head->setMaximumSize(60,60);
    pushButton_ShowGaze->setText("Show\nGaze");
    pushButton_ShowGaze->setMinimumSize(60,60);
    pushButton_ShowGaze->setMaximumSize(60,60);
    //pushButton_HeadRepeat->setText("Repeat");
    //pushButton_HeadRepeat->setMinimumSize(60,60);
   // pushButton_HeadRepeat->setMaximumSize(60,60);

	//  Set tracker input options
	std::vector<int> camIndList;
    //detectCam(0,10,camIndList);
    /*comboBox_trackerInputRight->addItem("None");
	comboBox_trackerInputRight->addItem("Video");
	comboBox_trackerInputRight->addItem("Image");
	comboBox_trackerInputLeft->addItem("None");
	comboBox_trackerInputLeft->addItem("Video");
    comboBox_trackerInputLeft->addItem("Image");*/
    comboBox_trackerInputBoth->addItem("None");
    comboBox_trackerInputBoth->addItem("Video");
    //comboBox_trackerInputBoth->addItem("Image");
    for ( int i = 0 ; i < 10/*camIndList.size()*/ ; i++ ) {
        /*comboBox_trackerInputRight->addItem(QString("Camera %1").arg(i));
        comboBox_trackerInputLeft->addItem(QString("Camera %1").arg(i));*/
        comboBox_trackerInputBoth->addItem(QString("Camera %1").arg(i));
	}

	comboBox_calibNumber->addItem("5 point");
	comboBox_calibNumber->addItem("9 point");
	comboBox_calibNumber->addItem("16 point");
    comboBox_calibEye->addItem("Both Eyes");
    comboBox_calibEye->addItem("Right Eye");
    comboBox_calibEye->addItem("Left Eye");
//	comboBox_trackerType->addItem("da Vinci");
//	comboBox_trackerType->addItem("Head-mounted");

	QDesktopWidget* deskWin = QApplication::desktop();
	int numScreens = deskWin->screenCount();
    for (int j = 0 ; j < numScreens ; j++ ) {
		comboBox_calibMonitor->addItem(QString("Monitor %1").arg(j));
    }
    comboBox_calibMonitor->addItem(QString("DeckLink"));
    comboBox_calibMonitor->setCurrentIndex(numScreens-1);
	// Layout management
	QGridLayout* layout1 = new QGridLayout;
/*
	QLabel* label_rightEye = new QLabel("Right:");
	layout1->addWidget(label_rightEye,0,0);
	layout1->addWidget(comboBox_trackerInputRight,0,1);

	QLabel* label_leftEye =  new QLabel("Left: ");
	layout1->addWidget(label_leftEye,1,0);
	layout1->addWidget(comboBox_trackerInputLeft,1,1);
*/
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
    //this->addWidget(pushButton_Calibrate3D);


   // this->addWidget(pushButton_streamGazePosition);
    this->addWidget(pushButton_ManualGlints);
    this->addWidget(pushButton_Head);
    //this->addWidget(pushButton_HeadRepeat);
    this->addSeparator();
    this->addWidget(pushButton_Log);
    this->addSeparator();
	// Signals and slots
	connect(pushButton_Start,SIGNAL(clicked()),this,SLOT(onButtonClickStart()));
    connect(pushButton_Log, SIGNAL(clicked()),this,SLOT(onButtonClickLog()));
    connect(pushButton_ShowGaze,SIGNAL(clicked()), this, SLOT(onButtonClick_ShowGaze()));
   // connect(pushButton_Calibrate3D,SIGNAL(clicked()),this,SLOT(onButtonClick_Calibrate3D();
   // connect(pushButton_streamGazePosition, SIGNAL(clicked()),this,SLOT(onButtonClick_StreamGaze()));


}

void GazeToolbar::onButtonClickCalibNumber() {


}


void GazeToolbar::onButtonClickLog(){


    if (logButtonOn == true) {
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
    logButtonOn = !logButtonOn;
}

void GazeToolbar::onButtonClickStart() {

	if (startButtonOn == true) {
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
	startButtonOn = !startButtonOn;

}

void GazeToolbar::onComboBoxSelect_trackerType() {

}

/*
void GazeToolbar::onButtonClick_Calibrate3D() {

    qDebug() << "Clicked 3D Calibrate button!";

}*/

void GazeToolbar::onButtonClick_StreamGaze() {
    if (streamGazeButtonOn == true) {
        pushButton_streamGazePosition->setText("ROS");
        streamGazeButtonOn = false;
    } else {
        pushButton_streamGazePosition->setText("STOP");
        streamGazeButtonOn = true;
    }

}

void GazeToolbar::onButtonClick_ShowGaze() {

}

void GazeToolbar::setStatus_pushButton_streamGazePosition(bool status) {
    if (status == true) {
        pushButton_streamGazePosition->setText("STOP");
        streamGazeButtonOn = true;
    } else {
        pushButton_streamGazePosition->setText("ROS");
        streamGazeButtonOn = false;
    }
}

bool GazeToolbar::getStatus_pushButton_streamGazePosition() {
    return streamGazeButtonOn;
}

void GazeToolbar::setStatus_pushButton_manualGlint(bool status)
{
    if (status == true) {
        pushButton_ManualGlints->setText("STOP");
        manualGlints = true;
    } else {
        pushButton_ManualGlints->setText("Glint");
        manualGlints = false;
    }
}

bool GazeToolbar::getStatus_pushButton_manualGlint()
{
    return manualGlints;
}

