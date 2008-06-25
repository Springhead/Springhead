#define SPR_OVERRIDEMEMBERFUNCOF_FIFileIf(base)	\
	bool Load(Spr::ObjectIfs& objs, const char* fn){	return	base::Load(objs, fn);}	\
	bool Save(const Spr::ObjectIfs& objs, const char* fn){	return	base::Save(objs, fn);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FIFileXIf(base)	\
	bool Load(Spr::ObjectIfs& objs, const char* fn){	return	base::Load(objs, fn);}	\
	bool Save(const Spr::ObjectIfs& objs, const char* fn){	return	base::Save(objs, fn);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FIFileVRMLIf(base)	\
	bool Load(Spr::ObjectIfs& objs, const char* fn){	return	base::Load(objs, fn);}	\
	bool Save(const Spr::ObjectIfs& objs, const char* fn){	return	base::Save(objs, fn);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FIFileCOLLADAIf(base)	\
	bool Load(Spr::ObjectIfs& objs, const char* fn){	return	base::Load(objs, fn);}	\
	bool Save(const Spr::ObjectIfs& objs, const char* fn){	return	base::Save(objs, fn);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FISdkIf(base)	\
	Spr::FIFileXIf* CreateFileX(){	return	base::CreateFileX();}	\
	Spr::FIFileVRMLIf* CreateFileVRML(){	return	base::CreateFileVRML();}	\
	Spr::FIFileCOLLADAIf* CreateFileCOLLADA(){	return	base::CreateFileCOLLADA();}	\

