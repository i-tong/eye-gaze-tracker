/*
Gaze2DEstimation.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2017-2018 University of British Columbia

--- begin license - do not edit ---
    This file is a part of CGaze UI.
    
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


#ifndef GAZE2DESTIMATION_H
#define GAZE2DESTIMATION_H

#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <boost/numeric/ublas/triangular.hpp>

#include <cstring>
#include <fstream>
#include <iomanip>



// lstsq gaze
// Find 2nd order matrix to relate data and targetPos.
// For each data element in data there is a corresponding data element in targetPos.
bool EYEGAZECALIBRATION_API lstsq_gaze(std::vector<cv::Point2f>  data , std::vector<cv::Point2f> targetPos,boost::numeric::ublas::matrix<double>& res );

cv::Point2f EYEGAZECALIBRATION_API calc_gaze(cv::Vec2f pg, boost::numeric::ublas::matrix<double>& b);
cv::Point2f EYEGAZECALIBRATION_API calc_gaze(cv::Vec2f pg, cv::Mat& b);
cv::Point2f EYEGAZECALIBRATION_API calc_gaze(std::vector<cv::Point2f> pgVec, std::vector<boost::numeric::ublas::matrix<double> > matVec, int main, int sec, int& glintIndex);
cv::Point2f EYEGAZECALIBRATION_API calc_gaze_head_compensation(std::vector<cv::Point2f> pgVec, cv::Point2f pupilVec, std::vector<boost::numeric::ublas::matrix<double> > matVec,std::vector<std::vector<cv::Vec2f> > pupilPgMapping, std::vector<std::vector<cv::Vec2f> > pupilPogMapping, int main, int sec, int& glintIndex, cv::Point2f& correction);

/*!
* \brief Calculates the polyfit of points, returns vector of cv::Vec2f
*
* \param oX Source
* \param oY Destination
* \param nDegree Currently only works for first order
* \return Returns <(k_px,b_px),(k_py,b_py)>, where PG = pupil*k + b
*/
std::vector<cv::Vec2f> EYEGAZECALIBRATION_API getPolyfit(std::vector<cv::Point2f>& oX, std::vector<cv::Point2f>& oY, int nDegree);

#endif // GAZE2DESTIMATION_H
