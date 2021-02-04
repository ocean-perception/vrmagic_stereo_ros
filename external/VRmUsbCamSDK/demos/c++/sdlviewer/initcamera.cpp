// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Camera Initialization
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>

void initCamera(VRmUsbCamDevice device, VRmDWORD& port, VRmImageFormat& target_format)
{
	// some examples for properties / camera settings
	VRmBOOL supported;

	// check number of connected sensors
	VRmDWORD num_sensorports=0;
	VRMEXECANDCHECK(VRmUsbCamGetSensorPortListSize(device, &num_sensorports));

	// for this demo we switch off all connected sensor but the first one in the port list
	for(VRmDWORD ii=0; ii<num_sensorports;ii++)
	{
		VRMEXECANDCHECK(VRmUsbCamGetSensorPortListEntry(device, ii, &port));

		// on single sensor devices this property does not exist
		VRmPropId sensor_enable = (VRmPropId)(VRM_PROPID_GRAB_SENSOR_ENABLE_1_B-1+port);
		VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(device, sensor_enable, &supported));
		if(supported)
		{
			//enable first sensor in port list
			VRmBOOL enable = 1;
			if(ii)
				enable = 0;
			VRMEXECANDCHECK(VRmUsbCamSetPropertyValueB(device, sensor_enable, &enable));
		}
	}

	//now get the first sensor port
	VRMEXECANDCHECK(VRmUsbCamGetSensorPortListEntry(device, 0, &port));

	//check if exposure time can be set
	VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(device, VRM_PROPID_CAM_EXPOSURE_TIME_F, &supported));
	if(supported)
	{
		float value=25.f;
		// uncomment the following lines to change exposure time to 25ms
		// when camera supports this feature
		/*		VRMEXECANDCHECK(VRmUsbCamSetPropertyValueF(device, VRM_PROPID_CAM_EXPOSURE_TIME_F, &value));
		*/	}

	// uncomment the following to disable trigger modes for this demo
	/*	VRmPropId mode= VRM_PROPID_GRAB_MODE_FREERUNNING;
	VRMEXECANDCHECK(VRmUsbCamSetPropertyValueE(device, VRM_PROPID_GRAB_MODE_E, &mode));
	*/

	VRmImageFormat source_format;

	// get the current source format
	VRMEXECANDCHECK(VRmUsbCamGetSourceFormatEx(device, port, &source_format));

	const char *source_color_format_str;
	VRMEXECANDCHECK(VRmUsbCamGetStringFromColorFormat(source_format.m_color_format, &source_color_format_str));
	std::cout << "Selected source format: "
		<< source_format.m_width << "x" << source_format.m_height
		<< " (" << source_color_format_str << ")" << std::endl;


	// select a target format from the list of formats we can convert the source images to.
	// we search for the ARGB_4X8 format (that is always included in the list), since
	// rendering of this format will never fail
	VRmDWORD number_of_target_formats, i;
	VRMEXECANDCHECK(VRmUsbCamGetTargetFormatListSizeEx2( device, port, &number_of_target_formats ) );
	for ( i = 0; i < number_of_target_formats; ++i )
	{
		VRMEXECANDCHECK(VRmUsbCamGetTargetFormatListEntryEx2(device, port, i, &target_format));
#ifdef D2_PLATFORM
		//the davinci based cameras are using RGB565 for video output
		if ( target_format.m_color_format == VRM_RGB_565 )
#else
		if ( target_format.m_color_format == VRM_ARGB_4X8 )
#endif
			break;
	}

	const char *target_color_format_str;
	VRMEXECANDCHECK(VRmUsbCamGetStringFromColorFormat(target_format.m_color_format, &target_color_format_str));
	std::cout << "Selected target format: "
		<< target_format.m_width << "x" << target_format.m_height
		<< " (" << target_color_format_str << ")" << std::endl;


}
