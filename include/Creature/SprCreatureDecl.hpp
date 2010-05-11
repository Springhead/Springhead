#define SPR_OVERRIDEMEMBERFUNCOF_CRBodyPartIf(base)	\
	const char* GetLabel()const{	return	base::GetLabel();}	\
	void SetLabel(const char* str){	base::SetLabel(str);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRSolidIf(base)	\
	Spr::PHSolidIf* GetPHSolid(){	return	base::GetPHSolid();}	\
	void SetPHSolid(Spr::PHSolidIf* so){	base::SetPHSolid(so);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRIKSolidIf(base)	\
	PHIKEndEffectorIf* GetIKEndEffector(){	return	base::GetIKEndEffector();}	\
	void SetIKEndEffector(PHIKEndEffectorIf* ikEE){	base::SetIKEndEffector(ikEE);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRJointIf(base)	\
	Spr::PHJointIf* GetPHJoint(){	return	base::GetPHJoint();}	\
	void SetPHJoint(Spr::PHJointIf* jo){	base::SetPHJoint(jo);}	\
	void SetSpringRatio(double springRatio, double damperRatio){	base::SetSpringRatio(springRatio, damperRatio);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRIKJointIf(base)	\
	PHIKActuatorIf* GetIKActuator(){	return	base::GetIKActuator();}	\
	void SetIKActuator(PHIKActuatorIf* ikAct){	base::SetIKActuator(ikAct);}	\
	void SetIKSpringRatio(double springRatio, double damperRatio){	base::SetIKSpringRatio(springRatio, damperRatio);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRBodyIf(base)	\
	int NSolids(){	return	base::NSolids();}	\
	Spr::CRSolidIf* GetSolid(int i){	return	base::GetSolid(i);}	\
	int NJoints(){	return	base::NJoints();}	\
	Spr::CRJointIf* GetJoint(int i){	return	base::GetJoint(i);}	\
	Vec3d GetCenterOfMass(){	return	base::GetCenterOfMass();}	\
	double GetSumOfMass(){	return	base::GetSumOfMass();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRCreatureIf(base)	\
	void Step(){	base::Step();}	\
	Spr::CRBodyIf* CreateBody(const IfInfo* ii, const Spr::CRBodyDesc& desc){	return	base::CreateBody(ii, desc);}	\
	Spr::CRBodyIf* GetBody(int i){	return	base::GetBody(i);}	\
	int NBodies(){	return	base::NBodies();}	\
	Spr::CREngineIf* CreateEngine(const IfInfo* ii, const Spr::CREngineDesc& desc){	return	base::CreateEngine(ii, desc);}	\
	Spr::CREngineIf* GetEngine(int i){	return	base::GetEngine(i);}	\
	int NEngines(){	return	base::NEngines();}	\
	Spr::CRSceneIf* CreateScene(const IfInfo* ii, const Spr::CRSceneDesc& desc){	return	base::CreateScene(ii, desc);}	\
	Spr::CRSceneIf* GetScene(int i){	return	base::GetScene(i);}	\
	int NScenes(){	return	base::NScenes();}	\
	PHSceneIf* GetPHScene(){	return	base::GetPHScene();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CREngineIf(base)	\
	int GetPriority()const{	return	base::GetPriority();}	\
	void Step(){	base::Step();}	\
	void SetEnable(bool enable){	base::SetEnable(enable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\
	void Render(GRRenderIf* render){	base::Render(render);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CREyeControllerIf(base)	\
	void LookAt(Vec3f pos, Vec3f vel){	base::LookAt(pos, vel);}	\
	bool IsSaccading(){	return	base::IsSaccading();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRNeckControllerIf(base)	\
	void LookAt(Vec3f pos, Vec3f vel, float attractiveness){	base::LookAt(pos, vel, attractiveness);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRGazeControllerIf(base)	\
	void LookAt(Vec3f pos, Vec3f vel, float attractiveness){	base::LookAt(pos, vel, attractiveness);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRAttentionControllerIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRReachingControllerIf(base)	\
	void SetPos(Vec3f pos){	base::SetPos(pos);}	\
	void Start(Vec3d pose, Vec3d v, float t){	base::Start(pose, v, t);}	\
	void Stop(){	base::Stop();}	\
	void SetCRSolid(Spr::CRIKSolidIf* cso){	base::SetCRSolid(cso);}	\
	Spr::CRIKSolidIf* GetCRSolid(){	return	base::GetCRSolid();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRReachingControllersIf(base)	\
	Spr::CRReachingControllerIf* GetReachingController(Spr::PHSolidIf* solid){	return	base::GetReachingController(solid);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRGrabControllerIf(base)	\
	bool Reach(Spr::PHSolidIf* solid, float radius){	return	base::Reach(solid, radius);}	\
	bool IsReachable(Spr::PHSolidIf* solid){	return	base::IsReachable(solid);}	\
	bool IsReachable(Spr::PHSolidIf* solid, float safety){	return	base::IsReachable(solid, safety);}	\
	bool IsReachComplete(){	return	base::IsReachComplete();}	\
	bool Uphold(){	return	base::Uphold();}	\
	bool IsUpholdable(){	return	base::IsUpholdable();}	\
	bool IsUpholdComplete(){	return	base::IsUpholdComplete();}	\
	bool Place(Vec3d pos){	return	base::Place(pos);}	\
	bool IsPlaceable(Vec3d pos){	return	base::IsPlaceable(pos);}	\
	bool IsPlaceable(Vec3d pos, float safety){	return	base::IsPlaceable(pos, safety);}	\
	bool IsPlaceComplete(){	return	base::IsPlaceComplete();}	\
	void Abort(){	base::Abort();}	\
	void AbortAll(){	base::AbortAll();}	\
	Spr::CRGrabControllerIf::CRGCControlState GetControlState(){	return	base::GetControlState();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRSceneIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRSdkIf(base)	\
	Spr::CRCreatureIf* CreateCreature(const IfInfo* ii, const Spr::CRCreatureDesc& desc){	return	base::CreateCreature(ii, desc);}	\
	int NCreatures()const{	return	base::NCreatures();}	\
	Spr::CRCreatureIf* GetCreature(int index){	return	base::GetCreature(index);}	\
	void Step(){	base::Step();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTouchSensorIf(base)	\
	int NContacts(){	return	base::NContacts();}	\
	Spr::CRContactInfo GetContact(int n){	return	base::GetContact(n);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTrajectoryIf(base)	\
	Vec3f GetPosition(){	return	base::GetPosition();}	\
	Vec3f GetVelocity(){	return	base::GetVelocity();}	\
	Quaternionf GetOrientation(){	return	base::GetOrientation();}	\
	Vec3f GetAngularVelocity(){	return	base::GetAngularVelocity();}	\
	void Start(){	base::Start();}	\
	void Step(){	base::Step();}	\
	void SetTimeLimit(double tL){	base::SetTimeLimit(tL);}	\
	void SetTargetPosition(Vec3f pos){	base::SetTargetPosition(pos);}	\
	void EnablePositionControl(bool enable){	base::EnablePositionControl(enable);}	\
	void SetTargetPositionInSolid(Vec3f pos){	base::SetTargetPositionInSolid(pos);}	\
	void SetTargetVelocity(Vec3f vel){	base::SetTargetVelocity(vel);}	\
	void EnableVelocityControl(bool enable){	base::EnableVelocityControl(enable);}	\
	void SetTargetOrientation(Vec3f ori){	base::SetTargetOrientation(ori);}	\
	void EnableOrientationControl(bool enable){	base::EnableOrientationControl(enable);}	\
	void SetTargetAngularVelocity(Vec3f angvel){	base::SetTargetAngularVelocity(angvel);}	\
	void EnableAngularVelocityControl(bool enable){	base::EnableAngularVelocityControl(enable);}	\
	void Enable(bool enable){	base::Enable(enable);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTrajectoryControllerIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRVisualSensorIf(base)	\
	bool IsVisible(Spr::PHSolidIf* solid){	return	base::IsVisible(solid);}	\
	bool IsVisible(Vec3f pos){	return	base::IsVisible(pos);}	\
	bool IsInCenter(Spr::PHSolidIf* solid){	return	base::IsInCenter(solid);}	\
	bool IsInCenter(Vec3f pos){	return	base::IsInCenter(pos);}	\
	bool IsSelfSolid(Spr::PHSolidIf* solid){	return	base::IsSelfSolid(solid);}	\

