/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 3.01.75 */
/* at Tue May 25 11:09:32 1999
 */
/* Compiler settings for TkInput.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID IID_ITk3dInput = {0x9E6CA5D0,0xB131,0x11D2,{0xAC,0xE7,0x00,0x48,0x54,0x00,0x24,0x78}};


const IID LIBID_TKINPUTLib = {0x9E6CA5C1,0xB131,0x11D2,{0xAC,0xE7,0x00,0x48,0x54,0x00,0x24,0x78}};


const CLSID CLSID_Tk3dInput = {0x59F99DD1,0xABBF,0x11D2,{0xAC,0xE7,0x00,0x48,0x54,0x00,0x24,0x78}};


#ifdef __cplusplus
}
#endif

