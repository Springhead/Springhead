//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CDShapeIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual float CalcVolume(){
		return ((OBJ*)(MIF*)this)->CalcVolume();
	}
	virtual Matrix3f MomentOfInertia(){
		return ((OBJ*)(MIF*)this)->MomentOfInertia();
	}
};
struct CDShapeIf;	class CDShape;
typedef CDShapeIfStubTemplate<CDShapeIf, ObjectIfBuf, CDShape>	CDShapeIfStub;
typedef IfInitTemplate<CDShapeIfStub, CDShape>	CDShapeIfInit;

template <class IF, class MIF, class OBJ> struct CDShapeIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CDConvexIfStubTemplate: public CDShapeIfStubTemplate<IF, MIF, OBJ> {
};
struct CDConvexIf;	class CDConvex;
typedef CDConvexIfStubTemplate<CDConvexIf, ObjectIfBuf, CDConvex>	CDConvexIfStub;
typedef IfInitTemplate<CDConvexIfStub, CDConvex>	CDConvexIfInit;

template <class IF, class MIF, class OBJ>
struct CDFaceIfStubTemplate: public IF {
	virtual int NIndex(){
		return ((OBJ*)(MIF*)this)->NIndex();
	}
	virtual int * GetIndices(){
		return ((OBJ*)(MIF*)this)->GetIndices();
	}
};
struct CDFaceIf;	class CDFace;
typedef CDFaceIfStubTemplate<CDFaceIf, ObjectIfBuf, CDFace>	CDFaceIfStub;
typedef IfInitTemplate<CDFaceIfStub, CDFace>	CDFaceIfInit;

template <class IF, class MIF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CDConvexMeshIfStubTemplate: public CDConvexIfStubTemplate<IF, MIF, OBJ> {
	virtual CDFaceIf * GetFace(size_t i){
		return ((OBJ*)(MIF*)this)->GetFace(i);
	}
	virtual size_t NFace(){
		return ((OBJ*)(MIF*)this)->NFace();
	}
	virtual Vec3f * GetVertices(){
		return ((OBJ*)(MIF*)this)->GetVertices();
	}
	virtual size_t NVertex(){
		return ((OBJ*)(MIF*)this)->NVertex();
	}
};
struct CDConvexMeshIf;	class CDConvexMesh;
typedef CDConvexMeshIfStubTemplate<CDConvexMeshIf, ObjectIfBuf, CDConvexMesh>	CDConvexMeshIfStub;
typedef IfInitTemplate<CDConvexMeshIfStub, CDConvexMesh>	CDConvexMeshIfInit;

template <class IF, class MIF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CDSphereIfStubTemplate: public CDConvexIfStubTemplate<IF, MIF, OBJ> {
	virtual float GetRadius(){
		return ((OBJ*)(MIF*)this)->GetRadius();
	}
};
struct CDSphereIf;	class CDSphere;
typedef CDSphereIfStubTemplate<CDSphereIf, ObjectIfBuf, CDSphere>	CDSphereIfStub;
typedef IfInitTemplate<CDSphereIfStub, CDSphere>	CDSphereIfInit;

template <class IF, class MIF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CDCapsuleIfStubTemplate: public CDConvexIfStubTemplate<IF, MIF, OBJ> {
	virtual float GetRadius(){
		return ((OBJ*)(MIF*)this)->GetRadius();
	}
	virtual float GetLength(){
		return ((OBJ*)(MIF*)this)->GetLength();
	}
};
struct CDCapsuleIf;	class CDCapsule;
typedef CDCapsuleIfStubTemplate<CDCapsuleIf, ObjectIfBuf, CDCapsule>	CDCapsuleIfStub;
typedef IfInitTemplate<CDCapsuleIfStub, CDCapsule>	CDCapsuleIfInit;

template <class IF, class MIF, class OBJ> struct CDConvexIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CDBoxIfStubTemplate: public CDConvexIfStubTemplate<IF, MIF, OBJ> {
	virtual Vec3f GetBoxSize(){
		return ((OBJ*)(MIF*)this)->GetBoxSize();
	}
	virtual Vec3f * GetVertices(){
		return ((OBJ*)(MIF*)this)->GetVertices();
	}
	virtual CDFaceIf * GetFace(size_t i){
		return ((OBJ*)(MIF*)this)->GetFace(i);
	}
};
struct CDBoxIf;	class CDBox;
typedef CDBoxIfStubTemplate<CDBoxIf, ObjectIfBuf, CDBox>	CDBoxIfStub;
typedef IfInitTemplate<CDBoxIfStub, CDBox>	CDBoxIfInit;
}	//	namespace Spr; 
