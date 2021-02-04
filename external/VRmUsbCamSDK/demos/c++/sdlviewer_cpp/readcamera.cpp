// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C++ API Wrapper Demo Application
// ==============================================================================================
// Camera Reading / Main Loop
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>
#include <sstream>

#ifdef __linux__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

using namespace VRmUsbCamCPP;

void readCamera(DevicePtr device, int port, ImageFormat target_format)
{
	// SDL keystate array
	Uint8 *keystate;

	// initialize viewer
	std::stringstream buf;
	buf << "VRmagic VRmUsbCam C++ API Wrapper Demo - Device: "
		<< device->get_DeviceKey()->ToString();
	if (!SDLWindowInit(buf.str().c_str(), target_format))
		throw Exception("Could not initialize SDL output window.");

	// ------------------------------------------------------------------------
	// main loop: read images and draw them to screen
	// ------------------------------------------------------------------------

	std::cout << "Reading from camera..." << std::endl;

	device->ResetFrameCounter();

	// start grabber at first
	device->Start();

	// and enter the loop
	do
	{
		// lock next (raw) image for read access, convert it to the desired
		// format and unlock it again, so that grabbing can go on
		ImagePtr p_source_img;
		int frames_dropped;
		try
		{
			p_source_img= device->LockNextImage(port, &frames_dropped, 5000);

			// lock the SDL off-screen buffer to output the image to the screen.
			// The screen_buffer_pitch variable will receive the pitch (byte size of
			// one line) of the buffer.
			VRmDWORD screen_buffer_pitch;
			VRmBYTE* p_screen_buffer=SDLLockBuffer(screen_buffer_pitch);

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

			// give the off-screen buffer back to SDL
			SDLUnlockBuffer();

			// and update the screen
			SDLUpdate();

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

		SDL_PumpEvents();

		// get key state of ESC
		keystate = SDL_GetKeyState(NULL);
	} while (!keystate[SDLK_ESCAPE] && !kbhit());

	// stop grabber
	device->Stop();

	// close SDL output window
	SDLWindowClose();
}
