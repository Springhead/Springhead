#define SPR_DECLMEMBEROF_CRBodyPartDesc \
protected:\
	std::string	label;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		label = ((CRBodyPartDesc*)ptr)->label;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((CRBodyPartDesc*)ptr)->label = label;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRSolidDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBodyPart::SetDesc((CRBodyPartDesc*)(CRSolidDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBodyPart::GetDesc((CRBodyPartDesc*)(CRSolidDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKSolidDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRSolid::SetDesc((CRSolidDesc*)(CRIKSolidDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRSolid::GetDesc((CRSolidDesc*)(CRIKSolidDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRBodyPart::SetDesc((CRBodyPartDesc*)(CRJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRBodyPart::GetDesc((CRBodyPartDesc*)(CRJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRIKJointDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CRJoint::SetDesc((CRJointDesc*)(CRIKJointDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRJoint::GetDesc((CRJointDesc*)(CRIKJointDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRBodyDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRCreatureDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREngineDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREyeControllerState \
protected:\
	enum Spr::CREyeControllerState::ControlState	controlState;	\
public:\
	virtual void SetState(const void* ptr){ \
		controlState = ((CREyeControllerState*)ptr)->controlState;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((CREyeControllerState*)ptr)->controlState = controlState;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CREyeControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CREyeControllerDesc*)ptr);	\
		CREyeController::SetState((CREyeControllerState*)(CREyeControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CREyeControllerDesc*)ptr);	\
		CREyeController::GetState((CREyeControllerState*)(CREyeControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRNeckControllerState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(void* ptr) const { \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRNeckControllerDesc \
protected:\
	float	lowerAttractiveness;	\
	float	upperAttractiveness;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRNeckControllerDesc*)ptr);	\
		CRNeckController::SetState((CRNeckControllerState*)(CRNeckControllerDesc*)ptr);	\
		lowerAttractiveness = ((CRNeckControllerDesc*)ptr)->lowerAttractiveness;	\
		upperAttractiveness = ((CRNeckControllerDesc*)ptr)->upperAttractiveness;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRNeckControllerDesc*)ptr);	\
		CRNeckController::GetState((CRNeckControllerState*)(CRNeckControllerDesc*)ptr);	\
		((CRNeckControllerDesc*)ptr)->lowerAttractiveness = lowerAttractiveness;	\
		((CRNeckControllerDesc*)ptr)->upperAttractiveness = upperAttractiveness;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGazeControllerState \
protected:\
public:\
	virtual void SetState(const void* ptr){ \
	}\
	virtual bool GetState(void* ptr) const { \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGazeControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRGazeControllerDesc*)ptr);	\
		CRGazeController::SetState((CRGazeControllerState*)(CRGazeControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRGazeControllerDesc*)ptr);	\
		CRGazeController::GetState((CRGazeControllerState*)(CRGazeControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRAttentionControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRAttentionControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRAttentionControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRReachingControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRReachingControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRReachingControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRReachingControllersDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRReachingControllersDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRReachingControllersDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGrabControllerDesc \
protected:\
	double	rateSpringSoften;	\
	double	rateDamperSoften;	\
	double	rateSpringHarden;	\
	double	rateDamperHarden;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRGrabControllerDesc*)ptr);	\
		rateSpringSoften = ((CRGrabControllerDesc*)ptr)->rateSpringSoften;	\
		rateDamperSoften = ((CRGrabControllerDesc*)ptr)->rateDamperSoften;	\
		rateSpringHarden = ((CRGrabControllerDesc*)ptr)->rateSpringHarden;	\
		rateDamperHarden = ((CRGrabControllerDesc*)ptr)->rateDamperHarden;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRGrabControllerDesc*)ptr);	\
		((CRGrabControllerDesc*)ptr)->rateSpringSoften = rateSpringSoften;	\
		((CRGrabControllerDesc*)ptr)->rateDamperSoften = rateDamperSoften;	\
		((CRGrabControllerDesc*)ptr)->rateSpringHarden = rateSpringHarden;	\
		((CRGrabControllerDesc*)ptr)->rateDamperHarden = rateDamperHarden;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRSceneDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRSdkDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTouchSensorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRTouchSensorDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRTouchSensorDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTrajectoryState \
protected:\
	bool	posEnabled;	\
	bool	velEnabled;	\
	bool	oriEnabled;	\
	bool	angvelEnabled;	\
	double	time;	\
	Vec3f	posStart;	\
	Vec3f	posEnd;	\
	Vec3f	velStart;	\
	Vec3f	velEnd;	\
	Quaternionf	oriStart;	\
	Quaternionf	oriEnd;	\
	Vec3f	angvelStart;	\
	Vec3f	angvelEnd;	\
	Vec3f	currentPosition;	\
	Quaternionf	currentOrientation;	\
public:\
	virtual void SetState(const void* ptr){ \
		posEnabled = ((CRTrajectoryState*)ptr)->posEnabled;	\
		velEnabled = ((CRTrajectoryState*)ptr)->velEnabled;	\
		oriEnabled = ((CRTrajectoryState*)ptr)->oriEnabled;	\
		angvelEnabled = ((CRTrajectoryState*)ptr)->angvelEnabled;	\
		time = ((CRTrajectoryState*)ptr)->time;	\
		posStart = ((CRTrajectoryState*)ptr)->posStart;	\
		posEnd = ((CRTrajectoryState*)ptr)->posEnd;	\
		velStart = ((CRTrajectoryState*)ptr)->velStart;	\
		velEnd = ((CRTrajectoryState*)ptr)->velEnd;	\
		oriStart = ((CRTrajectoryState*)ptr)->oriStart;	\
		oriEnd = ((CRTrajectoryState*)ptr)->oriEnd;	\
		angvelStart = ((CRTrajectoryState*)ptr)->angvelStart;	\
		angvelEnd = ((CRTrajectoryState*)ptr)->angvelEnd;	\
		currentPosition = ((CRTrajectoryState*)ptr)->currentPosition;	\
		currentOrientation = ((CRTrajectoryState*)ptr)->currentOrientation;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((CRTrajectoryState*)ptr)->posEnabled = posEnabled;	\
		((CRTrajectoryState*)ptr)->velEnabled = velEnabled;	\
		((CRTrajectoryState*)ptr)->oriEnabled = oriEnabled;	\
		((CRTrajectoryState*)ptr)->angvelEnabled = angvelEnabled;	\
		((CRTrajectoryState*)ptr)->time = time;	\
		((CRTrajectoryState*)ptr)->posStart = posStart;	\
		((CRTrajectoryState*)ptr)->posEnd = posEnd;	\
		((CRTrajectoryState*)ptr)->velStart = velStart;	\
		((CRTrajectoryState*)ptr)->velEnd = velEnd;	\
		((CRTrajectoryState*)ptr)->oriStart = oriStart;	\
		((CRTrajectoryState*)ptr)->oriEnd = oriEnd;	\
		((CRTrajectoryState*)ptr)->angvelStart = angvelStart;	\
		((CRTrajectoryState*)ptr)->angvelEnd = angvelEnd;	\
		((CRTrajectoryState*)ptr)->currentPosition = currentPosition;	\
		((CRTrajectoryState*)ptr)->currentOrientation = currentOrientation;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTrajectoryDesc \
protected:\
	Vec3f	posInSolid;	\
	Vec3f	posInWorld;	\
	double	timeLimit;	\
	bool	bEnabled;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRTrajectoryDesc*)ptr);	\
		CRTrajectory::SetState((CRTrajectoryState*)(CRTrajectoryDesc*)ptr);	\
		posInSolid = ((CRTrajectoryDesc*)ptr)->posInSolid;	\
		posInWorld = ((CRTrajectoryDesc*)ptr)->posInWorld;	\
		timeLimit = ((CRTrajectoryDesc*)ptr)->timeLimit;	\
		bEnabled = ((CRTrajectoryDesc*)ptr)->bEnabled;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRTrajectoryDesc*)ptr);	\
		CRTrajectory::GetState((CRTrajectoryState*)(CRTrajectoryDesc*)ptr);	\
		((CRTrajectoryDesc*)ptr)->posInSolid = posInSolid;	\
		((CRTrajectoryDesc*)ptr)->posInWorld = posInWorld;	\
		((CRTrajectoryDesc*)ptr)->timeLimit = timeLimit;	\
		((CRTrajectoryDesc*)ptr)->bEnabled = bEnabled;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRTrajectoryControllerDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRTrajectoryControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRTrajectoryControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRVisualSensorDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRVisualSensorDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRVisualSensorDesc*)ptr);	\
		return true;	\
	}\

