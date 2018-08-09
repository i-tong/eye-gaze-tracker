/*
GuiPositionDisplay.h

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


#ifndef GAZEPOSITIONDISPLAY_H
#define GAZEPOSITIONDISPLAY_H

#include <QDialog>
#include <qpoint.h>
#include <qpainter.h>
#include <qtimer.h>
#include <qelapsedtimer.h>
#include "EyeTracker.h"
#include "EyeGazeCommon.h"
#include "UtilGui.h"
#include <qdebug.h>


class GazePositionDisplay : public QDialog
{
    Q_OBJECT

public:
    GazePositionDisplay(EyeTracker* tracker);
    ~GazePositionDisplay(void);
public slots:
   void update();

 private:
   void paintEvent(QPaintEvent *event);
  EyeTracker*  pTracker;
    QTimer* mtimer;
};

#endif // GAZEPOSITIONDISPLAY_H
