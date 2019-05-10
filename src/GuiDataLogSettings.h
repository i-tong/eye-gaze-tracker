/*
GuiDataLogSettings.h

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

#ifndef DIALOG_DATALOGSETTINGS_H
#define DIALOG_DATALOGSETTINGS_H

#include <QSettings>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QFileDialog>
#include <QSettings>
#include <QStandardPaths>
class Dialog_DataLogSettings : public QDialog
{
    Q_OBJECT
public:
    Dialog_DataLogSettings( QSettings* settings );
    ~Dialog_DataLogSettings();
private slots:
    void onPushButtonPressedBrowse();
    void onCheckBoxSelected();
private:
    void loadSettings();
    void initializeWidgets();

    QSettings* _settings;
    QCheckBox* _checkBoxList[17];
    QString _settings_filename;
    QGridLayout* _layout;
    QPushButton* _button_browse;
    QLineEdit* _lineEdit_logDirectory;
};

#endif // DIALOG_DATALOGSETTINGS_H
