# RCL Eye Gaze Tracker

This is an eye gaze tracking solution designed for the surgeon console of the da Vinci surgical robot platforms. This eye gaze tracker determines where a surgeon is looking within the surgical scene.

The software provided here open-source is the UI front-end component of the eye gaze tracker software. A license must be purchased from the UBC RCL for access to the gaze tracking libraries which are needed in order to run the eye gaze tracker.

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Install Qt and Qt Creator. You can download Qt from https://www.qt.io/download.

Not included within this Github repository are the library files for required modules. Other than our eye gaze tracking library, the required dependencies are Boost, Ffmpeg, and OpenCV. We have packaged the necessary library modules into a single zip file named lib.zip. This can be downloaded from the RCL website (link to follow).

You may replace these library files with updated versions of each module. However, note that on Windows we have used the MSVC2015 64-bit compiler and on Ubuntu we use GCC, and we have been developing with the following versions:

* Ffmpeg 4.0.2
* OpenCV 3.3
* Boost 1.66
* Qt 5.9.3

### Setup

First, download or clone this repository. Then, you will need to get the lib files for eye gaze tracking and other dependencies from the RCL website (link to follow) and unpackage the files into the lib folder. 

#### Windows

Copy the following *.dll files from the lib folder into the build/release folder:
* avcodec-58.dll
* avdevice-58.dll
* avfilter-7.dll
* avformat-58.dll
* avutil-56.dll
* swresample-3.dll
* swscale-5.dll
* postproc-55.dll
* boost_chrono-vc140-mt-x64-1_66.dll
* boost_system-vc140-mt-x64-1_66.dll
* boost_thread-vc140-mt-x64-1_66.dll
* EyeGazeCalibrationLib.dll
* EyeGazeCameraLib.dll
* EyeGazeDetectionLib.dll
* EyeGazeTrackerLib.dll
* Qt5Core.dll
* Qt5Gui.dll
* Qt5Network.dll
* Qt5Widgets.dll

#### Ubuntu


### Deployment

You should now open the Qt project (src/gazeTrackGUI_release.pro) in Qt Creator. Set the project to use the appropriate compiler and build the project in Release mode. The output of the build will be contained in the build/release folder. The output executable is named gazeTrackGUI.

## Authors

* Irene Tong
* Maxwell Li

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the ___ License - see the [LICENSE.md](LICENSE.md) file for details

