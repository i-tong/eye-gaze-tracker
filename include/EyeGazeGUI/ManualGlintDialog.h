/*
ManualGlintDialog.h

Author(s): Irene Tong, Maxwell Li
Created on: August 1, 2018

(c) Copyright 2017-2018 University of British Columbia

--- begin license - do not edit ---
    This file is a part of CGaze UI.
    
    CGaze UI is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CGaze UI is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CGaze UI.  If not, see <https://www.gnu.org/licenses/>.
--- end license ---
*/


#ifndef MANUALGLINTWINDOW_H
#define MANUALGLINTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QMouseEvent>
#include "GuiToolbar.h"
#include "GuiDisplayWidget.h"
#include "EyeTracker.h"
#include <QSettings>

class ManualGlintDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ManualGlintDialog(GazeToolbar* mainToolbar,EyeTracker* tracker, QSettings* settings,int window_size,int actual_size);
    void drawImage(QImage image);

private:
    GazeToolbar* mToolbar;
    EyeTracker* mtracker;
    QSettings* _settings;
    QImage displayImage;
    QLabel* instruction;
    QLabel* nextGlint;
    QLabel* nomenclature;
    QPushButton* nextButton;
    std::vector<QPoint> glints;//L0, L1, L2,R0,R1,R2;
    enum GLINTID {L0,L1,L2,R0,R1,R2};
    GLINTID glintid;
    float ratio;

    void setTemplate();
    std::vector<QPoint> glintmark;
//    QGroupBox* groupBox;

signals:

private slots:
    void onButtonClick();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
};

#endif // MANUALGLINTWINDOW_H
