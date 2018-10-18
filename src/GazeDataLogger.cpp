/*
GazeDataLogger.cpp

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




#include "GazeDataLogger.h"

GazeDataLogger::GazeDataLogger()
{
    m_settingsFilename = "/home/davinci3/gaze/branches/gazeTrackGUI/gaze_datalog.ini";
    loadSettings();
    mSocket = new utilSocket(312);
    mSocket->setAddress("137.82.56.245","12345");

    isLooking = false; // TODO: Remove this.
}

GazeDataLogger::~GazeDataLogger() {
    this->stop();
    
}

void GazeDataLogger::start() {
    // Settings
    this->loadSettings();

    // Create file name
    QDateTime date = QDateTime::currentDateTime();
    QString dateString = date.toString("'GazeLog'_dd:MM:yyyy_hh:mm:ss.log");
    dateString.replace(":","");
    m_logFilename = m_logFileDirectory;
    if (m_logFilename.endsWith("/") == false) {
        m_logFilename.append("/");
    }
    m_logFilename.append(dateString);

    writeHeader(m_logFilename);

    // Socket connection
//    m_flag_socket = mSocket->connect();
    //mSocket->connectSocket("127.0.0.1",12345); //use a local IP for prototyping

    //mSocket->connectSocket("137.82.56.245",12345);
    //m_flag_socket = mSocket->getStatus();

    if (m_flag_socket == false) {
        qDebug() << "Warning: Could not make socket connection!";
    }
    // Start
    m_startTime = QDateTime::currentMSecsSinceEpoch();
    m_runFlag = true;
    m_thr = boost::thread(&GazeDataLogger::run,this);

}

void GazeDataLogger::stop() {
    qDebug() << "Stopping gaze data logger!";

    if (m_runFlag == true) {
        {
            boost::lock_guard<boost::mutex> lock(m_runMutex);
            m_runFlag = false;
        }
        m_thr.join();
    }
    qDebug() << "Thread stopped";
}
/*
void GazeDataLogger::logData( const char* value, ...) {
    // Input to log data is:
    // logData( "number of values","value #1 name",value1, "value #2 name", value2,...);
    //https://cs50.harvard.edu/resources/cppreference.com/stdother/va_arg.html
    va_list args;
    va_start(args, value);
    int num = atoi(value);
    QHash<QString, double> hash;
    
    for (int i = 0; i < num; i++) {
    
        char* key = va_arg(args, char *);
        qDebug() << i << "In:" << key;
        double val = va_arg(args, double);
        qDebug() << i << "In:" << val;
        hash.insert(key,val);
    }
    m_data.push_back(hash);
    va_end(args);
}*/

void GazeDataLogger::logData( cv::RotatedRect& pupilRight, int glintIndexRight, cv::Vec2f& pgRight, cv::Point2f& pogRight, cv::Point2f& correctionRight, std::vector<cv::Point2f>& glintListRight, bool eyeFoundRight, \
                              cv::RotatedRect& pupilLeft, int glintIndexLeft ,cv::Vec2f& pgLeft,  cv::Point2f& pogLeft,cv::Point2f& correctionLeft, std::vector<cv::Point2f>& glintListLeft,  bool eyeFoundLeft, cv::Vec3f& pog3D,\
                              cv::Point2f& target, std::vector<cv::Point2f>& recalib, std::vector<float>& robotPose, std::vector<cv::Point2f>& pjt) {
    if ( m_runFlag == true ) {       
        qint64 msec = QDateTime::currentMSecsSinceEpoch();// - m_startTime;

        QHash<QString, double> hash;
        hash.insert("timestamp",(double)msec);
        double val = hash.value("timestamp");

        hash.insert("pupil_x_position_right", pupilRight.center.x);
        hash.insert("pupil_y_position_right", pupilRight.center.y);
        hash.insert("pupil_width_right", pupilRight.size.width);
        hash.insert("pupil_height_right", pupilRight.size.height);
        hash.insert("pupil_angle_right", pupilRight.angle);
        
        hash.insert("pog_x_position_right",pogRight.x);
        hash.insert("pog_y_position_right",pogRight.y);

        hash.insert("pog_x_correction_right",correctionRight.x);
        hash.insert("pog_y_correction_right",correctionRight.y);

        hash.insert("glint_index_right",glintIndexRight);

        hash.insert("pg_x_right",pgRight[0]);
        hash.insert("pg_y_right",pgRight[1]);

        switch (glintListRight.size()) {
        case 0:
            hash.insert("glint0_x_position_right",-1);
            hash.insert("glint0_y_position_right",-1);
            hash.insert("glint1_x_position_right",-1);
            hash.insert("glint1_y_position_right",-1);
            break;
        case 1:
            hash.insert("glint0_x_position_right",glintListRight.at(0).x);
            hash.insert("glint0_y_position_right",glintListRight.at(0).y);
            hash.insert("glint1_x_position_right",-1);
            hash.insert("glint1_y_position_right",-1);
            break;
        case 2:
            hash.insert("glint0_x_position_right",glintListRight.at(0).x);
            hash.insert("glint0_y_position_right",glintListRight.at(0).y);
            hash.insert("glint1_x_position_right",glintListRight.at(1).x);
            hash.insert("glint1_y_position_right",glintListRight.at(1).y);
            break;
        case 3:
            hash.insert("glint0_x_position_right",glintListRight.at(0).x);
            hash.insert("glint0_y_position_right",glintListRight.at(0).y);
            hash.insert("glint1_x_position_right",glintListRight.at(1).x);
            hash.insert("glint1_y_position_right",glintListRight.at(1).y);
            hash.insert("glint2_x_position_right",glintListRight.at(2).x);
            hash.insert("glint2_y_position_right",glintListRight.at(2).y);
            break;
        default:
            hash.insert("glint0_x_position_right",-1);
            hash.insert("glint0_y_position_right",-1);
            hash.insert("glint1_x_position_right",-1);
            hash.insert("glint1_y_position_right",-1);
            hash.insert("glint2_x_position_right",-1);
            hash.insert("glint2_y_position_right",-1);
            break;
        }
        hash.insert("eye_found_right",(float)eyeFoundRight);
        hash.insert("pupil_x_position_left", pupilLeft.center.x);
        hash.insert("pupil_y_position_left", pupilLeft.center.y);
        hash.insert("pupil_width_left", pupilLeft.size.width);
        hash.insert("pupil_height_left", pupilLeft.size.height);
        hash.insert("pupil_angle_left", pupilLeft.angle);
        
        hash.insert("pog_x_position_left",pogLeft.x);
        hash.insert("pog_y_position_left",pogLeft.y);

        hash.insert("pog_x_correction_left",correctionLeft.x);
        hash.insert("pog_y_correctoin_left",correctionLeft.y);

        hash.insert("glint_index_left",glintIndexLeft);
        
        hash.insert("pg_x_left",pgLeft[0]);
        hash.insert("pg_y_left",pgLeft[1]);
        
        switch (glintListLeft.size()) {
        case 0:
            hash.insert("glint0_x_position_left",-1);
            hash.insert("glint0_y_position_left",-1);
            hash.insert("glint1_x_position_left",-1);
            hash.insert("glint1_y_position_left",-1);
            break;
        case 1:
            hash.insert("glint0_x_position_left",glintListLeft.at(0).x);
            hash.insert("glint0_y_position_left",glintListLeft.at(0).y);
            hash.insert("glint1_x_position_left",-1);
            hash.insert("glint1_y_position_left",-1);
            break;
        case 2:
            hash.insert("glint0_x_position_left",glintListLeft.at(0).x);
            hash.insert("glint0_y_position_left",glintListLeft.at(0).y);
            hash.insert("glint1_x_position_left",glintListLeft.at(1).x);
            hash.insert("glint1_y_position_left",glintListLeft.at(1).y);
            break;
        case 3:
            hash.insert("glint0_x_position_left",glintListLeft.at(0).x);
            hash.insert("glint0_y_position_left",glintListLeft.at(0).y);
            hash.insert("glint1_x_position_left",glintListLeft.at(1).x);
            hash.insert("glint1_y_position_left",glintListLeft.at(1).y);
            hash.insert("glint2_x_position_left",glintListLeft.at(2).x);
            hash.insert("glint2_y_position_left",glintListLeft.at(2).y);
            break;
        default:
            hash.insert("glint0_x_position_left",-1);
            hash.insert("glint0_y_position_left",-1);
            hash.insert("glint1_x_position_left",-1);
            hash.insert("glint1_y_position_left",-1);
            hash.insert("glint2_x_position_left",-1);
            hash.insert("glint2_y_position_left",-1);
            break;
        }
        hash.insert("eye_found_left",(float)eyeFoundLeft);

        hash.insert("pog_x_position_3d",pog3D[0]);
        hash.insert("pog_y_position_3d",pog3D[1]);
        hash.insert("pog_z_position_3d",pog3D[2]);

        // robot
        hash.insert("psm2_x_pos",robotPose.at(0));
        hash.insert("psm2_y_pos",robotPose.at(1));
        hash.insert("psm2_z_pos",robotPose.at(2));
        hash.insert("psm2_qx",robotPose.at(3));
        hash.insert("psm2_qy",robotPose.at(4));
        hash.insert("psm2_qz",robotPose.at(5));
        hash.insert("psm2_qw",robotPose.at(6));

        // projection
        hash.insert("psm2_proj_left_x",pjt.at(0).x);
        hash.insert("psm2_proj_left_y",pjt.at(0).y);
        hash.insert("psm2_proj_right_x",pjt.at(1).x);
        hash.insert("psm2_proj_right_y",pjt.at(1).y);

        hash.insert("is_looking",(float)isLooking);

        // target position for test
        hash.insert("target_x", target.x);
        hash.insert("target_y", target.y);


        // recalibration shift
        hash.insert("recalib0_left_x", recalib.at(0).x);
        hash.insert("recalib0_left_y", recalib.at(0).y);
        hash.insert("recalib0_right_x", recalib.at(1).x);
        hash.insert("recalib0_right_y", recalib.at(1).y);
        hash.insert("recalib1_left_x", recalib.at(0).x);
        hash.insert("recalib1_left_y", recalib.at(0).y);
        hash.insert("recalib1_right_x", recalib.at(1).x);
        hash.insert("recalib1_right_y", recalib.at(1).y);
        hash.insert("recalib2_left_x", recalib.at(0).x);
        hash.insert("recalib2_left_y", recalib.at(0).y);
        hash.insert("recalib2_right_x", recalib.at(1).x);
        hash.insert("recalib2_right_y", recalib.at(1).y);


        m_data.push_back(hash);

        qint64 msec2 = QDateTime::currentMSecsSinceEpoch()- m_startTime - msec;
        //qDebug() << "Logging takes" << msec2 << "ms";
    }
    
}

void GazeDataLogger::setStatus(bool status) {
    isLooking = status;
}

void GazeDataLogger::run() {

    while (true) {
        {
            boost::lock_guard<boost::mutex> lock(m_runMutex);
            if (m_runFlag == false) {
                break;
            }
        }

        if (m_data.size() <= 0) {
            continue;
        }
        QHash<QString, double> data = m_data.at(0);
        // Open gaze log
        std::fstream log;
        log.open(m_logFilename.toStdString(), std::fstream::in | std::fstream::out | std::fstream::app);
        log << std::setprecision(32) << data.value("timestamp") << ",";
        log << std::setprecision(32) << data.value("timetick") << ",";
        log << data.value("frame_count") << ",";
        // Right
        if (m_flag_pupilPositionRight) {
            log << data.value("pupil_x_position_right") << "," << data.value("pupil_y_position_right") << ",";
        }
        if (m_flag_pogPositionRight) {
            log << data.value("pog_x_position_right") << "," << data.value("pog_y_position_right") << ",";
        }
        log << data.value("pog_x_correction_right")<<","<<data.value("pog_y_correction_right")<<",";
        log << data.value("glint_index_right") << ",";
        if (m_flag_glintPositionRight) {
            log << data.value("glint0_x_position_right") << "," << data.value("glint0_y_position_right") << ",";
            log << data.value("glint1_x_position_right") << "," << data.value("glint1_y_position_right") << ",";
            log << data.value("glint2_x_position_right") << "," << data.value("glint2_y_position_right") << ",";
        }
        if (m_flag_pgRight) {
            log << data.value("pg_x_right") << "," << data.value("pg_y_right") << ",";
        }
        if (m_flag_pupilSizeRight) {
            log << data.value("pupil_width_right") << "," << data.value("pupil_height_right") << "," << data.value("pupil_angle_right") << ",";
        }
        if (m_flag_eyeFoundRight) {
            log << data.value("eye_found_right") << ",";
        }


        // Left
        if (m_flag_pupilPositionLeft) {
            log << data.value("pupil_x_position_left") << "," << data.value("pupil_y_position_left") << ",";
        }
        if (m_flag_pogPositionLeft) {
            log << data.value("pog_x_position_left") << "," << data.value("pog_y_position_left") << ",";
        }
        log << data.value("pog_x_correction_left")<<","<<data.value("pog_y_correctoin_left")<<",";
        log << data.value("glint_index_left") << ",";
        if (m_flag_glintPositionLeft) {
            log << data.value("glint0_x_position_left") << "," << data.value("glint0_y_position_left") << ",";
            log << data.value("glint1_x_position_left") << "," << data.value("glint1_y_position_left") << ",";
            log << data.value("glint2_x_position_left") << "," << data.value("glint2_y_position_left") << ",";
        }
        if (m_flag_pgLeft) {
            log << data.value("pg_x_left") << "," << data.value("pg_y_left") << ",";
        }
        if (m_flag_pupilSizeLeft) {
            log << data.value("pupil_width_left") << "," << data.value("pupil_height_left") << "," << data.value("pupil_angle_left") << ",";
        }
        if (m_flag_eyeFoundLeft) {
            log << data.value("eye_found_left") << ",";
        }

        if (m_flag_pogPosition3D) {
            log << data.value("pog_x_position_3d") << ",";
            log << data.value("pog_y_position_3d") << ",";
            log << data.value("pog_z_position_3d") << ",";
        }
        // Robot
        if (m_flag_psm1_pos) {
            log << data.value("psm1_x_pos") << ",";
            log << data.value("psm1_y_pos") << ",";
            log << data.value("psm1_z_pos") << ",";
        }
        if (m_flag_psm2_pos) {
            log << data.value("psm2_x_pos") << ",";
            log << data.value("psm2_y_pos") << ",";
            log << data.value("psm2_z_pos") << ",";

            log << data.value("psm2_qx") << ",";
            log << data.value("psm2_qy") << ",";
            log << data.value("psm2_qz") << ",";
            log << data.value("psm2_qw") << ",";

            log << data.value("psm2_proj_left_x") <<",";
            log << data.value("psm2_proj_left_y") <<",";
            log << data.value("psm2_proj_right_x") <<",";
            log << data.value("psm2_proj_right_y") <<",";
            log << data.value("gripper_cnt") <<",";

//            log << data.value("psm2_r00") << ",";
//            log << data.value("psm2_r01") << ",";
//            log << data.value("psm2_r02") << ",";
//            log << data.value("psm2_r10") << ",";
//            log << data.value("psm2_r11") << ",";
//            log << data.value("psm2_r12") << ",";
//            log << data.value("psm2_r20") << ",";
//            log << data.value("psm2_r21") << ",";
//            log << data.value("psm2_r22") << ",";
        }
        if (m_flag_psm3_pos) {
            log << data.value("psm3_x_pos") << ",";
            log << data.value("psm3_y_pos") << ",";
            log << data.value("psm3_z_pos") << ",";
        }
        log << data.value("is_looking") << ",";

        log << data.value("target_x") << ",";
        log << data.value("target_y") << ",";

        log << data.value("recalib0_left_x") << ",";
        log << data.value("recalib0_left_y") << ",";
        log << data.value("recalib0_right_x") << ",";
        log << data.value("recalib0_right_y") << ",";
        log << data.value("recalib1_left_x") << ",";
        log << data.value("recalib1_left_y") << ",";
        log << data.value("recalib1_right_x") << ",";
        log << data.value("recalib1_right_y") << ",";
        log << data.value("recalib2_left_x") << ",";
        log << data.value("recalib2_left_y") << ",";
        log << data.value("recalib2_right_x") << ",";
        log << data.value("recalib2_right_y");
        log << "\n";
        log.close();
        m_data.erase(m_data.begin());

        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    }
}

void GazeDataLogger::writeHeader(QString filename) {
    std::fstream log;
    log.open(filename.toStdString(), std::fstream::in | std::fstream::out | std::fstream::app);

    log << "Timestamp,";

    // Right
    if (m_flag_pupilPositionRight) {
        log << "pupil_x_position_right,pupil_y_position_right,";
    }
    if (m_flag_pogPositionRight) {
        log << "pog_x_position_right,pog_y_position_right,";
    }
    log <<"pog_x_correction_right,pog_y_correction_right,";
    log << "glint_index_right,";
    if (m_flag_glintPositionRight) {
        log << "glint0_x_position_right,glint0_y_position_right,";
        log << "glint1_x_position_right,glint1_y_position_right,";
        log << "glint2_x_position_right,glint2_y_position_right,";
    }
    if (m_flag_pgRight) {
        log << "pg_x_right,pg_y_right,";
    }
    if (m_flag_pupilSizeRight) {
        log << "pupil_width_right,pupil_height_right,pupil_angle_right,";
    }
    if (m_flag_eyeFoundRight) {
        log << "eye_found_right,";
    }

    // Left
    if (m_flag_pupilPositionLeft) {
        log << "pupil_x_position_left,pupil_y_position_left,";
    }
    if (m_flag_pogPositionLeft) {
        log << "pog_x_position_left,pog_y_position_left,";
    }
    log <<"pog_x_correction_left,pog_y_correction_left,";
    log << "glint_index_left,";
    if (m_flag_glintPositionLeft) {
        log << "glint0_x_position_left,glint0_y_position_left,";
        log << "glint1_x_position_left,glint1_y_position_left,";
        log << "glint2_x_position_left,glint2_y_position_left,";
    }
    if (m_flag_pgLeft) {
        log << "pg_x_left,pg_y_left,";
    }
    if (m_flag_pupilSizeLeft) {
        log << "pupil_width_left,pupil_height_left,pupil_angle_left,";
    }
    if (m_flag_eyeFoundLeft) {
        log << "eye_found_left,";
    }
    if (m_flag_pogPosition3D) {
        log<< "pog_x_position_3d,pog_y_position_3d,pog_z_position_3d,";
    }
    if (m_flag_psm1_pos) {
        log << "psm1_x,psm1_y,psm1_z,";
    }
    if (m_flag_psm2_pos) {
        log << "psm2_x,psm2_y,psm2_z,psm2_qx,psm2_qy,psm2_qz,psm2_qw,psm2_proj_left_x,psm2_proj_left_y,psm2_proj_right_x,psm2_proj_right_y,gripper_cnt,";
    }
    if (m_flag_psm3_pos) {
        log << "psm3_x,psm3_y,psm3_z,";
    }
    log << "is_looking,";
    log << "target_x,target_y,";
    log << "recalib0_left_x,recalib0_left_y,recalib0_right_x,recalib0_right_y,recalib1_left_x,recalib1_left_y,recalib1_right_x,recalib1_right_y,recalib2_left_x,recalib2_left_y,recalib2_right_x,recalib2_right_y";
    // TODO: For data collection, remove this or clean up
    log << "\n";
    log.close();
}

void GazeDataLogger::loadSettings() {

    QSettings settings(m_settingsFilename,QSettings::IniFormat);
    qDebug() << "Loading settings" << m_settingsFilename;
    m_logFileDirectory = settings.value("logfile_directory").toString();
    m_flag_pupilPositionRight = settings.value("pupil_position_right").toBool();
    m_flag_pogPositionRight = settings.value("pog_position_right").toBool();
    m_flag_glintPositionRight = settings.value("glint_position_right").toBool();
    m_flag_pgRight = settings.value("pg_right").toBool();
    m_flag_pupilSizeRight = settings.value("pupil_size_right").toBool();
    m_flag_eyeFoundRight = settings.value("eye_found_right").toBool();
    m_flag_pupilPositionLeft = settings.value("pupil_position_left").toBool();
    m_flag_pogPositionLeft = settings.value("pog_position_left").toBool();
    m_flag_glintPositionLeft = settings.value("glint_position_left").toBool();
    m_flag_pgLeft = settings.value("pg_left").toBool();
    m_flag_pupilSizeLeft = settings.value("pupil_size_left").toBool();
    m_flag_eyeFoundLeft = settings.value("eye_found_left").toBool();
    m_flag_psm1_pos = settings.value("robot_psm1_cartesian_position").toBool();
    m_flag_psm2_pos = settings.value("robot_psm2_cartesian_position").toBool();
    m_flag_psm3_pos = settings.value("robot_psm3_cartesian_position").toBool();
    m_flag_pogPosition3D = settings.value("pog_position_3d").toBool();
}