/*
GuiHeadCompensationDialog.h

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


#ifndef HEADCOMPENSATIONDIALOG_H
#define HEADCOMPENSATIONDIALOG_H

#include <QDialog>
#include <EyeTracker.h>
#include <CalibrationHead.h>
#include <QMessageBox>

namespace Ui {
class HeadCompensationDialog;
}

enum headDirection {
    HEAD_LEFT,
    HEAD_RIGHT,
    HEAD_UP,
    HEAD_DOWN,
    HEAD_NONE
};

class HeadCompensationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HeadCompensationDialog(EyeTracker* tracker,QWidget *parent = 0);
    ~HeadCompensationDialog();
    void showEvent(QShowEvent *);
    void setDisplayMonitor(int monitorId);

private slots:
    void onButtonClick_left();
    void onButtonClick_right();
    void onButtonClick_up();
    void onButtonClick_down();
    void onButtonClick_finished();
    void finished();
private:
    Ui::HeadCompensationDialog *ui;
    EyeTracker* _tracker;
    CalibrationHead* _calHead;
    void showCalibrationError();
    headDirection _currDir;
    std::vector<int> _countDoneVector;
    int _monitorId;
};

#endif // HEADCOMPENSATIONDIALOG_H
