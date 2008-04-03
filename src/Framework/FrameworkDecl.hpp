#define SPR_DECLMEMBEROF_FWObjectDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
	}\
	virtual bool GetDesc(const void* ptr){ \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWSceneDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
	}\
	virtual bool GetDesc(const void* ptr){ \
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FWSdkDesc \
protected:\
public:\
	virtual void SetDesc(const void* ptr){ \
	}\
	virtual bool GetDesc(const void* ptr){ \
		return true;	\
	}\

