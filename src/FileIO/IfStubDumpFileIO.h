//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class OBJ>
struct FIFileIfStubTemplate: public ObjectIfStubTemplate<IF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Save(objs, fn);
	}
	virtual void RegisterGroup(const char *  gp){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::RegisterGroup(gp);
	}
};
struct FIFileIf;	class FIFile;
typedef FIFileIfStubTemplate<FIFileIf, FIFile>	FIFileIfStub;
typedef IfInitTemplate<FIFileIfStub, FIFile>	FIFileIfInit;

template <class IF, class OBJ> struct FIFileIfStubTemplate;
template <class IF, class OBJ>
struct FIFileXIfStubTemplate: public FIFileIfStubTemplate<IF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Save(objs, fn);
	}
};
struct FIFileXIf;	class FIFileX;
typedef FIFileXIfStubTemplate<FIFileXIf, FIFileX>	FIFileXIfStub;
typedef IfInitTemplate<FIFileXIfStub, FIFileX>	FIFileXIfInit;

template <class IF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class OBJ>
struct FISdkIfStubTemplate: public ObjectIfStubTemplate<IF, OBJ> {
	virtual FIFileXIf *  CreateFileX(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateFileX();
	}
	virtual void Clear(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Clear();
	}
};
struct FISdkIf;	class FISdk;
typedef FISdkIfStubTemplate<FISdkIf, FISdk>	FISdkIfStub;
typedef IfInitTemplate<FISdkIfStub, FISdk>	FISdkIfInit;
}	//	namespace Spr; 
