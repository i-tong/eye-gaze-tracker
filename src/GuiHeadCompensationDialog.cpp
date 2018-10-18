/*
GuiHeadCompensationDialog.cpp

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

#include "GuiHeadCompensationDialog.h"
#include "ui_HeadCompensationDialog.h"

/*!
 * \brief HeadCompensationDialog::HeadCompensationDialog This dialog is used to run the calibration for HEAD compensation.
 * \param tracker Eye gaze tracker
 * \param parent Parent Qt class
 */
HeadCompensationDialog::HeadCompensationDialog(EyeTracker* tracker, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HeadCompensationDialog)
{
    this->setWindowIcon(QIcon(":/Icons/Resources/cgaze_icon.png"));

    ui->setupUi(this);

    _tracker = tracker;
    _cal_head = new CalibrationHead(*_tracker);

    QPixmap pix(":/Icons/Resources/calib_icon_light.png");// /Resources/play_icon_light.png
    QIcon icon(pix);
    ui->pushButton_calibrate->setIcon(icon);
    ui->pushButton_calibrate->setIconSize(pix.size()*0.5);

    // connect
    connect(ui->pushButton_left, SIGNAL(clicked()),this,SLOT(onButtonClick_left()));
    connect(ui->pushButton_right, SIGNAL(clicked()),this,SLOT(onButtonClick_right()));
    connect(ui->pushButton_up, SIGNAL(clicked()),this,SLOT(onButtonClick_up()));
    connect(ui->pushButton_down, SIGNAL(clicked()),this,SLOT(onButtonClick_down()));
    connect(_cal_head,SIGNAL(target_finished()),this,SLOT(finished()));
    connect(ui->pushButton_finished,SIGNAL(clicked()),this,SLOT(onButtonClick_finished()));
    connect(ui->pushButton_calibrate,SIGNAL(clicked()),this,SLOT(startCalibration()));
    _curr_dir = HEAD_NONE;

    _paint_timer = new QTimer();
    connect(_paint_timer,SIGNAL(timeout()),this, SLOT(update()));


}

HeadCompensationDialog::~HeadCompensationDialog()
{
    _paint_timer->stop();
    delete ui;
}

void HeadCompensationDialog::closeEvent(QCloseEvent *event) {
    _cal_head->finishCalibration();
    _cal_head->close();
}

void HeadCompensationDialog::setDisplayMonitor(int monitorId) {
    // Sets the monitor to display the calibration sequence on
    _monitor_id = monitorId;
    _cal_head->setMonitor(_monitor_id);
}

void HeadCompensationDialog::showEvent(QShowEvent *) {

    _curr_dir = HEAD_NONE;

    ui->pushButton_left->setFocus();
    this->onButtonClick_left();
    _cal_head->setMonitor(_monitor_id);
    _cal_head->initCalibration(10,20,0.1,0.1,0.9,800);

    _paint_timer->start(30);

    ui->gazeDisplay->setFixedHeight(240);
    ui->gazeDisplay->setFixedWidth(320);
    ui->label_leftstatus->setText("Left: Uncalibrated");
    ui->label_rightstatus->setText("Right: Uncalibrated");
    ui->label_upstatus->setText("Up: Uncalibrated");
    ui->label_downstatus->setText("Down: Uncalibrated");


}

void HeadCompensationDialog::onButtonClick_left() {

    if (_tracker->isCalibrated()) {
        _curr_dir = HEAD_LEFT;
        ui->label_instructions->setText("Instruct user to look at center point and move their head to the left until the blue circle is centered.   ");

        ui->pushButton_left->setFocus();
        _cal_head->startCalibrationGuide(_curr_dir);
    }
}


void HeadCompensationDialog::onButtonClick_right() {
    if (_tracker->isCalibrated()) {
        _curr_dir = HEAD_RIGHT;
        ui->label_instructions->setText("Instruct user to look at center point and move their head to the right until the blue circle is centered.");

        ui->pushButton_right->setFocus();
        _cal_head->startCalibrationGuide(_curr_dir);
    }
}

void HeadCompensationDialog::onButtonClick_up() {
    if (_tracker->isCalibrated()) {
        _curr_dir = HEAD_UP;
        ui->label_instructions->setText("Instruct user to look at center point and move their head up until the blue circle is centered.   ");
        ui->pushButton_up->setFocus();
        _cal_head->startCalibrationGuide(_curr_dir);
    }
}

void HeadCompensationDialog::onButtonClick_down() {
    if (_tracker->isCalibrated()) {
        _curr_dir = HEAD_DOWN;
        ui->label_instructions->setText("Instruct user to look at center point and move their head down until the blue circle is centered. ");
        ui->pushButton_down->setFocus();
        _cal_head->startCalibrationGuide(_curr_dir);
    }
}

void HeadCompensationDialog::onButtonClick_finished() {
    _cal_head->finishCalibration();

    this->done(1);
}

void HeadCompensationDialog::startCalibration() {
    if (_curr_dir != HEAD_NONE) {
        _cal_head->startCalibration(_curr_dir);
    }
}

void HeadCompensationDialog::showCalibrationError() {
    QMessageBox errorBox;
    errorBox.setText("Please calibrate before HEAD calibration");
    errorBox.setWindowTitle("Error");
    errorBox.setIcon(QMessageBox::Warning);
    errorBox.show();
}

void HeadCompensationDialog::finished() {

    switch(_curr_dir){
    case HEAD_LEFT:
        ui->label_leftstatus->setText("Left: Finished");
        break;
    case HEAD_RIGHT:
        ui->label_rightstatus->setText("Right: Finished");
        break;
    case HEAD_UP:
        ui->label_upstatus->setText("Up: Finished");
        break;
    case HEAD_DOWN:
        ui->label_downstatus->setText("Down: Finished");
        break;
    }


    _curr_dir = HEAD_NONE;

}

void HeadCompensationDialog::paintEvent(QPaintEvent *event) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    int dispx, dispy, dispw, disph;
    dispx = ui->gazeDisplay->pos().x();
    dispy = ui->gazeDisplay->pos().y();
    dispw = ui->gazeDisplay->width();
    disph = ui->gazeDisplay->height();

    // Fill background
    p.fillRect(QRect(dispx,dispy,dispw,disph),Qt::gray);

    // Draw centre
    p.setPen(QPen(QColor(90,90,90),3));
    p.setBrush(Qt::white);
    p.drawEllipse(QPoint(dispx+0.5*dispw,dispy+0.5*disph),20,20);

    // Get gaze position
    cv::Point2f pog;
    bool gazeok = _tracker->getPOG(pog,rclgaze::BOTH_EYES);
    if (!gazeok) {
        gazeok = _tracker->getPOG(pog,rclgaze::RIGHT_EYE);
    }
    if (!gazeok) {
        gazeok = _tracker->getPOG(pog,rclgaze::LEFT_EYE);
    }
    if (gazeok) {
        \
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(Qt::blue);
        p.setPen(Qt::NoPen);
        QPoint pos = QPoint(int(dispw * pog.x/100.0),int(disph * pog.y/100.0));
        switch (_curr_dir) {
        case 1: // left
            pos.setX(pos.x() + int(dispw/4));
            break;
        case 2: //right
            pos.setX(pos.x() - int(dispw/4));
            break;
        case 3: //up
            pos.setY(pos.y() + int(disph/4));
            break;
        case 4: // down
            pos.setY(pos.y() - int(disph/4));
            break;
        }
        pos.setX(pos.x()+dispx);
        pos.setY(pos.y()+dispy);

        if (pos.x() > dispx && pos.x() < dispx+dispw \
                && pos.y() > dispy && pos.y() < dispy+disph) {

            p.drawEllipse(pos,15,15);

        }
    }
}
