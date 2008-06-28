#define SPR_OVERRIDEMEMBERFUNCOF_CDShapeIf(base)	\
	float CalcVolume(){	return	base::CalcVolume();}	\
	Vec3f CalcCenterOfMass(){	return	base::CalcCenterOfMass();}	\
	Matrix3f CalcMomentOfInertia(){	return	base::CalcMomentOfInertia();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDConvexIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDFaceIf(base)	\
	int NIndex(){	return	base::NIndex();}	\
	int* GetIndices(){	return	base::GetIndices();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDQuadFaceIf(base)	\
	int NIndex(){	return	base::NIndex();}	\
	int* GetIndices(){	return	base::GetIndices();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDConvexMeshIf(base)	\
	Spr::CDFaceIf* GetFace(size_t i){	return	base::GetFace(i);}	\
	size_t NFace(){	return	base::NFace();}	\
	Vec3f* GetVertices(){	return	base::GetVertices();}	\
	size_t NVertex(){	return	base::NVertex();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDSphereIf(base)	\
	float GetRadius(){	return	base::GetRadius();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDCapsuleIf(base)	\
	float GetRadius(){	return	base::GetRadius();}	\
	float GetLength(){	return	base::GetLength();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDBoxIf(base)	\
	Vec3f GetBoxSize(){	return	base::GetBoxSize();}	\
	Vec3f* GetVertices(){	return	base::GetVertices();}	\
	Spr::CDFaceIf* GetFace(size_t i){	return	base::GetFace(i);}	\
	Vec3f SetBoxSize(Vec3f boxsize){	return	base::SetBoxSize(boxsize);}	\

