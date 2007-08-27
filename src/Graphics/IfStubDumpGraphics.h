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
	virtual void Render(GRRenderIf * r){
		return ((OBJ*)(MIF*)this)->Render(r);
	}
	virtual void Rendered(GRRenderIf * r){
		return ((OBJ*)(MIF*)this)->Rendered(r);
	}
};
struct GRVisualIf;	class GRVisual;
typedef GRVisualIfStubTemplate<GRVisualIf, ObjectIfBuf, GRVisual>	GRVisualIfStub;
typedef IfInitTemplate<GRVisualIfStub, GRVisual>	GRVisualIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRFrameIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
	virtual GRFrameIf * GetParent(){
		return ((OBJ*)(MIF*)this)->GetParent();
	}
	virtual void SetParent(GRFrameIf * fr){
		return ((OBJ*)(MIF*)this)->SetParent(fr);
	}
	virtual int NChildren(){
		return ((OBJ*)(MIF*)this)->NChildren();
	}
	virtual GRVisualIf * * GetChildren(){
		return ((OBJ*)(MIF*)this)->GetChildren();
	}
	virtual Affinef GetTransform(){
		return ((OBJ*)(MIF*)this)->GetTransform();
	}
	virtual Affinef GetWorldTransform(){
		return ((OBJ*)(MIF*)this)->GetWorldTransform();
	}
	virtual void SetTransform(const Affinef & af){
		return ((OBJ*)(MIF*)this)->SetTransform(af);
	}
	virtual void Print(std::ostream & os) const {
		return ((OBJ*)(MIF*)this)->Print(os);
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
		return ((OBJ*)(MIF*)this)->IsOpaque();
	}
};
struct GRMaterialIf;	class GRMaterial;
typedef GRMaterialIfStubTemplate<GRMaterialIf, ObjectIfBuf, GRMaterial>	GRMaterialIfStub;
typedef IfInitTemplate<GRMaterialIfStub, GRMaterial>	GRMaterialIfInit;

template <class IF, class MIF, class OBJ> struct GRVisualIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRCameraIfStubTemplate: public GRVisualIfStubTemplate<IF, MIF, OBJ> {
	virtual GRFrameIf * GetFrame(){
		return ((OBJ*)(MIF*)this)->GetFrame();
	}
	virtual void SetFrame(GRFrameIf * fr){
		return ((OBJ*)(MIF*)this)->SetFrame(fr);
	}
};
struct GRCameraIf;	class GRCamera;
typedef GRCameraIfStubTemplate<GRCameraIf, ObjectIfBuf, GRCamera>	GRCameraIfStub;
typedef IfInitTemplate<GRCameraIfStub, GRCamera>	GRCameraIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRRenderBaseIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetViewport(Vec2f pos, Vec2f sz){
		return ((OBJ*)(MIF*)this)->SetViewport(pos, sz);
	}
	virtual void ClearBuffer(){
		return ((OBJ*)(MIF*)this)->ClearBuffer();
	}
	virtual void BeginScene(){
		return ((OBJ*)(MIF*)this)->BeginScene();
	}
	virtual void EndScene(){
		return ((OBJ*)(MIF*)this)->EndScene();
	}
	virtual void SetViewMatrix(const Affinef & afv){
		return ((OBJ*)(MIF*)this)->SetViewMatrix(afv);
	}
	virtual void SetProjectionMatrix(const Affinef & afp){
		return ((OBJ*)(MIF*)this)->SetProjectionMatrix(afp);
	}
	virtual void GetProjectionMatrix(const Affinef & afp){
		return ((OBJ*)(MIF*)this)->GetProjectionMatrix(afp);
	}
	virtual void SetModelMatrix(const Affinef & afw){
		return ((OBJ*)(MIF*)this)->SetModelMatrix(afw);
	}
	virtual void MultModelMatrix(const Affinef & afw){
		return ((OBJ*)(MIF*)this)->MultModelMatrix(afw);
	}
	virtual void PushModelMatrix(){
		return ((OBJ*)(MIF*)this)->PushModelMatrix();
	}
	virtual void PopModelMatrix(){
		return ((OBJ*)(MIF*)this)->PopModelMatrix();
	}
	virtual void ClearBlendMatrix(){
		return ((OBJ*)(MIF*)this)->ClearBlendMatrix();
	}
	virtual bool SetBlendMatrix(const Affinef & afb, unsigned int id = 0){
		return ((OBJ*)(MIF*)this)->SetBlendMatrix(afb, id);
	}
	virtual void SetVertexFormat(const GRVertexElement * e){
		return ((OBJ*)(MIF*)this)->SetVertexFormat(e);
	}
	virtual void SetVertexShader(void * shader){
		return ((OBJ*)(MIF*)this)->SetVertexShader(shader);
	}
	virtual void DrawDirect(GRRenderBaseIf::TPrimitiveType ty, void * vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->DrawDirect(ty, vtx, count, stride);
	}
	virtual void DrawIndexed(GRRenderBaseIf::TPrimitiveType ty, size_t * idx, void * vtx, size_t count, size_t stride = 0){
		return ((OBJ*)(MIF*)this)->DrawIndexed(ty, idx, vtx, count, stride);
	}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, GRVertexArray * arrays, size_t count){
		return ((OBJ*)(MIF*)this)->DrawArrays(ty, arrays, count);
	}
	virtual void DrawArrays(GRRenderBaseIf::TPrimitiveType ty, size_t * idx, GRVertexArray * arrays, size_t count){
		return ((OBJ*)(MIF*)this)->DrawArrays(ty, idx, arrays, count);
	}
	virtual void DrawSphere(float radius, int slices, int stacks){
		return ((OBJ*)(MIF*)this)->DrawSphere(radius, slices, stacks);
	}
	virtual int StartList(){
		return ((OBJ*)(MIF*)this)->StartList();
	}
	virtual void EndList(){
		return ((OBJ*)(MIF*)this)->EndList();
	}
	virtual void DrawList(int i){
		return ((OBJ*)(MIF*)this)->DrawList(i);
	}
	virtual void ReleaseList(int i){
		return ((OBJ*)(MIF*)this)->ReleaseList(i);
	}
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont & font = 0){
		return ((OBJ*)(MIF*)this)->DrawFont(pos, str, font);
	}
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont & font = 0){
		return ((OBJ*)(MIF*)this)->DrawFont(pos, str, font);
	}
	virtual void SetMaterial(const GRMaterialDesc & mat){
		return ((OBJ*)(MIF*)this)->SetMaterial(mat);
	}
	virtual void SetMaterial(const GRMaterialIf * mat){
		return ((OBJ*)(MIF*)this)->SetMaterial(mat);
	}
	virtual void SetLineWidth(float w){
		return ((OBJ*)(MIF*)this)->SetLineWidth(w);
	}
	virtual void PushLight(const GRLightDesc & light){
		return ((OBJ*)(MIF*)this)->PushLight(light);
	}
	virtual void PushLight(const GRLightIf * light){
		return ((OBJ*)(MIF*)this)->PushLight(light);
	}
	virtual void PopLight(){
		return ((OBJ*)(MIF*)this)->PopLight();
	}
	virtual void SetDepthWrite(bool b){
		return ((OBJ*)(MIF*)this)->SetDepthWrite(b);
	}
	virtual void SetDepthTest(bool b){
		return ((OBJ*)(MIF*)this)->SetDepthTest(b);
	}
	virtual void SetDepthFunc(GRRenderBaseIf::TDepthFunc f){
		return ((OBJ*)(MIF*)this)->SetDepthFunc(f);
	}
	virtual void SetAlphaTest(bool b){
		return ((OBJ*)(MIF*)this)->SetAlphaTest(b);
	}
	virtual void SetAlphaMode(GRRenderBaseIf::TBlendFunc src, GRRenderBaseIf::TBlendFunc dest){
		return ((OBJ*)(MIF*)this)->SetAlphaMode(src, dest);
	}
	virtual unsigned int LoadTexture(const std::string filename){
		return ((OBJ*)(MIF*)this)->LoadTexture(filename);
	}
	virtual void InitShader(){
		return ((OBJ*)(MIF*)this)->InitShader();
	}
	virtual void SetShaderFormat(GRShaderFormat::ShaderType type){
		return ((OBJ*)(MIF*)this)->SetShaderFormat(type);
	}
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler & shader){
		return ((OBJ*)(MIF*)this)->CreateShader(vShaderFile, fShaderFile, shader);
	}
	virtual GRHandler CreateShader(){
		return ((OBJ*)(MIF*)this)->CreateShader();
	}
	virtual bool ReadShaderSource(GRHandler shader, std::string file){
		return ((OBJ*)(MIF*)this)->ReadShaderSource(shader, file);
	}
	virtual void GetShaderLocation(GRHandler shader, void * location){
		return ((OBJ*)(MIF*)this)->GetShaderLocation(shader, location);
	}
};
struct GRRenderBaseIf;	class GRRenderBase;
typedef GRRenderBaseIfStubTemplate<GRRenderBaseIf, ObjectIfBuf, GRRenderBase>	GRRenderBaseIfStub;
typedef IfInitTemplate<GRRenderBaseIfStub, GRRenderBase>	GRRenderBaseIfInit;

template <class IF, class MIF, class OBJ> struct GRRenderBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRRenderIfStubTemplate: public GRRenderBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetDevice(GRDeviceIf * dev){
		return ((OBJ*)(MIF*)this)->SetDevice(dev);
	}
	virtual void SetCamera(const GRCameraDesc & cam){
		return ((OBJ*)(MIF*)this)->SetCamera(cam);
	}
	virtual void Reshape(Vec2f pos, Vec2f screenSize){
		return ((OBJ*)(MIF*)this)->Reshape(pos, screenSize);
	}
};
struct GRRenderIf;	class GRRender;
typedef GRRenderIfStubTemplate<GRRenderIf, ObjectIfBuf, GRRender>	GRRenderIfStub;
typedef IfInitTemplate<GRRenderIfStub, GRRender>	GRRenderIfInit;

template <class IF, class MIF, class OBJ> struct GRRenderBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceIfStubTemplate: public GRRenderBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual void Print(std::ostream & os) const {
		return ((OBJ*)(MIF*)this)->Print(os);
	}
};
struct GRDeviceIf;	class GRDevice;
typedef GRDeviceIfStubTemplate<GRDeviceIf, ObjectIfBuf, GRDevice>	GRDeviceIfStub;
typedef IfInitTemplate<GRDeviceIfStub, GRDevice>	GRDeviceIfInit;

template <class IF, class MIF, class OBJ> struct GRDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDeviceGLIfStubTemplate: public GRDeviceIfStubTemplate<IF, MIF, OBJ> {
};
struct GRDeviceGLIf;	class GRDeviceGL;
typedef GRDeviceGLIfStubTemplate<GRDeviceGLIf, ObjectIfBuf, GRDeviceGL>	GRDeviceGLIfStub;
typedef IfInitTemplate<GRDeviceGLIfStub, GRDeviceGL>	GRDeviceGLIfInit;

template <class IF, class MIF, class OBJ> struct GRRenderIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRDebugRenderIfStubTemplate: public GRRenderIfStubTemplate<IF, MIF, OBJ> {
	virtual void DrawScene(PHSceneIf * scene){
		return ((OBJ*)(MIF*)this)->DrawScene(scene);
	}
	virtual void DrawSolid(PHSolidIf * so){
		return ((OBJ*)(MIF*)this)->DrawSolid(so);
	}
	virtual void DrawFace(CDFaceIf * face, Vec3f * base){
		return ((OBJ*)(MIF*)this)->DrawFace(face, base);
	}
	virtual void SetMaterialSample(GRDebugRenderIf::TMaterialSample matname){
		return ((OBJ*)(MIF*)this)->SetMaterialSample(matname);
	}
};
struct GRDebugRenderIf;	class GRDebugRender;
typedef GRDebugRenderIfStubTemplate<GRDebugRenderIf, ObjectIfBuf, GRDebugRender>	GRDebugRenderIfStub;
typedef IfInitTemplate<GRDebugRenderIfStub, GRDebugRender>	GRDebugRenderIfInit;

template <class IF, class MIF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSceneIfStubTemplate: public SceneIfStubTemplate<IF, MIF, OBJ> {
	virtual void Render(GRRenderIf * r){
		return ((OBJ*)(MIF*)this)->Render(r);
	}
	virtual GRFrameIf * GetWorld(){
		return ((OBJ*)(MIF*)this)->GetWorld();
	}
	virtual GRCameraIf * GetCamera(){
		return ((OBJ*)(MIF*)this)->GetCamera();
	}
	virtual void SetCamera(const GRCameraDesc & desc){
		return ((OBJ*)(MIF*)this)->SetCamera(desc);
	}
	virtual GRVisualIf * CreateVisual(const IfInfo * info, const GRVisualDesc & desc, GRFrameIf * parent = NULL){
		return ((OBJ*)(MIF*)this)->CreateVisual(info, desc, parent);
	}
	virtual GRSdkIf * GetSdk(){
		return ((OBJ*)(MIF*)this)->GetSdk();
	}
};
struct GRSceneIf;	class GRScene;
typedef GRSceneIfStubTemplate<GRSceneIf, ObjectIfBuf, GRScene>	GRSceneIfStub;
typedef IfInitTemplate<GRSceneIfStub, GRScene>	GRSceneIfInit;

template <class IF, class MIF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct GRSdkIfStubTemplate: public SdkIfStubTemplate<IF, MIF, OBJ> {
	virtual GRDebugRenderIf * CreateDebugRender(){
		return ((OBJ*)(MIF*)this)->CreateDebugRender();
	}
	virtual GRDeviceGLIf * CreateDeviceGL(){
		return ((OBJ*)(MIF*)this)->CreateDeviceGL();
	}
	virtual GRSceneIf * CreateScene(const GRSceneDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateScene(desc);
	}
	virtual GRSceneIf * GetScene(size_t i){
		return ((OBJ*)(MIF*)this)->GetScene(i);
	}
	virtual size_t NScene(){
		return ((OBJ*)(MIF*)this)->NScene();
	}
	virtual void MergeScene(GRSceneIf * scene0, GRSceneIf * scene1){
		return ((OBJ*)(MIF*)this)->MergeScene(scene0, scene1);
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
