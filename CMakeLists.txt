cmake_minimum_required(VERSION 3.10.0)
set (CMAKE_CXX_STANDARD 11)

project(vrmagic_driftcam)

set(VRMAGICUSB2_LIBRARIES "/usr/lib/libvrmusbcam2.so.3")
set(VRMAGICUSB2_INCLUDE_DIRS "external/VRmUsbCamSDK/include external/VRmUsbCamSDK/wrappers/c++/")

find_package(Boost REQUIRED system)

include_directories(
  include
  external/VRmUsbCamSDK/include
  external/VRmUsbCamSDK/wrappers/c++
  ${Boost_INCLUDE_DIRS}
)


add_library(vrmusbcamcpp external/VRmUsbCamSDK/wrappers/c++/vrmusbcamcpp.cpp)

add_executable(vrmagic_driftcam src/main.cpp
                src/api_handle.cpp
								src/camera_handle.cpp
								src/serial_port.cpp)
target_link_libraries(vrmagic_driftcam
                      vrmusbcamcpp
					  ${VRMAGICUSB2_LIBRARIES}
					  ${Boost_LIBRARIES})

