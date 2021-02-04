' ==============================================================================================
' This file is part of the VRmagic VRmUsbCam2 .NET API Demo Application for VB 2005
' ==============================================================================================
' Camera Reading Loop
' ----------------------------------------------------------------------------------------------
Imports VRmUsbCamNET
Imports System.Console
Imports System.Drawing
Imports System.Windows.Forms

Partial Class MainModule
    Shared Sub ReadCamera(ByRef device As VRmUsbCamNET.Device, ByVal port As Integer, ByVal targetFormat As VRmUsbCamNET.ImageFormat)

        ' initialize viewer
        Dim viewer As New Viewer(targetFormat.Size)
        Dim g As Graphics = viewer.CreateGraphics

        viewer.Text = "VRmagic VRmUsbCam2 .NET API Demo for VB.NET - Device: " + device.DeviceKey.ToString

        ' ------------------------------------------------------------------------
        ' main loop: read images and draw them to screen
        ' ------------------------------------------------------------------------

        ' alloc a buffer for the converted images
        Dim targetImg As VRmUsbCamNET.Image = VRmUsbCam.NewImage(targetFormat)

        ' start image grabbing and pop up the viewer window
        device.Start()
        viewer.Show()

        Do While viewer.Visible
            Dim framesDropped As Integer
            ' lock next (raw) image for read access, convert it to the desired 
            ' format (see above) and unlock it again, so that grabbing can
            ' go on
            Dim sourceImg As VRmUsbCamNET.Image = Nothing
            Try
                sourceImg = device.LockNextImage(port, framesDropped, 5000)
                VRmUsbCam.ConvertImage(sourceImg, targetImg)
                device.UnlockNextImage(sourceImg)

                ' NOTE: you could natively access or modify the targetImg here, using:
                ' Dim buffer(,) As Byte = targetImg.LockBits
                ' buffer(...,...) = ...    or     ... = buffer(...,...)
                ' targetImg.UnlockBits(buffer)

                ' detect whether frames have been dropped
                If framesDropped <> 0 Then WriteLine("- {0} frame(s) dropped -", framesDropped)

                ' and draw the image
                g.DrawImage(targetImg.ToBitmap, 0, 0)

                ' uncomment this to output time stamp of image
                ' WriteLine("frame {0}, port {1}, timestamp {2}, now {3}", targetImg.FrameCounter, targetImg.SensorPort, targetImg.TimeStamp, VRmUsbCam.CurrentTime)
            Catch err As VRmUsbCamNET.Exception
                ' in case of an error, check for trigger timeouts and trigger stalls.
                ' both can be recovered, so continue. otherwise exit the app
                Select Case err.Number
                    Case VRmUsbCamNET.ErrorNumber.E_TRIGGER_TIMEOUT, VRmUsbCamNET.ErrorNumber.E_TRIGGER_STALL, VRmUsbCamNET.ErrorNumber.E_FUNCTION_CALL_TIMEOUT
                        WriteLine(err.Description)
                    Case Else
                        Throw
                End Select
            End Try

            ' do some window event handling
            Application.DoEvents()
        Loop
        ' finally, stop image grabbing before we leave the application
        device.Stop()
    End Sub
End Class
