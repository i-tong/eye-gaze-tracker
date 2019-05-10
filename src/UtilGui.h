/*
UtilGui.h

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
#ifndef UTILQT_H
#define UTILQT_H

#include <QPoint>
#include <QImage>
#include <QDebug>
#include <QString>
// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


void findVecDirection(QPointF point1, QPointF point2, QPointF &ret);
void cvtMat2Qimage(cv::Mat src, QImage &dst);
void displayCVMat(cv::Mat M, QString name) ;
std::vector<float> getNumberFromString(QString inStr);

#endif // UTILQT_H
