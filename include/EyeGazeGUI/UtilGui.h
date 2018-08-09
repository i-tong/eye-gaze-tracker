/*
UtilGui.h

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

/*!
    @brief Finds the direction vector between Qt points.
    @param point1 starting point
    @param point2 ending point
    @param ret vector from point1 to point2
*/
void findVecDirection(QPointF point1, QPointF point2, QPointF &ret);

/*!
    @brief Convert cv::Mat to Qt::Image

    @param src OpenCV image
    @param dst Qt image
*/
void cvtMat2Qimage(cv::Mat src, QImage &dst);


/*!
 * \brief displayCVMat prints out an OpenCV Mat object.
 * \param M OpenCV Mat matrix
 * \param name Name of matrix to display
 */
void displayCVMat(cv::Mat M, QString name) ;
std::vector<float> getNumberFromString(QString inStr);

#endif // UTILQT_H
