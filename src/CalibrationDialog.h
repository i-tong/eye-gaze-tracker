/*
CalibrationDialog.h

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
    ~CalibrationDialog(void);

    void initCalibration(int numberOfTargets,
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
                         int externalHeight = 360);


    void startCalibration();
    void stopCalibration();

    // Target movement
    bool isCalibrationRunning();
    bool addCalibrationPoint(float x, float y);
    float getCalibrationTimeout();
    float getCalibrationDelay();
    float getNumTargets();
    void getCalibrationPoints(std::vector<cv::Point2f>& pos);
    void setNumTargets(int numTargets);
    void setCalibrationTimeout(float timeout);
    void setCalibrationDelay(float delay);
    void setEyeSide(rclgaze::eyeSide side);
public slots:
    void changeTarget();

signals:
    void CalibrationFinished();
    void ShowFrame(float x, float y, float size, QColor color, bool isHead);
private:
    void _holdTarget(int elapsedms, int holdTime);
    void _moveTarget(int elapsedms);
    void _growTarget(int elapsedms);
    void _shrinkTarget(int elapsedms);
    void _calibrateTarget(int elaspedms);
    void paintEvent(QPaintEvent *);

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

    int _num_targets; // number of target points
    int _hold_time_start; // start time of holding, used to find the time difference
    std::vector<QPointF>	_target_positions; // target positions based on number of targets
    QTimer*					_timer; // a timer to continously check/change calibration state
    QElapsedTimer			_elapsed_timer; // a timer to keep track of elapsed time for holing target
    rclgaze::eyeSide _side_to_calibrate; // side of eyes calibrated, left or right
    // Target handling
    int _curr_target; // an index to track the current target point
    float _curr_radius; // current radius of target, in px
    QPointF _curr_position; // current position of target point, in px
    QPointF _curr_direction; // vector that points to the destination point from current position
    Qt::GlobalColor _curr_color; // current calibration color, red/green

    // Gaze tracker
    EyeTracker* _p_tracker; // gaze tracker object reference

    bool _is_external_display;
    int _external_width;
    int _external_height;
};

#endif
