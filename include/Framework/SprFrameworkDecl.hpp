#define SPR_OVERRIDEMEMBERFUNCOF_FWBoneIf(base)	\
	Spr::PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(Spr::PHSolidIf* s){	base::SetPHSolid(s);}	\
	Spr::GRFrameIf* GetGRFrame(){	return	base::GetGRFrame();}	\
	void SetGRFrame(Spr::GRFrameIf* f){	base::SetGRFrame(f);}	\
	Spr::CDRoundConeIf* GetShape(){	return	base::GetShape();}	\
	void SetShapeRadius(double r1, double r2){	base::SetShapeRadius(r1, r2);}	\
	Spr::PHJointIf* GetJoint(){	return	base::GetJoint();}	\
	void SetJointKDD2(double K, double D, double D2){	base::SetJointKDD2(K, D, D2);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWFemMeshIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWGraphicsAdapteeIf(base)	\
	void Init(int argc, char** argv){	base::Init(argc, argv);}	\
	void StartMainLoop(){	base::StartMainLoop();}	\
	void EnableIdleFunc(bool on){	base::EnableIdleFunc(on);}	\
	void EnterGameMode(){	base::EnterGameMode();}	\
	void LeaveGameMode(){	base::LeaveGameMode();}	\
	FWWinIf* CreateWin(const FWWinDesc& d){	return	base::CreateWin(d);}	\
	void DestroyWin(FWWinIf* w){	base::DestroyWin(w);}	\
	void SetCurrentWin(FWWinIf* w){	base::SetCurrentWin(w);}	\
	FWWinIf* GetCurrentWin(){	return	base::GetCurrentWin();}	\
	int GetWinFromId(){	return	base::GetWinFromId();}	\
	void PostRedisplay(){	base::PostRedisplay();}	\
	int GetModifiers(){	return	base::GetModifiers();}	\
	Spr::GRDeviceIf* GetGRDevice(){	return	base::GetGRDevice();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWInteractPointerIf(base)	\
	void SetPointerSolid(Spr::PHSolidIf* solid){	base::SetPointerSolid(solid);}	\
	Spr::PHSolidIf* GetPointerSolid(){	return	base::GetPointerSolid();}	\
	void SetHI(Spr::HIBaseIf* hi){	base::SetHI(hi);}	\
	Spr::HIBaseIf* GetHI(){	return	base::GetHI();}	\
	void SetPosScale(double s){	base::SetPosScale(s);}	\
	double GetPosScale(){	return	base::GetPosScale();}	\
	void SetForceScale(double s){	base::SetForceScale(s);}	\
	double GetForceScale(){	return	base::GetForceScale();}	\
	void SetTorqueScale(double s){	base::SetTorqueScale(s);}	\
	double GetTorqueScale(){	return	base::GetTorqueScale();}	\
	void SetLocalRange(double r){	base::SetLocalRange(r);}	\
	double GetLocalRange(){	return	base::GetLocalRange();}	\
	void SetPointersCalibPosition(Posed p){	base::SetPointersCalibPosition(p);}	\
	Posed GetPointersCalibPosition(){	return	base::GetPointersCalibPosition();}	\
	void SetDefaultPosition(Posed p){	base::SetDefaultPosition(p);}	\
	Posed GetDefaultPosition(){	return	base::GetDefaultPosition();}	\
	void SetSpringCoeff(double k){	base::SetSpringCoeff(k);}	\
	double GetSpringCoeff(){	return	base::GetSpringCoeff();}	\
	void SetDamperCoeff(double d){	base::SetDamperCoeff(d);}	\
	double GetDamperCoeff(){	return	base::GetDamperCoeff();}	\
	void EnableForce(bool b){	base::EnableForce(b);}	\
	void EnableVibration(bool b){	base::EnableVibration(b);}	\
	void Calibration(){	base::Calibration();}	\
	void SetGrabFlag(int f){	base::SetGrabFlag(f);}	\
	int GetGrabFlag(){	return	base::GetGrabFlag();}	\
	void SetCameraOri(Quaterniond q){	base::SetCameraOri(q);}	\
	Quaterniond GetCameraOri(){	return	base::GetCameraOri();}	\
	void SetSpringK(double k){	base::SetSpringK(k);}	\
	double GetSpringK(){	return	base::GetSpringK();}	\
	void SetDamperD(double d){	base::SetDamperD(d);}	\
	double GetDamperD(){	return	base::GetDamperD();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWInteractSceneIf(base)	\
	Spr::FWSceneIf* GetScene(){	return	base::GetScene();}	\
	void CreateIAAdaptee(Spr::FWInteractMode mode){	base::CreateIAAdaptee(mode);}	\
	Spr::FWInteractAdaptee* GetIAAdaptee(){	return	base::GetIAAdaptee();}	\
	Spr::FWInteractPointerIf* CreateIAPointer(const Spr::FWInteractPointerDesc& desc){	return	base::CreateIAPointer(desc);}	\
	Spr::FWInteractPointerIf* GetIAPointer(int i){	return	base::GetIAPointer(i);}	\
	int NIAPointers(){	return	base::NIAPointers();}	\
	Spr::FWInteractMode GetIAMode(){	return	base::GetIAMode();}	\
	Spr::FWHapticMode GetHMode(){	return	base::GetHMode();}	\
	void SetHMode(Spr::FWHapticMode hMode){	base::SetHMode(hMode);}	\
	void Init(){	base::Init();}	\
	void Clear(){	base::Clear();}	\
	void Step(){	base::Step();}	\
	void CallBackHapticLoop(){	base::CallBackHapticLoop();}	\
	bool GetContactFlag(){	return	base::GetContactFlag();}	\
	Vec3d GetPicCenterPosition(){	return	base::GetPicCenterPosition();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWObjectIf(base)	\
	Spr::PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(Spr::PHSolidIf* s){	base::SetPHSolid(s);}	\
	Spr::GRFrameIf* GetGRFrame(){	return	base::GetGRFrame();}	\
	void SetGRFrame(Spr::GRFrameIf* f){	base::SetGRFrame(f);}	\
	bool LoadMesh(const char* filename, const IfInfo* ii, Spr::GRFrameIf* frame){	return	base::LoadMesh(filename, ii, frame);}	\
	void GenerateCDMesh(Spr::GRFrameIf* frame, const PHMaterial& mat){	base::GenerateCDMesh(frame, mat);}	\
	void Sync(bool ph_to_gr){	base::Sync(ph_to_gr);}	\
	double GetSolidLength(){	return	base::GetSolidLength();}	\
	void SetSolidLength(double l){	base::SetSolidLength(l);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWBoneObjectIf(base)	\
	Spr::PHJointIf* GetPHJoint(){	return	base::GetPHJoint();}	\
	void SetPHJoint(Spr::PHJointIf* j){	base::SetPHJoint(j);}	\
	Spr::GRFrameIf* GetEndFrame(){	return	base::GetEndFrame();}	\
	void SetEndFrame(Spr::GRFrameIf* f){	base::SetEndFrame(f);}	\
	void SetAdaptType(int t){	base::SetAdaptType(t);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWStructureIf(base)	\
	Spr::FWBoneObjectIf* GetBone(int n){	return	base::GetBone(n);}	\
	Spr::FWBoneObjectIf* GetBone(char* n){	return	base::GetBone(n);}	\
	int GetBoneSize(){	return	base::GetBoneSize();}	\
	void SetPose(Posed p){	base::SetPose(p);}	\
	void AddBone(Spr::FWBoneObjectIf* o){	base::AddBone(o);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWSceneIf(base)	\
	Spr::PHSceneIf* GetPHScene(){	return	base::GetPHScene();}	\
	void SetPHScene(Spr::PHSceneIf* s){	base::SetPHScene(s);}	\
	Spr::GRSceneIf* GetGRScene(){	return	base::GetGRScene();}	\
	void SetGRScene(Spr::GRSceneIf* s){	base::SetGRScene(s);}	\
	Spr::FWObjectIf* CreateFWObject(){	return	base::CreateFWObject();}	\
	int NObject()const{	return	base::NObject();}	\
	Spr::FWObjectIf** GetObjects(){	return	base::GetObjects();}	\
	void Sync(bool ph_to_gr){	base::Sync(ph_to_gr);}	\
	void Step(){	base::Step();}	\
	void Draw(Spr::GRRenderIf* grRender, bool debug){	base::Draw(grRender, debug);}	\
	void DrawPHScene(Spr::GRRenderIf* render){	base::DrawPHScene(render);}	\
	void DrawSolid(Spr::GRRenderIf* render, Spr::PHSolidIf* solid, bool solid_or_wire){	base::DrawSolid(render, solid, solid_or_wire);}	\
	void DrawShape(Spr::GRRenderIf* render, Spr::CDShapeIf* shape, bool solid){	base::DrawShape(render, shape, solid);}	\
	void DrawConstraint(Spr::GRRenderIf* render, Spr::PHConstraintIf* con){	base::DrawConstraint(render, con);}	\
	void DrawContact(Spr::GRRenderIf* render, Spr::PHContactPointIf* con){	base::DrawContact(render, con);}	\
	void DrawIK(Spr::GRRenderIf* render, Spr::PHIKEngineIf* ikEngine){	base::DrawIK(render, ikEngine);}	\
	void SetRenderMode(bool solid, bool wire){	base::SetRenderMode(solid, wire);}	\
	void EnableRender(Spr::ObjectIf* obj, bool enable){	base::EnableRender(obj, enable);}	\
	void SetSolidMaterial(int mat, Spr::PHSolidIf* solid){	base::SetSolidMaterial(mat, solid);}	\
	void SetWireMaterial(int mat, Spr::PHSolidIf* solid){	base::SetWireMaterial(mat, solid);}	\
	void EnableRenderAxis(bool world, bool solid, bool con){	base::EnableRenderAxis(world, solid, con);}	\
	void SetAxisMaterial(int matX, int matY, int matZ){	base::SetAxisMaterial(matX, matY, matZ);}	\
	void SetAxisScale(float scaleWorld, float scaleSolid, float scaleCon){	base::SetAxisScale(scaleWorld, scaleSolid, scaleCon);}	\
	void SetAxisStyle(int style){	base::SetAxisStyle(style);}	\
	void EnableRenderForce(bool solid, bool constraint){	base::EnableRenderForce(solid, constraint);}	\
	void SetForceMaterial(int matForce, int matMoment){	base::SetForceMaterial(matForce, matMoment);}	\
	void SetForceScale(float scaleForce, float scaleMoment){	base::SetForceScale(scaleForce, scaleMoment);}	\
	void EnableRenderContact(bool enable){	base::EnableRenderContact(enable);}	\
	void SetContactMaterial(int mat){	base::SetContactMaterial(mat);}	\
	void EnableRenderGrid(bool x, bool y, bool z){	base::EnableRenderGrid(x, y, z);}	\
	void SetGridOption(char axis, float offset, float size, int slice){	base::SetGridOption(axis, offset, size, slice);}	\
	void SetGridMaterial(int matX, int matY, int matZ){	base::SetGridMaterial(matX, matY, matZ);}	\
	void EnableRenderIK(bool enable){	base::EnableRenderIK(enable);}	\
	void SetIKMaterial(int mat){	base::SetIKMaterial(mat);}	\
	void SetIKScale(float scale){	base::SetIKScale(scale);}	\
	void AddHumanInterface(Spr::HIForceDevice6D* d){	base::AddHumanInterface(d);}	\
	void SetFWBones(Spr::FWBoneIf* b){	base::SetFWBones(b);}	\
	std::vector< UTRef< Spr::FWBoneIf > > GetFWBones(){	return	base::GetFWBones();}	\
	void CreateFWStructure(){	base::CreateFWStructure();}	\
	void AddFWStructure(Spr::FWStructureIf* o){	base::AddFWStructure(o);}	\
	Spr::FWStructureIf* GetFWStructure(){	return	base::GetFWStructure();}	\
	Spr::FWStructureIf* GetFWStructure(int n){	return	base::GetFWStructure(n);}	\
	size_t NFWStructure(){	return	base::NFWStructure();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWSdkIf(base)	\
	Spr::FWSceneIf* CreateScene(const PHSceneDesc& phdesc, const GRSceneDesc& grdesc){	return	base::CreateScene(phdesc, grdesc);}	\
	bool LoadScene(UTString filename, Spr::ImportIf* ex, const IfInfo* ii, Spr::ObjectIfs* objs){	return	base::LoadScene(filename, ex, ii, objs);}	\
	bool SaveScene(UTString filename, Spr::ImportIf* ex, const IfInfo* ii, Spr::ObjectIfs* objs){	return	base::SaveScene(filename, ex, ii, objs);}	\
	int NScene()const{	return	base::NScene();}	\
	Spr::FWSceneIf* GetScene(int index){	return	base::GetScene(index);}	\
	void MergeScene(Spr::FWSceneIf* scene0, Spr::FWSceneIf* scene1){	base::MergeScene(scene0, scene1);}	\
	Spr::FWInteractSceneIf* CreateIAScene(const Spr::FWInteractSceneDesc& desc){	return	base::CreateIAScene(desc);}	\
	Spr::FWInteractSceneIf* GetIAScene(int index){	return	base::GetIAScene(index);}	\
	int NIAScenes(){	return	base::NIAScenes();}	\
	void ClearIAScenes(){	base::ClearIAScenes();}	\
	PHSdkIf* GetPHSdk(){	return	base::GetPHSdk();}	\
	GRSdkIf* GetGRSdk(){	return	base::GetGRSdk();}	\
	Spr::FISdkIf* GetFISdk(){	return	base::GetFISdk();}	\
	Spr::HISdkIf* GetHISdk(){	return	base::GetHISdk();}	\
	bool GetDebugMode(){	return	base::GetDebugMode();}	\
	void SetDebugMode(bool debug){	base::SetDebugMode(debug);}	\
	void Step(){	base::Step();}	\
	void Draw(){	base::Draw();}	\
	void Reshape(int w, int h){	base::Reshape(w, h);}	\
	Spr::GRRenderIf* GetRender(){	return	base::GetRender();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWWinIf(base)	\
	void SetPosition(int left, int top){	base::SetPosition(left, top);}	\
	void SetSize(int width, int height){	base::SetSize(width, height);}	\
	void SetTitle(UTString title){	base::SetTitle(title);}	\
	UTString GetTitle(){	return	base::GetTitle();}	\
	void SetFullScreen(bool full){	base::SetFullScreen(full);}	\
	bool GetFullScreen(){	return	base::GetFullScreen();}	\
	Vec2i GetPosition(){	return	base::GetPosition();}	\
	int GetWidth(){	return	base::GetWidth();}	\
	int GetHeight(){	return	base::GetHeight();}	\
	int GetID(){	return	base::GetID();}	\
	void SetID(int newID){	base::SetID(newID);}	\
	Spr::GRRenderIf* GetRender(){	return	base::GetRender();}	\
	void SetRender(Spr::GRRenderIf* data){	base::SetRender(data);}	\
	Spr::FWSceneIf* GetScene(){	return	base::GetScene();}	\
	void SetScene(Spr::FWSceneIf* s){	base::SetScene(s);}	\
	Spr::DVKeyMouseIf* GetKeyMouse(){	return	base::GetKeyMouse();}	\
	void SetKeyMouse(Spr::DVKeyMouseIf* dv){	base::SetKeyMouse(dv);}	\
	Spr::DVJoyStickIf* GetJoyStick(){	return	base::GetJoyStick();}	\
	void SetJoyStick(Spr::DVJoyStickIf* dv){	base::SetJoyStick(dv);}	\
	Spr::HITrackballIf* GetTrackball(){	return	base::GetTrackball();}	\
	void SetTrackball(Spr::HITrackballIf* dv){	base::SetTrackball(dv);}	\
	Spr::HIDraggerIf* GetDragger(){	return	base::GetDragger();}	\
	void SetDragger(Spr::HIDraggerIf* dv){	base::SetDragger(dv);}	\
	void SetDebugMode(bool ph_or_gr){	base::SetDebugMode(ph_or_gr);}	\
	bool GetDebugMode(){	return	base::GetDebugMode();}	\
	void Display(){	base::Display();}	\
	void Reshape(int w, int h){	base::Reshape(w, h);}	\

