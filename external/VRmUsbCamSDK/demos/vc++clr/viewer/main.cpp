// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam .NET API Demo Application for VC++/CLR 2005
// ==============================================================================================
// Main Function
// ----------------------------------------------------------------------------------------------

using namespace System;
using namespace System::Windows::Forms;
using namespace VRmUsbCamNET;

void InitCamera(VRmUsbCamNET::Device^ device, int% port, VRmUsbCamNET::ImageFormat^% targetFormat);
void ReadCamera(VRmUsbCamNET::Device^ device, int port, VRmUsbCamNET::ImageFormat^ targetFormat);

int main(array<System::String^>^ args)
{
	try
	{
		Console::WriteLine("=======================================================");
		Console::WriteLine("===  VRmagic VRmUsbCam2 .NET API Demo for VC++/CLR  ===");
		Console::WriteLine("=======================================================");
		Console::WriteLine("(v.{0})", VRmUsbCam::Version);
		Console::WriteLine();

		// uncomment one of the following lines to enable logging features (for customer support)
		//VRmUsbCam::EnableLogging(); // save logfile to default location 
		//VRmUsbCam::EnableLoggingEx("mylogfile.log"); //save logfile to user defined location

		// check for connected devices
		array<VRmUsbCamNET::DeviceKey^>^ devlist = VRmUsbCam::DeviceKeyList;

		// open first usable device
		VRmUsbCamNET::Device^ device = nullptr;
		for (int i=0; i<devlist->Length; ++i)
		{
			if (!devlist[i]->Busy)
			{
				device= VRmUsbCam::OpenDevice(devlist[i]);
				break;
			}
		}

		// display error when no camera has been found
		if (!device)
		{
			MessageBox::Show("No suitable VRmagic device found!", "VRmUsbCamNET Demo",
				MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		// init camera, change some settings...
		// we get a targetFormat in return, which is necessary to initialize our
		// viewer window
		VRmUsbCamNET::ImageFormat^ targetFormat = nullptr;
		int port=0;
		InitCamera(device,port,targetFormat);

		// open settings window of device (optional, may fail if GUI component is not installed)
		VRmUsbCamNET::DevicePropertyPage^ propPage = nullptr;
		try
		{
			propPage = device->CreatePropertyPage();
			propPage->Location = Drawing::Point(80,80);
			propPage->Show();
		}
		catch (VRmUsbCamNET::Exception^) {}

		// and read pictures...
		ReadCamera(device, port, targetFormat);

		// finally, close settings window and the device
		delete propPage;
		delete device;

		Console::WriteLine("exit.");
	}
	// some error handling
	catch (VRmUsbCamNET::Exception^ err)
	{
		MessageBox::Show("VRmUsbCam Error: " + err->Message, "VRmUsbCamNET Demo",
			 MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
	return 0;
}
