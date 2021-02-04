// ==============================================================================================
// This file is part of the VRmagic USB2.0 Camera Demo Application
// ==============================================================================================
#ifndef VRMUSBCAMDEMO_H
#define VRMUSBCAMDEMO_H

// include the api of the library
#include  <vrmusbcam2.h>
#ifdef WIN32
	#include <vrmusbcam2win32.h>
#endif

// function prototypes "main.cpp"
void LogExit();

// small helper macro to check function for success and call LogExit when function fails
#define VRMEXECANDCHECK(function)\
{\
	if (VRM_SUCCESS!=function)\
	LogExit();\
}

// function prototypes "readcamera.cpp"
void readCamera(VRmUsbCamDevice device, VRmDWORD port, VRmImageFormat target_format);

// function prototypes "initcamera.cpp"
void initCamera(VRmUsbCamDevice device, VRmDWORD& port, VRmImageFormat& target_format);

// function prototypes "sdlwindow.cpp"
bool SDLWindowInit( const char* fp_caption, VRmImageFormat f_format);
void SDLWindowClose();
unsigned char* SDLLockBuffer( unsigned int& f_pitch );
void SDLUnlockBuffer();
void SDLUpdate();
bool kbhit();

// global variables "sdlwindow.cpp"
extern bool g_use_sysmem;

#endif //VRMUSBCAMDEMO_H
