#define SPR_DECLMEMBEROF_FWFemMeshDesc \
protected:\
	unsigned int	kogePics;	\
	unsigned int	thermoPics;	\
	unsigned int	waterPics;	\
	std::string	meshRoughness;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		FWObject::SetDesc((FWObjectDesc*)(FWFemMeshDesc*)ptr);	\
		kogePics = ((FWFemMeshDesc*)ptr)->kogePics;	\
		thermoPics = ((FWFemMeshDesc*)ptr)->thermoPics;	\
		waterPics = ((FWFemMeshDesc*)ptr)->waterPics;	\
		meshRoughness = ((FWFemMeshDesc*)ptr)->meshRoughness;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWObject::GetDesc((FWObjectDesc*)(FWFemMeshDesc*)ptr);	\
		((FWFemMeshDesc*)ptr)->kogePics = kogePics;	\
		((FWFemMeshDesc*)ptr)->thermoPics = thermoPics;	\
		((FWFemMeshDesc*)ptr)->waterPics = waterPics;	\
		((FWFemMeshDesc*)ptr)->meshRoughness = meshRoughness;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWFemMeshNewDesc \
protected:\
	unsigned int	kogetex;	\
	std::string	meshRoughness;	\
	bool	spheric;	\
	unsigned int	texture_mode;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		FWObject::SetDesc((FWObjectDesc*)(FWFemMeshNewDesc*)ptr);	\
		kogetex = ((FWFemMeshNewDesc*)ptr)->kogetex;	\
		meshRoughness = ((FWFemMeshNewDesc*)ptr)->meshRoughness;	\
		spheric = ((FWFemMeshNewDesc*)ptr)->spheric;	\
		texture_mode = ((FWFemMeshNewDesc*)ptr)->texture_mode;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		FWObject::GetDesc((FWObjectDesc*)(FWFemMeshNewDesc*)ptr);	\
		((FWFemMeshNewDesc*)ptr)->kogetex = kogetex;	\
		((FWFemMeshNewDesc*)ptr)->meshRoughness = meshRoughness;	\
		((FWFemMeshNewDesc*)ptr)->spheric = spheric;	\
		((FWFemMeshNewDesc*)ptr)->texture_mode = texture_mode;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWObjectDesc \
protected:\
	bool	bAbsolute;	\
	enum Spr::FWObjectDesc::FWObjectSyncSource	syncSource;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		bAbsolute = ((FWObjectDesc*)ptr)->bAbsolute;	\
		syncSource = ((FWObjectDesc*)ptr)->syncSource;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWObjectDesc*)ptr)->bAbsolute = bAbsolute;	\
		((FWObjectDesc*)ptr)->syncSource = syncSource;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWSceneDesc \
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

#define SPR_DECLMEMBEROF_FWSdkDesc \
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

#define SPR_DECLMEMBEROF_FWSkeletonSensorDesc \
protected:\
	bool	bCreatePHSkeleton;	\
	bool	bCreatePHSolid;	\
	bool	bCreateCDShape;	\
	bool	bCreatePHSpring;	\
	Vec2d	radius;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		bCreatePHSkeleton = ((FWSkeletonSensorDesc*)ptr)->bCreatePHSkeleton;	\
		bCreatePHSolid = ((FWSkeletonSensorDesc*)ptr)->bCreatePHSolid;	\
		bCreateCDShape = ((FWSkeletonSensorDesc*)ptr)->bCreateCDShape;	\
		bCreatePHSpring = ((FWSkeletonSensorDesc*)ptr)->bCreatePHSpring;	\
		radius = ((FWSkeletonSensorDesc*)ptr)->radius;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((FWSkeletonSensorDesc*)ptr)->bCreatePHSkeleton = bCreatePHSkeleton;	\
		((FWSkeletonSensorDesc*)ptr)->bCreatePHSolid = bCreatePHSolid;	\
		((FWSkeletonSensorDesc*)ptr)->bCreateCDShape = bCreateCDShape;	\
		((FWSkeletonSensorDesc*)ptr)->bCreatePHSpring = bCreatePHSpring;	\
		((FWSkeletonSensorDesc*)ptr)->radius = radius;	\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
	virtual const void* GetDescAddress() const { return NULL; }\
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
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		FWWinBase::SetDesc((FWWinBaseDesc*)(FWWinDesc*)ptr);	\
		debugMode = ((FWWinDesc*)ptr)->debugMode;	\
		fullscreen = ((FWWinDesc*)ptr)->fullscreen;	\
		useKeyMouse = ((FWWinDesc*)ptr)->useKeyMouse;	\
		useJoyStick = ((FWWinDesc*)ptr)->useJoyStick;	\
		joyStickPollInterval = ((FWWinDesc*)ptr)->joyStickPollInterval;	\
		useTrackball = ((FWWinDesc*)ptr)->useTrackball;	\
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
		return true;	\
	}\

