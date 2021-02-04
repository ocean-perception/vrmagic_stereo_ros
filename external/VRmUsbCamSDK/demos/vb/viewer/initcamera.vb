' ==============================================================================================
' This file is part of the VRmagic VRmUsbCam .NET API v2 Demo Application for VB 2005
' ==============================================================================================
' Camera Initialization
' ----------------------------------------------------------------------------------------------
Imports VRmUsbCamNET
Imports System.Console

Partial Class MainModule
    Shared Sub InitCamera(ByRef device As VRmUsbCamNET.Device, ByRef firstPort As Integer, ByRef targetFormat As VRmUsbCamNET.ImageFormat)
        'switch off all connected sensors but the first one
        firstPort = device.SensorPortList(0)

        For Each port As Integer In device.SensorPortList
            Dim sensorEnable As VRmUsbCamNET.PropId = DirectCast(VRmUsbCamNET.PropId.GRAB_SENSOR_ENABLE_1_B - 1 + port, VRmUsbCamNET.PropId)
            If device.PropertySupported(sensorEnable) Then
                If port = firstPort Then
                    device.PropertyValue(sensorEnable) = True
                Else
                    device.PropertyValue(sensorEnable) = False
                End If
            End If
        Next

        ' just for example: change exposure time to 25ms when camera
        ' supports this feature
        If device.PropertySupported(VRmUsbCamNET.PropId.CAM_EXPOSURE_TIME_F) Then
            ' uncomment this line
            ' device.PropertyValue(VRmUsbCamNET.PropId.CAM_EXPOSURE_TIME_F) = 25
        End If

        ' uncomment the following to disable trigger modes for this demo
        ' device.PropertyValue(PropId.GRAB_MODE_E) = PropId.GRAB_MODE_FREERUNNING

        Dim sourceFormat As VRmUsbCamNET.ImageFormat

        ' optional: uncomment the following lines to select a source (raw) format
        ' Dim sourceFormats() As VRmUsbCamNET.ImageFormat
        ' sourceFormats = device.SourceFormatList
        ' sourceFormat = sourceFormats(0)
        ' device.SourceFormat = sourceFormat

        ' get the currently selected source (raw) format of the first sensor port
        sourceFormat = device.SourceFormat(firstPort)

        WriteLine("Selected source format: " + sourceFormat.ToString)

        ' select a target format from the list of formats we can convert the source images to.
        ' we search for the ARGB_4X8 format (that is always included in the list), since
        ' rendering of this format will never fail
        For Each fmt As VRmUsbCamNET.ImageFormat In device.TargetFormatList(firstPort)
            If fmt.ColorFormat = VRmUsbCamNET.ColorFormat.ARGB_4X8 Then
                targetFormat = fmt
                Exit For
            End If
        Next

        WriteLine("Selected target format: " + targetFormat.ToString)

        ' uncomment these lines to flip output image vertical / horizontal:
        ' targetFormat.FlipHorizontal
        ' targetFormat.FlipVertical

    End Sub
End Class
