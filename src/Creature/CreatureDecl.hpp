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
	float	oricontTimeRatio;	\
	Vec3d	initPos;	\
	Vec3d	initVel;	\
	Vec3d	initAVel;	\
	Quaterniond	initOri;	\
	Vec3d	viaPos;	\
	Quaterniond	viaOri;	\
	Vec3d	targPos;	\
	Vec3d	targVel;	\
	Vec3d	targAVel;	\
	Quaterniond	targOri;	\
	Vec3d	currPos;	\
	Vec3d	currVel;	\
	Vec3d	currAVel;	\
	Quaterniond	currOri;	\
	Vec3d	finalPos;	\
	Vec3d	finalVel;	\
	Vec3d	finalAVel;	\
	Quaterniond	finalOri;	\
public:\
	virtual void SetState(const void* ptr){ \
		time = ((CRReachControllerState*)ptr)->time;	\
		reachTime = ((CRReachControllerState*)ptr)->reachTime;	\
		viaTime = ((CRReachControllerState*)ptr)->viaTime;	\
		oricontTimeRatio = ((CRReachControllerState*)ptr)->oricontTimeRatio;	\
		initPos = ((CRReachControllerState*)ptr)->initPos;	\
		initVel = ((CRReachControllerState*)ptr)->initVel;	\
		initAVel = ((CRReachControllerState*)ptr)->initAVel;	\
		initOri = ((CRReachControllerState*)ptr)->initOri;	\
		viaPos = ((CRReachControllerState*)ptr)->viaPos;	\
		viaOri = ((CRReachControllerState*)ptr)->viaOri;	\
		targPos = ((CRReachControllerState*)ptr)->targPos;	\
		targVel = ((CRReachControllerState*)ptr)->targVel;	\
		targAVel = ((CRReachControllerState*)ptr)->targAVel;	\
		targOri = ((CRReachControllerState*)ptr)->targOri;	\
		currPos = ((CRReachControllerState*)ptr)->currPos;	\
		currVel = ((CRReachControllerState*)ptr)->currVel;	\
		currAVel = ((CRReachControllerState*)ptr)->currAVel;	\
		currOri = ((CRReachControllerState*)ptr)->currOri;	\
		finalPos = ((CRReachControllerState*)ptr)->finalPos;	\
		finalVel = ((CRReachControllerState*)ptr)->finalVel;	\
		finalAVel = ((CRReachControllerState*)ptr)->finalAVel;	\
		finalOri = ((CRReachControllerState*)ptr)->finalOri;	\
	}\
	virtual bool GetState(void* ptr) const { \
		((CRReachControllerState*)ptr)->time = time;	\
		((CRReachControllerState*)ptr)->reachTime = reachTime;	\
		((CRReachControllerState*)ptr)->viaTime = viaTime;	\
		((CRReachControllerState*)ptr)->oricontTimeRatio = oricontTimeRatio;	\
		((CRReachControllerState*)ptr)->initPos = initPos;	\
		((CRReachControllerState*)ptr)->initVel = initVel;	\
		((CRReachControllerState*)ptr)->initAVel = initAVel;	\
		((CRReachControllerState*)ptr)->initOri = initOri;	\
		((CRReachControllerState*)ptr)->viaPos = viaPos;	\
		((CRReachControllerState*)ptr)->viaOri = viaOri;	\
		((CRReachControllerState*)ptr)->targPos = targPos;	\
		((CRReachControllerState*)ptr)->targVel = targVel;	\
		((CRReachControllerState*)ptr)->targAVel = targAVel;	\
		((CRReachControllerState*)ptr)->targOri = targOri;	\
		((CRReachControllerState*)ptr)->currPos = currPos;	\
		((CRReachControllerState*)ptr)->currVel = currVel;	\
		((CRReachControllerState*)ptr)->currAVel = currAVel;	\
		((CRReachControllerState*)ptr)->currOri = currOri;	\
		((CRReachControllerState*)ptr)->finalPos = finalPos;	\
		((CRReachControllerState*)ptr)->finalVel = finalVel;	\
		((CRReachControllerState*)ptr)->finalAVel = finalAVel;	\
		((CRReachControllerState*)ptr)->finalOri = finalOri;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_CRReachControllerDesc \
protected:\
	double	margin;	\
	double	averageSpeed;	\
	double	waitVel;	\
	double	restartVel;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		CRController::SetDesc((CRControllerDesc*)(CRReachControllerDesc*)ptr);	\
		CRReachController::SetState((CRReachControllerState*)(CRReachControllerDesc*)ptr);	\
		margin = ((CRReachControllerDesc*)ptr)->margin;	\
		averageSpeed = ((CRReachControllerDesc*)ptr)->averageSpeed;	\
		waitVel = ((CRReachControllerDesc*)ptr)->waitVel;	\
		restartVel = ((CRReachControllerDesc*)ptr)->restartVel;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		CRController::GetDesc((CRControllerDesc*)(CRReachControllerDesc*)ptr);	\
		CRReachController::GetState((CRReachControllerState*)(CRReachControllerDesc*)ptr);	\
		((CRReachControllerDesc*)ptr)->margin = margin;	\
		((CRReachControllerDesc*)ptr)->averageSpeed = averageSpeed;	\
		((CRReachControllerDesc*)ptr)->waitVel = waitVel;	\
		((CRReachControllerDesc*)ptr)->restartVel = restartVel;	\
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

