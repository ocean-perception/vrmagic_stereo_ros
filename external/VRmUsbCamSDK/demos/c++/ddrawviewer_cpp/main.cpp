// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam2 C++ API Demo Application
// ==============================================================================================
// Main Function
// ----------------------------------------------------------------------------------------------

#include "demo.h"

// some stuff we need here:
#include <windows.h>
#include <iostream>
#include <sstream>

using namespace VRmUsbCamCPP;

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

int main(int argc, char** argv)
{
	try
	{
		// parse command line
		g_use_sysmem= false;	   // by default, use video memory
		if (argc>1) g_use_sysmem= strcmp(argv[1], "sysmem")?false:true;

		std::cout<< "========================================================" << std::endl
			<< "===    VRmagic VRmUsbCam2 C++ API Wrapper Demo       ===" << std::endl
			<< "========================================================" << std::endl
			<< "(v." << VRmUsbCam::get_Version() << ")" << std::endl << std::endl;

		// enable logging for debugging purposes
#ifdef _DEBUG
		// uncomment one of the following lines to enable logging features (for customer support)
		//VRmUsbCam::EnableLogging(); // save logfile to default location
		//VRmUsbCam::EnableLoggingEx("mylogfile.log"); //save logfile to user defined location
#endif

		// check for connected devices
		std::vector<DeviceKeyPtr> devlist = VRmUsbCam::get_DeviceKeyList();

		// open first usable device
		DevicePtr device;
		for (size_t i=0; i<devlist.size(); ++i)
		{
			if (!devlist[i]->get_Busy())
			{
				device= VRmUsbCam::OpenDevice(devlist[i]);
				break;
			}
		}

		// display error when no camera has been found
		if(!device)
		{
			MessageBox(0, "No suitable VRmagic device found!", "VRmUsbCam Demo", MB_ICONERROR);
			exit(-1);
		}

		// NOTE:
		// from now on, the "device" handle can be used to access the camera board.
		// reset the device pointer (by device.reset()) to end the usage

		// open settings window of device (optional, may fail if GUI component is not installed)
		DevicePropertyPagePtr propPage;
		try
		{
			propPage= device->CreatePropertyPage();
			propPage->set_Location(PointI(80, 80));
			propPage->Show();
		}
		catch (const Exception&) {}

		// init camera, change some settings...
		// we get a target_format in return, which is necessary to initialize our
		// viewer window
		ImageFormat target_format;
		int port= 0;
		initCamera(device, port, target_format);

		// and read pictures...
		readCamera(device, port, target_format);

		// finally, close the settings window
		propPage.reset();

		// ...and the device
		device.reset();
	}
	catch (const Exception& exc)
	{
		std::stringstream msg;
		msg << "VRmUsbCam Error: " <<  exc.what();
		MessageBox(0, msg.str().c_str(), "VRmUsbCam Demo", MB_ICONERROR);
		std::cerr << msg.str() << "\nApplication exit" << std::endl;
		exit(-1);
	}

	std::cout << "exit." << std::endl;

	return 0;
}
