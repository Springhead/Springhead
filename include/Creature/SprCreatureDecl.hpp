#define SPR_OVERRIDEMEMBERFUNCOF_CRBodyIf(base)	\
	void Init(){	base::Init();}	\
	int NSolids(){	return	base::NSolids();}	\
	Spr::PHSolidIf* GetSolid(int i){	return	base::GetSolid(i);}	\
	int NJoints(){	return	base::NJoints();}	\
	Spr::PHJointIf* GetJoint(int i){	return	base::GetJoint(i);}	\
	int NIKNodes(){	return	base::NIKNodes();}	\
	PHIKNodeIf* GetIKNode(int i){	return	base::GetIKNode(i);}	\
	int NControlPoints(){	return	base::NControlPoints();}	\
	PHIKControlPointIf* GetControlPoint(int i){	return	base::GetControlPoint(i);}	\
	Vec3d GetCenterOfMass(){	return	base::GetCenterOfMass();}	\
	double GetSumOfMass(){	return	base::GetSumOfMass();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRHingeHumanBodyIf(base)	\
	void SetUpperBodyStiffness(float stiffness){	base::SetUpperBodyStiffness(stiffness);}	\
	void KeepUpperBodyPose(){	base::KeepUpperBodyPose();}	\
	void ResetUpperBodyPose(){	base::ResetUpperBodyPose();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRBallHumanBodyIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRFourLegsAnimalBodyIf(base)	\
	void Init(){	base::Init();}	\
	Vec3d GetUpperCenterOfMass(){	return	base::GetUpperCenterOfMass();}	\
	int NBallJoints(){	return	base::NBallJoints();}	\
	int NHingeJoints(){	return	base::NHingeJoints();}	\
	double VSolid(int i){	return	base::VSolid(i);}	\
	double VSolids(){	return	base::VSolids();}	\
	double GetTotalMass(){	return	base::GetTotalMass();}	\
	void SetTotalMass(double value){	base::SetTotalMass(value);}	\
	double GetLegLength(int i){	return	base::GetLegLength(i);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRDebugLinkBodyIf(base)	\
	int NBallJoints(){	return	base::NBallJoints();}	\
	int NHingeJoints(){	return	base::NHingeJoints();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTrunkFootHumanBodyIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRFourLegsTinyAnimalBodyIf(base)	\
	Vec2d GetSwingLimit(){	return	base::GetSwingLimit();}	\
	Vec2d GetTwistLimit(){	return	base::GetTwistLimit();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRControllerIf(base)	\
	void Init(){	base::Init();}	\
	void Step(){	base::Step();}	\
	void SetEnable(bool enable){	base::SetEnable(enable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CREyeControllerIf(base)	\
	void LookAt(Vec3f pos, Vec3f vel){	base::LookAt(pos, vel);}	\
	bool IsSaccading(){	return	base::IsSaccading();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRNeckControllerIf(base)	\
	void LookAt(Vec3f pos, Vec3f vel, float attractiveness){	base::LookAt(pos, vel, attractiveness);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRReachingControllerIf(base)	\
	Spr::PHSolidIf* GetSolid(){	return	base::GetSolid();}	\
	void SetTargetPos(Vec3f p, Vec3f v){	base::SetTargetPos(p, v);}	\
	void SetTargetOri(Quaterniond q, Vec3f av){	base::SetTargetOri(q, av);}	\
	void SetTargetTime(float t){	base::SetTargetTime(t);}	\
	void Start(Spr::CRReachingControllerIf::ConstraintMode mode, float keeptime){	base::Start(mode, keeptime);}	\
	float GetRemainingTime(){	return	base::GetRemainingTime();}	\
	Spr::CRReachingControllerIf::ReachState GetReachState(){	return	base::GetReachState();}	\
	void Reset(){	base::Reset();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRGazeControllerIf(base)	\
	void LookAt(Vec3f pos, Vec3f vel, float attractiveness){	base::LookAt(pos, vel, attractiveness);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRAttentionControllerIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTryStandingUpControllerIf(base)	\
	void Init(){	base::Init();}	\
	void Step(){	base::Step();}	\
	void Sync(){	base::Sync();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRWalkingControllerIf(base)	\
	void SetSpeed(float speed){	base::SetSpeed(speed);}	\
	void SetRotationAngle(float rot){	base::SetRotationAngle(rot);}	\
	void SetRotationWorldCoordinate(double r){	base::SetRotationWorldCoordinate(r);}	\
	void Stop(){	base::Stop();}	\
	void Reverse(){	base::Reverse();}	\
	void SetPos(Vec3f pos){	base::SetPos(pos);}	\
	double GetBasicCycle(){	return	base::GetBasicCycle();}	\
	bool IsCompleteFall(){	return	base::IsCompleteFall();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CREseWalkingControllerIf(base)	\
	void SetSpeed(float speed){	base::SetSpeed(speed);}	\
	void SetRotationAngle(float rot){	base::SetRotationAngle(rot);}	\
	void SetPos(Vec3f pos){	base::SetPos(pos);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTravelControllerIf(base)	\
	void SetGoal(Vec3f goal){	base::SetGoal(goal);}	\

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

#define SPR_OVERRIDEMEMBERFUNCOF_CRCreatureIf(base)	\
	void Init(){	base::Init();}	\
	void Step(){	base::Step();}	\
	void ClearInternalScene(){	base::ClearInternalScene();}	\
	void SensorStep(){	base::SensorStep();}	\
	void InternalSceneStep(){	base::InternalSceneStep();}	\
	void ControllerStep(){	base::ControllerStep();}	\
	Spr::CRBodyIf* CreateBody(const IfInfo* ii, const Spr::CRBodyDesc& desc){	return	base::CreateBody(ii, desc);}	\
	Spr::CRBodyIf* GetBody(int i){	return	base::GetBody(i);}	\
	int NBodies(){	return	base::NBodies();}	\
	Spr::CRSensorIf* CreateSensor(const IfInfo* ii, const Spr::CRSensorDesc& desc){	return	base::CreateSensor(ii, desc);}	\
	Spr::CRSensorIf* GetSensor(int i){	return	base::GetSensor(i);}	\
	int NSensors(){	return	base::NSensors();}	\
	Spr::CRControllerIf* CreateController(const IfInfo* ii, const Spr::CRControllerDesc& desc){	return	base::CreateController(ii, desc);}	\
	Spr::CRControllerIf* GetController(int i){	return	base::GetController(i);}	\
	int NControllers(){	return	base::NControllers();}	\
	Spr::CRInternalSceneIf* CreateInternalScene(const Spr::CRInternalSceneDesc& desc){	return	base::CreateInternalScene(desc);}	\
	Spr::CRInternalSceneIf* GetInternalScene(){	return	base::GetInternalScene();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRInternalSceneObjectIf(base)	\
	bool IsA(const char* typestr){	return	base::IsA(typestr);}	\
	const char* GetISObjType(){	return	base::GetISObjType();}	\
	Spr::PHSolidIf* GetSolid(){	return	base::GetSolid();}	\
	Vec3f GetPos(){	return	base::GetPos();}	\
	void SetPos(Vec3d pos){	base::SetPos(pos);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRISAttractiveObjectIf(base)	\
	float GetTotalAttractiveness(){	return	base::GetTotalAttractiveness();}	\
	void AddBottomupAttr(float attr){	base::AddBottomupAttr(attr);}	\
	void ClearBottomupAttr(){	base::ClearBottomupAttr();}	\
	void SetTopdownAttr(float attr){	base::SetTopdownAttr(attr);}	\
	void IncUncertainty(){	base::IncUncertainty();}	\
	void DecUncertainty(){	base::DecUncertainty();}	\
	float GetUncertainty(){	return	base::GetUncertainty();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRISTravelPotentialObjectIf(base)	\
	Vec2f GetStrengthCoeff(){	return	base::GetStrengthCoeff();}	\
	void SetStrengthCoeff(Vec2f strength){	base::SetStrengthCoeff(strength);}	\
	Vec2f GetDecayCoeff(){	return	base::GetDecayCoeff();}	\
	void SetDecayCoeff(Vec2f decay){	base::SetDecayCoeff(decay);}	\
	Vec2f GetPotential(Vec2f currPos){	return	base::GetPotential(currPos);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRInternalSceneIf(base)	\
	void Step(){	base::Step();}	\
	void ClearAttractiveness(){	base::ClearAttractiveness();}	\
	void SortByAttractiveness(){	base::SortByAttractiveness();}	\
	Spr::CRInternalSceneObjectIf* FindObject(Spr::PHSolidIf* solid, Vec3f pos){	return	base::FindObject(solid, pos);}	\
	Spr::CRInternalSceneObjectIf* GetISObject(int i){	return	base::GetISObject(i);}	\
	int NObjects(){	return	base::NObjects();}	\
	Spr::CRInternalSceneObjectIf* CreateInternalSceneObject(const IfInfo* ii, const Spr::CRInternalSceneObjectDesc& desc){	return	base::CreateInternalSceneObject(ii, desc);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRSensorIf(base)	\
	void Init(){	base::Init();}	\
	void Step(){	base::Step();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CROpticalSensorIf(base)	\
	bool IsVisible(Spr::PHSolidIf* solid){	return	base::IsVisible(solid);}	\
	bool IsInCenter(Spr::PHSolidIf* solid){	return	base::IsInCenter(solid);}	\
	bool IsVisible(Vec3f pos){	return	base::IsVisible(pos);}	\
	bool IsInCenter(Vec3f pos){	return	base::IsInCenter(pos);}	\
	bool IsSelfSolid(Spr::PHSolidIf* solid){	return	base::IsSelfSolid(solid);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_CRTouchSensorIf(base)	\
	int NContacts(){	return	base::NContacts();}	\
	Spr::PHSolidIf* GetContactSolidMe(int n){	return	base::GetContactSolidMe(n);}	\
	Spr::PHSolidIf* GetContactSolidOther(int n){	return	base::GetContactSolidOther(n);}	\
	double GetContactPressure(int n){	return	base::GetContactPressure(n);}	\
	Vec3d GetContactPos(int n){	return	base::GetContactPos(n);}	\

