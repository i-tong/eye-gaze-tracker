/*
DeckLinkDisplay.cpp

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

#include "DeckLinkDisplay.h"


DeckLinkDisplay::DeckLinkDisplay(QObject *parent)
{
    _running = false;
    //_decklink = new DeckLinkCap();
}


DeckLinkDisplay::~DeckLinkDisplay()
{
}

bool DeckLinkDisplay::start() {

    if (_decklink->IsInitialized()) {
        _decklink_output_left = _decklink->InitOutputAtPort(VIDEO_OUT_L);
        _decklink_output_right = _decklink->InitOutputAtPort(VIDEO_OUT_R);

        _decklink_output_left->Start();
        _decklink_output_right->Start();
        _running = true;
    } else {
        return false;
    }
    return true;
}

bool DeckLinkDisplay::stop() {
    if (_running == true) {
        _decklink_output_left->Stop();
        _decklink_output_right->Stop();
        _running = false;
    }
    return true;
}


bool DeckLinkDisplay::isRunning() {
    return _running;
}

void DeckLinkDisplay::displayImage(QImage newim) {
    qDebug() << "displayim" << _running;
    if (_running == true) {
        cv::Mat cv_im(newim.height(),newim.width(),CV_8UC3,(void *)newim.constBits(),newim.bytesPerLine());
        cv::Mat final_im;

        cv::cvtColor(cv_im,final_im,CV_RGB2BGRA);
        cv::flip(final_im,final_im,0);
        int imsize = final_im.rows * final_im.cols * final_im.channels();
        _decklink_output_left->UpdateBuffers(final_im.data,imsize);
        _decklink_output_right->UpdateBuffers(final_im.data, imsize);
    }
}


