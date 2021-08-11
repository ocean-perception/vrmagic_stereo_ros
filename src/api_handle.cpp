// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstring>

#include "vrmagic_driftcam/api_handle.h"

Driftcam::ApiHandle::ApiHandle(
        const std::string& cam_serial, 
        const std::string& cam_path, 
        const std::string& mission_name, 
        bool open_on_start) {
    cam_serial_ = cam_serial;
    cam_path_ = cam_path;
    mission_name__ = mission_name_;
    if (open_on_start) {
        open();
    }
}

void Driftcam::ApiHandle::update()
{
    // check for connected devices
    VRmDWORD size;
    VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListSize(&size));
    if (size > 0)
    {

        VRmDeviceKey *p_device_key;
        VRmSTRING p_device_str;

        bool cam_found = false;

        for (VRmDWORD i = 0; i < size; ++i)
        {
            VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListEntry(i, &p_device_key));
            VRMEXECANDCHECK(VRmUsbCamGetSerialString(p_device_key, &p_device_str));
            if (std::strcmp(p_device_str, cam_serial_.c_str()) == 0)
            {
                std::cout << "Found device: "
                          << " \n\t Key:    " << p_device_key->m_serial
                          << " \n\t Serial: " << p_device_str << std::endl;
                if (!cam_.opened())
                {
                    cam_.open(p_device_key, p_device_str);
                }
                cam_.setPath(cam_path_ + "/" + mission_name_ + "/" + cam_serial_);
                //cam_.setTriggerMode(Driftcam::TRIG_EXTERNAL);
                cam_found = true;
            }
            VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_device_key));
        }

        if (!cam_found && cam_.opened())
        {
            std::cout << "Camera " << cam_serial_ << " disconnected" << std::endl;
            cam_.disconnected();
        }
    }
    else
    {
        std::cout << "No " << cam_serial_ << " device found" << std::endl;
    }
}
