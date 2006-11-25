//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class OBJ>
struct FWObjectIfStubTemplate: public NamedObjectIfStubTemplate<IF, OBJ> {
	virtual PHSolidIf *  GetPHSolid(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetPHSolid();
	}
	virtual GRFrameIf *  GetGRFrame(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetGRFrame();
	}
};
struct FWObjectIf;	class FWObject;
typedef FWObjectIfStubTemplate<FWObjectIf, FWObject>	FWObjectIfStub;
typedef IfInitTemplate<FWObjectIfStub, FWObject>	FWObjectIfInit;

template <class IF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class OBJ>
struct FWSceneIfStubTemplate: public SceneIfStubTemplate<IF, OBJ> {
	virtual PHSceneIf *  GetPHScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetPHScene();
	}
	virtual GRSceneIf *  GetGRScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetGRScene();
	}
	virtual void Sync(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Sync();
	}
	virtual void Step(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Step();
	}
	virtual void Draw(GRRenderIf *  grRender, bool debug = false){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Draw(grRender, debug);
	}
	virtual void AddHumanInterface(HIForceDevice6D *  d){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddHumanInterface(d);
	}
};
struct FWSceneIf;	class FWScene;
typedef FWSceneIfStubTemplate<FWSceneIf, FWScene>	FWSceneIfStub;
typedef IfInitTemplate<FWSceneIfStub, FWScene>	FWSceneIfInit;

template <class IF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class OBJ>
struct FWSdkIfStubTemplate: public ObjectIfStubTemplate<IF, OBJ> {
	virtual FWSceneIf *  CreateScene(const FWSceneDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene(desc);
	}
	virtual int NScene() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NScene();
	}
	virtual FWSceneIf *  GetScene(size_t i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene(i);
	}
	virtual PHSdkIf *  GetPHSdk(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetPHSdk();
	}
	virtual GRSdkIf *  GetGRSdk(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetGRSdk();
	}
	virtual FISdkIf *  GetFISdk(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetFISdk();
	}
};
struct FWSdkIf;	class FWSdk;
typedef FWSdkIfStubTemplate<FWSdkIf, FWSdk>	FWSdkIfStub;
typedef IfInitTemplate<FWSdkIfStub, FWSdk>	FWSdkIfInit;
}	//	namespace Spr; 
