/*
clientSocket.cpp

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




#include "clientSocket.h"

clientSocket::clientSocket(QObject *parent) : QObject(parent)
{
    client = new QTcpSocket(this);
    my_flag_socket = new bool(false);
    msgSuccess = new bool (false);

    connect(client, SIGNAL(connected()),
            this, SLOT(showConnection()));
    connect(client,SIGNAL(readyRead()),
            this, SLOT(parseMsg()));
}

clientSocket::~clientSocket()
{
    client->close();
}

void clientSocket::parseMsg()
{
    char buffer[1024] = {0};
    client->read(buffer, client->bytesAvailable());
    QString line = buffer;
    QStringList line_split = line.split(",");
    msg = QString("Gaze X:%1,Gaze Y:%2,Gaze Z:%3")
                  .arg(line_split.at(0)).arg(line_split.at(1)).arg(line_split.at(2));

    *msgSuccess = true;
    hash.insert("Gaze X", line_split.at(0).toInt());
    hash.insert("Gaze Y", line_split.at(1).toInt());
    hash.insert("Gaze Z", line_split.at(2).toInt());
//    clientBrowser->append(hash);
}

void clientSocket::connectSocket(QString address, int port)
{
    //connect to server at address and port
    QHostAddress addr(address);
    client->connectToHost(addr, port);

//    if (client->waitForConnected(3000)){
//        qDebug() << "Connected";
//        *my_flag_socket = true;
//    }
}

bool clientSocket::getMsg(std::string& message)
{
    if (my_flag_socket && msg!="") {
        message = msg.toLocal8Bit().constData();
        return *msgSuccess;
    }
    return false;
}

void clientSocket::showConnection()
{
    *my_flag_socket = true;
    qDebug() << "Socket is connected!!!!!!";

}

bool clientSocket::getStatus()
{
    return *my_flag_socket;
}