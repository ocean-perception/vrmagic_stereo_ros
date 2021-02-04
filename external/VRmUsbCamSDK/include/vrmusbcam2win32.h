// ==============================================================================================
/// @file vrmusbcam2win32.h                      VRmUsbCam C API Win32-related Functions v3.5.0.0
//  Copyright VRmagic 2004-2014
// ==============================================================================================

#ifndef VRMUSBCAM2WIN32_H
#define VRMUSBCAM2WIN32_H

// define this if you do not want <windows.h> included. it is needed for definition of HWND.
#ifndef VRM_NO_INCLUDE_WINDOWS_H
#include <windows.h>
#endif

#if defined(__cplusplus) && !defined(VRM_NO_EXTERN_C)
extern "C" {
#endif

/// VRmUsbCam is C calling convention (cdecl). define this appropriate if your compiler
/// has a different default
#ifndef VRM_API
#define VRM_API
#endif

// don't define this. for internal use only
#ifndef VRM_EXTERN
#define VRM_EXTERN extern
#endif

// don't define this. for internal use only
#ifndef VRM_STRUCT
#define VRM_STRUCT struct
#endif

// ------------------------------------------------------------------------
/// @defgroup DevPropPage            Device Property Page Support Functions
// ------------------------------------------------------------------------
/// @{

/// represents a device property page (device GUI).
/// use VRmUsbCamCreateDevicePropertyPage() to create it and destroy it afterwards with
/// VRmUsbCamDestroyDevicePropertyPage(). NOTE: you may move, resize, hide/show the
/// page with Windows API calls using the m_handle Window Handle
typedef VRM_STRUCT _VRmDevicePropertyPage {
	/// window handle of the page
	HWND m_handle;
	/// the size this page looks best
	VRmSizeI m_size_hint;
	/// API private informations
	void* mp_private;
} VRmDevicePropertyPage;

/// create a property page for the device.
/// if f_hwnd_parent is null, a new independent window is created,
/// if f_hwnd_parent is not null, a child window is created within the given parent.
/// fp_rectangle may optionally point to a rectangle containing the initial window position.
/// NOTE: in case a new independent window is created, it is not shown automatically, so you need to show the window
/// by
/// @code
///		ShowWindow((*fpp_page)->m_handle, SW_SHOW)
/// @endcode
VRM_EXTERN VRmRetVal VRM_API VRmUsbCamCreateDevicePropertyPage(VRmUsbCamDevice f_device, HWND f_hwnd_parent, const VRmRectI* fp_rectangle, VRmDevicePropertyPage** fpp_page);

/// destroy the given property page
VRM_EXTERN VRmRetVal VRM_API VRmUsbCamDestroyDevicePropertyPage(VRmDevicePropertyPage** fpp_page);

/// @}
// ------------------------------------------------------------------------
/// @defgroup DeviceMngAdv                       Advanced Device Management
// ------------------------------------------------------------------------
/// @{

/// activates a VRmagic USB device in the current hardware profile, identified by
/// vendor id, product id and serial number
VRM_EXTERN VRmRetVal VRM_API VRmUsbCamActivateDevice(VRmWORD f_vendor_id, VRmWORD f_product_id, VRmDWORD f_serial);

/// deactivates a VRmagic USB device in the current hardware profile, identified by
/// vendor id, product id and serial number.
/// deactivates devices no longer consume USB bandwidth and are marked by a red cross in
/// the device manager. they will also not be included in the device key list returned by
/// VRmUsbCamGetDeviceKeyListSize()/VRmUsbCamGetDeviceKeyListSize()!
/// NOTE: deactivated devices remain deactivated until explicit re-activation,
/// even after replug!
VRM_EXTERN VRmRetVal VRM_API VRmUsbCamDeactivateDevice(VRmWORD f_vendor_id, VRmWORD f_product_id, VRmDWORD f_serial);

/// activates all VRmagic USB devices in the current hardware profile that are
/// currently deactivated
VRM_EXTERN VRmRetVal VRM_API VRmUsbCamActivateAllDevices(void);

/// @}


#if defined(__cplusplus) && !defined(VRM_NO_EXTERN_C)
};//extern "C"
#endif

#endif // VRMUSBCAM2WIN32_H
