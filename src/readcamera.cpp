// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Camera Reading / Main Loop
// ----------------------------------------------------------------------------------------------

#include "vrmagic_driftcam/demo.h"

#include <iostream>
#include <sstream>
#include <iomanip>

// #ifdef __linux__
// #include <SDL/SDL.h>
// #else
// #include <SDL.h>
// #endif

// get a tuple of source images with equal frame counter
void getNextTuple(VRmUsbCamDevice device, VRmDWORD num_ports, VRmBOOL* active_port_list, VRmImage** p_source_img)
{

	VRmDWORD max_framecounter=0;
	VRmBOOL completed=false;
	VRmDWORD fc[4]={0, 0, 0, 0};
	VRmDWORD num_cycles=0;

	while(!completed && num_cycles<10)
	{
		for(VRmDWORD ii=0;ii<num_ports;ii++)
		{
			if(active_port_list[ii])
			{
				VRmDWORD port;
				VRMEXECANDCHECK(VRmUsbCamGetSensorPortListEntry(device, ii, &port));

				VRmDWORD frames_dropped=0;
				if(!p_source_img[ii])
				{
					if(!VRmUsbCamLockNextImageEx2(device, port, &p_source_img[ii], &frames_dropped, 5000))
					{
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

					// see, if we had to drop some frames due to data transfer stalls. if so,
					// output a message
					if (frames_dropped)
						std::cout << "- " << frames_dropped <<  " frame(s) dropped -" << std::endl;
					if(p_source_img[ii])
					{
						VRMEXECANDCHECK(VRmUsbCamGetFrameCounter(p_source_img[ii], &fc[ii]));
						if(fc[ii]>=max_framecounter)
						{
							max_framecounter=fc[ii];
						}
					}
				}
			}
		}

		VRmBOOL all_locked=true;

		for(VRmDWORD ii=0;ii<num_ports;ii++)
		{
			if(active_port_list[ii])
			{
				if(fc[ii]<max_framecounter)
				{
					std::cout << ii << ": completeness check: fc: "<< fc[ii] << ", max_fc: " << max_framecounter << std::endl;
					if(p_source_img[ii])
						VRMEXECANDCHECK(VRmUsbCamUnlockNextImage(device, &p_source_img[ii]));

				}
				if(!p_source_img[ii])
					all_locked=false;
			}
		}
		completed=all_locked;
		num_cycles++;
	}

	if(num_cycles==10 && !completed)
	{
		std::cerr << "Check grabbing settings! Could not generate synchronous tuple in 10 cycles, exiting!" << std::endl;
		exit(-1);
	}
}

void readCamera(VRmUsbCamDevice device, VRmDWORD num_ports, VRmBOOL* active_port_list, VRmImageFormat* target_format)
{
	// SDL keystate array
	//unsigned char *keystate;

	// initialize viewer
	std::stringstream buf;
	VRmDeviceKey* p_key;
	VRMEXECANDCHECK(VRmUsbCamGetDeviceKey(device, &p_key));
	VRmSTRING l_serial;
	VRMEXECANDCHECK(VRmUsbCamGetSerialString(p_key, &l_serial));
	std::cout << "VRmagic VRmUsbCam C API v2 Demo - Device: "
		<< p_key->mp_product_str
		<< " #" << l_serial << std::endl;
	VRmImageFormat screen_format=target_format[0];
	for(VRmDWORD ii=1;ii<num_ports;ii++)
	{
		if(active_port_list[ii])
		{
			if(screen_format.m_width<target_format[ii].m_width)
				screen_format.m_width=target_format[ii].m_width;

			screen_format.m_height+=target_format[ii].m_height;

		}
	}
	// if (!SDLWindowInit(buf.str().c_str(), screen_format))
	// {
	// 	std::cerr << "Could not initialize SDL output window." << std::endl;
	// 	VRmUsbCamCloseDevice(device);
	// 	exit(-1);
	// }
	VRMEXECANDCHECK(VRmUsbCamFreeDeviceKey(&p_key));

	// ------------------------------------------------------------------------
	// main loop: read images and draw them to screen
	// ------------------------------------------------------------------------

	std::cout << "Reading from camera..." << std::endl;

	VRMEXECANDCHECK(VRmUsbCamResetFrameCounter(device));

	// start grabber at first
	VRMEXECANDCHECK(VRmUsbCamStart(device));

	// and enter the loop
	do
	{
		//lock a tuple of images (one from each sensor) for read access
		VRmImage* p_source_img[4]={0, 0, 0, 0};
		getNextTuple(device, num_ports, active_port_list, p_source_img);

		// lock the SDL off-screen buffer to output the images below each other to the screen.
		// The screen_buffer_pitch variable will receive the pitch (byte size of
		// one line) of the buffer.
		VRmDWORD screen_buffer_pitch = p_source_img->m_width*8*3;
		VRmBYTE* p_screen_buffer(new VRmBYTE(p_source_img->m_width*8*p_source_img->m_height*3));//=SDLLockBuffer(screen_buffer_pitch);

		// now, we wrap a VRmImage around the locked screen buffer to receive the converted image
		for(VRmDWORD ii = 0; ii<num_ports;ii++)
		{
			if(active_port_list[ii])
			{
				VRmImage* p_target_img=0;
				VRMEXECANDCHECK(VRmUsbCamSetImage(&p_target_img, target_format[ii], p_screen_buffer, screen_buffer_pitch));
				VRMEXECANDCHECK(VRmUsbCamConvertImage(p_source_img[ii], p_target_img));
				VRmUsbCamFreeImage(&p_target_img);

			}
		}
		// give the off-screen buffer back to SDL
		//SDLUnlockBuffer();

		// and update the screen
		//SDLUpdate();

		for(VRmDWORD ii=0;ii<num_ports;ii++)
		{
			if(p_source_img[ii])
			{
				VRMEXECANDCHECK(VRmUsbCamUnlockNextImage(device, &p_source_img[ii]));
			}
		}

		//SDL_PumpEvents();

		// get key state of ESC
		//keystate = SDL_GetKeyState(NULL);
	//} while (!keystate[SDLK_ESCAPE] && !kbhit());
	} while (true);

	// stop grabber
	VRMEXECANDCHECK(VRmUsbCamStop(device));

	// close SDL output window
	//SDLWindowClose();
}
