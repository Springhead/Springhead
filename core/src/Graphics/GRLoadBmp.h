#ifndef GRBMPLOADER_H
#define GRBMPLOADER_H

namespace Spr{;

int SPR_CDECL LoadBmpCreate(const char* fn);
int SPR_CDECL LoadBmpRelease(int hBmp);
int SPR_CDECL LoadBmpGetBmp(int hBmp, char* bmp);
int SPR_CDECL LoadBmpGetWidth(int hBmp);
int SPR_CDECL LoadBmpGetHeight(int hBmp);
int SPR_CDECL LoadBmpGetBitPerPixel(int hBmp);
int SPR_CDECL LoadBmpGetBytePerPixel(int hBmp);
int SPR_CDECL LoadBmpGetSize(int hBmp);
int SPR_CDECL LoadBmpHasAlpha(int hBmp);
int SPR_CDECL LoadBmpIsGrayscale(int hBmp);


};

#endif
