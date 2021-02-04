// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam .NET API Demo Application for VC++/CLR 2005
// ==============================================================================================
// Camera Reading Loop
// ----------------------------------------------------------------------------------------------

#include "viewer.h"

using namespace VRmUsbCamNET;
using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

void ReadCamera(VRmUsbCamNET::Device^ device, int port, VRmUsbCamNET::ImageFormat^ targetFormat)
{
	// initialize viewer
	Form^ viewer = gcnew Viewer(targetFormat->Size);
	Graphics^ g = viewer->CreateGraphics();

	viewer->Text = "VRmagic VRmUsbCam2 .NET API Demo for VC++/CLR - Device: " + device->DeviceKey->ToString();

	// ------------------------------------------------------------------------
	// main loop: read images and draw them to screen
	// ------------------------------------------------------------------------

	// alloc a buffer for the converted images
	VRmUsbCamNET::Image^ targetImg = gcnew VRmUsbCamNET::Image(targetFormat);

	// start image grabbing and pop up the viewer window
	device->Start();
	viewer->Show();

	while (viewer->Visible)
	{
		int framesDropped;

		// lock next (raw) image for read access, convert it to the desired 
		// format (see above) and unlock it again, so that grabbing can
		// go on
		VRmUsbCamNET::Image^ sourceImg = nullptr;
		try
		{
			sourceImg = device->LockNextImage(port,framesDropped);

			VRmUsbCam::ConvertImage(sourceImg, targetImg);
			device->UnlockNextImage(sourceImg);

			// NOTE: you could natively access or modify the targetImg here, using:
			// array<unsigned char,2>^ buffer = targetImg->LockBits();
			// buffer[...,...] = 255;
			// targetImg->UnlockBits(buffer);

			// detect whether frames have been dropped
			if (framesDropped)
				Console::WriteLine("- {0} frame(s) dropped -",framesDropped);

			// and draw the image
			g->DrawImage(targetImg->ToBitmap(), 0, 0);

			// uncomment this to output time stamp of image
			//cout << "frame " << targetImg->FrameCounter << ", port " << targetImg->SensorPort << ", timestamp " << targetImg->TimeStamp << ", now " << VRmUsbCam::CurrentTime << endl;		
		}
		catch (VRmUsbCamNET::Exception^ err)
		{
			// in case of an error, check for trigger timeouts and trigger stalls.
			// both can be recovered, so continue. otherwise exit the app
			switch(err->Number)
			{
				case ErrorNumber::E_TRIGGER_TIMEOUT:
				case ErrorNumber::E_TRIGGER_STALL:
				case ErrorNumber::E_FUNCTION_CALL_TIMEOUT:
					Console::WriteLine(err->Description);
					break;
				default:
					throw;

			}
		}

		// do some window event handling
		Application::DoEvents();
	}
	// finally, stop image grabbing before we leave the application
	device->Stop();
}
