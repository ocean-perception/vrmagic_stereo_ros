#include "vrmagic_driftcam/camera_handle.h"

namespace Driftcam {
class ApiHandle {
  public:
    ApiHandle(const std::string& cam1_serial, const std::string& cam2_serial);
    ~ApiHandle() {
        if (cam1_.opened()) {
            cam1_.stop();
            cam1_.close();
        }
        if (cam2_.opened()) {
            cam2_.stop();
            cam2_.close();
        }
    }
    void update();
    void grab() {
        if (cam1_.opened()) cam1_.grab();
        if (cam2_.opened()) cam2_.grab();
    }

    void trigger() {
        if (cam1_.opened()) cam1_.trigger();
        if (cam2_.opened()) cam2_.trigger();
    }

  private:
    void VRmUsbCamCallbackProxy(VRmStaticCallbackType f_type, void* fp_user_data, const void* fcp_callback_params);
    std::string cam1_serial_;
    std::string cam2_serial_;
    Driftcam::CameraHandle cam1_;
    Driftcam::CameraHandle cam2_;
};
}
