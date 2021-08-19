// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <fstream>
#include <thread>
#include <signal.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <ros/ros.h>
#include <std_srvs/Empty.h>
#include <vrmagic_stereo_ros/StartAcquisition.h>

#include <vrmagic_driftcam/api_handle.h>

bool something_changed = false;
bool enable_acquisition = false;
std::string folder_name = "undefined";
std::string watchman_folder = "/media/driftcam/sdcard/watchman";
std::string data_folder = "/media/driftcam/sdcard/data/";

void removeWatchmanFile() 
{
    boost::filesystem::path wf(watchman_folder + "/path.txt");
    if (boost::filesystem::is_regular_file(wf))
        boost::filesystem::remove(wf);
}

void createWatchmanFile(const std::string& folder_name) 
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

    // Init ROS node
    ros::init(argc, argv, "vrmagic_stereo_ros", ros::init_options::NoSigintHandler);
    ros::NodeHandle nh;
    ros::NodeHandle nhp("~");

    // Provide a start and stop service
    ros::ServiceServer start_acquisition_srv = nhp.advertiseService("start_acquisition", startAcquisitionCb);
    ros::ServiceServer stop_acquisition_srv = nhp.advertiseService("stop_acquisition", stopAcquisitionCb);

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
            api.grab();
        }
        ros::spinOnce();
    }
    api.close();
    VRmUsbCamCleanup();
    ros::shutdown();
    return 0;
}
