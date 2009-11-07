#define SPR_DECLMEMBEROF_PHIKControlPointDesc \
protected:\
	bool	isEnabled;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		isEnabled = ((PHIKControlPointDesc*)ptr)->isEnabled;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHIKControlPointDesc*)ptr)->isEnabled = isEnabled;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKPosCtlDesc \
protected:\
	Vec3d	pos;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKControlPoint::SetDesc((PHIKControlPointDesc*)(PHIKPosCtlDesc*)ptr);	\
		pos = ((PHIKPosCtlDesc*)ptr)->pos;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKControlPoint::GetDesc((PHIKControlPointDesc*)(PHIKPosCtlDesc*)ptr);	\
		((PHIKPosCtlDesc*)ptr)->pos = pos;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKOriCtlDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKControlPoint::SetDesc((PHIKControlPointDesc*)(PHIKOriCtlDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKControlPoint::GetDesc((PHIKControlPointDesc*)(PHIKOriCtlDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKNodeDesc \
protected:\
	bool	isEnabled;	\
	float	bias;	\
	double	spring;	\
	double	damper;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		isEnabled = ((PHIKNodeDesc*)ptr)->isEnabled;	\
		bias = ((PHIKNodeDesc*)ptr)->bias;	\
		spring = ((PHIKNodeDesc*)ptr)->spring;	\
		damper = ((PHIKNodeDesc*)ptr)->damper;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHIKNodeDesc*)ptr)->isEnabled = isEnabled;	\
		((PHIKNodeDesc*)ptr)->bias = bias;	\
		((PHIKNodeDesc*)ptr)->spring = spring;	\
		((PHIKNodeDesc*)ptr)->damper = damper;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKSolidDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKNode::SetDesc((PHIKNodeDesc*)(PHIKSolidDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKNode::GetDesc((PHIKNodeDesc*)(PHIKSolidDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKBallJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKNode::SetDesc((PHIKNodeDesc*)(PHIKBallJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKNode::GetDesc((PHIKNodeDesc*)(PHIKBallJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKHingeJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKNode::SetDesc((PHIKNodeDesc*)(PHIKHingeJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKNode::GetDesc((PHIKNodeDesc*)(PHIKHingeJointDesc*)ptr);	\
		return true;	\
	}\

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

#define SPR_DECLMEMBEROF_PHJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHConstraint::SetDesc((PHConstraintDesc*)(PHJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHConstraint::GetDesc((PHConstraintDesc*)(PHJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHJoint1DDesc \
protected:\
	double	lower;	\
	double	upper;	\
	double	spring;	\
	double	targetPosition;	\
	double	damper;	\
	double	secondDamper;	\
	double	targetVelocity;	\
	double	offsetForce;	\
	double	rangeSpring;	\
	double	rangeDamper;	\
	double	fMax;	\
	double	fMin;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHJoint1DDesc*)ptr);	\
		lower = ((PHJoint1DDesc*)ptr)->lower;	\
		upper = ((PHJoint1DDesc*)ptr)->upper;	\
		spring = ((PHJoint1DDesc*)ptr)->spring;	\
		targetPosition = ((PHJoint1DDesc*)ptr)->targetPosition;	\
		damper = ((PHJoint1DDesc*)ptr)->damper;	\
		secondDamper = ((PHJoint1DDesc*)ptr)->secondDamper;	\
		targetVelocity = ((PHJoint1DDesc*)ptr)->targetVelocity;	\
		offsetForce = ((PHJoint1DDesc*)ptr)->offsetForce;	\
		rangeSpring = ((PHJoint1DDesc*)ptr)->rangeSpring;	\
		rangeDamper = ((PHJoint1DDesc*)ptr)->rangeDamper;	\
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
		((PHJoint1DDesc*)ptr)->targetPosition = targetPosition;	\
		((PHJoint1DDesc*)ptr)->damper = damper;	\
		((PHJoint1DDesc*)ptr)->secondDamper = secondDamper;	\
		((PHJoint1DDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PHJoint1DDesc*)ptr)->offsetForce = offsetForce;	\
		((PHJoint1DDesc*)ptr)->rangeSpring = rangeSpring;	\
		((PHJoint1DDesc*)ptr)->rangeDamper = rangeDamper;	\
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
	bool	bConstraintY;	\
	bool	bConstraintRollX;	\
	bool	bConstraintRollZ;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint1D::SetDesc((PHJoint1DDesc*)(PHSliderJointDesc*)ptr);	\
		bConstraintY = ((PHSliderJointDesc*)ptr)->bConstraintY;	\
		bConstraintRollX = ((PHSliderJointDesc*)ptr)->bConstraintRollX;	\
		bConstraintRollZ = ((PHSliderJointDesc*)ptr)->bConstraintRollZ;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint1D::GetDesc((PHJoint1DDesc*)(PHSliderJointDesc*)ptr);	\
		((PHSliderJointDesc*)ptr)->bConstraintY = bConstraintY;	\
		((PHSliderJointDesc*)ptr)->bConstraintRollX = bConstraintRollX;	\
		((PHSliderJointDesc*)ptr)->bConstraintRollZ = bConstraintRollZ;	\
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
	Quaterniond	targetPosition;	\
	Vec3d	targetVelocity;	\
	Vec3d	offsetForce;	\
	double	fMax;	\
	double	fMin;	\
	Vec2d	poleTwist;	\
	enum Spr::PHBallJointDesc::PHDeformationType	type;	\
	double	secondDamper;	\
	double	yieldStress;	\
	double	hardnessRate;	\
	Vec3d	Inertia;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		spring = ((PHBallJointDesc*)ptr)->spring;	\
		damper = ((PHBallJointDesc*)ptr)->damper;	\
		limitSwing = ((PHBallJointDesc*)ptr)->limitSwing;	\
		limitTwist = ((PHBallJointDesc*)ptr)->limitTwist;	\
		limitDir = ((PHBallJointDesc*)ptr)->limitDir;	\
		targetPosition = ((PHBallJointDesc*)ptr)->targetPosition;	\
		targetVelocity = ((PHBallJointDesc*)ptr)->targetVelocity;	\
		offsetForce = ((PHBallJointDesc*)ptr)->offsetForce;	\
		fMax = ((PHBallJointDesc*)ptr)->fMax;	\
		fMin = ((PHBallJointDesc*)ptr)->fMin;	\
		poleTwist = ((PHBallJointDesc*)ptr)->poleTwist;	\
		type = ((PHBallJointDesc*)ptr)->type;	\
		secondDamper = ((PHBallJointDesc*)ptr)->secondDamper;	\
		yieldStress = ((PHBallJointDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PHBallJointDesc*)ptr)->hardnessRate;	\
		Inertia = ((PHBallJointDesc*)ptr)->Inertia;	\
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
		((PHBallJointDesc*)ptr)->targetPosition = targetPosition;	\
		((PHBallJointDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PHBallJointDesc*)ptr)->offsetForce = offsetForce;	\
		((PHBallJointDesc*)ptr)->fMax = fMax;	\
		((PHBallJointDesc*)ptr)->fMin = fMin;	\
		((PHBallJointDesc*)ptr)->poleTwist = poleTwist;	\
		((PHBallJointDesc*)ptr)->type = type;	\
		((PHBallJointDesc*)ptr)->secondDamper = secondDamper;	\
		((PHBallJointDesc*)ptr)->yieldStress = yieldStress;	\
		((PHBallJointDesc*)ptr)->hardnessRate = hardnessRate;	\
		((PHBallJointDesc*)ptr)->Inertia = Inertia;	\
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
		PHJoint::SetDesc((PHJointDesc*)(PHSpringDesc*)ptr);	\
		spring = ((PHSpringDesc*)ptr)->spring;	\
		damper = ((PHSpringDesc*)ptr)->damper;	\
		springOri = ((PHSpringDesc*)ptr)->springOri;	\
		damperOri = ((PHSpringDesc*)ptr)->damperOri;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHSpringDesc*)ptr);	\
		((PHSpringDesc*)ptr)->spring = spring;	\
		((PHSpringDesc*)ptr)->damper = damper;	\
		((PHSpringDesc*)ptr)->springOri = springOri;	\
		((PHSpringDesc*)ptr)->damperOri = damperOri;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PH3ElementDesc \
protected:\
	Vec3d	secondDamper;	\
	double	yieldStress;	\
	double	hardnessRate;	\
	Vec3d	I;	\
	bool	yieldFlag;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHSpring::SetDesc((PHSpringDesc*)(PH3ElementDesc*)ptr);	\
		secondDamper = ((PH3ElementDesc*)ptr)->secondDamper;	\
		yieldStress = ((PH3ElementDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PH3ElementDesc*)ptr)->hardnessRate;	\
		I = ((PH3ElementDesc*)ptr)->I;	\
		yieldFlag = ((PH3ElementDesc*)ptr)->yieldFlag;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHSpring::GetDesc((PHSpringDesc*)(PH3ElementDesc*)ptr);	\
		((PH3ElementDesc*)ptr)->secondDamper = secondDamper;	\
		((PH3ElementDesc*)ptr)->yieldStress = yieldStress;	\
		((PH3ElementDesc*)ptr)->hardnessRate = hardnessRate;	\
		((PH3ElementDesc*)ptr)->I = I;	\
		((PH3ElementDesc*)ptr)->yieldFlag = yieldFlag;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHRayDesc \
protected:\
	Vec3d	origin;	\
	Vec3d	direction;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		origin = ((PHRayDesc*)ptr)->origin;	\
		direction = ((PHRayDesc*)ptr)->direction;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHRayDesc*)ptr)->origin = origin;	\
		((PHRayDesc*)ptr)->direction = direction;	\
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

#define SPR_DECLMEMBEROF_PHConstraintState \
protected:\
	SpatialVector	f;	\
	SpatialVector	F;	\
	Vec3d	motorf;	\
	Vec3d	limitf;	\
public:\
	virtual void SetState(const void* ptr){ \
		f = ((PHConstraintState*)ptr)->f;	\
		F = ((PHConstraintState*)ptr)->F;	\
		motorf = ((PHConstraintState*)ptr)->motorf;	\
		limitf = ((PHConstraintState*)ptr)->limitf;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHConstraintState*)ptr)->f = f;	\
		((PHConstraintState*)ptr)->F = F;	\
		((PHConstraintState*)ptr)->motorf = motorf;	\
		((PHConstraintState*)ptr)->limitf = limitf;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHContactPointState \
protected:\
	int	shapePair;	\
	Vec3d	pos;	\
public:\
	virtual void SetState(const void* ptr){ \
		PHConstraint::SetState((PHConstraintState*)(PHContactPointState*)ptr);	\
		shapePair = ((PHContactPointState*)ptr)->shapePair;	\
		pos = ((PHContactPointState*)ptr)->pos;	\
	}\
	virtual bool GetState(void* ptr) const { \
		PHConstraint::GetState((PHConstraintState*)(PHContactPointState*)ptr);	\
		((PHContactPointState*)ptr)->shapePair = shapePair;	\
		((PHContactPointState*)ptr)->pos = pos;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PH3ElementState \
protected:\
	SpatialVector	xs;	\
public:\
	virtual void SetState(const void* ptr){ \
		xs = ((PH3ElementState*)ptr)->xs;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PH3ElementState*)ptr)->xs = xs;	\
		return true;	\
	}\

