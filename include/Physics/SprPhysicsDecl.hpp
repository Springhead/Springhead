#define SPR_OVERRIDEMEMBERFUNCOF_PHEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHConstraintEngineIf(base)	\
	PHConstraintsIf* GetContactPoints(){	return	base::GetContactPoints();}	\
	void SetVelCorrectionRate(double value){	base::SetVelCorrectionRate(value);}	\
	void SetPosCorrectionRate(double value){	base::SetPosCorrectionRate(value);}	\
	void SetContactCorrectionRate(double value){	base::SetContactCorrectionRate(value);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHGravityEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPenaltyEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKEngineIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKControlPointIf(base)	\
	void SetGoal(Vec3d goal){	base::SetGoal(goal);}	\
	Vec3d GetGoal(){	return	base::GetGoal();}	\
	void SetSolid(PHSolidIf* solid){	base::SetSolid(solid);}	\
	PHSolidIf* GetSolid(){	return	base::GetSolid();}	\
	void SetNumber(int i){	base::SetNumber(i);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKPosCtlIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKOriCtlIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKNodeIf(base)	\
	void PrepareSolve(){	base::PrepareSolve();}	\
	void ProceedSolve(){	base::ProceedSolve();}	\
	PTM::VVector< double > GetDTheta(){	return	base::GetDTheta();}	\
	void Move(){	base::Move();}	\
	void AddControlPoint(Spr::PHIKControlPointIf* control){	base::AddControlPoint(control);}	\
	void SetBias(float bias){	base::SetBias(bias);}	\
	float GetBias(){	return	base::GetBias();}	\
	void SetNumber(int i){	base::SetNumber(i);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKSolidIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKBallJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHIKHingeJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHConstraintIf(base)	\
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
	void DisableTrajectoryVelocity(){	base::DisableTrajectoryVelocity();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHConstraintsIf(base)	\
	Spr::PHConstraintIf* FindBySolidPair(PHSolidIf* lhs, PHSolidIf* rhs){	return	base::FindBySolidPair(lhs, rhs);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHContactPointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHJoint1DIf(base)	\
	void SetRange(double lower, double upper){	base::SetRange(lower, upper);}	\
	void GetRange(double& lower, double& upper){	base::GetRange(lower, upper);}	\
	void SetMotorTorque(double t){	base::SetMotorTorque(t);}	\
	double GetMotorTorque(){	return	base::GetMotorTorque();}	\
	void SetDesiredVelocity(double v){	base::SetDesiredVelocity(v);}	\
	double GetDesiredVelocity(){	return	base::GetDesiredVelocity();}	\
	void SetTrajectoryVelocity(double v){	base::SetTrajectoryVelocity(v);}	\
	double GetTrajectoryVelocity(){	return	base::GetTrajectoryVelocity();}	\
	void SetSpring(double spring){	base::SetSpring(spring);}	\
	double GetSpring(){	return	base::GetSpring();}	\
	void SetSpringOrigin(double origin){	base::SetSpringOrigin(origin);}	\
	double GetSpringOrigin(){	return	base::GetSpringOrigin();}	\
	void SetDamper(double damper){	base::SetDamper(damper);}	\
	double GetDamper(){	return	base::GetDamper();}	\
	double GetPosition(){	return	base::GetPosition();}	\
	double GetVelocity(){	return	base::GetVelocity();}	\
	void SetTorqueMax(double max){	base::SetTorqueMax(max);}	\
	double GetTorqueMax(){	return	base::GetTorqueMax();}	\
	void SetTorqueMin(double min){	base::SetTorqueMin(min);}	\
	double GetTorqueMin(){	return	base::GetTorqueMin();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHHingeJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSliderJointIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPathIf(base)	\
	void AddPoint(double s, const Posed& pose){	base::AddPoint(s, pose);}	\
	void SetLoop(bool bOnOff){	base::SetLoop(bOnOff);}	\
	bool IsLoop(){	return	base::IsLoop();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPathJointIf(base)	\
	void SetPosition(double q){	base::SetPosition(q);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointIf(base)	\
	void SetSwingRange(Vec2d range){	base::SetSwingRange(range);}	\
	void GetSwingRange(Vec2d& range){	base::GetSwingRange(range);}	\
	void SetTwistRange(Vec2d range){	base::SetTwistRange(range);}	\
	void GetTwistRange(Vec2d& range){	base::GetTwistRange(range);}	\
	void SetMotorTorque(const Vec3d& torque){	base::SetMotorTorque(torque);}	\
	Vec3d GetMotorTorque(){	return	base::GetMotorTorque();}	\
	Vec3d GetAngle(){	return	base::GetAngle();}	\
	Quaterniond GetPosition(){	return	base::GetPosition();}	\
	Vec3d GetVelocity(){	return	base::GetVelocity();}	\
	void SetTorqueMax(double max){	base::SetTorqueMax(max);}	\
	double GetTorqueMax(){	return	base::GetTorqueMax();}	\
	void SetTorqueMin(double min){	base::SetTorqueMin(min);}	\
	double GetTorqueMin(){	return	base::GetTorqueMin();}	\
	void SetDesiredVelocity(Vec3d q){	base::SetDesiredVelocity(q);}	\
	Vec3d GetDesiredVelocity(){	return	base::GetDesiredVelocity();}	\
	void SetTrajectoryVelocity(Vec3d q){	base::SetTrajectoryVelocity(q);}	\
	Vec3d GetTrajectoryVelocity(){	return	base::GetTrajectoryVelocity();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PH3ElementBallJointIf(base)	\
	double GetSecondDamper(){	return	base::GetSecondDamper();}	\
	void SetSecondDamper(double input){	base::SetSecondDamper(input);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSpringIf(base)	\
	void SetSpring(const Vec3d& spring){	base::SetSpring(spring);}	\
	Vec3d GetSpring(){	return	base::GetSpring();}	\
	void SetDamper(const Vec3d& damper){	base::SetDamper(damper);}	\
	Vec3d GetDamper(){	return	base::GetDamper();}	\
	void SetSpringOri(const double spring){	base::SetSpringOri(spring);}	\
	double GetSpringOri(){	return	base::GetSpringOri();}	\
	void SetDamperOri(const double damper){	base::SetDamperOri(damper);}	\
	double GetDamperOri(){	return	base::GetDamperOri();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PH3ElementIf(base)	\
	void SetSecondDamper(const Vec3d& secondDamper){	base::SetSecondDamper(secondDamper);}	\
	Vec3d GetSecondDamper(){	return	base::GetSecondDamper();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHTreeNodeIf(base)	\
	void Enable(bool bEnable){	base::Enable(bEnable);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHRootNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHTreeNode1DIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHHingeJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSliderJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHPathJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHBallJointNodeIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHGearIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHRayIf(base)	\
	Vec3d GetOrigin(){	return	base::GetOrigin();}	\
	void SetOrigin(const Vec3d& ori){	base::SetOrigin(ori);}	\
	Vec3d GetDirection(){	return	base::GetDirection();}	\
	void SetDirection(const Vec3d& dir){	base::SetDirection(dir);}	\
	void Apply(){	base::Apply();}	\
	int NHits(){	return	base::NHits();}	\
	Spr::PHRaycastHit* GetHits(){	return	base::GetHits();}	\
	Spr::PHRaycastHit* GetNearest(){	return	base::GetNearest();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSceneIf(base)	\
	Spr::PHSdkIf* GetSdk(){	return	base::GetSdk();}	\
	PHSolidIf* CreateSolid(const PHSolidDesc& desc){	return	base::CreateSolid(desc);}	\
	int NSolids()const{	return	base::NSolids();}	\
	PHSolidIf** GetSolids(){	return	base::GetSolids();}	\
	int NSolidPairs()const{	return	base::NSolidPairs();}	\
	PHSolidPairForLCPIf* GetSolidPair(int i, int j){	return	base::GetSolidPair(i, j);}	\
	void SetContactMode(PHSolidIf* lhs, PHSolidIf* rhs, Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(lhs, rhs, mode);}	\
	void SetContactMode(PHSolidIf** group, size_t length, Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(group, length, mode);}	\
	void SetContactMode(PHSolidIf* solid, Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(solid, mode);}	\
	void SetContactMode(Spr::PHSceneDesc::ContactMode mode){	base::SetContactMode(mode);}	\
	int GetNumIteration(){	return	base::GetNumIteration();}	\
	void SetNumIteration(int n){	base::SetNumIteration(n);}	\
	Spr::PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const IfInfo* ii, const Spr::PHJointDesc& desc){	return	base::CreateJoint(lhs, rhs, ii, desc);}	\
	int NJoints()const{	return	base::NJoints();}	\
	Spr::PHConstraintIf* GetJoint(int i){	return	base::GetJoint(i);}	\
	int NContacts()const{	return	base::NContacts();}	\
	Spr::PHContactPointIf* GetContact(int i){	return	base::GetContact(i);}	\
	Spr::PHRootNodeIf* CreateRootNode(PHSolidIf* root, const Spr::PHRootNodeDesc& desc){	return	base::CreateRootNode(root, desc);}	\
	int NRootNodes()const{	return	base::NRootNodes();}	\
	Spr::PHRootNodeIf* GetRootNode(int i){	return	base::GetRootNode(i);}	\
	Spr::PHTreeNodeIf* CreateTreeNode(Spr::PHTreeNodeIf* parent, PHSolidIf* child, const Spr::PHTreeNodeDesc& desc){	return	base::CreateTreeNode(parent, child, desc);}	\
	Spr::PHGearIf* CreateGear(Spr::PHJoint1DIf* lhs, Spr::PHJoint1DIf* rhs, const Spr::PHGearDesc& desc){	return	base::CreateGear(lhs, rhs, desc);}	\
	int NGears()const{	return	base::NGears();}	\
	Spr::PHGearIf* GetGear(int i){	return	base::GetGear(i);}	\
	Spr::PHPathIf* CreatePath(const Spr::PHPathDesc& desc){	return	base::CreatePath(desc);}	\
	int NPaths(){	return	base::NPaths();}	\
	Spr::PHPathIf* GetPath(int i){	return	base::GetPath(i);}	\
	Spr::PHRayIf* CreateRay(const Spr::PHRayDesc& desc){	return	base::CreateRay(desc);}	\
	int NRays(){	return	base::NRays();}	\
	Spr::PHRayIf* GetRay(int i){	return	base::GetRay(i);}	\
	Spr::PHIKNodeIf* CreateIKNode(const IfInfo* ii, const Spr::PHIKNodeDesc& desc){	return	base::CreateIKNode(ii, desc);}	\
	Spr::PHIKControlPointIf* CreateIKControlPoint(const IfInfo* ii, const Spr::PHIKControlPointDesc& desc){	return	base::CreateIKControlPoint(ii, desc);}	\
	double GetTimeStep()const{	return	base::GetTimeStep();}	\
	void SetTimeStep(double dt){	base::SetTimeStep(dt);}	\
	unsigned int GetCount()const{	return	base::GetCount();}	\
	void SetCount(unsigned int count){	base::SetCount(count);}	\
	void Step(){	base::Step();}	\
	void ClearForce(){	base::ClearForce();}	\
	void GenerateForce(){	base::GenerateForce();}	\
	void Integrate(){	base::Integrate();}	\
	void IntegratePart1(){	base::IntegratePart1();}	\
	void IntegratePart2(){	base::IntegratePart2();}	\
	void Clear(){	base::Clear();}	\
	void SetGravity(const Vec3d& accel){	base::SetGravity(accel);}	\
	Vec3d GetGravity(){	return	base::GetGravity();}	\
	int NEngines(){	return	base::NEngines();}	\
	Spr::PHEngineIf* GetEngine(int i){	return	base::GetEngine(i);}	\
	Spr::PHConstraintEngineIf* GetConstraintEngine(){	return	base::GetConstraintEngine();}	\
	Spr::PHGravityEngineIf* GetGravityEngine(){	return	base::GetGravityEngine();}	\
	Spr::PHPenaltyEngineIf* GetPenaltyEngine(){	return	base::GetPenaltyEngine();}	\
	void SetStateMode(bool bConstraints){	base::SetStateMode(bConstraints);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSdkIf(base)	\
	Spr::PHSceneIf* CreateScene(){	return	base::CreateScene();}	\
	Spr::PHSceneIf* CreateScene(const Spr::PHSceneDesc& desc){	return	base::CreateScene(desc);}	\
	int NScene(){	return	base::NScene();}	\
	Spr::PHSceneIf* GetScene(size_t i){	return	base::GetScene(i);}	\
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
	Vec3d GetDeltaPosition(const Vec3d& p)const{	return	base::GetDeltaPosition(p);}	\
	Vec3d GetDeltaPosition()const{	return	base::GetDeltaPosition();}	\
	Quaterniond GetOrientation()const{	return	base::GetOrientation();}	\
	void SetOrientation(const Quaterniond& q){	base::SetOrientation(q);}	\
	Posed GetPose()const{	return	base::GetPose();}	\
	Vec3d GetPoseR()const{	return	base::GetPoseR();}	\
	Quaterniond GetPoseQ()const{	return	base::GetPoseQ();}	\
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
	void SetGravity(bool bOn){	base::SetGravity(bOn);}	\
	void SetDynamical(bool bOn){	base::SetDynamical(bOn);}	\
	bool IsDynamical(){	return	base::IsDynamical();}	\
	void SetFrozen(bool bOn){	base::SetFrozen(bOn);}	\
	bool IsFrozen(){	return	base::IsFrozen();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_PHSolidPairForLCPIf(base)	\
	Spr::PHSolidIf* GetSolid(int i){	return	base::GetSolid(i);}	\
	int GetContactState(int i, int j){	return	base::GetContactState(i, j);}	\
	Vec3d GetCommonPoint(int i, int j){	return	base::GetCommonPoint(i, j);}	\

