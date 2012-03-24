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

#define SPR_DECLMEMBEROF_FWFemMeshDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWObject::SetDesc((FWObjectDesc*)(FWFemMeshDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWObject::GetDesc((FWObjectDesc*)(FWFemMeshDesc*)ptr);	\
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

#define SPR_DECLMEMBEROF_FWWinBaseDesc \
protected:\
	int	width;	\
	int	height;	\
	int	left;	\
	int	top;	\
	UTString	title;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		width = ((FWWinBaseDesc*)ptr)->width;	\
		height = ((FWWinBaseDesc*)ptr)->height;	\
		left = ((FWWinBaseDesc*)ptr)->left;	\
		top = ((FWWinBaseDesc*)ptr)->top;	\
		title = ((FWWinBaseDesc*)ptr)->title;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWWinBaseDesc*)ptr)->width = width;	\
		((FWWinBaseDesc*)ptr)->height = height;	\
		((FWWinBaseDesc*)ptr)->left = left;	\
		((FWWinBaseDesc*)ptr)->top = top;	\
		((FWWinBaseDesc*)ptr)->title = title;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWControlDesc \
protected:\
	UTString	label;	\
	int	align;	\
	int	style;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		label = ((FWControlDesc*)ptr)->label;	\
		align = ((FWControlDesc*)ptr)->align;	\
		style = ((FWControlDesc*)ptr)->style;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWControlDesc*)ptr)->label = label;	\
		((FWControlDesc*)ptr)->align = align;	\
		((FWControlDesc*)ptr)->style = style;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWPanelDesc \
protected:\
	bool	open;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWPanelDesc*)ptr);	\
		open = ((FWPanelDesc*)ptr)->open;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWPanelDesc*)ptr);	\
		((FWPanelDesc*)ptr)->open = open;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWButtonDesc \
protected:\
	int	style;	\
	bool	checked;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWButtonDesc*)ptr);	\
		style = ((FWButtonDesc*)ptr)->style;	\
		checked = ((FWButtonDesc*)ptr)->checked;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWButtonDesc*)ptr);	\
		((FWButtonDesc*)ptr)->style = style;	\
		((FWButtonDesc*)ptr)->checked = checked;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWStaticTextDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWStaticTextDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWStaticTextDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWTextBoxDesc \
protected:\
	UTString	text;	\
	int	intMin;	\
	int	intMax;	\
	float	floatMin;	\
	float	floatMax;	\
	bool	spinner;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWTextBoxDesc*)ptr);	\
		text = ((FWTextBoxDesc*)ptr)->text;	\
		intMin = ((FWTextBoxDesc*)ptr)->intMin;	\
		intMax = ((FWTextBoxDesc*)ptr)->intMax;	\
		floatMin = ((FWTextBoxDesc*)ptr)->floatMin;	\
		floatMax = ((FWTextBoxDesc*)ptr)->floatMax;	\
		spinner = ((FWTextBoxDesc*)ptr)->spinner;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWTextBoxDesc*)ptr);	\
		((FWTextBoxDesc*)ptr)->text = text;	\
		((FWTextBoxDesc*)ptr)->intMin = intMin;	\
		((FWTextBoxDesc*)ptr)->intMax = intMax;	\
		((FWTextBoxDesc*)ptr)->floatMin = floatMin;	\
		((FWTextBoxDesc*)ptr)->floatMax = floatMax;	\
		((FWTextBoxDesc*)ptr)->spinner = spinner;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWListBoxDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWListBoxDesc*)ptr);	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWListBoxDesc*)ptr);	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWRotationControlDesc \
protected:\
	Matrix3f	rot;	\
	float	damping;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWRotationControlDesc*)ptr);	\
		rot = ((FWRotationControlDesc*)ptr)->rot;	\
		damping = ((FWRotationControlDesc*)ptr)->damping;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWRotationControlDesc*)ptr);	\
		((FWRotationControlDesc*)ptr)->rot = rot;	\
		((FWRotationControlDesc*)ptr)->damping = damping;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWTranslationControlDesc \
protected:\
	Vec3f	pos;	\
	float	speed;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWControl::SetDesc((FWControlDesc*)(FWTranslationControlDesc*)ptr);	\
		pos = ((FWTranslationControlDesc*)ptr)->pos;	\
		speed = ((FWTranslationControlDesc*)ptr)->speed;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWControl::GetDesc((FWControlDesc*)(FWTranslationControlDesc*)ptr);	\
		((FWTranslationControlDesc*)ptr)->pos = pos;	\
		((FWTranslationControlDesc*)ptr)->speed = speed;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWDialogDesc \
protected:\
	bool	dock;	\
	int	dockPos;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWWinBase::SetDesc((FWWinBaseDesc*)(FWDialogDesc*)ptr);	\
		dock = ((FWDialogDesc*)ptr)->dock;	\
		dockPos = ((FWDialogDesc*)ptr)->dockPos;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWWinBase::GetDesc((FWWinBaseDesc*)(FWDialogDesc*)ptr);	\
		((FWDialogDesc*)ptr)->dock = dock;	\
		((FWDialogDesc*)ptr)->dockPos = dockPos;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWWinDesc \
protected:\
	bool	debugMode;	\
	bool	fullscreen;	\
	bool	useKeyMouse;	\
	bool	useJoyStick;	\
	int	joyStickPollInterval;	\
	bool	useTrackball;	\
	bool	useDragger;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		FWWinBase::SetDesc((FWWinBaseDesc*)(FWWinDesc*)ptr);	\
		debugMode = ((FWWinDesc*)ptr)->debugMode;	\
		fullscreen = ((FWWinDesc*)ptr)->fullscreen;	\
		useKeyMouse = ((FWWinDesc*)ptr)->useKeyMouse;	\
		useJoyStick = ((FWWinDesc*)ptr)->useJoyStick;	\
		joyStickPollInterval = ((FWWinDesc*)ptr)->joyStickPollInterval;	\
		useTrackball = ((FWWinDesc*)ptr)->useTrackball;	\
		useDragger = ((FWWinDesc*)ptr)->useDragger;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWWinBase::GetDesc((FWWinBaseDesc*)(FWWinDesc*)ptr);	\
		((FWWinDesc*)ptr)->debugMode = debugMode;	\
		((FWWinDesc*)ptr)->fullscreen = fullscreen;	\
		((FWWinDesc*)ptr)->useKeyMouse = useKeyMouse;	\
		((FWWinDesc*)ptr)->useJoyStick = useJoyStick;	\
		((FWWinDesc*)ptr)->joyStickPollInterval = joyStickPollInterval;	\
		((FWWinDesc*)ptr)->useTrackball = useTrackball;	\
		((FWWinDesc*)ptr)->useDragger = useDragger;	\
		return true;	\
	}\

