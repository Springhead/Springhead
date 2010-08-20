#define SPR_OVERRIDEMEMBERFUNCOF_FIFileIf(base)	\
	bool Load(Spr::ObjectIfs& objs, const char* fn){	return	base::Load(objs, fn);}	\
	bool Save(const Spr::ObjectIfs& objs, const char* fn){	return	base::Save(objs, fn);}	\
	void SetImport(ImportIf* import){	base::SetImport(import);}	\
	ImportIf* GetImport(){	return	base::GetImport();}	\
	void SetDSTR(bool f){	base::SetDSTR(f);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FIFileSprIf(base)	\
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

#define SPR_OVERRIDEMEMBERFUNCOF_FIFileBinaryIf(base)	\
	bool Load(Spr::ObjectIfs& objs, const char* fn){	return	base::Load(objs, fn);}	\
	bool Save(const Spr::ObjectIfs& objs, const char* fn){	return	base::Save(objs, fn);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_ImportIf(base)	\
	void SetLoadOnly(bool on){	base::SetLoadOnly(on);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_FISdkIf(base)	\
	Spr::FIFileXIf* CreateFileX(){	return	base::CreateFileX();}	\
	Spr::FIFileSprIf* CreateFileSpr(){	return	base::CreateFileSpr();}	\
	Spr::FIFileVRMLIf* CreateFileVRML(){	return	base::CreateFileVRML();}	\
	Spr::FIFileCOLLADAIf* CreateFileCOLLADA(){	return	base::CreateFileCOLLADA();}	\
	Spr::FIFileBinaryIf* CreateFileBinary(){	return	base::CreateFileBinary();}	\
	Spr::FIFileIf* CreateFile(const IfInfo* ii){	return	base::CreateFile(ii);}	\
	Spr::FIFileIf* CreateFileFromExt(UTString filename){	return	base::CreateFileFromExt(filename);}	\
	Spr::ImportIf* CreateImport(){	return	base::CreateImport();}	\
	Spr::ImportIf* CreateImport(Spr::ImportIf* parent, UTString path, Spr::ObjectIf* owner, const Spr::ObjectIfs& children){	return	base::CreateImport(parent, path, owner, children);}	\
	bool DelChildObject(Spr::ObjectIf* o){	return	base::DelChildObject(o);}	\
	void Clear(){	base::Clear();}	\

