#define SPR_DECLMEMBEROF_ImportDesc \
protected:\
	std::string	path;	\
public:\
	virtual const void* GetDescAddress() const { return NULL; }\
	virtual void SetDesc(const void* ptr){ \
		path = ((ImportDesc*)ptr)->path;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((ImportDesc*)ptr)->path = path;	\
		return true;	\
	}\

#define SPR_DECLMEMBEROF_FIFileSprDesc \
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

#define SPR_DECLMEMBEROF_FIFileXDesc \
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

#define SPR_DECLMEMBEROF_FIFileVRMLDesc \
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

#define SPR_DECLMEMBEROF_FIFileCOLLADADesc \
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

#define SPR_DECLMEMBEROF_FIFileBinaryDesc \
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

