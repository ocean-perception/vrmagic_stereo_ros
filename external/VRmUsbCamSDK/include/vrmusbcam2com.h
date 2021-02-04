// ==============================================================================================
/// @file vrmusbcam2com.h                                     VRmUsbCam COM API Importer v3.5.0.0
//  Copyright VRmagic 2004-2014
// ==============================================================================================

#ifndef VRMUSBCAM2COM_H
#define VRMUSBCAM2COM_H

// This file imports the vrmusbcam2.dll type library and adds some utilities, so that the COM API
// can easily be used. see DShow Demo for an example!

#include <oleauto.h>

#define LIBVER_VRmUsbCamCOM 3
#define LIBSUBVER_VRmUsbCamCOM 5

namespace VRmUsbCamCOM {

// Small utility class that helps in dealing with record(UDT)-typed COM VARIANTs of VRmUsbCamCOM
// like Size, Point and Rectangle
template<typename T>
class _udt_variant_t : public ::tagVARIANT {
public:
	// create a null-initialized record variant
	_udt_variant_t() {
		vt= VT_EMPTY;
		getRecInfo();
		pvRecord= pRecInfo->RecordCreate();
		vt= VT_RECORD;
	}
	// create a record variant from another VARIANT or _variant_t
	_udt_variant_t(const tagVARIANT& var) {
		vt= VT_EMPTY;
		getRecInfo();
		if (var.vt!=VT_RECORD || pRecInfo!=var.pRecInfo)
			_com_issue_error(DISP_E_TYPEMISMATCH);
		_com_util::CheckError(pRecInfo->RecordCreateCopy(var.pvRecord, &pvRecord));
		vt= VT_RECORD;
	}
	// create a record variant from the contained UDT
	_udt_variant_t(const T& record) {
		vt= VT_EMPTY;
		getRecInfo();
		_com_util::CheckError(pRecInfo->RecordCreateCopy((PVOID)&record, &pvRecord));
		vt= VT_RECORD;
	}
	// default d-tor
	~_udt_variant_t() {
		_com_util::CheckError(::VariantClear(this));
	}
	// convert to contained UDT
	operator T() const { return *(T*)pvRecord; }
	// access contained UDT
	T* operator ->() { return (T*)pvRecord; }

private:
	void getRecInfo() {
		_com_util::CheckError(
			GetRecordInfoFromGuids(
			LIBID_VRmUsbCamCOM,
			LIBVER_VRmUsbCamCOM,
			LIBSUBVER_VRmUsbCamCOM,
			0,
			__uuidof(T), &pRecInfo));
	}
};

}

// finally, import the VRmUsbCamCOM API into VRmUsbCamCOM namespace
#ifdef _M_IX86
	#import "..\wrappers\com\x86\VRmUsbCamCOM.tlb" named_guids
#elif defined(_M_X64)
	#import "..\wrappers\com\x64\VRmUsbCamCOM.tlb" named_guids
#endif

#endif//VRMUSBCAM2COM_H
