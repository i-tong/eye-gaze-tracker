/*
Gui.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2017-2018 University of British Columbia

--- begin license - do not edit ---
    This file is a part of CGaze UI.
    
    CGaze UI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CGaze UI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CGaze UI.  If not, see <https://www.gnu.org/licenses/>.
--- end license ---
*/


#ifndef GAZETRACKGUI_H
#define GAZETRACKGUI_H

//#include <QtWidgets/QMainWindow>
#include <QMainWindow>
#include <QPushButton>
#include <QTimer>
#include <QImage>
#include <QMessageBox>
#include <qdebug.h>
#include <qfiledialog.h>
#include <QProgressDialog>
#include "qapplication.h"
#include <QDesktopWidget>
#include "qhash.h"

#include <boost/lockfree/queue.hpp>

#include "ui_gazetrackgui.h"

// Qt
#include <QApplication>
#include <QDesktopWidget>
#include <QHash>
#include <QSettings>
#include <QStandardPaths>

// Gaze
#include "GuiToolbar.h"
#include "OpenGazeAPIServer.h"
#include "EyeTracker.h"
#include "GazeDataLogger.h"
#include "CalibrationDialog.h"
#include "GuiDisplayWidget.h"
#include "GuiToolbar.h"
#include "GuiParamToolbar.h"
#include "GuiDebugWindow.h"
#include "GuiDataLogSettings.h"
#include "GuiPositionDisplay.h"
#include "GuiHeadCompensationDialog.h"
#include "ManualGlintDialog.h"
#include "CalibrationReDialog.h"
#include "CalibrationHead.h"
#include "OpenGazeAPIServer.h"
#include "UtilGui.h"

// Boost
#include <boost/lockfree/queue.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>

#include <time.h>
#include <cmath>
#include <fstream>
enum displaySource {
    TRACKER,
    NO_SOURCE
    //SCENE?
};

class GazeTrackGUI : public QMainWindow
{
    Q_OBJECT

public:
    GazeTrackGUI(QWidget *parent = 0);
    ~GazeTrackGUI();
    CalibrationDialog* dialog_calibration;
public slots:
    void refreshDisplaySource();
    void buttonClickedStartGazeTracker();
    void buttonClickedStartCalibration();
    void buttonClickedLog();

    void buttonClickedStreamGazePosition();
    void buttonClickedManualGlints();
    void buttonClickedHead();
    void buttonClickedShowGaze();
//    void comboBoxChangedTrackerType(QString str);
    void menuItemSelectedVideoSourceRight();
    void menuItemSelectedVideoSourceLeft();
    void menuItemSelectedVideoSourceLeft_Right();
    void menuItemSelectedStartRecording();
    void menuItemSelectedStopRecording();
    void menuItemSelectedCaptureSettings();
    void gazeParamChanged(QHash<QString, float> paramHash);
    void comboBoxChanged_calibEye(QString str);
    void comboBoxChanged_calibNumber(QString str);
    void getValue(QString ID);
    void setValue(QString ID, float value);
    void addCalibrationPoint(float X, float Y) ;
    void showCalibrationResults();
signals:
    void returnValue(QString ID, float value);
    void returnScreenSize(QString ID, int x, int y, int width, int height);
    void returnCameraSize(QString ID, int width, int height);
    void returnCalibrationSummary(QString id, float errLeft, float errRight, int numValidLeft, int numValidRight);
    void streamCalibrationTargetPositions(QString ID, std::vector<cv::Point2f> targetPos);
    void returnNoValue(QString ID);
private:

    Ui::gazeTrackGUIClass ui;
    EyeTracker* tracker;
    displaySource display;
    OpenGazeAPIServer* _api_server;
    QTimer* im_timer;
    QImage dispImage;
    cv::RotatedRect pFound_R;
    cv::RotatedRect pFound_L;
    cv::VideoWriter video;
    std::ofstream logfile;
    double frameRate;
    std::vector<double> frameRateList;
    QSettings* _settings;

    // Calibration
    CalibrationHead* calHead;
    void initCalibrationByGUI();
    HeadCompensationDialog* dialog_head_calibration;
    float caliberr_right;
    float caliberr_left;
    // Gaze display ui
    GazePositionDisplay* gazedisplay;

    // UI Setup
    GazeToolbar* mainToolbar;
    // Settings
    QString videoSourceRightFilename;
    QString videoSourceLeftFilename;

    //error pop up
    QMessageBox errorBox;

    //ros publisher
    //RosPublisher* rospublisher;


    //glint window
    ManualGlintDialog* glintWindow;

    // for robot recalibration
    //RobotReCalibration* robotRe;

    // Open Gaze API
    OpenGazeAPIServer* opapi_server;

    // Logging
    void logData(rclgaze::log_data data);
    void showCalibrationWindow(bool show);
protected:
     void closeEvent(QCloseEvent *event);

};

#endif // GAZETRACKGUI_H
