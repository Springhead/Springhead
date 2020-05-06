//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRAnimationMeshIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetMotion(const std::string & name){
		return ((OBJ*)(MIF*)this)->SetMotion(name);
	}
	virtual void SetTime(double time){
		return ((OBJ*)(MIF*)this)->SetTime(time);
	}
	virtual void SetBlend(const std::vector <GRAnimationMeshBlendData> & data){
		return ((OBJ*)(MIF*)this)->SetBlend(data);
	}
	virtual Posed GetBoneKeyframePose(const std::string & name){
		return ((OBJ*)(MIF*)this)->GetBoneKeyframePose(name);
	}
	virtual void OverrideBoneOrientation(const std::string & name, const Quaterniond & orientation, double weight){
		return ((OBJ*)(MIF*)this)->OverrideBoneOrientation(name, orientation, weight);
	}
	virtual void OverrideBonePose(const std::string & name, const Posed & pose, double weight, bool posRelative = false){
		return ((OBJ*)(MIF*)this)->OverrideBonePose(name, pose, weight, posRelative);
	}
	virtual void SetAllBonesOverrideWeight(double weight){
		return ((OBJ*)(MIF*)this)->SetAllBonesOverrideWeight(weight);
	}
	virtual void AddDrawSubsetListener(GRAnimationMeshDrawSubsetListenerFunc beforeFunc, GRAnimationMeshDrawSubsetListenerFunc afterFunc, void * ptr = NULL){
		return ((OBJ*)(MIF*)this)->AddDrawSubsetListener(beforeFunc, afterFunc, ptr);
	}
	virtual void SetEffect(LPD3DXEFFECT effect, int matrixPaletteSize){
		return ((OBJ*)(MIF*)this)->SetEffect(effect, matrixPaletteSize);
	}
};
struct GRAnimationMeshIf;	class GRAnimationMesh;
typedef GRAnimationMeshIfStubTemplate<GRAnimationMeshIf, ObjectIfBuf, GRAnimationMesh>	GRAnimationMeshIfStub;
typedef IfInitTemplate<GRAnimationMeshIfStub, GRAnimationMesh>	GRAnimationMeshIfInit;

template <class IF, class MIF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceD3DIfStubTemplate: public GRDeviceIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetConfirmDeviceFunc(GRDeviceD3DConfirmDeviceFunc func){
		return ((OBJ*)(MIF*)this)->SetConfirmDeviceFunc(func);
	}
	virtual void AddLostDeviceListener(GRDeviceD3DListenerFunc func){
		return ((OBJ*)(MIF*)this)->AddLostDeviceListener(func);
	}
	virtual void AddResetDeviceListener(GRDeviceD3DListenerFunc func){
		return ((OBJ*)(MIF*)this)->AddResetDeviceListener(func);
	}
	virtual void ToggleFullScreen(){
		return ((OBJ*)(MIF*)this)->ToggleFullScreen();
	}
	virtual bool IsFullScreen(){
		return ((OBJ*)(MIF*)this)->IsFullScreen();
	}
};
struct GRDeviceD3DIf;	class GRDeviceD3D;
typedef GRDeviceD3DIfStubTemplate<GRDeviceD3DIf, ObjectIfBuf, GRDeviceD3D>	GRDeviceD3DIfStub;
typedef IfInitTemplate<GRDeviceD3DIfStub, GRDeviceD3D>	GRDeviceD3DIfInit;

template <class IF, class MIF, class OBJ> struct GRDebugRenderIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDebugRenderD3DIfStubTemplate: public GRDebugRenderIfStubTemplate<IF, MIF, OBJ> {
};
struct GRDebugRenderD3DIf;	class GRDebugRenderD3D;
typedef GRDebugRenderD3DIfStubTemplate<GRDebugRenderD3DIf, ObjectIfBuf, GRDebugRenderD3D>	GRDebugRenderD3DIfStub;
typedef IfInitTemplate<GRDebugRenderD3DIfStub, GRDebugRenderD3D>	GRDebugRenderD3DIfInit;

template <class IF, class MIF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSdkD3DIfStubTemplate: public SdkIfStubTemplate<IF, MIF, OBJ> {
	virtual GRDeviceD3DIf * CreateDeviceD3D(){
		return ((OBJ*)(MIF*)this)->CreateDeviceD3D();
	}
	virtual GRDebugRenderD3DIf * CreateDebugRenderD3D(){
		return ((OBJ*)(MIF*)this)->CreateDebugRenderD3D();
	}
	virtual GRAnimationMeshIf * CreateAnimationMesh(const GRAnimationMeshDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateAnimationMesh(desc);
	}
};
struct GRSdkD3DIf;	class GRSdkD3D;
typedef GRSdkD3DIfStubTemplate<GRSdkD3DIf, ObjectIfBuf, GRSdkD3D>	GRSdkD3DIfStub;
typedef IfInitTemplate<GRSdkD3DIfStub, GRSdkD3D>	GRSdkD3DIfInit;
}	//	namespace Spr; 
