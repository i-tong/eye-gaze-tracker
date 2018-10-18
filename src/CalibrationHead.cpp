/*
CalibrationHead.cpp

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

#include "CalibrationHead.h"

/*!
 * \brief constructor for HEAD calibration objector
 *
 * \param tracker eye gaze tracker that is running
 */
CalibrationHead::CalibrationHead(EyeTracker &tracker) : _tracker(&tracker)
{
    this->setWindowState(Qt::WindowFullScreen);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowIcon(QIcon(":/Icons/Resources/cgaze_icon.png"));

    this->setWindowOpacity(1.0);

    // Setup background colour
    this->setAutoFillBackground(true);
    QPalette pal = this->palette();
    int intensity = 150;
    pal.setColor(this->backgroundRole(),QColor(intensity,intensity,intensity,255));
    this->setPalette(pal);

    _paintTimer = new QTimer(this);
    connect(_paintTimer, SIGNAL(timeout()), this, SLOT(update()));
}

/*!
 * \brief
 *
 */
CalibrationHead::~CalibrationHead(void)
{
    qDebug() << "Exitting HEAD movement calibration dialog";
    this->close();
    this->deleteLater();
}

/*!
 * \brief initialize calibration variables
 *
 * \param small_radius  the smallest radius that the calibration circle shrinks
 * \param large_radius  the largest radius that the calibration circle grows
 * \param shrink_rate   pixels per millisecond
 * \param grow_rate     pixels per millisecond
 * \param move_rate     pixels per millisecond
 * \param time_calibrate   length of data collection period in ms
 */
void CalibrationHead::initCalibration(int small_radius,
                                      int large_radius,
                                      float shrink_rate,
                                      float grow_rate,
                                      float move_rate,
                                      int time_calibrate)
{
    // Set calibration parameters
    SMALL_RADIUS = small_radius;
    LARGE_RADIUS = large_radius;
    SHRINK_RATE = shrink_rate;
    GROW_RATE = grow_rate;
    MOVE_RATE = move_rate;
    TIME_CALIBRATE = time_calibrate;
    TIME_BUFFER = 1000;

    _targetPosition = QPoint(50,50);
    _currRadius = LARGE_RADIUS;
    _currColor = Qt::green;

    QPointF desPos = _targetPosition;
    desPos.setX(this->width() * ( (float)desPos.x() / 100.0 ));
    desPos.setY(this->height() * ( (float)desPos.y() / 100.0 ));
    _currPosition = desPos;
    _tracker->initReCalibration();
    _paintTimer->start(30);
}


void CalibrationHead::startCalibrationGuide(int index) {
    this->show();
    this->raise();
    _headIndex = index;
    _currState = GUIDE;
}

/*!
 * \brief start Head calibration
 *
 * \param headIdx current index of head position
 */
void CalibrationHead::startCalibration(int index)
{
    _tracker->initReCalibration(false);
    _headIndex = index;
    this->show();
    this->raise();
    _currState = CALIBRATE;
    _calibTimer.start();
    _tracker->startRecalibration(cv::Point2f(_targetPosition.x(),_targetPosition.y()));
}

/*!
 * \brief calibration, connected to timer "timeOut" signal
 *
 */
void CalibrationHead::calibration()
{
    // Called every frame upon timer elapsed during calibration
    int elapsedms = _calibTimer.elapsed();
    if (elapsedms > TIME_BUFFER){
        _currColor = Qt::red;
        if (_currRadius > 0.5*SMALL_RADIUS) {
            _currRadius -= 0.01*SHRINK_RATE * (elapsedms-TIME_BUFFER);
        } else {
            _currRadius = 0.5*SMALL_RADIUS;
        }
    }
    if (elapsedms > TIME_BUFFER + TIME_CALIBRATE) { // Stop calibration

        // Stop collecting data

        // Calculate HEAD compensation at this direction
        bool success = _tracker->stopReCalibration(cv::Point2f(_targetPosition.x(),_targetPosition.y()),_headIndex);
        if (success == false) {
            qDebug()<< "Error in this head-movement calibration";
        }

        emit target_finished();
        _currRadius = LARGE_RADIUS;
        _currColor = Qt::green;
        _currState = GUIDE;
    }
    this->update();
}

/*!
 * \brief paint even for drawing the circles
 *
 * \param default function call from Qt
 */
void CalibrationHead::paintEvent(QPaintEvent *) {

    if (_currState == CALIBRATE) {
        this->calibration();
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        if (this->_currColor == Qt::red) {
            p.setBrush(Qt::red);
        } else {
            p.setBrush(Qt::green);
        }
        p.setPen(Qt::NoPen);
        p.drawEllipse(_currPosition,(int)_currRadius,(int)_currRadius);
        if (this->_currColor == Qt::red) {
            p.setBrush(Qt::darkRed);
        } else {
            p.setBrush(Qt::darkGreen);
        }
        p.drawEllipse(_currPosition,(int)(SMALL_RADIUS*0.5),(int)(SMALL_RADIUS*0.5));
    }
    if (_currState == GUIDE) {
        bool gazeok = false;
        cv::Point2f pog = cv::Point2f(0,0);
        gazeok = _tracker->getPOG(pog,rclgaze::BOTH_EYES);
        if (!gazeok) {
            gazeok = _tracker->getPOG(pog,rclgaze::RIGHT_EYE);
        }
        if (!gazeok) {
            gazeok = _tracker->getPOG(pog,rclgaze::LEFT_EYE);

        }
        if (!gazeok) {
            return;
        }
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        p.setBrush(Qt::lightGray);
        p.drawEllipse(QPoint(int(this->width()/2),int(this->height()/2)),40,40);

        p.setBrush(Qt::blue);
        p.setPen(Qt::NoPen);
        QPoint pos = QPoint(int(this->width() * pog.x/100.0),int(this->height() * pog.y/100.0));
        switch (_headIndex) {
        case 1: // left
            pos.setX(pos.x() + int(this->width()/4));
            break;
        case 2: //right
            pos.setX(pos.x() - int(this->width()/4));
            break;
        case 3: //up
            pos.setY(pos.y() + int(this->height()/4));
            break;
        case 4: // down
            pos.setY(pos.y() - int(this->height()/4));
            break;
        }

        p.drawEllipse(pos,20,20);

    }

}

/*!
 * \brief finishCalibration and call finalizeCalibration to pass the calibration target to
 * eye gaze tracker, and tracker will process the data. Connected to "finished" signal
 *
 */
void CalibrationHead::finishCalibration()
{

    _tracker->finalizeReCalibration();
    _headIndex = 0;
    this->close();
}

/*!
  * \brief Sets the monitor to display head claibration
  *
  * \param monitorId monitor ID index
  */
void CalibrationHead::setMonitor(int monitorId) {
    QDesktopWidget* deskWin = QApplication::desktop();
    QRect monitorRect = deskWin->availableGeometry(monitorId);
    this->move(QPoint(QPoint(monitorRect.x(),monitorRect.y())));

}
