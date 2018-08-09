/*
GazeDataLogger.h

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


#ifndef GAZEDATALOGGER_H
#define GAZEDATALOGGER_H

#include <QString>
#include <fstream>
#include <QDebug>
#include <QHash>
#include <QDateTime>
#include <boost/thread.hpp>
#include <QSettings>
#include <QThread>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>

#include "UtilSocket.h"
#include "clientSocket.h"

class GazeDataLogger
{
public:    
    GazeDataLogger();
    ~GazeDataLogger();
    void logData( cv::RotatedRect& pupilRight, int glintIndexRight, cv::Vec2f& pgRight, cv::Point2f& pogRight, cv::Point2f& correctionRight, std::vector<cv::Point2f>& glintListRight, bool eyeFoundRight, \
                  cv::RotatedRect& pupilLeft,  int glintIndexLeft, cv::Vec2f& pgLeft,  cv::Point2f& pogLeft, cv::Point2f& correctionLeft, std::vector<cv::Point2f>& glintListLeft,  bool eyeFoundLeft, cv::Vec3f& pog3D,\
                  cv::Point2f& target, std::vector<cv::Point2f>& recalib, std::vector<float>& robotPose, std::vector<cv::Point2f>& pjt);
    void start();
    void stop();
    void setStatus(bool status);
    //clientSocket *mSocket;
    utilSocket* mSocket;

private:
    boost::thread m_thr;
    bool m_runFlag;
    bool m_flag_socket;
    QString m_settingsFilename;
    QString m_logFilename;
    std::vector<QHash<QString, double> > m_data;

    bool isLooking;
    // Settings
    boost::mutex m_runMutex;
    QString m_logFileDirectory;
    bool m_flag_pupilPositionRight;
    bool m_flag_pogPositionRight;
    bool m_flag_glintPositionRight;
    bool m_flag_pgRight;
    bool m_flag_pupilSizeRight;
    bool m_flag_eyeFoundRight;
    bool m_flag_pupilPositionLeft;
    bool m_flag_pogPositionLeft;
    bool m_flag_glintPositionLeft;
    bool m_flag_pgLeft;
    bool m_flag_pupilSizeLeft;
    bool m_flag_eyeFoundLeft;
    bool m_flag_psm1_pos;
    bool m_flag_psm2_pos;
    bool m_flag_psm3_pos;
    bool m_flag_pogPosition3D ;
    qint64 m_startTime;
    void loadSettings();
    void writeHeader(QString filename);
    void run();

//    utilSocket* mSocket;
};

#endif // GAZEDATALOGGER_H
