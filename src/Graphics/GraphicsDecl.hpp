#define SPR_DECLMEMBEROF_GRBlendMeshDesc \
protected:\
	std::vector< Vec3f >	positions;	\
	std::vector< Vec3f >	normals;	\
	std::vector< Vec4f >	colors;	\
	std::vector< Vec2f >	texCoords;	\
	std::vector< size_t >	faces;	\
	std::vector< Vec4f >	blends;	\
	std::vector< Vec4f >	matrixIndices;	\
	std::vector< Vec4f >	numMatrix;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		positions = ((GRBlendMeshDesc*)ptr)->positions;	\
		normals = ((GRBlendMeshDesc*)ptr)->normals;	\
		colors = ((GRBlendMeshDesc*)ptr)->colors;	\
		texCoords = ((GRBlendMeshDesc*)ptr)->texCoords;	\
		faces = ((GRBlendMeshDesc*)ptr)->faces;	\
		blends = ((GRBlendMeshDesc*)ptr)->blends;	\
		matrixIndices = ((GRBlendMeshDesc*)ptr)->matrixIndices;	\
		numMatrix = ((GRBlendMeshDesc*)ptr)->numMatrix;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((GRBlendMeshDesc*)ptr)->positions = positions;	\
		((GRBlendMeshDesc*)ptr)->normals = normals;	\
		((GRBlendMeshDesc*)ptr)->colors = colors;	\
		((GRBlendMeshDesc*)ptr)->texCoords = texCoords;	\
		((GRBlendMeshDesc*)ptr)->faces = faces;	\
		((GRBlendMeshDesc*)ptr)->blends = blends;	\
		((GRBlendMeshDesc*)ptr)->matrixIndices = matrixIndices;	\
		((GRBlendMeshDesc*)ptr)->numMatrix = numMatrix;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRVisualDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRFrameDesc \
protected:\
	Affinef	transform;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRFrameDesc*)ptr);	\
		transform = ((GRFrameDesc*)ptr)->transform;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRFrameDesc*)ptr);	\
		((GRFrameDesc*)ptr)->transform = transform;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRDummyFrameDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRDummyFrameDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRDummyFrameDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRAnimationDesc \
protected:\
	std::vector< Spr::GRAnimationKey >	keys;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		keys = ((GRAnimationDesc*)ptr)->keys;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((GRAnimationDesc*)ptr)->keys = keys;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRAnimationSetDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRAnimationControllerDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRSkinWeightDesc \
protected:\
	Affinef	offset;	\
	std::vector< unsigned int >	indices;	\
	std::vector< float >	weights;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		offset = ((GRSkinWeightDesc*)ptr)->offset;	\
		indices = ((GRSkinWeightDesc*)ptr)->indices;	\
		weights = ((GRSkinWeightDesc*)ptr)->weights;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((GRSkinWeightDesc*)ptr)->offset = offset;	\
		((GRSkinWeightDesc*)ptr)->indices = indices;	\
		((GRSkinWeightDesc*)ptr)->weights = weights;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRMeshDesc \
protected:\
	std::vector< Vec3f >	vertices;	\
	std::vector< Spr::GRMeshFace >	faces;	\
	std::vector< Vec3f >	normals;	\
	std::vector< Spr::GRMeshFace >	faceNormals;	\
	std::vector< Vec4f >	colors;	\
	std::vector< Vec2f >	texCoords;	\
	std::vector< int >	materialList;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRMeshDesc*)ptr);	\
		vertices = ((GRMeshDesc*)ptr)->vertices;	\
		faces = ((GRMeshDesc*)ptr)->faces;	\
		normals = ((GRMeshDesc*)ptr)->normals;	\
		faceNormals = ((GRMeshDesc*)ptr)->faceNormals;	\
		colors = ((GRMeshDesc*)ptr)->colors;	\
		texCoords = ((GRMeshDesc*)ptr)->texCoords;	\
		materialList = ((GRMeshDesc*)ptr)->materialList;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRMeshDesc*)ptr);	\
		((GRMeshDesc*)ptr)->vertices = vertices;	\
		((GRMeshDesc*)ptr)->faces = faces;	\
		((GRMeshDesc*)ptr)->normals = normals;	\
		((GRMeshDesc*)ptr)->faceNormals = faceNormals;	\
		((GRMeshDesc*)ptr)->colors = colors;	\
		((GRMeshDesc*)ptr)->texCoords = texCoords;	\
		((GRMeshDesc*)ptr)->materialList = materialList;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRLightDesc \
protected:\
	Vec4f	ambient;	\
	Vec4f	diffuse;	\
	Vec4f	specular;	\
	Vec4f	position;	\
	float	range;	\
	float	attenuation0;	\
	float	attenuation1;	\
	float	attenuation2;	\
	Vec3f	spotDirection;	\
	float	spotFalloff;	\
	float	spotInner;	\
	float	spotCutoff;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRLightDesc*)ptr);	\
		ambient = ((GRLightDesc*)ptr)->ambient;	\
		diffuse = ((GRLightDesc*)ptr)->diffuse;	\
		specular = ((GRLightDesc*)ptr)->specular;	\
		position = ((GRLightDesc*)ptr)->position;	\
		range = ((GRLightDesc*)ptr)->range;	\
		attenuation0 = ((GRLightDesc*)ptr)->attenuation0;	\
		attenuation1 = ((GRLightDesc*)ptr)->attenuation1;	\
		attenuation2 = ((GRLightDesc*)ptr)->attenuation2;	\
		spotDirection = ((GRLightDesc*)ptr)->spotDirection;	\
		spotFalloff = ((GRLightDesc*)ptr)->spotFalloff;	\
		spotInner = ((GRLightDesc*)ptr)->spotInner;	\
		spotCutoff = ((GRLightDesc*)ptr)->spotCutoff;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRLightDesc*)ptr);	\
		((GRLightDesc*)ptr)->ambient = ambient;	\
		((GRLightDesc*)ptr)->diffuse = diffuse;	\
		((GRLightDesc*)ptr)->specular = specular;	\
		((GRLightDesc*)ptr)->position = position;	\
		((GRLightDesc*)ptr)->range = range;	\
		((GRLightDesc*)ptr)->attenuation0 = attenuation0;	\
		((GRLightDesc*)ptr)->attenuation1 = attenuation1;	\
		((GRLightDesc*)ptr)->attenuation2 = attenuation2;	\
		((GRLightDesc*)ptr)->spotDirection = spotDirection;	\
		((GRLightDesc*)ptr)->spotFalloff = spotFalloff;	\
		((GRLightDesc*)ptr)->spotInner = spotInner;	\
		((GRLightDesc*)ptr)->spotCutoff = spotCutoff;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRMaterialDesc \
protected:\
	Vec4f	ambient;	\
	Vec4f	diffuse;	\
	Vec4f	specular;	\
	Vec4f	emissive;	\
	float	power;	\
	std::string	texname;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRMaterialDesc*)ptr);	\
		ambient = ((GRMaterialDesc*)ptr)->ambient;	\
		diffuse = ((GRMaterialDesc*)ptr)->diffuse;	\
		specular = ((GRMaterialDesc*)ptr)->specular;	\
		emissive = ((GRMaterialDesc*)ptr)->emissive;	\
		power = ((GRMaterialDesc*)ptr)->power;	\
		texname = ((GRMaterialDesc*)ptr)->texname;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRMaterialDesc*)ptr);	\
		((GRMaterialDesc*)ptr)->ambient = ambient;	\
		((GRMaterialDesc*)ptr)->diffuse = diffuse;	\
		((GRMaterialDesc*)ptr)->specular = specular;	\
		((GRMaterialDesc*)ptr)->emissive = emissive;	\
		((GRMaterialDesc*)ptr)->power = power;	\
		((GRMaterialDesc*)ptr)->texname = texname;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRCameraDesc \
protected:\
	Vec2f	size;	\
	Vec2f	center;	\
	float	front;	\
	float	back;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRCameraDesc*)ptr);	\
		size = ((GRCameraDesc*)ptr)->size;	\
		center = ((GRCameraDesc*)ptr)->center;	\
		front = ((GRCameraDesc*)ptr)->front;	\
		back = ((GRCameraDesc*)ptr)->back;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRCameraDesc*)ptr);	\
		((GRCameraDesc*)ptr)->size = size;	\
		((GRCameraDesc*)ptr)->center = center;	\
		((GRCameraDesc*)ptr)->front = front;	\
		((GRCameraDesc*)ptr)->back = back;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRShadowLightDesc \
protected:\
	bool	directional;	\
	Vec3f	position;	\
	Vec3f	lookat;	\
	Vec3f	up;	\
	Vec2f	size;	\
	float	fov;	\
	float	front;	\
	float	back;	\
	int	texWidth;	\
	int	texHeight;	\
	float	offset;	\
	Vec4f	color;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		GRVisual::SetDesc((GRVisualDesc*)(GRShadowLightDesc*)ptr);	\
		directional = ((GRShadowLightDesc*)ptr)->directional;	\
		position = ((GRShadowLightDesc*)ptr)->position;	\
		lookat = ((GRShadowLightDesc*)ptr)->lookat;	\
		up = ((GRShadowLightDesc*)ptr)->up;	\
		size = ((GRShadowLightDesc*)ptr)->size;	\
		fov = ((GRShadowLightDesc*)ptr)->fov;	\
		front = ((GRShadowLightDesc*)ptr)->front;	\
		back = ((GRShadowLightDesc*)ptr)->back;	\
		texWidth = ((GRShadowLightDesc*)ptr)->texWidth;	\
		texHeight = ((GRShadowLightDesc*)ptr)->texHeight;	\
		offset = ((GRShadowLightDesc*)ptr)->offset;	\
		color = ((GRShadowLightDesc*)ptr)->color;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		GRVisual::GetDesc((GRVisualDesc*)(GRShadowLightDesc*)ptr);	\
		((GRShadowLightDesc*)ptr)->directional = directional;	\
		((GRShadowLightDesc*)ptr)->position = position;	\
		((GRShadowLightDesc*)ptr)->lookat = lookat;	\
		((GRShadowLightDesc*)ptr)->up = up;	\
		((GRShadowLightDesc*)ptr)->size = size;	\
		((GRShadowLightDesc*)ptr)->fov = fov;	\
		((GRShadowLightDesc*)ptr)->front = front;	\
		((GRShadowLightDesc*)ptr)->back = back;	\
		((GRShadowLightDesc*)ptr)->texWidth = texWidth;	\
		((GRShadowLightDesc*)ptr)->texHeight = texHeight;	\
		((GRShadowLightDesc*)ptr)->offset = offset;	\
		((GRShadowLightDesc*)ptr)->color = color;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRSceneDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRSdkDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRShaderDesc \
protected:\
	std::string	vsname;	\
	std::string	fsname;	\
	bool	bEnableLighting;	\
	bool	bEnableTexture;	\
	bool	bShadowCreate;	\
	bool	bShadowRender;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		vsname = ((GRShaderDesc*)ptr)->vsname;	\
		fsname = ((GRShaderDesc*)ptr)->fsname;	\
		bEnableLighting = ((GRShaderDesc*)ptr)->bEnableLighting;	\
		bEnableTexture = ((GRShaderDesc*)ptr)->bEnableTexture;	\
		bShadowCreate = ((GRShaderDesc*)ptr)->bShadowCreate;	\
		bShadowRender = ((GRShaderDesc*)ptr)->bShadowRender;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((GRShaderDesc*)ptr)->vsname = vsname;	\
		((GRShaderDesc*)ptr)->fsname = fsname;	\
		((GRShaderDesc*)ptr)->bEnableLighting = bEnableLighting;	\
		((GRShaderDesc*)ptr)->bEnableTexture = bEnableTexture;	\
		((GRShaderDesc*)ptr)->bShadowCreate = bShadowCreate;	\
		((GRShaderDesc*)ptr)->bShadowRender = bShadowRender;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_GRSphereDesc \
protected:\
	float	radius;	\
	int	slices;	\
	int	stacks;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		radius = ((GRSphereDesc*)ptr)->radius;	\
		slices = ((GRSphereDesc*)ptr)->slices;	\
		stacks = ((GRSphereDesc*)ptr)->stacks;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((GRSphereDesc*)ptr)->radius = radius;	\
		((GRSphereDesc*)ptr)->slices = slices;	\
		((GRSphereDesc*)ptr)->stacks = stacks;	\
		return true;	\
	}\

