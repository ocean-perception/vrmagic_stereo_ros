' ==============================================================================================
' This file is part of the VRmagic VRmUsbCam2 .NET API Demo Application for VB 2005
' ==============================================================================================
' Main Function
' ----------------------------------------------------------------------------------------------
Imports VRmUsbCamNET
Imports System.Console
Imports System.Windows.Forms

Partial Class MainModule
    Shared Sub Main()
        Try
            WriteLine("========================================================")
            WriteLine("===   VRmagic VRmUsbCam2 .NET API Demo for VB.NET    ===")
            WriteLine("========================================================")
            WriteLine("(v.{0})", VRmUsbCam.Version)
            WriteLine()

            ' uncomment one of the following lines to enable logging features of VRmUsbCam (for customer support)
            ' VRmUsbCam.EnableLogging()  ' save logfile to default location 
            ' VRmUsbCam.EnableLoggingEx("mylogfile.log")  ' save logfile to user defined location

            ' check for connected devices
            Dim devlist() As VRmUsbCamNET.DeviceKey = VRmUsbCam.DeviceKeyList

            ' open first usable device
            Dim device As VRmUsbCamNET.Device = Nothing
            For Each key As VRmUsbCamNET.DeviceKey In devlist
                If Not key.Busy Then
                    device = VRmUsbCam.OpenDevice(key)
                    Exit For
                End If
            Next

            ' display error when no camera has been found
            If device Is Nothing Then
                Console.WriteLine("No suitable VRmagic device found!")
                Exit Sub
            End If

            ' init camera, change some settings...
            ' we get a targetFormat in return, which is necessary to initialize our
            ' viewer window
            Dim targetFormat As New VRmUsbCamNET.ImageFormat
            Dim firstPort As Integer
            InitCamera(device, firstPort, targetFormat)

            ' open settings window of device (optional, may fail if GUI component is not installed)
            Dim propPage As VRmUsbCamNET.DevicePropertyPage = Nothing
            Try
                propPage = device.CreatePropertyPage()
                propPage.Location = New Drawing.Point(80, 80)
                propPage.Show()
            Catch err As VRmUsbCamNET.Exception
            End Try

            ' and read pictures...
            ReadCamera(device, firstPort, targetFormat)

            ' finally, close settings window and the device
            If propPage IsNot Nothing Then propPage.Dispose()
            device.Dispose()

            WriteLine("exit.")

            ' some error handling
        Catch err As VRmUsbCamNET.Exception
            MessageBox.Show("VRmUsbCam Error: " + err.Message, "VRmUsbCamNET Demo", MessageBoxButtons.OK, MessageBoxIcon.Error)
        End Try
    End Sub
End Class
