// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <thread>

#include <ros/ros.h>

#include <vrmagic_driftcam/api_handle.h>

bool something_changed = false;

// callback/event handling
static void VRmUsbCamCallbackProxy(VRmStaticCallbackType f_type, void* fp_user_data, const void* fcp_callback_params) {
    switch (f_type) {
    case VRM_STATIC_CALLBACK_TYPE_DEVICE_CHANGE:
        if (fcp_callback_params) {
            VRmDeviceChangeType l_type= *reinterpret_cast<const VRmDeviceChangeType*>(fcp_callback_params);
            switch (l_type) {
                case VRM_DEVICE_CHANGE_TYPE_ARRIVAL:
                    //std::cout << "VRM_DEVICE_CHANGE_TYPE_ARRIVAL" << std::endl;
                    something_changed = true;
                    break;
                case VRM_DEVICE_CHANGE_TYPE_REMOVECOMPLETE:
                    //std::cout << "VRM_DEVICE_CHANGE_TYPE_REMOVECOMPLETE" << std::endl;
                    something_changed = true;
                    break;
                case VRM_DEVICE_CHANGE_TYPE_BUSY:
                    //std::cout << "VRM_DEVICE_CHANGE_TYPE_BUSY" << std::endl;
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

int main(int argc, char** argv) {
    // at first, be sure to call VRmUsbCamCleanup() at exit, even in case
    // of an error
    atexit(VRmUsbCamCleanup);

    // Init ROS node
    ros::init(argc, argv, "vrmagic_stereo_ros");
    ros::NodeHandle nh;
    ros::NodeHandle nhp("~");

    std::string save_path;

    if (!nhp.hasParam("save_path")) {
        ROS_INFO("No param named 'save_path'");
        return -1;
    }

    nhp.getParam("save_path", save_path);

    std::cout << "Param val" << save_path << std::endl;

    // read libversion (for informational purposes only)
    VRmDWORD libversion;
    VRMEXECANDCHECK(VRmUsbCamGetVersion(&libversion));

    std::cout << "========================================================\n";
    std::cout << "===                 VRmagic Driftcam                 ===\n";
    std::cout << "========================================================\n";
    std::cout << "VRmUsbCam2 C API (v." << libversion << ")\n\n";

    int sp_timeout_ms = 1000;

    std::string cam1_serial("QERQR5");
    std::string cam2_serial("FEQH45");

    Driftcam::ApiHandle api(
        cam1_serial, save_path, 
        cam2_serial, save_path);
    VRmUsbCamRegisterStaticCallback(VRmUsbCamCallbackProxy, 0);

    //cam1.listAllProperties();
    //cam2.listAllProperties();

    ros::Rate loop_rate(10);

    int count = 0;
    while(ros::ok()) {
        VRMEXECANDCHECK(VRmUsbCamUpdateDeviceKeyList());
        if (something_changed) {
            api.update();
            something_changed = false;
        }


        api.trigger();
        api.grab();

        ros::spinOnce();
        loop_rate.sleep();
        
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        count++;
    }
    return 0;
}
