// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <thread>

#include <vrmagic_driftcam/camera_handle.h>
#include <vrmagic_driftcam/serial_port.h>


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
    serial_port.open("COM4");
    serial_port.setBaudRate(Driftcam::EBaudRate::BAUD_9600);
    serial_port.setCharSize(Driftcam::ECharacterSize::CHAR_SIZE_8);
    serial_port.setNumOfStopBits(Driftcam::EStopBits::STOP_BITS_1);
    serial_port.setParity(Driftcam::EParity::PARITY_NONE);
    serial_port.setFlowControl(Driftcam::EFlowControl::FLOW_CONTROL_NONE);

    int sp_timeout_ms = 1000;

    Driftcam::CameraHandle cam1("QERQR5");
    Driftcam::CameraHandle cam2("FEQH45");

    int i = 0;
    while (i < 10) {
        std::string line = serial_port.readLine(sp_timeout_ms) ;
        std::cout << "Received: " << line << "\n";

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
