#define SPR_OVERRIDEMEMBERFUNCOF_FWBoneIf(base)	\
	PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(PHSolidIf* s){	base::SetPHSolid(s);}	\
	GRFrameIf* GetGRFrame(){	return	base::GetGRFrame();}	\
	void SetGRFrame(GRFrameIf* f){	base::SetGRFrame(f);}	\
	CDRoundConeIf* GetShape(){	return	base::GetShape();}	\
	void SetShapeRadius(double r1, double r2){	base::SetShapeRadius(r1, r2);}	\
	PHJointIf* GetJoint(){	return	base::GetJoint();}	\
	void SetJointKDD2(double K, double D, double D2){	base::SetJointKDD2(K, D, D2);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWInteractPointerIf(base)	\
	void SetPointerSolid(PHSolidIf* solid){	base::SetPointerSolid(solid);}	\
	PHSolidIf* GetPointerSolid(){	return	base::GetPointerSolid();}	\
	void SetHI(HIBaseIf* hi){	base::SetHI(hi);}	\
	HIBaseIf* GetHI(){	return	base::GetHI();}	\
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
	Vec3d* GetProxyPoint(){	return	base::GetProxyPoint();}	\
	Vec3d* GetForce(){	return	base::GetForce();}	\
	bool GetContactFlag(){	return	base::GetContactFlag();}	\
	Vec3d GetPicCenterPosition(){	return	base::GetPicCenterPosition();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWObjectIf(base)	\
	PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(PHSolidIf* s){	base::SetPHSolid(s);}	\
	GRFrameIf* GetGRFrame(){	return	base::GetGRFrame();}	\
	void SetGRFrame(GRFrameIf* f){	base::SetGRFrame(f);}	\
	double GetSolidLength(){	return	base::GetSolidLength();}	\
	void SetSolidLength(double l){	base::SetSolidLength(l);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWBoneObjectIf(base)	\
	PHJointIf* GetPHJoint(){	return	base::GetPHJoint();}	\
	void SetPHJoint(PHJointIf* j){	base::SetPHJoint(j);}	\
	GRFrameIf* GetEndFrame(){	return	base::GetEndFrame();}	\
	void SetEndFrame(GRFrameIf* f){	base::SetEndFrame(f);}	\
	void SetAdaptType(int t){	base::SetAdaptType(t);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWStructureIf(base)	\
	Spr::FWBoneObjectIf* GetBone(int n){	return	base::GetBone(n);}	\
	Spr::FWBoneObjectIf* GetBone(char* n){	return	base::GetBone(n);}	\
	int GetBoneSize(){	return	base::GetBoneSize();}	\
	void SetPose(Posed p){	base::SetPose(p);}	\
	void AddBone(Spr::FWBoneObjectIf* o){	base::AddBone(o);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FWSceneIf(base)	\
	PHSceneIf* GetPHScene(){	return	base::GetPHScene();}	\
	void SetPHScene(PHSceneIf* s){	base::SetPHScene(s);}	\
	GRSceneIf* GetGRScene(){	return	base::GetGRScene();}	\
	void SetGRScene(GRSceneIf* s){	base::SetGRScene(s);}	\
	Spr::FWObjectIf* CreateFWObject(){	return	base::CreateFWObject();}	\
	int NObject()const{	return	base::NObject();}	\
	Spr::FWObjectIf** GetObjects(){	return	base::GetObjects();}	\
	void Sync(){	base::Sync();}	\
	void Step(){	base::Step();}	\
	void Draw(GRRenderIf* grRender, bool debug){	base::Draw(grRender, debug);}	\
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
	bool LoadScene(UTString filename, const IfInfo* ii){	return	base::LoadScene(filename, ii);}	\
	bool SaveScene(UTString filename, const IfInfo* ii){	return	base::SaveScene(filename, ii);}	\
	int NScene()const{	return	base::NScene();}	\
	void SwitchScene(Spr::FWSceneIf* scene){	base::SwitchScene(scene);}	\
	Spr::FWSceneIf* GetScene(int index){	return	base::GetScene(index);}	\
	void MergeScene(Spr::FWSceneIf* scene0, Spr::FWSceneIf* scene1){	base::MergeScene(scene0, scene1);}	\
	GRRenderIf* CreateRender(){	return	base::CreateRender();}	\
	int NRender()const{	return	base::NRender();}	\
	GRRenderIf* GetRender(int index){	return	base::GetRender(index);}	\
	void SwitchRender(GRRenderIf* render){	base::SwitchRender(render);}	\
	PHSdkIf* GetPHSdk(){	return	base::GetPHSdk();}	\
	GRSdkIf* GetGRSdk(){	return	base::GetGRSdk();}	\
	FISdkIf* GetFISdk(){	return	base::GetFISdk();}	\
	bool GetDebugMode(){	return	base::GetDebugMode();}	\
	void SetDebugMode(bool debug){	base::SetDebugMode(debug);}	\
	void Step(){	base::Step();}	\
	void Draw(){	base::Draw();}	\
	void Reshape(int w, int h){	base::Reshape(w, h);}	\
	void SetDSTR(bool f){	base::SetDSTR(f);}	\

