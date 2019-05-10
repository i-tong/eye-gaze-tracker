/*
Gui.h

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

#ifndef GAZETRACKGUI_H
#define GAZETRACKGUI_H

// Qt
#include <QApplication>
#include <QStandardPaths>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QHash>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QSettings>
#include <QTimer>

#include "ui_gazetrackgui.h"

// Gaze
#include "GuiToolbar.h"
#include "OpenGazeAPIServer.h"
#include "EyeTracker.h"
#include "CalibrationDialog.h"
#include "GuiDisplayWidget.h"
#include "GuiToolbar.h"
#include "GuiParamToolbar.h"
#include "GuiDataLogSettings.h"
#include "GuiPositionDisplay.h"
#include "GuiHeadCompensationDialog.h"
#include "ManualGlintDialog.h"
#include "CalibrationHead.h"
#include "OpenGazeAPIServer.h"
#include "UtilGui.h"
#include "GuiOpenGazeSettingsDialog.h"

// Boost
#include <boost/lockfree/queue.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/numeric/ublas/matrix.hpp>

// std
#include <time.h>
#include <cmath>
#include <fstream>

/*!
 * \brief The displaySource enum is used to keep track of the source for the display.
*/
enum displaySource {
    TRACKER,
    NO_SOURCE
};

/*!
 * \brief The GazeTrackGUI class is the main window of the GUI.
 *
 * The GazeTrackGUI class handles all of the user input in the main window. It communicates
 * with the eye gaze tracker. From this GUI calibration and logging can be carried out.
 */
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

    void buttonClickedManualGlints();
    void buttonClickedHead();
    void buttonClickedShowGaze();
    void menuItemSelectedVideoSourceRight();
    void menuItemSelectedVideoSourceLeft();
    void menuItemSelectedCaptureSettings();
    void menuItemSelectedOPAPISettings();
    void comboBoxChanged_calibEye(QString str);
    void comboBoxChanged_calibNumber(QString str);
    void getValue(QString ID);
    void setValue(QString ID, float value);
    void addCalibrationPoint(float X, float Y) ;
    void showCalibrationResults();
    void logCalibration(std::vector<boost::numeric::ublas::matrix<double>> lefttransmat, \
                        std::vector<boost::numeric::ublas::matrix<double>> righttransmat, \
                        int main_glint_left, int main_glint_right, \
                        int sec_glint_left, int sec_glint_right);
    void logHEADCalibration(std::vector<std::vector<cv::Vec2f>> pg_pog_mapping_left, \
                            std::vector<std::vector<cv::Vec2f>> pg_pog_mapping_right, \
                            std::vector<std::vector<cv::Vec2f>> pupil_pg_mapping_left,\
                            std::vector<std::vector<cv::Vec2f>> pupil_pg_mapping_right, \
                            int glintdist);
signals:
    void returnValue(QString ID, float value);
    void returnScreenSize(QString ID, int x, int y, int width, int height);
    void returnCameraSize(QString ID, int width, int height);
    void returnCalibrationSummary(QString id, float errLeft, float errRight, int numValidLeft, int numValidRight);
    void streamCalibrationTargetPositions(QString ID, std::vector<cv::Point2f> targetPos);
    void returnNoValue(QString ID);
private:
    // UI
    void closeEvent(QCloseEvent *event);
    Ui::gazeTrackGUIClass ui; /*!< Qt UI for gazeTrackGUI */
    displaySource display; /*! The current display source */
    QTimer* im_timer; /*! Timer for refreshing the display */
    QImage dispImage; /*! Current image of eyes to be displayed in UI */
    QMessageBox errorBox; /*! Error dialog */
    GazeToolbar* mainToolbar; /*! Toolbar containing buttons and controls */

    // Settings
    QString videoSourceRightFilename; /*! The filename of the video of the right eye */
    QString videoSourceLeftFilename; /*! The filename of the video of the left eye */
    QSettings* _settings; /*! Settings file */
    GuiOpenGazeSettingsDialog* _opapiSettingsDialog;
    // Eye tracker
    EyeTracker* tracker; /*! Eye gaze tracker */
    OpenGazeAPIServer* _api_server; /*! TCP/IP server for the Open Gaze API */
    cv::RotatedRect pFound_R; /*! Right eye pupil */
    cv::RotatedRect pFound_L; /*! Left eye pupil */
    double frameRate; /*! Current frame rate */
    std::vector<double> frameRateList; /*! List of previous frame rates, used to calculate mean FPS */

    // Calibration
    void initCalibrationByGUI();
    CalibrationHead* calHead; /*! Head calibration target dialog */
    HeadCompensationDialog* dialog_head_calibration; /*! Main head calibration dialog */
    float caliberr_right; /*! Current calibration error result for the right eye */
    float caliberr_left; /*! Current calibration error result for the left eye */
    GazePositionDisplay* gazedisplay; /*! Dialog for displaying the current gaze position, helpful for monitoring tracking */

    // Glint
    ManualGlintDialog* glintWindow; /*! Glint template calibration dialog */

    // Logging
    void logData(rclgaze::log_data data);
    void showCalibrationWindow(bool show);
    std::ofstream logfile; /*! Output log file */


};

#endif // GAZETRACKGUI_H
