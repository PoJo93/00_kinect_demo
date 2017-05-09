///////////////////////////////////////////////////////////////////////////
//
// Main class for HCI2 assignments
// Authors:
//		Stefan Neubert (2015)
//		Stephan Richter (2011)
//		Patrick Löhne (2012)
//
///////////////////////////////////////////////////////////////////////////

#include "Application.h"

#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "framework/DepthCamera.h"
#include "framework/KinectMotor.h"


void Application::processFrame()
{
    m_depthImage *= BRIGHTNESS_FACTOR;
    // remove the background
    cv::absdiff(m_depthImage, m_refImage, m_depthImage);

    m_depthImage.convertTo(m_outputImage, CV_8UC1, (float)1/256);

    // threshold anything that is located to high
    cv::threshold(m_outputImage, m_outputImage, UPPER_THRESHOLD_VALUE, 255, CV_THRESH_TOZERO_INV);

    // filter noise
    cv::blur(m_outputImage, m_outputImage, cv::Size(15,15));

    // threshold actual touch
    cv::threshold(m_outputImage, m_outputImage, LOWER_THRESHOLD_VALUE, 255, CV_THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    // find the conturs
    cv::findContours(m_outputImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    std::vector<cv::RotatedRect> ellipses( contours.size() );

    for (int i=0; i<contours.size(); i++) {
        cv::Scalar contour_color = cv::Scalar( 128 );
        cv::Scalar ellipse_color = cv::Scalar( 255 );
        cv::drawContours(m_outputImage, contours, i, contour_color, 2);
        if (contours[i].size() > 5) {
            ellipses[i] = cv::fitEllipse(contours[i]);
            float area = CV_PI * (ellipses[i].size.height / 2.0) * (ellipses[i].size.width/2.0);
            if (area > MAUKEN_THRESHOLD) {
                cv::ellipse(m_outputImage, ellipses[i], ellipse_color, 2);
                cv::circle(m_drawImage, ellipses[i].center, 7, ellipse_color, 14);
            }
        }
    }

	///////////////////////////////////////////////////////////////////////////
	//
	// To do:
	//
	// This method will be called every frame of the camera. Insert code here in
	// order to fulfill the assignment. These images will help you doing so:
	//
	// * m_bgrImage: The image of the Kinect's color camera
	// * m_depthImage: The image of the Kinects's depth sensor
	// * m_outputImage: The image in which you can draw the touch circles.
	//
	///////////////////////////////////////////////////////////////////////////

	// Sample code brightening up the depth image to make the values visible
	//m_depthImage *= 10;
}

void Application::loop()
{
	int key = cv::waitKey(40);
	switch (key)
	{
	case 'q': // quit
		m_isFinished = true;
		break;
	case 's': // screenshot
		makeScreenshots();
		break;
	}
	m_depthCamera->getFrame(m_bgrImage, m_depthImage);
	processFrame();

	cv::imshow("bgr", m_bgrImage);
	// cv::imshow("depth", m_depthImage);
	// cv::imshow("output", m_outputImage);
    cv::imshow("artwork", m_drawImage);

	count++;
	if (count == 1) {
		startPerfTime = clock();
	} else if (count == 10) {
		double time = double(clock() - startPerfTime) / CLOCKS_PER_SEC;
		double FPS = 10.0 / time;
		std::cout << "Current FPS is " << FPS << std::endl;
		count = 0;
	}


	// update (if necessary the flipdot)
#if AWESOME_OPTION
	if (double(clock() - flipdot_lastTime) / CLOCKS_PER_SEC > 0.2) {
		flipdot_lastTime = clock();
		transferFrameToFlipdot();
	}
#endif
}

void Application::makeScreenshots()
{
	cv::imwrite("color.png", m_bgrImage);
	cv::imwrite("depth.png", m_depthImage);
	cv::imwrite("output.png", m_outputImage);
}

// Capture a background image as reference
void Application::captureBackground()
{
    m_depthCamera->getFrame(m_bgrImage, m_refImage);
    m_refImage *= BRIGHTNESS_FACTOR;
}

Application::Application()
	: m_isFinished(false)
	, m_depthCamera(nullptr)
	, m_kinectMotor(nullptr)
{
	// If you want to control the motor / LED

	m_depthCamera = new DepthCamera;

	// open windows
	cv::namedWindow("output", 1);
	cv::namedWindow("depth", 1);
	cv::namedWindow("bgr", 1);
    cv::namedWindow("artwork", 1);
	cv::namedWindow("flipdot", cv::WINDOW_NORMAL);

    // create work buffer
	m_bgrImage = cv::Mat(480, 640, CV_8UC3);
	m_depthImage = cv::Mat(480, 640, CV_16UC1);
	m_outputImage = cv::Mat(480, 640, CV_8UC1);
    m_refImage = cv::Mat(480, 640, CV_16UC1);
    // init a black output image
    m_drawImage = cv::Mat(480, 640, CV_8UC1, cv::Scalar(0));
    // init a black flipdot image
    m_flipdotImage = cv::Mat(25, 16, CV_8UC1, cv::Scalar(0));

    // capture the background image
    captureBackground();

	flipdot_lastTime = clock();

#if AWESOME_OPTION
    memset(buff, 0, 50);
	addr.sin_family = AF_INET;
	inet_aton("192.168.178.161", &addr.sin_addr);
	addr.sin_port = htons(1234);

	m_socket = socket(PF_INET, SOCK_DGRAM, 0);
	if (m_socket == -1) { perror("socket error"); exit(1); }
#endif
}

Application::~Application()
{
	if (m_depthCamera) delete m_depthCamera;
	if (m_kinectMotor) delete m_kinectMotor;
}

bool Application::isFinished()
{
	return m_isFinished;
}

void Application::transferFrameToFlipdot() {
	cv::resize(m_drawImage, m_flipdotImage, cv::Size(25,16), 0, 0, CV_INTER_CUBIC);
	cv::threshold(m_flipdotImage, m_flipdotImage, 128, 255, CV_THRESH_BINARY);
	cv::Mat demoImage;
	cv::resize(m_flipdotImage, demoImage, cv::Size(250,160), 0, 0, cv::INTER_NEAREST);
	cv::imshow("flipdot", demoImage);

	for(int x=0; x<25; x++) {
		uint8_t upper = 0, lower = 0;
		for (int y=0; y<8; y++) {
			upper |= ((m_flipdotImage.at<uchar>(y,x) == 255) ? 1 : 0) << (8-y);
		}
		for (int y=8; y<16; y++) {
			lower |= ((m_flipdotImage.at<uchar>(y,x) == 255) ? 1 : 0) << (8-(y-8));
		}
		buff[49 - x*2] = upper;
		buff[48 - (x*2)] = lower;

		std::bitset<8> upper_bin(buff[2*x]);
		std::bitset<8> lower_bin(buff[2*x+1]);
		std::cout << upper_bin << lower_bin << std::endl;
	}

	std::cout << std::endl << std::endl << std::endl;

	int ret = sendto(m_socket, buff, 50, 0, (struct sockaddr *)&addr, sizeof(addr));
}