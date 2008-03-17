/* ========================================================================
*  PROJECT: DirectShow Video Processing Library (DSVL)
*  Version: 0.0.8 (05/13/2005)
*  ========================================================================
*  Author:  Thomas Pintaric, Vienna University of Technology
*  Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
*  =======================================================================
* 
*  Copyright (C) 2005  Vienna University of Technology
* 
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU General Public License
*  as published by the Free Software Foundation; either version 2
*  of the License, or (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program; if not, write to the Free Software
*  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
*  For further information please contact Thomas Pintaric under
*  <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
*  Vienna University of Technology, Favoritenstr. 9-11/E188/2, A-1040
*  Vienna, Austria.
* ========================================================================*/

#ifndef __PIXEL_FORMAT__
#define __PIXEL_FORMAT__

#include <GL/gl.h>

//-----------------------------------------------------------------------------------------------------------
#include "DSVL_PixelFormatTypes.h"

// format conversion helpers
int PXBitsPerPixel(PIXELFORMAT format);
WORD PXtoOpenGL(PIXELFORMAT format, bool bWIN32format = true);
GUID PXtoMEDIASUBTYPE(PIXELFORMAT format);
PIXELFORMAT MEDIASUBTYPEtoPX(GUID format);
PIXELFORMAT OpenGLtoPX(WORD format);

const char* PXtoString(PIXELFORMAT format);
PIXELFORMAT StringToPX(char* formatName);

/* FORMAT CORRESPONDENCE

>>> Video decoder's pixel format: (AM_MEDIA_TYPE.subtype for video) DirectShow 9.0

[1] MEDIASUBTYPE_UYVY    UYVY format data. A packed YUV format. A Y sample at every pixel, 
						 a U and V sample at every second pixel horizontally on each line;
						 every vertical line sampled. Probably the most popular of the various
						 YUV 4:2:2 formats. Byte ordering (lowest first) is U0, Y0, V0, Y1, U2, Y2,
						 V2, Y3, U4, Y4, V4, Y5, where the suffix 0 is the leftmost pixel and increasing
						 numbers are pixels increasing left to right. Each 4-byte block is 2 image pixels.
[2] MEDIASUBTYPE_YUY2    YUY2 format data. Same as UYVY but with different pixel ordering. Byte ordering
						 (lowest first) is Y0, U0, Y1, V0, Y2, U2, Y3, V2, Y4, U4, Y5, V4, where the suffix
						 0 is the leftmost pixel and increasing numbers are pixels increasing left to right.
						 Each 4-byte block is 2 image pixels.  						 
[3] MEDIASUBTYPE_RGB565  565 format of RGB, 16 bits per pixel. Uncompressed RGB samples.  
[4] MEDIASUBTYPE_RGB555  555 format of RGB, 16 bits per pixel. Uncompressed RGB samples.  
[5] MEDIASUBTYPE_RGB24	 RGB, 24 bits per pixel. Uncompressed RGB samples.  
[6] MEDIASUBTYPE_RGB32	 RGB, 32 bits per pixel. Uncompressed RGB samples. Do not use the alpha bits 
						 with this media type. (Compare MEDIASUBTYPE_ARGB32.)  
*/

#endif