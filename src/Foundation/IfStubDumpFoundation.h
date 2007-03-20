//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ>
struct ObjectIfStubTemplate: public IF {
	virtual Object *  GetObj(const UTTypeInfo *  info) const {
		return ((OBJ*)(MIF*)this)->GetObj(info);
	}
	virtual ObjectIf *  GetIfDynamic(const IfInfo *  info) const {
		return ((OBJ*)(MIF*)this)->GetIfDynamic(info);
	}
	virtual void Print(std::ostream &  os) const {
		return ((OBJ*)(MIF*)this)->Print(os);
	}
	virtual void PrintShort(std::ostream &  os) const {
		return ((OBJ*)(MIF*)this)->PrintShort(os);
	}
	virtual int AddRef(){
		return ((OBJ*)(MIF*)this)->AddRef();
	}
	virtual int DelRef(){
		return ((OBJ*)(MIF*)this)->DelRef();
	}
	virtual int RefCount(){
		return ((OBJ*)(MIF*)this)->RefCount();
	}
	virtual size_t NChildObject() const {
		return ((OBJ*)(MIF*)this)->NChildObject();
	}
	virtual ObjectIf *  GetChildObject(size_t pos){
		return ((OBJ*)(MIF*)this)->GetChildObject(pos);
	}
	virtual const ObjectIf *  GetChildObject(size_t pos) const {
		return ((OBJ*)(MIF*)this)->GetChildObject(pos);
	}
	virtual bool AddChildObject(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->AddChildObject(o);
	}
	virtual bool DelChildObject(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->DelChildObject(o);
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->Clear();
	}
	virtual ObjectIf *  CreateObject(const IfInfo *  info, const void *  desc){
		return ((OBJ*)(MIF*)this)->CreateObject(info, desc);
	}
	virtual const void *  GetDescAddress() const {
		return ((OBJ*)(MIF*)this)->GetDescAddress();
	}
	virtual bool GetDesc(void *  desc) const {
		return ((OBJ*)(MIF*)this)->GetDesc(desc);
	}
	virtual void SetDesc(const void *  desc){
		return ((OBJ*)(MIF*)this)->SetDesc(desc);
	}
	virtual size_t GetDescSize() const {
		return ((OBJ*)(MIF*)this)->GetDescSize();
	}
	virtual const void *  GetStateAddress() const {
		return ((OBJ*)(MIF*)this)->GetStateAddress();
	}
	virtual bool GetState(void *  state) const {
		return ((OBJ*)(MIF*)this)->GetState(state);
	}
	virtual void SetState(const void *  state){
		return ((OBJ*)(MIF*)this)->SetState(state);
	}
	virtual size_t GetStateSize() const {
		return ((OBJ*)(MIF*)this)->GetStateSize();
	}
	virtual void ConstructState(void *  m) const {
		return ((OBJ*)(MIF*)this)->ConstructState(m);
	}
	virtual void DestructState(void *  m) const {
		return ((OBJ*)(MIF*)this)->DestructState(m);
	}
};
struct ObjectIf;	class Object;
typedef ObjectIfStubTemplate<ObjectIf, ObjectIfBuf, Object>	ObjectIfStub;
typedef IfInitTemplate<ObjectIfStub, Object>	ObjectIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct NamedObjectIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual const char *  GetName() const {
		return ((OBJ*)(MIF*)this)->GetName();
	}
	virtual void SetName(const char *  n){
		return ((OBJ*)(MIF*)this)->SetName(n);
	}
	virtual NameManagerIf *  GetNameManager(){
		return ((OBJ*)(MIF*)this)->GetNameManager();
	}
};
struct NamedObjectIf;	class NamedObject;
typedef NamedObjectIfStubTemplate<NamedObjectIf, ObjectIfBuf, NamedObject>	NamedObjectIfStub;
typedef IfInitTemplate<NamedObjectIfStub, NamedObject>	NamedObjectIfInit;

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct SceneObjectIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual SceneIf *  GetScene(){
		return ((OBJ*)(MIF*)this)->GetScene();
	}
};
struct SceneObjectIf;	class SceneObject;
typedef SceneObjectIfStubTemplate<SceneObjectIf, ObjectIfBuf, SceneObject>	SceneObjectIfStub;
typedef IfInitTemplate<SceneObjectIfStub, SceneObject>	SceneObjectIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct ObjectStatesIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void AllocateState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->AllocateState(o);
	}
	virtual void ReleaseState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->ReleaseState(o);
	}
	virtual size_t CalcStateSize(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->CalcStateSize(o);
	}
	virtual void SaveState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->SaveState(o);
	}
	virtual void LoadState(ObjectIf *  o){
		return ((OBJ*)(MIF*)this)->LoadState(o);
	}
};
struct ObjectStatesIf;	class ObjectStates;
typedef ObjectStatesIfStubTemplate<ObjectStatesIf, ObjectIfBuf, ObjectStates>	ObjectStatesIfStub;
typedef IfInitTemplate<ObjectStatesIfStub, ObjectStates>	ObjectStatesIfInit;

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct NameManagerIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual NamedObjectIf *  FindObject(UTString name, UTString cls = ""){
		return ((OBJ*)(MIF*)this)->FindObject(name, cls);
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
