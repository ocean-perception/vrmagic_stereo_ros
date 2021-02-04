Description of C# Demos
-----------------------

How to use:

linux-pc-x86
------------
1) sudo apt-get update
2) sudo apt-get install monodevelop mono-gmcs
3) Open *.sln file in monodevelop
4) Make sure VRmagic camera .NET wrapper reference is correct
5) Build solution

linux-d3-armhf
--------------
1) Install VRmagic camera .NET wrapper (vrm-usbcamnet) on D3
2) Build solution on host PC
3) Copy executables to D3.

Folder "exposuretrigger":
C# Demo program which takes a series of pictures from first capture device,
with project for Visual C# 2008

Folder "viewer":
C# Demo program using VRmUsbCam .NET API v2 and .Net Viewer,
with project for Visual C# 2008

Folder "xmldeviceinfo":
C# Demo program using VRmUsbCam .NET API v2 which generates a xml file with infos of connected devices,
with project for Visual C# 2008
