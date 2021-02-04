// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C++ API Wrapper Demo Application
// ==============================================================================================
// Main include file
// ----------------------------------------------------------------------------------------------
#ifndef VRMUSBCAMDEMO_H
#define VRMUSBCAMDEMO_H

// include the c++ api wrapper of the library
#include "../../../wrappers/c++/vrmusbcamcpp.h"

#ifdef WIN32
	#include <vrmusbcam2win32.h>
#endif

// function prototypes "readcamera.cpp"
void readCamera(VRmUsbCamCPP::DevicePtr device, int port, VRmUsbCamCPP::ImageFormat target_format);

// function prototypes "initcamera.cpp"
void initCamera(VRmUsbCamCPP::DevicePtr device, int& port, VRmUsbCamCPP::ImageFormat& target_format);

// function prototypes "sdlwindow.cpp"
bool SDLWindowInit( const char* fp_caption, VRmUsbCamCPP::ImageFormat f_format);
void SDLWindowClose();
unsigned char* SDLLockBuffer( unsigned int& f_pitch );
void SDLUnlockBuffer();
void SDLUpdate();
bool kbhit();

// global variables "sdlwindow.cpp"
extern bool g_use_sysmem;

#endif //VRMUSBCAMDEMO_H
