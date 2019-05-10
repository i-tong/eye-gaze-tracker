/*
GuiOpenGazeSettingsDialog.h

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


#ifndef GUIOPENGAZESETTINGSDIALOG_H
#define GUIOPENGAZESETTINGSDIALOG_H


#include <QSettings>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QString>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QSettings>
#include <QStandardPaths>
#include <QDialogButtonBox>


class GuiOpenGazeSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    GuiOpenGazeSettingsDialog(QString ip="", int port = 0);
    ~GuiOpenGazeSettingsDialog();

    QString getIP();
    int getPort();

private:
    QGridLayout* _layout;
    QLineEdit* _lineEdit_ip;
    QLineEdit* _lineEdit_port;
    QLabel* _label_ip;
    QLabel* _label_port;
    QDialogButtonBox* _button_box;


};

#endif // GUIOPENGAZESETTINGSDIALOG_H
