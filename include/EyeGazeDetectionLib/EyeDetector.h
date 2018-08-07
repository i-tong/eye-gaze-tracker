/*
EyeDetector.h

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


#ifndef EYEDETECTOR_H
#define EYEDETECTOR_H
#include "stdafx.h"
// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "EyeGazeCommon.h"

class EYEGAZEDETECTION_API EyeDetector
{
public:
	EyeDetector(rclgaze::eyeSide side, std::string settings_directory);
	~EyeDetector();

	void init();
	bool findPupilGlintVector(cv::Mat srcFrame);

	bool getPupil(cv::RotatedRect& retPupil);
	bool getGlintList(std::vector<cv::Point2f>& retGlintList);
	float getGlintDistance(int index0, int index1);
	cv::Vec3f getGlintDistanceAll();
	cv::Point2f getPGVectorZero(float glintRef = -1, int glintIdx = 0);
	std::vector<cv::Point2f> getPGVectorAll(cv::Vec3f glintRef = cv::Vec3f(-1, -1, -1));
	rclgaze::eyeSide getEyeSide();

	void setGlintTemplate(int newDx, int newDy, int newDx_sec, int newDy_sec);
	void setEyeSide(rclgaze::eyeSide side);

private:
	rclgaze::eyeSide _side;

};

#endif
