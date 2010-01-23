#define SPR_DECLMEMBEROF_FWBoneDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWInteractPointerDesc \
protected:\
	PHSolidIf*	pointerSolid;	\
	HIBaseIf*	humanInterface;	\
	double	springK;	\
	double	damperD;	\
	double	springOriK;	\
	double	damperOriD;	\
	Posed	defaultPosition;	\
	Posed	pointersCalibPosition;	\
	double	posScale;	\
	double	worldScale;	\
	double	forceScale;	\
	double	torqueScale;	\
	double	localRange;	\
	Quaterniond	cameraOri;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		pointerSolid = ((FWInteractPointerDesc*)ptr)->pointerSolid;	\
		humanInterface = ((FWInteractPointerDesc*)ptr)->humanInterface;	\
		springK = ((FWInteractPointerDesc*)ptr)->springK;	\
		damperD = ((FWInteractPointerDesc*)ptr)->damperD;	\
		springOriK = ((FWInteractPointerDesc*)ptr)->springOriK;	\
		damperOriD = ((FWInteractPointerDesc*)ptr)->damperOriD;	\
		defaultPosition = ((FWInteractPointerDesc*)ptr)->defaultPosition;	\
		pointersCalibPosition = ((FWInteractPointerDesc*)ptr)->pointersCalibPosition;	\
		posScale = ((FWInteractPointerDesc*)ptr)->posScale;	\
		worldScale = ((FWInteractPointerDesc*)ptr)->worldScale;	\
		forceScale = ((FWInteractPointerDesc*)ptr)->forceScale;	\
		torqueScale = ((FWInteractPointerDesc*)ptr)->torqueScale;	\
		localRange = ((FWInteractPointerDesc*)ptr)->localRange;	\
		cameraOri = ((FWInteractPointerDesc*)ptr)->cameraOri;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWInteractPointerDesc*)ptr)->pointerSolid = pointerSolid;	\
		((FWInteractPointerDesc*)ptr)->humanInterface = humanInterface;	\
		((FWInteractPointerDesc*)ptr)->springK = springK;	\
		((FWInteractPointerDesc*)ptr)->damperD = damperD;	\
		((FWInteractPointerDesc*)ptr)->springOriK = springOriK;	\
		((FWInteractPointerDesc*)ptr)->damperOriD = damperOriD;	\
		((FWInteractPointerDesc*)ptr)->defaultPosition = defaultPosition;	\
		((FWInteractPointerDesc*)ptr)->pointersCalibPosition = pointersCalibPosition;	\
		((FWInteractPointerDesc*)ptr)->posScale = posScale;	\
		((FWInteractPointerDesc*)ptr)->worldScale = worldScale;	\
		((FWInteractPointerDesc*)ptr)->forceScale = forceScale;	\
		((FWInteractPointerDesc*)ptr)->torqueScale = torqueScale;	\
		((FWInteractPointerDesc*)ptr)->localRange = localRange;	\
		((FWInteractPointerDesc*)ptr)->cameraOri = cameraOri;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWInteractSceneDesc \
protected:\
	Spr::FWSceneIf*	fwScene;	\
	Spr::FWInteractMode	iaMode;	\
	Spr::FWHapticMode	hMode;	\
	double	hdt;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		fwScene = ((FWInteractSceneDesc*)ptr)->fwScene;	\
		iaMode = ((FWInteractSceneDesc*)ptr)->iaMode;	\
		hMode = ((FWInteractSceneDesc*)ptr)->hMode;	\
		hdt = ((FWInteractSceneDesc*)ptr)->hdt;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWInteractSceneDesc*)ptr)->fwScene = fwScene;	\
		((FWInteractSceneDesc*)ptr)->iaMode = iaMode;	\
		((FWInteractSceneDesc*)ptr)->hMode = hMode;	\
		((FWInteractSceneDesc*)ptr)->hdt = hdt;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWObjectDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWBoneObjectDesc \
protected:\
	bool	modifyShapeLength;	\
	bool	modifyShapeWidth;	\
	bool	modifyShapePose;	\
	bool	modifyPlugSocketPose;	\
	enum Spr::FWBoneObjectDesc::FWBoneObjectAdaptType	AdaptType;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		modifyShapeLength = ((FWBoneObjectDesc*)ptr)->modifyShapeLength;	\
		modifyShapeWidth = ((FWBoneObjectDesc*)ptr)->modifyShapeWidth;	\
		modifyShapePose = ((FWBoneObjectDesc*)ptr)->modifyShapePose;	\
		modifyPlugSocketPose = ((FWBoneObjectDesc*)ptr)->modifyPlugSocketPose;	\
		AdaptType = ((FWBoneObjectDesc*)ptr)->AdaptType;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWBoneObjectDesc*)ptr)->modifyShapeLength = modifyShapeLength;	\
		((FWBoneObjectDesc*)ptr)->modifyShapeWidth = modifyShapeWidth;	\
		((FWBoneObjectDesc*)ptr)->modifyShapePose = modifyShapePose;	\
		((FWBoneObjectDesc*)ptr)->modifyPlugSocketPose = modifyPlugSocketPose;	\
		((FWBoneObjectDesc*)ptr)->AdaptType = AdaptType;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWStructureDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWSceneDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWSdkDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		return true;	\
	}\

