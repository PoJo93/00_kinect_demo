cmake_minimum_required(VERSION 3.0)
project(00_kinect_demo)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Kinect REQUIRED)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES
        framework/DepthCamera.cpp
        framework/DepthCamera.h
        framework/DepthCameraException.cpp
        framework/DepthCameraException.h
        framework/KinectMotor.cpp
        framework/KinectMotor.h
        Application.cpp
        Application.h
        main.cpp)



add_executable(00_kinect_demo ${SOURCE_FILES})

target_include_directories(00_kinect_demo PRIVATE ${KINECT_INCLUDES})

target_link_libraries(00_kinect_demo opencv_core opencv_imgproc opencv_calib3d opencv_video opencv_features2d opencv_ml opencv_highgui opencv_objdetect opencv_contrib opencv_legacy opencv_gpu ${KINECT_LIBRARY})
