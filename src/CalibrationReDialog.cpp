/*
CalibrationReDialog.cpp

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

#include "CalibrationReDialog.h"

CalibrationReDialog::CalibrationReDialog(EyeTracker &tracker) : mTracker(&tracker)
{
    this->setWindowState(Qt::WindowFullScreen);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.9);
    this->move(1600,0);
    this->show();
    this->raise();

    this->setAutoFillBackground(true);
    QPalette pal = this->palette();
    int intensity = 150;
    pal.setColor(this->backgroundRole(),QColor(intensity,intensity,intensity,255));
    this->setPalette(pal);
    this->resize(640,480);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(calibration()));
    connect(this,SIGNAL(finished()),this,SLOT(finishCalibration()));
    initCalibration(1,10,20,0.1,0.1,0.9,800);
}

CalibrationReDialog::~CalibrationReDialog(void)
{
    qDebug() << "Exitting reCalibration dialog";
    timer->stop();
//    timer = NULL;
    this->close();
//    mTracker = NULL;
    this->deleteLater();
}


void CalibrationReDialog::initCalibration(int numberOfTargets,
                                          int small_radius,
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
    numTargets = numberOfTargets;

    switch (numTargets) {
    case(1):
        targetPosition = QPoint(50,50);
        currRadius = LARGE_RADIUS;
        currColor = Qt::red;
    }
    QPointF desPos = targetPosition;
    desPos.setX(this->width() * ( (float)desPos.x() / 100.0 ));
    desPos.setY(this->height() * ( (float)desPos.y() / 100.0 ));
    currPosition = desPos;
    mTracker->initReCalibration();
}

void CalibrationReDialog::startCalibration()
{
    timer->start(30);
    elapsedTimer.start();
}

void CalibrationReDialog::calibration()
{
    mTracker->startRecalibration(cv::Point2f(targetPosition.x(),targetPosition.y()));
    int elapsedms = elapsedTimer.elapsed();
    if (elapsedms > TIME_BUFFER){
        if (currRadius > 0.5*SMALL_RADIUS) {
            currRadius -= 0.01*SHRINK_RATE * (elapsedms-TIME_BUFFER);
        } else {
            currRadius = 0.5*SMALL_RADIUS;
        }
    }
    if (elapsedms > TIME_BUFFER + TIME_CALIBRATE) {
        mTracker->stopCalibrating();
        currRadius = LARGE_RADIUS;
        emit finished();
        timer->stop();
    }

    this->update();
}

void CalibrationReDialog::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    if (this->currColor == Qt::red) {
        p.setBrush(Qt::red);
    } else {
        p.setBrush(Qt::green);
    }
    p.setPen(Qt::NoPen);
    p.drawEllipse(currPosition,(int)currRadius,(int)currRadius);
    if (this->currColor == Qt::red) {
        p.setBrush(Qt::darkRed);
    } else {
        p.setBrush(Qt::darkGreen);
    }
    p.drawEllipse(currPosition,(int)(SMALL_RADIUS*0.5),(int)(SMALL_RADIUS*0.5));
}

void CalibrationReDialog::finishCalibration()
{
    if (mTracker->finalizeReCalibration(cv::Point2f(targetPosition.x(),targetPosition.y()),true)){
        qDebug()<< "Re-calibration finished";
    }
    else{
        qDebug()<< "Error in re-calibration";
    }
    this->~CalibrationReDialog();
}

/*
CalibrationReDialog.cpp

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

