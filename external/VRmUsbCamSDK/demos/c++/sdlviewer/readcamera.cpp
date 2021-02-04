// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam2 C API Demo Application
// ==============================================================================================
// Camera Reading / Main Loop
// ----------------------------------------------------------------------------------------------

#include "demo.h"

#include <iostream>
#include <sstream>
#include <iomanip>

#ifdef __linux__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

void readCamera(VRmUsbCamDevice device, VRmDWORD port, VRmImageFormat target_format)
{
	// SDL keystate array
	Uint8 *keystate;

	// initialize viewer
	std::stringstream buf;
	VRmDeviceKey* p_key;
	VRMEXECANDCHECK(VRmUsbCamGetDeviceKey(device, &p_key));
	VRmSTRING l_serial;
	VRMEXECANDCHECK(VRmUsbCamGetSerialString(p_key, &l_serial));
	buf << "VRmagic VRmUsbCam C API v2 Demo - Device: "
		<< p_key->mp_product_str
		<< " #" << l_serial;
	if (!SDLWindowInit(buf.str().c_str(), target_format))
	{
		std::cerr << "Could not initialize SDL output window." << std::endl;
		VRmUsbCamCloseDevice(device);
		exit(-1);
	}
	VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_key));

	// ------------------------------------------------------------------------
	// main loop: read images and draw them to screen
	// ------------------------------------------------------------------------

	std::cout << "Reading from camera..." << std::endl;

	VRMEXECANDCHECK(VRmUsbCamResetFrameCounter(device));

	// start grabber first
	VRMEXECANDCHECK(VRmUsbCamStart(device));

	// and enter the loop
	do
	{
		// lock next (raw) image for read access, convert it to the desired
		// format and unlock it again, so that grabbing can
		// go on
		VRmImage* p_source_img=0;
		VRmDWORD frames_dropped=0;
		if(VRmUsbCamLockNextImageEx2(device, port, &p_source_img, &frames_dropped, 5000))
		{
			// VRmUsbCamLockNextImageEx2() successfully returned an image
			// ----------------------------------------------------------

			// lock the SDL off-screen buffer to output the image to the screen.
			// The screen_buffer_pitch variable will receive the pitch (byte size of
			// one line) of the buffer.
			VRmDWORD screen_buffer_pitch;
			VRmBYTE* p_screen_buffer=SDLLockBuffer(screen_buffer_pitch);

			// now, wrap a VRmImage around the locked screen buffer to receive the converted image
			VRmImage* p_target_img=0;
			VRmUsbCamSetImage(&p_target_img, target_format, p_screen_buffer, screen_buffer_pitch);

			VRMEXECANDCHECK(VRmUsbCamConvertImage(p_source_img, p_target_img));

			VRmDWORD frame_counter;
			VRMEXECANDCHECK(VRmUsbCamGetFrameCounter(p_source_img, &frame_counter));

			VRMEXECANDCHECK(VRmUsbCamUnlockNextImage(device, &p_source_img));

			// see, if we had to drop some frames due to data transfer stalls. if so,
			// output a message
			if (frames_dropped)
				std::cout << "- " << frames_dropped <<  " frame(s) dropped -" << std::endl;

			double now;
			VRmUsbCamGetCurrentTime(&now);

			VRmDWORD img_sensorport;
			VRmUsbCamGetImageSensorPort(p_target_img, &img_sensorport);

			// uncomment this to output time stamp and frame counter of image
			//			std::cout << "frame " << frame_counter << ", image sensor port " << img_sensorport << ", timestamp " << p_target_img->m_time_stamp << ", now " << now << std::endl;

			// free the resources of the target image
			VRMEXECANDCHECK(VRmUsbCamFreeImage(&p_target_img));
			// give the off-screen buffer back to SDL
			SDLUnlockBuffer();

			// and update the screen
			SDLUpdate();
		}
		else
		{
			// VRmUsbCamLockNextImageEx2() didi not return an image. why?
			// ----------------------------------------------------------


			int error_code = VRmUsbCamGetLastErrorCode();
			switch(VRmUsbCamGetLastErrorCode())
			{
			case VRM_ERROR_CODE_FUNCTION_CALL_TIMEOUT:
			case VRM_ERROR_CODE_TRIGGER_TIMEOUT:
			case VRM_ERROR_CODE_TRIGGER_STALL:
				std::cout << "VRmUsbCamLockNextImageEx2() failed with " << VRmUsbCamGetLastError() << std::endl;
				break;

			case VRM_ERROR_CODE_GENERIC_ERROR:
			default:
				LogExit();
			}
		}

		SDL_PumpEvents();

		// get key state of ESC
		keystate = SDL_GetKeyState(NULL);
	} while (!keystate[SDLK_ESCAPE] && !kbhit());

	// stop grabber
	VRMEXECANDCHECK(VRmUsbCamStop(device));

	// close SDL output window
	SDLWindowClose();
}
