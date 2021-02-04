// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Camera Initialization
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>

void initCamera(VRmUsbCamDevice device, VRmDWORD& num_sensorports, VRmBOOL* active_port_list, VRmImageFormat* target_format) {

	// some examples for properties / camera settings
	VRmBOOL supported;

	//first set some global properties
	// uncomment the following to disable trigger modes for this demo
	/*	VRmPropId mode= VRM_PROPID_GRAB_MODE_FREERUNNING;
	VRMEXECANDCHECK(VRmUsbCamSetPropertyValueE(device, VRM_PROPID_GRAB_MODE_E, &mode));
	*/

	// check number of connected sensors
	num_sensorports=0;
	VRMEXECANDCHECK(VRmUsbCamGetSensorPortListSize(device, &num_sensorports));

	// we check which of the attached sensors are enabled for grabbing and set sensor specific properties
	for(VRmDWORD ii=0; ii<num_sensorports;ii++)
	{
		VRmDWORD port;
		VRMEXECANDCHECK(VRmUsbCamGetSensorPortListEntry(device, ii, &port));

		// on single sensor devices this property does not exist
		VRmPropId sensor_enable = (VRmPropId)(VRM_PROPID_GRAB_SENSOR_ENABLE_1_B-1+port);
		VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(device, sensor_enable, &supported));
		if(supported)
		{
			//fill list of active ports
			VRMEXECANDCHECK(VRmUsbCamGetPropertyValueB(device, sensor_enable, &active_port_list[ii]));
		}

		// check if property bank switch is supported
		VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(device, VRM_PROPID_GRAB_SENSOR_PROPS_SELECT_E, &supported));
		if(supported)
		{
			//switch property bank to adjust sensor specific properties
			VRmPropId property_bank = (VRmPropId) (VRM_PROPID_GRAB_SENSOR_PROPS_SELECT_1-1+port);
			VRMEXECANDCHECK(VRmUsbCamSetPropertyValueE(device, VRM_PROPID_GRAB_SENSOR_PROPS_SELECT_E, &property_bank));
		}

		// adjustable ROI?
		VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(device, VRM_PROPID_GRAB_USER_ROI_RECT_I, &supported));
		if(supported)
		{
			//we check for the maximal possible ROI and adapt the image height, leaving everything else maximal
			VRmPropAttribsRectI roi_attribs;
			VRMEXECANDCHECK(VRmUsbCamGetPropertyAttribsRectI(device, VRM_PROPID_GRAB_USER_ROI_RECT_I, &roi_attribs));
			VRmRectI roi = roi_attribs.m_max;
			roi.m_height=(4-port+1)*120;
			VRMEXECANDCHECK(VRmUsbCamSetPropertyValueRectI(device, VRM_PROPID_GRAB_USER_ROI_RECT_I, &roi));
		}

		//check if exposure time can be set
		VRMEXECANDCHECK(VRmUsbCamGetPropertySupported(device, VRM_PROPID_CAM_EXPOSURE_TIME_F, &supported));
		if(supported)
		{
			float value=10.f;
			// uncomment the following lines to change exposure time to 25ms
			// when camera supports this feature
			VRMEXECANDCHECK(VRmUsbCamSetPropertyValueF(device, VRM_PROPID_CAM_EXPOSURE_TIME_F, &value));
		}

		VRmImageFormat source_format;
		// get the current source format
		VRMEXECANDCHECK(VRmUsbCamGetSourceFormatEx(device, port, &source_format));

		const char *source_color_format_str;
		VRMEXECANDCHECK(VRmUsbCamGetStringFromColorFormat(source_format.m_color_format, &source_color_format_str));

		std::cout << "Sensor port "<< port << " with source format "
			<< source_format.m_width << "x" << source_format.m_height
			<< " (" << source_color_format_str << ")";

		if(active_port_list[ii])
			std::cout << " enabled "<< std::endl;
		else
			std::cout <<" disabled" << std::endl;

		// select a target format from the list of formats we can convert the source images to.
		// we search for the ARGB_4X8 format (that is always included in the list), since
		// rendering of this format will never fail
		VRmDWORD number_of_target_formats, jj;
		VRMEXECANDCHECK(VRmUsbCamGetTargetFormatListSizeEx2( device, port, &number_of_target_formats ) );
		for ( jj = 0; jj < number_of_target_formats; ++jj )
		{
			VRMEXECANDCHECK(VRmUsbCamGetTargetFormatListEntryEx2(device, port, jj, &target_format[ii]));
#ifdef __arm__
			//the davinci based cameras are using RGB565 for video output
			if ( target_format[ii].m_color_format == VRM_RGB_565 )
#else
			if ( target_format[ii].m_color_format == VRM_ARGB_4X8 )
#endif
				break;
		}

		const char *target_color_format_str;
		VRMEXECANDCHECK(VRmUsbCamGetStringFromColorFormat(target_format[ii].m_color_format, &target_color_format_str));
		std::cout << "Selected target format: "
			<< target_format[ii].m_width << "x" << target_format[ii].m_height
			<< " (" << target_color_format_str << ")" << std::endl;
	}

}
