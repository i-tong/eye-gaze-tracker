/*
CalibrationHead.h

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

#ifndef CALIBRATIONHEAD_H
#define CALIBRATIONHEAD_H

#include <QObject>
#include <QDialog>
#include <qpoint.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qelapsedtimer.h>
#include <qdebug.h>
#include "EyeTracker.h"
#include <QDesktopWidget>
#include <QRect>
#include <QApplication>
#include <QIcon>

enum calibration_head_state {
    CALIBRATE,
    GUIDE,
};

/**
 * @brief CalibrationHead class, which initiates an QDialog object with one point
 * calibration at the center. Head calibration calibrates the user's POG at 4
 * different head positions - left (idx = 1), right (idx = 2), up (idx = 3) and down (idx = 4) in order.
 */
class CalibrationHead : public QDialog
{
    Q_OBJECT

public:

    explicit CalibrationHead(EyeTracker &tracker);


    ~CalibrationHead();

    void startCalibration(int headIdx);
    void finishCalibration();
    void setMonitor(int monitorId);
    void startCalibrationGuide(int index);
    void initCalibration(int small_radius,
                         int large_radius,
                         float shrink_rate,
                         float grow_rate,
                         float move_rate,
                         int time_calibrate,
                         bool external_display = false,
                         int external_width = 640,
                         int external_height = 360);

signals:
    void target_finished();
    void ShowFrameHead(float x, float y, float size, QColor color, bool isHead);
private:
    int		SMALL_RADIUS; // the smallest radius that the calibration circle shrinks in px
    int		LARGE_RADIUS; // the largest radius that the calibration circle grows
    float	SHRINK_RATE; // pixels per millisecond
    float	GROW_RATE; // pixels per millisecond
    float	MOVE_RATE; // pixels per millisecond
    int		TIME_CALIBRATE; // length of data collection period in ms
    int     TIME_BUFFER; // buffer time in the beginning so that the user can fixate at

    void paintEvent(QPaintEvent *);

    bool _is_external_display;
    int _external_width;
    int _external_height;



    EyeTracker*            _tracker; // eye gaze tracker that is running
    QPointF                 _targetPosition; // gaze calibration position, at the center
    QElapsedTimer			_calibTimer; // an elasped timer to keep track of time

    QTimer*					_paintTimer; // a timer to start clibration once calibration button has been clicked
    QPointF                 _currPosition; // current position of calibration circle
    Qt::GlobalColor         _currColor; // current color of calibration circle
    float                   _currRadius; // current radius of calibration circle
    int                     _headIndex; // current index of Head calibration sequence
    calibration_head_state  _currState;


private slots:

    void calibration();


};

#endif // CALIBRATIONHEAD_H
