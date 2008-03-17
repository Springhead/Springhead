/* ========================================================================
* PROJECT: DirectShow Video Processing Library (DSVL)
* Version: 0.0.8 (05/13/2005)
* ========================================================================
* Author:  Thomas Pintaric, Vienna University of Technology
* Contact: pintaric@ims.tuwien.ac.at http://ims.tuwien.ac.at/~thomas
* =======================================================================
*
* Copyright (C) 2005  Vienna University of Technology
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
* For further information please contact Thomas Pintaric under
* <pintaric@ims.tuwien.ac.at> or write to Thomas Pintaric,
* Vienna University of Technology, Favoritenstr. 9-11/E188/2, A1040 Vienna,
* Austria.
* ========================================================================*/


#ifndef __DSVL_HELPERS__
#define __DSVL_HELPERS__

HRESULT getPin(IBaseFilter *flt, PIN_DIRECTION dir, int number, CComPtr<IPin> &pRetPin);
HRESULT ConnectFilters(IBaseFilter *filter_out, int out_pin_nr,
					   IBaseFilter *in_filter, int in_pin_nr);
HRESULT AutoConnectFilters(IBaseFilter *filter_out, int out_pin_nr, IBaseFilter *in_filter,
											  int in_pin_nr, IGraphBuilder *pGraphBuilder);
char* strtok_prefix(char* str, const char* prefix);
void uuidToString(char* dest, int dest_size, GUID* uuid_p);

// Rounds a number to a specified number of digits.
// Number is the number you want to round.
// Num_digits specifies the number of digits to which you want to round number.
// If num_digits is greater than 0, then number is rounded to the specified number of decimal places.
// If num_digits is 0, then number is rounded to the nearest integer.
// Examples
//		ROUND(2.15, 1)		equals 2.2
//		ROUND(2.149, 1)		equals 2.1
//		ROUND(-1.475, 2)	equals -1.48
float Round(const float &number, const int num_digits);
double RoundDouble(double doValue, int nPrecision);

HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);
void RemoveFromRot(DWORD pdwRegister);

HRESULT DisplayPinProperties(CComPtr<IPin> pSrcPin, HWND hWnd = NULL);
HRESULT DisplayFilterProperties(IBaseFilter *pFilter, HWND hWnd = NULL);

HRESULT FindCaptureDevice(IBaseFilter ** ppSrcFilter, 
						  LPWSTR filterNameSubstring = NULL,
						  bool matchDeviceName = false,
						  char* ieee1394id_str = NULL); // unique 64-bit identifier 
                                                        // (use output from IEEE1394_id.exe)

bool CanDeliverVideo(IPin *pin);
// checks if a pin can deliver MEDIAFORMAT_Video

bool CanDeliverDV(IPin *pin);
// checks if a pin can deliver compressed DV formats
// (refer to Microsoft DirectX Documentation, "DV Video Subtypes" for more information)

/*
FOURCC	GUID				Data Rate	Description 
'dvsl'	MEDIASUBTYPE_dvsl	12.5 Mbps	SD-DVCR 525-60 or SD-DVCR 625-50 
'dvsd'	MEDIASUBTYPE_dvsd	25 Mbps		SDL-DVCR 525-60 or SDL-DVCR 625-50 
'dvhd'	MEDIASUBTYPE_dvhd	50 Mbps		HD-DVCR 1125-60 or HD-DVCR 1250-50 
*/


HRESULT AMErrorMessage(HRESULT hr, char* error_title = "DirectShow ERROR");

void FlipImageRGB32(BYTE* pBuf, int width, int height, 
					bool flipImageH, bool flipImageV);


#endif