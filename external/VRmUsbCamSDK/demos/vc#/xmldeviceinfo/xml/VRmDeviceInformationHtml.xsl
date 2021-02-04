<?xml version='1.0'?>
<!-- Stylesheet to convert Device Information XML data to HTML -->
<!-- (R) VRmagic GmbH 2006-2007 - S.Sichler                    -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
<xsl:output method="html"/>

<xsl:template match="Device">
  <html><head><title><xsl:value-of select="@product"/> Device Information</title></head>
  <body>
  <h1><b><xsl:value-of select="@product"/> (Rev <xsl:value-of select="@hardware"/>) Device Information</b></h1>
  <hr/>
  (Firmware Rev <xsl:value-of select="@firmware"/> - VRmUsbCam API v<xsl:value-of select="@api"/>)<br/><br/>
  <xsl:apply-templates select="SourceFormats"/>
  <xsl:apply-templates select="PropertyList"/>
  </body></html>
</xsl:template>

<xsl:template match="SourceFormats">
<h2><b>Default Source Format</b></h2>
<table border="1" style="border-collapse:collapse">
<thead> <tr>
<th>Sensor Port</th>
<th>Description</th>
<th>Resolution</th>
<th>Color Format</th>
<th>Depth(bits)</th>
<th>Flags</th>
<th>Framerate Maximum</th>
</tr></thead>
<tbody>
  <xsl:for-each select="SensorPort">
  <tr>
  <!-- the name is redundant and thus hidden: <td><xsl:value-of select="@name"/></td>-->
  <td><xsl:value-of select="@number"/>
  </td>
    <xsl:for-each select="Format">
    <td><xsl:value-of select="Description"/></td>
    <td><xsl:apply-templates select="Size"/></td>
    <td><xsl:value-of select="ColorFormat"/></td>
    <td><xsl:value-of select="PixelDepth*8"/></td>
    <td><xsl:apply-templates select="Flags"/></td>
    <td><xsl:apply-templates select="MaxFrameRate"/></td>
    </xsl:for-each>
  </tr>
  </xsl:for-each>
</tbody>
</table>
</xsl:template>

<xsl:template match="Format/Flags">
	<tt>
	<!--NOTE: we only show some selected flags-->
	<xsl:for-each select="INTERLACED_FRAME|INTERLACED_FIELD01|SUBSAMPLED|RUN_LENGTH_ENCODED|ORDER_TFF|ORDER_BFF"><xsl:value-of select="name()"/><br/></xsl:for-each>
	</tt>
</xsl:template>

<xsl:template match="PropertyList">
  <h2><b>Read-Only Properties</b></h2>
  <xsl:call-template name="listprops">
  <xsl:with-param name="writeable">False</xsl:with-param>
  </xsl:call-template>
  <h2><b>Writeable Properties</b></h2>
  <xsl:call-template name="listprops">
  <xsl:with-param name="writeable">True</xsl:with-param>
  </xsl:call-template>
</xsl:template>

<xsl:template name="listprops">
<xsl:param name="writeable"/>
<table border="1" style="border-collapse:collapse">
<thead> <tr>
<th>Name</th>
<th>Description</th>
<th>Default</th>
<xsl:if test="$writeable='True'">
	<th>Possible Values (Step)</th>
</xsl:if>
</tr></thead>
<tbody>
  <xsl:for-each select="Property[@writeable=$writeable]">
  	<tr>
	<td><tt><xsl:value-of select="@name"/></tt></td>
  	<td><xsl:value-of select="Description"/></td>
  	<td><xsl:apply-templates select="Default|Value"/></td>
	<xsl:if test="$writeable='True'">
  		<td><xsl:apply-templates select="Constants|Range"/></td>
	</xsl:if>
  	</tr>
  </xsl:for-each>  
</tbody>
</table>
</xsl:template>

<xsl:template match="Property[@type='Enum']/Default">
	<tt><xsl:value-of select="."/></tt>
</xsl:template>
<xsl:template match="Property[@type='Enum']/Value">
	<tt><xsl:value-of select="."/></tt>
</xsl:template>

<xsl:template match="Constants">
  <xsl:for-each select="Constant">
  <tt><xsl:value-of select="@name"/></tt> "<xsl:value-of select="Description"/>"<br/>
  </xsl:for-each>  
</xsl:template>

<xsl:template match="Range">
  <xsl:apply-templates select="Min"/> - <xsl:apply-templates select="Max"/>
  (<xsl:apply-templates select="Step"/>)
</xsl:template>

<xsl:template match="Compound[@type='Size']">
	[<xsl:value-of select="Width"/>x<xsl:value-of select="Height"/>]
</xsl:template>
<xsl:template match="Compound[@type='Point']">
	[<xsl:value-of select="X"/>,<xsl:value-of select="Y"/>]
</xsl:template>
<xsl:template match="Compound[@type='Rectangle']">
	[<xsl:value-of select="Left"/>,<xsl:value-of select="Top"/>,<xsl:value-of select="Width"/>,<xsl:value-of select="Height"/>]
</xsl:template>

</xsl:stylesheet>

