#define SPR_OVERRIDEMEMBERFUNCOF_ObjectIf(base)	\
	void Print(std::ostream& os)const{	base::Print(os);}	\
	void PrintShort(std::ostream& os)const{	base::PrintShort(os);}	\
	int AddRef(){	return	base::AddRef();}	\
	int RefCount(){	return	base::RefCount();}	\
	size_t NChildObject()const{	return	base::NChildObject();}	\
	Spr::ObjectIf* GetChildObject(size_t pos){	return	base::GetChildObject(pos);}	\
	const Spr::ObjectIf* GetChildObject(size_t pos)const{	return	base::GetChildObject(pos);}	\
	bool AddChildObject(Spr::ObjectIf* o){	return	base::AddChildObject(o);}	\
	bool DelChildObject(Spr::ObjectIf* o){	return	base::DelChildObject(o);}	\
	void Clear(){	base::Clear();}	\
	Spr::ObjectIf* CreateObject(const IfInfo* info, const void* desc){	return	base::CreateObject(info, desc);}	\
	const void* GetDescAddress()const{	return	base::GetDescAddress();}	\
	bool GetDesc(void* desc)const{	return	base::GetDesc(desc);}	\
	void SetDesc(const void* desc){	base::SetDesc(desc);}	\
	size_t GetDescSize()const{	return	base::GetDescSize();}	\
	const void* GetStateAddress()const{	return	base::GetStateAddress();}	\
	bool GetState(void* state)const{	return	base::GetState(state);}	\
	void SetState(const void* state){	base::SetState(state);}	\
	size_t GetStateSize()const{	return	base::GetStateSize();}	\
	void ConstructState(void* m)const{	base::ConstructState(m);}	\
	void DestructState(void* m)const{	base::DestructState(m);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_NamedObjectIf(base)	\
	const char* GetName()const{	return	base::GetName();}	\
	void SetName(const char* n){	base::SetName(n);}	\
	Spr::NameManagerIf* GetNameManager(){	return	base::GetNameManager();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_SceneObjectIf(base)	\
	Spr::SceneIf* GetScene(){	return	base::GetScene();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_ObjectStatesIf(base)	\
	void AllocateState(Spr::ObjectIf* o){	base::AllocateState(o);}	\
	void ReleaseState(Spr::ObjectIf* o){	base::ReleaseState(o);}	\
	size_t CalcStateSize(Spr::ObjectIf* o){	return	base::CalcStateSize(o);}	\
	void SaveState(Spr::ObjectIf* o){	base::SaveState(o);}	\
	void LoadState(Spr::ObjectIf* o){	base::LoadState(o);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_NameManagerIf(base)	\
	Spr::NamedObjectIf* FindObject(UTString name, UTString cls){	return	base::FindObject(name, cls);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_SceneIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_SdkIf(base)	\

