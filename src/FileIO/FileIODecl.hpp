#define SPR_DECLMEMBEROF_ImportDesc \
protected:\
	std::string	path;	\
public:\
	virtual void SetDesc(const void* ptr){ \
		path = ((ImportDesc*)ptr)->path;	\
		AfterSetDesc();	\
	}\
	virtual bool GetDesc(void* ptr) const { \
		BeforeGetDesc();	\
		((ImportDesc*)ptr)->path = path;	\
		return true;	\
	}\

