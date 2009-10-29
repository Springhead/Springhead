#define SPR_OVERRIDEMEMBERFUNCOF_GRBlendMeshIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRVisualIf(base)	\
	void Render(Spr::GRRenderIf* r){	base::Render(r);}	\
	void Rendered(Spr::GRRenderIf* r){	base::Rendered(r);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRFrameIf(base)	\
	Spr::GRFrameIf* GetParent(){	return	base::GetParent();}	\
	void SetParent(Spr::GRFrameIf* fr){	base::SetParent(fr);}	\
	int NChildren(){	return	base::NChildren();}	\
	Spr::GRVisualIf** GetChildren(){	return	base::GetChildren();}	\
	Affinef GetTransform(){	return	base::GetTransform();}	\
	Affinef GetWorldTransform(){	return	base::GetWorldTransform();}	\
	void SetTransform(const Affinef& af){	base::SetTransform(af);}	\
	void Clear(){	base::Clear();}	\
	void Print(std::ostream& os)const{	base::Print(os);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRDummyFrameIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRAnimationIf(base)	\
	void BlendPose(float time, float weight){	base::BlendPose(time, weight);}	\
	void ResetPose(){	base::ResetPose();}	\
	void LoadInitialPose(){	base::LoadInitialPose();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRAnimationSetIf(base)	\
	void BlendPose(float time, float weight){	base::BlendPose(time, weight);}	\
	void ResetPose(){	base::ResetPose();}	\
	void LoadInitialPose(){	base::LoadInitialPose();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRAnimationControllerIf(base)	\
	void BlendPose(UTString name, float time, float weight){	base::BlendPose(name, time, weight);}	\
	void ResetPose(){	base::ResetPose();}	\
	void LoadInitialPose(){	base::LoadInitialPose();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRMeshIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRLightIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRMaterialIf(base)	\
	bool IsOpaque()const{	return	base::IsOpaque();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRCameraIf(base)	\
	Spr::GRFrameIf* GetFrame(){	return	base::GetFrame();}	\
	void SetFrame(Spr::GRFrameIf* fr){	base::SetFrame(fr);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRRenderBaseIf(base)	\
	void SetViewport(Vec2f pos, Vec2f sz){	base::SetViewport(pos, sz);}	\
	void ClearBuffer(){	base::ClearBuffer();}	\
	void GetClearColor(Vec4f& color){	base::GetClearColor(color);}	\
	void SetClearColor(const Vec4f& color){	base::SetClearColor(color);}	\
	void BeginScene(){	base::BeginScene();}	\
	void EndScene(){	base::EndScene();}	\
	void SetViewMatrix(const Affinef& afv){	base::SetViewMatrix(afv);}	\
	void SetProjectionMatrix(const Affinef& afp){	base::SetProjectionMatrix(afp);}	\
	void GetProjectionMatrix(Affinef& afp){	base::GetProjectionMatrix(afp);}	\
	void SetModelMatrix(const Affinef& afw){	base::SetModelMatrix(afw);}	\
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
	void DrawSphere(float radius, int slices, int stacks, bool solid){	base::DrawSphere(radius, slices, stacks, solid);}	\
	void DrawCone(float radius, float height, int slice, bool solid){	base::DrawCone(radius, height, slice, solid);}	\
	void DrawCylinder(float radius, float height, int slice, bool solid){	base::DrawCylinder(radius, height, slice, solid);}	\
	int StartList(){	return	base::StartList();}	\
	void EndList(){	base::EndList();}	\
	void DrawList(int i){	base::DrawList(i);}	\
	void ReleaseList(int i){	base::ReleaseList(i);}	\
	void SetFont(const Spr::GRFont& font){	base::SetFont(font);}	\
	void DrawFont(Vec2f pos, const std::string str){	base::DrawFont(pos, str);}	\
	void DrawFont(Vec3f pos, const std::string str){	base::DrawFont(pos, str);}	\
	void SetMaterial(const Spr::GRMaterialDesc& mat){	base::SetMaterial(mat);}	\
	void SetMaterial(const Spr::GRMaterialIf* mat){	base::SetMaterial(mat);}	\
	void SetLineWidth(float w){	base::SetLineWidth(w);}	\
	void PushLight(const Spr::GRLightDesc& light){	base::PushLight(light);}	\
	void PushLight(const Spr::GRLightIf* light){	base::PushLight(light);}	\
	void PopLight(){	base::PopLight();}	\
	void SetDepthWrite(bool b){	base::SetDepthWrite(b);}	\
	void SetDepthTest(bool b){	base::SetDepthTest(b);}	\
	void SetDepthFunc(Spr::GRRenderBaseIf::TDepthFunc f){	base::SetDepthFunc(f);}	\
	void SetAlphaTest(bool b){	base::SetAlphaTest(b);}	\
	void SetAlphaMode(Spr::GRRenderBaseIf::TBlendFunc src, Spr::GRRenderBaseIf::TBlendFunc dest){	base::SetAlphaMode(src, dest);}	\
	void SetLighting(bool l){	base::SetLighting(l);}	\
	unsigned int LoadTexture(const std::string filename){	return	base::LoadTexture(filename);}	\
	void SetTextureImage(const std::string id, int components, int xsize, int ysize, int format, char* tb){	base::SetTextureImage(id, components, xsize, ysize, format, tb);}	\
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

#define SPR_OVERRIDEMEMBERFUNCOF_GRDeviceIf(base)	\
	void Init(){	base::Init();}	\
	void Print(std::ostream& os)const{	base::Print(os);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRDeviceGLIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRDebugRenderIf(base)	\
	void DrawScene(PHSceneIf* scene){	base::DrawScene(scene);}	\
	void DrawSolid(PHSolidIf* solid){	base::DrawSolid(solid);}	\
	void SetMaterialSample(Spr::GRDebugRenderIf::TMaterialSample matname){	base::SetMaterialSample(matname);}	\
	void SetRenderMode(bool solid, bool wire){	base::SetRenderMode(solid, wire);}	\
	void EnableRenderWorldAxis(bool enable){	base::EnableRenderWorldAxis(enable);}	\
	void EnableRenderAxis(bool enable, float scale){	base::EnableRenderAxis(enable, scale);}	\
	void EnableRenderForce(bool enable, float scale){	base::EnableRenderForce(enable, scale);}	\
	void EnableRenderContact(bool enable){	base::EnableRenderContact(enable);}	\
	void EnableGrid(bool enable, double y, double span){	base::EnableGrid(enable, y, span);}	\
	void EnableRenderIK(bool enable, float scale){	base::EnableRenderIK(enable, scale);}	\
	void DrawFaceSolid(CDFaceIf* face, Vec3f* base){	base::DrawFaceSolid(face, base);}	\
	void DrawFaceWire(CDFaceIf* face, Vec3f* base){	base::DrawFaceWire(face, base);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSceneIf(base)	\
	void Render(Spr::GRRenderIf* r){	base::Render(r);}	\
	Spr::GRFrameIf* GetWorld(){	return	base::GetWorld();}	\
	Spr::GRCameraIf* GetCamera(){	return	base::GetCamera();}	\
	Spr::GRAnimationControllerIf* GetAnimationController(){	return	base::GetAnimationController();}	\
	void SetCamera(const Spr::GRCameraDesc& desc){	base::SetCamera(desc);}	\
	Spr::GRVisualIf* CreateVisual(const IfInfo* info, const Spr::GRVisualDesc& desc, Spr::GRFrameIf* parent){	return	base::CreateVisual(info, desc, parent);}	\
	Spr::GRSdkIf* GetSdk(){	return	base::GetSdk();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSdkIf(base)	\
	Spr::GRDebugRenderIf* CreateDebugRender(){	return	base::CreateDebugRender();}	\
	Spr::GRDeviceGLIf* CreateDeviceGL(){	return	base::CreateDeviceGL();}	\
	Spr::GRSceneIf* CreateScene(const Spr::GRSceneDesc& desc){	return	base::CreateScene(desc);}	\
	Spr::GRSceneIf* GetScene(size_t i){	return	base::GetScene(i);}	\
	size_t NScene(){	return	base::NScene();}	\
	void MergeScene(Spr::GRSceneIf* scene0, Spr::GRSceneIf* scene1){	base::MergeScene(scene0, scene1);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_GRSphereIf(base)	\

