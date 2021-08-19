cmake_minimum_required(VERSION 3.5.0)
set (CMAKE_CXX_STANDARD 11)

project(vrmagic_stereo_ros)

set(VRMAGICUSB2_LIBRARIES "/usr/lib/libvrmusbcam2.so.3")
set(VRMAGICUSB2_INCLUDE_DIRS "external/VRmUsbCamSDK/include external/VRmUsbCamSDK/wrappers/c++/")

find_package(catkin REQUIRED COMPONENTS
  # ROS Dependencies
  roscpp
  std_msgs
  std_srvs
  message_generation
)

find_package(Boost REQUIRED filesystem system)

add_service_files(
  DIRECTORY srv
  FILES StartAcquisition.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

# Declare things to be passed to dependent projects
catkin_package(
   INCLUDE_DIRS include
   CATKIN_DEPENDS roscpp message_runtime std_msgs std_srvs
)

include_directories(
  include
  external/VRmUsbCamSDK/include
  external/VRmUsbCamSDK/wrappers/c++
  ${Boost_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

add_library(
  vrmusbcamcpp
  external/VRmUsbCamSDK/wrappers/c++/vrmusbcamcpp.cpp
)

add_executable(
  vrmagic_stereo_ros 
  src/main.cpp
  src/api_handle.cpp
  src/camera_handle.cpp
)

add_dependencies(vrmagic_stereo_ros ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(
  vrmagic_stereo_ros
  vrmusbcamcpp
  ${VRMAGICUSB2_LIBRARIES}
  ${Boost_LIBRARIES}
  ${catkin_LIBRARIES}
)

install(
  TARGETS vrmagic_stereo_ros
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
