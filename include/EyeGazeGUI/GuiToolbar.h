/*
GuiToolbar.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2018 University of British Columbia

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


#ifndef GAZETOOLBAR_H
#define GAZETOOLBAR_H

#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include "EyeGazeCommon.h"
//#include "UtilCamera.h"
#include "qapplication.h"
#include <QDesktopWidget>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QProcess>

class GazeToolbar : public QToolBar
{
	Q_OBJECT
public:
	GazeToolbar(void);
	~GazeToolbar(void);

	// Widgets
	QPushButton* pushButton_Start;
	QPushButton* pushButton_Calibrate;
    QPushButton* pushButton_ShowGaze;
    //QComboBox* comboBox_trackerInputRight;
    //QComboBox* comboBox_trackerInputLeft;
    QComboBox* comboBox_trackerInputBoth;
	QComboBox* comboBox_calibNumber;
//	QComboBox* comboBox_trackerType;
	QComboBox* comboBox_calibMonitor;
    QComboBox* comboBox_calibEye;
    QPushButton* pushButton_Log;
    //QPushButton* pushButton_Calibrate3D;
    QPushButton* pushButton_streamGazePosition;
    QPushButton* pushButton_ManualGlints;
    QPushButton* pushButton_Head;
    //QPushButton* pushButton_HeadRepeat;

    bool isLooking();
    
    // Set/Get
    void setStatus_pushButton_streamGazePosition(bool status);
    bool getStatus_pushButton_streamGazePosition();
    void setStatus_pushButton_manualGlint(bool status);
    bool getStatus_pushButton_manualGlint();
public slots:
    void onButtonClickLog();
	void onButtonClickStart();
	void onButtonClickCalibNumber();
	void onComboBoxSelect_trackerType();
   // void onButtonClick_Calibrate3D();
    void onButtonClick_StreamGaze();
    void onButtonClick_ShowGaze();

private:
	void initLayout();
	bool startButtonOn;
	bool calibrateButtonOn;
    bool showGazeButtonOn;
    bool logButtonOn;
    bool streamGazeButtonOn;
    bool manualGlints;
};

#endif

