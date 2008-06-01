#define SPR_DECLMEMBEROF_PHConstraintDesc \
protected:\
	bool	bEnabled;	\
	Posed	poseSocket;	\
	Posed	posePlug;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHConstraintDesc*)ptr)->bEnabled;	\
		poseSocket = ((PHConstraintDesc*)ptr)->poseSocket;	\
		posePlug = ((PHConstraintDesc*)ptr)->posePlug;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHConstraintDesc*)ptr)->bEnabled = bEnabled;	\
		((PHConstraintDesc*)ptr)->poseSocket = poseSocket;	\
		((PHConstraintDesc*)ptr)->posePlug = posePlug;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHJoint1DDesc \
protected:\
	double	lower;	\
	double	upper;	\
	double	spring;	\
	double	origin;	\
	double	damper;	\
	double	desiredVelocity;	\
	double	torque;	\
	double	fMax;	\
	double	fMin;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHJoint1DDesc*)ptr);	\
		lower = ((PHJoint1DDesc*)ptr)->lower;	\
		upper = ((PHJoint1DDesc*)ptr)->upper;	\
		spring = ((PHJoint1DDesc*)ptr)->spring;	\
		origin = ((PHJoint1DDesc*)ptr)->origin;	\
		damper = ((PHJoint1DDesc*)ptr)->damper;	\
		desiredVelocity = ((PHJoint1DDesc*)ptr)->desiredVelocity;	\
		torque = ((PHJoint1DDesc*)ptr)->torque;	\
		fMax = ((PHJoint1DDesc*)ptr)->fMax;	\
		fMin = ((PHJoint1DDesc*)ptr)->fMin;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHJoint1DDesc*)ptr);	\
		((PHJoint1DDesc*)ptr)->lower = lower;	\
		((PHJoint1DDesc*)ptr)->upper = upper;	\
		((PHJoint1DDesc*)ptr)->spring = spring;	\
		((PHJoint1DDesc*)ptr)->origin = origin;	\
		((PHJoint1DDesc*)ptr)->damper = damper;	\
		((PHJoint1DDesc*)ptr)->desiredVelocity = desiredVelocity;	\
		((PHJoint1DDesc*)ptr)->torque = torque;	\
		((PHJoint1DDesc*)ptr)->fMax = fMax;	\
		((PHJoint1DDesc*)ptr)->fMin = fMin;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHTreeNodeDesc \
protected:\
	bool	bEnabled;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHTreeNodeDesc*)ptr)->bEnabled;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHTreeNodeDesc*)ptr)->bEnabled = bEnabled;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHRootNodeDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode::SetDesc((PHTreeNodeDesc*)(PHRootNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode::GetDesc((PHTreeNodeDesc*)(PHRootNodeDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHTreeNode1DDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode::SetDesc((PHTreeNodeDesc*)(PHTreeNode1DDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode::GetDesc((PHTreeNodeDesc*)(PHTreeNode1DDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHHingeJointNodeDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode1D::SetDesc((PHTreeNode1DDesc*)(PHHingeJointNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode1D::GetDesc((PHTreeNode1DDesc*)(PHHingeJointNodeDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSliderJointNodeDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode1D::SetDesc((PHTreeNode1DDesc*)(PHSliderJointNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode1D::GetDesc((PHTreeNode1DDesc*)(PHSliderJointNodeDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPathJointNodeDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode1D::SetDesc((PHTreeNode1DDesc*)(PHPathJointNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode1D::GetDesc((PHTreeNode1DDesc*)(PHPathJointNodeDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointNodeDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode::SetDesc((PHTreeNodeDesc*)(PHBallJointNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode::GetDesc((PHTreeNodeDesc*)(PHBallJointNodeDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHGearDesc \
protected:\
	double	ratio;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		ratio = ((PHGearDesc*)ptr)->ratio;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHGearDesc*)ptr)->ratio = ratio;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHHingeJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint1D::SetDesc((PHJoint1DDesc*)(PHHingeJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint1D::GetDesc((PHJoint1DDesc*)(PHHingeJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSliderJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint1D::SetDesc((PHJoint1DDesc*)(PHSliderJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint1D::GetDesc((PHJoint1DDesc*)(PHSliderJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPathDesc \
protected:\
	std::vector<(Spr::PHPathPoint)>	points;	\
	bool	bLoop;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		points = ((PHPathDesc*)ptr)->points;	\
		bLoop = ((PHPathDesc*)ptr)->bLoop;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHPathDesc*)ptr)->points = points;	\
		((PHPathDesc*)ptr)->bLoop = bLoop;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPathJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint1D::SetDesc((PHJoint1DDesc*)(PHPathJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint1D::GetDesc((PHJoint1DDesc*)(PHPathJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointDesc \
protected:\
	double	spring;	\
	double	damper;	\
	Vec2d	limitSwing;	\
	Vec2d	limitTwist;	\
	Vec3d	limitDir;	\
	Quaterniond	goal;	\
	Vec3d	torque;	\
	double	fMax;	\
	double	fMin;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		spring = ((PHBallJointDesc*)ptr)->spring;	\
		damper = ((PHBallJointDesc*)ptr)->damper;	\
		limitSwing = ((PHBallJointDesc*)ptr)->limitSwing;	\
		limitTwist = ((PHBallJointDesc*)ptr)->limitTwist;	\
		limitDir = ((PHBallJointDesc*)ptr)->limitDir;	\
		goal = ((PHBallJointDesc*)ptr)->goal;	\
		torque = ((PHBallJointDesc*)ptr)->torque;	\
		fMax = ((PHBallJointDesc*)ptr)->fMax;	\
		fMin = ((PHBallJointDesc*)ptr)->fMin;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		((PHBallJointDesc*)ptr)->spring = spring;	\
		((PHBallJointDesc*)ptr)->damper = damper;	\
		((PHBallJointDesc*)ptr)->limitSwing = limitSwing;	\
		((PHBallJointDesc*)ptr)->limitTwist = limitTwist;	\
		((PHBallJointDesc*)ptr)->limitDir = limitDir;	\
		((PHBallJointDesc*)ptr)->goal = goal;	\
		((PHBallJointDesc*)ptr)->torque = torque;	\
		((PHBallJointDesc*)ptr)->fMax = fMax;	\
		((PHBallJointDesc*)ptr)->fMin = fMin;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSpringDesc \
protected:\
	Vec3d	spring;	\
	Vec3d	damper;	\
	double	springOri;	\
	double	damperOri;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHConstraint::SetDesc((PHConstraintDesc*)(PHSpringDesc*)ptr);	\
		spring = ((PHSpringDesc*)ptr)->spring;	\
		damper = ((PHSpringDesc*)ptr)->damper;	\
		springOri = ((PHSpringDesc*)ptr)->springOri;	\
		damperOri = ((PHSpringDesc*)ptr)->damperOri;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHConstraint::GetDesc((PHConstraintDesc*)(PHSpringDesc*)ptr);	\
		((PHSpringDesc*)ptr)->spring = spring;	\
		((PHSpringDesc*)ptr)->damper = damper;	\
		((PHSpringDesc*)ptr)->springOri = springOri;	\
		((PHSpringDesc*)ptr)->damperOri = damperOri;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSceneState \
protected:\
	double	timeStep;	\
	unsigned int	count;	\
public:\
	virtual void SetState(const void* ptr){ \
		timeStep = ((PHSceneState*)ptr)->timeStep;	\
		count = ((PHSceneState*)ptr)->count;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHSceneState*)ptr)->timeStep = timeStep;	\
		((PHSceneState*)ptr)->count = count;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSceneDesc \
protected:\
	Vec3f	gravity;	\
	int	numIteration;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHScene::SetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		gravity = ((PHSceneDesc*)ptr)->gravity;	\
		numIteration = ((PHSceneDesc*)ptr)->numIteration;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHScene::GetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		((PHSceneDesc*)ptr)->gravity = gravity;	\
		((PHSceneDesc*)ptr)->numIteration = numIteration;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSdkDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFrameDesc \
protected:\
	Posed	pose;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		pose = ((PHFrameDesc*)ptr)->pose;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHFrameDesc*)ptr)->pose = pose;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSolidState \
protected:\
	Vec3d	velocity;	\
	Vec3d	angVelocity;	\
	Posed	pose;	\
	Vec3d	force;	\
	Vec3d	torque;	\
	Vec3d	nextForce;	\
	Vec3d	nextTorque;	\
public:\
	virtual void SetState(const void* ptr){ \
		velocity = ((PHSolidState*)ptr)->velocity;	\
		angVelocity = ((PHSolidState*)ptr)->angVelocity;	\
		pose = ((PHSolidState*)ptr)->pose;	\
		force = ((PHSolidState*)ptr)->force;	\
		torque = ((PHSolidState*)ptr)->torque;	\
		nextForce = ((PHSolidState*)ptr)->nextForce;	\
		nextTorque = ((PHSolidState*)ptr)->nextTorque;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHSolidState*)ptr)->velocity = velocity;	\
		((PHSolidState*)ptr)->angVelocity = angVelocity;	\
		((PHSolidState*)ptr)->pose = pose;	\
		((PHSolidState*)ptr)->force = force;	\
		((PHSolidState*)ptr)->torque = torque;	\
		((PHSolidState*)ptr)->nextForce = nextForce;	\
		((PHSolidState*)ptr)->nextTorque = nextTorque;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSolidDesc \
protected:\
	double	mass;	\
	Matrix3d	inertia;	\
	Vec3d	center;	\
	bool	dynamical;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHSolid::SetState((PHSolidState*)(PHSolidDesc*)ptr);	\
		mass = ((PHSolidDesc*)ptr)->mass;	\
		inertia = ((PHSolidDesc*)ptr)->inertia;	\
		center = ((PHSolidDesc*)ptr)->center;	\
		dynamical = ((PHSolidDesc*)ptr)->dynamical;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHSolid::GetState((PHSolidState*)(PHSolidDesc*)ptr);	\
		((PHSolidDesc*)ptr)->mass = mass;	\
		((PHSolidDesc*)ptr)->inertia = inertia;	\
		((PHSolidDesc*)ptr)->center = center;	\
		((PHSolidDesc*)ptr)->dynamical = dynamical;	\
		return true;	\
	}\

