#ifndef EYEGAZETRACKER_H
#define EYEGAZETRACKER_H
#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../UtilLib/EyeGazeCommon.h"
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/numeric/ublas/matrix.hpp>

typedef boost::function<void(int, cv::Point2f)> calibstart_callback;
typedef boost::function<void(int, cv::Point2f)> calibstop_callback;
typedef boost::function<void(int, \
							std::vector<cv::Point2f>, \
							std::vector<cv::Point2f>, \
							std::vector<cv::Point2f>, \
							std::vector<bool>, \
							std::vector<bool>)> calibresults_callback;

typedef boost::function<void(rclgaze::OpenAPIDataRecord)> gazedata_callback;//rclgaze::timeData, \

typedef boost::function<void(rclgaze::log_data)> gazelog_callback;

typedef boost::function<void(std::vector<boost::numeric::ublas::matrix<double>>, \
	std::vector<boost::numeric::ublas::matrix<double>>, \
	int, int, int, int)> caliblog_callback;

typedef boost::function<void(std::vector<std::vector<cv::Vec2f>>, \
	std::vector<std::vector<cv::Vec2f>>, \
	std::vector<std::vector<cv::Vec2f>>, \
	std::vector<std::vector<cv::Vec2f>>, \
	int)> headcaliblog_callback;

class EYEGAZETRACKER_API EyeTracker
{
public:
	EyeTracker(std::string settings_directory);
	~EyeTracker();
	
	bool setInputSource(int deviceId, rclgaze::eyeSide side);
	bool setInputSource(const char* filename, rclgaze::eyeSide side);
	void removeInputSource(rclgaze::eyeSide side);
	bool start();
	void stop();
	bool getFrame(cv::Mat& ret, rclgaze::eyeSide side);
	bool getPupil(cv::RotatedRect& retEllipse, rclgaze::eyeSide side);
	bool getGlints(std::vector<cv::Point2f>& retGlintList, rclgaze::eyeSide side);
	cv::Vec2f getCalibrationError(rclgaze::eyeSide side);
	int getCalibrationNumValid(rclgaze::eyeSide side);
	void startLogging(bool log_data, bool record_video, std::string directory);
	void stopLogging();
	// Calibration
	void removeCalibrationTargetData(int targetID);
	void initCalibration(int numTargets, rclgaze::eyeSide side);
	bool finalizeCalibration(rclgaze::eyeSide side);
	void startCalibrating(int targetID, float targetX, float targetY);
	void startCalibrating(float targetX, float targetY, bool accuracyTest);
	bool stopCalibrating();

	// Recalibration
	void initReCalibration(bool clearError = true);
	void startRecalibration(cv::Point2f target);
	bool stopReCalibration(cv::Point2f targetPosition, int headIndex);
	void finalizeReCalibration();

	// mapping pupil pog
	void map_pupilPOG();

	// set glint template
	void setGlintTemplate(cv::Vec2f gl01, cv::Vec2f gl02, cv::Vec2f gr01, cv::Vec2f gr02);

	// recording
	void startRecording(std::string directory);
	void stopRecording();
	void saveFrame();
	bool recordingFlag;
	cv::VideoWriter video;

	bool isRunning();
	bool isCalibInitialized();
	bool isCalibrated();
	bool isLogging();
	bool getPOG(cv::Point2f& pog, rclgaze::eyeSide side);
	float getFPS();
	int getTestResR();
	int getTestResL();

	void registerCalibrationCallbacks(calibstart_callback cb_start_calib, calibstop_callback cb_stop_calib, calibresults_callback cb_calib_results);
	void registerStreamCallback(gazedata_callback cb);
	void registerLogCallback(gazelog_callback cb);
	void registerCalibLogCallback(caliblog_callback cb);
	void registerHEADCalibLogCallback(headcaliblog_callback cb);
};

#endif