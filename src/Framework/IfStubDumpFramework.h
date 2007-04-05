//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FWObjectIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSolidIf *  GetPHSolid(){
		return ((OBJ*)(MIF*)this)->GetPHSolid();
	}
	virtual void SetPHSolid(PHSolidIf *  s){
		return ((OBJ*)(MIF*)this)->SetPHSolid(s);
	}
	virtual GRFrameIf *  GetGRFrame(){
		return ((OBJ*)(MIF*)this)->GetGRFrame();
	}
	virtual void SetGRFrame(GRFrameIf *  f){
		return ((OBJ*)(MIF*)this)->SetGRFrame(f);
	}
};
struct FWObjectIf;	class FWObject;
typedef FWObjectIfStubTemplate<FWObjectIf, ObjectIfBuf, FWObject>	FWObjectIfStub;
typedef IfInitTemplate<FWObjectIfStub, FWObject>	FWObjectIfInit;

template <class IF, class MIF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FWSceneIfStubTemplate: public SceneIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSceneIf *	 GetPHScene(){
		return ((OBJ*)(MIF*)this)->GetPHScene();
	}
	virtual void SetPHScene(PHSceneIf *  s){
		return ((OBJ*)(MIF*)this)->SetPHScene(s);
	}
	virtual GRSceneIf *	 GetGRScene(){
		return ((OBJ*)(MIF*)this)->GetGRScene();
	}
	virtual void SetGRScene(GRSceneIf *  s){
		return ((OBJ*)(MIF*)this)->SetGRScene(s);
	}
	virtual FWObjectIf *	 CreateObject(const PHSolidDesc &  soliddesc = PHSolidDesc(), const GRFrameDesc &  framedesc = GRFrameDesc()){
		return ((OBJ*)(MIF*)this)->CreateObject(soliddesc, framedesc);
	}
	virtual int NObject() const {
		return ((OBJ*)(MIF*)this)->NObject();
	}
	virtual FWObjectIf * *  GetObjects(){
		return ((OBJ*)(MIF*)this)->GetObjects();
	}
	virtual void Sync(){
		return ((OBJ*)(MIF*)this)->Sync();
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
	virtual void Draw(GRRenderIf *  grRender, bool debug = false){
		return ((OBJ*)(MIF*)this)->Draw(grRender, debug);
	}
	virtual void AddHumanInterface(HIForceDevice6D *  d){
		return ((OBJ*)(MIF*)this)->AddHumanInterface(d);
	}
};
struct FWSceneIf;	class FWScene;
typedef FWSceneIfStubTemplate<FWSceneIf, ObjectIfBuf, FWScene>	FWSceneIfStub;
typedef IfInitTemplate<FWSceneIfStub, FWScene>	FWSceneIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FWSdkIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual FWSceneIf *  CreateScene(const PHSceneDesc &  phdesc = PHSceneDesc(), const GRSceneDesc &  grdesc = GRSceneDesc()){
		return ((OBJ*)(MIF*)this)->CreateScene(phdesc, grdesc);
	}
	virtual bool LoadScene(UTString filename){
		return ((OBJ*)(MIF*)this)->LoadScene(filename);
	}
	virtual bool SaveScene(UTString filename){
		return ((OBJ*)(MIF*)this)->SaveScene(filename);
	}
	virtual int NScene() const {
		return ((OBJ*)(MIF*)this)->NScene();
	}
	virtual void SwitchScene(FWSceneIf *  scene){
		return ((OBJ*)(MIF*)this)->SwitchScene(scene);
	}
	virtual FWSceneIf *  GetScene(int index = -1){
		return ((OBJ*)(MIF*)this)->GetScene(index);
	}
	virtual void MergeScene(FWSceneIf *  scene0, FWSceneIf *  scene1){
		return ((OBJ*)(MIF*)this)->MergeScene(scene0, scene1);
	}
	virtual PHSdkIf *  GetPHSdk(){
		return ((OBJ*)(MIF*)this)->GetPHSdk();
	}
	virtual GRSdkIf *  GetGRSdk(){
		return ((OBJ*)(MIF*)this)->GetGRSdk();
	}
	virtual FISdkIf *  GetFISdk(){
		return ((OBJ*)(MIF*)this)->GetFISdk();
	}
	virtual GRRenderIf *  GetRender(){
		return ((OBJ*)(MIF*)this)->GetRender();
	}
	virtual void SetRender(GRRenderIf *  render){
		return ((OBJ*)(MIF*)this)->SetRender(render);
	}
	virtual GRDeviceIf *  GetDevice(){
		return ((OBJ*)(MIF*)this)->GetDevice();
	}
	virtual void SetDevice(GRDeviceIf *  device){
		return ((OBJ*)(MIF*)this)->SetDevice(device);
	}
	virtual bool GetDebugMode(){
		return ((OBJ*)(MIF*)this)->GetDebugMode();
	}
	virtual void SetDebugMode(bool debug = true){
		return ((OBJ*)(MIF*)this)->SetDebugMode(debug);
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
	virtual void Draw(){
		return ((OBJ*)(MIF*)this)->Draw();
	}
	virtual void Reshape(int w, int h){
		return ((OBJ*)(MIF*)this)->Reshape(w, h);
	}
};
struct FWSdkIf;	class FWSdk;
typedef FWSdkIfStubTemplate<FWSdkIf, ObjectIfBuf, FWSdk>	FWSdkIfStub;
typedef IfInitTemplate<FWSdkIfStub, FWSdk>	FWSdkIfInit;
}	//	namespace Spr; 
