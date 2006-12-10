//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FIFileIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->OBJ::Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->OBJ::Save(objs, fn);
	}
	virtual void RegisterGroup(const char *  gp){
		return ((OBJ*)(MIF*)this)->OBJ::RegisterGroup(gp);
	}
};
struct FIFileIf;	class FIFile;
typedef FIFileIfStubTemplate<FIFileIf, ObjectIfBuf, FIFile>	FIFileIfStub;
typedef IfInitTemplate<FIFileIfStub, FIFile>	FIFileIfInit;

template <class IF, class MIF, class OBJ> struct FIFileIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FIFileXIfStubTemplate: public FIFileIfStubTemplate<IF, MIF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->OBJ::Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->OBJ::Save(objs, fn);
	}
};
struct FIFileXIf;	class FIFileX;
typedef FIFileXIfStubTemplate<FIFileXIf, ObjectIfBuf, FIFileX>	FIFileXIfStub;
typedef IfInitTemplate<FIFileXIfStub, FIFileX>	FIFileXIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FISdkIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual FIFileXIf *  CreateFileX(){
		return ((OBJ*)(MIF*)this)->OBJ::CreateFileX();
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->OBJ::Clear();
	}
};
struct FISdkIf;	class FISdk;
typedef FISdkIfStubTemplate<FISdkIf, ObjectIfBuf, FISdk>	FISdkIfStub;
typedef IfInitTemplate<FISdkIfStub, FISdk>	FISdkIfInit;
}	//	namespace Spr; 
