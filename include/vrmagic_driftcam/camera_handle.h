// Copyright (c) 2018 University of Southampton

#ifndef INCLUDE_VRMAGIC_DRIFTCAM_CAMERA_HANDLE_H_
#define INCLUDE_VRMAGIC_DRIFTCAM_CAMERA_HANDLE_H_

#include <iostream>
#include <vector>
#include <string>

#include  <vrmusbcam2.h>
#ifdef WIN32
#include <vrmusbcam2win32.h>
#endif

void LogExit();

// small helper macro to check function for success and call LogExit
#define VRMEXECANDCHECK(function) {if (VRM_SUCCESS != function) LogExit();}

namespace Driftcam {

enum TriggerMode {
    TRIG_INTERNAL = 0,
    TRIG_EXTERNAL = 1,
    TRIG_SOFTWARE = 2,
    FREERUNNING = 3
};

enum FormatMode {
    /// 32bit Pixel, Order A R G B using 32bit (DWORD) access
    VRM_ARGB_4X8,
    /// 24bit Pixel, Order B G R using 8bit (BYTE) access
    VRM_BGR_3X8,
    /// 16bit Pixel, Order R(5) G(6) B(5) using 16bit (WORD) access
    VRM_RGB_565,
    /// (effective) 16bit Pixel, Order Y U Y V, U+V = color for 2 pixels
    VRM_YUYV_4X8,
    /// 8bit Pixel
    VRM_GRAY_8,
    /// 8bit Pixel, Order G B (even lines) R G (odd lines)
    VRM_BAYER_GBRG_8,
    /// 8bit Pixel, Order B G (even lines) G R (odd lines)
    VRM_BAYER_BGGR_8,
    /// 8bit Pixel, Order R G (even lines) G B (odd lines)
    VRM_BAYER_RGGB_8,
    /// 8bit Pixel, Order G R (even lines) B G (odd lines)
    VRM_BAYER_GRBG_8,
    /// 10bit Pixel, storing upper 8bit in first byte,
    /// storing lower 2bit in second byte (lowest bits)
    VRM_GRAY_10,
    /// 10bit Pixel, Order G B (even lines) R G (odd lines)
    VRM_BAYER_GBRG_10,
    /// 10bit Pixel, Order B G (even lines) G R (odd lines)
    VRM_BAYER_BGGR_10,
    /// 10bit Pixel, Order R G (even lines) G B (odd lines)
    VRM_BAYER_RGGB_10,
    /// 10bit Pixel, Order G R (even lines) B G (odd lines)
    VRM_BAYER_GRBG_10,
    /// 16bit Pixel, using 16bit (WORD) access
    VRM_GRAY_16,
    /// 48bit Pixel, using 16bit (WORD) access
    VRM_BGR_3X16,
    /// 16bit Pixel, Order G B (even lines) R G (odd lines)
    VRM_BAYER_GBRG_16,
    /// 16bit Pixel, Order B G (even lines) G R (odd lines)
    VRM_BAYER_BGGR_16,
    /// 16bit Pixel, Order G R (even lines) B G (odd lines)
    VRM_BAYER_RGGB_16,
    /// 16bit Pixel, Order R G (even lines) G B (odd lines)
    VRM_BAYER_GRBG_16,
    /// (effective) 16bit Pixel, Order U Y V Y, U+V = color for 2 pixels
    VRM_UYVY_4X8
};

class CameraHandle {
 public:
    explicit CameraHandle(const std::string& camera_serial);
    void start(const std::string& camera_serial);
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

 private:
    VRmUsbCamDevice device_;
    VRmDWORD port_;
    std::string identifier_;
    double clock_epoch_;
    bool setPropertyPort();
    bool verifyFrameRate();
};  // class CameraHandle

}  // namespace Driftcam

#endif  // INCLUDE_VRMAGIC_DRIFTCAM_CAMERA_HANDLE_H_
