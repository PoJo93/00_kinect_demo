
find_path(OPENNI_INCLUDE_DIR XnCppWrapper.h
        PATH_SUFFIXES ni)

find_path(NITE_INCLUDE_DIR XnVNite.h
        PATH_SUFFIXES nite)

find_library(xn_openni NAMES libOpenNI.dylib)
find_library(xn_core NAMES libXnCore.dylib)
find_library(xn_ddk NAMES libXnDDK.dylib)
find_library(xn_devicefile NAMES libXnDeviceFile.dylib PATHS)
find_library(xn_sensorkm NAMES libXnDeviceSensorV2KM.dylib)
find_library(xn_formats NAMES libXnFormats.dylib)
find_library(xn_nite NAMES libXnVCNITE_1_5_2.dylib)
find_library(xn_features NAMES libXnVFeatures_1_5_2.dylib)
find_library(xn_hand NAMES libXnVHandGenerator_1_5_2.dylib)
find_library(xn_vnite NAMES libXnVNite_1_5_2.dylib)
find_library(xn_codecs NAMES libnimCodecs.dylib)
find_library(xn_nodes NAMES libnimMockNodes.dylib)
find_library(xn_recorder NAMES libnimRecorder.dylib)
find_library(xn_usb NAMES libusb-1.0.0.dylib)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Kinect  DEFAULT_MSG
        OPENNI_INCLUDE_DIR
        NITE_INCLUDE_DIR
        xn_openni
        xn_core
        xn_ddk
        xn_devicefile
        xn_sensorkm
        xn_formats
        xn_nite
        xn_features
        xn_hand
        xn_vnite
        xn_codecs
        xn_nodes
        xn_recorder
        xn_usb)

set(KINECT_INCLUDES
        ${OPENNI_INCLUDE_DIR}
        ${NITE_INCLUDE_DIR})

set(KINECT_LIBRARY
        ${xn_openni}
        ${xn_core}
        ${xn_ddk}
        ${xn_devicefile}
        ${xn_sensorkm}
        ${xn_formats}
        ${xn_nite}
        ${xn_features}
        ${xn_hand}
        ${xn_vnite}
        ${xn_codecs}
        ${xn_nodes}
        ${xn_recorder}
        ${xn_usb})
