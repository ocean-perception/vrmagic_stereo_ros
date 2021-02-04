Using VRmagic Devices with the MATLAB Image Acquisition Toolbox
(c) VRmagic GmbH, 2010-2015

(vers 3.5.0.0)

0. Installation of the SDK

Before registering the MATLAB adaptor for your VRmagic imaging device you
have to install the VRmagic VRmUsbCam DevKit from the accompanying CD.
Alternatively, you can download it from <http://www.vrmagic.com/imaging/downloads/>.

There a two ways of connecting VRmagic imaging devices to the Image
Acquisition Toolbox. You can either use the generic Video for Windows
adaptor or the VRmagic adaptor.  

1. VIDEO FOR WINDOWS:

The generic winvideo adaptor will work without any additional
registering, but offers only limited access to the device properties,
e.g. hardware triggering of the camera is not supported. The connected
camera appears in the device list returned from "imaqhwinfo('winvideo')"
and can be accessed from MATLAB, as long as no other applications
(e.g. CamLab) grab images from the device.


2. VRMAGIC ADAPTOR:

The VRmagic adaptor offers complete access to all device properties.
Furthermore it supports external triggering, User ROIs, 10 bit formats, etc.

2.1 Registering the VRmagic adaptor

In order to use the VRmagic adaptor you have to unregister the VRmagic
DirectShow wrapper. This is done by invoking the script
<VRmagicRuntime_Installation_Dir>\VRmUsbCamLibrary\install\vrmusbcamds_unregister.bat. 
(To revert these changes and re-register the DirectShow wrapper invoke
"vrmusbcamds_register.bat" located in the same directory.) After this you
have to register the VRmagic Adaptor corresponding to your Matlab Release:

	MATLAB R2007b - R2009a: vrmusbcamimaq_R2007b.dll (32bit support only)
	MATLAB R2009b - R2012b: vrmusbcamimaq_R2009b.dll (32bit support only)
	MATLAB R2013a: 			vrmusbcamimaq_R2013a.dll (32bit support only)
	MATLAB R2013b - R2015a:	vrmusbcamimaq_R2013b.dll (32bit and 64bit suppport)

The adaptor DLLs are located in the directory 
<VRmagicSDK_Installation_Dir>\VRmUsbCamLibrary\wrappers\matlab. 
To navigate there, follow the link 'SDK' in the VRmagic section of the Windows
Start Menu. You can either use the "imaqregister" command or the interactive
"imaqtool" to register the adaptor. Please consult the MATLAB 
documentation if more information on registering third party adaptors is
needed.


3. Using the VRmagic adaptor

Read the documentation of the Image Acquisition Toolbox on how to detect 
connected hardware and how to create videoinput objects for detected 
devices. The command "imaqhwinfo(‘vrmusbcamimaq_R20xxx’)" will return a
list of all devices detected by the VRmagic adaptor. 

If you are already familiar with other adaptors you will note that the
SupportedFormats field does not contain the usual format strings like
those returned by e.g. the winvideo adaptor. All VRmagic imaging devices
allow the saving of up to 9 user configurations directly on the hardware
The saved configurations and the factory defaults are returned in the
SupportedFormats field of the DeviceInfo struct array. These
configurations contain the settings of all device properties, including
the image size and color format of the images returned by the device,
which can be set after the creation of a videoinput object.

3.1 Device Properties

The VRmagic adaptor offers access to most of the imaging device's
properties as known from the VRmUsbCam C-API and from the CamLab
application. After creation of a videoinput object for a certain
configuration, all available properties are listed in the Device
Properties Tab of the imaqtool GUI or in the corresponding videosource
object, which you can create with the "getselectedsource(vid)" command.
Setting them in MATLAB causes the device to update automatically.

For your convenience, some of the VRmUsbCam properties have been mapped
on properties provided by the MATLAB Image Acquisition Toolbox, e.g. the
property for the trigger configuration.

In addition to the properties also known in the VRmUsbCam API, there are
two new properties: 
a. save_as_config_no
b. save_current_config

The format of the returned image is determined by several device properties. 
These are:
- 'grab_user_roi_rect_i' to set the hardware region of interest 
- 'grab_readout_origin_point_i' to move the origin of the ROI
- 'grab_source_format_e' to set the pixel depth of the source image

The specified properties for image size and pixel depth are used internally by the
adaptor to convert the captured images into the format specified by the
videoinput object property "ReturnedColorSpace".
8 bit source formats are returned as three channel color or single
channel 8 bit gray images, while the 10 bit formats and RGB565 are
returned as 16 bit formats. 
E.g. if you set grab_source_format = ‘GRAB_SOURCE_FORMAT_8BIT_RAW’ and
ReturnedColorSpace=’rgb’, you will get a 3 channel, 8 bit RGB image, 
if you set grab_source_format = ‘GRAB_SOURCE_FORMAT_16BIT_RAW’ and
ReturnedColorSpace=’grayscale’, you will get a 1 channel, 16 bit
grayscale image, etc.

The properties "save_as_config_no" and "save_current_config" can be used
to store the current configuration on board of the device. By setting
"save_current_config" to 1, the configuration can be stored to the slot
number which is specified by the property "save_as_config_no". The
numbers in brackets behind the configurations in the supported format
list equal the slot number (e.g. user defaults (1)). After the
configuration has been stored to the device the "save_current_config"
property will be reset to 0 by the adaptor. This allows you to transfer
the configurations from MATLAB to CamLab or your own applications
written with the VRmUsbCam C API. Additionally, you can change the name
shown in the SupportedFormats list by changing the string property
"grab_config_description_s". However, changes will be updated in Matlab
after refreshing the toolbox with a call to "imaqreset". Note, that you 
cannot replace the factory defaults of slot (0).

Hint: Instead of setting the properties in Matlab, you can also do all
device settings in Camlab, save the configuration to a user
configuration, and then use it from within MATLAB.


3.2 User Region of Interest

This adaptor provides two ways of setting User ROIs: 
a) hardware ROI and b) software ROI. 
The software ROI has the advantage that is easier to use, because you
directly specify the image region you want to get out of the toolbox.
The advantage of the hardware ROI is that only a smaller amount of data
has to be transferred from the device, thus allowing higher frame rates.
You can also combine the two methods.

a) The software ROI you can set in the ROI tab of imaqtool by selecting a
region in the shown preview image or by setting the ROIPosition property
of the videoinput object. A larger image is acquired from the device and 
cropped in the adaptor.

b) The hardware ROI can be set by changing the property 
grab_user_roi_rect_i" and choosing one of the source formats starting
with "User ROI". This property directly specifies the region on the
imaging sensor that is read out by the hardware and is applied BEFORE any
image processing like mirroring, subsampling or color conversion. Note
that on color devices the hardware ROI is at least 2 pixels larger than
the final image, because of the Bayer filtering.

3.3 Triggering

The adaptor supports immediate trigger and manual triggers for all 
VRmagic devices. Additionally, since many VRmagic devices offer an 
external hardware trigger, a third MATLAB trigger configuration
has been created. You can switch to hardware triggering using the 
following commands:

conf=triggerconfig(...);
triggerconfig(...,conf(3));

Note that the property TriggerCondition has only one possible value ("Use 
property CAM_TRIGGER_POLARITY_E'), which indicates that the equivalent
property in the VRmUsbCam property set is used to determine the trigger
condition. It has four possible values, edge sensitive (pos/neg) and
level sensitive (pos/neg).
VRmagic devices do not support grabbing multiple frames upon one trigger
signal, so you need to generate FramesPerTrigger * (TriggerRepeat+1)
trigger signals to grab all frames requested by the toolbox.


3.4. Metadata

VRmagic has extended the metadata fields that are returned by
[data, time, metadata] = getdata(...).
It now contains the following fields: 
	
		AbsTime
		FrameNumber
		RelativeFrame
		TriggerIndex
(NEW)   image_capture_time_stamp 
(NEW)   sensor_port 

The field "image_capture_time_stamp" specifies the end time of image
acquisition on the device in milliseconds since the last reset of the
internal device timer. The device timer is reset when the device is
detected by the toolbox ("imaqhwinfo"). 

The field "sensor_port" specifies the sensor number of the acquired
image, but is only needed for applications utilizing VRmagic multi-sensor
devices.

4. Support for multi-sensor devices (VRm-MFC-X, VRm-DMFC-X)

VRmagic multi-sensor devices which can acquire up to 4 images pixel-
synchronously are supported by this adaptor. The images from the
different sensors are queued into the toolbox's image acquisition chain
in a non-deterministic order. Use the "sensor_port" and
"image_capture_time_stamp" properties to sort them (see previous
 chapter). 
Set the FramesPerTrigger property of the videoinput object to the number
of sensors of the device if you want to acquire a complete set of images
from all sensors for every trigger event. 

Limitation: Currently, the adaptor does not support different formats on the
different sensors of the multi-sensor camera. 
