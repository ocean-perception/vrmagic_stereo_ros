// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C++ API Wrapper Demo Application
// ==============================================================================================
// Main Function
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>
#include <cstdlib>
#include <cstring>

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
		bool g_use_sysmem= false;	   // by default, use video memory
		if (argc>1) g_use_sysmem= strcmp(argv[1], "sysmem")?false:true;

		std::cout<< "========================================================" << std::endl
			<< "===     VRmagic VRmUsbCam2 C++ API Wrapper Demo      ===" << std::endl
			<< "========================================================" << std::endl
			<< "(v." << VRmUsbCam::get_Version() << ")" << std::endl << std::endl;

		// uncomment one of the following lines to enable logging features (for customer support)
		//VRmUsbCam::EnableLogging(); // save logfile to default location
		//VRmUsbCam::EnableLoggingEx("mylogfile.log"); //save logfile to user defined location

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
			std::cerr << "No suitable VRmagic device found!" << std::endl;
			exit(-1);
		}

		// NOTE:
		// from now on, the "device" handle can be used to access the camera board.
		// reset the device pointer (by device.reset()) to end the usage

#ifdef WIN32
		DevicePropertyPagePtr propPage;
		try
		{
			propPage= device->CreatePropertyPage();
			propPage->set_Location(PointI(80, 80));
			propPage->Show();
		}
		catch (const Exception&) {}
#endif

		// init camera, change some settings...
		// we get a target_format in return, which is necessary to initialize our
		// viewer window
		ImageFormat target_format;
		int port= 0;
		initCamera(device, port, target_format);

		// and read pictures...
		readCamera(device, port, target_format);

#ifdef WIN32
		// finally, close the settings window
		propPage.reset();
#endif

		// ...and the device
		device.reset();
	}
	catch (const Exception& exc)
	{
		std::cerr << "VRmUsbCam Error: " <<  exc.what() << "\nApplication exit" << std::endl;
		exit(-1);
	}

	std::cout << "exit." << std::endl;

	return 0;
}
