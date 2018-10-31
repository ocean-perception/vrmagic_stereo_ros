// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Main Function
// ----------------------------------------------------------------------------------------------

#include "vrmagic_driftcam/demo.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

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
	std::cerr << "VRmUsbCam Error: " <<  VRmUsbCamGetLastError() << "\nApplication exit" << std::endl;
	exit(-1);
}

int main(int argc, char** argv)
{
	// at first, be sure to call VRmUsbCamCleanup() at exit, even in case
	// of an error
	atexit(VRmUsbCamCleanup);

	// parse command line
	bool g_use_sysmem= false;	   // by default, use video memory
	if (argc>1) g_use_sysmem= strcmp(argv[1], "sysmem")?false:true;

	// read libversion (for informational purposes only)
	VRmDWORD libversion;
	VRMEXECANDCHECK(VRmUsbCamGetVersion(&libversion));

	std::cout << "========================================================" << std::endl
		<< "===         VRmagic VRmUsbCam2 C API Demo            ===" << std::endl
		<< "========================================================" << std::endl
		<< "(v." << libversion << ")" << std::endl << std::endl;

	// uncomment this to enable logging features of VRmUsbCam (for customer support)
	//VRmUsbCamEnableLogging();

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
		std::cerr << "No suitable VRmagic device found!" << std::endl;
		exit(-1);
	}

	// NOTE:
	// from now on, the "device" handle can be used to access the camera board.
	// use VRmUsbCamCloseDevice to end the usage

#ifdef WIN32
	// open settings window of device (optional, may fail if GUI component is not installed)
	VRmDevicePropertyPage* lp_page= 0;
	if (VRmUsbCamCreateDevicePropertyPage(device, 0, 0, &lp_page))
	{
		MoveWindow(lp_page->m_handle, 80, 80, lp_page->m_size_hint.m_width, lp_page->m_size_hint.m_height, false);
		ShowWindow(lp_page->m_handle, SW_SHOW);
	}
#endif
	// init camera, change some settings...
	// we get a target_format in return, which is necessary to initialize our
	// viewer window
	VRmImageFormat target_formats[4];
	VRmDWORD num_ports;
	VRmBOOL active_port_list[4]={0, 0, 0, 0};
	initCamera(device, num_ports, active_port_list, target_formats);
	if(num_ports<1)
	{
		std::cout << "no sensor(s) found, exiting!" << std::endl;
		exit(-1);
	}

	// and read pictures...
	readCamera(device, num_ports, active_port_list, target_formats);

#ifdef WIN32
	// finally, close the settings window
	VRMEXECANDCHECK(VRmUsbCamDestroyDevicePropertyPage(&lp_page));
#endif
	// ...and the device
	VRMEXECANDCHECK(VRmUsbCamCloseDevice(device));

	std::cout << "exit." << std::endl;

	return 0;
}
