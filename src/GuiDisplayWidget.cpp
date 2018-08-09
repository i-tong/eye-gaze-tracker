/*
GuiDisplayWidget.cpp

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
#include "GuiDisplayWidget.h"

GazeDisplayWidget::GazeDisplayWidget(QWidget *parent)
    : QWidget(parent)
{
	this->setMinimumWidth(640);
	this->setMinimumHeight(480);

}

GazeDisplayWidget::~GazeDisplayWidget()
{

}

void GazeDisplayWidget::drawImage(QImage image) {
	displayImage = image.copy();
	this->repaint();
}

void GazeDisplayWidget::paintEvent(QPaintEvent *) {
	QPainter p(this);
 	//p.fillRect(0,0,640,480,Qt::green);
    if (displayImage.isNull()) {
        //printf("NULL!\n");
		p.fillRect(0,0,this->width(),this->height(),Qt::gray);
	} else{
		// Determine 0,0 pixel locations
		p.fillRect(0,0,this->width(),this->height(),Qt::gray);
		int x = (this->width()-displayImage.width())/2;
		int y = (this->height()-displayImage.height())/2;
		p.drawImage(QPoint(x,y),displayImage);
	}
}


