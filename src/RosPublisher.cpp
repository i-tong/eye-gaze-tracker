/*
RosPublisher.cpp

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




#include "RosPublisher.h"

RosPublisher::RosPublisher(QObject *parent) :
    QObject(parent)
{
    _p_timer = new QTimer(this);
    connect(_p_timer,SIGNAL(timeout()),this,SLOT(streamGazePosition()));

    _pub = _n.advertise<sensor_msgs::JointState>("gazePosition", 1000);
}

RosPublisher::~RosPublisher()
{
    _p_timer->stop();
    delete _p_timer;
}

void RosPublisher::startGazePositionStream(GazeTracker* tracker)
{
    qDebug() << "Start streaming";
    _tracker = tracker;

    _p_timer->start(10); //stream position at 100 Hz
}

void RosPublisher::stopGazePositionStream()
{
    qDebug() << "Stop streaming";
    _p_timer->stop();
}

void RosPublisher::streamGazePosition()
{
    bool rightgood = _tracker->getPOG(_pogRight,RIGHT_EYE);
    bool leftgood = _tracker->getPOG(_pogLeft,LEFT_EYE);
    if (rightgood && leftgood) {
    _pogAvg.x = (_pogRight.x + _pogLeft.x)/2;
    _pogAvg.y = (_pogRight.y + _pogLeft.y)/2;
    }
    else if (rightgood) {
        _pogAvg.x = _pogRight.x;
        _pogAvg.y = _pogRight.y;
    } else if (leftgood) {
        _pogAvg.x = _pogLeft.x;
        _pogAvg.y = _pogLeft.y;
    }
    if (_pogAvg.x && _pogAvg.y){
        _msg.position = {_pogAvg.x, _pogAvg.y};
        _pub.publish(_msg);
    }
}
