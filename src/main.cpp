// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <thread>

#include <ros/ros.h>
#include <std_srvs/Empty.h>

#include <vrmagic_driftcam/api_handle.h>

bool something_changed = false;
bool enable_acquisition = false;

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

bool startAcquisitionCb(std_srvs::Empty::Request &request, std_srvs::Empty::Response &response)
{
    if (!enable_acquisition)
    {
        ROS_INFO("Starting acquisition");
        enable_acquisition = true;
    }
    return true;
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
    //VRmUsbCamRegisterStaticCallback(VRmUsbCamCallbackProxy, 0);

    //ros::Rate loop_rate(2);
    while (!g_request_shutdown)
    {

        if (enable_acquisition)
        {
            /*
            if (!api.isOpen())
            {
                api.open();
            }

            VRMEXECANDCHECK(VRmUsbCamUpdateDeviceKeyList());
            if (something_changed)
            {
                api.update();
                something_changed = false;
            }
            */

            // api.trigger();
            api.grab();
        }
        /*
        else
        {
            if (api.isOpen())
            {
                api.close();
            }
            ROS_INFO("Waiting for start acquisition service call");
        }
        */

        ros::spinOnce();
        //loop_rate.sleep();
    }
    VRmUsbCamCleanup();
    ros::shutdown();
    return 0;
}
