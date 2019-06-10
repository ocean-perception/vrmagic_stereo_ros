// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstring>

#include "vrmagic_driftcam/api_handle.h"


Driftcam::ApiHandle::ApiHandle(const std::string& cam1_serial, const std::string& cam2_serial) {
    cam1_serial_ = cam1_serial;
    cam2_serial_ = cam2_serial;
    update();
}

void Driftcam::ApiHandle::update() {
    // check for connected devices
    VRmDWORD size;
    VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListSize(&size));
    if (size > 0) {

        VRmDeviceKey* p_device_key;
        VRmSTRING p_device_str;

        bool cam1_found = false;
        bool cam2_found = false;

        for (VRmDWORD i = 0; i < size; ++i) {
            VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListEntry(i, &p_device_key));
            VRMEXECANDCHECK(VRmUsbCamGetSerialString(p_device_key, &p_device_str));
            if (std::strcmp(p_device_str, cam1_serial_.c_str()) == 0) {
                if (!cam1_.opened()) {
                    cam1_.open(p_device_key, p_device_str);
                }
                cam1_found = true;
            }
            if (std::strcmp(p_device_str, cam2_serial_.c_str()) == 0) {
                if (!cam2_.opened()) {
                    cam2_.open(p_device_key, p_device_str);
                }
                cam2_found = true;
            }
            VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_device_key));
        }

        if (!cam1_found && cam1_.opened()) {
            std::cout << "Camera 1 disconnected!" << std::endl;
            cam1_.disconnected();
        }
        if (!cam2_found && cam2_.opened()) {
            std::cout << "Camera 2 disconnected!" << std::endl;
            cam2_.disconnected();
        }
    } else {
        std::cout << "No devices found" << std::endl;
    }
}

