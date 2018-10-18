/*
GuiOpenGazeSettingsDialog.cpp

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

#include "GuiOpenGazeSettingsDialog.h"

/*!
 * \brief GuiOpenGazeSettingsDialog::GuiOpenGazeSettingsDialog Dialog for setting OPAPI server IP address and port number.
 * \param ip Current IP address
 * \param port Current port number
 *
 */
GuiOpenGazeSettingsDialog::GuiOpenGazeSettingsDialog(QString ip, int port)
{

    // Init widgets
    _label_ip = new QLabel("IP:");
    _label_port = new QLabel("Port:");
    _layout = new QGridLayout();
    _lineEdit_ip = new QLineEdit(ip);
    _lineEdit_port = new QLineEdit(QString::number(port));
    _button_box = new QDialogButtonBox();

    _button_box->addButton(tr("OK"),QDialogButtonBox::AcceptRole);
    _button_box->addButton(tr("Cancel"),QDialogButtonBox::RejectRole);
    connect(_button_box,SIGNAL(rejected()),this,SLOT(reject()));
    connect(_button_box,SIGNAL(accepted()),this,SLOT(accept()));

    _layout->addWidget(_label_ip,0,0);
    _layout->addWidget(_lineEdit_ip,0,1);
    _layout->addWidget(_label_port,1,0);
    _layout->addWidget(_lineEdit_port,1,1);
    _layout->addWidget(_button_box,2,1);

    this->setLayout(_layout);
}

GuiOpenGazeSettingsDialog::~GuiOpenGazeSettingsDialog()
{
}

QString GuiOpenGazeSettingsDialog::getIP() {
    return _lineEdit_ip->text();
}

int GuiOpenGazeSettingsDialog::getPort() {
    return _lineEdit_ip->text().toInt();
}


