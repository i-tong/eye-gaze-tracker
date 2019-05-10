/*
ManualGlintDialog.h

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
    enum GlintId {L0,L1,L2,R0,R1,R2};

    GazeToolbar* _toolbar;
    EyeTracker* _tracker;
    QSettings* _settings;
    QImage _display_image;
    QLabel* _instruction;
    QLabel* _next_glint;
    QLabel* _example_im;
    QPushButton* _next_button;
    std::vector<QPoint> _glints; //L0,L1,L2,R0,R1,R2;
    GlintId _glintid;
    float _ratio;

    void setTemplate();

signals:

private slots:
    void onButtonClick();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *e);
};

#endif // MANUALGLINTWINDOW_H
