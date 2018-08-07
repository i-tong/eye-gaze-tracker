/*
GuiHeadCompensationDialog.cpp

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


#include "GuiHeadCompensationDialog.h"
#include "ui_HeadCompensationDialog.h"

HeadCompensationDialog::HeadCompensationDialog(EyeTracker* tracker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadCompensationDialog)
{
    ui->setupUi(this);

    _tracker = tracker;
    _calHead = new CalibrationHead(*_tracker);

    ui->progressBar->setValue(0);
    // connect
    connect(ui->pushButton_left, SIGNAL(clicked()),this,SLOT(onButtonClick_left()));
    connect(ui->pushButton_right, SIGNAL(clicked()),this,SLOT(onButtonClick_right()));
    connect(ui->pushButton_up, SIGNAL(clicked()),this,SLOT(onButtonClick_up()));
    connect(ui->pushButton_down, SIGNAL(clicked()),this,SLOT(onButtonClick_down()));
    connect(_calHead,SIGNAL(recalibration_finished()),this,SLOT(finished()));
    connect(ui->pushButton_finished,SIGNAL(clicked()),this,SLOT(onButtonClick_finished()));

    _currDir = HEAD_NONE;
    for (unsigned int i = 0 ; i < 4 ; i++ ) {
        _countDoneVector.push_back(0);
    }
}

HeadCompensationDialog::~HeadCompensationDialog()
{
    delete ui;
}

void HeadCompensationDialog::setDisplayMonitor(int monitorId) {
    // Sets the monitor to display the calibration sequence on
    _monitorId = monitorId;
    _calHead->setMonitor(_monitorId);
}

void HeadCompensationDialog::showEvent(QShowEvent *) {
    for (unsigned int i = 0 ; i < 4 ; i++ ) {
        _countDoneVector.at(i) = 0;
    }
    _currDir = HEAD_NONE;
    ui->progressBar->setValue(0);
    ui->pushButton_left->setFocus();

    _calHead->setMonitor(_monitorId);
}

void HeadCompensationDialog::onButtonClick_left() {

    if (_tracker->isCalibrated()) {
        _currDir = HEAD_LEFT;
        ui->label_instructions->setText("Instruct user to move their head to the left.");
        _calHead->startCalibration(1);
        ui->pushButton_left->setFocus();
    } else {
        showCalibrationError();
    }
}


void HeadCompensationDialog::onButtonClick_right() {
    if (_tracker->isCalibrated()) {
        _currDir = HEAD_RIGHT;
        ui->label_instructions->setText("Instruct user to move their head to the right.");
        _calHead->startCalibration(2);
        ui->pushButton_right->setFocus();

    } else {
        showCalibrationError();
    }
}

void HeadCompensationDialog::onButtonClick_up() {
    if (_tracker->isCalibrated()) {
        _currDir = HEAD_UP;
        ui->label_instructions->setText("Instruct user to move their head up.");
        ui->pushButton_up->setFocus();
        _calHead->startCalibration(3);
    } else {
        showCalibrationError();
    }
}

void HeadCompensationDialog::onButtonClick_down() {
    if (_tracker->isCalibrated()) {
        _currDir = HEAD_DOWN;
        ui->label_instructions->setText("Instruct user to move their head down.");
        _calHead->startCalibration(4);
        ui->pushButton_down->setFocus();
    } else {
        showCalibrationError();
    }
}

void HeadCompensationDialog::onButtonClick_finished() {
    this->done(1);
}

void HeadCompensationDialog::showCalibrationError() {
    QMessageBox errorBox;
    qDebug() << "error";
    errorBox.setText("Please calibrate before HEAD calibration");
    errorBox.setWindowTitle("Error");
    errorBox.setIcon(QMessageBox::Warning);
    errorBox.show();
}

void HeadCompensationDialog::finished() {
    _countDoneVector.at(_currDir)+=1;
    if (_countDoneVector.at(_currDir) == 1 ) {
        int val = ui->progressBar->value() + 25;
        ui->progressBar->setValue(val);
    }

    _currDir = HEAD_NONE;
}


