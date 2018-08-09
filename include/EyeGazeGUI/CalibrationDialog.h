/*
CalibrationDialog.h

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


#ifndef CALIBRATIONDIALOG_H
#define CALIBRATIONDIALOG_H

#include <QDialog>
#include <qpoint.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qelapsedtimer.h>
#include <QGuiApplication>
#include <QWindow>
#include <QScreen>

#include "EyeTracker.h"
#include "EyeGazeCommon.h"
#include "UtilGui.h"
#include <qdebug.h>

/**
 * @brief CalibrationDialog class, which initiates an QDialog object with given number of target points spaced out
 * on the screen. Calibrate eye gaze position in 2D space.
 *
 */
class CalibrationDialog : public QDialog
{
	Q_OBJECT

/**
 * @brief Calibration states:
 * START - start calibration
 * HOLD - hold target still for certain duration to help user fixate
 * SHRINK - calibration target shrinks in px/ms
 * GROW - calibration target grows in px/ms
 * MOVE - calibraiton target moving to next point in px/ms
 * CALIBRATE - gaze tracker actively collects data
 * FIN - calibration finishes
 * STOP - mannually stop the calibration process
 *
 */
enum calibState {
    START,
    HOLD,
	SHRINK,
	GROW,
	MOVE,
	CALIBRATE,
	FIN,
	STOP
	};

public:
	CalibrationDialog();
    /**
     * @brief
     *
     */
    ~CalibrationDialog(void);

    /**
     * @brief initialize calibration parameters, set the passed parameters,
     * initialize currTarget = 0,
     * calibration state = START,
     * current radius to maximum,
     * set cursor position to the center of the screen,
     * initialize a vector of target point positions (targetPosition) based on number of target
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
    virtual void initCalibration(int numberOfTargets,
						 int small_radius, 
						 int large_radius, 
						 float shrink_rate, 
						 float grow_rate,
						 float move_rate, 
                         int time_hold,
						 int time_calibrate, 
                         rclgaze::eyeSide side,
                         EyeTracker &tracker,
                         bool externalDisplay = false,
                         int externalWidth = 640,
                         int externalHeight = 480);

    /**
     * @brief start calibration by start QTimer (continuously check/change calibration state) and ElaspedTimer (keep track of time passed), record
     * the start time of holding (holdTimerStart)
     */
    virtual void startCalibration();

    /**
     * @brief stops calibration by setting state to STOP
     *
     */
    virtual void stopCalibration();

	// Target movement
    /**
     * @brief hold target in position for certain duration to help user fixate
     *
     * @param elapsedms current elasped time in ms
     * @param holdTime time to hold in ms
     */
    void holdTarget(int elapsedms, int holdTime);

    /**
     * @brief move target to the destination (extracted from the targetPosition variable), the distance to travel from current position is calculated
     * by move_rate * elapsedms
     *
     * @param elapsedms elapsed time
     */
    virtual void moveTarget(int elapsedms);

    /**
     * @brief grow target to LARGE_RADIUS, the current radius is calculated by currRadius + grow_rate * elapsedms
     *
     * @param elapsedms elapsed time
     */
    virtual void growTarget(int elapsedms);

    /**
     * @brief shrink target to LARGE_RADIUS, radius is calculated by currRadius - shrink_rate * elapsedms
     *
     * @param elapsedms elapsed time
     */
    virtual void shrinkTarget(int elapsedms);

    /**
     * @brief calibrate current target, tell tracker to collect data at target point
     *
     * @param elaspedms total elapsed time
     */
    virtual void calibrateTarget(int elaspedms);

    /**
     * @brief paint event for drawing
     *
     * @param
     */
    virtual void paintEvent(QPaintEvent *);

    /**
     * @brief check if calibration is currently running
     *
     * @return bool true if state= FIN, false otherwise
     */
    bool isCalibrationRunning();

    /**
     * @brief add a calibration point at specific point
     *
     * @param x   calibration point position as percentage of screen width (0 - 1)
     * @param Y   calibration point position as percentage of screen height (0 - 1)
     *
     * @return bool true if point can be added, false otherwise
     */
    virtual bool addCalibrationPoint(float x, float y);

    /**
     * @brief getCalibrationTimeout
     * @return
     */
    virtual float getCalibrationTimeout();

    virtual float getCalibrationDelay();

    virtual float getNumTargets();

    virtual void getCalibrationPoints(std::vector<cv::Point2f>& pos);

    virtual void setNumTargets(int numTargets);

    virtual void setCalibrationTimeout(float timeout);

    virtual void setCalibrationDelay(float delay);

    virtual void setEyeSide(rclgaze::eyeSide side);
public slots:
    /**
     * @brief called upon timer timeout at 30ms interval, change/check states
     *
     */
    void changeTarget();

    /**
     * @brief readGazeData Slot for reading data being streamed by the gaze tracker.
     *
     *
     */
    void readGazeData(float time, \
                      long long timeTick, \
                      cv::Point2f POGLeft, \
                      cv::Point2f POGRight, \
                      bool pogValidLeft, \
                      bool pogValidRight, \
                      bool eyeFoundLeft, \
                      bool eyeFoundRight, \
                      cv::Point2f POGBest, \
                      bool pogValidBest, \
                      cv::RotatedRect pupilLeft, \
                      cv::RotatedRect pupilRight);
signals:
    void CalibrationFinished();
    void ShowFrame(QImage im);
private:
	// Calibration parameters
    int		_small_radius; // the smallest radius that the calibration circle shrinks in px
    int		_large_radius; // the largest radius that the calibration circle grows in px
    float	_shrink_rate; // pixels per millisecond
    float	_grow_rate; // pixels per millisecond
    float	_move_rate; // pixels per millisecond
    int		_time_calibrate; // length of data collection period in ms
    int     _time_hold;
	// State handling
    calibState	state; // state of calibration, refer to enum calibState
    int			currTarget; // an index to track the current target point
    int			numTargets; // number of target points
    int holdTimeStart; // start time of holding, used to find the time difference
    std::vector<QPointF>	targetPosition; // target positions based on number of targets
    QTimer*					timer; // a timer to continously check/change calibration state
    QElapsedTimer			elapsedTimer; // a timer to keep track of elapsed time for holing target
    rclgaze::eyeSide sideToCalibrate; // side of eyes calibrated, left or right
	// Target handling
    float	currRadius; // current radius of target, in px
    QPointF currPosition; // current position of target point, in px
    QPointF currDirection; // vector that points to the destination point from current position
    Qt::GlobalColor currColor; // current calibration color, red/green
	// Gaze tracker
    EyeTracker* pTracker; // gaze tracker object reference
    cv::Point2f leftPOG;
    cv::Point2f rightPOG;
    cv::Point2f bestPOG;

    bool _is_external_display;
    int _external_width;
    int _external_height;
};

#endif
