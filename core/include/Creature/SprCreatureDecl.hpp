#define SPR_OVERRIDEMEMBERFUNCOF_CRBodyIf(base)	\
	Spr::CRBoneIf* FindByLabel(UTString label){	return	base::FindByLabel(label);}	\
	int NBones(){	return	base::NBones();}	\
	Spr::CRBoneIf* GetBone(int i){	return	base::GetBone(i);}	\
	Vec3d GetCenterOfMass(){	return	base::GetCenterOfMass();}	\
	double GetSumOfMass(){	return	base::GetSumOfMass();}	\
	void Step(){	base::Step();}	\

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
	void SetOriginSolid(Spr::PHSolidIf* solid){	base::SetOriginSolid(solid);}	\
	void AddTrajectoryNode(Spr::CRTrajectoryNode node, bool clear){	base::AddTrajectoryNode(node, clear);}	\
	Spr::CRTrajectoryNode GetTrajectoryNode(int i){	return	base::GetTrajectoryNode(i);}	\
	int NTrajectoryNodes(){	return	base::NTrajectoryNodes();}	\
	Spr::CRTrajectoryNode GetTrajectoryNodeAt(float time){	return	base::GetTrajectoryNodeAt(time);}	\
	void SetTrajectoryNode(int i, Spr::CRTrajectoryNode node){	base::SetTrajectoryNode(i, node);}	\
	Spr::CRTrajectoryNode GetCurrentNode(){	return	base::GetCurrentNode();}	\
	void ClearTrajectory(){	base::ClearTrajectory();}	\
	void StepTrajectory(){	base::StepTrajectory();}	\
	void Plan(){	base::Plan();}	\
	bool IsPlanning(){	return	base::IsPlanning();}	\
	bool IsNewTrajectoryStarted(){	return	base::IsNewTrajectoryStarted();}	\
	bool IsTrajectoryChanged(){	return	base::IsTrajectoryChanged();}	\
	void SetTargetPos(Vec3d pos){	base::SetTargetPos(pos);}	\
	Vec3d GetTargetPos(){	return	base::GetTargetPos();}	\
	void SetTargetOri(Quaterniond ori){	base::SetTargetOri(ori);}	\
	Quaterniond GetTargetOri(){	return	base::GetTargetOri();}	\
	void SetTargetPose(Posed pose){	base::SetTargetPose(pose);}	\
	void SetTimeLimit(float timeLimit){	base::SetTimeLimit(timeLimit);}	\
	void Start(){	base::Start();}	\
	void Pause(){	base::Pause();}	\
	void Stop(){	base::Stop();}	\
	void SetViewArea(Posed relativePose, double horizRange, double vertRange){	base::SetViewArea(relativePose, horizRange, vertRange);}	\
	int NVisibleSolids(){	return	base::NVisibleSolids();}	\
	Spr::PHSolidIf* GetVisibleSolid(int i){	return	base::GetVisibleSolid(i);}	\
	void StepSearchArea(){	base::StepSearchArea();}	\
	int NTouches(){	return	base::NTouches();}	\
	Spr::CRTouchInfo GetTouchInfo(int i){	return	base::GetTouchInfo(i);}	\
	void AddTouchInfo(Spr::CRTouchInfo ci){	base::AddTouchInfo(ci);}	\
	void ClearTouchInfo(){	base::ClearTouchInfo();}	\
	Vec3f GetContactForce(int i){	return	base::GetContactForce(i);}	\
	double GetContactArea(int i){	return	base::GetContactArea(i);}	\
	Vec3f GetContactPosition(int i){	return	base::GetContactPosition(i);}	\
	Spr::PHSolidIf* GetContactSolid(int i){	return	base::GetContactSolid(i);}	\
	void StepListContact(){	base::StepListContact();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRControllerIf(base)	\
	void Reset(){	base::Reset();}	\
	int GetStatus(){	return	base::GetStatus();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRGazeControllerIf(base)	\
	void SetTargetPosition(Vec3d pos){	base::SetTargetPosition(pos);}	\
	Vec3d GetTargetPosition(){	return	base::GetTargetPosition();}	\
	void SetHeadBone(Spr::CRBoneIf* head){	base::SetHeadBone(head);}	\

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

