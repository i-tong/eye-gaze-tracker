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

Dialog_DataLogSettings::Dialog_DataLogSettings( QSettings* settings)
{

    _settings = settings;

    // Change setting
    initializeWidgets();
    //loadSettings();
    // Set values

}



Dialog_DataLogSettings::~Dialog_DataLogSettings() {
    // Change setting

    QString settingList[12] ={
                            "pog_position_right", \
                            "pg_right", \
                            "pupil_position_right", \
                            "pupil_size_right", \
                            "glint_position_right", \
                            "record_video_right", \
                            "pog_position_left", \
                            "pg_left", \
                            "pupil_position_left", \
                            "pupil_size_left", \
                            "glint_position_left", \
                            "record_video_left" };

    for (int i = 0 ; i < 12 ; i++ ) {
        _settings->setValue(settingList[i], (int)m_checkBoxList[i]->isChecked());
    }

    _settings->setValue("logfile_directory",   m_lineEdit_logDirectory->text());

}

void Dialog_DataLogSettings::initializeWidgets() {
    m_layout = new QGridLayout;

    QString eyeStringList[6] ={
                            "POG Position", \
                            "PG Vector", \
                            "Pupil Position", \
                            "Pupil Size", \
                            "Glint Position", \
                            "Record Video"};
    int numLabels = 6;

    // Populate checkboxes
    QLabel* labelRight = new QLabel("Right Eye");
    QLabel* labelLeft = new QLabel("Left Eye");

    m_layout->addWidget(labelRight,0,0);
    m_layout->addWidget(labelLeft,0,1);

    for (int i = 0 ; i < numLabels ; i++){ // Right eye options
        m_checkBoxList[i] = new QCheckBox(eyeStringList[i]);
        connect(m_checkBoxList[i], SIGNAL(stateChanged(int)),this,SLOT(onCheckBoxSelected()));
        m_layout->addWidget(m_checkBoxList[i],i+1,0);
    }
    for (int i = 0 ; i < numLabels ; i++){ // Left eye options
        m_checkBoxList[i+numLabels] = new QCheckBox(eyeStringList[i]);
        connect(m_checkBoxList[i+numLabels], SIGNAL(stateChanged(int)),this,SLOT(onCheckBoxSelected()));
        m_layout->addWidget(m_checkBoxList[i+numLabels],i+1,1);
    }

    QLabel* labelDirectory = new QLabel("Output Directory");
    m_button_browse = new QPushButton("Browse...");
    connect(m_button_browse,SIGNAL(released()),this,SLOT(onPushButtonPressedBrowse()));
    m_lineEdit_logDirectory = new QLineEdit();
    m_layout->addWidget(labelDirectory,numLabels+2,0);
    m_layout->addWidget(m_lineEdit_logDirectory,numLabels+3,0);
    m_layout->addWidget(m_button_browse,numLabels+3,1);

    loadSettings();

    this->setLayout(m_layout);

}

void Dialog_DataLogSettings::onCheckBoxSelected() {

}

void Dialog_DataLogSettings::loadSettings() {
    QString settingList[12] ={
                            "pog_position_right", \
                            "pg_right", \
                            "pupil_position_right", \
                            "pupil_size_right", \
                            "glint_position_right", \
                            "record_video_right", \
                            "pog_position_left", \
                            "pg_left", \
                            "pupil_position_left", \
                            "pupil_size_left", \
                            "glint_position_left", \
                            "record_video_left" };

    for (int i=0; i < 12;i++) {
        m_checkBoxList[i]->setChecked(_settings->value(settingList[i],0).toBool());
    }

    m_lineEdit_logDirectory->setText(_settings->value("logfile_directory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString());

}

void Dialog_DataLogSettings::onPushButtonPressedBrowse() {
    QString currDir = m_lineEdit_logDirectory->text();
    QString dir = QFileDialog::getExistingDirectory(this,tr("Select Folder"), \
                                                    currDir, \
                                                    QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
    m_lineEdit_logDirectory->setText(dir);
}

