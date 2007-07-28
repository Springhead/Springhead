//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceD3DIfStubTemplate: public GRDeviceIfStubTemplate<IF, MIF, OBJ> {
};
struct GRDeviceD3DIf;	class GRDeviceD3D;
typedef GRDeviceD3DIfStubTemplate<GRDeviceD3DIf, ObjectIfBuf, GRDeviceD3D>	GRDeviceD3DIfStub;
typedef IfInitTemplate<GRDeviceD3DIfStub, GRDeviceD3D>	GRDeviceD3DIfInit;

template <class IF, class MIF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSdkD3DIfStubTemplate: public SdkIfStubTemplate<IF, MIF, OBJ> {
	virtual GRDeviceD3DIf *  CreateDeviceD3D(){
		return ((OBJ*)(MIF*)this)->CreateDeviceD3D();
	}
};
struct GRSdkD3DIf;	class GRSdkD3D;
typedef GRSdkD3DIfStubTemplate<GRSdkD3DIf, ObjectIfBuf, GRSdkD3D>	GRSdkD3DIfStub;
typedef IfInitTemplate<GRSdkD3DIfStub, GRSdkD3D>	GRSdkD3DIfInit;
}	//	namespace Spr; 
