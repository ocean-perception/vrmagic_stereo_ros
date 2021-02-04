// ==============================================================================================
// This file is part of the VRmagic VRmUsbCam DS Capture Source Demo Application
// ==============================================================================================

// disable warnings for deprecated functions
#pragma warning( disable : 4995 )
#pragma warning( disable : 4996 )

// system headers
#include <comdef.h>
#include <windef.h>
#include <dshow.h>

// c++ headers
#include <exception>
#include <string>
#include <sstream>
#include <iostream>

// header for VRmUsbCamDS Capture Source
#include <vrmdshow.h>

// this imports the VRmUsbCamCOM API (only needed if you wish to configure
// the DS Capture Source using the COM API)
#include <vrmusbcam2com.h>

// include our resources
#include "resource.h"

using namespace VRmUsbCamCOM;

// also just for convenience, define some COM object smart pointers. they Release()
// their objects automatically when destroyed
_COM_SMARTPTR_TYPEDEF(IGraphBuilder, IID_IGraphBuilder);
_COM_SMARTPTR_TYPEDEF(IBaseFilter, IID_IBaseFilter);
_COM_SMARTPTR_TYPEDEF(IPin, IID_IPin);
_COM_SMARTPTR_TYPEDEF(IMediaControl, IID_IMediaControl);
_COM_SMARTPTR_TYPEDEF(IMediaEventEx, IID_IMediaEventEx);
_COM_SMARTPTR_TYPEDEF(IVideoWindow, IID_IVideoWindow);

// Global DirectShow/COM objects
IVideoWindowPtr gp_VideoWindow;
IMediaEventExPtr gp_MediaEventEx;

// our Window Message Code to notify our app window of DShow Graph events
const int WM_GRAPH_EVENT = WM_USER + 1;

// prototypes for helper functions
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
HRESULT RemoveFromRot(DWORD *pdwRegister);

// -----------------------------------------------------------------------------
// Main Function
// -----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	std::cout << "========================================================" << std::endl
		<< "===    VRmagic VRmUsbCam DS Capture Source Demo      ===" << std::endl
		<< "========================================================" << std::endl;

	DWORD dwGraphROTid= 0;

	// Main app Window
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = NULL;
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);
	WndClass.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_VRMAGIC_LOGO));
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "Window Class";
	WndClass.lpszMenuName = 0;
	WndClass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
	RegisterClass(&WndClass);
	HWND hWnd = CreateWindowEx(0, "Window Class", "VRmagic VRmUsbCam DS Capture Source Demo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0, 0, 0);

	try
	{
		// Initialize the COM library.
		CoInitialize(0);

		// DirectShow/COM Smart Object Pointers
		IGraphBuilderPtr p_Graph;
		IPinPtr p_CapturePin;
		IMediaControlPtr p_Control;

		// pointer to VRmUsbCam DS Capture Source
		IBaseFilterPtr p_VRmUsbCamDS;

		// (optional) pointer to VRmUsbCam COM API v2
		IVRmUsbCamPtr p_VRmUsbCam;

		// Create the VRmUsbCam DS Capture Source
		if FAILED(p_VRmUsbCamDS.CreateInstance(CLSID_VRmUsbCamDS))
			throw std::exception("Could not create the VRmUsbCamDS Capture Source Filter.\nMaybe there is no VRmagic device connected?");

		// the following code block demonstrates how to change some settings on the
		// capture source device using the VRmUsbCamCOM API. we also use it later to
		// get the device name of the device opened by VRmUsbCamDS.
		if FAILED(p_VRmUsbCam.CreateInstance(CLSID_VRmUsbCam))
			throw std::exception("Could not create VRmUsbCam COM object");

		// print out version of the underlying VRmUsbCamCOM API
		std::cout << "(v." << p_VRmUsbCam->Version << ")" << std::endl << std::endl;

		// now, attach to the capture source
		IDevicePtr p_Device= p_VRmUsbCam->AttachToVRmUsbCamDS(p_VRmUsbCamDS);

		// some examples for properties / camera settings

		SAFEARRAY* p_sensorPortList = p_Device->GetSensorPortList();
		long numSensorPorts=0;
		short port;
		SafeArrayGetUBound(p_sensorPortList, 1, &numSensorPorts);
		for(long ii=0;ii<=numSensorPorts;ii++)
		{
			SafeArrayGetElement(p_sensorPortList, &ii, &port);
			// on single sensor devices this property does not exist
			PropId sensor_enable = (PropId)(PROPID_GRAB_SENSOR_ENABLE_1_B-1+port);
			if(p_Device->PropertySupported[sensor_enable])
			{
				//enable first sensor in port list
				BOOL enable = 1;
				if(ii)
					enable = 0;
				p_Device->PropertyValue[sensor_enable]=enable;
			}
		}

		long first_index=0;
		SafeArrayGetElement(p_sensorPortList, &first_index, &port);
		SafeArrayDestroy(p_sensorPortList);
		// uncomment the following lines to change exposure time to 25ms
		// when camera supports this feature
		/*		if (p_Device->PropertySupported[PROPID_CAM_EXPOSURE_TIME_F])
		{
		p_Device->PropertyValue[PROPID_CAM_EXPOSURE_TIME_F]= 25.f;
		}
		*/
		// uncomment the following lines to read/change the "readout origin".
		// this example shows the access to Size/Point or Rectangle properties
		/*		if (p_Device->PropertySupported[PROPID_GRAB_READOUT_ORIGIN_POINT_I])
		{
		// EXAMPLE: these lines read the current readout origin
		_udt_variant_t<Point> origin1= p_Device->PropertyValue[PROPID_GRAB_READOUT_ORIGIN_POINT_I];
		printf("%d, %d", origin1->X, origin1->Y);

		// EXAMPLE: these lines set the current readout origin to [0, 0]
		_udt_variant_t<Point> origin2 = p_VRmUsbCam->NewPoint(0, 0);
		p_Device->PropertyValue[PROPID_GRAB_READOUT_ORIGIN_POINT_I]= origin2;
		}
		*/
		// uncomment the following to disable trigger modes for this demo
		//p_Device->PropertyValue[PROPID_GRAB_MODE_E]= PROPID_GRAB_MODE_FREERUNNING;

		// ask the DirectShow Capture source for its selected input format
		std::cout << "Selected source format: " << std::string(p_Device->SourceFormatEx[port]->ToString()) << std::endl;


		// Create the Filter Graph Manager and query for interfaces.
		if FAILED(p_Graph.CreateInstance(CLSID_FilterGraph))
			throw std::exception("Could not initialize FilterGraph");

		// build the graph:

		// add the Capture Device Filter
		if FAILED(p_Graph->AddFilter(p_VRmUsbCamDS, L"CamSource"))
			throw std::exception("Could not add camsource filter");

		// get the output pin of the Capture device Filter
		if FAILED(p_VRmUsbCamDS->FindPin(L"1", &p_CapturePin))
			throw std::exception("Could not find pin");

		// Render the Pin
		if FAILED(p_Graph->Render(p_CapturePin))
			throw std::exception("Could not render Pin");

		// get the IMediaControl interface.
		if FAILED(p_Graph.QueryInterface(IID_IMediaControl, &p_Control))
			throw std::exception("Could not obtain the Media Control interface.");

		// get the IVideoWindow interface.
		if FAILED(p_Graph.QueryInterface(IID_IVideoWindow, &gp_VideoWindow))
			throw std::exception("Could not obtain the VideoWindow interface.");

		// set VideoWindowOwner, Style and Position
		if FAILED(gp_VideoWindow->put_Owner((OAHWND)hWnd))
			throw std::exception("Could not set the VideoWindow Owner.");
		if FAILED(gp_VideoWindow->put_WindowStyle(WS_CHILD))
			throw std::exception("Could not set Windowstyle to VideoWindow.");
		if FAILED(gp_VideoWindow->put_Top(0))
			throw std::exception("Could not set Videowindow Top position.");
		if FAILED(gp_VideoWindow->put_Left(0))
			throw std::exception("Could not set Videowindow Left position.");

		// open a settings window for the device (optional)
		IDevicePropertyPagePtr p_page;
		try
		{
			p_page= p_Device->CreatePropertyPage(0);
			Point l_loc;
			l_loc.X= 80;
			l_loc.Y= 80;
			p_page->PutLocation(&l_loc);
			if FAILED(p_page->Show())
				throw std::exception("Could not show property page.");
		}
		catch(_com_error&)
		{
		}

		// MainWindow: set size and position
		RECT l_rect;
		ZeroMemory(&l_rect, sizeof(l_rect));
		HWND l_desktopHandle = GetDesktopWindow();
		if (!GetWindowRect(l_desktopHandle, (LPRECT)&l_rect))
			throw std::exception("Could not Obtain Desktop Rect.");
		long l_width, l_height;
		if FAILED(gp_VideoWindow->get_Width(&l_width))
			throw std::exception("Could not get videowindow width.");
		if FAILED(gp_VideoWindow->get_Height(&l_height))
			throw std::exception("Could not get videowindow height.");
		if (!SetWindowPos(hWnd, 0, int((l_rect.right-l_width)/(2)), int((l_rect.bottom-l_height)/(2)), l_width, l_height, 0))
			throw std::exception("Could not set mainwindow position.");
		if (!SetWindowText(hWnd,
			("VRmagic VRmUsbCam DS Capture Source Demo - "
			+ (std::string)(p_Device->DeviceKey->ToString())).c_str()) )
			throw std::exception("Could not set mainwindow text.");

		ShowWindow(hWnd, SW_SHOW);

		// get the IMediaEventEx interface
		if FAILED(p_Graph.QueryInterface(IID_IMediaEventEx, &gp_MediaEventEx))
			throw std::exception("Could not obtain the MediaEventEx interface.");
		// set Notify window
		if FAILED(gp_MediaEventEx->SetNotifyWindow((OAHWND)hWnd, WM_GRAPH_EVENT, 0))
			throw std::exception("Could not set SetNotifyWindow.");

		// register the graph for external access
		if (AddToRot(p_Graph, &dwGraphROTid)<0)
			throw std::exception("Could not add to rot.");

		// Run the Graph
		std::cout << "running the graph..." << std::endl;
		p_Control->Run();

		// Main loop
		BOOL bRet;
		MSG Msg;
		while( (bRet = GetMessage( &Msg, NULL, 0, 0 )) != 0)
		{
			if (bRet == -1)
			{
				break;
			}
			else
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
		}
	}
	catch(_com_error& e)
	{
		std::cerr << "Error: " << (char*)(e.ErrorInfo()?e.Description():e.ErrorMessage()) << std::endl;
		MessageBox(hWnd, (char*)(e.ErrorInfo()?e.Description():e.ErrorMessage()), "VRmUsbCamDS Demo - Error", MB_OK | MB_ICONERROR);
	}
	catch(std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		MessageBox(hWnd, e.what(), "VRmUsbCamDS Demo - Error", MB_OK | MB_ICONERROR);
	}

	std::cout << "Exit." << std::endl;

	// release the global objects
	gp_VideoWindow= 0;
	gp_MediaEventEx= 0;

	// and unregister the graph
	RemoveFromRot(&dwGraphROTid);

	CoUninitialize();

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return TRUE;
	case WM_KEYDOWN:
		if (wParam!=27)
			return TRUE; // otherwise we postQuitMessage()
	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;
	case WM_SIZE:
		long l_width, l_height;
		l_width = LOWORD(lParam);
		l_height = HIWORD(lParam);
		if (gp_VideoWindow != 0)
		{
			gp_VideoWindow->put_Width(l_width);
			gp_VideoWindow->put_Height(l_height);
		}
		return TRUE;
	case WM_ERASEBKGND:
		// disable flicker OnResize
		return TRUE;
	case WM_GRAPH_EVENT:
		long l_evCode;
		LONG_PTR l_param1, l_param2;
		gp_MediaEventEx->GetEvent(&l_evCode, &l_param1, &l_param2, 0);
		if (l_evCode == EC_ERROR_STILLPLAYING)
		{
			// NOTE: when we reach here, the DirectShow CaptureSource has encountered some
			// "recoverable" error. in this case, the graph will continue to run, but you could
			// perform some required actions here...
			if (l_param1==VRmUsbCamCOM::E_TRIGGER_TIMEOUT)
				std::cout << "trigger timeout" << std::endl;
			else if (l_param1==VRmUsbCamCOM::E_TRIGGER_STALL)
				std::cout << "trigger stall" << std::endl;
			else
				std::cerr << "MediaEvent ERROR (STILLPLAYING), code: 0x" << std::hex << ((int)l_param1);
		}
		else if (l_evCode == EC_ERRORABORT)
		{
			std::stringstream l_stream;
			l_stream << "MediaEvent ERROR (ABORT), code: 0x"<< std::hex << ((int)l_param1);
			std::cerr << l_stream.str().c_str() << std::endl;
			MessageBox(hWnd, l_stream.str().c_str() , "VRmUsbCamDS Demo - Error", MB_OK | MB_ICONERROR);
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
		gp_MediaEventEx->FreeEventParams(l_evCode, l_param1, l_param2);
		return TRUE;
	default:
		return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
	IMoniker * pMoniker;
	IRunningObjectTable *pROT;
	*pdwRegister= 0;
	if (FAILED(GetRunningObjectTable(0, &pROT))) {
		return E_FAIL;
	}
	WCHAR wsz[256];
	wsprintfW(wsz, L"FilterGraph %08x pid %08x", (DWORD_PTR)pUnkGraph, GetCurrentProcessId());
	HRESULT hr = CreateItemMoniker(L"!", wsz, &pMoniker);
	if (SUCCEEDED(hr)) {
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph,
			pMoniker, pdwRegister);
		pMoniker->Release();
	}
	pROT->Release();
	return hr;
}

HRESULT RemoveFromRot(DWORD *pdwRegister)
{
	if (!(*pdwRegister))
		return S_OK;
	IRunningObjectTable *pROT;
	if (FAILED(GetRunningObjectTable(0, &pROT)))
		return E_FAIL;
	HRESULT hr = pROT->Revoke(*pdwRegister);
	pROT->Release();
	*pdwRegister= 0;
	return hr;
}
