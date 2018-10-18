/*
GuiToolbar.h

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

#ifndef GAZETOOLBAR_H
#define GAZETOOLBAR_H

#include "EyeGazeCommon.h"

#include <QToolBar>
#include <QPushButton>
#include <QComboBox>
#include <QApplication>
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
    QComboBox* comboBox_trackerInputBoth;
    QComboBox* comboBox_calibNumber;
	QComboBox* comboBox_calibMonitor;
    QComboBox* comboBox_calibEye;
    QPushButton* pushButton_Log;
    QPushButton* pushButton_ManualGlints;
    QPushButton* pushButton_Head;

    bool isLooking();
    
    // Set/Get
    void setStatus_pushButton_streamGazePosition(bool status);
    bool getStatus_pushButton_streamGazePosition();
    void setStatus_pushButton_manualGlint(bool status);
    bool getStatus_pushButton_manualGlint();
public slots:
    void onButtonClickLog();
    void onButtonClickStart();
    void onButtonClick_ShowGaze();

private:
	void initLayout();
    bool _start_button_on;
    bool _calibrate_button_on;
    bool _log_button_on;
    bool _manual_glints_open;
};

#endif
