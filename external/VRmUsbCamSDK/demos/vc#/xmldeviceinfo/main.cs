using System;
using System.Xml;
using System.IO;
using System.Text;
using System.Threading;
using VRmUsbCamNET;

namespace XmlDeviceInfo
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	sealed class XmlDeviceInfo
	{
		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			Console.WriteLine("=====================================================");
			Console.WriteLine("===  VRmagic VRmUsbCam .NET API v2 XmlDeviceInfo  ===");
			Console.WriteLine("=====================================================");
			Console.WriteLine("(v.{0})", VRmUsbCam.Version);
			Console.WriteLine();

			// uncomment one of the following lines to enable logging features (for customer support)
			//VRmUsbCam.EnableLogging(); // save logfile to default location 
			//VRmUsbCam.EnableLoggingEx("mylogfile.log"); //save logfile to user defined location

			// switch to "english" culture to have "." as decimal point
			Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");

			// check for connected devices
			VRmUsbCamNET.DeviceKey[] devlist = VRmUsbCam.DeviceKeyList;

			// open first usable device
			VRmUsbCamNET.Device dev = null;

			// step through device list and use first one that is not busy
			foreach (DeviceKey key in devlist)
			{
				if (!key.Busy)
				{
					dev = VRmUsbCam.OpenDevice(key);
					break;
				}
			}

			// display error when no camera has been found
			if (dev == null)
			{
				Console.WriteLine("No suitable VRmagic device found!");
				return;
			}

			// reset device to factory settings
			dev.LoadConfig(0);

			// determine hardware, firmware and API version
			int fwRev = 0;
			if (dev.get_PropertySupported(PropId.DEVICE_FIRMWARE_REVISION_I))
				fwRev = (int)dev.get_PropertyValue(PropId.DEVICE_FIRMWARE_REVISION_I);
			int hwRev = 0;
			if (dev.get_PropertySupported(PropId.DEVICE_HARDWARE_REVISION_I))
				hwRev = (int)dev.get_PropertyValue(PropId.DEVICE_HARDWARE_REVISION_I);
			int apiRev = VRmUsbCam.Version;
			string fwRevString = (fwRev / 100).ToString() + "." + (fwRev % 100).ToString("D2");
			string hwRevString = (hwRev / 100).ToString() + "." + (hwRev % 100).ToString();
			string apiRevString = apiRev.ToString()[0] + "." + apiRev.ToString()[1]
				+ "." + apiRev.ToString()[2] + "." + apiRev.ToString()[3];

			// copy xsl to temporary directory
			File.Copy(Path.Combine("xml", "VRmDeviceInformationHtml.xsl"), Path.Combine(Path.GetTempPath(), "VRmDeviceInformationHtml.xsl"), true);

			// open XML output file
			string xmlFilename = System.IO.Path.Combine(Path.GetTempPath(), dev.DeviceKey.Product.Replace("/", "_") + "_Rev" + hwRevString + "_Fw" + fwRevString + ".xml");
			Console.WriteLine("Generating XML file {0}...", xmlFilename);
			XmlTextWriter doc = new XmlTextWriter(xmlFilename, null);
			doc.Formatting = Formatting.Indented;

			doc.WriteStartDocument();
			doc.WriteComment("This file is auto-generated by VRmUsbCam XmlDeviceInfo. (R) VRmagic GmbH 2006");
			doc.WriteProcessingInstruction("xml-stylesheet", "type='text/xsl' href='VRmDeviceInformationHtml.xsl'");

			// write out general informations about device & API
			doc.WriteStartElement("Device");
			doc.WriteAttributeString("product", dev.DeviceKey.Product);
			doc.WriteAttributeString("hardware", hwRevString);
			doc.WriteAttributeString("firmware", fwRevString);
			doc.WriteAttributeString("api", apiRevString);

			doc.WriteStartElement("SourceFormats");
			foreach (int port in dev.SensorPortList)
			{
				doc.WriteStartElement("SensorPort");
				doc.WriteAttributeString("number", port.ToString());
				if (dev.get_PropertySupported(PropId.GRAB_SENSOR_PROPS_SELECT_E))
				{
					dev.set_PropertyValue(PropId.GRAB_SENSOR_PROPS_SELECT_E, PropId.GRAB_SENSOR_PROPS_SELECT_1 + port - 1);
				}
				float maxFrameRate = 0.0f;
				if (dev.get_PropertySupported(PropId.CAM_FRAMERATE_MAX_F))
					maxFrameRate = (float)dev.get_PropertyValue(PropId.CAM_FRAMERATE_MAX_F);
				VRmUsbCamNET.ImageFormat currentSourceFormat = dev.get_SourceFormat(port);
				writeFormat(currentSourceFormat, doc, 0, true, maxFrameRate);
				doc.WriteEndElement();//end of "SensorPort"
			}
			if (dev.get_PropertySupported(PropId.GRAB_SENSOR_PROPS_SELECT_E))
			{
				dev.set_PropertyValue(PropId.GRAB_SENSOR_PROPS_SELECT_E, PropId.GRAB_SENSOR_PROPS_SELECT_1 + dev.SensorPortList[0] - 1);
			}
			doc.WriteEndElement();// end of "SourceFormats"

			// dump device properties
			doc.WriteStartElement("PropertyList");
			foreach (PropId id in dev.PropertyList)
				writeProperty(dev, doc, id);

			doc.WriteEndElement(); //end of "PropertyList"

			doc.WriteEndElement(); //end of "Device"
			doc.Close();
			Console.WriteLine("Finished!");

			// try to open the generated file 
			System.Diagnostics.Process viewer = new System.Diagnostics.Process();
			viewer.StartInfo.FileName = xmlFilename;
			viewer.Start();
		}

		private static void writeFormat(ImageFormat format, XmlTextWriter doc, int index, bool isSourceFormat, float maxFrameRate)
		{
			doc.WriteStartElement("Format");
			doc.WriteAttributeString("name", format.ToString());
			if (!isSourceFormat)
				doc.WriteAttributeString("index", index.ToString());
			doc.WriteElementString("Description", format.Description);
			writeVal("Size", doc, format.Size);
			doc.WriteElementString("ColorFormat", format.ColorFormat.ToString());
			doc.WriteElementString("PixelDepth", format.PixelDepth.ToString());
			if (maxFrameRate != 0.0f)
				doc.WriteElementString("MaxFrameRate", maxFrameRate.ToString());
			doc.WriteStartElement("Flags");
			foreach (ImageModifier flag in (Enum.GetValues(typeof(ImageModifier))))
				if ((format.ImageModifier & flag) != 0)
					doc.WriteElementString(flag.ToString(), "");
			doc.WriteEndElement(); // "Flags"
			if (isSourceFormat)
			{
				doc.WriteStartElement("TargetFormatList");
				for (int ti = 0; ti <= format.TargetFormatList.Length - 1; ++ti)
					writeFormat(format.TargetFormatList[ti], doc, ti, false, maxFrameRate);
				doc.WriteEndElement(); //end of "TargetFormatList";
			}
			doc.WriteEndElement(); // end of "Format"
		}

		// writes a property out to XML
		private static void writeProperty(Device dev, XmlTextWriter doc, PropId id)
		{
			PropInfo pi = dev.get_PropertyInfo(id);

			bool isEnumConst = ((int)id >= 0x10000);

			// filter away hardware/firmware revision, because it's already in the XML document header
			if (id == PropId.DEVICE_FIRMWARE_REVISION_I || id == PropId.DEVICE_HARDWARE_REVISION_I)
				return;

			if (!isEnumConst)
				doc.WriteStartElement("Property");
			else
				doc.WriteStartElement("Constant");

			doc.WriteAttributeString("name", pi.IdString);
			doc.WriteAttributeString("type", pi.Type.Name.Replace("PropId", "Enum"));
			doc.WriteAttributeString("writeable", pi.Writeable.ToString());
			doc.WriteAttributeString("id", "H" + id.ToString("X"));

			doc.WriteElementString("Description", pi.Description);

			if (!isEnumConst)
			{
				PropAttribs pa = dev.get_PropertyAttribs(id);

				// write out current value (this is the factory default value, because in the beginning, we 
				// loaded factory defaults);
				writeVal("Default", doc, dev.get_PropertyValue(id));
				if (pi.Type.Name == "PropId")
				{
					// for enum properties, list out all possible constants
					doc.WriteStartElement("Constants");
					for (PropId enumconst = (PropId)pa.Min; enumconst <= (PropId)pa.Max; ++enumconst)
						if (dev.get_PropertySupported(enumconst))
							writeProperty(dev, doc, enumconst);
					doc.WriteEndElement();
				}
				else if (pi.Writeable)
				{
					// otherwise, list value range
					doc.WriteStartElement("Range");
					writeVal("Min", doc, pa.Min);
					writeVal("Max", doc, pa.Max);
					writeVal("Step", doc, pa.Step);
					doc.WriteEndElement();
				}
			}
			else
			{
				// for enum property constants, just print out the value
				writeVal("Value", doc, dev.get_PropertyValue(id));
			}
			doc.WriteEndElement();
		}

		// writes a property value out to the XML. reformats Size,Point and Rectangle data type
		private static void writeVal(string elementname, XmlTextWriter doc, Object val)
		{
			doc.WriteStartElement(elementname);
			if (val is System.Drawing.Size)
			{
				System.Drawing.Size size = (System.Drawing.Size)val;
				doc.WriteStartElement("Compound");
				doc.WriteAttributeString("type", size.GetType().Name);
				doc.WriteElementString("Width", size.Width.ToString());
				doc.WriteElementString("Height", size.Height.ToString());
				doc.WriteEndElement();
			}
			else if (val is System.Drawing.Point)
			{
				System.Drawing.Point point = (System.Drawing.Point)val;
				doc.WriteStartElement("Compound");
				doc.WriteAttributeString("type", point.GetType().Name);
				doc.WriteElementString("X", point.X.ToString());
				doc.WriteElementString("Y", point.Y.ToString());
				doc.WriteEndElement();
			}
			else if (val is System.Drawing.Rectangle)
			{
				System.Drawing.Rectangle rect = (System.Drawing.Rectangle)val;
				doc.WriteStartElement("Compound");
				doc.WriteAttributeString("type", rect.GetType().Name);
				doc.WriteElementString("Left", rect.Left.ToString());
				doc.WriteElementString("Top", rect.Top.ToString());
				doc.WriteElementString("Width", rect.Width.ToString());
				doc.WriteElementString("Height", rect.Height.ToString());
				doc.WriteEndElement();
			}
			else
			{
				doc.WriteString(val.ToString());
			}
			doc.WriteEndElement();
		}
	}
}
