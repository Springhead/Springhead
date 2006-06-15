/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.75 */
/* at Tue May 25 11:09:32 1999
 */
/* Compiler settings for TkInput.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"
#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __TkInput_h__
#define __TkInput_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ITk3dInput_FWD_DEFINED__
#define __ITk3dInput_FWD_DEFINED__
typedef interface ITk3dInput ITk3dInput;
#endif 	/* __ITk3dInput_FWD_DEFINED__ */


#ifndef __Tk3dInput_FWD_DEFINED__
#define __Tk3dInput_FWD_DEFINED__

#ifdef __cplusplus
typedef class Tk3dInput Tk3dInput;
#else
typedef struct Tk3dInput Tk3dInput;
#endif /* __cplusplus */

#endif 	/* __Tk3dInput_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/****************************************
 * Generated header for interface: __MIDL_itf_TkInput_0000
 * at Tue May 25 11:09:32 1999
 * using MIDL 3.01.75
 ****************************************/
/* [local] */ 


typedef long RETINFO;

typedef long __RPC_FAR *PRETINFO;

typedef struct  TTK3DADSTDDATA
    {
    double Offset[ 7 ];
    double Gain[ 7 ];
    double Non_Move[ 3 ];
    double Ht_Z;
    double Ipsiron1;
    double Comp_Lev;
    }	TK3DADSTDDATA;

typedef TK3DADSTDDATA __RPC_FAR *PTK3DADSTDDATA;

typedef TK3DADSTDDATA __RPC_FAR *PTK3DNOWADSTDDATA;

typedef struct  TTK3DISTATE
    {
    double Alfa;
    double Beta;
    double Gamma;
    double Alfa_North;
    double AccX;
    double AccY;
    double VeloX;
    double VeloY;
    double LocaX;
    double LocaY;
    long Button;
    long mlX;
    long mlY;
    long ubFlag;
    long wRoll;
    long wPitch;
    long wTurn;
    long wLeanFB;
    long wLeanRL;
    long wMoveFB;
    long wMoveRL;
    long uwButton;
    long uwLED;
    }	TK3DISTATE;

typedef TK3DISTATE __RPC_FAR *PTK3DISTATE;



extern RPC_IF_HANDLE __MIDL_itf_TkInput_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_TkInput_0000_v0_0_s_ifspec;

#ifndef __ITk3dInput_INTERFACE_DEFINED__
#define __ITk3dInput_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: ITk3dInput
 * at Tue May 25 11:09:32 1999
 * using MIDL 3.01.75
 ****************************************/
/* [object][unique][helpstring][uuid] */ 



EXTERN_C const IID IID_ITk3dInput;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    interface DECLSPEC_UUID("9E6CA5D0-B131-11D2-ACE7-004854002478")
    ITk3dInput : public IUnknown
    {
    public:
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CreateDevice( 
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE SetAdStdData( 
            /* [in] */ short DeviceNumber,
            /* [in] */ PTK3DADSTDDATA pTk3dAdStdData,
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetAdStdData( 
            /* [in] */ short DeviceNumber,
            /* [out][in] */ PTK3DNOWADSTDDATA pTk3dNowAdStdData,
            /* [out] */ BOOL __RPC_FAR *AdStdData,
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE Poll( 
            /* [in] */ short DeviceNumber,
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE GetDeviceData( 
            /* [in] */ short DeviceNumber,
            /* [out][in] */ PTK3DISTATE pTk3dIState,
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StartMouseData( 
            /* [in] */ long count_x,
            /* [in] */ long count_y,
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE StopMouseData( void) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE CloseDevice( 
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
        virtual /* [helpstring] */ HRESULT STDMETHODCALLTYPE InitDevice( 
            /* [in] */ short DeviceNumber,
            /* [retval][out] */ PRETINFO pRetInfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITk3dInputVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ITk3dInput __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ITk3dInput __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDevice )( 
            ITk3dInput __RPC_FAR * This,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetAdStdData )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ short DeviceNumber,
            /* [in] */ PTK3DADSTDDATA pTk3dAdStdData,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAdStdData )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ short DeviceNumber,
            /* [out][in] */ PTK3DNOWADSTDDATA pTk3dNowAdStdData,
            /* [out] */ BOOL __RPC_FAR *AdStdData,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Poll )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ short DeviceNumber,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetDeviceData )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ short DeviceNumber,
            /* [out][in] */ PTK3DISTATE pTk3dIState,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StartMouseData )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ long count_x,
            /* [in] */ long count_y,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *StopMouseData )( 
            ITk3dInput __RPC_FAR * This);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CloseDevice )( 
            ITk3dInput __RPC_FAR * This,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        /* [helpstring] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *InitDevice )( 
            ITk3dInput __RPC_FAR * This,
            /* [in] */ short DeviceNumber,
            /* [retval][out] */ PRETINFO pRetInfo);
        
        END_INTERFACE
    } ITk3dInputVtbl;

    interface ITk3dInput
    {
        CONST_VTBL struct ITk3dInputVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITk3dInput_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITk3dInput_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITk3dInput_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITk3dInput_CreateDevice(This,pRetInfo)	\
    (This)->lpVtbl -> CreateDevice(This,pRetInfo)

#define ITk3dInput_SetAdStdData(This,DeviceNumber,pTk3dAdStdData,pRetInfo)	\
    (This)->lpVtbl -> SetAdStdData(This,DeviceNumber,pTk3dAdStdData,pRetInfo)

#define ITk3dInput_GetAdStdData(This,DeviceNumber,pTk3dNowAdStdData,AdStdData,pRetInfo)	\
    (This)->lpVtbl -> GetAdStdData(This,DeviceNumber,pTk3dNowAdStdData,AdStdData,pRetInfo)

#define ITk3dInput_Poll(This,DeviceNumber,pRetInfo)	\
    (This)->lpVtbl -> Poll(This,DeviceNumber,pRetInfo)

#define ITk3dInput_GetDeviceData(This,DeviceNumber,pTk3dIState,pRetInfo)	\
    (This)->lpVtbl -> GetDeviceData(This,DeviceNumber,pTk3dIState,pRetInfo)

#define ITk3dInput_StartMouseData(This,count_x,count_y,pRetInfo)	\
    (This)->lpVtbl -> StartMouseData(This,count_x,count_y,pRetInfo)

#define ITk3dInput_StopMouseData(This)	\
    (This)->lpVtbl -> StopMouseData(This)

#define ITk3dInput_CloseDevice(This,pRetInfo)	\
    (This)->lpVtbl -> CloseDevice(This,pRetInfo)

#define ITk3dInput_InitDevice(This,DeviceNumber,pRetInfo)	\
    (This)->lpVtbl -> InitDevice(This,DeviceNumber,pRetInfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_CreateDevice_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_CreateDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_SetAdStdData_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [in] */ short DeviceNumber,
    /* [in] */ PTK3DADSTDDATA pTk3dAdStdData,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_SetAdStdData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_GetAdStdData_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [in] */ short DeviceNumber,
    /* [out][in] */ PTK3DNOWADSTDDATA pTk3dNowAdStdData,
    /* [out] */ BOOL __RPC_FAR *AdStdData,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_GetAdStdData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_Poll_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [in] */ short DeviceNumber,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_Poll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_GetDeviceData_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [in] */ short DeviceNumber,
    /* [out][in] */ PTK3DISTATE pTk3dIState,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_GetDeviceData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_StartMouseData_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [in] */ long count_x,
    /* [in] */ long count_y,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_StartMouseData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_StopMouseData_Proxy( 
    ITk3dInput __RPC_FAR * This);


void __RPC_STUB ITk3dInput_StopMouseData_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_CloseDevice_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_CloseDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring] */ HRESULT STDMETHODCALLTYPE ITk3dInput_InitDevice_Proxy( 
    ITk3dInput __RPC_FAR * This,
    /* [in] */ short DeviceNumber,
    /* [retval][out] */ PRETINFO pRetInfo);


void __RPC_STUB ITk3dInput_InitDevice_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITk3dInput_INTERFACE_DEFINED__ */



#ifndef __TKINPUTLib_LIBRARY_DEFINED__
#define __TKINPUTLib_LIBRARY_DEFINED__

/****************************************
 * Generated header for library: TKINPUTLib
 * at Tue May 25 11:09:32 1999
 * using MIDL 3.01.75
 ****************************************/
/* [helpstring][version][uuid] */ 



EXTERN_C const IID LIBID_TKINPUTLib;

#ifdef __cplusplus
EXTERN_C const CLSID CLSID_Tk3dInput;

class DECLSPEC_UUID("59F99DD1-ABBF-11D2-ACE7-004854002478")
Tk3dInput;
#endif
#endif /* __TKINPUTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
