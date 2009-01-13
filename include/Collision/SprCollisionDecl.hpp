#define SPR_OVERRIDEMEMBERFUNCOF_CDShapeIf(base)	\
	void SetStaticFriction(float mu0){	base::SetStaticFriction(mu0);}	\
	float GetStaticFriction(){	return	base::GetStaticFriction();}	\
	void SetDynamicFriction(float mu){	base::SetDynamicFriction(mu);}	\
	float GetDynamicFriction(){	return	base::GetDynamicFriction();}	\
	void SetElasticity(float e){	base::SetElasticity(e);}	\
	float GetElasticity(){	return	base::GetElasticity();}	\
	void SetDensity(float d){	base::SetDensity(d);}	\
	float GetDensity(){	return	base::GetDensity();}	\
	void SetVibration(float vibA, float vibB, float vibW){	base::SetVibration(vibA, vibB, vibW);}	\
	void SetVibA(float vibA){	base::SetVibA(vibA);}	\
	float GetVibA(){	return	base::GetVibA();}	\
	void SetVibB(float vibB){	base::SetVibB(vibB);}	\
	float GetVibB(){	return	base::GetVibB();}	\
	void SetVibW(float vibW){	base::SetVibW(vibW);}	\
	float GetVibW(){	return	base::GetVibW();}	\
	void SetSpringDamper(float spring, float damper){	base::SetSpringDamper(spring, damper);}	\
	float GetSpringK(){	return	base::GetSpringK();}	\
	float GetDamperD(){	return	base::GetDamperD();}	\
	float CalcVolume(){	return	base::CalcVolume();}	\
	Vec3f CalcCenterOfMass(){	return	base::CalcCenterOfMass();}	\
	Matrix3f CalcMomentOfInertia(){	return	base::CalcMomentOfInertia();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDShapePairIf(base)	\

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

#define SPR_OVERRIDEMEMBERFUNCOF_CDConvexMeshInterpolateIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDSphereIf(base)	\
	float GetRadius(){	return	base::GetRadius();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDCapsuleIf(base)	\
	float GetRadius(){	return	base::GetRadius();}	\
	float GetLength(){	return	base::GetLength();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDRoundConeIf(base)	\
	Vec2f GetRadius(){	return	base::GetRadius();}	\
	float GetLength(){	return	base::GetLength();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CDBoxIf(base)	\
	Vec3f GetBoxSize(){	return	base::GetBoxSize();}	\
	Vec3f* GetVertices(){	return	base::GetVertices();}	\
	Spr::CDFaceIf* GetFace(size_t i){	return	base::GetFace(i);}	\
	Vec3f SetBoxSize(Vec3f boxsize){	return	base::SetBoxSize(boxsize);}	\

