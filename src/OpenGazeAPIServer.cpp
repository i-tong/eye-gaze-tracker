/*
OpenGazeAPIServer.cpp

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

#include "OpenGazeAPIServer.h"

OpenGazeAPIServer::OpenGazeAPIServer()
{
    _address = "127.0.0.1";
    _port = 4242;
    commands_map["ENABLE_SEND_DATA"] =       OpenGazeAPI::EnableSendData;
    commands_map["ENABLE_SEND_COUNTER"] =    OpenGazeAPI::EnableSendCounter;
    commands_map["ENABLE_SEND_TIME"] =       OpenGazeAPI::EnableSendTime;
    commands_map["ENABLE_SEND_TIME_TICK"] =  OpenGazeAPI::EnableSendTimeTick;
    commands_map["ENABLE_SEND_FRAME_CNT"] =  OpenGazeAPI::EnableSendFrameCount;
    commands_map["ENABLE_SEND_POG_FIX"] =    OpenGazeAPI::EnableSendPOGFix;
    commands_map["ENABLE_SEND_POG_LEFT"] =   OpenGazeAPI::EnableSendPOGLeft;
    commands_map["ENABLE_SEND_POG_RIGHT"] =  OpenGazeAPI::EnableSendPOGRight;
    commands_map["ENABLE_SEND_POG_BEST"] =   OpenGazeAPI::EnableSendPOGBest;
    commands_map["ENABLE_SEND_PUPIL_LEFT"] = OpenGazeAPI::EnableSendPupilLeft;
    commands_map["ENABLE_SEND_PUPIL_RIGHT"] = OpenGazeAPI::EnableSendPupilRight;
    commands_map["ENABLE_SEND_CURSOR"] =     OpenGazeAPI::EnableSendCursor;
    commands_map["ENABLE_SEND_BLINK"] =      OpenGazeAPI::EnableSendBlink;
    commands_map["ENABLE_SEND_USER_DATA"] =  OpenGazeAPI::EnableSendUserData;

    // Init data send flags to false
    for (unsigned int i = 0; i < commands_map.size(); i++) {
        state_flag.push_back(false);
    }

    _server = new QTcpServer();
    _xmlReader = new QXmlStreamReader();

    // Set up signals and slots
    qRegisterMetaType<rclgaze::OpenAPIDataRecord>("rclgaze::OpenAPIDataRecord");
    connect(_server,SIGNAL(newConnection()),this,SLOT(addClientConnection()));
    connect(this,SIGNAL(OPAPI_SendRecord_Callback(rclgaze::OpenAPIDataRecord)), \
            this, SLOT(OPAPI_SendRecord(rclgaze::OpenAPIDataRecord)));

     _count = 0; // Number of data packets sent

    _requestExists = false;

    this->StartListening();
}

OpenGazeAPIServer::~OpenGazeAPIServer() {
    for (unsigned i = 0 ; i < _socketVec.size(); i++) {
        _socketVec.at(i)->disconnect();
    }
}

void OpenGazeAPIServer::OPAPI_SendRecord(rclgaze::OpenAPIDataRecord rec) {
   // qDebug() << "OPAPI_SendRecord";
    this->sendRecord(rec);
}

void OpenGazeAPIServer::OPAPI_SendCalStart(int targetID, cv::Point2f target) {
    QString calstr = "<CAL ID=\"CALIB_START_PT\" ";
    calstr.append(QString("PT=\"%1\" ").arg(targetID));
    calstr.append(QString("CALX=\"%1\" ").arg(target.x));
    calstr.append(QString("CALY=\"%1\" ").arg(target.y));
    calstr.append("/>");

    this->sendReply(calstr);

}

void OpenGazeAPIServer::OPAPI_SendCalStop(int targetID, cv::Point2f target) {
    QString calstr = "<CAL ID=\"CALIB_RESULT_PT\" ";
    calstr.append(QString("PT=\"%1\" ").arg(targetID));
    calstr.append(QString("CALX=\"%1\" ").arg(target.x));
    calstr.append(QString("CALY=\"%1\" ").arg(target.y));
    calstr.append("/>");
    this->sendReply(calstr);
}

void OpenGazeAPIServer::streamData(rclgaze::OpenAPIDataRecord rec) {
    // This is a callback function to be called by the gaze tracker class.
    // A signal needs to be emitted because Qt sockets cannot be used by multiple threads.
    //qDebug() << "Stream data";
    if (state_flag.at(OpenGazeAPI::EnableSendData) == true) {
        emit OPAPI_SendRecord_Callback(rec);
    }
}

void OpenGazeAPIServer::sendRecord(rclgaze::OpenAPIDataRecord record) {
    qDebug() << "sendRecord";
    QString recstr = "<REC ";
    if (state_flag.at(OpenGazeAPI::EnableSendData) != true) {
         _count = 0;
        return;
    } else {
        // Counters
        if (state_flag.at(OpenGazeAPI::EnableSendCounter)) {
            recstr.append(QString("CNT=\"%1\" ").arg( _count));
        }
        if (state_flag.at(OpenGazeAPI::EnableSendTime)) {

            recstr.append(QString("TIME=\"%1\" ").arg(record.TIME/1000000,0,'f',6)); // Display in seconds, to microsecond precision
        }
        if (state_flag.at(OpenGazeAPI::EnableSendTimeTick)) {
            recstr.append(QString("TIME_TICK=\"%1\" ").arg(record.TIME_TICK));
        }
        if (state_flag.at(OpenGazeAPI::EnableSendFrameCount)) {
            recstr.append(QString("FRAME_CNT=\"%1\" ").arg(record.FRAME_CNT));
        }
        // Fixation POG
        if (state_flag.at(OpenGazeAPI::EnableSendPOGFix)) {
            recstr.append(QString("FPOGX=\"%1\" FPOGY=\"%2\" FPOGS=\"%3\" FPOGD=\"%4\" FPOGID=\"%5\" FPOGV=\"%6\" ").arg(record.FPOGX).arg(record.FPOGY).arg(record.FPOGS).arg(record.FPOGD).arg(record.FPOGID).arg(int(record.FPOGV)));
        }

        // Left Eye POG
        if (state_flag.at(OpenGazeAPI::EnableSendPOGLeft)) {
            recstr.append(QString("LPOGX=\"%1\" LPOGY=\"%2\" LPOGV=\"%3\" ").arg(record.LPOGX).arg(record.LPOGY).arg(int(record.LPOGV)));
        }

        // Right Eye POG
        if (state_flag.at(OpenGazeAPI::EnableSendPOGRight)) {
            recstr.append(QString("RPOGX=\"%1\" RPOGY=\"%2\" RPOGV=\"%3\" ").arg(record.RPOGX).arg(record.RPOGY).arg(int(record.RPOGV)));
        }

        // Best POG
        if (state_flag.at(OpenGazeAPI::EnableSendPOGBest)) {
            recstr.append(QString("BPOGX=\"%1\" BPOGY=\"%2\" BPOGV=\"%3\" ").arg(record.BPOGX).arg(record.BPOGY).arg(int(record.BPOGV)));
        }

        // Left Eye Pupil
        if (state_flag.at(OpenGazeAPI::EnableSendPupilLeft)) {
            recstr.append(QString("LPCX=\"%1\" LPCY=\"%2\" LPD=\"%3\" LPS=\"%4\" LPV=\"%5\" ").arg(record.LPCX).arg(record.LPCY).arg(record.LPD).arg(record.LPS).arg(int(record.LPV)));
        }

        // Right Eye Pupil
        if (state_flag.at(OpenGazeAPI::EnableSendPupilLeft)) {
            recstr.append(QString("RPCX=\"%1\" RPCY=\"%2\" RPD=\"%3\" RPS=\"%4\" RPV=\"%5\" ").arg(record.RPCX).arg(record.RPCY).arg(record.RPD).arg(record.RPS).arg(int(record.RPV)));
        }

        // Cursor position
        if (state_flag.at(OpenGazeAPI::EnableSendCursor)) {
            recstr.append(QString("CX=\"%1\" CY=\"%2\" CS=\"%3\" ").arg(record.CX).arg(record.CY).arg(record.CS));
        }

        // Blink
        if (state_flag.at(OpenGazeAPI::EnableSendBlink)) {
            recstr.append(QString("BKID=\"1\" BKDUR=\"0.00000\" BKPMIN=\"0\" "));
        }
        // User data
        /*if (state_flag.at(OpenGazeAPI::EnableSendUserData)) {
        recstr.append(QString("=\"%1\" ").arg(0));
        }*/

        recstr.append("/>");
        this->sendReply(recstr);
         _count += 1;
    }
}

void OpenGazeAPIServer::OPAPI_SendCalResult(int numPts, \
                                            std::vector<cv::Point2f> targetPos, \
                                            std::vector<cv::Point2f> leftGaze, \
                                            std::vector<cv::Point2f> rightGaze, \
                                            std::vector<bool> leftValid, \
                                            std::vector<bool> rightValid) {
    QString calstr = "<CAL ID=\"CALIB_RESULT\" ";
    // Sanity check

    if ((int)targetPos.size() != numPts || (int)leftGaze.size() != numPts \
            || (int)rightGaze.size() != numPts || (int)leftValid.size() != numPts || (int)rightValid.size() != numPts) {
        return;
    }

    for (int i = 0 ; i < numPts ; i++) {
        calstr.append(QString("CALX%1=\"%2\" ").arg(i).arg(targetPos.at(i).x));
        calstr.append(QString("CALY%1=\"%2\" ").arg(i).arg(targetPos.at(i).y));
        calstr.append(QString("LX%1=\"%2\" ").arg(i).arg(leftGaze.at(i).x));
        calstr.append(QString("LY%1=\"%2\" ").arg(i).arg(leftGaze.at(i).y));
        calstr.append(QString("LV%1=\"%2\" ").arg(i).arg(int(leftValid.at(i))));
        calstr.append(QString("RX%1=\"%2\" ").arg(i).arg(rightGaze.at(i).x));
        calstr.append(QString("RY%1=\"%2\" ").arg(i).arg(rightGaze.at(i).y));
        calstr.append(QString("RV%1=\"%2\" ").arg(i).arg(int(rightValid.at(i))));
    }

    calstr.append("/>");

    this->sendReply(calstr);
}

void OpenGazeAPIServer::OPAPI_ReplyValue(QString id, float value) {
    QString calstr = QString("<ACK ID=\"%1\" VALUE=\"%2\" />").arg(id).arg(value);
    qDebug() << "Reply value:" << calstr;
    this->sendReply(calstr);
}
void OpenGazeAPIServer::OPAPI_ReplyNoValue(QString id) {
    QString calstr = QString("<NACK ID=\"%1\" />").arg(id);
    qDebug() << calstr;
    this->sendReply(calstr);
}

void OpenGazeAPIServer::OPAPI_ReplyCameraSize(QString id, int width, int height) {
    QString calstr = QString("<ACK ID=\"%1\" WIDTH=\"%2\" HEIGHT=\"%3\" />").arg(id).arg(width).arg(height);
    qDebug() << "Reply value:" << calstr;
    this->sendReply(calstr);
}

void OpenGazeAPIServer::OPAPI_ReplyScreenSize(QString id, int x, int y, int width, int height) {
    QString calstr = QString("<ACK ID=\"%1\" X=\"%2\" Y=\"%3\" WIDTH=\"%4\" HEIGHT=\"%5\" />").arg(id).arg(x).arg(y).arg(width).arg(height);
    qDebug() << "Reply value:" << calstr;
    this->sendReply(calstr);
}

void OpenGazeAPIServer::OPAPI_ReplyCalibPoints(QString id, std::vector<cv::Point2f> points) {

    QString calstr = QString("<ACK ID=\"%1\" PTS=\"%2\" ").arg(id).arg(points.size());

    for (unsigned int i = 0 ; i < points.size() ; i++ ) {
        calstr.append(QString("X%1=\"%2\" Y%1=\"%3\" ").arg(i).arg(points.at(i).x).arg(points.at(i).y));
    }

    calstr.append("/>");
    this->sendReply(calstr);
}

void OpenGazeAPIServer::OPAPI_ReplyCalibSummary(QString id, float errLeft,float errRight, int numValidLeft, int numValidRight) {
    QString calstr = QString("<ACK ID=\"%1\" ").arg(id);

    calstr.append(QString("AVE_ERROR_LEFT=\"%1\" ").arg(errLeft));
    calstr.append(QString("AVE_ERROR_RIGHT=\"%1\" ").arg(errRight));

    calstr.append(QString("VALID_POINTS_LEFT=\"%1\" ").arg(numValidLeft));
    calstr.append(QString("VALID_POINTS_RIGHT=\"%1\" ").arg(numValidRight));

    calstr.append("/>");
    this->sendReply(calstr);

}
void OpenGazeAPIServer::StartListening() {
    _server->listen(QHostAddress(_address),_port);
    //qDebug() << "Listening";
}

void OpenGazeAPIServer::addClientConnection() {
    //qDebug() << "Adding new client!";

    _socketVec.push_back(_server->nextPendingConnection());

    connect(_socketVec.back(),SIGNAL(readyRead()),this,SLOT(msgReceived()));
    connect(_socketVec.back(),SIGNAL(disconnected()),this,SLOT(removeClientConnection()));

}

void OpenGazeAPIServer::removeClientConnection() {
    _mutex.lock();
    for (int i = int(_socketVec.size())-1 ; i >= 0; i--) {
        //qDebug() << "Removing client" << i;
        if (_socketVec.at(i)->state() != QTcpSocket::ConnectedState) {
            _socketVec.erase(_socketVec.begin() + i);
        }
    }
    _mutex.unlock();
}

void OpenGazeAPIServer::msgReceived() {
    //_mutex.lock();
    //qDebug() << "Message received!";
    for (unsigned int i = 0 ; i < _socketVec.size() ; i++ ) {
        QByteArray newdata;
        while(_socketVec.at(i)->bytesAvailable() > 0) {
            newdata.append(_socketVec.at(i)->readAll());
        }
        //m_socketVec.at(i)->flush();

        while (newdata.contains("\r\n")) {
            int endind = newdata.indexOf("\r\n") + 2;
            QByteArray xml_element = newdata.mid(0,endind);

            _xmlReader->addData(xml_element);

            qDebug() << xml_element;
            this->readXML();
            newdata.remove(0,endind);

        }
        /*if (m_socketVec.at(i)->canReadLine()) {
            char buf[1024];
            qint64 lineLength = _socketVec.at(i)->readLine(buf, sizeof(buf));
            _socketVec.at(i)->flush();
            if (lineLength != -1) {
                // Message received!
                qDebug() << buf;
                _xmlReader->addData(buf);
                this->readXML();
            }
        }*/
    }
    //_mutex.unlock();

}

void OpenGazeAPIServer::sendReply(QString msg) {
    //qDebug() << "sendReply" << msg;
    _mutex.lock();
    QByteArray data = msg.toUtf8();
    int msg_size = data.size();
    for (unsigned int i = 0 ; i < _socketVec.size() ; i++ ) {
       // qDebug() << "Sending reply:" << msg;
        int bytesWritten = 0;
        bool isConnected = (_socketVec.at(i)->state() == QTcpSocket::ConnectedState);
        while (bytesWritten < msg_size && isConnected) {
            bytesWritten += _socketVec.at(i)->write(data.data());
        }
        //qDebug() << "Done sending" << i;
    }
    _mutex.unlock();

}

void OpenGazeAPIServer::readXML() {
    // This function will parse the XML file, make sure it's been formatted correctly
    // and then emit a signal to the eye gaze tracker to act on the command.
    // Cannot parse multiple elements in the same document

    //qDebug() << "\nreadxml function";
    QXmlStreamReader::TokenType token = _xmlReader->readNext();

    while (token != QXmlStreamReader::EndElement ){
        // Element

        token = _xmlReader->readNext();
        // qDebug() << _xmlReader->text().toString() ;
        if (token == QXmlStreamReader::Invalid) {
           // qDebug() << "Error:" << _xmlReader->errorString();
        }
        // Attributes
        QXmlStreamAttributes att = _xmlReader->attributes();
        // Set
        if (att.size() < 1) { // No attributes
            //qDebug() << "No attributes";
            continue;
        }
        // qDebug() << att.value("ID").toString();
        if (att.value("ID").toString().contains("CALIBRATE")) {
            if ( _xmlReader->name().toString().compare("SET")==0) {
                qDebug() << "readXML" << "SET";
                if (att.hasAttribute("STATE")) {
                    emit OPAPI_SetValue(att.value("ID").toString(),att.value("STATE").toFloat());
                }
                if (att.hasAttribute("VALUE")) {
                    emit OPAPI_SetValue(att.value("ID").toString(),att.value("VALUE").toFloat());
                }
                if (att.hasAttribute("X") && att.hasAttribute(("Y"))) {
                    emit OPAPI_CalibrateAddpoint(att.value("X").toFloat(),att.value("Y").toFloat());
                }
            } else if ( _xmlReader->name().toString().compare("GET")==0) {
                // qDebug() << "readXML" << "GET";
                emit OPAPI_RequestValue(att.value("ID").toString());
            }
        }


        // qDebug() << att.value("ID");
        if (_xmlReader->name().toString().compare("SET")==0){
            if (att.value("ID").toString().contains("ENABLE")) { // ID: Enable (affects record streaming, no need to communicate this with gaze tracker.)
                state_flag.at(commands_map[att.value("ID").toString()]) = bool(att.value("STATE").toInt());
                sendReply(QString("<ACK ID=\"%1\" STATE=\"%2\"/>").arg(att.value("ID").toString()).arg(att.value("STATE").toString()));
            }
            if (att.value("ID").toString().contains("IMAGE_TX")) {
                //state_flag.at(commands_map[att.value("ID").toString()]) = bool(att.value("STATE").toInt());
                //sendReply(QString("<ACK ID=\"%1\" STATE=\"%2\"/>").arg(att.value("ID").toString()).arg(att.value("STATE").toString()));
            }
        } else if (_xmlReader->name().toString().compare("GET")==0) {
            emit OPAPI_RequestValue(att.value("ID").toString());
        }

    }


    // Send acknowledgement

    //this->sendReply(QString().arg());

    _xmlReader->clear();
   // qDebug() << "End readXML function";
}

void OpenGazeAPIServer::formatXML() {

}

void OpenGazeAPIServer::handleCalibrationRequest(QXmlStreamAttributes att , OpenGazeAPI::request request) {
    if (this->_requestList.size() > 0) {
        qDebug() << "Previous request has not yet been handled";
        // Should I wait here?
    }
    if (att.value("ID").toString().contains("CALIBRATE_START")) {
        qDebug() << " OPAPI Start Calibration";
        emit OPAPI_StartCalibration(bool(att.value("VALUE").toInt()),request);

    }
    if (att.value("ID").toString().contains("CALIBRATE") ) {

    }

    this->_requestList.push_back(att);
}

void OpenGazeAPIServer::respondStartCalibration(bool start) {
	(void)start;
}

// Public functions
void OpenGazeAPIServer::setHostAddress(QString ip) {
    _address = ip;
}

void OpenGazeAPIServer::setPort(int port) {
    _port = port;
}
