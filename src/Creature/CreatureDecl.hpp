#define SPR_DECLMEMBEROF_CRBodyDesc \
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

#define SPR_DECLMEMBEROF_CRBoneDesc \
protected:\
	std::string	label;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		label = ((CRBoneDesc*)ptr)->label;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((CRBoneDesc*)ptr)->label = label;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRControllerDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGazeControllerDesc \
protected:\
	Vec3d	front;	\
	Vec3d	up;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		front = ((CRGazeControllerDesc*)ptr)->front;	\
		up = ((CRGazeControllerDesc*)ptr)->up;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((CRGazeControllerDesc*)ptr)->front = front;	\
		((CRGazeControllerDesc*)ptr)->up = up;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRGrabControllerDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRGrabControllerDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRGrabControllerDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRCreatureDesc \
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

#define SPR_DECLMEMBEROF_CREngineDesc \
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

#define SPR_DECLMEMBEROF_CRReachControllerState \
protected:\
	float	time;	\
	float	reachTime;	\
	float	viaTime;	\
	Vec3d	pos0;	\
	Vec3d	vel0;	\
	Vec3d	pos1;	\
	Vec3d	vel1;	\
	Vec3d	pos1AtStartTime;	\
	Quaterniond	ori0;	\
	Quaterniond	ori1;	\
	Vec3d	viaPos;	\
	Vec3d	currPos;	\
	Vec3d	currVel;	\
	Vec3d	targetPos;	\
	Vec3d	lastTargetPos;	\
	Vec3d	tempTargetPos;	\
	Vec3d	lastTempTargetPos;	\
	Vec3d	tempTargetPosAtStartTime;	\
	double	posInnerMargin;	\
	double	posOuterMargin;	\
	double	dirInnerMargin;	\
	double	dirOuterMargin;	\
	Spr::CRReachControllerState::CRReachInOut	posInOut;	\
	Spr::CRReachControllerState::CRReachInOut	dirInOut;	\
	Spr::CRReachControllerState::CRReachStat	posStat;	\
	Spr::CRReachControllerState::CRReachStat	dirStat;	\
	double	posWaitStarted;	\
	double	dirWaitStarted;	\
public:\
	virtual void SetState(const void* ptr){ \
		time = ((CRReachControllerState*)ptr)->time;	\
		reachTime = ((CRReachControllerState*)ptr)->reachTime;	\
		viaTime = ((CRReachControllerState*)ptr)->viaTime;	\
		pos0 = ((CRReachControllerState*)ptr)->pos0;	\
		vel0 = ((CRReachControllerState*)ptr)->vel0;	\
		pos1 = ((CRReachControllerState*)ptr)->pos1;	\
		vel1 = ((CRReachControllerState*)ptr)->vel1;	\
		pos1AtStartTime = ((CRReachControllerState*)ptr)->pos1AtStartTime;	\
		ori0 = ((CRReachControllerState*)ptr)->ori0;	\
		ori1 = ((CRReachControllerState*)ptr)->ori1;	\
		viaPos = ((CRReachControllerState*)ptr)->viaPos;	\
		currPos = ((CRReachControllerState*)ptr)->currPos;	\
		currVel = ((CRReachControllerState*)ptr)->currVel;	\
		targetPos = ((CRReachControllerState*)ptr)->targetPos;	\
		lastTargetPos = ((CRReachControllerState*)ptr)->lastTargetPos;	\
		tempTargetPos = ((CRReachControllerState*)ptr)->tempTargetPos;	\
		lastTempTargetPos = ((CRReachControllerState*)ptr)->lastTempTargetPos;	\
		tempTargetPosAtStartTime = ((CRReachControllerState*)ptr)->tempTargetPosAtStartTime;	\
		posInnerMargin = ((CRReachControllerState*)ptr)->posInnerMargin;	\
		posOuterMargin = ((CRReachControllerState*)ptr)->posOuterMargin;	\
		dirInnerMargin = ((CRReachControllerState*)ptr)->dirInnerMargin;	\
		dirOuterMargin = ((CRReachControllerState*)ptr)->dirOuterMargin;	\
		posInOut = ((CRReachControllerState*)ptr)->posInOut;	\
		dirInOut = ((CRReachControllerState*)ptr)->dirInOut;	\
		posStat = ((CRReachControllerState*)ptr)->posStat;	\
		dirStat = ((CRReachControllerState*)ptr)->dirStat;	\
		posWaitStarted = ((CRReachControllerState*)ptr)->posWaitStarted;	\
		dirWaitStarted = ((CRReachControllerState*)ptr)->dirWaitStarted;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((CRReachControllerState*)ptr)->time = time;	\
		((CRReachControllerState*)ptr)->reachTime = reachTime;	\
		((CRReachControllerState*)ptr)->viaTime = viaTime;	\
		((CRReachControllerState*)ptr)->pos0 = pos0;	\
		((CRReachControllerState*)ptr)->vel0 = vel0;	\
		((CRReachControllerState*)ptr)->pos1 = pos1;	\
		((CRReachControllerState*)ptr)->vel1 = vel1;	\
		((CRReachControllerState*)ptr)->pos1AtStartTime = pos1AtStartTime;	\
		((CRReachControllerState*)ptr)->ori0 = ori0;	\
		((CRReachControllerState*)ptr)->ori1 = ori1;	\
		((CRReachControllerState*)ptr)->viaPos = viaPos;	\
		((CRReachControllerState*)ptr)->currPos = currPos;	\
		((CRReachControllerState*)ptr)->currVel = currVel;	\
		((CRReachControllerState*)ptr)->targetPos = targetPos;	\
		((CRReachControllerState*)ptr)->lastTargetPos = lastTargetPos;	\
		((CRReachControllerState*)ptr)->tempTargetPos = tempTargetPos;	\
		((CRReachControllerState*)ptr)->lastTempTargetPos = lastTempTargetPos;	\
		((CRReachControllerState*)ptr)->tempTargetPosAtStartTime = tempTargetPosAtStartTime;	\
		((CRReachControllerState*)ptr)->posInnerMargin = posInnerMargin;	\
		((CRReachControllerState*)ptr)->posOuterMargin = posOuterMargin;	\
		((CRReachControllerState*)ptr)->dirInnerMargin = dirInnerMargin;	\
		((CRReachControllerState*)ptr)->dirOuterMargin = dirOuterMargin;	\
		((CRReachControllerState*)ptr)->posInOut = posInOut;	\
		((CRReachControllerState*)ptr)->dirInOut = dirInOut;	\
		((CRReachControllerState*)ptr)->posStat = posStat;	\
		((CRReachControllerState*)ptr)->dirStat = dirStat;	\
		((CRReachControllerState*)ptr)->posWaitStarted = posWaitStarted;	\
		((CRReachControllerState*)ptr)->dirWaitStarted = dirWaitStarted;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRReachControllerDesc \
protected:\
	double	averageSpeed;	\
	double	minWait;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRReachControllerDesc*)ptr);	\
		CRReachController::SetState((CRReachControllerState*)(CRReachControllerDesc*)ptr);	\
		averageSpeed = ((CRReachControllerDesc*)ptr)->averageSpeed;	\
		minWait = ((CRReachControllerDesc*)ptr)->minWait;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRReachControllerDesc*)ptr);	\
		CRReachController::GetState((CRReachControllerState*)(CRReachControllerDesc*)ptr);	\
		((CRReachControllerDesc*)ptr)->averageSpeed = averageSpeed;	\
		((CRReachControllerDesc*)ptr)->minWait = minWait;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRSdkDesc \
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

#define SPR_DECLMEMBEROF_CRTouchSensorDesc \
protected:\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRTouchSensorDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRTouchSensorDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRVisualSensorDesc \
protected:\
	Vec2d	range;	\
	Vec2d	centerRange;	\
	Posed	pose;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		CREngine::SetDesc((CREngineDesc*)(CRVisualSensorDesc*)ptr);	\
		range = ((CRVisualSensorDesc*)ptr)->range;	\
		centerRange = ((CRVisualSensorDesc*)ptr)->centerRange;	\
		pose = ((CRVisualSensorDesc*)ptr)->pose;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CREngine::GetDesc((CREngineDesc*)(CRVisualSensorDesc*)ptr);	\
		((CRVisualSensorDesc*)ptr)->range = range;	\
		((CRVisualSensorDesc*)ptr)->centerRange = centerRange;	\
		((CRVisualSensorDesc*)ptr)->pose = pose;	\
		return true;	\
	}\

