// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Main Function
// ----------------------------------------------------------------------------------------------

#include "demo.h"

// some stuff we need here:
#include <windows.h>
#include <iostream>
#include <sstream>

// ----------------------------------------------------------------------------------------------
// main function
// ----------------------------------------------------------------------------------------------
//
// the main function accepts one parameter:
//
// - if the this parameter is 'sysmem', then the direct draw buffer is allocated in system
//   memory instead of video memory, which is the default.
//
// EXAMPLE:
//
// vrmusbcamdemo sysmem
//

void LogExit()
{
	std::stringstream msg;
	msg << "VRmUsbCam Error: " <<  VRmUsbCamGetLastError();
	MessageBox(0, msg.str().c_str(), "VRmUsbCam Demo", MB_ICONERROR);
	std::cerr << msg.str() << "\nApplication exit" << std::endl;
	exit(-1);
}

int main(int argc, char** argv)
{
	// at first, be sure to call VRmUsbCamCleanup() at exit, even in case
	// of an error
	atexit(VRmUsbCamCleanup);

	// parse command line
	g_use_sysmem= false;	   // by default, use video memory
	if (argc>1) g_use_sysmem= strcmp(argv[1], "sysmem")?false:true;

	// read libversion (for informational purposes only)
	VRmDWORD libversion;
	VRMEXECANDCHECK(VRmUsbCamGetVersion(&libversion));

	std::cout << "========================================================" << std::endl
		<< "===         VRmagic VRmUsbCam2 C API Demo            ===" << std::endl
		<< "========================================================" << std::endl
		<< "(v." << libversion << ")" << std::endl << std::endl;

	// uncomment one of the following lines to enable logging features of VRmUsbCam (for customer support)
	//VRmUsbCamEnableLogging(); // save logfile to default location
	//VRmUsbCamEnableLoggingEx("mylogfile.log"); //save logfile to user defined location

	// check for connected devices
	VRmDWORD size=0;
	VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListSize(&size));

	// open first usable device
	VRmUsbCamDevice device=0;
	VRmDeviceKey* p_device_key=0;
	for(VRmDWORD i=0; i<size && !device; ++i)
	{
		VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListEntry(i, &p_device_key));
		if(!p_device_key->m_busy) {
			VRMEXECANDCHECK(VRmUsbCamOpenDevice(p_device_key, &device));
		}
		VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_device_key));
	}

	// display error when no camera has been found
	if(!device)
	{
		MessageBox(0, "No suitable VRmagic device found!", "VRmUsbCam Demo", MB_ICONERROR);
		exit(-1);
	}

	// NOTE:
	// from now on, the "device" handle can be used to access the camera board.
	// use VRmUsbCamCloseDevice to end the usage

	// open settings window of device (optional, may fail if GUI component is not installed)
	VRmDevicePropertyPage* lp_page= 0;
	if (VRmUsbCamCreateDevicePropertyPage(device, 0, 0, &lp_page))
	{
		MoveWindow(lp_page->m_handle, 80, 80, lp_page->m_size_hint.m_width, lp_page->m_size_hint.m_height, false);
		ShowWindow(lp_page->m_handle, SW_SHOW);
	}

	// init camera, change some settings...
	// we get a target_format in return, which is necessary to initialize our
	// viewer window
	VRmImageFormat target_format;
	VRmDWORD port=0;
	initCamera(device, port, target_format);

	// and read pictures...
	readCamera(device, port, target_format);

	// finally, close the settings window
	VRMEXECANDCHECK(VRmUsbCamDestroyDevicePropertyPage(&lp_page));
	// ...and the device
	VRMEXECANDCHECK(VRmUsbCamCloseDevice(device));

	std::cout << "exit." << std::endl;

	return 0;
}
