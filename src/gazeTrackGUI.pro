#
# gazeTrackGUI.pro
#
# Author(s): Irene Tong, Maxwell Li
# Created on: August 1, 2018
#
# (c) Copyright 2018 University of British Columbia
#
# --- begin license - do not edit ---
#
#    This file is part of CGaze UI.
#
#    CGaze UI is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    Cgaze UI is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with Cgaze UI.  If not, see <https://www.gnu.org/licenses/>.
# --- end license ---
#
#-------------------------------------------------
#
# Project created by QtCreator 2016-01-06T10:18:53
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CGazeUI
TEMPLATE = app
CONFIG += c++11

#QMAKE_CXXFLAGS_RELEASE += -O2

SOURCES += \
        Gui.cpp \
        main.cpp \
        GuiDisplayWidget.cpp \
        GuiToolbar.cpp \
        OpenGazeAPIServer.cpp \
        UtilGui.cpp \
        CalibrationDialog.cpp \
        CalibrationHead.cpp \
        GuiDataLogSettings.cpp \
        GuiHeadCompensationDialog.cpp \
        GuiPositionDisplay.cpp \
        ManualGlintDialog.cpp \
        GuiOpenGazeSettingsDialog.cpp

HEADERS  +=  \
        Gui.h \
        GuiDisplayWidget.h \
        GuiToolbar.h \
        OpenGazeAPIServer.h \
        UtilGui.h \
        CalibrationDialog.h \
        CalibrationHead.h \
        GuiDataLogSettings.h \
        GuiHeadCompensationDialog.h \
        GuiPositionDisplay.h \
        ManualGlintDialog.h \
        GazeTrackerInterface.h \
        GuiOpenGazeSettingsDialog.h

FORMS    += gazetrackgui.ui \
    HeadCompensationDialog.ui

win32:CONFIG(debug, debug|release) {
INCLUDEPATH += $$PWD/../External/eyegaze/x64/vc14/include
INCLUDEPATH += $$PWD/../External/opencv/x64/vc14/include
INCLUDEPATH += $$PWD/../External/boost/x64/vc14/include

LIBS += -L$$PWD/../External/eyegaze/x64/vc14/lib -lEyeGazeTrackerLibd -lUtilLibd
LIBS += -L$$PWD/../External/opencv/x64/vc14/lib opencv_world330d.lib
LIBS += -L$$PWD/../External/boost/x64/vc14/lib boost_system-vc140-mt-gd-x64-1_66.lib boost_chrono-vc140-mt-gd-x64-1_66.lib boost_thread-vc140-mt-gd-x64-1_66.lib boost_timer-vc140-mt-gd-x64-1_66.lib
}

win32:CONFIG(release, debug|release) {
INCLUDEPATH += $$PWD/../External/eyegaze/x64/vc14/include
INCLUDEPATH += $$PWD/../External/opencv/x64/vc14/include
INCLUDEPATH += $$PWD/../External/boost/x64/vc14/include

LIBS += -L$$PWD/../External/eyegaze/x64/vc14/lib -lEyeGazeTrackerLib -lUtilLib
LIBS += -L$$PWD/../External/opencv/x64/vc14/lib opencv_world330.lib
LIBS += -L$$PWD/../External/boost/x64/vc14/lib boost_system-vc140-mt-x64-1_66.lib boost_chrono-vc140-mt-x64-1_66.lib boost_thread-vc140-mt-x64-1_66.lib boost_timer-vc140-mt-x64-1_66.lib

}else:unix {
INCLUDEPATH += $$PWD/../external/eyegaze/x64/linux/include
INCLUDEPATH += $$PWD/../external/opencv/x64/linux/include
INCLUDEPATH += /usr/local/include/boost
INCLUDEPATH += /usr/include/x86_64-linux-gnu

LIBS += -L$$PWD/../external/eyegaze/x64/linux/lib -lEyeGazeTracker -lEyeGazeTrackerCalibration -lEyeGazeTrackerCamera -lEyeGazeTrackerDetection -lUtil
LIBS += -L/usr/lib/x86_64-linux-gnu -lboost_thread -lboost_system -lboost_timer -lboost_chrono
LIBS += -L$$PWD/../external/opencv/x64/linux/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_objdetect -lopencv_imgcodecs -lopencv_videoio
LIBS += -L/usr/lib/x86_64-linux-gnu -l avcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale
}

RESOURCES += \
    $${SRCDIR}gazetrackguiresources.qrc
