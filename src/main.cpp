// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <thread>
#include <signal.h>

#include <ros/ros.h>
#include <vrmagic_driftcam/StartAcquisition.h>

#include <vrmagic_driftcam/api_handle.h>

bool something_changed = false;
bool enable_acquisition = false;
std::string folder_name = "undefined";

static sig_atomic_t volatile g_request_shutdown = 0;

// Replacement SIGINT handler
static void mySigIntHandler(int sig) { g_request_shutdown = 1; }

// callback/event handling
static void VRmUsbCamCallbackProxy(VRmStaticCallbackType f_type, void *fp_user_data, const void *fcp_callback_params)
{
    switch (f_type)
    {
    case VRM_STATIC_CALLBACK_TYPE_DEVICE_CHANGE:
        if (fcp_callback_params)
        {
            VRmDeviceChangeType l_type = *reinterpret_cast<const VRmDeviceChangeType *>(fcp_callback_params);
            switch (l_type)
            {
            case VRM_DEVICE_CHANGE_TYPE_ARRIVAL:
                std::cout << "VRM_DEVICE_CHANGE_TYPE_ARRIVAL" << std::endl;
                something_changed = true;
                break;
            case VRM_DEVICE_CHANGE_TYPE_REMOVECOMPLETE:
                std::cout << "VRM_DEVICE_CHANGE_TYPE_REMOVECOMPLETE" << std::endl;
                something_changed = true;
                break;
            case VRM_DEVICE_CHANGE_TYPE_BUSY:
                std::cout << "VRM_DEVICE_CHANGE_TYPE_BUSY" << std::endl;
                break;
            default:
                std::cout << "Unknown device change type" << std::endl;
                break;
            }
        }
        break;
    default:
        std::cout << "Unknown callback type" << std::endl;
        break;
    }
}

bool startAcquisitionCb(vrmagic_driftcam::StartAcquisition::Request &request, vrmagic_driftcam::StartAcquisition::Response &response)
{
    if (!enable_acquisition)
    {
        ROS_INFO("Starting acquisition");
        enable_acquisition = true;
        folder_name = request.folder_name;
        response.success = true;
    }
    return response;
}

bool stopAcquisitionCb(std_srvs::Empty::Request &request, std_srvs::Empty::Response &response)
{
    if (enable_acquisition)
    {
        ROS_INFO("Stopping acquisition");
        enable_acquisition = false;
    }
    return true;
}

int main(int argc, char **argv)
{
    // at first, be sure to call VRmUsbCamCleanup() at exit, even in case
    // of an error
    atexit(VRmUsbCamCleanup);
    signal(SIGINT, mySigIntHandler);
    signal(SIGSEGV, mySigIntHandler);

    // Init ROS node
    ros::init(argc, argv, "vrmagic_stereo_ros", ros::init_options::NoSigintHandler);
    ros::NodeHandle nh;
    ros::NodeHandle nhp("~");

    // Provide a start and stop service
    ros::ServiceServer start_acquisition_srv = nhp.advertiseService<vrmagic_driftcam::StartAcquisition>("start_acquisition", startAcquisitionCb);
    ros::ServiceServer stop_acquisition_srv = nhp.advertiseService<std_srvs::Empty>("stop_acquisition", stopAcquisitionCb);

    // Read the save path from the launchfile
    std::string save_path;

    if (!nhp.hasParam("save_path"))
    {
        ROS_INFO("No param named 'save_path'");
        return -1;
    }
    nhp.getParamCached("save_path", save_path);

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

    Driftcam::ApiHandle api(cam_serial, save_path, mission_name, enable_acquisition);
    VRmUsbCamRegisterStaticCallback(VRmUsbCamCallbackProxy, 0);

    while (ros::ok() && g_request_shutdown == 0)
    {
        if (enable_acquisition)
        {

            if (!api.isOpen())
            {
                std::cout << "Reopening device " << cam_serial << std::endl;
                api.open();
            }
            std::cout << "API Grabbing" << std::endl;
            api.setFoldername(folder_name);
            int images_available = api.grab();
        }
        ros::spinOnce();
    }
    api.close();
    VRmUsbCamCleanup();
    ros::shutdown();
    return 0;
}
