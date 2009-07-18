#define SPR_OVERRIDEMEMBERFUNCOF_FWBoneIf(base)	\
	PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(PHSolidIf* s){	base::SetPHSolid(s);}	\
	GRFrameIf* GetGRFrame(){	return	base::GetGRFrame();}	\
	void SetGRFrame(GRFrameIf* f){	base::SetGRFrame(f);}	\
	CDRoundConeIf* GetShape(){	return	base::GetShape();}	\
	void SetShapeRadius(double r1, double r2){	base::SetShapeRadius(r1, r2);}	\
	PHJointIf* GetJoint(){	return	base::GetJoint();}	\
	void SetJointKDD2(double K, double D, double D2){	base::SetJointKDD2(K, D, D2);}	\

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

#define SPR_OVERRIDEMEMBERFUNCOF_FWSdkIf(base)	\
	Spr::FWSceneIf* CreateScene(const PHSceneDesc& phdesc, const GRSceneDesc& grdesc){	return	base::CreateScene(phdesc, grdesc);}	\
	bool LoadScene(UTString filename){	return	base::LoadScene(filename);}	\
	bool SaveScene(UTString filename){	return	base::SaveScene(filename);}	\
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

