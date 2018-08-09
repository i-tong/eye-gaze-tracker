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
    mToolbar(mainToolbar),mtracker(tracker), _settings(settings)
{
    // set the window size to be 3:1
    this->setFixedSize(window_size*3,window_size);
    ratio = float(window_size)/float(actual_size); // ratio for scaling the eye image

    instruction = new QLabel(this);
    instruction->setText("Please click on the center of glint.\nClick NEXT to confirm selection.");
    instruction->move(this->height()*2+100,10);
    nextGlint = new QLabel(this);
    nextGlint->setText("L0");

    nextGlint->setFont(QFont("Helvetica",20,QFont::Bold));
    nextGlint->move(instruction->x()+120,instruction->y()+50);
    nextButton = new QPushButton(this);
    nextButton->setText("NEXT");
    nextButton->setMinimumSize(400,80);
    nextButton->setMaximumSize(400,80);
    nextButton->move(nextGlint->x()-200, nextGlint->y()+50);
    nomenclature = new QLabel(this);
    nomenclature->move(this->height()*2,this->height()/2);
    nomenclature->setPixmap(QPixmap(":/Icons/Resources/glintexample.png").scaledToWidth(this->height()));

    glintid = L0;
    glints = std::vector<QPoint> (6);
    glintmark = std::vector<QPoint> (6);
    connect(nextButton,SIGNAL(clicked()),this,SLOT(onButtonClick()));
}

void ManualGlintDialog::drawImage(QImage image) {
    displayImage = image.copy().scaled(QSize(image.width()*ratio,image.height()*ratio));
    this->repaint();
}

void ManualGlintDialog::setTemplate()
{
    cv::Vec2f gl01, gl02, gr01, gr02;
    //left eye
    gl01[0] = int((glints.at(L1).x()-glints.at(L0).x())/ratio); //dx 1
    gl01[1] = int((glints.at(L1).y()-glints.at(L0).y())/ratio); //dy 1
    gl02[0] = int((glints.at(L2).x()-glints.at(L0).x())/ratio); //dx 2
    gl02[1] = int((glints.at(L2).y()-glints.at(L0).y())/ratio); //dy 2
    //right eye
    gr01[0] = int((glints.at(R1).x()-glints.at(R0).x())/ratio); //dx 1
    gr01[1] = int((glints.at(R1).y()-glints.at(R0).y())/ratio); //dy 1
    gr02[0] = int((glints.at(R2).x()-glints.at(R0).x())/ratio); //dx 2
    gr02[1] = int((glints.at(R2).y()-glints.at(R0).y())/ratio); //dy 2

    //    for (int i =0; i < glintTemplate.size();i++)
    //        qDebug()<<glintTemplate.at(i);
    mtracker->setGlintTemplate(gl01,gl02,gr01,gr02);
    //    for (int i=0;i<glints.size();i++){
    //        qDebug()<<glints.at(i);
    //    }

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
    switch(glintid){
    case 0: nextGlint->setText("L1");break;
    case 1: nextGlint->setText("L2");break;
    case 2: nextGlint->setText("R0");break;
    case 3: nextGlint->setText("R1");break;
    case 4: nextGlint->setText("R2");break;
    case 5: setTemplate();this->close();mToolbar->setStatus_pushButton_manualGlint(false);break;
    }
    glintid = GLINTID(int(glintid) +1);

    //        qDebug() << "Maybe instead of this you can have a list of glint IDs and iterate through that list";
    //        qDebug() << "Then it will be easier to add/subtract glints from the system later on";
    //        qDebug() << "Also the code will be neater";
}

void ManualGlintDialog::paintEvent(QPaintEvent *) {
    QPainter p(this);
    //p.fillRect(0,0,640,480,Qt::green);
    if (displayImage.isNull()) {
        //printf("NULL!\n");
        p.fillRect(0,0,this->width(),this->height(),Qt::gray);
    } else{
        p.fillRect(0,0,this->width(),this->height(),Qt::gray);
        p.drawImage(QPoint(0,0),displayImage);

        QPen pen(Qt::green, 2, Qt::SolidLine);
        p.setPen(pen);
        p.setRenderHint(QPainter::Antialiasing, true);
        for (int i = 0 ; i <= int(glintid) ; i++) {
            int x = glints.at(i).x();
            int y = glints.at(i).y();
            if (x == 0 || y == 0) {
               // qDebug() << i << "nothing";
                continue;
            }
            p.drawEllipse(glints.at(i),10,10);
            p.drawLine(QLine(x-4,y,x+4,y));
            p.drawLine(QLine(x,y-4,x,y+4));
        }
    }

}

void ManualGlintDialog::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        glints.at(int(glintid))=event->pos();
        this->repaint();
        if (glintid == 5)
            nextButton->setText("Finish");
    }
}


