#define SPR_OVERRIDEMEMBERFUNCOF_CRBodyIf(base)	\
	Spr::CRBoneIf* FindByLabel(UTString label){	return	base::FindByLabel(label);}	\
	int NBones(){	return	base::NBones();}	\
	Spr::CRBoneIf* GetBone(int i){	return	base::GetBone(i);}	\
	Vec3d GetCenterOfMass(){	return	base::GetCenterOfMass();}	\
	double GetSumOfMass(){	return	base::GetSumOfMass();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRBoneIf(base)	\
	const char* GetLabel()const{	return	base::GetLabel();}	\
	void SetLabel(const char* str){	base::SetLabel(str);}	\
	Spr::PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(Spr::PHSolidIf* so){	base::SetPHSolid(so);}	\
	Spr::PHIKEndEffectorIf* GetIKEndEffector(){	return	base::GetIKEndEffector();}	\
	void SetIKEndEffector(Spr::PHIKEndEffectorIf* ikEE){	base::SetIKEndEffector(ikEE);}	\
	Spr::PHJointIf* GetPHJoint(){	return	base::GetPHJoint();}	\
	void SetPHJoint(Spr::PHJointIf* jo){	base::SetPHJoint(jo);}	\
	Spr::PHIKActuatorIf* GetIKActuator(){	return	base::GetIKActuator();}	\
	void SetIKActuator(Spr::PHIKActuatorIf* ikAct){	base::SetIKActuator(ikAct);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRControllerIf(base)	\
	void Reset(){	base::Reset();}	\
	int GetStatus(){	return	base::GetStatus();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRGazeControllerIf(base)	\
	void SetTargetPosition(Vec3d pos){	base::SetTargetPosition(pos);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRReachControllerIf(base)	\
	void SetTargetPosition(Vec3d pos){	base::SetTargetPosition(pos);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRGrabControllerIf(base)	\
	void SetTargetSolid(Spr::PHSolidIf* targetSolid){	base::SetTargetSolid(targetSolid);}	\
	Spr::PHSolidIf* GetGrabbingSolid(){	return	base::GetGrabbingSolid();}	\
	Spr::PHSolidIf* GetSolid(){	return	base::GetSolid();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRCreatureIf(base)	\
	void Step(){	base::Step();}	\
	Spr::CRBodyIf* CreateBody(const IfInfo* ii, const Spr::CRBodyDesc& desc){	return	base::CreateBody(ii, desc);}	\
	Spr::CRBodyIf* GetBody(int i){	return	base::GetBody(i);}	\
	int NBodies(){	return	base::NBodies();}	\
	Spr::CREngineIf* CreateEngine(const IfInfo* ii, const Spr::CREngineDesc& desc){	return	base::CreateEngine(ii, desc);}	\
	Spr::CREngineIf* GetEngine(int i){	return	base::GetEngine(i);}	\
	int NEngines(){	return	base::NEngines();}	\
	Spr::PHSceneIf* GetPHScene(){	return	base::GetPHScene();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CREngineIf(base)	\
	int GetPriority()const{	return	base::GetPriority();}	\
	void Init(){	base::Init();}	\
	void Step(){	base::Step();}	\
	void Enable(bool enable){	base::Enable(enable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRSdkIf(base)	\
	Spr::CRCreatureIf* CreateCreature(const IfInfo* ii, const Spr::CRCreatureDesc& desc){	return	base::CreateCreature(ii, desc);}	\
	int NCreatures()const{	return	base::NCreatures();}	\
	Spr::CRCreatureIf* GetCreature(int index){	return	base::GetCreature(index);}	\
	void Step(){	base::Step();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTouchSensorIf(base)	\
	int NContacts(){	return	base::NContacts();}	\
	Spr::CRContactInfo GetContact(int n){	return	base::GetContact(n);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRVisualSensorIf(base)	\
	bool IsVisible(Spr::PHSolidIf* solid){	return	base::IsVisible(solid);}	\
	bool IsVisible(Vec3f pos){	return	base::IsVisible(pos);}	\
	bool IsInCenter(Spr::PHSolidIf* solid){	return	base::IsInCenter(solid);}	\
	bool IsInCenter(Vec3f pos){	return	base::IsInCenter(pos);}	\
	bool IsSelfSolid(Spr::PHSolidIf* solid){	return	base::IsSelfSolid(solid);}	\

