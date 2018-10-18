/*
GuiDataLogSettings.cpp

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

#include "GuiDataLogSettings.h"
#define NUM_DATALOG_OPTIONS 12
Dialog_DataLogSettings::Dialog_DataLogSettings( QSettings* settings)
{
    _settings = settings;
    initializeWidgets();
}



Dialog_DataLogSettings::~Dialog_DataLogSettings() {
    // Change setting

    QString settingList[NUM_DATALOG_OPTIONS] ={
                            "pog_position_right", \
                            "pg_right", \
                            "pupil_position_right", \
                            "pupil_size_right", \
                            "glint_position_right", \
                            "pog_position_left", \
                            "pg_left", \
                            "pupil_position_left", \
                            "pupil_size_left", \
                            "glint_position_left", \
                            "record_video", \
                            "record_calibration" };

    for (int i = 0 ; i < NUM_DATALOG_OPTIONS ; i++ ) {
        _settings->setValue(settingList[i], (int)_checkBoxList[i]->isChecked());
    }

    _settings->setValue("logfile_directory",   _lineEdit_logDirectory->text());

}

void Dialog_DataLogSettings::initializeWidgets() {
    _layout = new QGridLayout;

    QString eyeStringList[5] ={
                            "POG Position", \
                            "PG Vector", \
                            "Pupil Position", \
                            "Pupil Size", \
                            "Glint Position"};

    int numLabels = 5;

    // Add labels
    QLabel* labelRight = new QLabel("Right Eye");
    QLabel* labelLeft = new QLabel("Left Eye");
    _layout->addWidget(labelRight,0,0);
    _layout->addWidget(labelLeft,0,1);

    // Populate checkboxes
    for (int i = 0 ; i < numLabels ; i++){ // Right eye options
        _checkBoxList[i] = new QCheckBox(eyeStringList[i]);
        _layout->addWidget(_checkBoxList[i],i+1,0);
    }
    for (int i = 0 ; i < numLabels ; i++){ // Left eye options
        _checkBoxList[i+numLabels] = new QCheckBox(eyeStringList[i]);
        _layout->addWidget(_checkBoxList[i+numLabels],i+1,1);
    }
    // Add labels that are not specific to the eye
    _checkBoxList[numLabels*2] = new QCheckBox(QString("Record Video"));
    _checkBoxList[numLabels*2 + 1] = new QCheckBox(QString("Calibration"));
    _layout->addWidget(_checkBoxList[numLabels*2],numLabels+1,0);
    _layout->addWidget(_checkBoxList[numLabels*2 + 1],numLabels+2,0);

    for (int i = 0 ; i < NUM_DATALOG_OPTIONS ; i++) {
       connect(_checkBoxList[i], SIGNAL(stateChanged(int)),this,SLOT(onCheckBoxSelected()));
    }

    QLabel* labelDirectory = new QLabel("Output Directory");
    _button_browse = new QPushButton("Browse...");
    connect(_button_browse,SIGNAL(released()),this,SLOT(onPushButtonPressedBrowse()));
    _lineEdit_logDirectory = new QLineEdit();
    _layout->addWidget(labelDirectory,numLabels+3,0);
    _layout->addWidget(_lineEdit_logDirectory,numLabels+4,0);
    _layout->addWidget(_button_browse,numLabels+4,1);

    loadSettings();

    this->setLayout(_layout);

}

void Dialog_DataLogSettings::onCheckBoxSelected() {

}

void Dialog_DataLogSettings::loadSettings() {
    QString settingList[NUM_DATALOG_OPTIONS] ={
                            "pog_position_right", \
                            "pg_right", \
                            "pupil_position_right", \
                            "pupil_size_right", \
                            "glint_position_right", \
                            "pog_position_left", \
                            "pg_left", \
                            "pupil_position_left", \
                            "pupil_size_left", \
                            "glint_position_left", \
                            "record_video", \
                            "record_calibration"};

    for (int i=0; i < NUM_DATALOG_OPTIONS;i++) {
        _checkBoxList[i]->setChecked(_settings->value(settingList[i],0).toBool());
    }

    _lineEdit_logDirectory->setText(_settings->value("logfile_directory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString());

}

void Dialog_DataLogSettings::onPushButtonPressedBrowse() {
    QString currDir = _lineEdit_logDirectory->text();
    QString dir = QFileDialog::getExistingDirectory(this,tr("Select Folder"), \
                                                    currDir, \
                                                    QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    _lineEdit_logDirectory->setText(dir);
}
