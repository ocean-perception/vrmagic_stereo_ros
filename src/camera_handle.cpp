// Copyright (c) 2018 University of Southampton

#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

#include "vrmagic_driftcam/camera_handle.h"

void LogExit() {
    std::cerr << "VRmUsbCam Error: "
              <<  VRmUsbCamGetLastError()
              << "\nApplication exit" << std::endl;
    exit(-1);
}

namespace Driftcam {

CameraHandle::CameraHandle(const std::string& camera_serial)
        : device_(0), port_(1) {
    // set empty device
    device_ = 0;
    /// NOTE: single-sensor devices do only have port #1
    port_ = 1;

    start(camera_serial);
}

void CameraHandle::start(const std::string& camera_serial) {
    // uncomment this to enable logging features of VRmUsbCam (customer support)
    // VRmUsbCamEnableLogging();

    // check for connected devices
    VRmDWORD size;
    VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListSize(&size));

    VRmDeviceKey* p_device_key;
    VRmSTRING p_device_str;
    for (VRmDWORD i = 0; i < size && !device_; ++i) {
        VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListEntry(i, &p_device_key));
        VRMEXECANDCHECK(VRmUsbCamGetSerialString(p_device_key, &p_device_str));
        // std::cout << "Found device: "
        //           << " \n\t Key:    " << p_device_key->m_serial
        //           << " \n\t Serial: " << p_device_str << std::endl;
        if (std::strcmp(p_device_str, camera_serial.c_str()) == 0) {
            if (!p_device_key->m_busy) {
                std::cout << "Opening device: "
                          << p_device_key->mp_product_str
                          << " #" << p_device_str << std::endl;
                identifier_ = std::string(p_device_str);
                VRMEXECANDCHECK(VRmUsbCamOpenDevice(p_device_key, &device_));
            }
        }
        VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_device_key));
    }

    // display error when no camera has been found
    if (!device_) {
        std::cerr << "No suitable VRmagic device found!" << std::endl;
        exit(-1);
    }



    // printColorFormats();

    VRmPropId mode = VRM_PROPID_GRAB_MODE_TRIGGERED_SOFT;
    VRMEXECANDCHECK(VRmUsbCamSetPropertyValueE(device_,
                                               VRM_PROPID_GRAB_MODE_E,
                                               &mode));
    VRMEXECANDCHECK(VRmUsbCamResetFrameCounter(device_));

    clock_epoch_ = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() / 1000.0;
    VRMEXECANDCHECK(VRmUsbCamRestartTimer());

    double timestamp = getCurrentTime();
    // std::cout << "Timestamp: " << timestamp << std::endl;
    // std::cout << std::setprecision(16)
    //           << "Epoch    : " << clock_epoch_ << std::endl;

    // start grabber at first
    VRMEXECANDCHECK(VRmUsbCamStart(device_));
}

double CameraHandle::getCurrentTime() {
    double timestamp = 0;
    VRmUsbCamGetCurrentTime(&timestamp);
    return timestamp;
}

void CameraHandle::stop() {
    // ...and the device
    std::cout << "Stopping camera..." << std::endl;
    VRMEXECANDCHECK(VRmUsbCamStop(device_));
}

void CameraHandle::close() {
    std::cout << "Closing camera..." << std::endl;
    VRMEXECANDCHECK(VRmUsbCamCloseDevice(device_));
}

void CameraHandle::trigger() {
    VRMEXECANDCHECK(VRmUsbCamSoftTrigger(device_));
}


void CameraHandle::grab() {
    int timeout_ms = 1000;
    bool images_available = false;

    // lock next (raw) image for read access, convert it to the desired
    // format and unlock it again, so that grabbing can go on
    VRmImage* p_source_img = 0;
    VRmDWORD frames_dropped = 0;
    images_available = VRmUsbCamLockNextImageEx2(device_,
                                                 port_,
                                                 &p_source_img,
                                                 &frames_dropped,
                                                 timeout_ms);
    if (images_available) {
        // VRmUsbCamLockNextImageEx2() successfully returned an image
        // ----------------------------------------------------------
        // VRmDWORD img_sensorport;
        // VRmUsbCamGetImageSensorPort(p_source_img, &img_sensorport);

        // VRmImageFormat target_format = image_formats_[img_sensorport-1];

        VRmImage* p_target_img = 0;

        // Copy image
        VRMEXECANDCHECK(VRmUsbCamCopyImage(&p_target_img, p_source_img));

        VRmDWORD frame_counter;
        VRMEXECANDCHECK(VRmUsbCamGetFrameCounter(p_source_img,
                                                 &frame_counter));
        VRMEXECANDCHECK(VRmUsbCamUnlockNextImage(device_,
                                                 &p_source_img));

        // see, if we had to drop some frames due to data transfer stalls.
        // if so, output a message
        if (frames_dropped) {
            std::cerr << "- " << frames_dropped
                      <<  " frame(s) dropped -" << std::endl;
        } else {
            // Make cv::Mat header for the recieved image (This does not copy
            // data, hence the use of Clone() later)
            // cv::Size img_size(p_target_img->m_image_format.m_width,
            //                p_target_img->m_image_format.m_height);
            // cv::Mat img = cv::Mat(img_size,
            //                    CV_8UC1,
            //                    p_target_img->mp_buffer,
            //                    cv::Mat::AUTO_STEP);

            /// Save a VRmImage to a PNG file. Compression level 0 is
            // uncompressed, max. is 9, use default with -1.
            int compression_level = 0;
            std::stringstream stream;
            stream << std::fixed << std::setprecision(6) << clock_epoch_ + p_target_img->m_time_stamp/1000.0;
            std::string stamp = stream.str();
            std::string filename = identifier_ + "_" + stamp + ".png";
            std::cout << "Saving file: " << filename << std::endl;
            VRMEXECANDCHECK(VRmUsbCamSavePNG(filename.c_str(),
                                             p_target_img,
                                             compression_level));
        }
        VRMEXECANDCHECK(VRmUsbCamFreeImage(&p_target_img));
    } else {
        // VRmUsbCamLockNextImageEx2() did not return an image. why?
        int error_code = VRmUsbCamGetLastErrorCode();
        std::string error_string = std::string(VRmUsbCamGetLastError());
        std::cerr << "VRmUsbCamLockNextImageEx2() failed with "
                  << error_string << std::endl;
    }
}

bool CameraHandle::setPropertyPort() {
    unsigned int r;

    VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(
        device_, VRM_PROPID_GRAB_SENSOR_PROPS_SELECT_E, &r));
    if (r) {
        VRmPropId tmp = VRmPropId(VRM_PROPID_GRAB_SENSOR_PROPS_SELECT_1);
        VRMEXECANDCHECK(VRmUsbCamSetPropertyValueE(
            device_, VRM_PROPID_GRAB_SENSOR_PROPS_SELECT_E, &tmp));
    }

    return static_cast<bool>(r);
}

void CameraHandle::printColorFormats() {
    VRmImageFormat src_format;
    VRmUsbCamGetSourceFormatEx(device_, port_, &src_format);
    std::cout << "Source format: " << src_format.m_width << "x"
              << src_format.m_height << " "
              << colorToString(src_format.m_color_format) << std::endl;
    VRmSTRING desc;
    VRmUsbCamGetSourceFormatDescription(device_, port_, &desc);
    std::cout << "Source format: " << desc;

    VRmDWORD target_format_size = 0;
    VRmUsbCamGetTargetFormatListSizeEx2(device_, port_, &target_format_size);
    for (size_t i = 0; i < target_format_size; i++) {
        VRmImageFormat format;
        VRmUsbCamGetTargetFormatListEntryEx2(device_, port_, i, &format);
        std::cout << "- " << format.m_width << "x" << format.m_height << " "
                  << colorToString(format.m_color_format) << std::endl;
    }
}

std::string CameraHandle::colorToString(const VRmColorFormat& fmt) {
    std::string msg;
    if (fmt == VRM_ARGB_4X8) {
        msg = "32bit Pixel, Order ARGB using 32bit (DWORD) access";
    } else if (fmt == VRM_BGR_3X8) {
        msg = "24bit Pixel, Order BGR using 8bit (BYTE) access";
    } else if (fmt == VRM_RGB_565) {
        msg = "16bit Pixel, Order R(5)G(6)B(5) using 16bit (WORD) access";
    } else if (fmt == VRM_YUYV_4X8) {
        msg = "(effective) 16bit Pixel, Order YUYV, U+V = color for 2 pixels";
    } else if (fmt == VRM_GRAY_8) {
        msg = "8bit Pixel";
    } else if (fmt == VRM_BAYER_GBRG_8) {
        msg = "8bit Pixel, Order G B (even lines) R G (odd lines)";
    } else if (fmt == VRM_BAYER_BGGR_8) {
        msg = "8bit Pixel, Order B G (even lines) G R (odd lines)";
    } else if (fmt == VRM_BAYER_RGGB_8) {
        msg = "8bit Pixel, Order R G (even lines) G B (odd lines)";
    } else if (fmt == VRM_BAYER_GRBG_8) {
        msg = "8bit Pixel, Order G R (even lines) B G (odd lines)";
    } else if (fmt == VRM_GRAY_10) {
        msg = "10bit Pixel, upper 8bit in first byte, lower 2bit in second";
    } else if (fmt == VRM_BAYER_GBRG_10) {
        msg = "10bit Pixel, Order G B (even lines) R G (odd lines)";
    } else if (fmt == VRM_BAYER_BGGR_10) {
        msg = "10bit Pixel, Order B G (even lines) G R (odd lines)";
    } else if (fmt == VRM_BAYER_RGGB_10) {
        msg = "10bit Pixel, Order R G (even lines) G B (odd lines)";
    } else if (fmt == VRM_BAYER_GRBG_10) {
        msg = "10bit Pixel, Order G R (even lines) B G (odd lines)";
    } else if (fmt == VRM_GRAY_16) {
        msg = "16bit Pixel, using 16bit (WORD) access";
    } else if (fmt == VRM_BGR_3X16) {
        msg = "48bit Pixel, using 16bit (WORD) access";
    } else if (fmt == VRM_BAYER_GBRG_16) {
        msg = "16bit Pixel, Order G B (even lines) R G (odd lines)";
    } else if (fmt == VRM_BAYER_BGGR_16) {
        msg = "16bit Pixel, Order B G (even lines) G R (odd lines)";
    } else if (fmt == VRM_BAYER_RGGB_16) {
        msg = "16bit Pixel, Order G R (even lines) B G (odd lines)";
    } else if (fmt == VRM_BAYER_GRBG_16) {
        msg = "16bit Pixel, Order R G (even lines) G B (odd lines)";
    } else {
        msg = "Unknown format";
    }
    return msg;
}

float CameraHandle::getExposure() {
    float exposure = -1;
    VRmBOOL supported = setPropertyPort();

    VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(
        device_, VRM_PROPID_CAM_EXPOSURE_TIME_F, &supported));
    if (supported) {
        VRMEXECANDCHECK(VRmUsbCamGetPropertyValueF(
            device_, VRM_PROPID_CAM_EXPOSURE_TIME_F, &exposure));
    }
    return exposure;
}

double CameraHandle::getLastExternalTriggerTimestamp() {
    double stamp = 0;
    VRmBOOL supported = setPropertyPort();

    VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(
        device_, VRM_PROPID_CAM_LAST_EXTERNAL_TRIGGER_TIMESTAMP_D, &supported));
    if (supported) {
        VRMEXECANDCHECK(VRmUsbCamGetPropertyValueD(
            device_, VRM_PROPID_CAM_LAST_EXTERNAL_TRIGGER_TIMESTAMP_D, &stamp));
    }
    return stamp;
}


bool CameraHandle::setExposure(float exposure) {
    bool ret = false;
    VRmBOOL supported = setPropertyPort();

    VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(
        device_, VRM_PROPID_CAM_EXPOSURE_TIME_F, &supported));
    if (supported) {
        VRMEXECANDCHECK(VRmUsbCamSetPropertyValueF(
            device_, VRM_PROPID_CAM_EXPOSURE_TIME_F, &exposure));
        ret = true;
    }
    // Scale framerate if too high
    verifyFrameRate();
    return ret;
}

bool CameraHandle::setGain(int gain) {
    bool ret = false;
    VRmBOOL supported = setPropertyPort();

    VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(
        device_, VRM_PROPID_CAM_GAIN_MONOCHROME_I, &supported));
    if (supported) {
        // uncomment the following lines to change exposure time to 25ms
        // when camera supports this feature
        VRMEXECANDCHECK(VRmUsbCamSetPropertyValueI(
            device_, VRM_PROPID_CAM_GAIN_MONOCHROME_I, &gain));
        ret = true;
    }
    return ret;
}

bool CameraHandle::setFramerate(float framerate) {
    bool ret = false;
    float new_frame_rate = framerate;
    float max_frame_rate = getMaxSupportedFramerate();

    if (max_frame_rate < new_frame_rate) {
        std::cerr << "Framerate too high - framerate will be set to maximum "
                  << "allowed! - which is: " << max_frame_rate << std::endl;
        new_frame_rate = max_frame_rate;
    }
    VRmBOOL supported;
    VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(
        device_, VRM_PROPID_CAM_INTERNAL_TRIGGER_RATE_F, &supported));
    if (supported) {
        // uncomment the following lines to change exposure time to 25ms
        // when camera supports this feature
        VRMEXECANDCHECK(VRmUsbCamSetPropertyValueF(
            device_, VRM_PROPID_CAM_INTERNAL_TRIGGER_RATE_F, &new_frame_rate));
        ret = true;
    }
    return ret;
}

bool CameraHandle::setMaxSupportedFramerate() {
    return setFramerate(getMaxSupportedFramerate());
}

bool CameraHandle::verifyFrameRate() {
    bool ret = true;
    float max_frame_rate = getMaxSupportedFramerate();
    float frame_rate = getFramerate();
    if (frame_rate > max_frame_rate) {
        std::cerr << "Framerate changed to: " << max_frame_rate << " from: "
                  << frame_rate << " due to changed exposure" << std::endl;
        setFramerate(max_frame_rate);
        ret = false;
    }
    return ret;
}

float CameraHandle::getMaxSupportedFramerate() {
    float max_frame_rate;
    VRMEXECANDCHECK(VRmUsbCamGetPropertyValueF(
        device_, VRM_PROPID_CAM_ACQUISITION_RATE_MAX_F, &max_frame_rate));
    return max_frame_rate;
}

float CameraHandle::getFramerate() {
    float frame_rate;
    VRMEXECANDCHECK(VRmUsbCamGetPropertyValueF(
        device_, VRM_PROPID_CAM_INTERNAL_TRIGGER_RATE_F, &frame_rate));
    return frame_rate;
}

bool CameraHandle::setTriggerMode(const TriggerMode& triggermode) {
    VRmPropId tmp;
    if (triggermode == TRIG_INTERNAL) {
        tmp = VRM_PROPID_GRAB_MODE_TRIGGERED_INTERNAL;
    } else if (triggermode == TRIG_EXTERNAL) {
        tmp = VRM_PROPID_GRAB_MODE_TRIGGERED_EXT;
    } else if (triggermode == TRIG_SOFTWARE) {
        tmp = VRM_PROPID_GRAB_MODE_TRIGGERED_SOFT;
    } else if (triggermode == FREERUNNING) {
        tmp = VRM_PROPID_GRAB_MODE_FREERUNNING_SEQUENTIAL;
    } else {
        std::cout << "Unknown triggermode" << std::endl;
        return false;
    }

    VRMEXECANDCHECK(VRmUsbCamSetPropertyValueE(
        device_, VRM_PROPID_GRAB_MODE_E, &tmp));

    return true;
}

VRmPropId CameraHandle::getTriggerMode() {
    VRmPropId ret;
    VRMEXECANDCHECK(VRmUsbCamGetPropertyValueE(
        device_, VRM_PROPID_GRAB_MODE_E, &ret));
    return ret;
}

}  // namespace Driftcam
