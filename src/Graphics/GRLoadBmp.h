#ifndef GRBMPLOADER_H
#define GRBMPLOADER_H

namespace Spr{;

int __cdecl LoadBmpCreate(const char* fn);
int __cdecl LoadBmpRelease(int hBmp);
int __cdecl LoadBmpGetBmp(int hBmp, char* bmp);
int __cdecl LoadBmpGetWidth(int hBmp);
int __cdecl LoadBmpGetHeight(int hBmp);
int __cdecl LoadBmpGetBytePerPixel(int hBmp);
int __cdecl LoadBmpGetSize(int hBmp);


};

#endif
