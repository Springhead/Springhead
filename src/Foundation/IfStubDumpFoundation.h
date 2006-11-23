//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class OBJ>
struct ObjectIfStubTemplate{
	virtual void Print(std::ostream &  os) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Print(os);
	}
	virtual void PrintShort(std::ostream &  os) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PrintShort(os);
	}
	virtual int AddRef(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddRef();
	}
	virtual int DelRef(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DelRef();
	}
	virtual int RefCount(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::RefCount();
	}
	virtual size_t NChildObject() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NChildObject();
	}
	virtual ObjectIf *  GetChildObject(size_t pos){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetChildObject(pos);
	}
	virtual const ObjectIf *  GetChildObject(size_t pos) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetChildObject(pos);
	}
	virtual bool AddChildObject(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddChildObject(o);
	}
	virtual bool DelChildObject(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DelChildObject(o);
	}
	virtual void Clear(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Clear();
	}
	virtual ObjectIf *  CreateObject(const IfInfo *  info, const void *  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateObject(info, desc);
	}
	virtual const void *  GetDescAddress() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetDescAddress();
	}
	virtual bool GetDesc(void *  desc) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetDesc(desc);
	}
	virtual size_t GetDescSize() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetDescSize();
	}
	virtual const void *  GetStateAddress() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetStateAddress();
	}
	virtual bool GetState(void *  state) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetState(state);
	}
	virtual void SetState(const void *  state){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetState(state);
	}
	virtual size_t GetStateSize() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetStateSize();
	}
	virtual void ConstructState(void *  m) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::ConstructState(m);
	}
	virtual void DestructState(void *  m) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DestructState(m);
	}
};
struct ObjectIf;	class Object;
typedef ObjectIfStubTemplate<ObjectIf, Object>	ObjectIfStub;
typedef IfInitTemplate<ObjectIfStub, Object>	ObjectIfInit;

template <class IF, class OBJ>
struct NamedObjectIfStubTemplate: public ObjectIfStubTemplate<IF, OBJ> {
	virtual const char *  GetName() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetName();
	}
	virtual void SetName(const char *  n){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetName(n);
	}
	virtual NameManagerIf *  GetNameManager(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetNameManager();
	}
};
struct NamedObjectIf;	class NamedObject;
typedef NamedObjectIfStubTemplate<NamedObjectIf, NamedObject>	NamedObjectIfStub;
typedef IfInitTemplate<NamedObjectIfStub, NamedObject>	NamedObjectIfInit;

template <class IF, class OBJ>
struct SceneObjectIfStubTemplate: public NamedObjectIfStubTemplate<IF, OBJ> {
	virtual SceneIf *  GetScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene();
	}
};
struct SceneObjectIf;	class SceneObject;
typedef SceneObjectIfStubTemplate<SceneObjectIf, SceneObject>	SceneObjectIfStub;
typedef IfInitTemplate<SceneObjectIfStub, SceneObject>	SceneObjectIfInit;

template <class IF, class OBJ>
struct ObjectStatesIfStubTemplate: public ObjectIfStubTemplate<IF, OBJ> {
	virtual void AllocateState(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AllocateState(o);
	}
	virtual void ReleaseState(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::ReleaseState(o);
	}
	virtual size_t CalcStateSize(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CalcStateSize(o);
	}
	virtual void SaveState(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SaveState(o);
	}
	virtual void LoadState(ObjectIf *  o){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::LoadState(o);
	}
};
struct ObjectStatesIf;	class ObjectStates;
typedef ObjectStatesIfStubTemplate<ObjectStatesIf, ObjectStates>	ObjectStatesIfStub;
typedef IfInitTemplate<ObjectStatesIfStub, ObjectStates>	ObjectStatesIfInit;

template <class IF, class OBJ>
struct NameManagerIfStubTemplate: public NamedObjectIfStubTemplate<IF, OBJ> {
	virtual NamedObjectIf *  FindObject(UTString name, UTString cls = ""){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::FindObject(name, cls);
	}
};
struct NameManagerIf;	class NameManager;
typedef NameManagerIfStubTemplate<NameManagerIf, NameManager>	NameManagerIfStub;
typedef IfInitTemplate<NameManagerIfStub, NameManager>	NameManagerIfInit;

template <class IF, class OBJ>
struct SceneIfStubTemplate: public NameManagerIfStubTemplate<IF, OBJ> {
};
struct SceneIf;	class Scene;
typedef SceneIfStubTemplate<SceneIf, Scene>	SceneIfStub;
typedef IfInitTemplate<SceneIfStub, Scene>	SceneIfInit;

template <class IF, class OBJ>
struct SdkIfStubTemplate: public NameManagerIfStubTemplate<IF, OBJ> {
};
struct SdkIf;	class Sdk;
typedef SdkIfStubTemplate<SdkIf, Sdk>	SdkIfStub;
typedef IfInitTemplate<SdkIfStub, Sdk>	SdkIfInit;
}	//	namespace Spr; 
