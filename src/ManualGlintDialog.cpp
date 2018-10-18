/*
ManualGlintDialog.cpp

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




#include "ManualGlintDialog.h"

ManualGlintDialog::ManualGlintDialog(GazeToolbar* mainToolbar,EyeTracker* tracker,QSettings* settings,int window_size,int actual_size) :
    _toolbar(mainToolbar),_tracker(tracker), _settings(settings)
{
    this->setWindowIcon(QIcon(":/Icons/Resources/cgaze_icon.png"));
    // set the window size to be 3:1
    this->setFixedSize(window_size*3,window_size);
    _ratio = float(window_size)/float(actual_size); // ratio for scaling the eye image

    _instruction = new QLabel(this);
    _instruction->setText("Please click on the center of glint.\nClick NEXT to confirm selection.");
    _instruction->move(this->height()*2+100,10);
    _next_glint = new QLabel(this);
    _next_glint->setText("L0");

    _next_glint->setFont(QFont("Helvetica",20,QFont::Bold));
    _next_glint->move(_instruction->x()+120,_instruction->y()+50);
    _next_button = new QPushButton(this);
    _next_button->setText("NEXT");
    _next_button->setMinimumSize(400,80);
    _next_button->setMaximumSize(400,80);
    _next_button->move(_next_glint->x()-200, _next_glint->y()+50);
    _example_im = new QLabel(this);
    _example_im->move(this->height()*2,this->height()/2);
    _example_im->setPixmap(QPixmap(":/Icons/Resources/glintexample.png").scaledToWidth(this->height()));

    _glintid = L0;
    _glints = std::vector<QPoint> (6);
    connect(_next_button,SIGNAL(clicked()),this,SLOT(onButtonClick()));
}

void ManualGlintDialog::drawImage(QImage image) {
    _display_image = image.copy().scaled(QSize(image.width()*_ratio,image.height()*_ratio));
    this->repaint();
}

void ManualGlintDialog::setTemplate()
{
    cv::Vec2f gl01, gl02, gr01, gr02;
    //left eye
    gl01[0] = int((_glints.at(L1).x()-_glints.at(L0).x())/_ratio); //dx 1
    gl01[1] = int((_glints.at(L1).y()-_glints.at(L0).y())/_ratio); //dy 1
    gl02[0] = int((_glints.at(L2).x()-_glints.at(L0).x())/_ratio); //dx 2
    gl02[1] = int((_glints.at(L2).y()-_glints.at(L0).y())/_ratio); //dy 2
    //right eye
    gr01[0] = int((_glints.at(R1).x()-_glints.at(R0).x())/_ratio); //dx 1
    gr01[1] = int((_glints.at(R1).y()-_glints.at(R0).y())/_ratio); //dy 1
    gr02[0] = int((_glints.at(R2).x()-_glints.at(R0).x())/_ratio); //dx 2
    gr02[1] = int((_glints.at(R2).y()-_glints.at(R0).y())/_ratio); //dy 2

    _tracker->setGlintTemplate(gl01,gl02,gr01,gr02);

    _settings->setValue("GL01X",gl01[0]);
    _settings->setValue("GL01Y",gl01[1]);
    _settings->setValue("GL02X",gl02[0]);
    _settings->setValue("GL02Y",gl02[1]);
    _settings->setValue("GR01X",gr01[0]);
    _settings->setValue("GR01Y",gr01[1]);
    _settings->setValue("GR02X",gr02[0]);
    _settings->setValue("GR02Y",gr02[1]);

}

void ManualGlintDialog::onButtonClick()
{
    switch(_glintid){
    case 0:
        _next_glint->setText("L1");
        break;
    case 1:
        _next_glint->setText("L2");
        break;
    case 2:
        _next_glint->setText("R0");
        break;
    case 3:
        _next_glint->setText("R1");
        break;
    case 4:
        _next_glint->setText("R2");
        break;
    case 5:
        setTemplate();
        this->close();
        _toolbar->setStatus_pushButton_manualGlint(false);
        break;
    }
    _glintid = GlintId(int(_glintid) + 1);

}

void ManualGlintDialog::paintEvent(QPaintEvent *) {
    QPainter p(this);

    if (_display_image.isNull()) {
        p.fillRect(0,0,this->width(),this->height(),Qt::gray);
    } else{
        p.fillRect(0,0,this->width(),this->height(),Qt::gray);
        p.drawImage(QPoint(0,0),_display_image);

        QPen pen(Qt::green, 2, Qt::SolidLine);
        p.setPen(pen);
        p.setRenderHint(QPainter::Antialiasing, true);
        for (int i = 0 ; i <= int(_glintid) ; i++) {
            int x = _glints.at(i).x();
            int y = _glints.at(i).y();
            if (x == 0 || y == 0) {
                continue;
            }
            p.drawEllipse(_glints.at(i),10,10);
            p.drawLine(QLine(x-4,y,x+4,y));
            p.drawLine(QLine(x,y-4,x,y+4));
        }
    }

}

void ManualGlintDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        _glints.at(int(_glintid))=event->pos();
        this->repaint();
        if (_glintid == 5)
            _next_button->setText("Finish");
    }
}
