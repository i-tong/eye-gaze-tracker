/*
clientSocket.h

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




#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QHash>
#include <QDebug>

class clientSocket : public QObject
{
    Q_OBJECT
public:
    explicit clientSocket(QObject *parent = 0);
    ~clientSocket();

    void connectSocket(QString address, int port);
    bool getMsg(std::string& message);
    bool getStatus();
    bool *my_flag_socket;
    bool *msgSuccess;

private:
    QTcpSocket *client;
    QString connectionStatus;
    QString msg;
    QHash<QString,int> hash;
//    bool *my_flag_socket;
//    bool *msgSuccess;

signals:

public slots:

private slots:
    void showConnection();
    void parseMsg();
};

#endif // CLIENTSOCKET_H
