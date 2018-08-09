/*
OpenGazeAPIServer.h

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


#ifndef OPENGAZEAPISERVER_H
#define OPENGAZEAPISERVER_H

/*!
 * \brief The OpenGazeAPIServer class uses the Open Gaze API protocol defined by Gazepoint.
 *
 * This class defined a server which communicates via the Open Gaze API protocol. This enables external
 * applications to communicate with the eye gaze tracker.
 * The Open Gaze API is documented here: http://www.gazept.com/dl/Gazepoint_API_v2.0.pdf
 *
 */

#include <QTcpServer>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QXmlStreamReader>
#include <QDebug>
#include <QMutex>
#include <QMetaType>

#include <opencv2/opencv.hpp>

//#include <QXmlDeclHandler>
#include "EyeGazeCommon.h"
#include <map>
#include <time.h>

/*!
 * \brief The OpenAPIDataRecord struct holds data to be send as a data record.

 From the Gazepoint Open Gaze API Documentation:

Data is transmit from the server to the client in a sequence of records formatted as XML strings. A data
record is identified with the <REC /> tag. An example for the data record counter would look as follows:

<REC CNT="1484" />

 In this example there is one data field CNT with a value of 1484.

 This class handles all socket communication. It also handles XML decoding, and encoding data in XML.


 */


namespace OpenGazeAPI {

/*!
  \brief The state enum describes the different data elements to stream
 */
enum state {
    EnableSendData,
    EnableSendCounter,
    EnableSendTime,
    EnableSendTimeTick,
    EnableSendFrameCount,
    EnableSendPOGFix,
    EnableSendPOGLeft,
    EnableSendPOGRight,
    EnableSendPOGBest,
    EnableSendPupilLeft,
    EnableSendPupilRight,
    EnableSendCursor,
    EnableSendBlink,
    EnableSendUserData

};

/*!
 * \brief The calib_request enum describes different commands and requests handled via the API regarding calibration
 */
enum request {
    CalibrateStart,
    CalibrateShow,
    CalibrateTimeout,
    CalibrateDelay,
    CalibrateResultsSummary,
    CalibrateClear,
    CalibrateReset,
    CalibrateAddPoint

};


}


class OpenGazeAPIServer : public QObject
{
    Q_OBJECT

public:
    OpenGazeAPIServer();
    ~OpenGazeAPIServer();
    void StartListening();


    void setHostAddress(QString ip);
    void setPort(int port);
    void streamData(rclgaze::OpenAPIDataRecord rec);


signals:
    // Calibration related signals
    void OPAPI_StartCalibration(bool start, OpenGazeAPI::request request);
    void OPAPI_RequestValue(QString id);
    void OPAPI_SetValue(QString id, float value);
    void OPAPI_CalibrateAddpoint(float X, float Y);
    // Used as a callback function for streaming record data
    void OPAPI_SendRecord_Callback(rclgaze::OpenAPIDataRecord);

public slots:
    void OPAPI_SendRecord(rclgaze::OpenAPIDataRecord);


    void OPAPI_SendCalRecord(cv::Point target);

    void OPAPI_SendCalResult(int numPts, \
                             std::vector<cv::Point2f> targetPos, \
                             std::vector<cv::Point2f> leftGaze, \
                             std::vector<cv::Point2f> rightGaze, \
                             std::vector<bool> leftValid, \
                             std::vector<bool> rightValid);

    void OPAPI_SendCalStart(int targetID, cv::Point2f target);
    void OPAPI_SendCalStop(int targetID, cv::Point2f target);
    void OPAPI_ReplyValue(QString id, float value);
    void OPAPI_ReplyCameraSize(QString id, int width, int height);
    void OPAPI_ReplyScreenSize(QString id, int x, int y, int width, int height);
    void OPAPI_ReplyCalibPoints(QString id, std::vector<cv::Point2f> points);
    void OPAPI_ReplyNoValue(QString id);
    void OPAPI_ReplyCalibSummary(QString id, float errLeft,float errRight, int numValidLeft, int numValidRight);
private slots:
    void addClientConnection();
    void removeClientConnection();
    void msgReceived();
    void respondStartCalibration(bool start);

private:
    // Data record booleans
    std::vector<bool> state_flag;
    QMutex _mutex;
    std::map<QString, int> commands_map;
    // Custom data records, only works if m_sendUserData is enabled
    bool m_sendPSM1Data;
    bool m_sendPSM2Data;
    bool m_sendPSM3Data;
    bool m_sendECMData;

    QTcpServer* m_server;
    QXmlStreamReader* m_xmlReader;
    std::vector<QTcpSocket*> m_socketVec;

    void readXML();
    void formatXML();
    void sendReply(QString msg);
    void sendRecord(rclgaze::OpenAPIDataRecord record);
    void sendCalibrationRecord();
    void handleCalibrationRequest(QXmlStreamAttributes att, OpenGazeAPI::request request);

    int count;

    bool m_requestExists;
    std::vector<QXmlStreamAttributes> m_requestList;
};

#endif // OPENGAZEAPISERVER_H
