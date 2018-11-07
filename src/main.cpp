// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <chrono>
#include <thread>

#include "vrmagic_driftcam/camera_handle.h"

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

    Driftcam::CameraHandle cam1("QERQR5");
    Driftcam::CameraHandle cam2("FEQH45");

    std::thread t[2];


    int i = 0;
    while (i < 10) {
        cam1.trigger();
        cam2.trigger();
        cam1.grab();
        cam2.grab();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        i++;
    }

    cam1.stop();
    cam2.stop();
    cam1.close();
    cam2.close();
    return 0;
}
