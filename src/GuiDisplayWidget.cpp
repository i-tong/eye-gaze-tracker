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

GuiDisplayWidget::GuiDisplayWidget(QWidget *parent)
    : QWidget(parent)
{
	this->setMinimumWidth(640);
	this->setMinimumHeight(480);
}

GuiDisplayWidget::~GuiDisplayWidget()
{
}

void GuiDisplayWidget::drawImage(QImage image) {
    _display_image = image.copy();
	this->repaint();
}

void GuiDisplayWidget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    if (_display_image.isNull()) {
		p.fillRect(0,0,this->width(),this->height(),Qt::gray);
	} else{
		// Determine 0,0 pixel locations
		p.fillRect(0,0,this->width(),this->height(),Qt::gray);
        int x = (this->width()-_display_image.width())/2;
        int y = (this->height()-_display_image.height())/2;
        p.drawImage(QPoint(x,y),_display_image);
	}
}
