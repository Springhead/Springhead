#define SPR_DECLMEMBEROF_CDShapeDesc \
protected:\
	Spr::PHMaterial	material;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		material = ((CDShapeDesc*)ptr)->material;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((CDShapeDesc*)ptr)->material = material;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CDConvexMeshDesc \
protected:\
	std::vector<(Vec3f)>	vertices;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CDShape::SetDesc((CDShapeDesc*)(CDConvexMeshDesc*)ptr);	\
		vertices = ((CDConvexMeshDesc*)ptr)->vertices;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CDShape::GetDesc((CDShapeDesc*)(CDConvexMeshDesc*)ptr);	\
		((CDConvexMeshDesc*)ptr)->vertices = vertices;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CDSphereDesc \
protected:\
	float	radius;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CDShape::SetDesc((CDShapeDesc*)(CDSphereDesc*)ptr);	\
		radius = ((CDSphereDesc*)ptr)->radius;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CDShape::GetDesc((CDShapeDesc*)(CDSphereDesc*)ptr);	\
		((CDSphereDesc*)ptr)->radius = radius;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CDCapsuleDesc \
protected:\
	float	radius;	\
	float	length;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CDShape::SetDesc((CDShapeDesc*)(CDCapsuleDesc*)ptr);	\
		radius = ((CDCapsuleDesc*)ptr)->radius;	\
		length = ((CDCapsuleDesc*)ptr)->length;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CDShape::GetDesc((CDShapeDesc*)(CDCapsuleDesc*)ptr);	\
		((CDCapsuleDesc*)ptr)->radius = radius;	\
		((CDCapsuleDesc*)ptr)->length = length;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CDBoxDesc \
protected:\
	Vec3f	boxsize;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CDShape::SetDesc((CDShapeDesc*)(CDBoxDesc*)ptr);	\
		boxsize = ((CDBoxDesc*)ptr)->boxsize;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CDShape::GetDesc((CDShapeDesc*)(CDBoxDesc*)ptr);	\
		((CDBoxDesc*)ptr)->boxsize = boxsize;	\
		return true;	\
	}\

