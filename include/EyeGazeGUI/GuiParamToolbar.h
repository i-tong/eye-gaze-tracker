/*
GuiParamToolbar.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2018 University of British Columbia

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


#ifndef GAZEPARAMTOOLBAR_H
#define GAZEPARAMTOOLBAR_H

#include "qwidget.h"
#include <qtoolbar.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include "Util.h"
#include "qapplication.h"
#include <QDesktopWidget>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qhash.h>

class GazeParamToolbar : public QToolBar
{
	Q_OBJECT
public:
	GazeParamToolbar(void);
	~GazeParamToolbar(void);

public slots:
	void onSetButtonClicked();
signals:
	void paramChanged(QHash<QString, float> outHash);
private:
	void initLayout();
    QHash<QString, float> paramHash;
    // Widgets
    QPushButton* pushButton_set;
    QLineEdit* textBox_numPixKeep;
    QLineEdit* textBox_numPixKeepROI;
    QLineEdit* textBox_roughPupilMinIQ;
    QLineEdit* textBox_roughPupilMinArea;
    QLineEdit* textBox_roughPupilMaxArea;
    QLineEdit* textBox_finePupilThresMultiplier;
    QLineEdit* textBox_finePupilMinIQ;
    QLineEdit* textBox_finePupilMinArea;
    QLineEdit* textBox_finePupilMaxArea;
};

#endif

