cmake_minimum_required(VERSION 2.8.3)

project(vrmagic_driftcam)

set(VRMAGICUSB2_LIBRARIES "C:/Program Files (x86)/Common Files/VRmagic/VRmUsbCamSDK/lib/x64/vrmusbcam2.lib")
set(VRMAGICUSB2_INCLUDE_DIRS "C:/Program Files (x86)/Common Files/VRmagic/VRmUsbCamSDK/include")

find_package(Boost REQUIRED system)

include_directories(
  include
  ${VRMAGICUSB2_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)


add_executable(vrmagic_driftcam src/main.cpp 
								src/camera_handle.cpp 
								src/serial_port.cpp)


target_link_libraries(vrmagic_driftcam 
					  ${VRMAGICUSB2_LIBRARIES} 
					  ${Boost_LIBRARIES})


