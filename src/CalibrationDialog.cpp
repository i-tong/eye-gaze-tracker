/*
CalibrationDialog.cpp

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

#include "CalibrationDialog.h"

/*!
 * \brief CalibrationDialog::CalibrationDialog Constructor for the CalibrationDialog class.
 *
 * This class is the dialog which displays the calibration target animation. It synchronizes with
 * the gaze tracker when collecting data points during calibration.
 */
CalibrationDialog::CalibrationDialog()
{
    this->setWindowState(Qt::WindowFullScreen);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowIcon(QIcon(":/Icons/Resources/cgaze_icon.png"));

    // Setup background colour
    this->setAutoFillBackground(true);
    QPalette pal = this->palette();
    int intensity = 150;
    pal.setColor(this->backgroundRole(),QColor(intensity,intensity,intensity,255));
    this->setPalette(pal);
    //this->resize(640,480);

    _is_external_display = false;
    _external_height = 100;
    _external_width = 100;

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(changeTarget()));

}

/*!
 * \brief CalibrationDialog::~CalibrationDialog Destructor for the calibration class. Cleans up pointers.
 */
CalibrationDialog::~CalibrationDialog(void)
{
    delete _timer;
    _p_tracker = NULL;
    delete _p_tracker;
}

/**
 * @brief initialize calibration parameters, set the passed parameters,
 * initialize _curr_target = 0,
 * calibration state = START,
 * current radius to maximum,
 * set cursor position to the center of the screen,
 * initialize a vector of target point positions (_target_positions) based on number of target
 * find the direction of movement
 *
 * @param numberOfTargets   number of targets to be calibrated, currently support 5, 9, 16 points
 * @param small_radius      smallest radius that calibration targets shrink to
 * @param large_radius      largest radius that calibration targets grow to
 * @param shrink_rate       the speed of shrinking in px/ms
 * @param grow_rate         the speed of growing in px/ms
 * @param move_rate         the speed of moving in px/ms
 * @param time_hold         the delay at each target position prior to calibration
 * @param time_calibrate    the active time of data collection
 * @param side              side of eyes, left or right
 * @param tracker           gaze tracker object
 */
void CalibrationDialog::initCalibration(int numberOfTargets,int small_radius, int large_radius, float shrink_rate, float grow_rate,
                                        float move_rate, int time_hold, int time_calibrate,rclgaze::eyeSide side, EyeTracker &tracker, \
                                        bool externalDisplay, int externalWidth, int externalHeight) {

    _side_to_calibrate = side;
    _p_tracker = &tracker;

    // Set calibration parameters
    _small_radius = small_radius;
    _large_radius = large_radius;
    _shrink_rate = shrink_rate;
    _grow_rate = grow_rate;
    _move_rate = move_rate;
    _time_calibrate = time_calibrate;
    _time_hold = time_hold;
    _curr_target = 0;
    _num_targets = numberOfTargets;
    state = STOP;
    _curr_radius = _large_radius;

    this->setNumTargets(numberOfTargets);

    _curr_position.setX(this->width() * 0.5 );
    _curr_position.setY(this->height() * 0.5 );

    // Specify current direction
    QPointF desPos = _target_positions.at(_curr_target);
    desPos.setX(this->width() * ( (float)desPos.x() / 100.0 ));
    desPos.setY(this->height() * ( (float)desPos.y() / 100.0 ));
    findVecDirection(_curr_position, desPos, _curr_direction);

    if (externalDisplay == true) {
        _is_external_display = true;
        _external_height = externalHeight;
        _external_width = externalWidth;

        // Shrink the calibration x field of view
        for (int i = 0 ; i < _target_positions.size() ; i++) {
            float x = _target_positions.at(i).x();
            if (x <= 20) {
                x = 30;
            }
            if (x >= 80) {
                x = 70;
            }
            _target_positions.at(i).setX(x);
        }

    }
}

/**
 * @brief start calibration by start QTimer (continuously check/change calibration state) and ElaspedTimer (keep track of time passed), record
 * the start time of holding (holdTimerStart)
 */
void CalibrationDialog::startCalibration() {
    _p_tracker->initCalibration(_target_positions.size(), _side_to_calibrate);

    _curr_target = 0;
    state = START;
    _curr_radius = _large_radius;
    _curr_position.setX(this->width() * 0.5 );
    _curr_position.setY(this->height() * 0.5 );

    QPointF desPos = _target_positions.at(_curr_target);
    desPos.setX(this->width() * ( (float)desPos.x() / 100.0 ));
    desPos.setY(this->height() * ( (float)desPos.y() / 100.0 ));
    findVecDirection(_curr_position, desPos, _curr_direction);

    _timer->start(30);
    _elapsed_timer.start();
    _hold_time_start = _elapsed_timer.elapsed();

}

/*!
 * \brief CalibrationDialog::stopCalibration stops calibration by setting state to STOP
 *
 */
void CalibrationDialog::stopCalibration() {
    state = STOP;

}

/*!
 * \brief CalibrationDialog::changeTarget Called at timer timeout, directs the current
 * state of the calibration target, and calls the appropriate function to calculate the
 * target behaviour.
 */
void CalibrationDialog::changeTarget() { // called for timer timeout
    int ms = _elapsed_timer.elapsed();

    switch (state) {
    case (START):
        // Wait for 1.5 second
        if (ms - _hold_time_start > _time_hold) {
            state = MOVE;
        }
        break;
    case (HOLD):
        this->_holdTarget(ms, _time_hold);
        break;
    case (SHRINK):
        this->_shrinkTarget(ms);
        break;
    case (GROW):
        this->_growTarget(ms);
        break;
    case (MOVE):
        this->_moveTarget(ms);
        break;
    case (CALIBRATE):
        this->_calibrateTarget(ms);
        break;
    case (FIN):
        break;
    case (STOP):
        break;
    }
    repaint();
    if (state != CALIBRATE && state != STOP && state != START && state != HOLD) {
        _elapsed_timer.restart();
    }
}

/*!
 * \brief CalibrationDialog::holdTarget hold target in position for certain duration to help user fixate
 * \param elapsedms current elasped time in ms
 * \param holdTime time to hold in ms
 */
void CalibrationDialog::_holdTarget(int elapsedms, int holdTime) {
    if (elapsedms - _hold_time_start > holdTime) {
        _curr_color = Qt::red;
        state = SHRINK;
    }
}

/*!
 * \brief CalibrationDialog::moveTarget move target to the destination (extracted from the _target_positions variable), the distance to travel from current position is calculated by move_rate * elapsedms
 * \param elapsedms elapsed time
 */
void CalibrationDialog::_moveTarget(int elapsedms) {

    QPointF desPos = _target_positions.at(_curr_target);
    int ms = _elapsed_timer.elapsed();
    desPos.setX(this->width() * ( (float)desPos.x() / 100.0 ));
    desPos.setY(this->height() * ( (float)desPos.y() / 100.0 ));

    QPointF diff = desPos - _curr_position;
    float D = diff.manhattanLength();
    float d = _move_rate * float(elapsedms);
    QPointF newDirection;
    findVecDirection(_curr_position, desPos, newDirection);
    if ( abs(D) > 10 && newDirection == _curr_direction) {
        int newx = _curr_position.x() + (float)diff.x() * (d/D);
        int newy = _curr_position.y() + (float)diff.y() * (d/D);
        _curr_position.setX(newx);
        _curr_position.setY(newy);

    } else {

        _curr_position.setX(desPos.x());
        _curr_position.setY(desPos.y());

        // Change state
        _hold_time_start = _elapsed_timer.elapsed();
        state = HOLD;
    }
}

/*!
 * \brief grow target to LARGE_RADIUS, the current radius is calculated by _curr_radius + grow_rate * elapsedms
 *
 * \param elapsedms elapsed time
 */
void CalibrationDialog::_growTarget(int elapsedms) {
    if ( _curr_radius < _large_radius ) {

        _curr_radius += _grow_rate * elapsedms;
    } else {
        _curr_radius = _large_radius;
        _curr_target += 1;
        if (_curr_target == _num_targets) {
            // Calibration is finished
            state = FIN;
            _p_tracker->finalizeCalibration(_side_to_calibrate);
            emit CalibrationFinished();

        } else {
            // Calibration is not yet finished, specify current direction
            QPointF desPos = _target_positions.at(_curr_target);
            desPos.setX(this->width() * ( (float)desPos.x() / 100.0 ));
            desPos.setY(this->height() * ( (float)desPos.y() / 100.0 ));
            findVecDirection(_curr_position, desPos, _curr_direction);

            // Change state
            state = MOVE;
        }
    }
}

/*!
 * \brief shrink target to LARGE_RADIUS, radius is calculated by _curr_radius - shrink_rate * elapsedms
 *
 * \param elapsedms elapsed time
 */
void CalibrationDialog::_shrinkTarget(int elapsedms) {
    // Change state
    _elapsed_timer.restart();
    state = CALIBRATE;
}


/*!
 * \brief calibrate current target, tell tracker to collect data at target point
 *
 * \param elaspedms total elapsed time
 */
void CalibrationDialog::_calibrateTarget(int elapsedms) {

    _p_tracker->startCalibrating(_curr_target, _target_positions.at(_curr_target).x(),_target_positions.at(_curr_target).y());

    if (_curr_radius > 0.5*_small_radius) {
        _curr_radius -= 0.01*_shrink_rate * elapsedms;
    } else {
        _curr_radius = 0.5*_small_radius;
    }

    if (_elapsed_timer.elapsed() < _time_calibrate) {
    } else {
        _p_tracker->stopCalibrating();
        _curr_color = Qt::green;
        state = GROW;
    }
}

/**
 * \brief paint event for drawing
 * \param
 */
void CalibrationDialog::paintEvent(QPaintEvent *) {
    cv::Point2f leftPOG = cv::Point2f(-10,-10);
    cv::Point2f rightPOG = cv::Point2f(-10,-10);
    cv::Point2f bestPOG = cv::Point2f(-10,-10);


    if (state != FIN) { // Show calibration sequence
        this->setWindowOpacity(1.0);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        if (this->_curr_color == Qt::red) {
            p.setBrush(Qt::red);
        } else {
            p.setBrush(Qt::green);
        }
        p.setPen(Qt::NoPen);
        p.drawEllipse(_curr_position,(int)_curr_radius,(int)_curr_radius);
        if (this->_curr_color == Qt::red) {
            p.setBrush(Qt::darkRed);
        } else {
            p.setBrush(Qt::darkGreen);
        }
        p.drawEllipse(_curr_position,(int)(_small_radius*0.5),(int)(_small_radius*0.5));
    }
    else { // Draw eye gaze position
        bool rok = _p_tracker->getPOG(rightPOG,rclgaze::RIGHT_EYE);
        bool lok = _p_tracker->getPOG(leftPOG, rclgaze::LEFT_EYE);
        bool bok = _p_tracker->getPOG(bestPOG, rclgaze::BOTH_EYES);

        this->setWindowOpacity(0.5);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Draw test background
        p.setPen(Qt::black);

        float border = this->height() / 5;
        float dx = (this->width()-2*border)/2;
        float dy = (this->height()-2*border)/2;
        int numRows = 3;
        int numCols = 3;
        for ( int i = 0 ; i < numRows ; i++ ) {
            for ( int j = 0 ; j < numCols ; j++ ) {
                QPoint newPoint(border+i*dx, border+j*dy);
                p.setBrush(Qt::white);
                p.drawEllipse(newPoint, 50, 50);
                p.setBrush(Qt::black);
                p.drawEllipse(newPoint, 2, 2);
            }
        }

        // Draw gaze point

        float pXR, pYR, pXL, pYL, pX, pY = 0;

        pXR = this->width()  * (rightPOG.x / 100.0);
        pYR = this->height() * (rightPOG.y / 100.0);
        pXL = this->width()  * (leftPOG.x / 100.0);
        pYL = this->height() * (leftPOG.y / 100.0);
        pX =  this->width()  * (bestPOG.x / 100.0);
        pY =  this->height() * (bestPOG.y / 100.0);

        if (!std::isnan(pX) && !std::isnan(pY)) {
            p.setBrush(Qt::magenta);
            p.setPen(Qt::NoPen);
            p.drawEllipse(QPointF(pX,pY),_small_radius,_small_radius);
        }
        if (!std::isnan(pXR) && !std::isnan(pYR)) {
            p.setBrush(Qt::blue);
            p.drawEllipse(QPointF(pXR,pYR),_small_radius,_small_radius);
        }
        if (!std::isnan(pXL) && !std::isnan(pYL)) {
            p.setBrush(Qt::red);
            p.drawEllipse(QPointF(pXL,pYL),_small_radius,_small_radius);
        }
    }

    if (_is_external_display) {
        float xratio = 0;
        float yratio = 0;
        float rad = 0;
        if (state!=FIN) {
            xratio = _curr_position.x() / this->width();
            yratio = _curr_position.y() / this->height();
            rad = _curr_radius;
        } else {
            xratio = bestPOG.x / 100.0;
            yratio = bestPOG.y / 100.0;
        }
        QColor col;
        if (_curr_color == Qt::red) {
            col = Qt::red;
        } else if (_curr_color == Qt::green) {
            col = Qt::green;
        }
        emit ShowFrame(xratio,yratio, rad, col);

    }

}

/*!
 * \brief check if calibration is currently running
 *
 * \return bool true if state= FIN, false otherwise
 */
bool CalibrationDialog::isCalibrationRunning() {
    if (state == FIN || state == STOP) {
        return false;
    } else {
        return true;
    }
}

/*!
 * \brief add a calibration point at specific point
 *
 * \param x   calibration point position as percentage of screen width (0 - 1)
 * \param Y   calibration point position as percentage of screen height (0 - 1)
 *
 * \return bool true if point can be added, false otherwise
 */
bool CalibrationDialog::addCalibrationPoint(float calibration_point_x, float calibration_point_y)
{
    if (calibration_point_x < 0 || calibration_point_x > 100 || calibration_point_y < 0 || calibration_point_y >100) {
        return false;
    }
    else{
        // TODO: check if the same position has been added
        //        for(auto& position: _target_positions) {

        //        }
        _target_positions.push_back(QPointF(calibration_point_x,calibration_point_y));
        return true;
    }
}

/*!
 * \brief getCalibrationTimeout Returns the time duration that the target is calibrating in ms.
 * \return Time duration in ms
 */
float CalibrationDialog::getCalibrationTimeout() {
    return _time_calibrate;
}

/*!
 * \brief CalibrationDialog::getCalibrationDelay Returns the delay before starting target calibration in ms.
 * \return Time duration in ms
 */
float CalibrationDialog::getCalibrationDelay(){
    return _time_hold;
}

/*!
 * \brief CalibrationDialog::get_num_targets Returns the number of different calibration target positions.
 * \return Count of calibration target positions
 */
float CalibrationDialog::getNumTargets() {
    return _target_positions.size();
}

/*!
 * \brief CalibrationDialog::getCalibrationPoints Returns the calibration target positions.
 * \param pos Vector of cv::Point2f describing the calibration target positions.
 */
void CalibrationDialog::getCalibrationPoints(std::vector<cv::Point2f>& pos) {
    pos.clear();
    for (unsigned int i = 0 ; i < _target_positions.size() ; i++) {
        pos.push_back(cv::Point2f(_target_positions.at(i).x(),_target_positions.at(i).y()));
    }
}

/*!
 * \brief CalibrationDialog::set_num_targets Sets the number of targets to 0, 5, 9, 16
 * \param _num_targets Number of desired targets.
 */
void CalibrationDialog::setNumTargets(int _num_targets) {
    // TODO: It would be nice if the user could set custom target positions
    _target_positions.clear();
    switch (_num_targets) {
    case (0):
        _target_positions.clear();
        break;
    case (5):
        _target_positions.push_back(QPointF( 20,30));
        _target_positions.push_back(QPointF( 80,30));
        _target_positions.push_back(QPointF( 50,50));
        _target_positions.push_back(QPointF( 20,70));
        _target_positions.push_back(QPointF( 80,70));
        break;

    case (9):
        _target_positions.push_back(QPointF( 20, 25));
        _target_positions.push_back(QPointF(50, 25));
        _target_positions.push_back(QPointF(80, 25));
        _target_positions.push_back(QPointF( 20,50));
        _target_positions.push_back(QPointF(50,50));
        _target_positions.push_back(QPointF(80,50));
        _target_positions.push_back(QPointF(20,75));
        _target_positions.push_back(QPointF(50,75));
        _target_positions.push_back(QPointF(80,75));
        break;
    case (16):
        _target_positions.push_back(QPointF(20,23));
        _target_positions.push_back(QPointF(40,23));
        _target_positions.push_back(QPointF(60,23));
        _target_positions.push_back(QPointF(80,23));

        _target_positions.push_back(QPointF(80,41));
        _target_positions.push_back(QPointF(60,41));
        _target_positions.push_back(QPointF(40,41));
        _target_positions.push_back(QPointF(20,41));

        _target_positions.push_back(QPointF(20,59));
        _target_positions.push_back(QPointF(40,59));
        _target_positions.push_back(QPointF(60,59));
        _target_positions.push_back(QPointF(80,59));

        _target_positions.push_back(QPointF(80,75));
        _target_positions.push_back(QPointF(60,75));
        _target_positions.push_back(QPointF(40,75));
        _target_positions.push_back(QPointF(20,75));
        break;
    default:
        break;
    }
}

/*!
 * \brief CalibrationDialog::setCalibrationTimeout Sets the time to calibrate each target in ms.
 * \param timeout Calibration time in ms.
 */
void CalibrationDialog::setCalibrationTimeout(float timeout) {
    if (timeout >= 0) {
        this->_time_calibrate = timeout;
    }
}

/*!
 * \brief CalibrationDialog::setCalibrationDelay Sets the time to wait before each calibration target in ms.
 * \param delay Hold time in ms.
 */
void CalibrationDialog::setCalibrationDelay(float delay) {
    if (delay >= 0) {
        this->_time_hold = delay;
    }
}

/*!
 * \brief CalibrationDialog::setEyeSide Sets the side to be calibrated.
 * \param side Side (rclgaze::RIGHT_EYE, rclgaze::LEFT_EYE, or rclgaze::BOTH_EYES)
 */
void CalibrationDialog::setEyeSide(rclgaze::eyeSide side) {
    this->_side_to_calibrate = side;
}
