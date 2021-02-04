// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Main Function
// ----------------------------------------------------------------------------------------------

#include <vrmusbcam2.h>

// some stuff we need here:
#ifdef WIN32
#include <windows.h>
#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>

// small helper macro to check function for success and call LogExit when function fails
#define VRMEXECANDCHECK(function)\
{\
	if (VRM_SUCCESS!=function)\
	LogExit();\
}

void LogExit()
{
	std::stringstream msg;
	msg << "VRmUsbCam Error: " <<  VRmUsbCamGetLastError();
#ifdef WIN32
	MessageBox(0, msg.str().c_str(), "VRmUsbCam Demo", MB_ICONERROR);
#endif
	std::cerr << msg.str() << "\nApplication exit" << std::endl;
	exit(-1);
}

int main(int argc, char** argv)
{
	// at first, be sure to call VRmUsbCamCleanup() at exit, even in case
	// of an error
	atexit(VRmUsbCamCleanup);

	// read libversion (for informational purposes only)
	VRmDWORD libversion;
	VRMEXECANDCHECK(VRmUsbCamGetVersion(&libversion));

	std::cout << "========================================================" << std::endl
		<< "===    VRmagic VRmUsbCam Restore Factory Defaults    ===" << std::endl
		<< "========================================================" << std::endl
		<< "(v." << libversion << ")" << std::endl << std::endl;

	// uncomment one of the following lines to enable logging features of VRmUsbCam (for customer support)
	//VRmUsbCamEnableLogging(); // save logfile to default location
	//VRmUsbCamEnableLoggingEx("mylogfile.log"); //save logfile to user defined location

	// scan for local and usb devices only
	VRMEXECANDCHECK(VRmUsbCamUpdateDeviceKeyListEx(true, true, false));

	// check for connected devices
	VRmDWORD size=0;
	VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListSize(&size));

	std::cout << "WARNING: This demo will restore all user settings to factory" << std::endl
		<< "         defaults on all connected VRmagic VRmUsbCam devices!" << std::endl
		<< "         (" << size << " device(s) found)" << std::endl << std::endl
		<< "Do you wish to do that now? (y/n) ";

	int success_count=0;
	int ignore_count=0;

	if (getchar()=='y')
	{
		std::cout << std::endl;
		// open first usable device
		VRmDeviceKey* p_device_key=0;
		for(VRmDWORD i=0; i<size;++i)
		{
			VRMEXECANDCHECK(VRmUsbCamGetDeviceKeyListEntry(i, &p_device_key));
			VRmSTRING l_serial;
			VRMEXECANDCHECK(VRmUsbCamGetSerialString(p_device_key, &l_serial));
			std::stringstream l_device_name;
			l_device_name << p_device_key->mp_manufacturer_str << " " <<  p_device_key->mp_product_str << " #" << l_serial;

			if(p_device_key->m_busy)
			{
				std::cout << "ignoring busy device " << l_device_name.str() << std::endl;
				ignore_count++;
			}
			else
			{
				VRmUsbCamDevice device=0;
				std::cout << "open device " << l_device_name.str() << std::endl;
				VRMEXECANDCHECK(VRmUsbCamOpenDevice(p_device_key, &device));

				std::cout << "backing up user defaults of device" << std::endl;
				VRmUserData* lp_user_defaults;
				VRMEXECANDCHECK(VRmUsbCamGetConfigData(device, &lp_user_defaults));
				std::string l_backupfilename= std::string("configbackup ")+l_device_name.str()+".vcc";
				// fix special characters in file name
				for(std::string::iterator pC=l_backupfilename.begin(); pC!=l_backupfilename.end(); ++pC)
					if (!isalnum(*pC) && std::string("#.+-").find(*pC)==std::string::npos) *pC='_';
				std::fstream l_backupfile(l_backupfilename.c_str(), std::ios_base::out|std::ios_base::binary);
				if (l_backupfile.good())
					l_backupfile.rdbuf()->sputn((char*)lp_user_defaults->mp_data, lp_user_defaults->m_length);
				VRMEXECANDCHECK(VRmUsbCamFreeUserData(&lp_user_defaults));

				std::cout << "reload factory defaults from device" << std::endl;
				VRMEXECANDCHECK(VRmUsbCamLoadConfig(device, 0));

				std::cout << "save as user defaults to device" << std::endl;
				VRMEXECANDCHECK(VRmUsbCamSaveConfig(device, 1));

				std::cout << "close device\n" << std::endl;
				VRMEXECANDCHECK(VRmUsbCamCloseDevice(device));
				success_count++;
			}
			VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_device_key));
		}

	}
	std::cout << std::endl << "exit." << std::endl;

	if(ignore_count)
	{
		std::stringstream msg;
		msg << "failed to reloaded factory settings on " <<  ignore_count << " busy device(s)";
#ifdef WIN32
		MessageBox(0, msg.str().c_str(), "VRmUsbCam Demo", MB_ICONWARNING);
#endif
	}
	if(success_count)
	{
		std::stringstream msg;
		msg << "successfully reloaded factory settings on " <<  success_count << " device(s)";
#ifdef WIN32
		MessageBox(0, msg.str().c_str(), "VRmUsbCam Demo", MB_ICONINFORMATION);
#endif
	}

#ifdef WIN32
	if(!size)
		MessageBox(0, "no device found", "VRmUsbCam Demo", MB_ICONINFORMATION);
#endif

	return 0;
}
