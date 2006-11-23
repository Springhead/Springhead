//	Do not edit. MakeTypeDesc.bat will update this file.

template <class IF, class OBJ>
struct CDShapeIfStub: public NamedObjectIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct CDConvexIfStub: public CDShapeIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct CDFaceIfStub{
	virtual int NIndex(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NIndex();
	}
	virtual int *  GetIndices(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetIndices();
	}
};

template <class IF, class OBJ>
struct CDConvexMeshIfStub: public CDConvexIfStub<IF, OBJ> {
	virtual CDFaceIf *  GetFace(size_t i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetFace(i);
	}
	virtual size_t NFace(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NFace();
	}
	virtual Vec3f *  GetVertices(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetVertices();
	}
	virtual size_t NVertex(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NVertex();
	}
};

template <class IF, class OBJ>
struct CDSphereIfStub: public CDConvexIfStub<IF, OBJ> {
	virtual float GetRadius(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetRadius();
	}
};

template <class IF, class OBJ>
struct CDCapsuleIfStub: public CDConvexIfStub<IF, OBJ> {
	virtual float GetRadius(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetRadius();
	}
	virtual float GetLength(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetLength();
	}
};

template <class IF, class OBJ>
struct CDBoxIfStub: public CDConvexIfStub<IF, OBJ> {
	virtual Vec3f GetBoxSize(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetBoxSize();
	}
	virtual Vec3f *  GetVertices(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetVertices();
	}
	virtual CDFaceIf *  GetFace(size_t i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetFace(i);
	}
};
