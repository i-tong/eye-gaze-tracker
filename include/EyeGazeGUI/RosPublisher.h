/*
RosPublisher.h

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




#ifndef ROSPUBLISHER_H
#define ROSPUBLISHER_H

#include "GazeTracker.h"
#include "Util.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <QTimer>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>

/**
 * @brief RosPublisher class, publishes eye gaze info in format of JointState
 *
 */
class RosPublisher : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief default constructor
     *
     * @param parent
     */
    RosPublisher(QObject *parent = 0);

    /**
     * @brief
     *
     */
    ~RosPublisher();

    /**
     * @brief start eye gaze streaming
     *
     * @param m_tracker
     */
    void startGazePositionStream(GazeTracker* _tracker);

    /**
     * @brief stop eye gaze streaming
     *
     */
    void stopGazePositionStream();

protected:
    GazeTracker* _tracker; // reference to eye gaze tracker
    cv::Point2f	_pogRight; // pog of right eye
    cv::Point2f	_pogLeft; // pog of left eye
    cv::Point2f _pogAvg; // filtered avg pog of left and right eyes
    QTimer *_p_timer; // timer that continuously publish pog
    sensor_msgs::JointState _msg;
    ros::Publisher _pub; // ros publisher
    ros::NodeHandle _n; // ros node

public slots:
    /**
     * @brief publish eye gaze position
     *
     */
    void streamGazePosition();
};

#endif // ROSPUBLISHER_H
