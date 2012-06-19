#ifndef DRUSBH8SIMPLEDEF_H
#define DRUSBH8SIMPLEDEF_H

///////////////////////////////////////////////////////////
//  Spidar user application interface
//  Copyright (c) 2001, CyVerse Corporation
///////////////////////////////////////////////////////////

#define SPIDAR_DEVICE_TYPE              0x8000
#define SPIDAR_MAKE_IOCTL(t,c)\
        (ULONG)CTL_CODE((t), 0x800+(c), METHOD_BUFFERED, FILE_ANY_ACCESS)

    // TODO:  Specify the interface for this ioctl
    // Input:
    // Output:
#define IOCTL_SPIDAR_SET_DATA\
            SPIDAR_MAKE_IOCTL(SPIDAR_DEVICE_TYPE, 0)

    // TODO:  Specify the interface for this ioctl
    // Input:
    // Output:
#define IOCTL_SPIDAR_GET_DATA\
            SPIDAR_MAKE_IOCTL(SPIDAR_DEVICE_TYPE, 1)

    // TODO:  Specify the interface for this ioctl
    // Input:
    // Output:
#define IOCTL_SPIDAR_PUT_DATA\
            SPIDAR_MAKE_IOCTL(SPIDAR_DEVICE_TYPE, 2)

    // TODO:  ユーザ･アプリケーションとの通信に用いられるデータ構造をここに記述します。


#endif
