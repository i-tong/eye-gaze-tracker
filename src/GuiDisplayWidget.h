/*
GuiDisplayWidget.h

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

#ifndef GUIDISPLAYWIDGET_H
#define GUIDISPLAYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <opencv2/core/core.hpp>

class GuiDisplayWidget : public QWidget
{
	Q_OBJECT

public:
    GuiDisplayWidget(QWidget *parent);
    ~GuiDisplayWidget();
	void drawImage(QImage image);

public Q_SLOTS:

	void paintEvent(QPaintEvent *);

private:
    QImage _display_image;
};

#endif // GUIDISPLAYWIDGET_H
