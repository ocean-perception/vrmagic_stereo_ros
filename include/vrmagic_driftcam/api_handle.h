#include "vrmagic_driftcam/camera_handle.h"

namespace Driftcam
{
    class ApiHandle
    {
    public:
        ApiHandle(
            const std::string &cam_serial,
            const std::string &cam_path,
            bool open_on_start);
        ~ApiHandle()
        {
            close();
        }
        bool isOpen()
        {
            return cam_.opened();
        }
        void open()
        {
            if (!cam_.opened())
                update();
        }
        void close()
        {
            if (cam_.opened())
            {
                cam_.stop();
                cam_.close();
            }
        }
        void update();
        void grab()
        {
            if (cam_.opened())
                cam_.grab();
        }

        void trigger()
        {
            if (cam_.opened())
                cam_.trigger();
        }

        // Sets the folder to save images to.
        void setFoldername(const std::string &folder_name)
        {
            folder_name_ = folder_name;
            if (cam_.opened())
            {
                cam_.setPath(cam_path_ + "/" + folder_name_ + "/" + cam_serial_);
            }
        }


    private:
        void VRmUsbCamCallbackProxy(
            VRmStaticCallbackType f_type,
            void *fp_user_data,
            const void *fcp_callback_params);
        std::string cam_serial_;
        std::string cam_path_;
        std::string folder_name_;
        Driftcam::CameraHandle cam_;
    };
}
