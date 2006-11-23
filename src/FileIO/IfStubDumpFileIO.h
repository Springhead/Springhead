//	Do not edit. MakeTypeDesc.bat will update this file.

template <class IF, class OBJ>
struct FIFileIfStub: public ObjectIfStub<IF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Save(objs, fn);
	}
	virtual void RegisterGroup(const char *  gp){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::RegisterGroup(gp);
	}
};

template <class IF, class OBJ>
struct FIFileXIfStub: public FIFileIfStub<IF, OBJ> {
	virtual bool Load(ObjectIfs &  objs, const char *  fn){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Load(objs, fn);
	}
	virtual bool Save(const ObjectIfs &  objs, const char *  fn){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Save(objs, fn);
	}
};

template <class IF, class OBJ>
struct FISdkIfStub: public ObjectIfStub<IF, OBJ> {
	virtual FIFileXIf *  CreateFileX(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateFileX();
	}
	virtual void Clear(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Clear();
	}
};
