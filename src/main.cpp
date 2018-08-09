/*
main.cpp

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

#include "Gui.h"
#include <QApplication>
#include <qfile.h>
#include <qdebug.h>

int main(int argc, char *argv[])
{
    qDebug() << "Starting Application";
    Q_INIT_RESOURCE(gazetrackguiresources);

    QApplication a(argc, argv);

    // Load style sheet
    QFile f(QString("..gazeTrackGUI/Resources/gazeStyle.qss"));//C:/Users/Sean/Documents/Gaze Track/Gaze SVN/branches/gazeTrackGUI/Resources/gazeStyle.qss
    if (f.open(QIODevice::ReadOnly)) {
        QString styleSheet = QLatin1String(f.readAll());
		qDebug() << styleSheet;
        a.setStyleSheet(styleSheet);
    }

    // Open window
    GazeTrackGUI w;
    w.show();

    return a.exec();
}

