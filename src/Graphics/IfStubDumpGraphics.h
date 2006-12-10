//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRBlendMeshIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
};
struct GRBlendMeshIf;	class GRBlendMesh;
typedef GRBlendMeshIfStubTemplate<GRBlendMeshIf, ObjectIfBuf, GRBlendMesh>	GRBlendMeshIfStub;
typedef IfInitTemplate<GRBlendMeshIfStub, GRBlendMesh>	GRBlendMeshIfInit;

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRVisualIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Render(GRRenderIf *  r){
		return ((OBJ*)(MIF*)this)->OBJ::Render(r);
	}
	virtual void Rendered(GRRenderIf *  r){
		return ((OBJ*)(MIF*)this)->OBJ::Rendered(r);
	}
};
struct GRVisualIf;	class GRVisual;
typedef GRVisualIfStubTemplate<GRVisualIf, ObjectIfBuf, GRVisual>	GRVisualIfStub;
typedef IfInitTemplate<GRVisualIfStub, GRVisual>	GRVisualIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRFrameIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
	virtual GRFrameIf *  GetParent(){
		return ((OBJ*)(MIF*)this)->OBJ::GetParent();
	}
	virtual void SetParent(GRFrameIf *  fr){
		return ((OBJ*)(MIF*)this)->OBJ::SetParent(fr);
	}
	virtual Affinef GetTransform(){
		return ((OBJ*)(MIF*)this)->OBJ::GetTransform();
	}
	virtual Affinef GetWorldTransform(){
		return ((OBJ*)(MIF*)this)->OBJ::GetWorldTransform();
	}
	virtual void SetTransform(Affinef &  af){
		return ((OBJ*)(MIF*)this)->OBJ::SetTransform(af);
	}
	virtual void Print(std::ostream &  os) const {
		return ((OBJ*)(MIF*)this)->OBJ::Print(os);
	}
};
struct GRFrameIf;	class GRFrame;
typedef GRFrameIfStubTemplate<GRFrameIf, ObjectIfBuf, GRFrame>	GRFrameIfStub;
typedef IfInitTemplate<GRFrameIfStub, GRFrame>	GRFrameIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRMeshIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
};
struct GRMeshIf;	class GRMesh;
typedef GRMeshIfStubTemplate<GRMeshIf, ObjectIfBuf, GRMesh>	GRMeshIfStub;
typedef IfInitTemplate<GRMeshIfStub, GRMesh>	GRMeshIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRLightIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
};
struct GRLightIf;	class GRLight;
typedef GRLightIfStubTemplate<GRLightIf, ObjectIfBuf, GRLight>	GRLightIfStub;
typedef IfInitTemplate<GRLightIfStub, GRLight>	GRLightIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRMaterialIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
	virtual bool IsOpaque() const {
		return ((OBJ*)(MIF*)this)->OBJ::IsOpaque();
	}
};
struct GRMaterialIf;	class GRMaterial;
typedef GRMaterialIfStubTemplate<GRMaterialIf, ObjectIfBuf, GRMaterial>	GRMaterialIfStub;
typedef IfInitTemplate<GRMaterialIfStub, GRMaterial>	GRMaterialIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRCameraIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
	virtual GRFrameIf *  GetFrame(){
		return ((OBJ*)(MIF*)this)->OBJ::GetFrame();
	}
	virtual void SetFrame(GRFrameIf *  fr){
		return ((OBJ*)(MIF*)this)->OBJ::SetFrame(fr);
	}
};
struct GRCameraIf;	class GRCamera;
typedef GRCameraIfStubTemplate<GRCameraIf, ObjectIfBuf, GRCamera>	GRCameraIfStub;
typedef IfInitTemplate<GRCameraIfStub, GRCamera>	GRCameraIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRRenderBaseIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetViewport(Vec2f pos, Vec2f sz){
		return ((OBJ*)(MIF*)this)->OBJ::SetViewport(pos, sz);
	}
	virtual void ClearBuffer(){
		return ((OBJ*)(MIF*)this)->OBJ::ClearBuffer();
	}
	virtual void BeginScene(){
		return ((OBJ*)(MIF*)this)->OBJ::BeginScene();
	}
	virtual void EndScene(){
		return ((OBJ*)(MIF*)this)->OBJ::EndScene();
	}
	virtual void SetViewMatrix(const Affinef &  afv){
		return ((OBJ*)(MIF*)this)->OBJ::SetViewMatrix(afv);
	}
	virtual void SetProjectionMatrix(const Affinef &  afp){
		return ((OBJ*)(MIF*)this)->OBJ::SetProjectionMatrix(afp);
	}
	virtual void GetProjectionMatrix(const Affinef &  afp){
		return ((OBJ*)(MIF*)this)->OBJ::GetProjectionMatrix(afp);
	}
	virtual void SetModelMatrix(const Affinef &  afw){
		return ((OBJ*)(MIF*)this)->OBJ::SetModelMatrix(afw);
	}
	virtual void MultModelMatrix(const Affinef &  afw){
		return ((OBJ*)(MIF*)this)->OBJ::MultModelMatrix(afw);
	}
	virtual void PushModelMatrix(){
		return ((OBJ*)(MIF*)this)->OBJ::PushModelMatrix();
	}
	virtual void PopModelMatrix(){
		return ((OBJ*)(MIF*)this)->OBJ::PopModelMatrix();
	}
	virtual void ClearBlendMatrix(){
		return ((OBJ*)(MIF*)this)->OBJ::ClearBlendMatrix();
	}
	virtual bool SetBlendMatrix(const Affinef &  afb, unsigned int id = 0){
		return ((OBJ*)(MIF*)this)->OBJ::SetBlendMatrix(afb, id);
	}
	virtual void SetVertexFormat(const GRVertexElement *  e){
		return ((OBJ*)(MIF*)this)->OBJ::SetVertexFormat(e);
	}
	virtual void SetVertexShader(void *  shader){
		return ((OBJ*)(MIF*)this)->OBJ::SetVertexShader(shader);
	}
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::DrawDirect(ty, vtx, count, stride);
	}
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::DrawIndexed(ty, idx, vtx, count, stride);
	}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, GRVertexArray *  arrays, size_t count){
		return ((OBJ*)(MIF*)this)->OBJ::DrawArrays(ty, arrays, count);
	}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, GRVertexArray *  arrays, size_t count){
		return ((OBJ*)(MIF*)this)->OBJ::DrawArrays(ty, idx, arrays, count);
	}
	virtual int CreateList(GRRenderBaseIf::TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::CreateList(ty, vtx, count, stride);
	}
	virtual int CreateList(GRMaterialIf *  mat, unsigned int texid, GRRenderBaseIf::TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::CreateList(mat, texid, ty, vtx, count, stride);
	}
	virtual int CreateList(float radius, int slices, int stacks){
		return ((OBJ*)(MIF*)this)->OBJ::CreateList(radius, slices, stacks);
	}
	virtual int CreateList(GRMaterialIf *  mat, float radius, int slices, int stacks){
		return ((OBJ*)(MIF*)this)->OBJ::CreateList(mat, radius, slices, stacks);
	}
	virtual int CreateIndexedList(GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::CreateIndexedList(ty, idx, vtx, count, stride);
	}
	virtual int CreateIndexedList(GRMaterialIf *  mat, GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::CreateIndexedList(mat, ty, idx, vtx, count, stride);
	}
	virtual int CreateShaderIndexedList(GRHandler shader, void *  location, GRRenderBaseIf::TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->OBJ::CreateShaderIndexedList(shader, location, ty, idx, vtx, count, stride);
	}
	virtual void DrawList(int i){
		return ((OBJ*)(MIF*)this)->OBJ::DrawList(i);
	}
	virtual void ReleaseList(int i){
		return ((OBJ*)(MIF*)this)->OBJ::ReleaseList(i);
	}
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont &  font = 0){
		return ((OBJ*)(MIF*)this)->OBJ::DrawFont(pos, str, font);
	}
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont &  font = 0){
		return ((OBJ*)(MIF*)this)->OBJ::DrawFont(pos, str, font);
	}
	virtual void SetMaterial(const GRMaterialDesc &  mat){
		return ((OBJ*)(MIF*)this)->OBJ::SetMaterial(mat);
	}
	virtual void SetMaterial(const GRMaterialIf *  mat){
		return ((OBJ*)(MIF*)this)->OBJ::SetMaterial(mat);
	}
	virtual void SetLineWidth(float w){
		return ((OBJ*)(MIF*)this)->OBJ::SetLineWidth(w);
	}
	virtual void PushLight(const GRLightDesc &  light){
		return ((OBJ*)(MIF*)this)->OBJ::PushLight(light);
	}
	virtual void PushLight(const GRLightIf *  light){
		return ((OBJ*)(MIF*)this)->OBJ::PushLight(light);
	}
	virtual void PopLight(){
		return ((OBJ*)(MIF*)this)->OBJ::PopLight();
	}
	virtual void SetDepthWrite(bool b){
		return ((OBJ*)(MIF*)this)->OBJ::SetDepthWrite(b);
	}
	virtual void SetDepthTest(bool b){
		return ((OBJ*)(MIF*)this)->OBJ::SetDepthTest(b);
	}
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){
		return ((OBJ*)(MIF*)this)->OBJ::SetDepthFunc(f);
	}
	virtual void SetAlphaTest(bool b){
		return ((OBJ*)(MIF*)this)->OBJ::SetAlphaTest(b);
	}
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest){
		return ((OBJ*)(MIF*)this)->OBJ::SetAlphaMode(src, dest);
	}
	virtual unsigned int LoadTexture(const std::string filename){
		return ((OBJ*)(MIF*)this)->OBJ::LoadTexture(filename);
	}
	virtual void InitShader(){
		return ((OBJ*)(MIF*)this)->OBJ::InitShader();
	}
	virtual void SetShaderFormat(GRShaderFormat::ShaderType type){
		return ((OBJ*)(MIF*)this)->OBJ::SetShaderFormat(type);
	}
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler &  shader){
		return ((OBJ*)(MIF*)this)->OBJ::CreateShader(vShaderFile, fShaderFile, shader);
	}
	virtual GRHandler CreateShader(){
		return ((OBJ*)(MIF*)this)->OBJ::CreateShader();
	}
	virtual bool ReadShaderSource(GRHandler shader, std::string file){
		return ((OBJ*)(MIF*)this)->OBJ::ReadShaderSource(shader, file);
	}
	virtual void GetShaderLocation(GRHandler shader, void *  location){
		return ((OBJ*)(MIF*)this)->OBJ::GetShaderLocation(shader, location);
	}
};
struct GRRenderBaseIf;	class GRRenderBase;
typedef GRRenderBaseIfStubTemplate<GRRenderBaseIf, ObjectIfBuf, GRRenderBase>	GRRenderBaseIfStub;
typedef IfInitTemplate<GRRenderBaseIfStub, GRRenderBase>	GRRenderBaseIfInit;

template <class IF, class MIF, class OBJ> struct GRRenderBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRRenderIfStubTemplate: public GRRenderBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetDevice(GRDeviceIf *  dev){
		return ((OBJ*)(MIF*)this)->OBJ::SetDevice(dev);
	}
	virtual void SetCamera(const GRCameraDesc &  cam){
		return ((OBJ*)(MIF*)this)->OBJ::SetCamera(cam);
	}
	virtual void Reshape(Vec2f pos, Vec2f screenSize){
		return ((OBJ*)(MIF*)this)->OBJ::Reshape(pos, screenSize);
	}
};
struct GRRenderIf;	class GRRender;
typedef GRRenderIfStubTemplate<GRRenderIf, ObjectIfBuf, GRRender>	GRRenderIfStub;
typedef IfInitTemplate<GRRenderIfStub, GRRender>	GRRenderIfInit;

template <class IF, class MIF, class OBJ> struct GRRenderBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceIfStubTemplate: public GRRenderBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->OBJ::Init();
	}
	virtual void Print(std::ostream &  os) const {
		return ((OBJ*)(MIF*)this)->OBJ::Print(os);
	}
};
struct GRDeviceIf;	class GRDevice;
typedef GRDeviceIfStubTemplate<GRDeviceIf, ObjectIfBuf, GRDevice>	GRDeviceIfStub;
typedef IfInitTemplate<GRDeviceIfStub, GRDevice>	GRDeviceIfInit;

template <class IF, class MIF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceGLIfStubTemplate: public GRDeviceIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetWindow(int w){
		return ((OBJ*)(MIF*)this)->OBJ::SetWindow(w);
	}
};
struct GRDeviceGLIf;	class GRDeviceGL;
typedef GRDeviceGLIfStubTemplate<GRDeviceGLIf, ObjectIfBuf, GRDeviceGL>	GRDeviceGLIfStub;
typedef IfInitTemplate<GRDeviceGLIfStub, GRDeviceGL>	GRDeviceGLIfInit;

template <class IF, class MIF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceD3DIfStubTemplate: public GRDeviceIfStubTemplate<IF, MIF, OBJ> {
};
struct GRDeviceD3DIf;	class GRDeviceD3D;
typedef GRDeviceD3DIfStubTemplate<GRDeviceD3DIf, ObjectIfBuf, GRDeviceD3D>	GRDeviceD3DIfStub;
typedef IfInitTemplate<GRDeviceD3DIfStub, GRDeviceD3D>	GRDeviceD3DIfInit;

template <class IF, class MIF, class OBJ> struct GRRenderIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDebugRenderIfStubTemplate: public GRRenderIfStubTemplate<IF, MIF, OBJ> {
	virtual void DrawScene(PHSceneIf *  scene){
		return ((OBJ*)(MIF*)this)->OBJ::DrawScene(scene);
	}
	virtual void DrawSolid(PHSolidIf *  so){
		return ((OBJ*)(MIF*)this)->OBJ::DrawSolid(so);
	}
	virtual void DrawFace(CDFaceIf *  face, Vec3f *  base){
		return ((OBJ*)(MIF*)this)->OBJ::DrawFace(face, base);
	}
	virtual void SetMaterialSample(GRDebugRenderIf::TMaterialSample matname){
		return ((OBJ*)(MIF*)this)->OBJ::SetMaterialSample(matname);
	}
};
struct GRDebugRenderIf;	class GRDebugRender;
typedef GRDebugRenderIfStubTemplate<GRDebugRenderIf, ObjectIfBuf, GRDebugRender>	GRDebugRenderIfStub;
typedef IfInitTemplate<GRDebugRenderIfStub, GRDebugRender>	GRDebugRenderIfInit;

template <class IF, class MIF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSceneIfStubTemplate: public SceneIfStubTemplate<IF, MIF, OBJ> {
	virtual GRFrameIf *  CreateFrame(const GRFrameDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateFrame(desc);
	}
	virtual void Render(GRRenderIf *  r){
		return ((OBJ*)(MIF*)this)->OBJ::Render(r);
	}
	virtual GRFrameIf *  GetWorld(){
		return ((OBJ*)(MIF*)this)->OBJ::GetWorld();
	}
	virtual GRCameraIf *  GetCamera(){
		return ((OBJ*)(MIF*)this)->OBJ::GetCamera();
	}
	virtual GRSdkIf *  GetSdk(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSdk();
	}
};
struct GRSceneIf;	class GRScene;
typedef GRSceneIfStubTemplate<GRSceneIf, ObjectIfBuf, GRScene>	GRSceneIfStub;
typedef IfInitTemplate<GRSceneIfStub, GRScene>	GRSceneIfInit;

template <class IF, class MIF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSdkIfStubTemplate: public SdkIfStubTemplate<IF, MIF, OBJ> {
	virtual GRDebugRenderIf *  CreateDebugRender(){
		return ((OBJ*)(MIF*)this)->OBJ::CreateDebugRender();
	}
	virtual GRDeviceGLIf *  CreateDeviceGL(int window){
		return ((OBJ*)(MIF*)this)->OBJ::CreateDeviceGL(window);
	}
	virtual GRSceneIf *  CreateScene(){
		return ((OBJ*)(MIF*)this)->OBJ::CreateScene();
	}
	virtual size_t NChildObject() const {
		return ((OBJ*)(MIF*)this)->OBJ::NChildObject();
	}
	virtual ObjectIf *  GetChildObject(size_t i){
		return ((OBJ*)(MIF*)this)->OBJ::GetChildObject(i);
	}
	virtual GRSceneIf *  GetScene(size_t i){
		return ((OBJ*)(MIF*)this)->OBJ::GetScene(i);
	}
	virtual size_t NScene(){
		return ((OBJ*)(MIF*)this)->OBJ::NScene();
	}
};
struct GRSdkIf;	class GRSdk;
typedef GRSdkIfStubTemplate<GRSdkIf, ObjectIfBuf, GRSdk>	GRSdkIfStub;
typedef IfInitTemplate<GRSdkIfStub, GRSdk>	GRSdkIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSphereIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
};
struct GRSphereIf;	class GRSphere;
typedef GRSphereIfStubTemplate<GRSphereIf, ObjectIfBuf, GRSphere>	GRSphereIfStub;
typedef IfInitTemplate<GRSphereIfStub, GRSphere>	GRSphereIfInit;
}	//	namespace Spr; 
