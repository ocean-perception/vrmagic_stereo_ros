// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam C++ API v2 Demo Application
// ==============================================================================================
// Direct draw Stuff
// ----------------------------------------------------------------------------------------------

#include "demo.h"

// Parts of this code is taken from the TinyPTC by Gaffer,
// see www.gaffer.org/tinyptc

#define WIN32_LEAN_AND_MEAN
#include <ddraw.h>
#include <Mmsystem.h>

#include <iostream>

// typedefs
typedef HRESULT (WINAPI * DIRECTDRAWCREATE) (GUID FAR *lpGUID, LPDIRECTDRAW FAR *lplpDD, IUnknown FAR *pUnkOuter);

// local function prototypes
static void DDrawPaintPrimary();

int g_bufferwidth;
int g_bufferheight;

bool g_use_sysmem;

// local variables

static HMODULE library = 0;
static LPDIRECTDRAW lpDD = 0;
static LPDIRECTDRAWSURFACE lpDDS = 0;
static LPDIRECTDRAWSURFACE lpDDS_back;
static WNDCLASS wc;
static HWND wnd;

static LPDIRECTDRAWCLIPPER lpDDC = 0;
static LPDIRECTDRAWSURFACE lpDDS_secondary = 0;

static void (*gp_keystroke_hook)(unsigned char key) = 0;

// functions

void DDrawSetKeystrokeHook(void (*hook)(unsigned char key)) {
	gp_keystroke_hook= hook;
}

bool DDrawChangeBufferSize(VRmUsbCamCPP::ImageFormat f_format) {
	DDSURFACEDESC descriptor;

	g_bufferwidth = f_format.get_Size().m_width;
	g_bufferheight = f_format.get_Size().m_height;

	if (lpDDS_secondary)
	{
		// release secondary
		lpDDS_secondary->Release();
		lpDDS_secondary = 0;
	}
	// create secondary surface
	descriptor.dwSize  = sizeof(descriptor);
	descriptor.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	descriptor.dwWidth = f_format.get_Size().m_width;
	descriptor.dwHeight = f_format.get_Size().m_height;
	descriptor.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);

	descriptor.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | (g_use_sysmem?DDSCAPS_SYSTEMMEMORY:DDSCAPS_VIDEOMEMORY);

	switch(f_format.get_ColorFormat())
	{
	case VRM_ARGB_4X8:
		descriptor.ddpfPixelFormat.dwFlags=DDPF_RGB;
		descriptor.ddpfPixelFormat.dwRGBBitCount=32;
		descriptor.ddpfPixelFormat.dwRBitMask=0x00FF0000;
		descriptor.ddpfPixelFormat.dwGBitMask=0x0000FF00;
		descriptor.ddpfPixelFormat.dwBBitMask=0x000000FF;
		break;
	case VRM_BGR_3X8:
		descriptor.ddpfPixelFormat.dwFlags=DDPF_RGB;
		descriptor.ddpfPixelFormat.dwRGBBitCount=24;
		descriptor.ddpfPixelFormat.dwRBitMask=0x00FF0000;
		descriptor.ddpfPixelFormat.dwGBitMask=0x0000FF00;
		descriptor.ddpfPixelFormat.dwBBitMask=0x000000FF;
		break;
	case VRM_RGB_565:
		descriptor.ddpfPixelFormat.dwFlags=DDPF_RGB ;
		descriptor.ddpfPixelFormat.dwRGBBitCount=16;
		descriptor.ddpfPixelFormat.dwRBitMask=0x0000F800;
		descriptor.ddpfPixelFormat.dwGBitMask=0x000007E0;
		descriptor.ddpfPixelFormat.dwBBitMask=0x0000001F;
		break;
	case VRM_YUYV_4X8:
		descriptor.ddpfPixelFormat.dwFlags=DDPF_FOURCC ;
		descriptor.ddpfPixelFormat.dwFourCC = mmioFOURCC('Y', 'U', 'Y', '2');
		break;
#ifdef DDPF_LUMINANCE // older DX versions will not support this
	case VRM_GRAY_8:
		descriptor.ddpfPixelFormat.dwFlags=DDPF_LUMINANCE;
		descriptor.ddpfPixelFormat.dwRGBBitCount=8;
		break;
#endif
	default:
		return false;
	}

	if (FAILED(lpDD->CreateSurface(&descriptor, &lpDDS_secondary, 0))) return false;

	lpDDS_back=lpDDS_secondary;

	DDrawPaintPrimary();
	return true;
}

static void DDrawPaintPrimary()
{
	RECT source;
	RECT destination;

	if(!lpDDS) return;

	// get origin of client area
	POINT Corner={0, 0};
	GetClientRect(wnd, &destination);
	Corner.x=0; Corner.y=0;
	ClientToScreen(wnd, &Corner);
	destination.left+=Corner.x; destination.right+=Corner.x;
	destination.top+=Corner.y;  destination.bottom+=Corner.y;

	// setup source rectangle
	source.left = 0;
	source.top = 0;
	source.right = g_bufferwidth;
	source.bottom = g_bufferheight;

	// blt secondary to primary surface
	lpDDS->Blt(&destination, lpDDS_secondary, &source, DDBLT_WAIT, 0);
}


static LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// result data
	int result = 0;

	// handle message
	switch (message)
	{
	case WM_MOVING:
		break;

	case WM_PAINT:
		// paint primary
		DDrawPaintPrimary();
		// call default window painting
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;

	case WM_CHAR:
		if (gp_keystroke_hook) gp_keystroke_hook((unsigned char)(wParam&0xFF));
		break;

	case WM_CLOSE:
		// map window close to ESC character
		gp_keystroke_hook(27);
		break;

	default:
		// unhandled messages
		result = (int) DefWindowProc(hWnd, message, wParam, lParam);
	}

	// finished
	return result;
}


bool DDrawOpen(const char *title, VRmUsbCamCPP::ImageFormat f_format)
{
	RECT rect;
	DDSURFACEDESC descriptor;
	DIRECTDRAWCREATE DirectDrawCreate;

	std::cout << "Initializing output window with backbuffers in "
		<< (g_use_sysmem?"system":"video") << " memory..." << std::endl;

	// load direct draw library
	library = (HMODULE) LoadLibrary("ddraw.dll");
	if (!library) return false;

	// get directdraw create function address
	DirectDrawCreate = (DIRECTDRAWCREATE) GetProcAddress(library, "DirectDrawCreate");
	if (!DirectDrawCreate) return false;

	// create directdraw interface
	if (FAILED(DirectDrawCreate(0, &lpDD, 0))) return false;

	// register window class
	wc.style = CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = wndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_VRMAGIC_LOGO));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = title;
	RegisterClass(&wc);

	// calculate window size
	rect.left = 0;
	rect.top = 0;
	rect.right = f_format.get_Size().m_width;
	rect.bottom = f_format.get_Size().m_height;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, 0);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	// create fixed window
	wnd = CreateWindow(title, title, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, 0, 0, 0, 0);

	// show window
	ShowWindow(wnd, SW_NORMAL);

	// enter cooperative mode
	if (FAILED(lpDD->SetCooperativeLevel(wnd, DDSCL_NORMAL))) return false;

	// primary with no back buffers
	descriptor.dwSize  = sizeof(descriptor);
	descriptor.dwFlags = DDSD_CAPS;
	descriptor.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	if (FAILED(lpDD->CreateSurface(&descriptor, &lpDDS, 0))) return false;

	// create clipper
	if (FAILED(lpDD->CreateClipper(0, &lpDDC, 0))) return false;

	// set clipper to window
	if (FAILED(lpDDC->SetHWnd(0, wnd))) return false;

	// attach clipper object to primary surface
	if (FAILED(lpDDS->SetClipper(lpDDC))) return false;

	if( !DDrawChangeBufferSize(f_format)) return false;

	// disable screensaver while direct draw window is open
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);

	// success
	return true;
}

void DDrawUpdate() {
	// paint primary
	DDrawPaintPrimary();
}

unsigned char* DDrawLockBuffer(unsigned int& fr_dstpitch) {

	DDSURFACEDESC descriptor;

	// restore surfaces
	lpDDS->Restore();
	lpDDS_secondary->Restore();

	// lock back surface
	descriptor.dwSize = sizeof descriptor;
	if (FAILED(lpDDS_back->Lock(0, &descriptor, DDLOCK_WAIT, 0))) return 0;

	// return buffer pitch
	fr_dstpitch= descriptor.lPitch;

	// return buffer pointer
	return (unsigned char *) descriptor.lpSurface;
}

void DDrawUnlockBuffer(unsigned char* fp_buffer) {
	// unlock back surface
	lpDDS_back->Unlock(fp_buffer);
}


void DDrawClose()
{

	// check secondary
	if (lpDDS_secondary)
	{
		// release secondary
		lpDDS_secondary->Release();
		lpDDS_secondary = 0;
	}


	// check
	if (lpDDS)
	{
		// release primary
		lpDDS->Release();
		lpDDS = 0;
	}

	// check
	if (lpDD)
	{
		// leave display mode
		lpDD->RestoreDisplayMode();

		// leave exclusive mode
		lpDD->SetCooperativeLevel(wnd, DDSCL_NORMAL);

		// free direct draw
		lpDD->Release();
		lpDD = 0;
	}

	// destroy window
	DestroyWindow(wnd);

	// check
	if (library)
	{
		// free library
		FreeLibrary(library);
		library = 0;
	}

	// re-enable screensaver now that the direct draw window is closed
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 1, 0, 0);
}
