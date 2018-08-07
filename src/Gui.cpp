/*
Gui.cpp

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

#include "Gui.h"

GazeTrackGUI::GazeTrackGUI(QWidget *parent)
    : QMainWindow(parent)
{

    videoSourceLeftFilename = "/home/davinci3/Videos/Eye Videos/eyeVideo_test38.avi";//C:/Users/Irene/Videos/eyeVideo_test38.avi";
    videoSourceRightFilename = "/home/davinci3/Videos/Eye Videos/eyeVideo_test38.avi";//C:/Users/Irene/Videos/eyeVideo_test38.avi";


    tracker = new EyeTracker( QCoreApplication::applicationDirPath().toStdString());
    ui.setupUi(this);

    mainToolbar = new GazeToolbar();
    this->addToolBar(mainToolbar);
    // Start gaze tracker playback
    im_timer = new QTimer();
    im_timer->setInterval(10);
    im_timer->setSingleShot(false);
    im_timer->start();
    gazedisplay = new GazePositionDisplay(tracker);
    connect(gazedisplay,SIGNAL(rejected()),this,SLOT(buttonClickedShowGaze()));


    // Actions
    // Connect signals and slots
    // Main Toolbar signals
    connect(mainToolbar->pushButton_Start,SIGNAL(released()),this,SLOT(buttonClickedStartGazeTracker()));
    connect(mainToolbar->pushButton_Calibrate,SIGNAL(released()),this,SLOT(buttonClickedStartCalibration()));
    connect(mainToolbar->pushButton_Log,SIGNAL(released()),this,SLOT(buttonClickedLog()));

    //connect(mainToolbar->pushButton_Calibrate3D,SIGNAL(released()),this,SLOT(buttonClickedStartCalibration3D()));
    connect(im_timer,SIGNAL(timeout()),this,SLOT(refreshDisplaySource()));
    connect(mainToolbar->pushButton_streamGazePosition,SIGNAL(released()),this,SLOT(buttonClickedStreamGazePosition()));
    connect(mainToolbar->pushButton_ManualGlints,SIGNAL(released()),this,SLOT(buttonClickedManualGlints()));
    connect(mainToolbar->pushButton_Head,SIGNAL(clicked()),this,SLOT(buttonClickedHead()));
    connect(mainToolbar->pushButton_ShowGaze,SIGNAL(released()),this,SLOT(buttonClickedShowGaze()));
    // Parameter toolbar signals
    // Menubar signals
    connect(ui.actionRight_Video_Source,SIGNAL(triggered()),this,SLOT(menuItemSelectedVideoSourceRight()));
    connect(ui.actionLeft_Video_Source,SIGNAL(triggered()),this,SLOT(menuItemSelectedVideoSourceLeft()));
    connect(ui.actionLeft_Right_Video_Source,SIGNAL(triggered()),this,SLOT(menuItemSelectedVideoSourceLeft_Right()));
    connect(ui.actionCapture_Settings, SIGNAL(triggered()),this,SLOT(menuItemSelectedCaptureSettings()));
    connect(ui.actionStart_Recording, SIGNAL(triggered()),this,SLOT(menuItemSelectedStartRecording()));
    connect(ui.actionStop_Recording, SIGNAL(triggered()),this,SLOT(menuItemSelectedStopRecording()));

    connect(mainToolbar->comboBox_calibEye, SIGNAL(currentTextChanged(QString)),this,SLOT(comboBoxChanged_calibEye(QString)));
    connect(mainToolbar->comboBox_calibNumber, SIGNAL(currentTextChanged(QString)),this,SLOT(comboBoxChanged_calibNumber(QString)));

    // Init settings
    _settings = new QSettings(QSettings::NativeFormat,QSettings::UserScope,"RCL", "GazeTrackGUI");
    // Glint settings
    int GL01X, GL01Y,GL02X,GL02Y,GR01X,GR01Y,GR02X,GR02Y;
    GL01X = _settings->value("GL01X",3).toInt();
    GL01Y = _settings->value("GL01Y",-17).toInt();
    GL02X = _settings->value("GL02X",33).toInt();
    GL02Y = _settings->value("GL02Y",-17).toInt();

    GR01X = _settings->value("GR01X",-5).toInt();
    GR01Y = _settings->value("GR01Y",-16).toInt();
    GR02X = _settings->value("GR02X",-31).toInt();
    GR02Y = _settings->value("GR02Y",-11).toInt();

    tracker->setGlintTemplate(cv::Vec2f(GL01X,GL01Y),cv::Vec2f(GL02X,GL02Y),cv::Vec2f(GR01X,GR01Y),cv::Vec2f(GR02X,GR02Y));

    // Init calibration
    dialog_calibration = new CalibrationDialog();
    connect(dialog_calibration, SIGNAL(CalibrationFinished()),this, SLOT(showCalibrationResults()));
    initCalibrationByGUI();
    dialog_head_calibration = new HeadCompensationDialog(tracker);

    // Open Gaze API
    _api_server = new OpenGazeAPIServer();
    qRegisterMetaType< cv::Point2f >("cv::Point2f");
    qRegisterMetaType< cv::RotatedRect >("cv::RotatedRect");

    tracker->registerCalibrationCallbacks(boost::bind(&OpenGazeAPIServer::OPAPI_SendCalStart,_api_server,_1,_2), \
                                          boost::bind(&OpenGazeAPIServer::OPAPI_SendCalStop,_api_server,_1,_2), \
                                          boost::bind(&OpenGazeAPIServer::OPAPI_SendCalResult,_api_server,_1,_2,_3,_4,_5,_6));
    tracker->registerStreamCallback(boost::bind(&OpenGazeAPIServer::streamData,_api_server,_1));

    tracker->registerLogCallback(boost::bind(&GazeTrackGUI::logData,this,_1));

    //connect(tracker,SIGNAL(streamData(float,long long,cv::Point2f,cv::Point2f,bool,bool,bool,bool,cv::Point2f,bool,cv::RotatedRect,cv::RotatedRect)), \
    //         _api_server,SLOT(OPAPI_SendRecord(float,long long,cv::Point2f,cv::Point2f,bool,bool,bool,bool,cv::Point2f,bool,cv::RotatedRect,cv::RotatedRect)));

    // connect(tracker,SIGNAL(streamCalResult(int,std::vector<cv::Point2f>,std::vector<cv::Point2f>,std::vector<cv::Point2f>,std::vector<bool>,std::vector<bool>)), \
    //         _api_server,SLOT(OPAPI_SendCalResult(int,std::vector<cv::Point2f>,std::vector<cv::Point2f>,std::vector<cv::Point2f>,std::vector<bool>,std::vector<bool>)));

    // connect(tracker, SIGNAL(streamCalStart(int,cv::Point2f)), \
    //         _api_server, SLOT(OPAPI_SendCalStart(int,cv::Point2f)));

    // connect(tracker, SIGNAL(streamCalStop(int,cv::Point2f)), \
    //         _api_server, SLOT(OPAPI_SendCalStop(int,cv::Point2f)));

    connect(_api_server, SIGNAL(OPAPI_RequestValue(QString)),this, SLOT(getValue(QString)));

    connect(this,SIGNAL(returnValue(QString,float)), \
            _api_server,SLOT(OPAPI_ReplyValue(QString,float)));

    connect(this, SIGNAL(returnCameraSize(QString,int,int)), \
            _api_server,SLOT(OPAPI_ReplyCameraSize(QString,int,int)));

    connect(this, SIGNAL(returnScreenSize(QString,int,int,int,int)), \
            _api_server, SLOT(OPAPI_ReplyScreenSize(QString,int,int,int,int)));

    connect(this,SIGNAL(streamCalibrationTargetPositions(QString,std::vector<cv::Point2f>)), \
            _api_server, SLOT(OPAPI_ReplyCalibPoints(QString,std::vector<cv::Point2f>)));

    connect(this,SIGNAL(returnCalibrationSummary(QString,float,float,int,int)), \
            _api_server, SLOT(OPAPI_ReplyCalibSummary(QString,float,float,int,int)));

    connect(this, SIGNAL(returnNoValue(QString)),_api_server, SLOT(OPAPI_ReplyNoValue(QString)));

    connect(_api_server, SIGNAL(OPAPI_SetValue(QString,float)),this, SLOT(setValue(QString,float)));

    connect(_api_server, SIGNAL(OPAPI_CalibrateAddpoint(float,float)), \
            this, SLOT(addCalibrationPoint(float,float)));


}

GazeTrackGUI::~GazeTrackGUI()
{
    if (tracker->isRunning()) {
        tracker->stop();
    }
    if (logfile.is_open()) {
        logfile.close();
    }
    qDebug() << "End";
}

// Event handling

void GazeTrackGUI::buttonClickedStartGazeTracker() {

    if ( tracker->isRunning() == false ) { // Start gaze tracker
        //QString rightSel = mainToolbar->comboBox_trackerInputRight->currentText();
       // QString leftSel = mainToolbar->comboBox_trackerInputLeft->currentText();
        QString rightSel = mainToolbar->comboBox_trackerInputBoth->currentText();
        QString leftSel = mainToolbar->comboBox_trackerInputBoth->currentText();

        // Start gaze tracker
        ui.statusLabelCamera->setText("Loading video source...");

        this->repaint();

        // Right eye
        const char* video = "";//"/home/davinci3/Videos/Eye Videos/eyeVideo_test38.avi";// "C:/Users/Irene/Videos/eyeVideo_test38.avi";
        if (rightSel == "Video") {
            video = videoSourceRightFilename.toLatin1().data();
            tracker->setInputSource(video,rclgaze::RIGHT_EYE);
        } else if (rightSel.startsWith("Camera")) {
            std::vector<float> num = getNumberFromString(rightSel);
            if (num.size() > 0) {
                tracker->setInputSource(num.at(0),rclgaze::RIGHT_EYE);
            } else {
                qDebug() << "Error: Camera ID not recognized";
            }
        } else if (rightSel == "Image") {
            tracker->setInputSource(videoSourceRightFilename.toLatin1().data(),rclgaze::RIGHT_EYE);
        }else if (rightSel == "None") {
            tracker->removeInputSource(rclgaze::RIGHT_EYE);
        }
        // Left eye
        if (leftSel == "Video") { video = videoSourceLeftFilename.toLatin1().data();
            tracker->setInputSource(video,rclgaze::LEFT_EYE);
        } else if (leftSel.startsWith("Camera")) {
            std::vector<float> num = getNumberFromString(leftSel);
            if (num.size() > 0) {
                tracker->setInputSource(num.at(0),rclgaze::LEFT_EYE);
            } else {
                qDebug() << "Error: Camera ID not recognized";
            }
        } else if (leftSel == "Image") {
            tracker->setInputSource(videoSourceLeftFilename.toLatin1().data(),rclgaze::LEFT_EYE);
            //tracker->setInputSource("/home/davinci3/Documents/sampleImage.png",rclgaze::LEFT_EYE);
        } else if (leftSel == "None") {
            tracker->removeInputSource(rclgaze::LEFT_EYE);
        }

        bool startok = tracker->start();
        if (startok) {
            display = TRACKER;
            ui.statusLabelCamera->setText("Tracker running.");
        } else {
            ui.statusLabelCamera->setText("Could not start tracker");
            mainToolbar->onButtonClickStart();
        }

    } else if ( tracker->isRunning() == true ) { // Stop gaze tracker
        display = NO_SOURCE;
        dialog_calibration->stopCalibration();
        if (tracker->isLogging() == true) {
            tracker->stopLogging();
            if (logfile.is_open()) {
                logfile.close();
            }
            // Tracker will also stop logging if it is stopped while logging.
            mainToolbar->onButtonClickLog();
        }

        tracker->stop();
        ui.statusLabelCamera->setText("");
    }

    // Close wait dialog
}

void GazeTrackGUI::buttonClickedStartCalibration() {

    if (tracker->isRunning()) {
        if (tracker->isCalibrated()) {
            //dialog_calibration = NULL;
            //delete dialog_calibration;
            dialog_calibration->stopCalibration();
        }
        this->initCalibrationByGUI();
        this->showCalibrationWindow(true);
        dialog_calibration->startCalibration();

    }
    else {
        errorBox.setText("Please run tracker before calibration");
        errorBox.setWindowTitle("Error");
        errorBox.setIcon(QMessageBox::Warning);
        errorBox.show();
    }

}

void GazeTrackGUI::showCalibrationWindow(bool show){

    if (show == true) {

        dialog_calibration->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        dialog_calibration->show();
        dialog_calibration->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
        dialog_calibration->raise();
        dialog_calibration->activateWindow();

        // Determine which window to display on
        QString monitorIdFullStr = mainToolbar->comboBox_calibMonitor->currentText();

        if (monitorIdFullStr.compare("DeckLink") != 0 ) {
            QDesktopWidget* deskWin = QApplication::desktop();
            int monitorID = mainToolbar->comboBox_calibMonitor->currentIndex();
            QRect monitorRect = deskWin->availableGeometry(monitorID);
            dialog_calibration->move(QPoint(monitorRect.x(),monitorRect.y()));
            dialog_calibration->resize(monitorRect.size());
        } else {
            QDesktopWidget* deskWin = QApplication::desktop();
            QRect monitorRect = deskWin->availableGeometry(0);
            dialog_calibration->move(QPoint(monitorRect.x(),monitorRect.y()));
            dialog_calibration->resize(QSize(640,480));
        }

    } else if (show == false) {
        dialog_calibration->hide();
    }

}


void GazeTrackGUI::buttonClickedLog() {
    if (tracker->isLogging() == true) {
        tracker->stopLogging();
        if (logfile.is_open()) {
            logfile.close();
        }
    } else {
        // Get log directory
        QString dir = _settings->value("logfile_directory",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();

        bool log_data = false;
        if (_settings->value("pog_position_right").toBool() || \
                _settings->value("pg_right").toBool() || \
                _settings->value("pupil_position_right").toBool() || \
                _settings->value("pupil_size_right").toBool() || \
                _settings->value("glint_position_right").toBool() || \
                _settings->value("record_video_right").toBool() || \
                _settings->value("pog_position_right").toBool() || \
                _settings->value("pg_left").toBool() || \
                _settings->value("pupil_position_left").toBool() || \
                _settings->value("pupil_size_left").toBool() || \
                _settings->value("glint_position_left").toBool() || \
                _settings->value("record_video_left").toBool()    ) {
            log_data = true;
        }

        bool log_video = _settings->value("record_video_left").toBool() || _settings->value("record_video_right").toBool();
        if (!dir.endsWith("/")) {
            dir.append("/");
        }
        tracker->startLogging(log_data, log_video, dir.toStdString());
        
        // Create log tool
        if (log_data) {
            if (logfile.is_open()) {
                logfile.close();
            }

            QString fn = dir;
            QDateTime date = QDateTime::currentDateTime();
            QString dateString = date.toString("'gazelog'_dd:MM:yyyy_hh:mm:ss.'txt'");
            dateString.replace(":","-");
            fn.append(dateString);
            qDebug() << fn;
            logfile.open(fn.toStdString(),std::ios::out);

            logfile << "timestamp, timetick,";

            if (_settings->value("pog_position_right").toBool()) {
                logfile << "pog_right_x,pog_right_y,pog_valid_right,";
            }
            if (_settings->value("pg_right").toBool()) {
                logfile << "pg0_right_x,pg0_right_y,";
                logfile << "pg1_right_x,pg1_right_y,";
                logfile << "pg2_right_x,pg2_right_y,";
            }
            if (_settings->value("pupil_position_right").toBool()) {
                logfile << "pupil_right_x,pupil_right_y,";
            }
            if (_settings->value("pupil_size_right").toBool()) {
                logfile << "pupil_right_width,pupil_right_height,pupil_right_angle,";
            }
            if (_settings->value("pupil_position_right").toBool() || _settings->value("pupil_size_right").toBool()) {
                logfile << "pupil_found_right,";
            }
            if (_settings->value("glint_position_right").toBool()) {
                logfile << "glint0_right_x,glint0_right_y,";
                logfile << "glint1_right_x,glint1_right_y,";
                logfile << "glint2_right_x,glint2_right_y,";
            }

            // Left
            if (_settings->value("pog_position_left").toBool()) {
                logfile << "pog_left_x,pog_left_y,pog_valid_left,";
            }
            if (_settings->value("pg_left").toBool()) {
                logfile << "pg0_left_x,pg0_left_y,";
                logfile << "pg1_left_x,pg1_left_y,";
                logfile << "pg2_left_x,pg2_left_y,";
            }
            if (_settings->value("pupil_position_left").toBool()) {
                logfile << "pupil_left_x,pupil_left_y,";
            }
            if (_settings->value("pupil_size_left").toBool()) {
                logfile << "pupil_left_width,pupil_left_height,pupil_left_angle,";
            }
            if (_settings->value("pupil_position_left").toBool() || _settings->value("pupil_size_left").toBool()) {
                logfile << "pupil_found_left,";
            }
            if (_settings->value("glint_position_left").toBool()) {
                logfile << "glint0_left_x,glint0_left_y,";
                logfile << "glint1_left_x,glint1_left_y,";
                logfile << "glint2_left_x,glint2_left_y,";
            }

            logfile << "\n";
        }
        // Display log directory on GUI
        
    }

}

void GazeTrackGUI::buttonClickedManualGlints()
{
    // TODO: Check if this works!
    if (tracker->isRunning()) {
        if (!mainToolbar->getStatus_pushButton_manualGlint()){
            int window_size = 480;
            int actual_size = 100;
            glintWindow = new ManualGlintDialog(mainToolbar,tracker,_settings,window_size,actual_size);
            glintWindow->show();
            mainToolbar->setStatus_pushButton_manualGlint(true);

            // Get eye image
            cv::Mat right_eye, left_eye = cv::Mat::zeros(480,640,CV_8UC1);
            tracker->getFrame(right_eye, rclgaze::RIGHT_EYE);
            tracker->getFrame(left_eye, rclgaze::LEFT_EYE);

            cv::Mat cmbImg;
            QImage dis;
            cv::Point2f sides = cv::Point2f(actual_size/2,actual_size/2);
            cv::Mat leftROI = cv::Mat(left_eye,cv::Rect(pFound_L.center-sides,pFound_L.center+sides));
            cv::Mat rightROI = cv::Mat(right_eye,cv::Rect(pFound_R.center-sides,pFound_R.center+sides));
            cv::hconcat(leftROI,rightROI,cmbImg);
            cvtMat2Qimage(cmbImg, dis);
            glintWindow->drawImage(dis);
        }
        else{
            glintWindow->close();
            mainToolbar->setStatus_pushButton_manualGlint(false);
            // Record results

        }
    }
    else {
        errorBox.setText("Please run tracker");
        errorBox.setWindowTitle("Error");
        errorBox.setIcon(QMessageBox::Warning);
        errorBox.show();
    }
}

void GazeTrackGUI::buttonClickedHead()
{
    qDebug() << tracker->isRunning() << tracker->isCalibrated();

    if (tracker->isRunning() && tracker->isCalibrated()) {

        QString monitorIdFullStr = mainToolbar->comboBox_calibMonitor->currentText();
        QDesktopWidget* deskWin = QApplication::desktop();
        int monitorID = mainToolbar->comboBox_calibMonitor->currentIndex();
        dialog_head_calibration->setDisplayMonitor(monitorID);
        dialog_head_calibration->show();
    } else{
        errorBox.setText("Please run and calibrate tracker before accomodating HEAD movement");
        errorBox.setWindowTitle("Error");
        errorBox.setIcon(QMessageBox::Warning);
        errorBox.show();
    }

}

void GazeTrackGUI::buttonClickedShowGaze() {
    if (mainToolbar->pushButton_ShowGaze->text() == "Show\nGaze") {
        qDebug() << "show";
        if (tracker->isRunning() && tracker->isCalibrated()) {
            gazedisplay->show();
            mainToolbar->pushButton_ShowGaze->setText("Hide\nGaze");
        } else {
            errorBox.setText("Please run and calibrate tracker");
            errorBox.setWindowTitle("Error");
            errorBox.setIcon(QMessageBox::Warning);
            errorBox.show();
        }
    } else {
        qDebug() << "hide";
        gazedisplay->close();
        mainToolbar->pushButton_ShowGaze->setText("Show\nGaze");
    }
}

void GazeTrackGUI::buttonClickedStreamGazePosition() {
    // Start streaming gaze position through ROS node or socket

    if (tracker->isRunning()){
        if (mainToolbar->getStatus_pushButton_streamGazePosition()==false) {
            //rospublisher = new RosPublisher;
            // Make sure calibration is not taking place (can only stream after calibration is finished)
            if (dialog_calibration->isCalibrationRunning() ) {
                qDebug() << "Warning: Calibration is running, cannot start ROS node";
                mainToolbar->setStatus_pushButton_streamGazePosition(false);

            } else {

                // Start streaming
                //rospublisher->startGazePositionStream(tracker);
            }
        }
        else if (mainToolbar->getStatus_pushButton_streamGazePosition() == true) {
            // Stop streaming
            //rospublisher->stopGazePositionStream();
        }
    }

}

//void GazeTrackGUI::comboBoxChangedTrackerType(QString str) {
//    if (tracker->isRunning()) {
//        tracker->stop();
//    }
//    delete tracker;

//    if (mainToolbar->comboBox_trackerType->currentText() == "da Vinci" ) {
//        tracker = new GazeTracker;
//    } /*else if (mainToolbar->comboBox_trackerType->currentText() == "Head-mounted") {
//        tracker = new hmGazeTracker;
//    }*/
//}

void GazeTrackGUI::menuItemSelectedVideoSourceRight() {

    videoSourceRightFilename = QFileDialog::getOpenFileName(this,tr("Select right eye video file"),"",tr("Video Files (*.avi);;Image Files (*.png)"));
    //mainToolbar->comboBox_trackerInputRight->setCurrentText("Video");

    if (ui.actionLeft_Right_Video_Source->isChecked()) {
        videoSourceLeftFilename = videoSourceRightFilename;
        //mainToolbar->comboBox_trackerInputLeft->setCurrentText("Video");
    }
    mainToolbar->comboBox_trackerInputBoth->setCurrentText("Video");
}

void GazeTrackGUI::menuItemSelectedVideoSourceLeft() {

    videoSourceLeftFilename = QFileDialog::getOpenFileName(this,tr("Select left eye video file"),"",tr("Video Files (*.avi);;Image Files (*.png)"));
    //mainToolbar->comboBox_trackerInputLeft->setCurrentText("Video");

    if (ui.actionLeft_Right_Video_Source->isChecked()) {
        videoSourceRightFilename = videoSourceLeftFilename;
        //mainToolbar->comboBox_trackerInputRight->setCurrentText("Video");
    }
    mainToolbar->comboBox_trackerInputBoth->setCurrentText("Video");
}

void GazeTrackGUI::menuItemSelectedVideoSourceLeft_Right() {

   if (ui.actionLeft_Right_Video_Source->isChecked()) {
       videoSourceRightFilename = QFileDialog::getOpenFileName(this,tr("Select both eyes video file"),"",tr("Video Files (*.avi);;Image Files (*.png)"));

       videoSourceLeftFilename = videoSourceRightFilename;
        //mainToolbar->comboBox_trackerInputRight->setCurrentText("Video");
        //mainToolbar->comboBox_trackerInputLeft->setCurrentText("Video");
        mainToolbar->comboBox_trackerInputBoth->setCurrentText("Video");
    }

}

void GazeTrackGUI::menuItemSelectedStartRecording() {
    if (tracker->isRunning()){
        QString dir;
        // Load recording settings
        //tracker->startRecording();
    }
    else {
        errorBox.setText("Please run tracker");
        errorBox.setWindowTitle("Error");
        errorBox.setIcon(QMessageBox::Warning);
        errorBox.show();
    }

}

void GazeTrackGUI::menuItemSelectedStopRecording() {
    tracker->stopRecording();
}

void GazeTrackGUI::logData(rclgaze::log_data data) {
    logfile << data.timestamp << data.timetick;
    //qDebug() << data.framecount;
    if (_settings->value("pog_position_right").toBool()) {
        logfile << data.pog_right_x << "," << data.pog_right_y << "," << data.pog_right_valid << ",";
    }
    if (_settings->value("pg_right").toBool()) {
        logfile << data.pg0_right_x << "," << data.pg0_right_y << ",";
        logfile << data.pg1_right_x << "," << data.pg1_right_y << ",";
        logfile << data.pg2_right_x << "," << data.pg2_right_y << ",";
    }
    if (_settings->value("pupil_position_right").toBool()) {
        logfile << data.pupil_right_x << "," << data.pupil_right_y << ",";
    }
    if (_settings->value("pupil_size_right").toBool()) {
        logfile << data.pupil_right_width << "," << data.pupil_right_height << "," << data.pupil_right_angle << ",";
    }
    if (_settings->value("pupil_position_right").toBool() || _settings->value("pupil_size_right").toBool()) {
        logfile << data.eye_right_found << ",";
    }
    if (_settings->value("glint_position_right").toBool()) {
        logfile << data.glint0_right_x << "," << data.glint0_right_y << ",";
        logfile << data.glint1_right_x << "," << data.glint1_right_y << ",";
        logfile << data.glint2_right_x << "," << data.glint2_right_y << ",";
    }

    // Left
    if (_settings->value("pog_position_left").toBool()) {
        logfile << data.pog_left_x << "," << data.pog_left_y << "," << data.pog_left_valid << ",";
    }
    if (_settings->value("pg_left").toBool()) {
        logfile << data.pg0_left_x << "," << data.pg0_left_y << ",";
        logfile << data.pg1_left_x << "," << data.pg1_left_y << ",";
        logfile << data.pg2_left_x << "," << data.pg2_left_y << ",";
    }
    if (_settings->value("pupil_position_left").toBool()) {
        logfile << data.pupil_left_x << "," << data.pupil_left_y << ",";
    }
    if (_settings->value("pupil_size_left").toBool()) {
        logfile << data.pupil_left_width << "," << data.pupil_left_height << "," << data.pupil_left_angle << ",";
    }
    if (_settings->value("pupil_position_left").toBool() || _settings->value("pupil_size_left").toBool()) {
        logfile << data.eye_left_found << ",";
    }
    if (_settings->value("glint_position_left").toBool()) {
        logfile << data.glint0_left_x << "," << data.glint0_left_y << ",";
        logfile << data.glint1_left_x << "," << data.glint1_left_y << ",";
        logfile << data.glint2_left_x << "," << data.glint2_left_y << ",";
    }

    logfile << "\n";
}

void GazeTrackGUI::menuItemSelectedCaptureSettings() {
    Dialog_DataLogSettings d(_settings);
    d.exec();
}

void GazeTrackGUI::comboBoxChanged_calibEye(QString str) {

    rclgaze::eyeSide calibEyeSide;
    if (str == "Both Eyes") {
        calibEyeSide = rclgaze::BOTH_EYES;
    } else if (str == "Right Eye") {
        calibEyeSide = rclgaze::RIGHT_EYE;
    } else if (str == "Left Eye") {
        calibEyeSide = rclgaze::LEFT_EYE;
    }
    dialog_calibration->setEyeSide(calibEyeSide);

}

void GazeTrackGUI::comboBoxChanged_calibNumber(QString str) {

    QString numCalib_str = str;
    float move_rate = 0.7;
    int numTargets = 5;
    if (numCalib_str == "5 point") {
        numTargets = 5;
    } else if (numCalib_str == "9 point") {
        numTargets = 9;
        move_rate = 0.6;
    } else if (numCalib_str == "16 point") {
        numTargets = 16;
        move_rate = 0.4;
    }
    dialog_calibration->setNumTargets(numTargets);

}

void GazeTrackGUI::refreshDisplaySource() {
    cv::Mat dispImage_cv;
    cv::Mat combImage;
    std::vector<cv::Point2f> gList;

    bool resRight = false;
    bool resLeft = false;
    bool pupilFound;
    bool glintFound;
    cv::Mat origImageRight;
    cv::Mat origImageLeft;
    double avgRate = 0;
    int test ;
    switch(display) {
    case (TRACKER):
        // Check if tracker is running
        if (!tracker->isRunning()) {
            display = NO_SOURCE;
            break;
        }

        // Right
        // Get image
        // qDebug() << tracker->getTestResR() << tracker->getTestResL();
        resRight = tracker->getFrame(origImageRight, rclgaze::RIGHT_EYE);

        // Overlay pupil and glint centers
        pupilFound = tracker->getPupil(pFound_R, rclgaze::RIGHT_EYE);
        glintFound = tracker->getGlints(gList, rclgaze::RIGHT_EYE);
        if (pupilFound) {
            cv::ellipse(origImageRight,pFound_R,cv::Scalar(0,255,0),2,8);
            cv::circle(origImageRight,pFound_R.center,2,cv::Scalar(0,255,0),-1);
        }
        if (glintFound && !std::isnan(gList.at(0).x)) {
            cv::circle(origImageRight,gList.at(0),3,cv::Scalar(255,255,0),1,8);
        }
        if (glintFound && !std::isnan(gList.at(1).x)) {
            cv::circle(origImageRight,gList.at(1),3,cv::Scalar(255,0,0),1,8);
        }
        if (glintFound && !std::isnan(gList.at(2).x)) {
            cv::circle(origImageRight,gList.at(2),3,cv::Scalar(255,0,255),1,8);
        }



        // Left
        // Get image
        resLeft = tracker->getFrame(origImageLeft, rclgaze::LEFT_EYE);

        // Overlay pupil and glint centers
        pupilFound = tracker->getPupil(pFound_L, rclgaze::LEFT_EYE);
        glintFound = tracker->getGlints(gList, rclgaze::LEFT_EYE);
        if (pupilFound){
            cv::ellipse(origImageLeft,pFound_L,cv::Scalar(0,255,0),2,8);
            cv::circle(origImageLeft,pFound_L.center,2,cv::Scalar(0,255,0),-1);
        }
        // plot glints
        if (glintFound && !std::isnan(gList.at(0).x)) {
            cv::circle(origImageLeft,gList.at(0),3,cv::Scalar(255,255,0),1,8);
        }
        if (glintFound && !std::isnan(gList.at(1).x)) {
            cv::circle(origImageLeft,gList.at(1),3,cv::Scalar(255,0,0),1,8);
        }
        if (glintFound && !std::isnan(gList.at(2).x)) {
            cv::circle(origImageLeft,gList.at(2),3,cv::Scalar(255,0,255),1,8);
        }

        // Concatenate right/left images
        if (resRight == false && resLeft == false ) {
            break;
        } else if ( resRight == true && resLeft == false ) {
            origImageRight.copyTo(combImage);
        } else if ( resRight == false && resLeft == true ) {
            origImageLeft.copyTo(combImage);
        } else if ( resRight == true && resLeft == true ) {
            // First make sure both are filler to the smaller image
            if (origImageLeft.cols != origImageRight.cols ||
                    origImageLeft.rows != origImageRight.rows) {
                int newWidth = origImageLeft.cols > origImageRight.cols ? origImageLeft.cols : origImageRight.cols;
                int newHeight = origImageLeft.rows > origImageRight.rows ? origImageLeft.rows : origImageRight.rows;
                cv::Mat largeImage;
                if (origImageLeft.cols > origImageRight.cols ||
                        origImageLeft.rows > origImageRight.rows ) {
                    //fill_image(origImageRight, largeImage,newWidth, newHeight);
                    cv::resize(origImageLeft, largeImage, origImageRight.size());
                    cv::hconcat(largeImage,origImageRight,combImage);
                } else {
                    //fill_image(origImageLeft, largeImage,newWidth, newHeight);
                    cv::resize(origImageLeft, largeImage, origImageRight.size());
                    cv::hconcat(origImageLeft,largeImage,combImage);
                }
            } else {
                cv::hconcat(origImageLeft,origImageRight,combImage);
            }
        }

        // Convert image
        cvtMat2Qimage(combImage,dispImage);

        // Set display
        ui.widget->drawImage(dispImage);
        frameRate = tracker->getFPS();

        frameRateList.push_back(frameRate);
        while (frameRateList.size() > 20) {
            frameRateList.erase(frameRateList.begin());
        }

        for (int i = 0 ; i < frameRateList.size() ; i++) {
            avgRate += frameRateList.at(i);
        }
        avgRate = avgRate / frameRateList.size();
        ui.statusLabelFPS->setText(QString("Frame Rate: %1 fps").arg(int(avgRate)));

        // Status bar: Display calibration error
        if (tracker->isCalibrated()) {
            int total = this->dialog_calibration->getNumTargets();

            cv::Vec2f err_left = tracker->getCalibrationError(rclgaze::LEFT_EYE);
            int numValid_left = tracker->getCalibrationNumValid(rclgaze::LEFT_EYE);

            cv::Vec2f err_right = tracker->getCalibrationError(rclgaze::RIGHT_EYE);
            int numValid_right = tracker->getCalibrationNumValid(rclgaze::RIGHT_EYE);

            // Get screen width and height
            QRect calscreen = dialog_calibration->geometry();

            // Calculate error
            float lefterrx, lefterry, righterrx, righterry, lefterrnet, righterrnet;
            lefterrx = float(calscreen.width()) * (err_left[0]/100.0);
            lefterry = float(calscreen.height()) * (err_left[1]/100.0);

            righterrx = float(calscreen.width()) * (err_right[0]/100.0);
            righterry = float(calscreen.height()) * (err_right[1]/100.0);

            lefterrnet = sqrt(lefterrx*lefterrx + lefterry*lefterry);
            righterrnet = sqrt(righterrx*righterrx + righterry*righterry);
            //qDebug() << err_left[0] << err_left[1];
            QString calres = QString("Calibration Error");
            calres.append(QString("Left: %1 px (%3/%4)").arg(lefterrnet).arg(numValid_left).arg(total));
            calres.append(QString("Right: %1 px (%3/%4)").arg(righterrnet).arg(numValid_right).arg(total));
            //qDebug() << calres;

        }


        break;
    case (NO_SOURCE):

        ui.statusBar->showMessage(QString("Frame Rate: 0 fps"));
        break;
    }
}

void GazeTrackGUI::gazeParamChanged(QHash<QString, float> paramHash) {
    //tracker->updateGazeParam(paramHash);
}

void GazeTrackGUI::closeEvent(QCloseEvent *event) {
    if (dialog_calibration->isVisible()) {
        qDebug() << "Close dialog";
        dialog_calibration->close();
    }
    if (tracker->isRunning()) {
        tracker->stop();
    }
    qDebug() << "Closing application\n";

}

// Open Gaze API

void GazeTrackGUI::getValue(QString ID) {
    qDebug() << "GET" << ID;
    if (ID.compare("CALIBRATE_START") == 0) {
        bool ret = dialog_calibration->isCalibrationRunning();
        emit returnValue(ID,int(ret));
    } else if (ID.compare("CALIBRATE_SHOW") == 0) {
        bool ret = dialog_calibration->isVisible();
        emit returnValue(ID,int(ret));
    } else if (ID.compare("CALIBRATE_TIMEOUT") == 0) {
        emit returnValue(ID,dialog_calibration->getCalibrationTimeout());
    } else if (ID.compare("CALIBRATE_DELAY") == 0) {
        emit returnValue(ID,dialog_calibration->getCalibrationDelay());
    } else if (ID.compare("CALIBRATE_RESULTS_SUMMARY") == 0) {
        if (tracker->isCalibrated()) {
            cv::Vec2f err_left = tracker->getCalibrationError(rclgaze::LEFT_EYE);
            int numValid_left = tracker->getCalibrationNumValid(rclgaze::LEFT_EYE);

            cv::Vec2f err_right = tracker->getCalibrationError(rclgaze::RIGHT_EYE);
            int numValid_right = tracker->getCalibrationNumValid(rclgaze::RIGHT_EYE);

            // Calculate overall error in pixels
            // Get screen width and height
            QRect calscreen = dialog_calibration->geometry();

            // Calculate error
            float lefterrx, lefterry, righterrx, righterry, lefterrnet, righterrnet;
            lefterrx = float(calscreen.width()) * (err_left[0]/100.0);
            lefterry = float(calscreen.height()) * (err_left[1]/100.0);

            righterrx = float(calscreen.width()) * (err_right[0]/100.0);
            righterry = float(calscreen.height()) * (err_right[1]/100.0);

            lefterrnet = sqrt(lefterrx*lefterrx + lefterry*lefterry);
            righterrnet = sqrt(righterrx*righterrx + righterry*righterry);
            emit returnCalibrationSummary(ID,lefterrnet,righterrnet,numValid_left,numValid_right);

        }else {
            // return NACK
            emit returnNoValue(ID);
        }

    } else if (ID.compare("CALIBRATE_CLEAR") == 0) {
        emit returnValue(ID,dialog_calibration->getNumTargets());
    } else if (ID.compare("CALIBRATE_RESET") == 0) {
        emit returnValue(ID,dialog_calibration->getNumTargets());
    } else if (ID.compare("CALIBRATE_ADDPOINT") == 0) {
        int numPts = dialog_calibration->getNumTargets();
        std::vector<cv::Point2f> targetPos;
        dialog_calibration->getCalibrationPoints(targetPos);
        emit streamCalibrationTargetPositions(ID, targetPos);
    } else if (ID.compare("TRACKER_DISPLAY") == 0) {
    } else if (ID.compare("TIME_TICK_FREQUENCY") == 0) {
         emit returnValue(ID,CLOCKS_PER_SEC);
    } else if (ID.compare("SCREEN_SIZE") == 0) {
        emit returnScreenSize(ID,0,0,640,480);
    } else if (ID.compare("CAMERA_SIZE") == 0) {
        emit returnCameraSize(ID,1280,480);
    } else if (ID.compare("PRODUCT_ID") == 0) {
        emit returnValue(ID, 0);
    } else if (ID.compare("SERIAL_ID") == 0) {
        emit returnValue(ID, 0);
    } else if (ID.compare("COMPANY_ID") == 0) {
        emit returnValue(ID, 0);
    } else if (ID.compare("API_ID") == 0) {
        emit returnValue(ID,1);
    } else if (ID.compare("") == 0) {
    } else {
        // Need to respond with NACK
        emit returnNoValue(ID);
    }
}

void GazeTrackGUI::setValue(QString ID, float value) {

    bool fail = false;
    if (ID.compare("CALIBRATE_START") == 0) {
        if (tracker->isRunning()) {
            this->buttonClickedStartCalibration();
        } else {
            fail = true;
        }
    } else if (ID.compare("CALIBRATE_SHOW") == 0) {
        if (value == 1) {
            // Hide calibration window
            showCalibrationWindow(true);
        } else if (value == 0) {
            // Show calibration
            showCalibrationWindow(false);
        }
    } else if (ID.compare("CALIBRATE_TIMEOUT") == 0) {
        if (this->dialog_calibration->isCalibrationRunning() == false) {
            this->dialog_calibration->setCalibrationTimeout(value);
        } else {
            fail = true;
        }
    } else if (ID.compare("CALIBRATE_DELAY") == 0) {
        if (this->dialog_calibration->isCalibrationRunning() == false) {
            this->dialog_calibration->setCalibrationDelay(value);
        } else {
            fail = true;
        }
    } else if (ID.compare("CALIBRATE_CLEAR") == 0) {
        if (this->dialog_calibration->isCalibrationRunning() == false) {
            this->dialog_calibration->setNumTargets(0);
        } else {
            fail = true;
        }
    } else if (ID.compare("CALIBRATE_RESET") == 0) {
        if (this->dialog_calibration->isCalibrationRunning() == false) {
            QString defnum = mainToolbar->comboBox_calibNumber->currentText();
            this->comboBoxChanged_calibNumber(defnum);
        } else {
            fail = true;
        }
    } else {
        fail = true;
    }


    // Call GET value to return state
    if (fail == false) {
        this->getValue(ID);
    } else {
        emit returnNoValue(ID);
    }
}

void GazeTrackGUI::addCalibrationPoint(float X, float Y) {
    if (this->dialog_calibration->isCalibrationRunning() == false) {
        dialog_calibration->addCalibrationPoint(X,Y);
        this->getValue("CALIBRATE_ADDPOINT");
    } else {
        emit returnNoValue("CALIBRATE_ADDPOINT");
    }
}

void GazeTrackGUI::initCalibrationByGUI() {
    // Determine number of calibration points set
    QString numCalib_str = mainToolbar->comboBox_calibNumber->currentText();
    float move_rate = 0.7;
    int numTargets = 5;
    if (numCalib_str == "5 point") {
        numTargets = 5;
    } else if (numCalib_str == "9 point") {
        numTargets = 9;
        move_rate = 0.6;
    } else if (numCalib_str == "16 point") {
        numTargets = 16;
        move_rate = 0.4;
    }

    //dialog_calibration = new CalibrationDialog();

    // Select the eye to calibrate
    rclgaze::eyeSide calibEyeSide = rclgaze::BOTH_EYES;
    QString calibEye_str = mainToolbar->comboBox_calibEye->currentText();
    if (calibEye_str == "Both Eyes") {
        calibEyeSide = rclgaze::BOTH_EYES;
    } else if (calibEye_str == "Right Eye") {
        calibEyeSide = rclgaze::RIGHT_EYE;
    } else if (calibEye_str == "Left Eye") {
        calibEyeSide = rclgaze::LEFT_EYE;
    }

    QString monitorname = mainToolbar->comboBox_calibMonitor->currentText();
    bool useDecklink = (monitorname.compare("DeckLink") == 0);
    if (useDecklink) {
        // start decklink connection

    }
    dialog_calibration->initCalibration(numTargets,10,20,0.1,0.02,move_rate,1500,1000,calibEyeSide,*tracker,useDecklink, 640, 480);

}

void GazeTrackGUI::showCalibrationResults() {
    // Display calibration results on status bar
    cv::Vec2f righterr = tracker->getCalibrationError(rclgaze::RIGHT_EYE);
    cv::Vec2f lefterr = tracker->getCalibrationError(rclgaze::LEFT_EYE);

    int rightnumvalid = tracker->getCalibrationNumValid(rclgaze::RIGHT_EYE);
    int leftnumvalid = tracker->getCalibrationNumValid(rclgaze::LEFT_EYE);

    QString res = QString("Calibration Error: Left: x: %1%, y: %2%  Right: x: %3%, y: %4% ").arg(lefterr[0],1,'f',1).arg(lefterr[1],1,'f',1).arg(righterr[0],1,'f',1).arg(righterr[1],1,'f',1);
    QString monitorname = mainToolbar->comboBox_calibMonitor->currentText();
    bool useDecklink = (monitorname.compare("DeckLink") == 0);
    //if (useDecklink) {

    //}
    ui.statusLabelCalibration->setText(res);
}

