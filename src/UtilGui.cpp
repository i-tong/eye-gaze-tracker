/*
UtilGui.cpp

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

#include "UtilGui.h"

/*!
    @brief Finds the direction vector between Qt points.
    @param point1 starting point
    @param point2 ending point
    @param ret vector from point1 to point2
*/
void findVecDirection(QPointF point1, QPointF point2, QPointF &ret) {
    // Returns direction moving from point 1 to point 2
    // 1: positive direction
    // 0: no change
    //-1: negative direction
    QPointF diff = point2 - point1;

    // Determine x direction
    if (diff.x() > 1) {
        ret.setX(1);
    } else if (diff.x() < -1) {
        ret.setX(-1);
    } else {
        ret.setX(0);
    }

    // Determine y direction
    if (diff.y() > 1) {
        ret.setY(1);
    } else if (diff.y() < -1) {
        ret.setY(-1);
    } else {
        ret.setY(0);
    }
}

/*!
    @brief Convert cv::Mat to Qt::Image

    @param src OpenCV image
    @param dst Qt image
*/
void cvtMat2Qimage(cv::Mat src, QImage &dst) {

    switch (src.type()) {
        case CV_8UC1:
            cv::cvtColor(src,src,CV_BGR2GRAY);
            break;
        case CV_8UC3:
            cv::cvtColor(src,src,CV_BGR2RGB);
            break;
    }

    dst = QImage(src.data, src.cols, src.rows, src.cols*3, QImage::Format_RGB888);

}

/*!
 * \brief displayCVMat prints out an OpenCV Mat object.
 * \param M OpenCV Mat matrix
 * \param name Name of matrix to display
 */
void displayCVMat(cv::Mat M, QString name) {
    // This function is used to print to debug console an opencv mat
    QDebug debug = qDebug();

    debug << "\n" << name << ":\n";

    for (int i = 0; i < M.rows; i++) {
        QString row = "";
        for (int j = 0; j < M.cols ; j++) {
            row.append(QString::number(M.at<float>(i,j)));
            row.append(" ");

        }
        debug << row << "\n";
    }
    debug << "Depth" << M.depth() << CV_32F;

}

/*!
 * \brief getNumberFromString This function searches through a string and extracts any numerical values.
 * \param inStr string to search
 * \return Vector of numbers found
 *
 */

std::vector<float> getNumberFromString(QString inStr) {
    std::vector<float> retList;
    QRegExp rx("(\\d+)");
    QStringList strList;

    int pos = 0;
    while ((pos = rx.indexIn(inStr,pos)) != -1) {
        strList << rx.cap(1);
        retList.push_back(rx.cap(1).toFloat());
        pos += rx.matchedLength();
    }

    return retList;
}
