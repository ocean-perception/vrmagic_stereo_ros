//////////////////////////////////////////////////////////////////////////////////////
// exposuretrigger C# Demo Application for VRmagic Camera devices (c) VRmagic, 2013 //
//////////////////////////////////////////////////////////////////////////////////////

using System;
using VRmUsbCamNET;

namespace exposuretrigger
{
	class Program
	{
		// device object for camera
		static private VRmUsbCamNET.Device m_device;
		// object for target image
		static private VRmUsbCamNET.Image m_targetImg = null;

		static void Main(string[] args)
		{
			// update the device key list
			VRmUsbCam.UpdateDeviceKeyList();

			// check for connected devices
			VRmUsbCamNET.DeviceKey[] devlist = VRmUsbCam.DeviceKeyList;

			if (devlist.Length == 0)
			{
				Console.WriteLine("No device found!");
				return;
			}

			// Open first device
			m_device = VRmUsbCam.OpenDevice(devlist[0]);

			// print some basic info about device
			Console.WriteLine("Opened: " + m_device.DeviceKey.Product);

			// Select the first target format in the list
			m_targetImg = VRmUsbCam.NewImage(m_device.get_TargetFormatList(1)[0]);

			// set grabbing mode to software trigger
			m_device.set_PropertyValue(PropId.GRAB_MODE_E, PropId.GRAB_MODE_TRIGGERED_SOFT);

			// activate camera
			m_device.Start();

			VRmUsbCamNET.Image[] target_image = new VRmUsbCamNET.Image[4];

			Console.WriteLine("Camera is set up!");

			// setup timer for complete acquisition
			System.DateTime timer = System.DateTime.Now;

			// set exposure time for first image: 50ms
			m_device.set_PropertyValue(PropId.CAM_EXPOSURE_TIME_F, 50);
			target_image[0] = ReadImage();

			// set exposure time for second image: 25ms
			m_device.set_PropertyValue(PropId.CAM_EXPOSURE_TIME_F, 25);
			target_image[1] = ReadImage();

			// set exposure time for third image: 10ms
			m_device.set_PropertyValue(PropId.CAM_EXPOSURE_TIME_F, 10);
			target_image[2] = ReadImage();

			// set exposure time for fourth image: 5ms
			m_device.set_PropertyValue(PropId.CAM_EXPOSURE_TIME_F, 5);
			target_image[3] = ReadImage();

			System.TimeSpan duration = System.DateTime.Now - timer;
			Console.WriteLine("Acquisition of 4 images took: {0}ms", duration.TotalMilliseconds);
			timer = System.DateTime.Now;

			// Save images to disk
			int i = 0;
			foreach (VRmUsbCamNET.Image image in target_image)
			{
				image.ToBitmap().Save("image" + (++i).ToString() + ".png", System.Drawing.Imaging.ImageFormat.Png);
			}

			duration = System.DateTime.Now - timer;
			Console.WriteLine("Saving took: {0}ms", duration.TotalMilliseconds);

			// cleanup
			m_device.Stop();
			m_device.Dispose();
		}

		static private VRmUsbCamNET.Image ReadImage()
		{
			// lock next (raw); image for read access, convert it to the desired 
			// format (see above); and unlock it again, so that grabbing can go on
			VRmUsbCamNET.Image sourceImg = null;

			while (true)
			{
				try
				{
					m_device.SoftTrigger();
					int framesDropped;
					sourceImg = m_device.LockNextImage(0, out framesDropped, 500);
					VRmUsbCam.ConvertImage(sourceImg, m_targetImg);
					VRmUsbCamNET.Image returnImage = (VRmUsbCamNET.Image)m_targetImg.Clone();
					return returnImage;
				}
				catch (VRmUsbCamNET.Exception e)
				{
					// Unfortunately we have encountered an error:
					Console.WriteLine(e.ToString());
				}
				finally
				{
					// Always unlock the image, otherwise the device cannot be correctly closed and reopened
					if (sourceImg != null)
						m_device.UnlockNextImage(sourceImg);
				}
			}
		}

	}
}
