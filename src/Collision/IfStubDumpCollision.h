//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class OBJ>
struct CDShapeIfStubTemplate: public NamedObjectIfStubTemplate<IF, OBJ> {
};
struct CDShapeIf;	class CDShape;
typedef CDShapeIfStubTemplate<CDShapeIf, CDShape>	CDShapeIfStub;
typedef IfInitTemplate<CDShapeIfStub, CDShape>	CDShapeIfInit;

template <class IF, class OBJ> struct CDShapeIfStubTemplate;
template <class IF, class OBJ>
struct CDConvexIfStubTemplate: public CDShapeIfStubTemplate<IF, OBJ> {
};
struct CDConvexIf;	class CDConvex;
typedef CDConvexIfStubTemplate<CDConvexIf, CDConvex>	CDConvexIfStub;
typedef IfInitTemplate<CDConvexIfStub, CDConvex>	CDConvexIfInit;

template <class IF, class OBJ>
struct CDFaceIfStubTemplate: public IF {
	virtual int NIndex(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NIndex();
	}
	virtual int *  GetIndices(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetIndices();
	}
};
struct CDFaceIf;	class CDFace;
typedef CDFaceIfStubTemplate<CDFaceIf, CDFace>	CDFaceIfStub;
typedef IfInitTemplate<CDFaceIfStub, CDFace>	CDFaceIfInit;

template <class IF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class OBJ>
struct CDConvexMeshIfStubTemplate: public CDConvexIfStubTemplate<IF, OBJ> {
	virtual CDFaceIf *  GetFace(size_t i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetFace(i);
	}
	virtual size_t NFace(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NFace();
	}
	virtual Vec3f *  GetVertices(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetVertices();
	}
	virtual size_t NVertex(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NVertex();
	}
};
struct CDConvexMeshIf;	class CDConvexMesh;
typedef CDConvexMeshIfStubTemplate<CDConvexMeshIf, CDConvexMesh>	CDConvexMeshIfStub;
typedef IfInitTemplate<CDConvexMeshIfStub, CDConvexMesh>	CDConvexMeshIfInit;

template <class IF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class OBJ>
struct CDSphereIfStubTemplate: public CDConvexIfStubTemplate<IF, OBJ> {
	virtual float GetRadius(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetRadius();
	}
};
struct CDSphereIf;	class CDSphere;
typedef CDSphereIfStubTemplate<CDSphereIf, CDSphere>	CDSphereIfStub;
typedef IfInitTemplate<CDSphereIfStub, CDSphere>	CDSphereIfInit;

template <class IF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class OBJ>
struct CDCapsuleIfStubTemplate: public CDConvexIfStubTemplate<IF, OBJ> {
	virtual float GetRadius(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetRadius();
	}
	virtual float GetLength(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetLength();
	}
};
struct CDCapsuleIf;	class CDCapsule;
typedef CDCapsuleIfStubTemplate<CDCapsuleIf, CDCapsule>	CDCapsuleIfStub;
typedef IfInitTemplate<CDCapsuleIfStub, CDCapsule>	CDCapsuleIfInit;

template <class IF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class OBJ>
struct CDBoxIfStubTemplate: public CDConvexIfStubTemplate<IF, OBJ> {
	virtual Vec3f GetBoxSize(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetBoxSize();
	}
	virtual Vec3f *  GetVertices(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetVertices();
	}
	virtual CDFaceIf *  GetFace(size_t i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetFace(i);
	}
};
struct CDBoxIf;	class CDBox;
typedef CDBoxIfStubTemplate<CDBoxIf, CDBox>	CDBoxIfStub;
typedef IfInitTemplate<CDBoxIfStub, CDBox>	CDBoxIfInit;
}	//	namespace Spr; 
