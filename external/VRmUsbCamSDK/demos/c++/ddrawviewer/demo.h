// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C API v2 Demo Application
// ==============================================================================================
// Main include file
// ----------------------------------------------------------------------------------------------
#ifndef VRMUSBCAMDEMO_H
#define VRMUSBCAMDEMO_H

// include the api of the library
#include <vrmusbcam2.h>
#ifdef WIN32
	#include <vrmusbcam2win32.h>
#endif

// include our resources
#include "resource.h"

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
void keystrokeHook(unsigned char key);
void DoEvents();

// function prototypes "initcamera.cpp"
void initCamera(VRmUsbCamDevice device, VRmDWORD &port, VRmImageFormat& target_format);

// function prototypes "directdraw.cpp"
bool DDrawOpen(const char *title, VRmImageFormat f_format);
void DDrawUpdate();
void DDrawClose();
unsigned char* DDrawLockBuffer(unsigned int& fr_dstpitch);
void DDrawUnlockBuffer(unsigned char* fp_buffer);
void DDrawSetKeystrokeHook(void (*hook)(unsigned char key));
bool DDrawChangeBufferSize(VRmImageFormat f_format);

// global variables "directdraw.cpp"
extern bool g_use_sysmem;

#endif //VRMUSBCAMDEMO_H
