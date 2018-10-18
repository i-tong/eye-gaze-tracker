#-------------------------------------------------
# gazeTrackGUI_release.pro
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
#-------------------------------------------------

#-------------------------------------------------
#
# Project created by QtCreator 2016-01-06T10:18:53
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gazeTrackGUI
TEMPLATE = app
CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE += -O2

DESTDIR =               $$PWD/../build/
Release:DESTDIR =       $$PWD/../build/release
Release:OBJECTS_DIR =   $$PWD/../build/release/.obj
Release:MOC_DIR =       $$PWD/../build/release/.moc
Release:RCC_DIR =       $$PWD/../build/release/.rcc
Release:UI_DIR =        $$PWD/../build/release/.ui

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
        GuiDebugWindow.cpp \
        GuiHeadCompensationDialog.cpp \
        GuiPositionDisplay.cpp \
        ManualGlintDialog.cpp \
        GazeDataLogger.cpp \
        UtilSocket.cpp \
        GuiOpenGazeSettingsDialog.cpp
HEADERS  +=  \
        $$PWD/../include/EyeGazeGUI/Gui.h \
        $$PWD/../include/EyeGazeGUI/GuiDisplayWidget.h \
        $$PWD/../include/EyeGazeGUI/GuiToolbar.h \
        $$PWD/../include/EyeGazeGUI/OpenGazeAPIServer.h \
        $$PWD/../include/EyeGazeGUI/UtilGui.h \
        $$PWD/../include/EyeGazeGUI/CalibrationDialog.h \
        $$PWD/../include/EyeGazeGUI/CalibrationHead.h \
        $$PWD/../include/EyeGazeGUI/GuiDataLogSettings.h \
        $$PWD/../include/EyeGazeGUI/GuiDebugWindow.h \
        $$PWD/../include/EyeGazeGUI/GuiHeadCompensationDialog.h \
        $$PWD/../include/EyeGazeGUI/GuiPositionDisplay.h \
        $$PWD/../include/EyeGazeGUI/ManualGlintDialog.h \
        $$PWD/../include/EyeGazeGUI/GazeTrackerInterface.h \
        $$PWD/../include/EyeGazeGUI/GazeDataLogger.h \
        $$PWD/../include/EyeGazeGUI/GuiOpenGazeSettingsDialog.h \
        $$PWD/../include/UtilLib/EyeGazeCommon.h \
        $$PWD/../include/UtilLib/UtilSocket.h

FORMS    += gazetrackgui.ui \
    HeadCompensationDialog.ui

win32:CONFIG(release, debug|release) {

INCLUDEPATH += $$PWD/../include/EyeGazeGUI
INCLUDEPATH += $$PWD/../lib/opencv/include
INCLUDEPATH += $$PWD/../lib/boost/include
INCLUDEPATH += $$PWD/../lib/ffmpeg/include
INCLUDEPATH += $$PWD/../include/UtilLib
INCLUDEPATH += $$PWD/../include/EyeGazeTrackerLib
INCLUDEPATH += $$PWD/../include/EyeGazeDetectionLib
INCLUDEPATH += $$PWD/../include/EyeGazeCalibrationLib
INCLUDEPATH += $$PWD/../include/EyeGazeCameraLib

LIBS += -L$$PWD/../lib/opencv/lib opencv_world330.lib
LIBS += -L$$PWD/../lib/boost/lib boost_system-vc140-mt-x64-1_66.lib boost_chrono-vc140-mt-x64-1_66.lib boost_thread-vc140-mt-x64-1_66.lib boost_timer-vc140-mt-x64-1_66.lib
LIBS += -L$$PWD/../lib/EyeGazeLib/lib -lEyeGazeCalibrationLib -lEyeGazeTrackerLib -lEyeGazeDetectionLib -lEyeGazeCameraLib
LIBS += -L$$PWD/../lib/ffmpeg/lib avcodec.lib avdevice.lib avfilter.lib avformat.lib avutil.lib swresample.lib swscale.lib

}else:unix {
INCLUDEPATH += $$PWD/../include/EyeGazeGUI
INCLUDEPATH += $$PWD/../lib/opencv/include
INCLUDEPATH += $$PWD/../lib/boost/include
INCLUDEPATH += $$PWD/../include/UtilLib
INCLUDEPATH += $$PWD/../include/EyeGazeTrackerLib
INCLUDEPATH += $$PWD/../include/EyeGazeDetectionLib
INCLUDEPATH += $$PWD/../include/EyeGazeCalibrationLib
INCLUDEPATH += $$PWD/../include/EyeGazeCameraLib

LIBS += -L$$PWD/../EyeGazeTrackerLib -lEyeGazeTracker
LIBS += -L$$PWD/../EyeGazeDetectionLib/ -lEyeGazeTrackerDetection
LIBS += -L$$PWD/../EyeGazeCalibrationLib/ -lEyeGazeTrackerCalibration
LIBS += -L$$PWD/../EyeGazeCameraLib/ -lEyeGazeTrackerCamera

LIBS += -L$$PWD/../lib/opencv/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_calib3d -lopencv_objdetect -lopencv_imgcodecs -lopencv_videoio -lopencv_gpu
LIBS += -L$$PWD/../lib/boost/lib -lboost_thread -lboost_system -lboost_timer
LIBS += -L$$PWD/../lib/ffmpeg/lib -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale

}


RESOURCES += \
    $${SRCDIR}gazetrackguiresources.qrc
