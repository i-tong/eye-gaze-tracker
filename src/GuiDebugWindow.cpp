/*
GuiDebugWindow.cpp

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

#include "GuiDebugWindow.h"


GazeDebugWindow::GazeDebugWindow(void)
{
	// Start display thread

    runFlag = true;
    thr = boost::thread(boost::bind(&GazeDebugWindow::run, this));


}


GazeDebugWindow::~GazeDebugWindow(void)
{
	runFlag = false;
}

bool GazeDebugWindow::showWindow(const char* windowTitle, cv::Mat srcImage) {
	imageData data;
	data.title= windowTitle;
	srcImage.copyTo(data.image);
    boost::lock_guard<boost::mutex> lock(imagedata_mutex);
    currData.insert(currData.begin(),data);
	return true;
}


void GazeDebugWindow::run() {
    int frame_width =  640;
    int frame_height=  480;
    cv::VideoWriter video("debug_all_glint.avi",CV_FOURCC('M','J','P','G'),20, cv::Size(frame_width,frame_height));
    int cnt = 0;
    bool writeVid = false;

    while (runFlag == true) {
		imageData newData;

		if (currData.size() > 0) {
			boost::lock_guard<boost::mutex> lock(imagedata_mutex);
            newData = currData.back();

            cv::imshow(newData.title,newData.image);
            //cv::moveWindow(newData.title,700,10);
            // video write script
            if (writeVid == true) {
                cv::Mat writeFrame;
                newData.image.copyTo(writeFrame);

                if (strcmp(newData.title,"dispR") == 0 ) {
                    cnt+=1;
                    //cv::cvtColor(newData.image,writeFrame,CV_GRAY2BGR);
                    if (writeFrame.rows==frame_height  && writeFrame.cols==frame_width && cnt > 1200) {
                        video.write(writeFrame);
                        //qDebug() << cnt;
                    }
                    if (cnt==1655) {
                        video.release();
                        writeVid = false;
                        //qDebug() << "Finished writing to video.";
                    }
                }
            }
			currData.pop_back();
		}
        char c = (char)cv::waitKey(1);
		
	}
    //video.release();
}


/* Extra code you can delete this

                    if (writeFrame.cols < 120) {
                        cv::Mat xfill = cv::Mat::zeros(cv::Size(120-writeFrame.cols,writeFrame.rows),writeFrame.type());

                        cv::hconcat(xfill,writeFrame,writeFrame);
                    }
                    if (writeFrame.rows < 80) {
                        cv::Mat yfill = cv::Mat::zeros(cv::Size(writeFrame.cols,80-writeFrame.rows),writeFrame.type());
                        cv::vconcat(yfill,writeFrame,writeFrame);
                    }
                    if (writeFrame.channels() == 1) {
                        cv::cvtColor(writeFrame,writeFrame,cv::COLOR_GRAY2BGR);
                    }

*/

