//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRBlendMeshIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
};
struct GRBlendMeshIf;	class GRBlendMesh;
typedef GRBlendMeshIfStubTemplate<GRBlendMeshIf, GRBlendMesh>	GRBlendMeshIfStub;
typedef IfInitTemplate<GRBlendMeshIfStub, GRBlendMesh>	GRBlendMeshIfInit;

template <class IF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class OBJ>
struct GRVisualIfStubTemplate: public NamedObjectIfStubTemplate<IF, OBJ> {
	virtual void Render(GRRenderIf *  r){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Render(r);
	}
	virtual void Rendered(GRRenderIf *  r){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Rendered(r);
	}
};
struct GRVisualIf;	class GRVisual;
typedef GRVisualIfStubTemplate<GRVisualIf, GRVisual>	GRVisualIfStub;
typedef IfInitTemplate<GRVisualIfStub, GRVisual>	GRVisualIfInit;

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRFrameIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
	virtual GRFrameIf *  GetParent(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetParent();
	}
	virtual void SetParent(GRFrameIf *  fr){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetParent(fr);
	}
	virtual Affinef GetTransform(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetTransform();
	}
	virtual Affinef GetWorldTransform(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetWorldTransform();
	}
	virtual void SetTransform(Affinef &  af){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetTransform(af);
	}
	virtual void Print(std::ostream &  os) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Print(os);
	}
};
struct GRFrameIf;	class GRFrame;
typedef GRFrameIfStubTemplate<GRFrameIf, GRFrame>	GRFrameIfStub;
typedef IfInitTemplate<GRFrameIfStub, GRFrame>	GRFrameIfInit;

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRMeshIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
};
struct GRMeshIf;	class GRMesh;
typedef GRMeshIfStubTemplate<GRMeshIf, GRMesh>	GRMeshIfStub;
typedef IfInitTemplate<GRMeshIfStub, GRMesh>	GRMeshIfInit;

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRLightIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
};
struct GRLightIf;	class GRLight;
typedef GRLightIfStubTemplate<GRLightIf, GRLight>	GRLightIfStub;
typedef IfInitTemplate<GRLightIfStub, GRLight>	GRLightIfInit;

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRMaterialIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
	virtual bool IsOpaque() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::IsOpaque();
	}
};
struct GRMaterialIf;	class GRMaterial;
typedef GRMaterialIfStubTemplate<GRMaterialIf, GRMaterial>	GRMaterialIfStub;
typedef IfInitTemplate<GRMaterialIfStub, GRMaterial>	GRMaterialIfInit;

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRCameraIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
	virtual GRFrameIf *  GetFrame(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetFrame();
	}
	virtual void SetFrame(GRFrameIf *  fr){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetFrame(fr);
	}
};
struct GRCameraIf;	class GRCamera;
typedef GRCameraIfStubTemplate<GRCameraIf, GRCamera>	GRCameraIfStub;
typedef IfInitTemplate<GRCameraIfStub, GRCamera>	GRCameraIfInit;

template <class IF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class OBJ>
struct GRRenderBaseIfStubTemplate: public ObjectIfStubTemplate<IF, OBJ> {
	virtual void SetViewport(Vec2f pos, Vec2f sz){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetViewport(pos, sz);
	}
	virtual void ClearBuffer(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::ClearBuffer();
	}
	virtual void BeginScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::BeginScene();
	}
	virtual void EndScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::EndScene();
	}
	virtual void SetViewMatrix(const Affinef &  afv){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetViewMatrix(afv);
	}
	virtual void SetProjectionMatrix(const Affinef &  afp){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetProjectionMatrix(afp);
	}
	virtual void SetModelMatrix(const Affinef &  afw){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetModelMatrix(afw);
	}
	virtual void MultModelMatrix(const Affinef &  afw){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::MultModelMatrix(afw);
	}
	virtual void PushModelMatrix(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PushModelMatrix();
	}
	virtual void PopModelMatrix(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PopModelMatrix();
	}
	virtual bool SetModelMatrices(const Affinef &  afw, unsigned int matrixId, unsigned int elementId){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetModelMatrices(afw, matrixId, elementId);
	}
	virtual bool MultModelMatrices(const Affinef &  afw, unsigned int matrixId){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::MultModelMatrices(afw, matrixId);
	}
	virtual bool PushModelMatrices(const Affinef &  afw, unsigned int matrixId){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PushModelMatrices(afw, matrixId);
	}
	virtual bool PopModelMatrices(unsigned int matrixId){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PopModelMatrices(matrixId);
	}
	virtual void SetVertexFormat(const GRVertexElement *  e){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetVertexFormat(e);
	}
	virtual void SetVertexShader(void *  shader){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetVertexShader(shader);
	}
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawDirect(ty, vtx, count, stride);
	}
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawIndexed(ty, idx, vtx, count, stride);
	}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, GRVertexArray *  arrays, size_t count){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawArrays(ty, arrays, count);
	}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, GRVertexArray *  arrays, size_t count){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawArrays(ty, idx, arrays, count);
	}
	virtual int CreateList(GRRenderBaseIf::TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(ty, vtx, count, stride);
	}
	virtual int CreateList(GRMaterialIf *  mat, unsigned int texid, GRRenderBaseIf::TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(mat, texid, ty, vtx, count, stride);
	}
	virtual int CreateList(float radius, int slices, int stacks){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(radius, slices, stacks);
	}
	virtual int CreateList(GRMaterialIf *  mat, float radius, int slices, int stacks){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(mat, radius, slices, stacks);
	}
	virtual int CreateIndexedList(GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateIndexedList(ty, idx, vtx, count, stride);
	}
	virtual int CreateIndexedList(GRMaterialIf *  mat, GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateIndexedList(mat, ty, idx, vtx, count, stride);
	}
	virtual void DrawList(int i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawList(i);
	}
	virtual void ReleaseList(int i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::ReleaseList(i);
	}
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont &  font = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawFont(pos, str, font);
	}
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont &  font = 0){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawFont(pos, str, font);
	}
	virtual void SetMaterial(const GRMaterialDesc &  mat){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaterial(mat);
	}
	virtual void SetMaterial(const GRMaterialIf *  mat){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaterial(mat);
	}
	virtual void SetLineWidth(float w){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetLineWidth(w);
	}
	virtual void PushLight(const GRLightDesc &  light){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PushLight(light);
	}
	virtual void PushLight(const GRLightIf *  light){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PushLight(light);
	}
	virtual void PopLight(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::PopLight();
	}
	virtual void SetDepthWrite(bool b){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDepthWrite(b);
	}
	virtual void SetDepthTest(bool b){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDepthTest(b);
	}
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDepthFunc(f);
	}
	virtual void SetAlphaTest(bool b){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetAlphaTest(b);
	}
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetAlphaMode(src, dest);
	}
	virtual unsigned int LoadTexture(const std::string filename){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::LoadTexture(filename);
	}
	virtual void InitShader(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::InitShader();
	}
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler &  shaderProgram){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShader(vShaderFile, fShaderFile, shaderProgram);
	}
	virtual bool CreateShader(std::string vShaderFile, GRHandler &  shaderProgram){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShader(vShaderFile, shaderProgram);
	}
	virtual bool ReadShaderSource(GRHandler shader, std::string file){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::ReadShaderSource(shader, file);
	}
};
struct GRRenderBaseIf;	class GRRenderBase;
typedef GRRenderBaseIfStubTemplate<GRRenderBaseIf, GRRenderBase>	GRRenderBaseIfStub;
typedef IfInitTemplate<GRRenderBaseIfStub, GRRenderBase>	GRRenderBaseIfInit;

template <class IF, class OBJ> struct GRRenderBaseIfStubTemplate;
template <class IF, class OBJ>
struct GRRenderIfStubTemplate: public GRRenderBaseIfStubTemplate<IF, OBJ> {
	virtual void SetDevice(GRDeviceIf *  dev){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDevice(dev);
	}
	virtual void SetCamera(const GRCameraDesc &  cam){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetCamera(cam);
	}
	virtual void Reshape(Vec2f pos, Vec2f screenSize){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Reshape(pos, screenSize);
	}
};
struct GRRenderIf;	class GRRender;
typedef GRRenderIfStubTemplate<GRRenderIf, GRRender>	GRRenderIfStub;
typedef IfInitTemplate<GRRenderIfStub, GRRender>	GRRenderIfInit;

template <class IF, class OBJ> struct GRRenderBaseIfStubTemplate;
template <class IF, class OBJ>
struct GRDeviceIfStubTemplate: public GRRenderBaseIfStubTemplate<IF, OBJ> {
	virtual void Init(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Init();
	}
	virtual void Print(std::ostream &  os) const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Print(os);
	}
};
struct GRDeviceIf;	class GRDevice;
typedef GRDeviceIfStubTemplate<GRDeviceIf, GRDevice>	GRDeviceIfStub;
typedef IfInitTemplate<GRDeviceIfStub, GRDevice>	GRDeviceIfInit;

template <class IF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class OBJ>
struct GRDeviceGLIfStubTemplate: public GRDeviceIfStubTemplate<IF, OBJ> {
	virtual void SetWindow(int w){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetWindow(w);
	}
};
struct GRDeviceGLIf;	class GRDeviceGL;
typedef GRDeviceGLIfStubTemplate<GRDeviceGLIf, GRDeviceGL>	GRDeviceGLIfStub;
typedef IfInitTemplate<GRDeviceGLIfStub, GRDeviceGL>	GRDeviceGLIfInit;

template <class IF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class OBJ>
struct GRDeviceD3DIfStubTemplate: public GRDeviceIfStubTemplate<IF, OBJ> {
};
struct GRDeviceD3DIf;	class GRDeviceD3D;
typedef GRDeviceD3DIfStubTemplate<GRDeviceD3DIf, GRDeviceD3D>	GRDeviceD3DIfStub;
typedef IfInitTemplate<GRDeviceD3DIfStub, GRDeviceD3D>	GRDeviceD3DIfInit;

template <class IF, class OBJ> struct GRRenderIfStubTemplate;
template <class IF, class OBJ>
struct GRDebugRenderIfStubTemplate: public GRRenderIfStubTemplate<IF, OBJ> {
	virtual void DrawScene(PHSceneIf *  scene){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawScene(scene);
	}
	virtual void DrawSolid(PHSolidIf *  so){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawSolid(so);
	}
	virtual void DrawFace(CDFaceIf *  face, Vec3f *  base){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::DrawFace(face, base);
	}
	virtual void SetMaterialSample(GRDebugRenderIf::TMaterialSample matname){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaterialSample(matname);
	}
};
struct GRDebugRenderIf;	class GRDebugRender;
typedef GRDebugRenderIfStubTemplate<GRDebugRenderIf, GRDebugRender>	GRDebugRenderIfStub;
typedef IfInitTemplate<GRDebugRenderIfStub, GRDebugRender>	GRDebugRenderIfInit;

template <class IF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class OBJ>
struct GRSceneIfStubTemplate: public SceneIfStubTemplate<IF, OBJ> {
	virtual GRFrameIf *  CreateFrame(const GRFrameDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateFrame(desc);
	}
	virtual void Render(GRRenderIf *  r){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Render(r);
	}
	virtual GRFrameIf *  GetWorld(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetWorld();
	}
	virtual GRCameraIf *  GetCamera(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetCamera();
	}
	virtual GRSdkIf *  GetSdk(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSdk();
	}
};
struct GRSceneIf;	class GRScene;
typedef GRSceneIfStubTemplate<GRSceneIf, GRScene>	GRSceneIfStub;
typedef IfInitTemplate<GRSceneIfStub, GRScene>	GRSceneIfInit;

template <class IF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class OBJ>
struct GRSdkIfStubTemplate: public SdkIfStubTemplate<IF, OBJ> {
	virtual GRDebugRenderIf *  CreateDebugRender(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateDebugRender();
	}
	virtual GRDeviceGLIf *  CreateDeviceGL(int window){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateDeviceGL(window);
	}
	virtual GRSceneIf *  CreateScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene();
	}
	virtual size_t NChildObject() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NChildObject();
	}
	virtual ObjectIf *  GetChildObject(size_t i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetChildObject(i);
	}
	virtual GRSceneIf *  GetScene(size_t i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene(i);
	}
	virtual size_t NScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NScene();
	}
};
struct GRSdkIf;	class GRSdk;
typedef GRSdkIfStubTemplate<GRSdkIf, GRSdk>	GRSdkIfStub;
typedef IfInitTemplate<GRSdkIfStub, GRSdk>	GRSdkIfInit;

template <class IF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class OBJ>
struct GRSphereIfStubTemplate: public GRVisualIfStubTemplate<IF, OBJ> {
};
struct GRSphereIf;	class GRSphere;
typedef GRSphereIfStubTemplate<GRSphereIf, GRSphere>	GRSphereIfStub;
typedef IfInitTemplate<GRSphereIfStub, GRSphere>	GRSphereIfInit;
}	//	namespace Spr; 
