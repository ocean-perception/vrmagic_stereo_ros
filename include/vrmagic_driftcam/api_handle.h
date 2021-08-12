#include "vrmagic_driftcam/camera_handle.h"

namespace Driftcam
{
    class ApiHandle
    {
    public:
        // Constructor
        ApiHandle(
            const std::string &cam_serial,
            const std::string &cam_path,
            const std::string &mission_name,
            bool open_on_start);

        // Destructor
        ~ApiHandle()
        {
            close();
        }

        // Check if the camera is open.
        bool isOpen() const
        {
            return cam_.opened();
        }

        // Open the camera.
        void open()
        {
            if (!cam_.opened())
                update();
        }

        // Close the camera.
        void close()
        {
            if (cam_.opened())
            {
                cam_.stop();
                cam_.close();
                VRmUsbCamCleanup();
            }
        }

        // Grab a frame from the camera.
        void grab()
        {
            int num_images = -1;
            if (cam_.opened())
                num_images = cam_.grab();
            return num_images;
        }

        // Triggers a frame grab.
        void trigger()
        {
            if (cam_.opened())
                cam_.trigger();
        }

        // Sets the folder to save images to.
        void setFoldername(const std::string &folder_name)
        {
            folder_name_ = foldername;
            if (cam_.opened())
            {
                cam_.setPath(cam_path_ + "/" + folder_name_ + "/" + cam_serial_);
            }
        }

    private:
        void VRmUsbCamCallbackProxy(VRmStaticCallbackType f_type, void *fp_user_data, const void *fcp_callback_params);
        std::string cam_serial_;
        std::string cam_path_;
        std::string folder_name_;
        std::string mission_name_;
        Driftcam::CameraHandle cam_;
    };
}
