// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C++ API v2 Demo Application
// ==============================================================================================
// Main include file
// ----------------------------------------------------------------------------------------------
#ifndef VRMUSBCAMDEMO_H
#define VRMUSBCAMDEMO_H

// include the c++ api wrapper of the library
#include "../../../wrappers/c++/vrmusbcamcpp.h"

// include our resources
#include "resource.h"

// function prototypes "readcamera.cpp"
void readCamera(VRmUsbCamCPP::DevicePtr device, int port, VRmUsbCamCPP::ImageFormat target_format);
void keystrokeHook(unsigned char key);
void DoEvents();

// function prototypes "initcamera.cpp"
void initCamera(VRmUsbCamCPP::DevicePtr device, int& port, VRmUsbCamCPP::ImageFormat& target_format);

// function prototypes "directdraw.cpp"
bool DDrawOpen(const char *title, VRmUsbCamCPP::ImageFormat f_format);
void DDrawUpdate();
void DDrawClose();
unsigned char* DDrawLockBuffer(unsigned int& fr_dstpitch);
void DDrawUnlockBuffer(unsigned char* fp_buffer);
void DDrawSetKeystrokeHook(void (*hook)(unsigned char key));
bool DDrawChangeBufferSize(VRmUsbCamCPP::ImageFormat f_format);

// global variables "directdraw.cpp"
extern bool g_use_sysmem;

#endif //VRMUSBCAMDEMO_H
