//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ>
struct ObjectIfStubTemplate: public IF {
	virtual Object *  GetObj(const UTTypeInfo *  info) const {
		return ((OBJ*)(MIF*)this)->OBJ::GetObj(info);
	}
	virtual ObjectIf *  GetIfDynamic(const IfInfo *  info) const {
		return ((OBJ*)(MIF*)this)->OBJ::GetIfDynamic(info);
	}
	virtual void Print(std::ostream &  os) const {
		return ((OBJ*)(MIF*)this)->OBJ::Print(os);
	}
	virtual void PrintShort(std::ostream &  os) const {
		return ((OBJ*)(MIF*)this)->OBJ::PrintShort(os);
	}
	virtual int AddRef(){
		return ((OBJ*)(MIF*)this)->OBJ::AddRef();
	}
	virtual int DelRef(){
		return ((OBJ*)(MIF*)this)->OBJ::DelRef();
	}
	virtual int RefCount(){
		return ((OBJ*)(MIF*)this)->OBJ::RefCount();
	}
	virtual size_t NChildObject() const {
		return ((OBJ*)(MIF*)this)->OBJ::NChildObject();
	}
	virtual ObjectIf *  GetChildObject(size_t pos){
		return ((OBJ*)(MIF*)this)->OBJ::GetChildObject(pos);
	}
	virtual const ObjectIf *  GetChildObject(size_t pos) const {
		return ((OBJ*)(MIF*)this)->OBJ::GetChildObject(pos);
	}
	virtual bool AddChildObject(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::AddChildObject(o);
	}
	virtual bool DelChildObject(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::DelChildObject(o);
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->OBJ::Clear();
	}
	virtual ObjectIf *  CreateObject(const IfInfo *  info, const void *  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateObject(info, desc);
	}
	virtual const void *  GetDescAddress() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetDescAddress();
	}
	virtual bool GetDesc(void *  desc) const {
		return ((OBJ*)(MIF*)this)->OBJ::GetDesc(desc);
	}
	virtual size_t GetDescSize() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetDescSize();
	}
	virtual const void *  GetStateAddress() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetStateAddress();
	}
	virtual bool GetState(void *  state) const {
		return ((OBJ*)(MIF*)this)->OBJ::GetState(state);
	}
	virtual void SetState(const void *  state){
		return ((OBJ*)(MIF*)this)->OBJ::SetState(state);
	}
	virtual size_t GetStateSize() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetStateSize();
	}
	virtual void ConstructState(void *  m) const {
		return ((OBJ*)(MIF*)this)->OBJ::ConstructState(m);
	}
	virtual void DestructState(void *  m) const {
		return ((OBJ*)(MIF*)this)->OBJ::DestructState(m);
	}
};
struct ObjectIf;	class Object;
typedef ObjectIfStubTemplate<ObjectIf, ObjectIfBuf, Object>	ObjectIfStub;
typedef IfInitTemplate<ObjectIfStub, Object>	ObjectIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct NamedObjectIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual const char *  GetName() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetName();
	}
	virtual void SetName(const char *  n){
		return ((OBJ*)(MIF*)this)->OBJ::SetName(n);
	}
	virtual NameManagerIf *  GetNameManager(){
		return ((OBJ*)(MIF*)this)->OBJ::GetNameManager();
	}
};
struct NamedObjectIf;	class NamedObject;
typedef NamedObjectIfStubTemplate<NamedObjectIf, ObjectIfBuf, NamedObject>	NamedObjectIfStub;
typedef IfInitTemplate<NamedObjectIfStub, NamedObject>	NamedObjectIfInit;

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct SceneObjectIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual SceneIf *  GetScene(){
		return ((OBJ*)(MIF*)this)->OBJ::GetScene();
	}
};
struct SceneObjectIf;	class SceneObject;
typedef SceneObjectIfStubTemplate<SceneObjectIf, ObjectIfBuf, SceneObject>	SceneObjectIfStub;
typedef IfInitTemplate<SceneObjectIfStub, SceneObject>	SceneObjectIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct ObjectStatesIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void AllocateState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::AllocateState(o);
	}
	virtual void ReleaseState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::ReleaseState(o);
	}
	virtual size_t CalcStateSize(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::CalcStateSize(o);
	}
	virtual void SaveState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::SaveState(o);
	}
	virtual void LoadState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->OBJ::LoadState(o);
	}
};
struct ObjectStatesIf;	class ObjectStates;
typedef ObjectStatesIfStubTemplate<ObjectStatesIf, ObjectIfBuf, ObjectStates>	ObjectStatesIfStub;
typedef IfInitTemplate<ObjectStatesIfStub, ObjectStates>	ObjectStatesIfInit;

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct NameManagerIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual NamedObjectIf *  FindObject(UTString name, UTString cls = ""){
		return ((OBJ*)(MIF*)this)->OBJ::FindObject(name, cls);
	}
};
struct NameManagerIf;	class NameManager;
typedef NameManagerIfStubTemplate<NameManagerIf, ObjectIfBuf, NameManager>	NameManagerIfStub;
typedef IfInitTemplate<NameManagerIfStub, NameManager>	NameManagerIfInit;

template <class IF, class MIF, class OBJ> struct NameManagerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct SceneIfStubTemplate: public NameManagerIfStubTemplate<IF, MIF, OBJ> {
};
struct SceneIf;	class Scene;
typedef SceneIfStubTemplate<SceneIf, ObjectIfBuf, Scene>	SceneIfStub;
typedef IfInitTemplate<SceneIfStub, Scene>	SceneIfInit;

template <class IF, class MIF, class OBJ> struct NameManagerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct SdkIfStubTemplate: public NameManagerIfStubTemplate<IF, MIF, OBJ> {
};
struct SdkIf;	class Sdk;
typedef SdkIfStubTemplate<SdkIf, ObjectIfBuf, Sdk>	SdkIfStub;
typedef IfInitTemplate<SdkIfStub, Sdk>	SdkIfInit;
}	//	namespace Spr; 
