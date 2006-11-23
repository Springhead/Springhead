//	Do not edit. MakeTypeDesc.bat will update this file.

template <class IF, class OBJ>
struct GRBlendMeshIfStub: public GRVisualIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct GRVisualIfStub: public NamedObjectIfStub<IF, OBJ> {
	virtual void Render(GRRenderIf *  r){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Render(r);
	}
	virtual void Rendered(GRRenderIf *  r){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Rendered(r);
	}
};

template <class IF, class OBJ>
struct GRFrameIfStub: public GRVisualIfStub<IF, OBJ> {
	virtual GRFrameIf *  GetParent(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetParent();
	}
	virtual void SetParent(GRFrameIf *  fr){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetParent(fr);
	}
	virtual Affinef GetTransform(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetTransform();
	}
	virtual Affinef GetWorldTransform(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetWorldTransform();
	}
	virtual void SetTransform(Affinef &  af){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetTransform(af);
	}
	virtual void Print(std::ostream &  os) const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Print(os);
	}
};

template <class IF, class OBJ>
struct GRMeshIfStub: public GRVisualIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct GRLightIfStub: public GRVisualIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct GRMaterialIfStub: public GRVisualIfStub<IF, OBJ> {
	virtual bool IsOpaque() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::IsOpaque();
	}
};

template <class IF, class OBJ>
struct GRCameraIfStub: public GRVisualIfStub<IF, OBJ> {
	virtual GRFrameIf *  GetFrame(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetFrame();
	}
	virtual void SetFrame(GRFrameIf *  fr){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetFrame(fr);
	}
};

template <class IF, class OBJ>
struct GRRenderBaseIfStub: public ObjectIfStub<IF, OBJ> {
	virtual void SetViewport(Vec2f pos, Vec2f sz){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetViewport(pos, sz);
	}
	virtual void ClearBuffer(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::ClearBuffer();
	}
	virtual void BeginScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::BeginScene();
	}
	virtual void EndScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::EndScene();
	}
	virtual void SetViewMatrix(const Affinef &  afv){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetViewMatrix(afv);
	}
	virtual void SetProjectionMatrix(const Affinef &  afp){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetProjectionMatrix(afp);
	}
	virtual void SetModelMatrix(const Affinef &  afw){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetModelMatrix(afw);
	}
	virtual void MultModelMatrix(const Affinef &  afw){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::MultModelMatrix(afw);
	}
	virtual void PushModelMatrix(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PushModelMatrix();
	}
	virtual void PopModelMatrix(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PopModelMatrix();
	}
	virtual bool SetModelMatrices(const Affinef &  afw, unsigned int matrixId, unsigned int elementId){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetModelMatrices(afw, matrixId, elementId);
	}
	virtual bool MultModelMatrices(const Affinef &  afw, unsigned int matrixId){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::MultModelMatrices(afw, matrixId);
	}
	virtual bool PushModelMatrices(const Affinef &  afw, unsigned int matrixId){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PushModelMatrices(afw, matrixId);
	}
	virtual bool PopModelMatrices(unsigned int matrixId){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PopModelMatrices(matrixId);
	}
	virtual void SetVertexFormat(const GRVertexElement *  e){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetVertexFormat(e);
	}
	virtual void SetVertexShader(void *  shader){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetVertexShader(shader);
	}
	virtual void DrawDirect(TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawDirect(ty, vtx, count, stride);
	}
	virtual void DrawIndexed(TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawIndexed(ty, idx, vtx, count, stride);
	}
	virtual void DrawArrays(TPrimitiveType ty, GRVertexArray *  arrays, size_t count){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawArrays(ty, arrays, count);
	}
	virtual void DrawArrays(TPrimitiveType ty, size_t *  idx, GRVertexArray *  arrays, size_t count){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawArrays(ty, idx, arrays, count);
	}
	virtual int CreateList(TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(ty, vtx, count, stride);
	}
	virtual int CreateList(GRMaterialIf *  mat, unsigned int texid, TPrimitiveType ty, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(mat, texid, ty, vtx, count, stride);
	}
	virtual int CreateList(float radius, int slices, int stacks){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(radius, slices, stacks);
	}
	virtual int CreateList(GRMaterialIf *  mat, float radius, int slices, int stacks){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateList(mat, radius, slices, stacks);
	}
	virtual int CreateIndexedList(TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateIndexedList(ty, idx, vtx, count, stride);
	}
	virtual int CreateIndexedList(GRMaterialIf *  mat, TPrimitiveType ty, size_t *  idx, void *  vtx, size_t count, size_t stride = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateIndexedList(mat, ty, idx, vtx, count, stride);
	}
	virtual void DrawList(int i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawList(i);
	}
	virtual void ReleaseList(int i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::ReleaseList(i);
	}
	virtual void DrawFont(Vec2f pos, const std::string str, const GRFont &  font = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawFont(pos, str, font);
	}
	virtual void DrawFont(Vec3f pos, const std::string str, const GRFont &  font = 0){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawFont(pos, str, font);
	}
	virtual void SetMaterial(const GRMaterialDesc &  mat){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaterial(mat);
	}
	virtual void SetMaterial(const GRMaterialIf *  mat){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaterial(mat);
	}
	virtual void SetLineWidth(float w){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetLineWidth(w);
	}
	virtual void PushLight(const GRLightDesc &  light){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PushLight(light);
	}
	virtual void PushLight(const GRLightIf *  light){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PushLight(light);
	}
	virtual void PopLight(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::PopLight();
	}
	virtual void SetDepthWrite(bool b){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDepthWrite(b);
	}
	virtual void SetDepthTest(bool b){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDepthTest(b);
	}
	virtual void SetDepthFunc(TDepthFunc f){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDepthFunc(f);
	}
	virtual void SetAlphaTest(bool b){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetAlphaTest(b);
	}
	virtual void SetAlphaMode(TBlendFunc src, TBlendFunc dest){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetAlphaMode(src, dest);
	}
	virtual unsigned int LoadTexture(const std::string filename){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::LoadTexture(filename);
	}
	virtual void InitShader(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::InitShader();
	}
	virtual bool CreateShader(std::string vShaderFile, std::string fShaderFile, GRHandler &  shaderProgram){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShader(vShaderFile, fShaderFile, shaderProgram);
	}
	virtual bool CreateShader(std::string vShaderFile, GRHandler &  shaderProgram){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShader(vShaderFile, shaderProgram);
	}
	virtual bool ReadShaderSource(GRHandler shader, std::string file){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::ReadShaderSource(shader, file);
	}
};

template <class IF, class OBJ>
struct GRRenderIfStub: public GRRenderBaseIfStub<IF, OBJ> {
	virtual void SetDevice(GRDeviceIf *  dev){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDevice(dev);
	}
	virtual void SetCamera(const GRCameraDesc &  cam){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetCamera(cam);
	}
	virtual void Reshape(Vec2f pos, Vec2f screenSize){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Reshape(pos, screenSize);
	}
};

template <class IF, class OBJ>
struct GRDeviceIfStub: public GRRenderBaseIfStub<IF, OBJ> {
	virtual void Init(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Init();
	}
	virtual void Print(std::ostream &  os) const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Print(os);
	}
};

template <class IF, class OBJ>
struct GRDeviceGLIfStub: public GRDeviceIfStub<IF, OBJ> {
	virtual void SetWindow(int w){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetWindow(w);
	}
};

template <class IF, class OBJ>
struct GRDeviceD3DIfStub: public GRDeviceIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct GRDebugRenderIfStub: public GRRenderIfStub<IF, OBJ> {
	virtual void DrawScene(PHSceneIf *  scene){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawScene(scene);
	}
	virtual void DrawSolid(PHSolidIf *  so){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawSolid(so);
	}
	virtual void DrawFace(CDFaceIf *  face, Vec3f *  base){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::DrawFace(face, base);
	}
	virtual void SetMaterialSample(MaterialSample matname){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaterialSample(matname);
	}
};

template <class IF, class OBJ>
struct GRSceneIfStub: public SceneIfStub<IF, OBJ> {
	virtual GRFrameIf *  CreateFrame(const GRFrameDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateFrame(desc);
	}
	virtual void Render(GRRenderIf *  r){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Render(r);
	}
	virtual GRFrameIf *  GetWorld(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetWorld();
	}
	virtual GRCameraIf *  GetCamera(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetCamera();
	}
	virtual GRSdkIf *  GetSdk(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetSdk();
	}
};

template <class IF, class OBJ>
struct GRSdkIfStub: public SdkIfStub<IF, OBJ> {
	virtual GRDebugRenderIf *  CreateDebugRender(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateDebugRender();
	}
	virtual GRDeviceGLIf *  CreateDeviceGL(int window){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateDeviceGL(window);
	}
	virtual GRSceneIf *  CreateScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene();
	}
	virtual size_t NChildObject() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NChildObject();
	}
	virtual ObjectIf *  GetChildObject(size_t i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetChildObject(i);
	}
	virtual GRSceneIf *  GetScene(size_t i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene(i);
	}
	virtual size_t NScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NScene();
	}
};

template <class IF, class OBJ>
struct GRSphereIfStub: public GRVisualIfStub<IF, OBJ> {
};
