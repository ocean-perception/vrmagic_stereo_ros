// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C++ API Wrapper Demo Application
// ==============================================================================================
// Camera Initialization
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>

using namespace VRmUsbCamCPP;

void initCamera(DevicePtr device, int &port, ImageFormat &target_format)
{
	// get connected sensor ports
	std::vector<int> sensor_port_list= device->get_SensorPortList();

	// for this demo we switch off all connected sensor but the first one in the port list
	for(size_t ii=0; ii<sensor_port_list.size();ii++)
	{
		port= sensor_port_list[ii];

		// on single sensor devices this property does not exist
		VRmPropId sensor_enable = (VRmPropId)(VRM_PROPID_GRAB_SENSOR_ENABLE_1_B-1+port);
		if(device->get_PropertySupported(sensor_enable))
		{
			//enable first sensor in port list
			bool enable = 1;
			if(ii)
				enable = 0;
			device->set_PropertyValue(sensor_enable, enable);
		}
	}

	//now get the first sensor port
	port= sensor_port_list[0];

	// just for example: change exposure time to 25ms when camera
	// supports this feature
	if (device->get_PropertySupported(VRM_PROPID_CAM_EXPOSURE_TIME_F))
	{
		// uncomment this line
		//		device->set_PropertyValue(VRM_PROPID_CAM_EXPOSURE_TIME_F, 25.f);
	}

	// uncomment the following to disable trigger modes for this demo
	//	device->set_PropertyValue(VRM_PROPID_GRAB_MODE_E, VRM_PROPID_GRAB_MODE_FREERUNNING);

	// get the currently selected source (raw) format
	ImageFormat sourceFormat = device->get_SourceFormat(port);

	std::cout << "Selected source format: " << sourceFormat.ToString() << std::endl;

	// select a target format from the list of formats we can convert the source images to.
	// we search for the ARGB_4X8 format (that is always included in the list), since
	// rendering of this format will never fail
	std::vector<ImageFormat> targetFormatList= device->get_TargetFormatList(port);
	for ( size_t i = 0; i < targetFormatList.size(); ++i )
	{
#ifdef D2_PLATFORM
		//the davinci based cameras are using RGB565 for video output
		if ( targetFormatList[i].get_ColorFormat() == VRM_RGB_565 )
#else
		if ( targetFormatList[i].get_ColorFormat() == VRM_ARGB_4X8 )
#endif
		{
			target_format= targetFormatList[i];
			break;
		}
	}

	std::cout << "Selected target format: " + target_format.ToString() << std::endl;

	// uncomment these lines to flip output image vertical / horizontal:
	//	target_format.FlipHorizontal();
	//	target_format.FlipVertical();
}
