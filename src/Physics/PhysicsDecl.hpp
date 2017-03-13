#define SPR_DECLMEMBEROF_PHConstraintEngineDesc \
protected:\
	int	method;	\
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
	double	dfEps;	\
	bool	bSaveConstraints;	\
	bool	bUpdateAllState;	\
	bool	bUseContactSurface;	\
	bool	bReport;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		method = ((PHConstraintEngineDesc*)ptr)->method;	\
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
		dfEps = ((PHConstraintEngineDesc*)ptr)->dfEps;	\
		bSaveConstraints = ((PHConstraintEngineDesc*)ptr)->bSaveConstraints;	\
		bUpdateAllState = ((PHConstraintEngineDesc*)ptr)->bUpdateAllState;	\
		bUseContactSurface = ((PHConstraintEngineDesc*)ptr)->bUseContactSurface;	\
		bReport = ((PHConstraintEngineDesc*)ptr)->bReport;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHConstraintEngineDesc*)ptr)->method = method;	\
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
		((PHConstraintEngineDesc*)ptr)->dfEps = dfEps;	\
		((PHConstraintEngineDesc*)ptr)->bSaveConstraints = bSaveConstraints;	\
		((PHConstraintEngineDesc*)ptr)->bUpdateAllState = bUpdateAllState;	\
		((PHConstraintEngineDesc*)ptr)->bUseContactSurface = bUseContactSurface;	\
		((PHConstraintEngineDesc*)ptr)->bReport = bReport;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKEngineDesc \
protected:\
	size_t	numIter;	\
	double	maxVel;	\
	double	maxAngVel;	\
	double	maxActVel;	\
	double	regularizeParam;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		numIter = ((PHIKEngineDesc*)ptr)->numIter;	\
		maxVel = ((PHIKEngineDesc*)ptr)->maxVel;	\
		maxAngVel = ((PHIKEngineDesc*)ptr)->maxAngVel;	\
		maxActVel = ((PHIKEngineDesc*)ptr)->maxActVel;	\
		regularizeParam = ((PHIKEngineDesc*)ptr)->regularizeParam;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHIKEngineDesc*)ptr)->numIter = numIter;	\
		((PHIKEngineDesc*)ptr)->maxVel = maxVel;	\
		((PHIKEngineDesc*)ptr)->maxAngVel = maxAngVel;	\
		((PHIKEngineDesc*)ptr)->maxActVel = maxActVel;	\
		((PHIKEngineDesc*)ptr)->regularizeParam = regularizeParam;	\
		return true;	\
	}\

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
	double	thConduct_x;	\
	double	thConduct_y;	\
	double	thConduct_z;	\
	double	heatTrans;	\
	double	specificHeat;	\
	double	radiantHeat;	\
	double	initial_temp;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemMesh::SetDesc((PHFemMeshDesc*)(PHFemMeshThermoDesc*)ptr);	\
		rho = ((PHFemMeshThermoDesc*)ptr)->rho;	\
		thConduct = ((PHFemMeshThermoDesc*)ptr)->thConduct;	\
		thConduct_x = ((PHFemMeshThermoDesc*)ptr)->thConduct_x;	\
		thConduct_y = ((PHFemMeshThermoDesc*)ptr)->thConduct_y;	\
		thConduct_z = ((PHFemMeshThermoDesc*)ptr)->thConduct_z;	\
		heatTrans = ((PHFemMeshThermoDesc*)ptr)->heatTrans;	\
		specificHeat = ((PHFemMeshThermoDesc*)ptr)->specificHeat;	\
		radiantHeat = ((PHFemMeshThermoDesc*)ptr)->radiantHeat;	\
		initial_temp = ((PHFemMeshThermoDesc*)ptr)->initial_temp;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemMesh::GetDesc((PHFemMeshDesc*)(PHFemMeshThermoDesc*)ptr);	\
		((PHFemMeshThermoDesc*)ptr)->rho = rho;	\
		((PHFemMeshThermoDesc*)ptr)->thConduct = thConduct;	\
		((PHFemMeshThermoDesc*)ptr)->thConduct_x = thConduct_x;	\
		((PHFemMeshThermoDesc*)ptr)->thConduct_y = thConduct_y;	\
		((PHFemMeshThermoDesc*)ptr)->thConduct_z = thConduct_z;	\
		((PHFemMeshThermoDesc*)ptr)->heatTrans = heatTrans;	\
		((PHFemMeshThermoDesc*)ptr)->specificHeat = specificHeat;	\
		((PHFemMeshThermoDesc*)ptr)->radiantHeat = radiantHeat;	\
		((PHFemMeshThermoDesc*)ptr)->initial_temp = initial_temp;	\
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
	bool	spheric;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemMeshNew::SetState((PHFemMeshNewState*)(PHFemMeshNewDesc*)ptr);	\
		vertices = ((PHFemMeshNewDesc*)ptr)->vertices;	\
		tets = ((PHFemMeshNewDesc*)ptr)->tets;	\
		faces = ((PHFemMeshNewDesc*)ptr)->faces;	\
		spheric = ((PHFemMeshNewDesc*)ptr)->spheric;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemMeshNew::GetState((PHFemMeshNewState*)(PHFemMeshNewDesc*)ptr);	\
		((PHFemMeshNewDesc*)ptr)->vertices = vertices;	\
		((PHFemMeshNewDesc*)ptr)->tets = tets;	\
		((PHFemMeshNewDesc*)ptr)->faces = faces;	\
		((PHFemMeshNewDesc*)ptr)->spheric = spheric;	\
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
	double	thConduct_x;	\
	double	thConduct_y;	\
	double	thConduct_z;	\
	double	heatTrans;	\
	double	specificHeat;	\
	double	radiantHeat;	\
	double	initial_temp;	\
	double	weekPow_full;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemBase::SetDesc((PHFemBaseDesc*)(PHFemThermoDesc*)ptr);	\
		rho = ((PHFemThermoDesc*)ptr)->rho;	\
		thConduct = ((PHFemThermoDesc*)ptr)->thConduct;	\
		thConduct_x = ((PHFemThermoDesc*)ptr)->thConduct_x;	\
		thConduct_y = ((PHFemThermoDesc*)ptr)->thConduct_y;	\
		thConduct_z = ((PHFemThermoDesc*)ptr)->thConduct_z;	\
		heatTrans = ((PHFemThermoDesc*)ptr)->heatTrans;	\
		specificHeat = ((PHFemThermoDesc*)ptr)->specificHeat;	\
		radiantHeat = ((PHFemThermoDesc*)ptr)->radiantHeat;	\
		initial_temp = ((PHFemThermoDesc*)ptr)->initial_temp;	\
		weekPow_full = ((PHFemThermoDesc*)ptr)->weekPow_full;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemBase::GetDesc((PHFemBaseDesc*)(PHFemThermoDesc*)ptr);	\
		((PHFemThermoDesc*)ptr)->rho = rho;	\
		((PHFemThermoDesc*)ptr)->thConduct = thConduct;	\
		((PHFemThermoDesc*)ptr)->thConduct_x = thConduct_x;	\
		((PHFemThermoDesc*)ptr)->thConduct_y = thConduct_y;	\
		((PHFemThermoDesc*)ptr)->thConduct_z = thConduct_z;	\
		((PHFemThermoDesc*)ptr)->heatTrans = heatTrans;	\
		((PHFemThermoDesc*)ptr)->specificHeat = specificHeat;	\
		((PHFemThermoDesc*)ptr)->radiantHeat = radiantHeat;	\
		((PHFemThermoDesc*)ptr)->initial_temp = initial_temp;	\
		((PHFemThermoDesc*)ptr)->weekPow_full = weekPow_full;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHFemPorousWOMoveDesc \
protected:\
	double	wDiffAir;	\
	double	K;	\
	double	kc;	\
	double	kp;	\
	double	alpha;	\
	double	gamma;	\
	double	rhoWater;	\
	double	rhoOil;	\
	double	rhowInit;	\
	double	rhooInit;	\
	double	evapoRate;	\
	double	denatTemp;	\
	double	boundWaterRatio;	\
	double	equilWaterCont;	\
	double	limitWaterCont;	\
	double	boundaryThick;	\
	double	initMassAll;	\
	double	initWaterRatio;	\
	double	initOilRatio;	\
	double	shrinkageRatio;	\
	Vec3d	top;	\
	Vec3d	center;	\
	Vec3d	bottom;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHFemBase::SetDesc((PHFemBaseDesc*)(PHFemPorousWOMoveDesc*)ptr);	\
		wDiffAir = ((PHFemPorousWOMoveDesc*)ptr)->wDiffAir;	\
		K = ((PHFemPorousWOMoveDesc*)ptr)->K;	\
		kc = ((PHFemPorousWOMoveDesc*)ptr)->kc;	\
		kp = ((PHFemPorousWOMoveDesc*)ptr)->kp;	\
		alpha = ((PHFemPorousWOMoveDesc*)ptr)->alpha;	\
		gamma = ((PHFemPorousWOMoveDesc*)ptr)->gamma;	\
		rhoWater = ((PHFemPorousWOMoveDesc*)ptr)->rhoWater;	\
		rhoOil = ((PHFemPorousWOMoveDesc*)ptr)->rhoOil;	\
		rhowInit = ((PHFemPorousWOMoveDesc*)ptr)->rhowInit;	\
		rhooInit = ((PHFemPorousWOMoveDesc*)ptr)->rhooInit;	\
		evapoRate = ((PHFemPorousWOMoveDesc*)ptr)->evapoRate;	\
		denatTemp = ((PHFemPorousWOMoveDesc*)ptr)->denatTemp;	\
		boundWaterRatio = ((PHFemPorousWOMoveDesc*)ptr)->boundWaterRatio;	\
		equilWaterCont = ((PHFemPorousWOMoveDesc*)ptr)->equilWaterCont;	\
		limitWaterCont = ((PHFemPorousWOMoveDesc*)ptr)->limitWaterCont;	\
		boundaryThick = ((PHFemPorousWOMoveDesc*)ptr)->boundaryThick;	\
		initMassAll = ((PHFemPorousWOMoveDesc*)ptr)->initMassAll;	\
		initWaterRatio = ((PHFemPorousWOMoveDesc*)ptr)->initWaterRatio;	\
		initOilRatio = ((PHFemPorousWOMoveDesc*)ptr)->initOilRatio;	\
		shrinkageRatio = ((PHFemPorousWOMoveDesc*)ptr)->shrinkageRatio;	\
		top = ((PHFemPorousWOMoveDesc*)ptr)->top;	\
		center = ((PHFemPorousWOMoveDesc*)ptr)->center;	\
		bottom = ((PHFemPorousWOMoveDesc*)ptr)->bottom;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHFemBase::GetDesc((PHFemBaseDesc*)(PHFemPorousWOMoveDesc*)ptr);	\
		((PHFemPorousWOMoveDesc*)ptr)->wDiffAir = wDiffAir;	\
		((PHFemPorousWOMoveDesc*)ptr)->K = K;	\
		((PHFemPorousWOMoveDesc*)ptr)->kc = kc;	\
		((PHFemPorousWOMoveDesc*)ptr)->kp = kp;	\
		((PHFemPorousWOMoveDesc*)ptr)->alpha = alpha;	\
		((PHFemPorousWOMoveDesc*)ptr)->gamma = gamma;	\
		((PHFemPorousWOMoveDesc*)ptr)->rhoWater = rhoWater;	\
		((PHFemPorousWOMoveDesc*)ptr)->rhoOil = rhoOil;	\
		((PHFemPorousWOMoveDesc*)ptr)->rhowInit = rhowInit;	\
		((PHFemPorousWOMoveDesc*)ptr)->rhooInit = rhooInit;	\
		((PHFemPorousWOMoveDesc*)ptr)->evapoRate = evapoRate;	\
		((PHFemPorousWOMoveDesc*)ptr)->denatTemp = denatTemp;	\
		((PHFemPorousWOMoveDesc*)ptr)->boundWaterRatio = boundWaterRatio;	\
		((PHFemPorousWOMoveDesc*)ptr)->equilWaterCont = equilWaterCont;	\
		((PHFemPorousWOMoveDesc*)ptr)->limitWaterCont = limitWaterCont;	\
		((PHFemPorousWOMoveDesc*)ptr)->boundaryThick = boundaryThick;	\
		((PHFemPorousWOMoveDesc*)ptr)->initMassAll = initMassAll;	\
		((PHFemPorousWOMoveDesc*)ptr)->initWaterRatio = initWaterRatio;	\
		((PHFemPorousWOMoveDesc*)ptr)->initOilRatio = initOilRatio;	\
		((PHFemPorousWOMoveDesc*)ptr)->shrinkageRatio = shrinkageRatio;	\
		((PHFemPorousWOMoveDesc*)ptr)->top = top;	\
		((PHFemPorousWOMoveDesc*)ptr)->center = center;	\
		((PHFemPorousWOMoveDesc*)ptr)->bottom = bottom;	\
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

#define SPR_DECLMEMBEROF_PHIKEndEffectorState \
protected:\
	Posed	solidTempPose;	\
public:\
	virtual void SetState(const void* ptr){ \
		solidTempPose = ((PHIKEndEffectorState*)ptr)->solidTempPose;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHIKEndEffectorState*)ptr)->solidTempPose = solidTempPose;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKEndEffectorDesc \
protected:\
	bool	bEnabled;	\
	bool	bPosition;	\
	bool	bOrientation;	\
	int	oriCtlMode;	\
	bool	bForce;	\
	bool	bTorque;	\
	double	positionPriority;	\
	double	orientationPriority;	\
	double	forcePriority;	\
	double	torquePriority;	\
	Vec3d	targetPosition;	\
	Vec3d	targetVelocity;	\
	Vec3d	targetLocalPosition;	\
	Vec3d	targetLocalDirection;	\
	Quaterniond	targetOrientation;	\
	Vec3d	targetDirection;	\
	Vec3d	targetLookat;	\
	Vec3d	targetAngVel;	\
	Vec3d	targetForce;	\
	Vec3d	targetForceWorkingPoint;	\
	Vec3d	targetTorque;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHIKEndEffector::SetState((PHIKEndEffectorState*)(PHIKEndEffectorDesc*)ptr);	\
		bEnabled = ((PHIKEndEffectorDesc*)ptr)->bEnabled;	\
		bPosition = ((PHIKEndEffectorDesc*)ptr)->bPosition;	\
		bOrientation = ((PHIKEndEffectorDesc*)ptr)->bOrientation;	\
		oriCtlMode = ((PHIKEndEffectorDesc*)ptr)->oriCtlMode;	\
		bForce = ((PHIKEndEffectorDesc*)ptr)->bForce;	\
		bTorque = ((PHIKEndEffectorDesc*)ptr)->bTorque;	\
		positionPriority = ((PHIKEndEffectorDesc*)ptr)->positionPriority;	\
		orientationPriority = ((PHIKEndEffectorDesc*)ptr)->orientationPriority;	\
		forcePriority = ((PHIKEndEffectorDesc*)ptr)->forcePriority;	\
		torquePriority = ((PHIKEndEffectorDesc*)ptr)->torquePriority;	\
		targetPosition = ((PHIKEndEffectorDesc*)ptr)->targetPosition;	\
		targetVelocity = ((PHIKEndEffectorDesc*)ptr)->targetVelocity;	\
		targetLocalPosition = ((PHIKEndEffectorDesc*)ptr)->targetLocalPosition;	\
		targetLocalDirection = ((PHIKEndEffectorDesc*)ptr)->targetLocalDirection;	\
		targetOrientation = ((PHIKEndEffectorDesc*)ptr)->targetOrientation;	\
		targetDirection = ((PHIKEndEffectorDesc*)ptr)->targetDirection;	\
		targetLookat = ((PHIKEndEffectorDesc*)ptr)->targetLookat;	\
		targetAngVel = ((PHIKEndEffectorDesc*)ptr)->targetAngVel;	\
		targetForce = ((PHIKEndEffectorDesc*)ptr)->targetForce;	\
		targetForceWorkingPoint = ((PHIKEndEffectorDesc*)ptr)->targetForceWorkingPoint;	\
		targetTorque = ((PHIKEndEffectorDesc*)ptr)->targetTorque;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKEndEffector::GetState((PHIKEndEffectorState*)(PHIKEndEffectorDesc*)ptr);	\
		((PHIKEndEffectorDesc*)ptr)->bEnabled = bEnabled;	\
		((PHIKEndEffectorDesc*)ptr)->bPosition = bPosition;	\
		((PHIKEndEffectorDesc*)ptr)->bOrientation = bOrientation;	\
		((PHIKEndEffectorDesc*)ptr)->oriCtlMode = oriCtlMode;	\
		((PHIKEndEffectorDesc*)ptr)->bForce = bForce;	\
		((PHIKEndEffectorDesc*)ptr)->bTorque = bTorque;	\
		((PHIKEndEffectorDesc*)ptr)->positionPriority = positionPriority;	\
		((PHIKEndEffectorDesc*)ptr)->orientationPriority = orientationPriority;	\
		((PHIKEndEffectorDesc*)ptr)->forcePriority = forcePriority;	\
		((PHIKEndEffectorDesc*)ptr)->torquePriority = torquePriority;	\
		((PHIKEndEffectorDesc*)ptr)->targetPosition = targetPosition;	\
		((PHIKEndEffectorDesc*)ptr)->targetVelocity = targetVelocity;	\
		((PHIKEndEffectorDesc*)ptr)->targetLocalPosition = targetLocalPosition;	\
		((PHIKEndEffectorDesc*)ptr)->targetLocalDirection = targetLocalDirection;	\
		((PHIKEndEffectorDesc*)ptr)->targetOrientation = targetOrientation;	\
		((PHIKEndEffectorDesc*)ptr)->targetDirection = targetDirection;	\
		((PHIKEndEffectorDesc*)ptr)->targetLookat = targetLookat;	\
		((PHIKEndEffectorDesc*)ptr)->targetAngVel = targetAngVel;	\
		((PHIKEndEffectorDesc*)ptr)->targetForce = targetForce;	\
		((PHIKEndEffectorDesc*)ptr)->targetForceWorkingPoint = targetForceWorkingPoint;	\
		((PHIKEndEffectorDesc*)ptr)->targetTorque = targetTorque;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKActuatorState \
protected:\
	Posed	solidTempPose;	\
	Quaterniond	jointTempOri;	\
	double	jointTempAngle;	\
public:\
	virtual void SetState(const void* ptr){ \
		solidTempPose = ((PHIKActuatorState*)ptr)->solidTempPose;	\
		jointTempOri = ((PHIKActuatorState*)ptr)->jointTempOri;	\
		jointTempAngle = ((PHIKActuatorState*)ptr)->jointTempAngle;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHIKActuatorState*)ptr)->solidTempPose = solidTempPose;	\
		((PHIKActuatorState*)ptr)->jointTempOri = jointTempOri;	\
		((PHIKActuatorState*)ptr)->jointTempAngle = jointTempAngle;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKActuatorDesc \
protected:\
	bool	bEnabled;	\
	float	bias;	\
	double	pullbackRate;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHIKActuator::SetState((PHIKActuatorState*)(PHIKActuatorDesc*)ptr);	\
		bEnabled = ((PHIKActuatorDesc*)ptr)->bEnabled;	\
		bias = ((PHIKActuatorDesc*)ptr)->bias;	\
		pullbackRate = ((PHIKActuatorDesc*)ptr)->pullbackRate;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKActuator::GetState((PHIKActuatorState*)(PHIKActuatorDesc*)ptr);	\
		((PHIKActuatorDesc*)ptr)->bEnabled = bEnabled;	\
		((PHIKActuatorDesc*)ptr)->bias = bias;	\
		((PHIKActuatorDesc*)ptr)->pullbackRate = pullbackRate;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKBallActuatorDesc \
protected:\
	Quaterniond	pullbackTarget;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHIKActuator::SetDesc((PHIKActuatorDesc*)(PHIKBallActuatorDesc*)ptr);	\
		pullbackTarget = ((PHIKBallActuatorDesc*)ptr)->pullbackTarget;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKActuator::GetDesc((PHIKActuatorDesc*)(PHIKBallActuatorDesc*)ptr);	\
		((PHIKBallActuatorDesc*)ptr)->pullbackTarget = pullbackTarget;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHIKHingeActuatorDesc \
protected:\
	double	pullbackTarget;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHIKActuator::SetDesc((PHIKActuatorDesc*)(PHIKHingeActuatorDesc*)ptr);	\
		pullbackTarget = ((PHIKHingeActuatorDesc*)ptr)->pullbackTarget;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHIKActuator::GetDesc((PHIKActuatorDesc*)(PHIKHingeActuatorDesc*)ptr);	\
		((PHIKHingeActuatorDesc*)ptr)->pullbackTarget = pullbackTarget;	\
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
	bool	cyclic;	\
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
		cyclic = ((PH1DJointDesc*)ptr)->cyclic;	\
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
		((PH1DJointDesc*)ptr)->cyclic = cyclic;	\
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
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PH1DJoint::SetDesc((PH1DJointDesc*)(PHHingeJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PH1DJoint::GetDesc((PH1DJointDesc*)(PHHingeJointDesc*)ptr);	\
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

#define SPR_DECLMEMBEROF_PHGenericJointDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PH1DJoint::SetDesc((PH1DJointDesc*)(PHGenericJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PH1DJoint::GetDesc((PH1DJointDesc*)(PHGenericJointDesc*)ptr);	\
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

#define SPR_DECLMEMBEROF_PHFixJointDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHFixJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHFixJointDesc*)ptr);	\
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

#define SPR_DECLMEMBEROF_PHMateDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHJoint::SetDesc((PHJointDesc*)(PHMateDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHJoint::GetDesc((PHJointDesc*)(PHMateDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPointToPointMateDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHMate::SetDesc((PHMateDesc*)(PHPointToPointMateDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHMate::GetDesc((PHMateDesc*)(PHPointToPointMateDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPointToLineMateDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHMate::SetDesc((PHMateDesc*)(PHPointToLineMateDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHMate::GetDesc((PHMateDesc*)(PHPointToLineMateDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPointToPlaneMateDesc \
protected:\
	Vec2d	range;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHMate::SetDesc((PHMateDesc*)(PHPointToPlaneMateDesc*)ptr);	\
		range = ((PHPointToPlaneMateDesc*)ptr)->range;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHMate::GetDesc((PHMateDesc*)(PHPointToPlaneMateDesc*)ptr);	\
		((PHPointToPlaneMateDesc*)ptr)->range = range;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHLineToLineMateDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHMate::SetDesc((PHMateDesc*)(PHLineToLineMateDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHMate::GetDesc((PHMateDesc*)(PHLineToLineMateDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHPlaneToPlaneMateDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHMate::SetDesc((PHMateDesc*)(PHPlaneToPlaneMateDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHMate::GetDesc((PHMateDesc*)(PHPlaneToPlaneMateDesc*)ptr);	\
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

#define SPR_DECLMEMBEROF_PHGenericJointNodeDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode1D::SetDesc((PHTreeNode1DDesc*)(PHGenericJointNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode1D::GetDesc((PHTreeNode1DDesc*)(PHGenericJointNodeDesc*)ptr);	\
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

#define SPR_DECLMEMBEROF_PHFixJointNodeDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHTreeNode::SetDesc((PHTreeNodeDesc*)(PHFixJointNodeDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHTreeNode::GetDesc((PHTreeNodeDesc*)(PHFixJointNodeDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHGearDesc \
protected:\
	bool	bEnabled;	\
	double	ratio;	\
	double	offset;	\
	int	mode;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHGearDesc*)ptr)->bEnabled;	\
		ratio = ((PHGearDesc*)ptr)->ratio;	\
		offset = ((PHGearDesc*)ptr)->offset;	\
		mode = ((PHGearDesc*)ptr)->mode;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHGearDesc*)ptr)->bEnabled = bEnabled;	\
		((PHGearDesc*)ptr)->ratio = ratio;	\
		((PHGearDesc*)ptr)->offset = offset;	\
		((PHGearDesc*)ptr)->mode = mode;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PH1DJointLimitDesc \
protected:\
	bool	bEnabled;	\
	double	spring;	\
	double	damper;	\
	Vec2d	range;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PH1DJointLimitDesc*)ptr)->bEnabled;	\
		spring = ((PH1DJointLimitDesc*)ptr)->spring;	\
		damper = ((PH1DJointLimitDesc*)ptr)->damper;	\
		range = ((PH1DJointLimitDesc*)ptr)->range;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PH1DJointLimitDesc*)ptr)->bEnabled = bEnabled;	\
		((PH1DJointLimitDesc*)ptr)->spring = spring;	\
		((PH1DJointLimitDesc*)ptr)->damper = damper;	\
		((PH1DJointLimitDesc*)ptr)->range = range;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHBallJointLimitDesc \
protected:\
	bool	bEnabled;	\
	double	spring;	\
	double	damper;	\
	Vec3d	limitDir;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		bEnabled = ((PHBallJointLimitDesc*)ptr)->bEnabled;	\
		spring = ((PHBallJointLimitDesc*)ptr)->spring;	\
		damper = ((PHBallJointLimitDesc*)ptr)->damper;	\
		limitDir = ((PHBallJointLimitDesc*)ptr)->limitDir;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHBallJointLimitDesc*)ptr)->bEnabled = bEnabled;	\
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

#define SPR_DECLMEMBEROF_PH1DJointMotorDesc \
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

#define SPR_DECLMEMBEROF_PHBallJointMotorDesc \
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

#define SPR_DECLMEMBEROF_PHSpringMotorDesc \
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

#define SPR_DECLMEMBEROF_PHOpObjState \
protected:\
	Vec3d	velocity;	\
	Vec3d	angVelocity;	\
	Posed	pose;	\
public:\
	virtual void SetState(const void* ptr){ \
		velocity = ((PHOpObjState*)ptr)->velocity;	\
		angVelocity = ((PHOpObjState*)ptr)->angVelocity;	\
		pose = ((PHOpObjState*)ptr)->pose;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((PHOpObjState*)ptr)->velocity = velocity;	\
		((PHOpObjState*)ptr)->angVelocity = angVelocity;	\
		((PHOpObjState*)ptr)->pose = pose;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpHapticRendererDesc \
protected:\
	float	extForceSpring;	\
	float	outForceSpring;	\
	float	constraintSpring;	\
	float	timeStep;	\
	float	forceOnRadius;	\
	bool	useConstrainForce;	\
	bool	useProxyfixSubitr;	\
	bool	useIncreaseMeshMove;	\
	bool	sqrtAlphaForce;	\
	bool	hitWall;	\
	bool	useDualTranFix;	\
	bool	rigid;	\
	int	proxyItrtNum;	\
	int	proxyItrtMaxNum;	\
	int	currDtType;	\
	int	objNum;	\
	int	noCtcItrNum;	\
	Vec3f	proxyFixPos;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		extForceSpring = ((PHOpHapticRendererDesc*)ptr)->extForceSpring;	\
		outForceSpring = ((PHOpHapticRendererDesc*)ptr)->outForceSpring;	\
		constraintSpring = ((PHOpHapticRendererDesc*)ptr)->constraintSpring;	\
		timeStep = ((PHOpHapticRendererDesc*)ptr)->timeStep;	\
		forceOnRadius = ((PHOpHapticRendererDesc*)ptr)->forceOnRadius;	\
		useConstrainForce = ((PHOpHapticRendererDesc*)ptr)->useConstrainForce;	\
		useProxyfixSubitr = ((PHOpHapticRendererDesc*)ptr)->useProxyfixSubitr;	\
		useIncreaseMeshMove = ((PHOpHapticRendererDesc*)ptr)->useIncreaseMeshMove;	\
		sqrtAlphaForce = ((PHOpHapticRendererDesc*)ptr)->sqrtAlphaForce;	\
		hitWall = ((PHOpHapticRendererDesc*)ptr)->hitWall;	\
		useDualTranFix = ((PHOpHapticRendererDesc*)ptr)->useDualTranFix;	\
		rigid = ((PHOpHapticRendererDesc*)ptr)->rigid;	\
		proxyItrtNum = ((PHOpHapticRendererDesc*)ptr)->proxyItrtNum;	\
		proxyItrtMaxNum = ((PHOpHapticRendererDesc*)ptr)->proxyItrtMaxNum;	\
		currDtType = ((PHOpHapticRendererDesc*)ptr)->currDtType;	\
		objNum = ((PHOpHapticRendererDesc*)ptr)->objNum;	\
		noCtcItrNum = ((PHOpHapticRendererDesc*)ptr)->noCtcItrNum;	\
		proxyFixPos = ((PHOpHapticRendererDesc*)ptr)->proxyFixPos;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHOpHapticRendererDesc*)ptr)->extForceSpring = extForceSpring;	\
		((PHOpHapticRendererDesc*)ptr)->outForceSpring = outForceSpring;	\
		((PHOpHapticRendererDesc*)ptr)->constraintSpring = constraintSpring;	\
		((PHOpHapticRendererDesc*)ptr)->timeStep = timeStep;	\
		((PHOpHapticRendererDesc*)ptr)->forceOnRadius = forceOnRadius;	\
		((PHOpHapticRendererDesc*)ptr)->useConstrainForce = useConstrainForce;	\
		((PHOpHapticRendererDesc*)ptr)->useProxyfixSubitr = useProxyfixSubitr;	\
		((PHOpHapticRendererDesc*)ptr)->useIncreaseMeshMove = useIncreaseMeshMove;	\
		((PHOpHapticRendererDesc*)ptr)->sqrtAlphaForce = sqrtAlphaForce;	\
		((PHOpHapticRendererDesc*)ptr)->hitWall = hitWall;	\
		((PHOpHapticRendererDesc*)ptr)->useDualTranFix = useDualTranFix;	\
		((PHOpHapticRendererDesc*)ptr)->rigid = rigid;	\
		((PHOpHapticRendererDesc*)ptr)->proxyItrtNum = proxyItrtNum;	\
		((PHOpHapticRendererDesc*)ptr)->proxyItrtMaxNum = proxyItrtMaxNum;	\
		((PHOpHapticRendererDesc*)ptr)->currDtType = currDtType;	\
		((PHOpHapticRendererDesc*)ptr)->objNum = objNum;	\
		((PHOpHapticRendererDesc*)ptr)->noCtcItrNum = noCtcItrNum;	\
		((PHOpHapticRendererDesc*)ptr)->proxyFixPos = proxyFixPos;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpObjDesc \
protected:\
	double	mass;	\
	Matrix3d	inertia;	\
	Vec3d	center;	\
	bool	dynamical;	\
	Vec3f*	objTargetVts;	\
	float	objTargetVtsArr;	\
	int	objTargetVtsNum;	\
	int	assPsNum;	\
	int	assGrpNum;	\
	int	objGrouplinkCount;	\
	int	objId;	\
	bool	objUseDistCstr;	\
	bool	objNoMeshObj;	\
	bool	isRigid;	\
	int	objType;	\
	float	objAverRadius;	\
	float	objMaxRadius;	\
	int	objitrTime;	\
	bool	updateNormals;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHOpObj::SetState((PHOpObjState*)(PHOpObjDesc*)ptr);	\
		mass = ((PHOpObjDesc*)ptr)->mass;	\
		inertia = ((PHOpObjDesc*)ptr)->inertia;	\
		center = ((PHOpObjDesc*)ptr)->center;	\
		dynamical = ((PHOpObjDesc*)ptr)->dynamical;	\
		objTargetVts = ((PHOpObjDesc*)ptr)->objTargetVts;	\
		objTargetVtsArr = ((PHOpObjDesc*)ptr)->objTargetVtsArr;	\
		objTargetVtsNum = ((PHOpObjDesc*)ptr)->objTargetVtsNum;	\
		assPsNum = ((PHOpObjDesc*)ptr)->assPsNum;	\
		assGrpNum = ((PHOpObjDesc*)ptr)->assGrpNum;	\
		objGrouplinkCount = ((PHOpObjDesc*)ptr)->objGrouplinkCount;	\
		objId = ((PHOpObjDesc*)ptr)->objId;	\
		objUseDistCstr = ((PHOpObjDesc*)ptr)->objUseDistCstr;	\
		objNoMeshObj = ((PHOpObjDesc*)ptr)->objNoMeshObj;	\
		isRigid = ((PHOpObjDesc*)ptr)->isRigid;	\
		objType = ((PHOpObjDesc*)ptr)->objType;	\
		objAverRadius = ((PHOpObjDesc*)ptr)->objAverRadius;	\
		objMaxRadius = ((PHOpObjDesc*)ptr)->objMaxRadius;	\
		objitrTime = ((PHOpObjDesc*)ptr)->objitrTime;	\
		updateNormals = ((PHOpObjDesc*)ptr)->updateNormals;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHOpObj::GetState((PHOpObjState*)(PHOpObjDesc*)ptr);	\
		((PHOpObjDesc*)ptr)->mass = mass;	\
		((PHOpObjDesc*)ptr)->inertia = inertia;	\
		((PHOpObjDesc*)ptr)->center = center;	\
		((PHOpObjDesc*)ptr)->dynamical = dynamical;	\
		((PHOpObjDesc*)ptr)->objTargetVts = objTargetVts;	\
		((PHOpObjDesc*)ptr)->objTargetVtsArr = objTargetVtsArr;	\
		((PHOpObjDesc*)ptr)->objTargetVtsNum = objTargetVtsNum;	\
		((PHOpObjDesc*)ptr)->assPsNum = assPsNum;	\
		((PHOpObjDesc*)ptr)->assGrpNum = assGrpNum;	\
		((PHOpObjDesc*)ptr)->objGrouplinkCount = objGrouplinkCount;	\
		((PHOpObjDesc*)ptr)->objId = objId;	\
		((PHOpObjDesc*)ptr)->objUseDistCstr = objUseDistCstr;	\
		((PHOpObjDesc*)ptr)->objNoMeshObj = objNoMeshObj;	\
		((PHOpObjDesc*)ptr)->isRigid = isRigid;	\
		((PHOpObjDesc*)ptr)->objType = objType;	\
		((PHOpObjDesc*)ptr)->objAverRadius = objAverRadius;	\
		((PHOpObjDesc*)ptr)->objMaxRadius = objMaxRadius;	\
		((PHOpObjDesc*)ptr)->objitrTime = objitrTime;	\
		((PHOpObjDesc*)ptr)->updateNormals = updateNormals;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpParticleDesc \
protected:\
	int	pNvertex;	\
	int	pMyGroupInd;	\
	int	pPId;	\
	int	pObjId;	\
	float	pParaAlpha;	\
	bool	hitedByMouse;	\
	float	pTempSingleVMass;	\
	float	pTotalMass;	\
	float	pRadii;	\
	float	pDynamicRadius;	\
	Vec3f	pMainRadiusVec;	\
	Vec3f	pSecRadiusVec;	\
	Vec3f	pThrRadiusVec;	\
	float	pMainRadius;	\
	float	pSecRadius;	\
	float	pThrRadius;	\
	Vec3f	pCurrCtr;	\
	Vec3f	pOrigCtr;	\
	Vec3f	pNewCtr;	\
	Vec3f	pVelocity;	\
	bool	isColliedbySphash;	\
	bool	isColliedbyColliCube;	\
	bool	isFixedbyMouse;	\
	bool	isFixed;	\
	bool	isColliedSphashSolved;	\
	bool	isColliedSphashSolvedReady;	\
	Vec3f	pExternalForce;	\
	Vec3f	pExternalTorque;	\
	Vec3f	pExternalDisplacement;	\
	std::vector< int >	pFaceInd;	\
	Matrix3f	pMomentR;	\
	Matrix3f	pSmR;	\
	Matrix3f	pMomentInertia;	\
	Matrix3f	pInverseOfMomentInertia;	\
	Matrix3f	ellipRotMatrix;	\
	Quaternionf	ellipOrigOrint;	\
	Vec3f	pWvel;	\
	Matrix3f	pPreEllipA;	\
	Quaternionf	pCurrOrint;	\
	Quaternionf	pOrigOrint;	\
	Quaternionf	pNewOrint;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		pNvertex = ((PHOpParticleDesc*)ptr)->pNvertex;	\
		pMyGroupInd = ((PHOpParticleDesc*)ptr)->pMyGroupInd;	\
		pPId = ((PHOpParticleDesc*)ptr)->pPId;	\
		pObjId = ((PHOpParticleDesc*)ptr)->pObjId;	\
		pParaAlpha = ((PHOpParticleDesc*)ptr)->pParaAlpha;	\
		hitedByMouse = ((PHOpParticleDesc*)ptr)->hitedByMouse;	\
		pTempSingleVMass = ((PHOpParticleDesc*)ptr)->pTempSingleVMass;	\
		pTotalMass = ((PHOpParticleDesc*)ptr)->pTotalMass;	\
		pRadii = ((PHOpParticleDesc*)ptr)->pRadii;	\
		pDynamicRadius = ((PHOpParticleDesc*)ptr)->pDynamicRadius;	\
		pMainRadiusVec = ((PHOpParticleDesc*)ptr)->pMainRadiusVec;	\
		pSecRadiusVec = ((PHOpParticleDesc*)ptr)->pSecRadiusVec;	\
		pThrRadiusVec = ((PHOpParticleDesc*)ptr)->pThrRadiusVec;	\
		pMainRadius = ((PHOpParticleDesc*)ptr)->pMainRadius;	\
		pSecRadius = ((PHOpParticleDesc*)ptr)->pSecRadius;	\
		pThrRadius = ((PHOpParticleDesc*)ptr)->pThrRadius;	\
		pCurrCtr = ((PHOpParticleDesc*)ptr)->pCurrCtr;	\
		pOrigCtr = ((PHOpParticleDesc*)ptr)->pOrigCtr;	\
		pNewCtr = ((PHOpParticleDesc*)ptr)->pNewCtr;	\
		pVelocity = ((PHOpParticleDesc*)ptr)->pVelocity;	\
		isColliedbySphash = ((PHOpParticleDesc*)ptr)->isColliedbySphash;	\
		isColliedbyColliCube = ((PHOpParticleDesc*)ptr)->isColliedbyColliCube;	\
		isFixedbyMouse = ((PHOpParticleDesc*)ptr)->isFixedbyMouse;	\
		isFixed = ((PHOpParticleDesc*)ptr)->isFixed;	\
		isColliedSphashSolved = ((PHOpParticleDesc*)ptr)->isColliedSphashSolved;	\
		isColliedSphashSolvedReady = ((PHOpParticleDesc*)ptr)->isColliedSphashSolvedReady;	\
		pExternalForce = ((PHOpParticleDesc*)ptr)->pExternalForce;	\
		pExternalTorque = ((PHOpParticleDesc*)ptr)->pExternalTorque;	\
		pExternalDisplacement = ((PHOpParticleDesc*)ptr)->pExternalDisplacement;	\
		pFaceInd = ((PHOpParticleDesc*)ptr)->pFaceInd;	\
		pMomentR = ((PHOpParticleDesc*)ptr)->pMomentR;	\
		pSmR = ((PHOpParticleDesc*)ptr)->pSmR;	\
		pMomentInertia = ((PHOpParticleDesc*)ptr)->pMomentInertia;	\
		pInverseOfMomentInertia = ((PHOpParticleDesc*)ptr)->pInverseOfMomentInertia;	\
		ellipRotMatrix = ((PHOpParticleDesc*)ptr)->ellipRotMatrix;	\
		ellipOrigOrint = ((PHOpParticleDesc*)ptr)->ellipOrigOrint;	\
		pWvel = ((PHOpParticleDesc*)ptr)->pWvel;	\
		pPreEllipA = ((PHOpParticleDesc*)ptr)->pPreEllipA;	\
		pCurrOrint = ((PHOpParticleDesc*)ptr)->pCurrOrint;	\
		pOrigOrint = ((PHOpParticleDesc*)ptr)->pOrigOrint;	\
		pNewOrint = ((PHOpParticleDesc*)ptr)->pNewOrint;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHOpParticleDesc*)ptr)->pNvertex = pNvertex;	\
		((PHOpParticleDesc*)ptr)->pMyGroupInd = pMyGroupInd;	\
		((PHOpParticleDesc*)ptr)->pPId = pPId;	\
		((PHOpParticleDesc*)ptr)->pObjId = pObjId;	\
		((PHOpParticleDesc*)ptr)->pParaAlpha = pParaAlpha;	\
		((PHOpParticleDesc*)ptr)->hitedByMouse = hitedByMouse;	\
		((PHOpParticleDesc*)ptr)->pTempSingleVMass = pTempSingleVMass;	\
		((PHOpParticleDesc*)ptr)->pTotalMass = pTotalMass;	\
		((PHOpParticleDesc*)ptr)->pRadii = pRadii;	\
		((PHOpParticleDesc*)ptr)->pDynamicRadius = pDynamicRadius;	\
		((PHOpParticleDesc*)ptr)->pMainRadiusVec = pMainRadiusVec;	\
		((PHOpParticleDesc*)ptr)->pSecRadiusVec = pSecRadiusVec;	\
		((PHOpParticleDesc*)ptr)->pThrRadiusVec = pThrRadiusVec;	\
		((PHOpParticleDesc*)ptr)->pMainRadius = pMainRadius;	\
		((PHOpParticleDesc*)ptr)->pSecRadius = pSecRadius;	\
		((PHOpParticleDesc*)ptr)->pThrRadius = pThrRadius;	\
		((PHOpParticleDesc*)ptr)->pCurrCtr = pCurrCtr;	\
		((PHOpParticleDesc*)ptr)->pOrigCtr = pOrigCtr;	\
		((PHOpParticleDesc*)ptr)->pNewCtr = pNewCtr;	\
		((PHOpParticleDesc*)ptr)->pVelocity = pVelocity;	\
		((PHOpParticleDesc*)ptr)->isColliedbySphash = isColliedbySphash;	\
		((PHOpParticleDesc*)ptr)->isColliedbyColliCube = isColliedbyColliCube;	\
		((PHOpParticleDesc*)ptr)->isFixedbyMouse = isFixedbyMouse;	\
		((PHOpParticleDesc*)ptr)->isFixed = isFixed;	\
		((PHOpParticleDesc*)ptr)->isColliedSphashSolved = isColliedSphashSolved;	\
		((PHOpParticleDesc*)ptr)->isColliedSphashSolvedReady = isColliedSphashSolvedReady;	\
		((PHOpParticleDesc*)ptr)->pExternalForce = pExternalForce;	\
		((PHOpParticleDesc*)ptr)->pExternalTorque = pExternalTorque;	\
		((PHOpParticleDesc*)ptr)->pExternalDisplacement = pExternalDisplacement;	\
		((PHOpParticleDesc*)ptr)->pFaceInd = pFaceInd;	\
		((PHOpParticleDesc*)ptr)->pMomentR = pMomentR;	\
		((PHOpParticleDesc*)ptr)->pSmR = pSmR;	\
		((PHOpParticleDesc*)ptr)->pMomentInertia = pMomentInertia;	\
		((PHOpParticleDesc*)ptr)->pInverseOfMomentInertia = pInverseOfMomentInertia;	\
		((PHOpParticleDesc*)ptr)->ellipRotMatrix = ellipRotMatrix;	\
		((PHOpParticleDesc*)ptr)->ellipOrigOrint = ellipOrigOrint;	\
		((PHOpParticleDesc*)ptr)->pWvel = pWvel;	\
		((PHOpParticleDesc*)ptr)->pPreEllipA = pPreEllipA;	\
		((PHOpParticleDesc*)ptr)->pCurrOrint = pCurrOrint;	\
		((PHOpParticleDesc*)ptr)->pOrigOrint = pOrigOrint;	\
		((PHOpParticleDesc*)ptr)->pNewOrint = pNewOrint;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpGroupDesc \
protected:\
	int	gMyIndex;	\
	Matrix3f	gAgroup;	\
	Matrix3f	gEigen;	\
	float	gtotalMass;	\
	Vec3f	gOrigCenter;	\
	Vec3f	gCurrCenter;	\
	int	gNptcl;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		gMyIndex = ((PHOpGroupDesc*)ptr)->gMyIndex;	\
		gAgroup = ((PHOpGroupDesc*)ptr)->gAgroup;	\
		gEigen = ((PHOpGroupDesc*)ptr)->gEigen;	\
		gtotalMass = ((PHOpGroupDesc*)ptr)->gtotalMass;	\
		gOrigCenter = ((PHOpGroupDesc*)ptr)->gOrigCenter;	\
		gCurrCenter = ((PHOpGroupDesc*)ptr)->gCurrCenter;	\
		gNptcl = ((PHOpGroupDesc*)ptr)->gNptcl;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHOpGroupDesc*)ptr)->gMyIndex = gMyIndex;	\
		((PHOpGroupDesc*)ptr)->gAgroup = gAgroup;	\
		((PHOpGroupDesc*)ptr)->gEigen = gEigen;	\
		((PHOpGroupDesc*)ptr)->gtotalMass = gtotalMass;	\
		((PHOpGroupDesc*)ptr)->gOrigCenter = gOrigCenter;	\
		((PHOpGroupDesc*)ptr)->gCurrCenter = gCurrCenter;	\
		((PHOpGroupDesc*)ptr)->gNptcl = gNptcl;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpHapticControllerDesc \
protected:\
	float	posScale;	\
	float	rotScale;	\
	float	forceScale;	\
	Vec3f	userPos;	\
	Posef	userPose;	\
	Vec3f	hcCurrPPos;	\
	Vec3f	hcCurrUPos;	\
	Vec3f	hcLastUPos;	\
	Vec3f	hcFixsubGoal;	\
	float	c_obstRadius;	\
	int	hpObjIndex;	\
	Spr::PHOpHapticControllerDesc::HapticDOFType	hcType;	\
	bool	logForce;	\
	bool	hcReady;	\
	bool	IsSetForceReady;	\
	int	fileindex;	\
	Vec3f	lastHpRateF;	\
	bool	hcCollied;	\
	int	constrainCount;	\
	int	constrainCountHc;	\
	Vec3f	couterPlaneN;	\
	float	hcElmDtcRadius;	\
	int	collectCount;	\
	int	collectItrtor;	\
	bool	hcProxyOn;	\
	int	suspObjid;	\
	float	proxyRadius;	\
	int	surrCnstrs;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		posScale = ((PHOpHapticControllerDesc*)ptr)->posScale;	\
		rotScale = ((PHOpHapticControllerDesc*)ptr)->rotScale;	\
		forceScale = ((PHOpHapticControllerDesc*)ptr)->forceScale;	\
		userPos = ((PHOpHapticControllerDesc*)ptr)->userPos;	\
		userPose = ((PHOpHapticControllerDesc*)ptr)->userPose;	\
		hcCurrPPos = ((PHOpHapticControllerDesc*)ptr)->hcCurrPPos;	\
		hcCurrUPos = ((PHOpHapticControllerDesc*)ptr)->hcCurrUPos;	\
		hcLastUPos = ((PHOpHapticControllerDesc*)ptr)->hcLastUPos;	\
		hcFixsubGoal = ((PHOpHapticControllerDesc*)ptr)->hcFixsubGoal;	\
		c_obstRadius = ((PHOpHapticControllerDesc*)ptr)->c_obstRadius;	\
		hpObjIndex = ((PHOpHapticControllerDesc*)ptr)->hpObjIndex;	\
		hcType = ((PHOpHapticControllerDesc*)ptr)->hcType;	\
		logForce = ((PHOpHapticControllerDesc*)ptr)->logForce;	\
		hcReady = ((PHOpHapticControllerDesc*)ptr)->hcReady;	\
		IsSetForceReady = ((PHOpHapticControllerDesc*)ptr)->IsSetForceReady;	\
		fileindex = ((PHOpHapticControllerDesc*)ptr)->fileindex;	\
		lastHpRateF = ((PHOpHapticControllerDesc*)ptr)->lastHpRateF;	\
		hcCollied = ((PHOpHapticControllerDesc*)ptr)->hcCollied;	\
		constrainCount = ((PHOpHapticControllerDesc*)ptr)->constrainCount;	\
		constrainCountHc = ((PHOpHapticControllerDesc*)ptr)->constrainCountHc;	\
		couterPlaneN = ((PHOpHapticControllerDesc*)ptr)->couterPlaneN;	\
		hcElmDtcRadius = ((PHOpHapticControllerDesc*)ptr)->hcElmDtcRadius;	\
		collectCount = ((PHOpHapticControllerDesc*)ptr)->collectCount;	\
		collectItrtor = ((PHOpHapticControllerDesc*)ptr)->collectItrtor;	\
		hcProxyOn = ((PHOpHapticControllerDesc*)ptr)->hcProxyOn;	\
		suspObjid = ((PHOpHapticControllerDesc*)ptr)->suspObjid;	\
		proxyRadius = ((PHOpHapticControllerDesc*)ptr)->proxyRadius;	\
		surrCnstrs = ((PHOpHapticControllerDesc*)ptr)->surrCnstrs;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHOpHapticControllerDesc*)ptr)->posScale = posScale;	\
		((PHOpHapticControllerDesc*)ptr)->rotScale = rotScale;	\
		((PHOpHapticControllerDesc*)ptr)->forceScale = forceScale;	\
		((PHOpHapticControllerDesc*)ptr)->userPos = userPos;	\
		((PHOpHapticControllerDesc*)ptr)->userPose = userPose;	\
		((PHOpHapticControllerDesc*)ptr)->hcCurrPPos = hcCurrPPos;	\
		((PHOpHapticControllerDesc*)ptr)->hcCurrUPos = hcCurrUPos;	\
		((PHOpHapticControllerDesc*)ptr)->hcLastUPos = hcLastUPos;	\
		((PHOpHapticControllerDesc*)ptr)->hcFixsubGoal = hcFixsubGoal;	\
		((PHOpHapticControllerDesc*)ptr)->c_obstRadius = c_obstRadius;	\
		((PHOpHapticControllerDesc*)ptr)->hpObjIndex = hpObjIndex;	\
		((PHOpHapticControllerDesc*)ptr)->hcType = hcType;	\
		((PHOpHapticControllerDesc*)ptr)->logForce = logForce;	\
		((PHOpHapticControllerDesc*)ptr)->hcReady = hcReady;	\
		((PHOpHapticControllerDesc*)ptr)->IsSetForceReady = IsSetForceReady;	\
		((PHOpHapticControllerDesc*)ptr)->fileindex = fileindex;	\
		((PHOpHapticControllerDesc*)ptr)->lastHpRateF = lastHpRateF;	\
		((PHOpHapticControllerDesc*)ptr)->hcCollied = hcCollied;	\
		((PHOpHapticControllerDesc*)ptr)->constrainCount = constrainCount;	\
		((PHOpHapticControllerDesc*)ptr)->constrainCountHc = constrainCountHc;	\
		((PHOpHapticControllerDesc*)ptr)->couterPlaneN = couterPlaneN;	\
		((PHOpHapticControllerDesc*)ptr)->hcElmDtcRadius = hcElmDtcRadius;	\
		((PHOpHapticControllerDesc*)ptr)->collectCount = collectCount;	\
		((PHOpHapticControllerDesc*)ptr)->collectItrtor = collectItrtor;	\
		((PHOpHapticControllerDesc*)ptr)->hcProxyOn = hcProxyOn;	\
		((PHOpHapticControllerDesc*)ptr)->suspObjid = suspObjid;	\
		((PHOpHapticControllerDesc*)ptr)->proxyRadius = proxyRadius;	\
		((PHOpHapticControllerDesc*)ptr)->surrCnstrs = surrCnstrs;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpAnimationDesc \
protected:\
	float	timestep;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		timestep = ((PHOpAnimationDesc*)ptr)->timestep;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHOpAnimationDesc*)ptr)->timestep = timestep;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHOpSpHashColliAgentDesc \
protected:\
	bool	useDirColli;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		useDirColli = ((PHOpSpHashColliAgentDesc*)ptr)->useDirColli;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((PHOpSpHashColliAgentDesc*)ptr)->useDirColli = useDirColli;	\
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
	double	maxDeltaPosition;	\
	double	maxDeltaOrientation;	\
	double	maxVelocity;	\
	double	maxAngularVelocity;	\
	double	maxForce;	\
	double	maxMoment;	\
	int	numIteration;	\
	int	method;	\
	bool	bContactDetectionEnabled;	\
	bool	bCCDEnabled;	\
	int	broadPhaseMode;	\
	int	blendMode;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHScene::SetState((PHSceneState*)(PHSceneDesc*)ptr);	\
		gravity = ((PHSceneDesc*)ptr)->gravity;	\
		airResistanceRate = ((PHSceneDesc*)ptr)->airResistanceRate;	\
		contactTolerance = ((PHSceneDesc*)ptr)->contactTolerance;	\
		impactThreshold = ((PHSceneDesc*)ptr)->impactThreshold;	\
		frictionThreshold = ((PHSceneDesc*)ptr)->frictionThreshold;	\
		maxDeltaPosition = ((PHSceneDesc*)ptr)->maxDeltaPosition;	\
		maxDeltaOrientation = ((PHSceneDesc*)ptr)->maxDeltaOrientation;	\
		maxVelocity = ((PHSceneDesc*)ptr)->maxVelocity;	\
		maxAngularVelocity = ((PHSceneDesc*)ptr)->maxAngularVelocity;	\
		maxForce = ((PHSceneDesc*)ptr)->maxForce;	\
		maxMoment = ((PHSceneDesc*)ptr)->maxMoment;	\
		numIteration = ((PHSceneDesc*)ptr)->numIteration;	\
		method = ((PHSceneDesc*)ptr)->method;	\
		bContactDetectionEnabled = ((PHSceneDesc*)ptr)->bContactDetectionEnabled;	\
		bCCDEnabled = ((PHSceneDesc*)ptr)->bCCDEnabled;	\
		broadPhaseMode = ((PHSceneDesc*)ptr)->broadPhaseMode;	\
		blendMode = ((PHSceneDesc*)ptr)->blendMode;	\
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
		((PHSceneDesc*)ptr)->maxDeltaPosition = maxDeltaPosition;	\
		((PHSceneDesc*)ptr)->maxDeltaOrientation = maxDeltaOrientation;	\
		((PHSceneDesc*)ptr)->maxVelocity = maxVelocity;	\
		((PHSceneDesc*)ptr)->maxAngularVelocity = maxAngularVelocity;	\
		((PHSceneDesc*)ptr)->maxForce = maxForce;	\
		((PHSceneDesc*)ptr)->maxMoment = maxMoment;	\
		((PHSceneDesc*)ptr)->numIteration = numIteration;	\
		((PHSceneDesc*)ptr)->method = method;	\
		((PHSceneDesc*)ptr)->bContactDetectionEnabled = bContactDetectionEnabled;	\
		((PHSceneDesc*)ptr)->bCCDEnabled = bCCDEnabled;	\
		((PHSceneDesc*)ptr)->broadPhaseMode = broadPhaseMode;	\
		((PHSceneDesc*)ptr)->blendMode = blendMode;	\
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

#define SPR_DECLMEMBEROF_PHBoneDesc \
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

#define SPR_DECLMEMBEROF_PHSkeletonDesc \
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
	bool	stationary;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		PHSolid::SetState((PHSolidState*)(PHSolidDesc*)ptr);	\
		mass = ((PHSolidDesc*)ptr)->mass;	\
		inertia = ((PHSolidDesc*)ptr)->inertia;	\
		center = ((PHSolidDesc*)ptr)->center;	\
		dynamical = ((PHSolidDesc*)ptr)->dynamical;	\
		stationary = ((PHSolidDesc*)ptr)->stationary;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		PHSolid::GetState((PHSolidState*)(PHSolidDesc*)ptr);	\
		((PHSolidDesc*)ptr)->mass = mass;	\
		((PHSolidDesc*)ptr)->inertia = inertia;	\
		((PHSolidDesc*)ptr)->center = center;	\
		((PHSolidDesc*)ptr)->dynamical = dynamical;	\
		((PHSolidDesc*)ptr)->stationary = stationary;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_PHConstraintState \
protected:\
	Vec6d	f;	\
	Vec6d	F;	\
	Vec6d	xs;	\
	Vec6d	fAvg;	\
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

