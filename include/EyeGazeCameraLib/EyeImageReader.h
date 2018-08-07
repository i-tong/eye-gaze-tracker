/*
EyeImageReader.h

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


#ifndef EYEIMAGEREADER_H
#define EYEIMAGEREADER_H
#include "stdafx.h"

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"

#include "EyeGazeCommon.h"
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

typedef boost::function<void(cv::Mat)> newframe_callback;

class EYEGAZECAMERA_API EyeImageReader {
public:
	EyeImageReader();
	~EyeImageReader();

	bool addVideoDevice(const char* filename, rclgaze::eyeSide side);
	bool addCameraDevice(const char* devicename, rclgaze::eyeSide side);

	void resetDevice(rclgaze::eyeSide side);

	bool setCamDistProp(cv::Mat& intrinsic_mat, std::vector<float>& distortion_mat, rclgaze::eyeSide side);

	bool start();
	bool stop();

	bool getFrame(cv::Mat& refRight, cv::Mat& refLeft);
	bool isRightEyeActive();
	bool isLeftEyeActive();
	void registerVideoCallback(newframe_callback callback_left, newframe_callback callback_right);
};


#endif
