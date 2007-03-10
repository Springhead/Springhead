//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FIFileIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->Save(objs, fn);
	}
};
struct FIFileIf;	class FIFile;
typedef FIFileIfStubTemplate<FIFileIf, ObjectIfBuf, FIFile>	FIFileIfStub;
typedef IfInitTemplate<FIFileIfStub, FIFile>	FIFileIfInit;

template <class IF, class MIF, class OBJ> struct FIFileIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FIFileXIfStubTemplate: public FIFileIfStubTemplate<IF, MIF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->Save(objs, fn);
	}
};
struct FIFileXIf;	class FIFileX;
typedef FIFileXIfStubTemplate<FIFileXIf, ObjectIfBuf, FIFileX>	FIFileXIfStub;
typedef IfInitTemplate<FIFileXIfStub, FIFileX>	FIFileXIfInit;

template <class IF, class MIF, class OBJ> struct FIFileIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FIFileCOLLADAIfStubTemplate: public FIFileIfStubTemplate<IF, MIF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		return ((OBJ*)(MIF*)this)->Save(objs, fn);
	}
};
struct FIFileCOLLADAIf;	class FIFileCOLLADA;
typedef FIFileCOLLADAIfStubTemplate<FIFileCOLLADAIf, ObjectIfBuf, FIFileCOLLADA>	FIFileCOLLADAIfStub;
typedef IfInitTemplate<FIFileCOLLADAIfStub, FIFileCOLLADA>	FIFileCOLLADAIfInit;

template <class IF, class MIF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct FISdkIfStubTemplate: public SdkIfStubTemplate<IF, MIF, OBJ> {
	virtual FIFileXIf *  CreateFileX(){
		return ((OBJ*)(MIF*)this)->CreateFileX();
	}
	virtual FIFileCOLLADAIf *  CreateFileCOLLADA(){
		return ((OBJ*)(MIF*)this)->CreateFileCOLLADA();
	}
};
struct FISdkIf;	class FISdk;
typedef FISdkIfStubTemplate<FISdkIf, ObjectIfBuf, FISdk>	FISdkIfStub;
typedef IfInitTemplate<FISdkIfStub, FISdk>	FISdkIfInit;
}	//	namespace Spr; 
