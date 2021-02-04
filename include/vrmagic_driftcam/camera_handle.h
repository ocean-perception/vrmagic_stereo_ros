// Copyright (c) 2018 University of Southampton

#ifndef INCLUDE_VRMAGIC_DRIFTCAM_CAMERA_HANDLE_H_
#define INCLUDE_VRMAGIC_DRIFTCAM_CAMERA_HANDLE_H_

#include <iostream>
#include <vector>
#include <string>

#include  <vrmusbcam2.h>

void LogExit();

// small helper macro to check function for success and call LogExit
inline void VRMEXECANDCHECK(const int& function) {
    if (VRM_SUCCESS != function) LogExit();
}

namespace Driftcam {

enum TriggerMode {
    TRIG_FREERUNNING = 0,
    TRIG_EXTERNAL = 1,
    TRIG_SOFTWARE = 2,
    TRIG_FREERUNNING_SEQUENTIAL = 3
};

class CameraHandle {
 public:
    CameraHandle();
    CameraHandle(const std::string& camera_serial);
    void start(const std::string& camera_serial);
    void open(const VRmDeviceKey* p_device_key,
              const VRmSTRING& p_device_str);
    void stop();
    void close();
    void grab();
    void trigger();
    double getCurrentTime();
    float getExposure();
    double getLastExternalTriggerTimestamp();
    bool setExposure(float exposure);
    bool setGain(int gain);
    bool setFramerate(float framerate);
    bool setMaxSupportedFramerate();
    void printColorFormats();
    float getMaxSupportedFramerate();
    float getFramerate();
    bool setTriggerMode(const TriggerMode& triggermode);
    VRmPropId getTriggerMode();
    std::string colorToString(const VRmColorFormat& fmt);
    void listAllProperties();
    bool opened() {return opened_;}
    void disconnected() {opened_ = false;}
    void setPath(const std::string& path) {path_ = path;}

 private:
    VRmUsbCamDevice device_;
    VRmDWORD port_;
    std::string identifier_;
    std::string path_;
    double clock_epoch_;
    bool setPropertyPort();
    bool verifyFrameRate();
    bool opened_;
};  // class CameraHandle

}  // namespace Driftcam

#endif  // INCLUDE_VRMAGIC_DRIFTCAM_CAMERA_HANDLE_H_
