#define SPR_OVERRIDEMEMBERFUNCOF_PHEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHConstraintEngineIf(base)	\
	Spr::PHConstraintsIf* GetContactPoints(){	return	base::GetContactPoints();}	\
	void SetVelCorrectionRate(double value){	base::SetVelCorrectionRate(value);}	\
	double GetVelCorrectionRate(){	return	base::GetVelCorrectionRate();}	\
	void SetPosCorrectionRate(double value){	base::SetPosCorrectionRate(value);}	\
	double GetPosCorrectionRate(){	return	base::GetPosCorrectionRate();}	\
	void SetContactCorrectionRate(double value){	base::SetContactCorrectionRate(value);}	\
	double GetContactCorrectionRate(){	return	base::GetContactCorrectionRate();}	\
	void SetBSaveConstraints(bool value){	base::SetBSaveConstraints(value);}	\
	void SetUpdateAllSolidState(bool flag){	base::SetUpdateAllSolidState(flag);}	\
	void SetUseContactSurface(bool flag){	base::SetUseContactSurface(flag);}	\
	void SetShrinkRate(double data){	base::SetShrinkRate(data);}	\
	double GetShrinkRate(){	return	base::GetShrinkRate();}	\
	void SetShrinkRateCorrection(double data){	base::SetShrinkRateCorrection(data);}	\
	double GetShrinkRateCorrection(){	return	base::GetShrinkRateCorrection();}	\
	void EnableContactDetection(bool enable){	base::EnableContactDetection(enable);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHGravityEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPenaltyEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKEngineIf(base)	\
	bool IsEnabled(){	return	base::IsEnabled();}	\
	void Enable(bool bEnabled){	base::Enable(bEnabled);}	\
	void SetNumIter(int numIter){	base::SetNumIter(numIter);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHFemEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHHapticEngineIf(base)	\
	void EnableHapticEngine(bool b){	base::EnableHapticEngine(b);}	\
	void SetHapticEngineMode(Spr::PHHapticEngineDesc::HapticEngineMode mode){	base::SetHapticEngineMode(mode);}	\
	void StepPhysicsSimulation(){	base::StepPhysicsSimulation();}	\
	bool IsAfterStepPhysicsSimulation(){	return	base::IsAfterStepPhysicsSimulation();}	\
	void ReleaseState(){	base::ReleaseState();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHFemMeshIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHFemMeshThermoIf(base)	\
	int GetSurfaceVertex(int id){	return	base::GetSurfaceVertex(id);}	\
	int NSurfaceVertices(){	return	base::NSurfaceVertices();}	\
	void SetVertexTc(int id, double temp){	base::SetVertexTc(id, temp);}	\
	void SetVertexTc(int id, double temp, double heatTrans){	base::SetVertexTc(id, temp, heatTrans);}	\
	Vec3d GetPose(int id){	return	base::GetPose(id);}	\
	Vec3d GetSufVtxPose(unsigned int id){	return	base::GetSufVtxPose(id);}	\
	unsigned long GetStepCount(){	return	base::GetStepCount();}	\
	unsigned long GetStepCountCyc(){	return	base::GetStepCountCyc();}	\
	double GetVertexTemp(unsigned int id){	return	base::GetVertexTemp(id);}	\
	double GetSufVertexTemp(unsigned int id){	return	base::GetSufVertexTemp(id);}	\
	void SetVertexTemp(unsigned int id, double temp){	base::SetVertexTemp(id, temp);}	\
	void SetvecFAll(unsigned int id, double dqdt){	base::SetvecFAll(id, dqdt);}	\
	void SetRhoSpheat(double rho, double Cp){	base::SetRhoSpheat(rho, Cp);}	\
	unsigned int GetNFace(){	return	base::GetNFace();}	\
	std::vector< Vec3d > GetFaceEdgeVtx(unsigned int id){	return	base::GetFaceEdgeVtx(id);}	\
	Vec3d GetFaceEdgeVtx(unsigned int id, unsigned int vtx){	return	base::GetFaceEdgeVtx(id, vtx);}	\
	Vec2d GetIHbandDrawVtx(){	return	base::GetIHbandDrawVtx();}	\
	void CalcIHdqdt_atleast(double r, double R, double dqdtAll){	base::CalcIHdqdt_atleast(r, R, dqdtAll);}	\
	void UpdateIHheat(unsigned int heating){	base::UpdateIHheat(heating);}	\
	void DecrMoist(){	base::DecrMoist();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHHapticPointerIf(base)	\
	void SetHapticRenderMode(Spr::PHHapticPointerDesc::HapticRenderMode m){	base::SetHapticRenderMode(m);}	\
	void EnableForce(bool b){	base::EnableForce(b);}	\
	void EnableFriction(bool b){	base::EnableFriction(b);}	\
	void EnableVibration(bool b){	base::EnableVibration(b);}	\
	void EnableMultiPoints(bool b){	base::EnableMultiPoints(b);}	\
	void EnableDebugControl(bool b){	base::EnableDebugControl(b);}	\
	void SetReflexSpring(float s){	base::SetReflexSpring(s);}	\
	float GetReflexSpring(){	return	base::GetReflexSpring();}	\
	void SetReflexDamper(float d){	base::SetReflexDamper(d);}	\
	float GetReflexDamper(){	return	base::GetReflexDamper();}	\
	void SetReflexSpringOri(float s){	base::SetReflexSpringOri(s);}	\
	float GetReflexSpringOri(){	return	base::GetReflexSpringOri();}	\
	void SetReflexDamperOri(float d){	base::SetReflexDamperOri(d);}	\
	float GetReflexDamperOri(){	return	base::GetReflexDamperOri();}	\
	void SetLocalRange(float r){	base::SetLocalRange(r);}	\
	float GetLocalRange(){	return	base::GetLocalRange();}	\
	void SetPosScale(double scale){	base::SetPosScale(scale);}	\
	double GetPosScale(){	return	base::GetPosScale();}	\
	void SetRotationalWeight(double w){	base::SetRotationalWeight(w);}	\
	double GetRotationalWeight(){	return	base::GetRotationalWeight();}	\
	void SetDefaultPose(Posed p){	base::SetDefaultPose(p);}	\
	Posed GetDefaultPose(){	return	base::GetDefaultPose();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKEndEffectorIf(base)	\
	void SetSolid(Spr::PHSolidIf* solid){	base::SetSolid(solid);}	\
	Spr::PHSolidIf* GetSolid(){	return	base::GetSolid();}	\
	void Enable(bool enable){	base::Enable(enable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\
	void EnablePositionControl(bool enable){	base::EnablePositionControl(enable);}	\
	void SetPositionPriority(double priority){	base::SetPositionPriority(priority);}	\
	void SetTargetPosition(Vec3d position){	base::SetTargetPosition(position);}	\
	void SetTargetLocalPosition(Vec3d localPosition){	base::SetTargetLocalPosition(localPosition);}	\
	Vec3d GetTargetPosition(){	return	base::GetTargetPosition();}	\
	Vec3d GetTargetLocalPosition(){	return	base::GetTargetLocalPosition();}	\
	void EnableOrientationControl(bool enable){	base::EnableOrientationControl(enable);}	\
	void SetOrientationPriority(double priority){	base::SetOrientationPriority(priority);}	\
	void SetTargetOrientation(Quaterniond orientation){	base::SetTargetOrientation(orientation);}	\
	Quaterniond GetTargetOrientation(){	return	base::GetTargetOrientation();}	\
	void EnableForceControl(bool enable){	base::EnableForceControl(enable);}	\
	void SetTargetForce(Vec3d force, Vec3d workingPoint){	base::SetTargetForce(force, workingPoint);}	\
	Vec3d GetTargetForce(){	return	base::GetTargetForce();}	\
	Vec3d GetTargetForceWorkingPoint(){	return	base::GetTargetForceWorkingPoint();}	\
	void EnableTorqueControl(bool enable){	base::EnableTorqueControl(enable);}	\
	void SetTargetTorque(Vec3d torque){	base::SetTargetTorque(torque);}	\
	Vec3d GetTargetTorque(){	return	base::GetTargetTorque();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKActuatorIf(base)	\
	void PrepareSolve(){	base::PrepareSolve();}	\
	void ProceedSolve(){	base::ProceedSolve();}	\
	PTM::VVector< double > GetRawSolution(){	return	base::GetRawSolution();}	\
	void Move(){	base::Move();}	\
	void SetBias(float bias){	base::SetBias(bias);}	\
	float GetBias(){	return	base::GetBias();}	\
	void SetVelocityGain(double velocityGain){	base::SetVelocityGain(velocityGain);}	\
	double GetVelocityGain(){	return	base::GetVelocityGain();}	\
	void Enable(bool enable){	base::Enable(enable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKBallActuatorIf(base)	\
	void SetJoint(Spr::PHBallJointIf* joint){	base::SetJoint(joint);}	\
	Spr::PHBallJointIf* GetJoint(){	return	base::GetJoint();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKHingeActuatorIf(base)	\
	void SetJoint(Spr::PHHingeJointIf* joint){	base::SetJoint(joint);}	\
	Spr::PHHingeJointIf* GetJoint(){	return	base::GetJoint();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHConstraintIf(base)	\
	Spr::PHSolidIf* GetSocketSolid(){	return	base::GetSocketSolid();}	\
	Spr::PHSolidIf* GetPlugSolid(){	return	base::GetPlugSolid();}	\
	Spr::PHSceneIf* GetScene()const{	return	base::GetScene();}	\
	void Enable(bool bEnable){	base::Enable(bEnable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\
	void GetSocketPose(Posed& pose){	base::GetSocketPose(pose);}	\
	void SetSocketPose(const Posed& pose){	base::SetSocketPose(pose);}	\
	void GetPlugPose(Posed& pose){	base::GetPlugPose(pose);}	\
	void SetPlugPose(const Posed& pose){	base::SetPlugPose(pose);}	\
	void GetRelativePose(Posed& p){	base::GetRelativePose(p);}	\
	Vec3d GetRelativePoseR(){	return	base::GetRelativePoseR();}	\
	Quaterniond GetRelativePoseQ(){	return	base::GetRelativePoseQ();}	\
	Quaternionf GetAbsolutePoseQ(){	return	base::GetAbsolutePoseQ();}	\
	void GetRelativeVelocity(Vec3d& v, Vec3d& w){	base::GetRelativeVelocity(v, w);}	\
	void GetConstraintForce(Vec3d& f, Vec3d& t){	base::GetConstraintForce(f, t);}	\
	Vec3d GetForce(){	return	base::GetForce();}	\
	Vec3d GetTorque(){	return	base::GetTorque();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHConstraintsIf(base)	\
	Spr::PHConstraintIf* FindBySolidPair(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs){	return	base::FindBySolidPair(lhs, rhs);}	\
	Vec3d GetTotalForce(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs){	return	base::GetTotalForce(lhs, rhs);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHContactPointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHJointIf(base)	\
	void SetMaxForce(double max){	base::SetMaxForce(max);}	\
	double GetMaxForce(){	return	base::GetMaxForce();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PH1DJointIf(base)	\
	PH1DJointLimitIf* CreateLimit(const PH1DJointLimitDesc& desc){	return	base::CreateLimit(desc);}	\
	double GetPosition(){	return	base::GetPosition();}	\
	double GetVelocity(){	return	base::GetVelocity();}	\
	PH1DJointLimitIf* GetLimit(){	return	base::GetLimit();}	\
	void SetSpring(double spring){	base::SetSpring(spring);}	\
	double GetSpring(){	return	base::GetSpring();}	\
	void SetDamper(double damper){	base::SetDamper(damper);}	\
	double GetDamper(){	return	base::GetDamper();}	\
	double GetSecondDamper(){	return	base::GetSecondDamper();}	\
	void SetSecondDamper(double input){	base::SetSecondDamper(input);}	\
	void SetTargetPosition(double targetPosition){	base::SetTargetPosition(targetPosition);}	\
	double GetTargetPosition(){	return	base::GetTargetPosition();}	\
	void SetTargetVelocity(double v){	base::SetTargetVelocity(v);}	\
	double GetTargetVelocity(){	return	base::GetTargetVelocity();}	\
	void SetOffsetForce(double dat){	base::SetOffsetForce(dat);}	\
	double GetOffsetForce(){	return	base::GetOffsetForce();}	\
	double GetYieldStress(){	return	base::GetYieldStress();}	\
	void SetYieldStress(const double yS){	base::SetYieldStress(yS);}	\
	double GetHardnessRate(){	return	base::GetHardnessRate();}	\
	void SetHardnessRate(const double hR){	base::SetHardnessRate(hR);}	\
	void SetSecondMoment(const double& sM){	base::SetSecondMoment(sM);}	\
	double GetSecondMoment(){	return	base::GetSecondMoment();}	\
	double GetMotorForce(){	return	base::GetMotorForce();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHHingeJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSliderJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPathIf(base)	\
	void AddPoint(double s, const Posed& pose){	base::AddPoint(s, pose);}	\
	void SetLoop(bool bOnOff){	base::SetLoop(bOnOff);}	\
	bool IsLoop(){	return	base::IsLoop();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPathJointIf(base)	\
	void SetPosition(double q){	base::SetPosition(q);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointIf(base)	\
	PHBallJointLimitIf* CreateLimit(const IfInfo* ii, const PHBallJointLimitDesc& desc){	return	base::CreateLimit(ii, desc);}	\
	Vec3d GetAngle(){	return	base::GetAngle();}	\
	Quaterniond GetPosition(){	return	base::GetPosition();}	\
	Vec3d GetVelocity(){	return	base::GetVelocity();}	\
	PHBallJointLimitIf* GetLimit(){	return	base::GetLimit();}	\
	void SetSpring(double spring){	base::SetSpring(spring);}	\
	double GetSpring(){	return	base::GetSpring();}	\
	void SetDamper(double damper){	base::SetDamper(damper);}	\
	double GetDamper(){	return	base::GetDamper();}	\
	Vec3d GetSecondDamper(){	return	base::GetSecondDamper();}	\
	void SetSecondDamper(Vec3d damper2){	base::SetSecondDamper(damper2);}	\
	void SetTargetPosition(Quaterniond p){	base::SetTargetPosition(p);}	\
	Quaterniond GetTargetPosition(){	return	base::GetTargetPosition();}	\
	void SetTargetVelocity(Vec3d q){	base::SetTargetVelocity(q);}	\
	Vec3d GetTargetVelocity(){	return	base::GetTargetVelocity();}	\
	void SetOffsetForce(Vec3d ofst){	base::SetOffsetForce(ofst);}	\
	Vec3d GetOffsetForce(){	return	base::GetOffsetForce();}	\
	void SetYieldStress(const double yS){	base::SetYieldStress(yS);}	\
	double GetYieldStress(){	return	base::GetYieldStress();}	\
	void SetHardnessRate(const double hR){	base::SetHardnessRate(hR);}	\
	double GetHardnessRate(){	return	base::GetHardnessRate();}	\
	void SetSecondMoment(const Vec3d m){	base::SetSecondMoment(m);}	\
	Vec3d GetSecondMoment(){	return	base::GetSecondMoment();}	\
	Vec3d GetMotorForce(){	return	base::GetMotorForce();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSpringIf(base)	\
	void SetSpring(const Vec3d& spring){	base::SetSpring(spring);}	\
	Vec3d GetSpring(){	return	base::GetSpring();}	\
	void SetDamper(const Vec3d& damper){	base::SetDamper(damper);}	\
	Vec3d GetDamper(){	return	base::GetDamper();}	\
	void SetSecondDamper(const Vec3d& secondDamper){	base::SetSecondDamper(secondDamper);}	\
	Vec3d GetSecondDamper(){	return	base::GetSecondDamper();}	\
	void SetSpringOri(const double spring){	base::SetSpringOri(spring);}	\
	double GetSpringOri(){	return	base::GetSpringOri();}	\
	void SetDamperOri(const double damper){	base::SetDamperOri(damper);}	\
	double GetDamperOri(){	return	base::GetDamperOri();}	\
	void SetSecondDamperOri(const double& secondDamperOri){	base::SetSecondDamperOri(secondDamperOri);}	\
	double GetSecondDamperOri(){	return	base::GetSecondDamperOri();}	\
	void SetYieldStress(const double& yieldStress){	base::SetYieldStress(yieldStress);}	\
	double GetYieldStress(){	return	base::GetYieldStress();}	\
	void SetHardnessRate(const double& hardnessRate){	base::SetHardnessRate(hardnessRate);}	\
	double GetHardnessRate(){	return	base::GetHardnessRate();}	\
	void SetSecondMoment(const Vec3d& secondMoment){	base::SetSecondMoment(secondMoment);}	\
	Vec3d GetSecondMoment(){	return	base::GetSecondMoment();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHTreeNodeIf(base)	\
	void Enable(bool bEnable){	base::Enable(bEnable);}	\
	bool IsEnabled(){	return	base::IsEnabled();}	\
	int NChildren(){	return	base::NChildren();}	\
	Spr::PHTreeNodeIf* GetChild(int i){	return	base::GetChild(i);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHRootNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHTreeNode1DIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHHingeJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSliderJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPathJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHGearIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PH1DJointLimitIf(base)	\
	void SetRange(Vec2d range){	base::SetRange(range);}	\
	void GetRange(Vec2d& range){	base::GetRange(range);}	\
	void SetSpring(double spring){	base::SetSpring(spring);}	\
	double GetSpring(){	return	base::GetSpring();}	\
	void SetDamper(double damper){	base::SetDamper(damper);}	\
	double GetDamper(){	return	base::GetDamper();}	\
	bool IsOnLimit(){	return	base::IsOnLimit();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointLimitIf(base)	\
	void SetSpring(double rSpring){	base::SetSpring(rSpring);}	\
	double GetSpring(){	return	base::GetSpring();}	\
	void SetDamper(double rDamper){	base::SetDamper(rDamper);}	\
	double GetDamper(){	return	base::GetDamper();}	\
	void SetLimitDir(Vec3d limDir){	base::SetLimitDir(limDir);}	\
	Vec3d GetLimitDir(){	return	base::GetLimitDir();}	\
	bool IsOnLimit(){	return	base::IsOnLimit();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointConeLimitIf(base)	\
	void SetSwingRange(Vec2d range){	base::SetSwingRange(range);}	\
	void GetSwingRange(Vec2d& range){	base::GetSwingRange(range);}	\
	void SetTwistRange(Vec2d range){	base::SetTwistRange(range);}	\
	void GetTwistRange(Vec2d& range){	base::GetTwistRange(range);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointSplineLimitIf(base)	\
	void AddNode(Spr::PHSplineLimitNode node){	base::AddNode(node);}	\
	void AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax){	base::AddNode(S, SD, dS, dSD, tMin, tMax);}	\
	void AddNode(Spr::PHSplineLimitNode node, int pos){	base::AddNode(node, pos);}	\
	void AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax, int pos){	base::AddNode(S, SD, dS, dSD, tMin, tMax, pos);}	\
	void SetPoleTwistRange(Vec2d range){	base::SetPoleTwistRange(range);}	\
	void GetPoleTwistRange(Vec2d& range){	base::GetPoleTwistRange(range);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHRayIf(base)	\
	Vec3d GetOrigin(){	return	base::GetOrigin();}	\
	void SetOrigin(const Vec3d& ori){	base::SetOrigin(ori);}	\
	Vec3d GetDirection(){	return	base::GetDirection();}	\
	void SetDirection(const Vec3d& dir){	base::SetDirection(dir);}	\
	void Apply(){	base::Apply();}	\
	int NHits(){	return	base::NHits();}	\
	Spr::PHRaycastHit* GetHits(){	return	base::GetHits();}	\
	Spr::PHRaycastHit* GetNearest(){	return	base::GetNearest();}	\
	Spr::PHRaycastHit* GetDynamicalNearest(){	return	base::GetDynamicalNearest();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSceneIf(base)	\
	Spr::PHSdkIf* GetSdk(){	return	base::GetSdk();}	\
	Spr::PHSolidIf* CreateSolid(const PHSolidDesc& desc){	return	base::CreateSolid(desc);}	\
	int NSolids()const{	return	base::NSolids();}	\
	Spr::PHSolidIf** GetSolids(){	return	base::GetSolids();}	\
	int NSolidPairs()const{	return	base::NSolidPairs();}	\
	PHSolidPairForLCPIf* GetSolidPair(int i, int j){	return	base::GetSolidPair(i, j);}	\
	PHSolidPairForLCPIf* GetSolidPair(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs, bool& bSwap){	return	base::GetSolidPair(lhs, rhs, bSwap);}	\
	void SetContactMode(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs, Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(lhs, rhs, mode);}	\
	void SetContactMode(Spr::PHSolidIf** group, size_t length, Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(group, length, mode);}	\
	void SetContactMode(Spr::PHSolidIf* solid, Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(solid, mode);}	\
	void SetContactMode(Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(mode);}	\
	Spr::PHJointIf* CreateJoint(Spr::PHSolidIf* lhs, Spr::PHSolidIf* rhs, const IfInfo* ii, const Spr::PHJointDesc& desc){	return	base::CreateJoint(lhs, rhs, ii, desc);}	\
	int NJoints()const{	return	base::NJoints();}	\
	Spr::PHJointIf* GetJoint(int i){	return	base::GetJoint(i);}	\
	int NContacts()const{	return	base::NContacts();}	\
	Spr::PHContactPointIf* GetContact(int i){	return	base::GetContact(i);}	\
	int NFemMeshes()const{	return	base::NFemMeshes();}	\
	Spr::PHFemMeshIf* GetFemMesh(int i){	return	base::GetFemMesh(i);}	\
	Spr::PHRootNodeIf* CreateRootNode(Spr::PHSolidIf* root, const Spr::PHRootNodeDesc& desc){	return	base::CreateRootNode(root, desc);}	\
	int NRootNodes()const{	return	base::NRootNodes();}	\
	Spr::PHRootNodeIf* GetRootNode(int i){	return	base::GetRootNode(i);}	\
	Spr::PHTreeNodeIf* CreateTreeNode(Spr::PHTreeNodeIf* parent, Spr::PHSolidIf* child, const Spr::PHTreeNodeDesc& desc){	return	base::CreateTreeNode(parent, child, desc);}	\
	Spr::PHGearIf* CreateGear(Spr::PH1DJointIf* lhs, Spr::PH1DJointIf* rhs, const Spr::PHGearDesc& desc){	return	base::CreateGear(lhs, rhs, desc);}	\
	int NGears()const{	return	base::NGears();}	\
	Spr::PHGearIf* GetGear(int i){	return	base::GetGear(i);}	\
	Spr::PHPathIf* CreatePath(const Spr::PHPathDesc& desc){	return	base::CreatePath(desc);}	\
	int NPaths(){	return	base::NPaths();}	\
	Spr::PHPathIf* GetPath(int i){	return	base::GetPath(i);}	\
	Spr::PHRayIf* CreateRay(const Spr::PHRayDesc& desc){	return	base::CreateRay(desc);}	\
	int NRays(){	return	base::NRays();}	\
	Spr::PHRayIf* GetRay(int i){	return	base::GetRay(i);}	\
	Spr::PHIKActuatorIf* CreateIKActuator(const IfInfo* ii, const Spr::PHIKActuatorDesc& desc){	return	base::CreateIKActuator(ii, desc);}	\
	int NIKActuators(){	return	base::NIKActuators();}	\
	Spr::PHIKActuatorIf* GetIKActuator(int i){	return	base::GetIKActuator(i);}	\
	Spr::PHIKEndEffectorIf* CreateIKEndEffector(const Spr::PHIKEndEffectorDesc& desc){	return	base::CreateIKEndEffector(desc);}	\
	int NIKEndEffectors(){	return	base::NIKEndEffectors();}	\
	Spr::PHIKEndEffectorIf* GetIKEndEffector(int i){	return	base::GetIKEndEffector(i);}	\
	double GetTimeStep()const{	return	base::GetTimeStep();}	\
	double GetTimeStepInv()const{	return	base::GetTimeStepInv();}	\
	void SetTimeStep(double dt){	base::SetTimeStep(dt);}	\
	double GetHapticTimeStep()const{	return	base::GetHapticTimeStep();}	\
	void SetHapticTimeStep(double dt){	base::SetHapticTimeStep(dt);}	\
	unsigned int GetCount()const{	return	base::GetCount();}	\
	void SetCount(unsigned int count){	base::SetCount(count);}	\
	void SetGravity(const Vec3d& accel){	base::SetGravity(accel);}	\
	Vec3d GetGravity(){	return	base::GetGravity();}	\
	void SetAirResistanceRate(double rate){	base::SetAirResistanceRate(rate);}	\
	double GetAirResistanceRate(){	return	base::GetAirResistanceRate();}	\
	int GetNumIteration(){	return	base::GetNumIteration();}	\
	void SetNumIteration(int n){	base::SetNumIteration(n);}	\
	void SetStateMode(bool bConstraints){	base::SetStateMode(bConstraints);}	\
	void Step(){	base::Step();}	\
	void ClearForce(){	base::ClearForce();}	\
	void GenerateForce(){	base::GenerateForce();}	\
	void Integrate(){	base::Integrate();}	\
	void IntegratePart1(){	base::IntegratePart1();}	\
	void IntegratePart2(){	base::IntegratePart2();}	\
	void Clear(){	base::Clear();}	\
	int NEngines(){	return	base::NEngines();}	\
	Spr::PHEngineIf* GetEngine(int i){	return	base::GetEngine(i);}	\
	Spr::PHConstraintEngineIf* GetConstraintEngine(){	return	base::GetConstraintEngine();}	\
	Spr::PHGravityEngineIf* GetGravityEngine(){	return	base::GetGravityEngine();}	\
	Spr::PHPenaltyEngineIf* GetPenaltyEngine(){	return	base::GetPenaltyEngine();}	\
	Spr::PHIKEngineIf* GetIKEngine(){	return	base::GetIKEngine();}	\
	Spr::PHHapticEngineIf* GetHapticEngine(){	return	base::GetHapticEngine();}	\
	Spr::PHHapticPointerIf* CreateHapticPointer(){	return	base::CreateHapticPointer();}	\
	void StepHapticLoop(){	base::StepHapticLoop();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSdkIf(base)	\
	Spr::PHSceneIf* CreateScene(const Spr::PHSceneDesc& desc){	return	base::CreateScene(desc);}	\
	int NScene(){	return	base::NScene();}	\
	Spr::PHSceneIf* GetScene(int i){	return	base::GetScene(i);}	\
	void MergeScene(Spr::PHSceneIf* scene0, Spr::PHSceneIf* scene1){	base::MergeScene(scene0, scene1);}	\
	Spr::CDShapeIf* CreateShape(const IfInfo* ii, const Spr::CDShapeDesc& desc){	return	base::CreateShape(ii, desc);}	\
	int NShape(){	return	base::NShape();}	\
	Spr::CDShapeIf* GetShape(int i){	return	base::GetShape(i);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHFrameIf(base)	\
	Posed GetPose(){	return	base::GetPose();}	\
	void SetPose(Posed p){	base::SetPose(p);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSolidIf(base)	\
	void AddForce(Vec3d f){	base::AddForce(f);}	\
	void AddTorque(Vec3d t){	base::AddTorque(t);}	\
	void AddForce(Vec3d f, Vec3d r){	base::AddForce(f, r);}	\
	double GetMass(){	return	base::GetMass();}	\
	double GetMassInv(){	return	base::GetMassInv();}	\
	void SetMass(double m){	base::SetMass(m);}	\
	Vec3d GetCenterOfMass(){	return	base::GetCenterOfMass();}	\
	void SetCenterOfMass(const Vec3d& center){	base::SetCenterOfMass(center);}	\
	Matrix3d GetInertia(){	return	base::GetInertia();}	\
	Matrix3d GetInertiaInv(){	return	base::GetInertiaInv();}	\
	void SetInertia(const Matrix3d& I){	base::SetInertia(I);}	\
	void CompInertia(){	base::CompInertia();}	\
	Vec3d GetFramePosition()const{	return	base::GetFramePosition();}	\
	void SetFramePosition(const Vec3d& p){	base::SetFramePosition(p);}	\
	Vec3d GetCenterPosition()const{	return	base::GetCenterPosition();}	\
	void SetCenterPosition(const Vec3d& p){	base::SetCenterPosition(p);}	\
	Vec3d GetDeltaPosition()const{	return	base::GetDeltaPosition();}	\
	Vec3d GetDeltaPosition(const Vec3d& p)const{	return	base::GetDeltaPosition(p);}	\
	Quaterniond GetOrientation()const{	return	base::GetOrientation();}	\
	void SetOrientation(const Quaterniond& q){	base::SetOrientation(q);}	\
	Posed GetPose()const{	return	base::GetPose();}	\
	void SetPose(const Posed& p){	base::SetPose(p);}	\
	Vec3d GetVelocity()const{	return	base::GetVelocity();}	\
	void SetVelocity(const Vec3d& v){	base::SetVelocity(v);}	\
	Vec3d GetAngularVelocity()const{	return	base::GetAngularVelocity();}	\
	void SetAngularVelocity(const Vec3d& av){	base::SetAngularVelocity(av);}	\
	Vec3d GetForce()const{	return	base::GetForce();}	\
	Vec3d GetTorque()const{	return	base::GetTorque();}	\
	void AddShape(Spr::CDShapeIf* shape){	base::AddShape(shape);}	\
	int NShape(){	return	base::NShape();}	\
	Spr::CDShapeIf* GetShape(int index){	return	base::GetShape(index);}	\
	Posed GetShapePose(int index){	return	base::GetShapePose(index);}	\
	void SetShapePose(int index, const Posed& pose){	base::SetShapePose(index, pose);}	\
	void ClearShape(){	base::ClearShape();}	\
	void SetGravity(bool bOn){	base::SetGravity(bOn);}	\
	void SetDynamical(bool bOn){	base::SetDynamical(bOn);}	\
	bool IsDynamical(){	return	base::IsDynamical();}	\
	Spr::PHTreeNodeIf* GetTreeNode(){	return	base::GetTreeNode();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHShapePairForLCPIf(base)	\
	int NSectionVertexes(){	return	base::NSectionVertexes();}	\
	Vec3d GetSectionVertex(int i){	return	base::GetSectionVertex(i);}	\
	double GetContactDimension(){	return	base::GetContactDimension();}	\
	Vec3d GetNormalVector(){	return	base::GetNormalVector();}	\
	void GetClosestPoints(Vec3d& pa, Vec3d& pb){	base::GetClosestPoints(pa, pb);}	\
	Spr::CDShapeIf* GetShape(int i){	return	base::GetShape(i);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSolidPairForLCPIf(base)	\
	Spr::PHSolidIf* GetSolid(int i){	return	base::GetSolid(i);}	\
	int GetContactState(int i, int j){	return	base::GetContactState(i, j);}	\
	Vec3d GetCommonPoint(int i, int j){	return	base::GetCommonPoint(i, j);}	\
	unsigned int GetLastContactCount(int i, int j){	return	base::GetLastContactCount(i, j);}	\
	double GetContactDepth(int i, int j){	return	base::GetContactDepth(i, j);}	\
	Spr::PHShapePairForLCPIf* GetShapePair(int i, int j){	return	base::GetShapePair(i, j);}	\

