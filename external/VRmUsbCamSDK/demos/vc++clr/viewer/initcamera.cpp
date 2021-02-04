// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam .NET API v2 Demo Application for VC++/CLR 2005
// ==============================================================================================
// Camera Initialization
// ----------------------------------------------------------------------------------------------

using namespace VRmUsbCamNET;
using namespace System;

void InitCamera(VRmUsbCamNET::Device^ device, int% firstPort, VRmUsbCamNET::ImageFormat^% targetFormat)
{
	firstPort = device->SensorPortList[0];
	for each ( int port in device->SensorPortList)
	{
		VRmUsbCamNET::PropId sensor_enable = VRmUsbCamNET::PropId::GRAB_SENSOR_ENABLE_1_B + VRmUsbCamNET::PropId(port-1);
		if(device->PropertySupported[sensor_enable])
		{
			if(port==firstPort)
				device->PropertyValue[sensor_enable]=true;
			else
				device->PropertyValue[sensor_enable]=false;
		}
	}
	
	// just for example: change exposure time to 25ms when camera
	// supports this feature
	if (device->PropertySupported[PropId::CAM_EXPOSURE_TIME_F])
	{
		// uncomment this line
//		device->PropertyValue[PropId::CAM_EXPOSURE_TIME_F] = 25;
	}

	// uncomment the following to disable trigger modes for this demo
//	device->PropertyValue[PropId::GRAB_MODE_E] = PropId::GRAB_MODE_FREERUNNING;

	// get the name of the currently selected source (raw) format
	Console::WriteLine("Selected source format: " +  device->SourceFormat[firstPort]->ToString());

	// select a target format from the list of formats we can convert the source images to.
	// we search for the ARGB_4X8 format (that is always included in the list), since
	// rendering of this format will never fail
	for each (VRmUsbCamNET::ImageFormat^ fmt in device->TargetFormatList[firstPort])
	{
		if (fmt->ColorFormat == ColorFormat::ARGB_4X8)
		{
			targetFormat = fmt;
			break;
		}
	}

	Console::WriteLine("Selected target format: " + targetFormat->ToString());

	// uncomment these lines to flip output image vertical / horizontal:
//	targetFormat->FlipHorizontal();
//	targetFormat->FlipVertical();
}
