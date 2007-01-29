//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FWObjectIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSolidIf *  GetPHSolid(){
		return ((OBJ*)(MIF*)this)->OBJ::GetPHSolid();
	}
	virtual GRFrameIf *  GetGRFrame(){
		return ((OBJ*)(MIF*)this)->OBJ::GetGRFrame();
	}
};
struct FWObjectIf;	class FWObject;
typedef FWObjectIfStubTemplate<FWObjectIf, ObjectIfBuf, FWObject>	FWObjectIfStub;
typedef IfInitTemplate<FWObjectIfStub, FWObject>	FWObjectIfInit;

template <class IF, class MIF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FWSceneIfStubTemplate: public SceneIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSceneIf *	 GetPHScene(){
		return ((OBJ*)(MIF*)this)->OBJ::GetPHScene();
	}
	virtual void SetPHScene(PHSceneIf *  s){
		return ((OBJ*)(MIF*)this)->OBJ::SetPHScene(s);
	}
	virtual GRSceneIf *	 GetGRScene(){
		return ((OBJ*)(MIF*)this)->OBJ::GetGRScene();
	}
	virtual void SetGRScene(GRSceneIf *  s){
		return ((OBJ*)(MIF*)this)->OBJ::SetGRScene(s);
	}
	virtual void Sync(){
		return ((OBJ*)(MIF*)this)->OBJ::Sync();
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->OBJ::Step();
	}
	virtual void Draw(GRRenderIf *  grRender, bool debug = false){
		return ((OBJ*)(MIF*)this)->OBJ::Draw(grRender, debug);
	}
	virtual void AddHumanInterface(HIForceDevice6D *  d){
		return ((OBJ*)(MIF*)this)->OBJ::AddHumanInterface(d);
	}
};
struct FWSceneIf;	class FWScene;
typedef FWSceneIfStubTemplate<FWSceneIf, ObjectIfBuf, FWScene>	FWSceneIfStub;
typedef IfInitTemplate<FWSceneIfStub, FWScene>	FWSceneIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FWSdkIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual FWSceneIf *  CreateScene(const FWSceneDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateScene(desc);
	}
	virtual void LoadScene(UTString filename){
		return ((OBJ*)(MIF*)this)->OBJ::LoadScene(filename);
	}
	virtual int NScene() const {
		return ((OBJ*)(MIF*)this)->OBJ::NScene();
	}
	virtual void SwitchScene(FWSceneIf *  scene){
		return ((OBJ*)(MIF*)this)->OBJ::SwitchScene(scene);
	}
	virtual FWSceneIf *  GetScene(int index = -1){
		return ((OBJ*)(MIF*)this)->OBJ::GetScene(index);
	}
	virtual PHSdkIf *  GetPHSdk(){
		return ((OBJ*)(MIF*)this)->OBJ::GetPHSdk();
	}
	virtual GRSdkIf *  GetGRSdk(){
		return ((OBJ*)(MIF*)this)->OBJ::GetGRSdk();
	}
	virtual FISdkIf *  GetFISdk(){
		return ((OBJ*)(MIF*)this)->OBJ::GetFISdk();
	}
	virtual GRRenderIf *  GetRender(){
		return ((OBJ*)(MIF*)this)->OBJ::GetRender();
	}
	virtual void SetRender(GRRenderIf *  render){
		return ((OBJ*)(MIF*)this)->OBJ::SetRender(render);
	}
	virtual GRDeviceIf *  GetDevice(){
		return ((OBJ*)(MIF*)this)->OBJ::GetDevice();
	}
	virtual void SetDevice(GRDeviceIf *  device){
		return ((OBJ*)(MIF*)this)->OBJ::SetDevice(device);
	}
	virtual bool GetDebugMode(){
		return ((OBJ*)(MIF*)this)->OBJ::GetDebugMode();
	}
	virtual void SetDebugMode(bool debug = true){
		return ((OBJ*)(MIF*)this)->OBJ::SetDebugMode(debug);
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->OBJ::Step();
	}
	virtual void Draw(){
		return ((OBJ*)(MIF*)this)->OBJ::Draw();
	}
	virtual void Reshape(int w, int h){
		return ((OBJ*)(MIF*)this)->OBJ::Reshape(w, h);
	}
};
struct FWSdkIf;	class FWSdk;
typedef FWSdkIfStubTemplate<FWSdkIf, ObjectIfBuf, FWSdk>	FWSdkIfStub;
typedef IfInitTemplate<FWSdkIfStub, FWSdk>	FWSdkIfInit;
}	//	namespace Spr; 
