#pragma once

#include <opencv2/core/core.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BRIGHTNESS_FACTOR 30
#define UPPER_THRESHOLD_VALUE 5
#define LOWER_THRESHOLD_VALUE 2
#define MAUKEN_THRESHOLD 1500
// turn flipdot support on and off
#define AWESOME_OPTION true

class DepthCamera;
class KinectMotor;

class Application
{
public:
	Application();
	virtual ~Application();

	void loop();

	void processFrame();

	void makeScreenshots();
	void clearOutputImage();

	void captureBackground();

	bool isFinished();

protected:
    int count = 0;
	DepthCamera *m_depthCamera;
	KinectMotor *m_kinectMotor;

	cv::Mat m_bgrImage;
	cv::Mat m_refImage;
	cv::Mat m_depthImage;
	cv::Mat m_outputImage;
    cv::Mat m_drawImage;
	cv::Mat m_flipdotImage;

	bool m_isFinished;

#if AWESOME_OPTION
    struct sockaddr_in addr;
	uint8_t buff[50];

	int m_socket;
#endif

	// don't transfer too many frames to the flipdot
	clock_t flipdot_lastTime;
	clock_t startPerfTime;

	void transferFrameToFlipdot();
};