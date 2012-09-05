#define SPR_OVERRIDEMEMBERFUNCOF_GRBlendMeshIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRVisualIf(base)	\
	void Render(Spr::GRRenderIf* r){	base::Render(r);}	\
	void Rendered(Spr::GRRenderIf* r){	base::Rendered(r);}	\
	void Enable(bool on){	base::Enable(on);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRFrameIf(base)	\
	Spr::GRFrameIf* GetParent(){	return	base::GetParent();}	\
	void SetParent(Spr::GRFrameIf* fr){	base::SetParent(fr);}	\
	int NChildren(){	return	base::NChildren();}	\
	Spr::GRVisualIf** GetChildren(){	return	base::GetChildren();}	\
	Affinef GetTransform(){	return	base::GetTransform();}	\
	Affinef GetWorldTransform(){	return	base::GetWorldTransform();}	\
	void SetTransform(const Affinef& af){	base::SetTransform(af);}	\
	bool CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Affinef& aff){	return	base::CalcBBox(bbmin, bbmax, aff);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRDummyFrameIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRAnimationIf(base)	\
	void BlendPose(float time, float weight){	base::BlendPose(time, weight);}	\
	void ResetPose(){	base::ResetPose();}	\
	void LoadInitialPose(){	base::LoadInitialPose();}	\
	Spr::GRAnimationKey GetAnimationKey(int n){	return	base::GetAnimationKey(n);}	\
	int NAnimationKey(){	return	base::NAnimationKey();}	\
	void DeletePose(float t){	base::DeletePose(t);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRAnimationSetIf(base)	\
	void BlendPose(float time, float weight){	base::BlendPose(time, weight);}	\
	void ResetPose(){	base::ResetPose();}	\
	void LoadInitialPose(){	base::LoadInitialPose();}	\
	Spr::ObjectIf* GetChildObject(size_t p){	return	base::GetChildObject(p);}	\
	void SetCurrentAnimationPose(float t){	base::SetCurrentAnimationPose(t);}	\
	void DeleteAnimationPose(float t){	base::DeleteAnimationPose(t);}	\
	float GetLastKeyTime(){	return	base::GetLastKeyTime();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRAnimationControllerIf(base)	\
	void BlendPose(UTString name, float time, float weight){	base::BlendPose(name, time, weight);}	\
	void ResetPose(){	base::ResetPose();}	\
	void LoadInitialPose(){	base::LoadInitialPose();}	\
	bool AddChildObject(Spr::ObjectIf* o){	return	base::AddChildObject(o);}	\
	bool DelChildObject(Spr::ObjectIf* o){	return	base::DelChildObject(o);}	\
	size_t NChildObject(){	return	base::NChildObject();}	\
	Spr::ObjectIf* GetChildObject(size_t p){	return	base::GetChildObject(p);}	\
	Spr::GRAnimationSetIf* GetAnimationSet(size_t p){	return	base::GetAnimationSet(p);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSkinWeightIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRMeshIf(base)	\
	int NVertex(){	return	base::NVertex();}	\
	int NTriangle(){	return	base::NTriangle();}	\
	int NFace(){	return	base::NFace();}	\
	Vec3f* GetVertices(){	return	base::GetVertices();}	\
	Vec3f* GetNormals(){	return	base::GetNormals();}	\
	Vec4f* GetColors(){	return	base::GetColors();}	\
	Vec2f* GetTexCoords(){	return	base::GetTexCoords();}	\
	Spr::GRMeshFace* GetFaces(){	return	base::GetFaces();}	\
	Spr::GRMeshFace* GetFaceNormals(){	return	base::GetFaceNormals();}	\
	int* GetMaterialIndices(){	return	base::GetMaterialIndices();}	\
	void SwitchCoordinate(){	base::SwitchCoordinate();}	\
	void EnableTex3D(bool on){	base::EnableTex3D(on);}	\
	bool IsTex3D(){	return	base::IsTex3D();}	\
	Spr::GRSkinWeightIf* CreateSkinWeight(const Spr::GRSkinWeightDesc& desc){	return	base::CreateSkinWeight(desc);}	\
	void CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Affinef& aff){	base::CalcBBox(bbmin, bbmax, aff);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRLightIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRMaterialIf(base)	\
	bool IsOpaque()const{	return	base::IsOpaque();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRCameraIf(base)	\
	Spr::GRFrameIf* GetFrame(){	return	base::GetFrame();}	\
	void SetFrame(Spr::GRFrameIf* fr){	base::SetFrame(fr);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRRenderBaseIf(base)	\
	void SetViewport(Vec2f pos, Vec2f sz){	base::SetViewport(pos, sz);}	\
	void ClearBuffer(){	base::ClearBuffer();}	\
	void SwapBuffers(){	base::SwapBuffers();}	\
	void GetClearColor(Vec4f& color){	base::GetClearColor(color);}	\
	void SetClearColor(const Vec4f& color){	base::SetClearColor(color);}	\
	void BeginScene(){	base::BeginScene();}	\
	void EndScene(){	base::EndScene();}	\
	void SetViewMatrix(const Affinef& afv){	base::SetViewMatrix(afv);}	\
	void GetViewMatrix(Affinef& afv){	base::GetViewMatrix(afv);}	\
	void SetProjectionMatrix(const Affinef& afp){	base::SetProjectionMatrix(afp);}	\
	void GetProjectionMatrix(Affinef& afp){	base::GetProjectionMatrix(afp);}	\
	void SetModelMatrix(const Affinef& afw){	base::SetModelMatrix(afw);}	\
	void GetModelMatrix(Affinef& afw){	base::GetModelMatrix(afw);}	\
	void MultModelMatrix(const Affinef& afw){	base::MultModelMatrix(afw);}	\
	void PushModelMatrix(){	base::PushModelMatrix();}	\
	void PopModelMatrix(){	base::PopModelMatrix();}	\
	void ClearBlendMatrix(){	base::ClearBlendMatrix();}	\
	bool SetBlendMatrix(const Affinef& afb, unsigned int id){	return	base::SetBlendMatrix(afb, id);}	\
	void SetVertexFormat(const GRVertexElement* e){	base::SetVertexFormat(e);}	\
	void SetVertexShader(void* shader){	base::SetVertexShader(shader);}	\
	void DrawDirect(Spr::GRRenderBaseIf::TPrimitiveType ty, void* vtx, size_t count, size_t stride){	base::DrawDirect(ty, vtx, count, stride);}	\
	void DrawIndexed(Spr::GRRenderBaseIf::TPrimitiveType ty, size_t* idx, void* vtx, size_t count, size_t stride){	base::DrawIndexed(ty, idx, vtx, count, stride);}	\
	void DrawArrays(Spr::GRRenderBaseIf::TPrimitiveType ty, GRVertexArray* arrays, size_t count){	base::DrawArrays(ty, arrays, count);}	\
	void DrawArrays(Spr::GRRenderBaseIf::TPrimitiveType ty, size_t* idx, GRVertexArray* arrays, size_t count){	base::DrawArrays(ty, idx, arrays, count);}	\
	void DrawPoint(Vec3f p){	base::DrawPoint(p);}	\
	void DrawLine(Vec3f p0, Vec3f p1){	base::DrawLine(p0, p1);}	\
	void DrawSpline(Vec3f p0, Vec3f v0, Vec3f p1, Vec3f v1, int ndiv){	base::DrawSpline(p0, v0, p1, v1, ndiv);}	\
	void DrawArrow(Vec3f p0, Vec3f p1, float rbar, float rhead, float lhead, int slice, bool solid){	base::DrawArrow(p0, p1, rbar, rhead, lhead, slice, solid);}	\
	void DrawBox(float sx, float sy, float sz, bool solid){	base::DrawBox(sx, sy, sz, solid);}	\
	void DrawSphere(float radius, int slices, int stacks, bool solid){	base::DrawSphere(radius, slices, stacks, solid);}	\
	void DrawCone(float radius, float height, int slice, bool solid){	base::DrawCone(radius, height, slice, solid);}	\
	void DrawCylinder(float radius, float height, int slice, bool solid, bool cap){	base::DrawCylinder(radius, height, slice, solid, cap);}	\
	void DrawDisk(float radius, int slice, bool solid){	base::DrawDisk(radius, slice, solid);}	\
	void DrawCapsule(float radius, float height, int slice, bool solid){	base::DrawCapsule(radius, height, slice, solid);}	\
	void DrawRoundCone(float rbottom, float rtop, float height, int slice, bool solid){	base::DrawRoundCone(rbottom, rtop, height, slice, solid);}	\
	void DrawGrid(float size, int slice, float lineWidth){	base::DrawGrid(size, slice, lineWidth);}	\
	void DrawCurve(const Curve3f& curve){	base::DrawCurve(curve);}	\
	int StartList(){	return	base::StartList();}	\
	void EndList(){	base::EndList();}	\
	void DrawList(int i){	base::DrawList(i);}	\
	void ReleaseList(int i){	base::ReleaseList(i);}	\
	void SetFont(const Spr::GRFont& font){	base::SetFont(font);}	\
	void DrawFont(Vec2f pos, const std::string str){	base::DrawFont(pos, str);}	\
	void DrawFont(Vec3f pos, const std::string str){	base::DrawFont(pos, str);}	\
	void SetMaterial(const Spr::GRMaterialDesc& mat){	base::SetMaterial(mat);}	\
	void SetMaterial(const Spr::GRMaterialIf* mat){	base::SetMaterial(mat);}	\
	void SetMaterial(int matname){	base::SetMaterial(matname);}	\
	void SetPointSize(float sz, bool smooth){	base::SetPointSize(sz, smooth);}	\
	void SetLineWidth(float w, bool smooth){	base::SetLineWidth(w, smooth);}	\
	void PushLight(const Spr::GRLightDesc& light){	base::PushLight(light);}	\
	void PushLight(const Spr::GRLightIf* light){	base::PushLight(light);}	\
	void PopLight(){	base::PopLight();}	\
	int NLights(){	return	base::NLights();}	\
	void SetDepthWrite(bool b){	base::SetDepthWrite(b);}	\
	void SetDepthTest(bool b){	base::SetDepthTest(b);}	\
	void SetDepthFunc(Spr::GRRenderBaseIf::TDepthFunc f){	base::SetDepthFunc(f);}	\
	void SetAlphaTest(bool b){	base::SetAlphaTest(b);}	\
	void SetAlphaMode(Spr::GRRenderBaseIf::TBlendFunc src, Spr::GRRenderBaseIf::TBlendFunc dest){	base::SetAlphaMode(src, dest);}	\
	void SetLighting(bool l){	base::SetLighting(l);}	\
	unsigned int LoadTexture(const std::string filename){	return	base::LoadTexture(filename);}	\
	void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, const char* tb){	base::SetTextureImage(id, components, xsize, ysize, format, tb);}	\
	void InitShader(){	base::InitShader();}	\
	void SetShaderFormat(GRShaderFormat::ShaderType type){	base::SetShaderFormat(type);}	\
	bool CreateShader(std::string vShaderFile, std::string fShaderFile, Spr::GRHandler& shader){	return	base::CreateShader(vShaderFile, fShaderFile, shader);}	\
	Spr::GRHandler CreateShader(){	return	base::CreateShader();}	\
	bool ReadShaderSource(Spr::GRHandler shader, std::string file){	return	base::ReadShaderSource(shader, file);}	\
	void GetShaderLocation(Spr::GRHandler shader, void* location){	base::GetShaderLocation(shader, location);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRRenderIf(base)	\
	void SetDevice(Spr::GRDeviceIf* dev){	base::SetDevice(dev);}	\
	Spr::GRDeviceIf* GetDevice(){	return	base::GetDevice();}	\
	void SetCamera(const Spr::GRCameraDesc& cam){	base::SetCamera(cam);}	\
	const Spr::GRCameraDesc& GetCamera(){	return	base::GetCamera();}	\
	void Reshape(Vec2f pos, Vec2f screenSize){	base::Reshape(pos, screenSize);}	\
	Vec2f GetViewportPos(){	return	base::GetViewportPos();}	\
	Vec2f GetViewportSize(){	return	base::GetViewportSize();}	\
	Vec2f GetPixelSize(){	return	base::GetPixelSize();}	\
	Vec3f ScreenToCamera(int x, int y, float depth, bool LorR){	return	base::ScreenToCamera(x, y, depth, LorR);}	\
	void EnterScreenCoordinate(){	base::EnterScreenCoordinate();}	\
	void LeaveScreenCoordinate(){	base::LeaveScreenCoordinate();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRDeviceIf(base)	\
	void Init(){	base::Init();}	\
	void Print(std::ostream& os)const{	base::Print(os);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRDeviceGLIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSceneIf(base)	\
	void Render(Spr::GRRenderIf* r){	base::Render(r);}	\
	Spr::GRFrameIf* GetWorld(){	return	base::GetWorld();}	\
	Spr::GRCameraIf* GetCamera(){	return	base::GetCamera();}	\
	void SetCamera(const Spr::GRCameraDesc& desc){	base::SetCamera(desc);}	\
	Spr::GRAnimationControllerIf* GetAnimationController(){	return	base::GetAnimationController();}	\
	Spr::GRVisualIf* CreateVisual(const IfInfo* info, const Spr::GRVisualDesc& desc, Spr::GRFrameIf* parent){	return	base::CreateVisual(info, desc, parent);}	\
	Spr::GRSdkIf* GetSdk(){	return	base::GetSdk();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSdkIf(base)	\
	Spr::GRRenderIf* CreateRender(){	return	base::CreateRender();}	\
	Spr::GRDeviceGLIf* CreateDeviceGL(){	return	base::CreateDeviceGL();}	\
	Spr::GRSceneIf* CreateScene(const Spr::GRSceneDesc& desc){	return	base::CreateScene(desc);}	\
	Spr::GRSceneIf* GetScene(size_t i){	return	base::GetScene(i);}	\
	size_t NScene(){	return	base::NScene();}	\
	void MergeScene(Spr::GRSceneIf* scene0, Spr::GRSceneIf* scene1){	base::MergeScene(scene0, scene1);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSphereIf(base)	\

