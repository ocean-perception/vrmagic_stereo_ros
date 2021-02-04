using System;
using System.Drawing;
using System.Windows.Forms;
using VRmUsbCamNET;

namespace DemoCSViewer
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class DemoCSViewer
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			try
			{
				Console.WriteLine("===================================================");
				Console.WriteLine("===   VRmagic VRmUsbCam2 .NET API Demo for C#   ===");
				Console.WriteLine("===================================================");
				Console.WriteLine("(v.{0})", VRmUsbCam.Version);
				Console.WriteLine();

				// uncomment one of the following lines to enable logging features (for customer support)
				//VRmUsbCam.EnableLogging(); // save logfile to default location 
				//VRmUsbCam.EnableLoggingEx("mylogfile.log"); //save logfile to user defined location

				// check for connected devices
				VRmUsbCamNET.DeviceKey[] devlist = VRmUsbCam.DeviceKeyList;

				// open first usable device
				VRmUsbCamNET.Device device = null;

				// step through device list and use first one that is not busy
				foreach (DeviceKey key in devlist)
				{
					if (!key.Busy)
					{
						device = VRmUsbCam.OpenDevice(key);
						break;
					}
				}

				// display error when no camera has been found
				if (device == null)
				{
					Console.WriteLine("No suitable VRmagic device found!");
					return;
				}

				// init camera, change some settings...
				// we get a targetFormat, which is necessary to initialize our
				// viewer window
				ImageFormat targetFormat = null;
				System.Int32 port = 0;
				InitCamera(device, ref port, ref targetFormat);

				// open settings window of device (optional, may fail if GUI component is not installed)
				DevicePropertyPage propPage = null;
				try
				{
					propPage = device.CreatePropertyPage();
					propPage.Location = new System.Drawing.Point(80, 80);
					propPage.Show();
				}
				catch (VRmUsbCamNET.Exception) { }

				// and read pictures...
				ReadCamera(device, port, targetFormat);

				// finally, close settings window and the device
				if (propPage != null) propPage.Dispose();
				device.Dispose();

				Console.WriteLine("exit.");
			}
			// some error handling
			catch (VRmUsbCamNET.Exception err)
			{
				MessageBox.Show("VRmUsbCam Error: " + err.Message, "VRmUsbCamNET Demo", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		private static void InitCamera(VRmUsbCamNET.Device device, ref System.Int32 firstport, ref VRmUsbCamNET.ImageFormat targetFormat)
		{
			//switch off all connected sensors but the first one
			firstport = device.SensorPortList[0];
			foreach (System.Int32 port in device.SensorPortList)
			{
				VRmUsbCamNET.PropId sensor_enable = VRmUsbCamNET.PropId.GRAB_SENSOR_ENABLE_1_B - 1 + port;
				if (device.get_PropertySupported(sensor_enable))
				{
					if (port == firstport)
						device.set_PropertyValue(sensor_enable, true);
					else
						device.set_PropertyValue(sensor_enable, false);
				}
			}

			// just for example: change exposure time to 25ms when camera supports this feature
			if (device.get_PropertySupported(PropId.CAM_EXPOSURE_TIME_F))
			{
				// uncomment this line
				// device.set_PropertyValue(PropId.CAM_EXPOSURE_TIME_F,25);
			}

			// uncomment the following to disable trigger modes for this demo
			// device.set_PropertyValue(PropId.GRAB_MODE_E, PropId.GRAB_MODE_FREERUNNING);

			VRmUsbCamNET.ImageFormat sourceFormat;

			// optional: uncomment the following lines to select a source (raw) format
			// VRmUsbCamNET.ImageFormat sourceFormats[];
			// sourceFormats = device.SourceFormatList
			// sourceFormat = sourceFormats[0]
			// device.SourceFormat = sourceFormat

			// get the currently selected source (raw) format
			sourceFormat = device.get_SourceFormat(firstport);

			Console.WriteLine("Selected source format: " + sourceFormat.ToString());

			// select a target format from the list of formats we can convert the source images to.
			// we search for the ARGB_4X8 format (that is always included in the list), since
			// rendering of this format will never fail
			foreach (VRmUsbCamNET.ImageFormat fmt in device.get_TargetFormatList(firstport))
			{
				if (ColorFormat.ARGB_4X8 == fmt.ColorFormat)
				{
					targetFormat = fmt;
					break;
				}
			}

			Console.WriteLine("Selected target format: " + targetFormat.ToString());

			// uncomment these lines to flip output image vertical / horizontal:
			// targetFormat.FlipHorizontal();
			// targetFormat.FlipVertical();
		}

		private static void ReadCamera(VRmUsbCamNET.Device device, System.Int32 port, VRmUsbCamNET.ImageFormat targetFormat)
		{
			// initialize viewer
			Viewer viewer = new Viewer(targetFormat.Size);
			Graphics g = viewer.CreateGraphics();

			viewer.Text = "VRmagic VRmUsbCam2 .NET API Demo for C# - " + device.DeviceKey.ToString();

			// ------------------------------------------------------------------------
			// main loop: read images and draw them to screen
			// ------------------------------------------------------------------------

			// alloc a buffer for the converted images
			VRmUsbCamNET.Image targetImg = VRmUsbCam.NewImage(targetFormat);

			// start image grabbing and pop up the viewer window
			device.Start();
			viewer.Show();

			while (viewer.Visible)
			{
				int framesDropped = 0;

				// lock next (raw); image for read access, convert it to the desired 
				// format (see above); and unlock it again, so that grabbing can continue
				VRmUsbCamNET.Image sourceImg = null;
				try
				{
					sourceImg = device.LockNextImage(port, out framesDropped, 5000);

					VRmUsbCam.ConvertImage(sourceImg, targetImg);
					device.UnlockNextImage(sourceImg);

					// NOTE: you could natively access or modify the targetImg here, using:
					//  byte[,] buffer = targetImg.LockBits();
					//  buffer[..,..] = ...    or     ... = buffer[..,..];
					//  targetImg.UnlockBits(ref buffer);

					// detect whether frames have been dropped
					if (framesDropped != 0)
						Console.WriteLine("- {0} frame(s) dropped -", framesDropped);

					// and draw the image
					g.DrawImage(targetImg.ToBitmap(), 0, 0);

					// uncomment this to output time stamp of image
					// Console.WriteLine("frame {0}, port {1}, timestamp {2}, now {3}", targetImg.FrameCounter, targetImg.SensorPort, targetImg.TimeStamp, VRmUsbCam.CurrentTime);
				}
				catch (VRmUsbCamNET.Exception err)
				{
					// in case of an error, check for trigger timeouts and trigger stalls.
					// both can be recovered, so continue. otherwise exit the app
					switch (err.Number)
					{
						case ErrorNumber.E_TRIGGER_TIMEOUT:
						case ErrorNumber.E_TRIGGER_STALL:
						case ErrorNumber.E_FUNCTION_CALL_TIMEOUT:
							Console.WriteLine(err.Description);
							break;
						default:
							throw;
					}
				}

				// do some window event handling
				Application.DoEvents();
			}
			// finally, stop image grabbing before we leave the application
			device.Stop();
		}
	}
}
