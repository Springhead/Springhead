#define SPR_DECLMEMBEROF_PHHapticEngineDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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

#define SPR_DECLMEMBEROF_PHFemMeshNewState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(void* ptr) const { \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemMeshNewDesc \
protected:\
	std::vector< Vec3d >	vertices;	\
	std::vector< int >	tets;	\
	std::vector< int >	faces;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemMeshNew::SetState((PHFemMeshNewState*)(PHFemMeshNewDesc*)ptr);	\
		vertices = ((PHFemMeshNewDesc*)ptr)->vertices;	\
		tets = ((PHFemMeshNewDesc*)ptr)->tets;	\
		faces = ((PHFemMeshNewDesc*)ptr)->faces;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemMeshNew::GetState((PHFemMeshNewState*)(PHFemMeshNewDesc*)ptr);	\
		((PHFemMeshNewDesc*)ptr)->vertices = vertices;	\
		((PHFemMeshNewDesc*)ptr)->tets = tets;	\
		((PHFemMeshNewDesc*)ptr)->faces = faces;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemBaseDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemVibrationDesc \
protected:\
	double	young;	\
	double	poisson;	\
	double	density;	\
	double	alpha;	\
	double	beta;	\
	std::vector< int >	fixedVertices;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemBase::SetDesc((PHFemBaseDesc*)(PHFemVibrationDesc*)ptr);	\
		young = ((PHFemVibrationDesc*)ptr)->young;	\
		poisson = ((PHFemVibrationDesc*)ptr)->poisson;	\
		density = ((PHFemVibrationDesc*)ptr)->density;	\
		alpha = ((PHFemVibrationDesc*)ptr)->alpha;	\
		beta = ((PHFemVibrationDesc*)ptr)->beta;	\
		fixedVertices = ((PHFemVibrationDesc*)ptr)->fixedVertices;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemBase::GetDesc((PHFemBaseDesc*)(PHFemVibrationDesc*)ptr);	\
		((PHFemVibrationDesc*)ptr)->young = young;	\
		((PHFemVibrationDesc*)ptr)->poisson = poisson;	\
		((PHFemVibrationDesc*)ptr)->density = density;	\
		((PHFemVibrationDesc*)ptr)->alpha = alpha;	\
		((PHFemVibrationDesc*)ptr)->beta = beta;	\
		((PHFemVibrationDesc*)ptr)->fixedVertices = fixedVertices;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemThermoDesc \
protected:\
	double	rho;	\
	double	thConduct;	\
	double	heatTrans;	\
	double	specificHeat;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemBase::SetDesc((PHFemBaseDesc*)(PHFemThermoDesc*)ptr);	\
		rho = ((PHFemThermoDesc*)ptr)->rho;	\
		thConduct = ((PHFemThermoDesc*)ptr)->thConduct;	\
		heatTrans = ((PHFemThermoDesc*)ptr)->heatTrans;	\
		specificHeat = ((PHFemThermoDesc*)ptr)->specificHeat;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemBase::GetDesc((PHFemBaseDesc*)(PHFemThermoDesc*)ptr);	\
		((PHFemThermoDesc*)ptr)->rho = rho;	\
		((PHFemThermoDesc*)ptr)->thConduct = thConduct;	\
		((PHFemThermoDesc*)ptr)->heatTrans = heatTrans;	\
		((PHFemThermoDesc*)ptr)->specificHeat = specificHeat;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHHapticPointerDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	double	velocityGain;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHIKActuatorDesc*)ptr)->bEnabled;	\
		bias = ((PHIKActuatorDesc*)ptr)->bias;	\
		velocityGain = ((PHIKActuatorDesc*)ptr)->velocityGain;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHIKActuatorDesc*)ptr)->bEnabled = bEnabled;	\
		((PHIKActuatorDesc*)ptr)->bias = bias;	\
		((PHIKActuatorDesc*)ptr)->velocityGain = velocityGain;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKBallActuatorDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	double	fMax;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHConstraint::SetDesc((PHConstraintDesc*)(PHJointDesc*)ptr);	\
		fMax = ((PHJointDesc*)ptr)->fMax;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHConstraint::GetDesc((PHConstraintDesc*)(PHJointDesc*)ptr);	\
		((PHJointDesc*)ptr)->fMax = fMax;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PH1DJointDesc \
protected:\
	double	spring;	\
	double	damper;	\
	double	secondDamper;	\
	double	targetPosition;	\
	double	targetVelocity;	\
	double	offsetForce;	\
	double	yieldStress;	\
	double	hardnessRate;	\
	double	secondMoment;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PH1DJointDesc*)ptr);	\
		spring = ((PH1DJointDesc*)ptr)->spring;	\
		damper = ((PH1DJointDesc*)ptr)->damper;	\
		secondDamper = ((PH1DJointDesc*)ptr)->secondDamper;	\
		targetPosition = ((PH1DJointDesc*)ptr)->targetPosition;	\
		targetVelocity = ((PH1DJointDesc*)ptr)->targetVelocity;	\
		offsetForce = ((PH1DJointDesc*)ptr)->offsetForce;	\
		yieldStress = ((PH1DJointDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PH1DJointDesc*)ptr)->hardnessRate;	\
		secondMoment = ((PH1DJointDesc*)ptr)->secondMoment;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PH1DJointDesc*)ptr);	\
		((PH1DJointDesc*)ptr)->spring = spring;	\
		((PH1DJointDesc*)ptr)->damper = damper;	\
		((PH1DJointDesc*)ptr)->secondDamper = secondDamper;	\
		((PH1DJointDesc*)ptr)->targetPosition = targetPosition;	\
		((PH1DJointDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PH1DJointDesc*)ptr)->offsetForce = offsetForce;	\
		((PH1DJointDesc*)ptr)->yieldStress = yieldStress;	\
		((PH1DJointDesc*)ptr)->hardnessRate = hardnessRate;	\
		((PH1DJointDesc*)ptr)->secondMoment = secondMoment;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHHingeJointDesc \
protected:\
	bool	cyclic;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PH1DJoint::SetDesc((PH1DJointDesc*)(PHHingeJointDesc*)ptr);	\
		cyclic = ((PHHingeJointDesc*)ptr)->cyclic;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PH1DJoint::GetDesc((PH1DJointDesc*)(PHHingeJointDesc*)ptr);	\
		((PHHingeJointDesc*)ptr)->cyclic = cyclic;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSliderJointDesc \
protected:\
	bool	bConstraintY;	\
	bool	bConstraintRollX;	\
	bool	bConstraintRollZ;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PH1DJoint::SetDesc((PH1DJointDesc*)(PHSliderJointDesc*)ptr);	\
		bConstraintY = ((PHSliderJointDesc*)ptr)->bConstraintY;	\
		bConstraintRollX = ((PHSliderJointDesc*)ptr)->bConstraintRollX;	\
		bConstraintRollZ = ((PHSliderJointDesc*)ptr)->bConstraintRollZ;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PH1DJoint::GetDesc((PH1DJointDesc*)(PHSliderJointDesc*)ptr);	\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PH1DJoint::SetDesc((PH1DJointDesc*)(PHPathJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PH1DJoint::GetDesc((PH1DJointDesc*)(PHPathJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointDesc \
protected:\
	double	spring;	\
	double	damper;	\
	Vec3d	secondDamper;	\
	Quaterniond	targetPosition;	\
	Vec3d	targetVelocity;	\
	Vec3d	offsetForce;	\
	double	yieldStress;	\
	double	hardnessRate;	\
	Vec3d	secondMoment;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		spring = ((PHBallJointDesc*)ptr)->spring;	\
		damper = ((PHBallJointDesc*)ptr)->damper;	\
		secondDamper = ((PHBallJointDesc*)ptr)->secondDamper;	\
		targetPosition = ((PHBallJointDesc*)ptr)->targetPosition;	\
		targetVelocity = ((PHBallJointDesc*)ptr)->targetVelocity;	\
		offsetForce = ((PHBallJointDesc*)ptr)->offsetForce;	\
		yieldStress = ((PHBallJointDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PHBallJointDesc*)ptr)->hardnessRate;	\
		secondMoment = ((PHBallJointDesc*)ptr)->secondMoment;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHBallJointDesc*)ptr);	\
		((PHBallJointDesc*)ptr)->spring = spring;	\
		((PHBallJointDesc*)ptr)->damper = damper;	\
		((PHBallJointDesc*)ptr)->secondDamper = secondDamper;	\
		((PHBallJointDesc*)ptr)->targetPosition = targetPosition;	\
		((PHBallJointDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PHBallJointDesc*)ptr)->offsetForce = offsetForce;	\
		((PHBallJointDesc*)ptr)->yieldStress = yieldStress;	\
		((PHBallJointDesc*)ptr)->hardnessRate = hardnessRate;	\
		((PHBallJointDesc*)ptr)->secondMoment = secondMoment;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSpringDesc \
protected:\
	Vec3d	spring;	\
	Vec3d	damper;	\
	Vec3d	secondDamper;	\
	double	springOri;	\
	double	damperOri;	\
	double	secondDamperOri;	\
	double	yieldStress;	\
	double	hardnessRate;	\
	Vec3d	secondMoment;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHSpringDesc*)ptr);	\
		spring = ((PHSpringDesc*)ptr)->spring;	\
		damper = ((PHSpringDesc*)ptr)->damper;	\
		secondDamper = ((PHSpringDesc*)ptr)->secondDamper;	\
		springOri = ((PHSpringDesc*)ptr)->springOri;	\
		damperOri = ((PHSpringDesc*)ptr)->damperOri;	\
		secondDamperOri = ((PHSpringDesc*)ptr)->secondDamperOri;	\
		yieldStress = ((PHSpringDesc*)ptr)->yieldStress;	\
		hardnessRate = ((PHSpringDesc*)ptr)->hardnessRate;	\
		secondMoment = ((PHSpringDesc*)ptr)->secondMoment;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHSpringDesc*)ptr);	\
		((PHSpringDesc*)ptr)->spring = spring;	\
		((PHSpringDesc*)ptr)->damper = damper;	\
		((PHSpringDesc*)ptr)->secondDamper = secondDamper;	\
		((PHSpringDesc*)ptr)->springOri = springOri;	\
		((PHSpringDesc*)ptr)->damperOri = damperOri;	\
		((PHSpringDesc*)ptr)->secondDamperOri = secondDamperOri;	\
		((PHSpringDesc*)ptr)->yieldStress = yieldStress;	\
		((PHSpringDesc*)ptr)->hardnessRate = hardnessRate;	\
		((PHSpringDesc*)ptr)->secondMoment = secondMoment;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHTreeNodeDesc \
protected:\
	bool	bEnabled;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		ratio = ((PHGearDesc*)ptr)->ratio;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHGearDesc*)ptr)->ratio = ratio;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PH1DJointLimitDesc \
protected:\
	double	spring;	\
	double	damper;	\
	Vec2d	range;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		spring = ((PH1DJointLimitDesc*)ptr)->spring;	\
		damper = ((PH1DJointLimitDesc*)ptr)->damper;	\
		range = ((PH1DJointLimitDesc*)ptr)->range;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PH1DJointLimitDesc*)ptr)->spring = spring;	\
		((PH1DJointLimitDesc*)ptr)->damper = damper;	\
		((PH1DJointLimitDesc*)ptr)->range = range;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointLimitState \
protected:\
	Vec3d	f;	\
public:\
	virtual void SetState(const void* ptr){ \
		f = ((PHBallJointLimitState*)ptr)->f;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHBallJointLimitState*)ptr)->f = f;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointLimitDesc \
protected:\
	double	spring;	\
	double	damper;	\
	Vec3d	limitDir;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHBallJointLimit::SetState((PHBallJointLimitState*)(PHBallJointLimitDesc*)ptr);	\
		spring = ((PHBallJointLimitDesc*)ptr)->spring;	\
		damper = ((PHBallJointLimitDesc*)ptr)->damper;	\
		limitDir = ((PHBallJointLimitDesc*)ptr)->limitDir;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHBallJointLimit::GetState((PHBallJointLimitState*)(PHBallJointLimitDesc*)ptr);	\
		((PHBallJointLimitDesc*)ptr)->spring = spring;	\
		((PHBallJointLimitDesc*)ptr)->damper = damper;	\
		((PHBallJointLimitDesc*)ptr)->limitDir = limitDir;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointConeLimitDesc \
protected:\
	Vec2d	limitSwing;	\
	Vec2d	limitSwingDir;	\
	Vec2d	limitTwist;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHBallJointLimit::SetDesc((PHBallJointLimitDesc*)(PHBallJointConeLimitDesc*)ptr);	\
		limitSwing = ((PHBallJointConeLimitDesc*)ptr)->limitSwing;	\
		limitSwingDir = ((PHBallJointConeLimitDesc*)ptr)->limitSwingDir;	\
		limitTwist = ((PHBallJointConeLimitDesc*)ptr)->limitTwist;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHBallJointLimit::GetDesc((PHBallJointLimitDesc*)(PHBallJointConeLimitDesc*)ptr);	\
		((PHBallJointConeLimitDesc*)ptr)->limitSwing = limitSwing;	\
		((PHBallJointConeLimitDesc*)ptr)->limitSwingDir = limitSwingDir;	\
		((PHBallJointConeLimitDesc*)ptr)->limitTwist = limitTwist;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointSplineLimitDesc \
protected:\
	std::vector< Spr::PHSplineLimitNode >	nodes;	\
	Vec2d	poleTwist;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHBallJointLimit::SetDesc((PHBallJointLimitDesc*)(PHBallJointSplineLimitDesc*)ptr);	\
		nodes = ((PHBallJointSplineLimitDesc*)ptr)->nodes;	\
		poleTwist = ((PHBallJointSplineLimitDesc*)ptr)->poleTwist;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHBallJointLimit::GetDesc((PHBallJointLimitDesc*)(PHBallJointSplineLimitDesc*)ptr);	\
		((PHBallJointSplineLimitDesc*)ptr)->nodes = nodes;	\
		((PHBallJointSplineLimitDesc*)ptr)->poleTwist = poleTwist;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHRayDesc \
protected:\
	Vec3d	origin;	\
	Vec3d	direction;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
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
	double	haptictimeStep;	\
	unsigned int	count;	\
public:\
	virtual void SetState(const void* ptr){ \
		timeStep = ((PHSceneState*)ptr)->timeStep;	\
		haptictimeStep = ((PHSceneState*)ptr)->haptictimeStep;	\
		count = ((PHSceneState*)ptr)->count;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHSceneState*)ptr)->timeStep = timeStep;	\
		((PHSceneState*)ptr)->haptictimeStep = haptictimeStep;	\
		((PHSceneState*)ptr)->count = count;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSceneDesc \
protected:\
	Vec3d	gravity;	\
	double	airResistanceRate;	\
	double	contactTolerance;	\
	double	impactThreshold;	\
	double	frictionThreshold;	\
	double	maxVelocity;	\
	double	maxAngularVelocity;	\
	int	numIteration;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHScene::SetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		gravity = ((PHSceneDesc*)ptr)->gravity;	\
		airResistanceRate = ((PHSceneDesc*)ptr)->airResistanceRate;	\
		contactTolerance = ((PHSceneDesc*)ptr)->contactTolerance;	\
		impactThreshold = ((PHSceneDesc*)ptr)->impactThreshold;	\
		frictionThreshold = ((PHSceneDesc*)ptr)->frictionThreshold;	\
		maxVelocity = ((PHSceneDesc*)ptr)->maxVelocity;	\
		maxAngularVelocity = ((PHSceneDesc*)ptr)->maxAngularVelocity;	\
		numIteration = ((PHSceneDesc*)ptr)->numIteration;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHScene::GetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		((PHSceneDesc*)ptr)->gravity = gravity;	\
		((PHSceneDesc*)ptr)->airResistanceRate = airResistanceRate;	\
		((PHSceneDesc*)ptr)->contactTolerance = contactTolerance;	\
		((PHSceneDesc*)ptr)->impactThreshold = impactThreshold;	\
		((PHSceneDesc*)ptr)->frictionThreshold = frictionThreshold;	\
		((PHSceneDesc*)ptr)->maxVelocity = maxVelocity;	\
		((PHSceneDesc*)ptr)->maxAngularVelocity = maxAngularVelocity;	\
		((PHSceneDesc*)ptr)->numIteration = numIteration;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSdkDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
public:\
	virtual void SetState(const void* ptr){ \
		velocity = ((PHSolidState*)ptr)->velocity;	\
		angVelocity = ((PHSolidState*)ptr)->angVelocity;	\
		pose = ((PHSolidState*)ptr)->pose;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHSolidState*)ptr)->velocity = velocity;	\
		((PHSolidState*)ptr)->angVelocity = angVelocity;	\
		((PHSolidState*)ptr)->pose = pose;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHSolidDesc \
protected:\
	double	mass;	\
	Matrix3d	inertia;	\
	Vec3d	center;	\
	bool	dynamical;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
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
	SpatialVector	xs;	\
	SpatialVector	fAvg;	\
	bool	bYielded;	\
public:\
	virtual void SetState(const void* ptr){ \
		f = ((PHConstraintState*)ptr)->f;	\
		F = ((PHConstraintState*)ptr)->F;	\
		xs = ((PHConstraintState*)ptr)->xs;	\
		fAvg = ((PHConstraintState*)ptr)->fAvg;	\
		bYielded = ((PHConstraintState*)ptr)->bYielded;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHConstraintState*)ptr)->f = f;	\
		((PHConstraintState*)ptr)->F = F;	\
		((PHConstraintState*)ptr)->xs = xs;	\
		((PHConstraintState*)ptr)->fAvg = fAvg;	\
		((PHConstraintState*)ptr)->bYielded = bYielded;	\
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

