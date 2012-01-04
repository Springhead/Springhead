#define SPR_DECLMEMBEROF_PHFemMeshState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(void* ptr) const { \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemMeshDesc \
protected:\
	std::vector< Vec3d >	vertices;	\
	std::vector< int >	tets;	\
	std::vector< int >	faces;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHFemMesh::SetState((PHFemMeshState*)(PHFemMeshDesc*)ptr);	\
		vertices = ((PHFemMeshDesc*)ptr)->vertices;	\
		tets = ((PHFemMeshDesc*)ptr)->tets;	\
		faces = ((PHFemMeshDesc*)ptr)->faces;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemMesh::GetState((PHFemMeshState*)(PHFemMeshDesc*)ptr);	\
		((PHFemMeshDesc*)ptr)->vertices = vertices;	\
		((PHFemMeshDesc*)ptr)->tets = tets;	\
		((PHFemMeshDesc*)ptr)->faces = faces;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemMeshThermoDesc \
protected:\
	double	rho;	\
	double	thConduct;	\
	double	heatTrans;	\
	double	specificHeat;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHFemMesh::SetDesc((PHFemMeshDesc*)(PHFemMeshThermoDesc*)ptr);	\
		rho = ((PHFemMeshThermoDesc*)ptr)->rho;	\
		thConduct = ((PHFemMeshThermoDesc*)ptr)->thConduct;	\
		heatTrans = ((PHFemMeshThermoDesc*)ptr)->heatTrans;	\
		specificHeat = ((PHFemMeshThermoDesc*)ptr)->specificHeat;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemMesh::GetDesc((PHFemMeshDesc*)(PHFemMeshThermoDesc*)ptr);	\
		((PHFemMeshThermoDesc*)ptr)->rho = rho;	\
		((PHFemMeshThermoDesc*)ptr)->thConduct = thConduct;	\
		((PHFemMeshThermoDesc*)ptr)->heatTrans = heatTrans;	\
		((PHFemMeshThermoDesc*)ptr)->specificHeat = specificHeat;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKEndEffectorDesc \
protected:\
	bool	bEnabled;	\
	bool	bPosition;	\
	bool	bOrientation;	\
	bool	bForce;	\
	bool	bTorque;	\
	double	positionPriority;	\
	double	orientationPriority;	\
	double	forcePriority;	\
	double	torquePriority;	\
	Vec3d	targetPosition;	\
	Vec3d	targetLocalPosition;	\
	Quaterniond	targetOrientation;	\
	Vec3d	targetForce;	\
	Vec3d	targetForceWorkingPoint;	\
	Vec3d	targetTorque;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHIKEndEffectorDesc*)ptr)->bEnabled;	\
		bPosition = ((PHIKEndEffectorDesc*)ptr)->bPosition;	\
		bOrientation = ((PHIKEndEffectorDesc*)ptr)->bOrientation;	\
		bForce = ((PHIKEndEffectorDesc*)ptr)->bForce;	\
		bTorque = ((PHIKEndEffectorDesc*)ptr)->bTorque;	\
		positionPriority = ((PHIKEndEffectorDesc*)ptr)->positionPriority;	\
		orientationPriority = ((PHIKEndEffectorDesc*)ptr)->orientationPriority;	\
		forcePriority = ((PHIKEndEffectorDesc*)ptr)->forcePriority;	\
		torquePriority = ((PHIKEndEffectorDesc*)ptr)->torquePriority;	\
		targetPosition = ((PHIKEndEffectorDesc*)ptr)->targetPosition;	\
		targetLocalPosition = ((PHIKEndEffectorDesc*)ptr)->targetLocalPosition;	\
		targetOrientation = ((PHIKEndEffectorDesc*)ptr)->targetOrientation;	\
		targetForce = ((PHIKEndEffectorDesc*)ptr)->targetForce;	\
		targetForceWorkingPoint = ((PHIKEndEffectorDesc*)ptr)->targetForceWorkingPoint;	\
		targetTorque = ((PHIKEndEffectorDesc*)ptr)->targetTorque;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHIKEndEffectorDesc*)ptr)->bEnabled = bEnabled;	\
		((PHIKEndEffectorDesc*)ptr)->bPosition = bPosition;	\
		((PHIKEndEffectorDesc*)ptr)->bOrientation = bOrientation;	\
		((PHIKEndEffectorDesc*)ptr)->bForce = bForce;	\
		((PHIKEndEffectorDesc*)ptr)->bTorque = bTorque;	\
		((PHIKEndEffectorDesc*)ptr)->positionPriority = positionPriority;	\
		((PHIKEndEffectorDesc*)ptr)->orientationPriority = orientationPriority;	\
		((PHIKEndEffectorDesc*)ptr)->forcePriority = forcePriority;	\
		((PHIKEndEffectorDesc*)ptr)->torquePriority = torquePriority;	\
		((PHIKEndEffectorDesc*)ptr)->targetPosition = targetPosition;	\
		((PHIKEndEffectorDesc*)ptr)->targetLocalPosition = targetLocalPosition;	\
		((PHIKEndEffectorDesc*)ptr)->targetOrientation = targetOrientation;	\
		((PHIKEndEffectorDesc*)ptr)->targetForce = targetForce;	\
		((PHIKEndEffectorDesc*)ptr)->targetForceWorkingPoint = targetForceWorkingPoint;	\
		((PHIKEndEffectorDesc*)ptr)->targetTorque = targetTorque;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKActuatorDesc \
protected:\
	bool	bEnabled;	\
	float	bias;	\
	double	spring;	\
	double	damper;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHIKActuatorDesc*)ptr)->bEnabled;	\
		bias = ((PHIKActuatorDesc*)ptr)->bias;	\
		spring = ((PHIKActuatorDesc*)ptr)->spring;	\
		damper = ((PHIKActuatorDesc*)ptr)->damper;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHIKActuatorDesc*)ptr)->bEnabled = bEnabled;	\
		((PHIKActuatorDesc*)ptr)->bias = bias;	\
		((PHIKActuatorDesc*)ptr)->spring = spring;	\
		((PHIKActuatorDesc*)ptr)->damper = damper;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKBallActuatorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKActuator::SetDesc((PHIKActuatorDesc*)(PHIKBallActuatorDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKActuator::GetDesc((PHIKActuatorDesc*)(PHIKBallActuatorDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKHingeActuatorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHIKActuator::SetDesc((PHIKActuatorDesc*)(PHIKHingeActuatorDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKActuator::GetDesc((PHIKActuatorDesc*)(PHIKHingeActuatorDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHConstraintEngineDesc \
protected:\
	int	numIter;	\
	int	numIterCorrection;	\
	int	numIterContactCorrection;	\
	double	velCorrectionRate;	\
	double	posCorrectionRate;	\
	double	contactCorrectionRate;	\
	double	shrinkRate;	\
	double	shrinkRateCorrection;	\
	double	freezeThreshold;	\
	double	accelSOR;	\
	bool	bGearNodeReady;	\
	bool	bSaveConstraints;	\
	bool	bUpdateAllState;	\
	bool	bUseContactSurface;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		numIter = ((PHConstraintEngineDesc*)ptr)->numIter;	\
		numIterCorrection = ((PHConstraintEngineDesc*)ptr)->numIterCorrection;	\
		numIterContactCorrection = ((PHConstraintEngineDesc*)ptr)->numIterContactCorrection;	\
		velCorrectionRate = ((PHConstraintEngineDesc*)ptr)->velCorrectionRate;	\
		posCorrectionRate = ((PHConstraintEngineDesc*)ptr)->posCorrectionRate;	\
		contactCorrectionRate = ((PHConstraintEngineDesc*)ptr)->contactCorrectionRate;	\
		shrinkRate = ((PHConstraintEngineDesc*)ptr)->shrinkRate;	\
		shrinkRateCorrection = ((PHConstraintEngineDesc*)ptr)->shrinkRateCorrection;	\
		freezeThreshold = ((PHConstraintEngineDesc*)ptr)->freezeThreshold;	\
		accelSOR = ((PHConstraintEngineDesc*)ptr)->accelSOR;	\
		bGearNodeReady = ((PHConstraintEngineDesc*)ptr)->bGearNodeReady;	\
		bSaveConstraints = ((PHConstraintEngineDesc*)ptr)->bSaveConstraints;	\
		bUpdateAllState = ((PHConstraintEngineDesc*)ptr)->bUpdateAllState;	\
		bUseContactSurface = ((PHConstraintEngineDesc*)ptr)->bUseContactSurface;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHConstraintEngineDesc*)ptr)->numIter = numIter;	\
		((PHConstraintEngineDesc*)ptr)->numIterCorrection = numIterCorrection;	\
		((PHConstraintEngineDesc*)ptr)->numIterContactCorrection = numIterContactCorrection;	\
		((PHConstraintEngineDesc*)ptr)->velCorrectionRate = velCorrectionRate;	\
		((PHConstraintEngineDesc*)ptr)->posCorrectionRate = posCorrectionRate;	\
		((PHConstraintEngineDesc*)ptr)->contactCorrectionRate = contactCorrectionRate;	\
		((PHConstraintEngineDesc*)ptr)->shrinkRate = shrinkRate;	\
		((PHConstraintEngineDesc*)ptr)->shrinkRateCorrection = shrinkRateCorrection;	\
		((PHConstraintEngineDesc*)ptr)->freezeThreshold = freezeThreshold;	\
		((PHConstraintEngineDesc*)ptr)->accelSOR = accelSOR;	\
		((PHConstraintEngineDesc*)ptr)->bGearNodeReady = bGearNodeReady;	\
		((PHConstraintEngineDesc*)ptr)->bSaveConstraints = bSaveConstraints;	\
		((PHConstraintEngineDesc*)ptr)->bUpdateAllState = bUpdateAllState;	\
		((PHConstraintEngineDesc*)ptr)->bUseContactSurface = bUseContactSurface;	\
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
	enum Spr::PHJointDesc::PHDeformationType	type;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHConstraint::SetDesc((PHConstraintDesc*)(PHJointDesc*)ptr);	\
		type = ((PHJointDesc*)ptr)->type;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHConstraint::GetDesc((PHConstraintDesc*)(PHJointDesc*)ptr);	\
		((PHJointDesc*)ptr)->type = type;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHJoint1DDesc \
protected:\
	double	lower;	\
	double	upper;	\
	double	spring;	\
	double	targetPosition;	\
	double	damper;	\
	double	targetVelocity;	\
	double	offsetForce;	\
	double	rangeSpring;	\
	double	rangeDamper;	\
	double	fMax;	\
	double	secondDamper;	\
	double	yieldStress;	\
	double	hardnessRate;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHJoint1DDesc*)ptr);	\
		lower = ((PHJoint1DDesc*)ptr)->lower;	\
		upper = ((PHJoint1DDesc*)ptr)->upper;	\
		spring = ((PHJoint1DDesc*)ptr)->spring;	\
		targetPosition = ((PHJoint1DDesc*)ptr)->targetPosition;	\
		damper = ((PHJoint1DDesc*)ptr)->damper;	\
		targetVelocity = ((PHJoint1DDesc*)ptr)->targetVelocity;	\
		offsetForce = ((PHJoint1DDesc*)ptr)->offsetForce;	\
		rangeSpring = ((PHJoint1DDesc*)ptr)->rangeSpring;	\
		rangeDamper = ((PHJoint1DDesc*)ptr)->rangeDamper;	\
		fMax = ((PHJoint1DDesc*)ptr)->fMax;	\
		secondDamper = ((PHJoint1DDesc*)ptr)->secondDamper;	\
		yieldStress = ((PHJoint1DDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PHJoint1DDesc*)ptr)->hardnessRate;	\
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
		((PHJoint1DDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PHJoint1DDesc*)ptr)->offsetForce = offsetForce;	\
		((PHJoint1DDesc*)ptr)->rangeSpring = rangeSpring;	\
		((PHJoint1DDesc*)ptr)->rangeDamper = rangeDamper;	\
		((PHJoint1DDesc*)ptr)->fMax = fMax;	\
		((PHJoint1DDesc*)ptr)->secondDamper = secondDamper;	\
		((PHJoint1DDesc*)ptr)->yieldStress = yieldStress;	\
		((PHJoint1DDesc*)ptr)->hardnessRate = hardnessRate;	\
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
	std::vector< Spr::PHPathPoint >	points;	\
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
	Vec2d	limitSwingDir;	\
	Vec2d	limitTwist;	\
	Vec3d	limitDir;	\
	Quaterniond	targetPosition;	\
	Vec3d	targetVelocity;	\
	Vec3d	offsetForce;	\
	double	fMax;	\
	Vec2d	poleTwist;	\
	double	secondDamper;	\
	double	yieldStress;	\
	double	hardnessRate;	\
	Vec3d	Inertia;	\
	enum Spr::PHBallJointDesc::PHConstraintType	ConstMode;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		spring = ((PHBallJointDesc*)ptr)->spring;	\
		damper = ((PHBallJointDesc*)ptr)->damper;	\
		limitSwing = ((PHBallJointDesc*)ptr)->limitSwing;	\
		limitSwingDir = ((PHBallJointDesc*)ptr)->limitSwingDir;	\
		limitTwist = ((PHBallJointDesc*)ptr)->limitTwist;	\
		limitDir = ((PHBallJointDesc*)ptr)->limitDir;	\
		targetPosition = ((PHBallJointDesc*)ptr)->targetPosition;	\
		targetVelocity = ((PHBallJointDesc*)ptr)->targetVelocity;	\
		offsetForce = ((PHBallJointDesc*)ptr)->offsetForce;	\
		fMax = ((PHBallJointDesc*)ptr)->fMax;	\
		poleTwist = ((PHBallJointDesc*)ptr)->poleTwist;	\
		secondDamper = ((PHBallJointDesc*)ptr)->secondDamper;	\
		yieldStress = ((PHBallJointDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PHBallJointDesc*)ptr)->hardnessRate;	\
		Inertia = ((PHBallJointDesc*)ptr)->Inertia;	\
		ConstMode = ((PHBallJointDesc*)ptr)->ConstMode;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		((PHBallJointDesc*)ptr)->spring = spring;	\
		((PHBallJointDesc*)ptr)->damper = damper;	\
		((PHBallJointDesc*)ptr)->limitSwing = limitSwing;	\
		((PHBallJointDesc*)ptr)->limitSwingDir = limitSwingDir;	\
		((PHBallJointDesc*)ptr)->limitTwist = limitTwist;	\
		((PHBallJointDesc*)ptr)->limitDir = limitDir;	\
		((PHBallJointDesc*)ptr)->targetPosition = targetPosition;	\
		((PHBallJointDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PHBallJointDesc*)ptr)->offsetForce = offsetForce;	\
		((PHBallJointDesc*)ptr)->fMax = fMax;	\
		((PHBallJointDesc*)ptr)->poleTwist = poleTwist;	\
		((PHBallJointDesc*)ptr)->secondDamper = secondDamper;	\
		((PHBallJointDesc*)ptr)->yieldStress = yieldStress;	\
		((PHBallJointDesc*)ptr)->hardnessRate = hardnessRate;	\
		((PHBallJointDesc*)ptr)->Inertia = Inertia;	\
		((PHBallJointDesc*)ptr)->ConstMode = ConstMode;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSpringDesc \
protected:\
	Vec3d	spring;	\
	Vec3d	damper;	\
	double	springOri;	\
	double	damperOri;	\
	double	fMax;	\
	Vec3d	secondDamper;	\
	double	yieldStress;	\
	double	hardnessRate;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHSpringDesc*)ptr);	\
		spring = ((PHSpringDesc*)ptr)->spring;	\
		damper = ((PHSpringDesc*)ptr)->damper;	\
		springOri = ((PHSpringDesc*)ptr)->springOri;	\
		damperOri = ((PHSpringDesc*)ptr)->damperOri;	\
		fMax = ((PHSpringDesc*)ptr)->fMax;	\
		secondDamper = ((PHSpringDesc*)ptr)->secondDamper;	\
		yieldStress = ((PHSpringDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PHSpringDesc*)ptr)->hardnessRate;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHSpringDesc*)ptr);	\
		((PHSpringDesc*)ptr)->spring = spring;	\
		((PHSpringDesc*)ptr)->damper = damper;	\
		((PHSpringDesc*)ptr)->springOri = springOri;	\
		((PHSpringDesc*)ptr)->damperOri = damperOri;	\
		((PHSpringDesc*)ptr)->fMax = fMax;	\
		((PHSpringDesc*)ptr)->secondDamper = secondDamper;	\
		((PHSpringDesc*)ptr)->yieldStress = yieldStress;	\
		((PHSpringDesc*)ptr)->hardnessRate = hardnessRate;	\
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
	Vec3d	gravity;	\
	double	airResistanceRate;	\
	int	numIteration;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHScene::SetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		gravity = ((PHSceneDesc*)ptr)->gravity;	\
		airResistanceRate = ((PHSceneDesc*)ptr)->airResistanceRate;	\
		numIteration = ((PHSceneDesc*)ptr)->numIteration;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHScene::GetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		((PHSceneDesc*)ptr)->gravity = gravity;	\
		((PHSceneDesc*)ptr)->airResistanceRate = airResistanceRate;	\
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
	bool	integrate;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		PHSolid::SetState((PHSolidState*)(PHSolidDesc*)ptr);	\
		mass = ((PHSolidDesc*)ptr)->mass;	\
		inertia = ((PHSolidDesc*)ptr)->inertia;	\
		center = ((PHSolidDesc*)ptr)->center;	\
		dynamical = ((PHSolidDesc*)ptr)->dynamical;	\
		integrate = ((PHSolidDesc*)ptr)->integrate;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHSolid::GetState((PHSolidState*)(PHSolidDesc*)ptr);	\
		((PHSolidDesc*)ptr)->mass = mass;	\
		((PHSolidDesc*)ptr)->inertia = inertia;	\
		((PHSolidDesc*)ptr)->center = center;	\
		((PHSolidDesc*)ptr)->dynamical = dynamical;	\
		((PHSolidDesc*)ptr)->integrate = integrate;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHConstraintState \
protected:\
	SpatialVector	f;	\
	SpatialVector	F;	\
	Spr::Vec3d	motorf;	\
	Spr::Vec3d	limitf;	\
	Spr::Vec3d	fs;	\
	SpatialVector	xs;	\
public:\
	virtual void SetState(const void* ptr){ \
		f = ((PHConstraintState*)ptr)->f;	\
		F = ((PHConstraintState*)ptr)->F;	\
		motorf = ((PHConstraintState*)ptr)->motorf;	\
		limitf = ((PHConstraintState*)ptr)->limitf;	\
		fs = ((PHConstraintState*)ptr)->fs;	\
		xs = ((PHConstraintState*)ptr)->xs;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHConstraintState*)ptr)->f = f;	\
		((PHConstraintState*)ptr)->F = F;	\
		((PHConstraintState*)ptr)->motorf = motorf;	\
		((PHConstraintState*)ptr)->limitf = limitf;	\
		((PHConstraintState*)ptr)->fs = fs;	\
		((PHConstraintState*)ptr)->xs = xs;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHContactPointState \
protected:\
	int	shapePair;	\
	Spr::Vec3d	pos;	\
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

#define SPR_DECLMEMBEROF_PHHapticEngineDesc \
protected:\
	bool	bHaptic;	\
	double	hdt;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		bHaptic = ((PHHapticEngineDesc*)ptr)->bHaptic;	\
		hdt = ((PHHapticEngineDesc*)ptr)->hdt;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHHapticEngineDesc*)ptr)->bHaptic = bHaptic;	\
		((PHHapticEngineDesc*)ptr)->hdt = hdt;	\
		return true;	\
	}\

