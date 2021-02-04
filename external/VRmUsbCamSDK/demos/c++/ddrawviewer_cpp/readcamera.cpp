// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam2 C++ API Demo Application
// ==============================================================================================
// Camera Reading / Main Loop
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>
#include <sstream>
#include <iomanip>

using namespace VRmUsbCamCPP;

// global variable: user requested to finish
bool g_quit= false;

void readCamera(DevicePtr device, int port, ImageFormat target_format)
{
	// initialize viewer
	std::stringstream buf;
	buf << "VRmagic VRmUsbCam C++ API Wrapper Demo - Device: "
		<< device->get_DeviceKey()->ToString();
	if (!DDrawOpen(buf.str().c_str(), target_format))
		throw Exception("Could not initialize DirectDraw output window.");

	// set a keystroke hook, so that we can terminate the loop by closing the window
	// or pressing the ESC key (look at the end of this file):
	DDrawSetKeystrokeHook(keystrokeHook);

	// ------------------------------------------------------------------------
	// main loop: read images and draw them to screen
	// ------------------------------------------------------------------------

	std::cout << "Reading from camera..." << std::endl;

	// start grabber at first
	device->Start();

	// and enter the loop
	while (!g_quit)
	{
		// lock next (raw) image for read access, convert it to the desired
		// format and unlock it again, so that grabbing can go on
		ImagePtr p_source_img;
		int frames_dropped;
		try
		{
			p_source_img= device->LockNextImage(port, &frames_dropped, 5000);

			// lock the DirectDraw off-screen buffer to output the image to the screen.
			// The screen_buffer_pitch variable will receive the pitch (byte size of
			// one line) of the buffer.
			unsigned int screen_buffer_pitch;
			VRmBYTE* p_screen_buffer=DDrawLockBuffer(screen_buffer_pitch);

			// now, wrap a Image around the locked screen buffer to receive the converted image
			ImagePtr p_target_img= VRmUsbCam::SetImage(target_format, p_screen_buffer, screen_buffer_pitch);

			VRmUsbCam::ConvertImage(p_source_img, p_target_img);

			device->UnlockNextImage(p_source_img);

			// see, if we had to drop some frames due to data transfer stalls. if so,
			// output a message
			if (frames_dropped)
				std::cout << "- " << frames_dropped << " frame(s) dropped -" << std::endl;

			// uncomment this to output time stamp and frame counter of image
			/*			std::cout << "frame " << p_target_img->get_FrameCounter()
			<< ", port " << p_target_img->get_SensorPort()
			<< ", timestamp " << p_target_img->get_TimeStamp()
			<< ", now " << VRmUsbCam::get_CurrentTime()
			<< std::endl;
			*/
			// free the resources of the target image
			p_target_img.reset();

			// give the off-screen buffer back to DirectDraw
			DDrawUnlockBuffer(p_screen_buffer);

			// and update the screen
			DDrawUpdate();
		}
		catch (const Exception& err)
		{
			// in case of an error, check for trigger timeouts and trigger stalls.
			// both can be recovered, so continue. otherwise exit the app
			switch(err.get_Number())
			{
			case VRM_ERROR_CODE_FUNCTION_CALL_TIMEOUT:
			case VRM_ERROR_CODE_TRIGGER_TIMEOUT:
			case VRM_ERROR_CODE_TRIGGER_STALL:
				std::cout << "LockNextImage() failed with " << err.get_Description() << std::endl;
				break;

			case VRM_ERROR_CODE_GENERIC_ERROR:
			default:
				throw;
			}
		}

		// process window events
		DoEvents();
	}
	// stop grabber
	device->Stop();

	// close direct draw output window
	DDrawClose();
}

// this callback hook is called whenever our DirectDraw window receives a keystroke
void keystrokeHook(unsigned char key)
{
	// we only define one key: ESC to terminate the camera reading loop
	if (key==27) g_quit= true;
}

void DoEvents() {
	MSG message;
	// process messages
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		// translate and dispatch
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
	// sleep to dispatch
	Sleep(0);
}
