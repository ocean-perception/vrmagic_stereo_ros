// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <fstream>
#include <thread>
#include <signal.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <std_srvs/Empty.h>
#include <vrmagic_stereo_ros/StartAcquisition.h>

#include <vrmagic_driftcam/api_handle.h>

bool something_changed = false;
bool enable_acquisition = false;
std::string folder_name = "undefined";
std::string watchman_folder = "/media/driftcam/MICROSD/watchman";
std::string data_folder = "/media/driftcam/MICROSD/data/";

void removeWatchmanFile()
{
    boost::filesystem::path wf(watchman_folder + "/path.txt");
    if (boost::filesystem::is_regular_file(wf))
        boost::filesystem::remove(wf);
}

bool watchmanFileExists()
{
    boost::filesystem::path wf(watchman_folder + "/path.txt");
    return boost::filesystem::is_regular_file(wf);
}

void mySigintHandler(int sig)
{
    ROS_INFO("Cleaning up...");
    removeWatchmanFile();
    // api.close();
    VRmUsbCamCleanup();
    ros::shutdown();
}

void createWatchmanFile(const std::string &folder_name)
{
    std::ofstream myfile;
    myfile.open(watchman_folder + "/path.txt");
    myfile << data_folder << folder_name << "\nFEQH45\nQERQR5";
    myfile.close();
}

bool startAcquisitionCb(vrmagic_stereo_ros::StartAcquisition::Request &request, vrmagic_stereo_ros::StartAcquisition::Response &response)
{
    response.success = false;
    if (!enable_acquisition)
    {
        ROS_INFO("Starting acquisition");
        folder_name = request.folder_name;
        ros::NodeHandle nh;
        nh.setParam("/driftcam/save_image_dir", folder_name);
        createWatchmanFile(folder_name);
        enable_acquisition = true;
        response.success = true;
    }
    return true;
}

bool stopAcquisitionCb(std_srvs::Empty::Request &request, std_srvs::Empty::Response &response)
{
    if (enable_acquisition)
    {
        ROS_INFO("Stopping acquisition");
        removeWatchmanFile();
        enable_acquisition = false;
    }
    return true;
}

int main(int argc, char **argv)
{
    // at first, be sure to call VRmUsbCamCleanup() at exit, even in case
    // of an error
    atexit(VRmUsbCamCleanup);

    signal(SIGINT, mySigintHandler);

    // Init ROS node
    ros::init(argc, argv, "vrmagic_stereo_ros", ros::init_options::NoSigintHandler);
    ros::NodeHandle nh;
    ros::NodeHandle nhp("~");

    // Provide a start and stop service
    ros::ServiceServer start_acquisition_srv = nhp.advertiseService("start_acquisition", startAcquisitionCb);
    ros::ServiceServer stop_acquisition_srv = nhp.advertiseService("stop_acquisition", stopAcquisitionCb);

    ros::Publisher image_saved_pub = nhp.advertise<std_msgs::String>("image_saved", 1);

    // Read the save path from the launchfile
    std::string save_path;

    if (!nhp.hasParam("save_path"))
    {
        ROS_INFO("No param named 'save_path'");
        return -1;
    }
    nhp.getParam("save_path", save_path);

    std::string cam_serial("QERQR5");
    if (!nhp.hasParam("serial"))
    {
        ROS_INFO("No param named 'serial'");
        return -1;
    }
    nhp.getParam("serial", cam_serial);

    if (!nhp.hasParam("enable_acquisition"))
    {
        ROS_INFO("No param named 'enable_acquisition'");
        return -1;
    }
    nhp.getParam("enable_acquisition", enable_acquisition);

    if (watchmanFileExists())
    {
        enable_acquisition = true;
        nh.getParam("/driftcam/save_image_dir", folder_name);
    }

    Driftcam::ApiHandle api(cam_serial, save_path, enable_acquisition);

    while (ros::ok())
    {

        if (enable_acquisition)
        {

            if (!api.isOpen())
            {
                api.open();
            }

            api.setFoldername(folder_name);
            std::string filename = api.grab();
            image_saved_pub.publish(filename);
        }
        ros::spinOnce();
    }
    return 0;
}
