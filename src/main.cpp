// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <thread>

#include <vrmagic_driftcam/api_handle.h>
#include <vrmagic_driftcam/serial_port.h>

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

    // read libversion (for informational purposes only)
    VRmDWORD libversion;
    VRMEXECANDCHECK(VRmUsbCamGetVersion(&libversion));

    std::cout << "========================================================\n";
    std::cout << "===                 VRmagic Driftcam                 ===\n";
    std::cout << "========================================================\n";
    std::cout << "VRmUsbCam2 C API (v." << libversion << ")\n\n";

    Driftcam::SerialPort serial_port;
    bool arduino_available = false;
    /*try {
        serial_port.open("COM4");
        serial_port.setBaudRate(Driftcam::EBaudRate::BAUD_9600);
        serial_port.setCharSize(Driftcam::ECharacterSize::CHAR_SIZE_8);
        serial_port.setNumOfStopBits(Driftcam::EStopBits::STOP_BITS_1);
        serial_port.setParity(Driftcam::EParity::PARITY_NONE);
        serial_port.setFlowControl(Driftcam::EFlowControl::FLOW_CONTROL_NONE);
        arduino_available = true;
    } catch (const std::exception &e) {
        std::cout << e.what () << '\n';
    }
    */

    int sp_timeout_ms = 1000;

    std::string cam1_serial("QERQR5");
    std::string cam2_serial("FEQH45");

    Driftcam::ApiHandle api(cam1_serial, cam2_serial);
    VRmUsbCamRegisterStaticCallback(VRmUsbCamCallbackProxy, 0);

    //cam1.listAllProperties();
    //cam2.listAllProperties();

    int i = 0;
    for(;;) {
        VRMEXECANDCHECK(VRmUsbCamUpdateDeviceKeyList());
        if (something_changed) {
            api.update();
            something_changed = false;
        }

        if (arduino_available) {
            std::string line = serial_port.readLine(sp_timeout_ms) ;
            std::cout << "Received: " << line << "\n";
        }

        api.trigger();
        api.grab();

        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }
    return 0;
}
