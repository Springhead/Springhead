/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_DREzUSB_H
#define SPR_DREzUSB_H
//	Cypress開発環境の ezusbsys.h から必要な定数だけ持ってきました．
//	コメントなどは元ファイルを見てください．


namespace Spr{;



struct BULK_TRANSFER_CONTROL{
//	ULONG pipeNum;
	unsigned long pipeNum;
};
/*	
struct USB_DEVICE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
};
*/

struct USB_DEVICE_DESCRIPTOR {
    unsigned char bLength;
    unsigned char bDescriptorType;
    unsigned short bcdUSB;
    unsigned char bDeviceClass;
    unsigned char bDeviceSubClass;
    unsigned char bDeviceProtocol;
    unsigned char bMaxPacketSize0;
    unsigned short idVendor;
    unsigned short idProduct;
    unsigned short bcdDevice;
    unsigned char iManufacturer;
    unsigned char iProduct;
    unsigned char iSerialNumber;
    unsigned char bNumConfigurations;
};

#define Ezusb_IOCTL_INDEX  0x0800

#define IOCTL_Ezusb_GET_DEVICE_DESCRIPTOR CTL_CODE(FILE_DEVICE_UNKNOWN,	 \
												   Ezusb_IOCTL_INDEX+1,\
												   METHOD_BUFFERED,	 \
												   FILE_ANY_ACCESS)


#define IOCTL_EZUSB_BULK_READ			  CTL_CODE(FILE_DEVICE_UNKNOWN,	 \
												   Ezusb_IOCTL_INDEX+19,\
												   METHOD_OUT_DIRECT,  \
												   FILE_ANY_ACCESS)

#define IOCTL_EZUSB_BULK_WRITE			  CTL_CODE(FILE_DEVICE_UNKNOWN,	 \
												   Ezusb_IOCTL_INDEX+20,\
												   METHOD_IN_DIRECT,  \
												   FILE_ANY_ACCESS)

#define IOCTL_Ezusb_RESETPIPE  CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Ezusb_IOCTL_INDEX+13,\
                                                   METHOD_IN_DIRECT,  \
                                                   FILE_ANY_ACCESS)

#define IOCTL_Ezusb_ABORTPIPE  CTL_CODE(FILE_DEVICE_UNKNOWN,  \
                                                   Ezusb_IOCTL_INDEX+15,\
                                                   METHOD_IN_DIRECT,  \
                                                   FILE_ANY_ACCESS)
}	//	namespace Spr

#endif
