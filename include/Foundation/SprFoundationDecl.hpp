#define SPR_OVERRIDEMEMBERFUNCOF_ObjectIf(base)	\
	void Print(std::ostream& os)const{	return	base::Print(os);}	\
	void PrintShort(std::ostream& os)const{	return	base::PrintShort(os);}	\
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
	void SetDesc(const void* desc){	return	base::SetDesc(desc);}	\
	size_t GetDescSize()const{	return	base::GetDescSize();}	\
	const void* GetStateAddress()const{	return	base::GetStateAddress();}	\
	bool GetState(void* state)const{	return	base::GetState(state);}	\
	void SetState(const void* state){	return	base::SetState(state);}	\
	bool WriteStateR(std::ostream& fout){	return	base::WriteStateR(fout);}	\
	bool WriteState(std::string fileName){	return	base::WriteState(fileName);}	\
	bool ReadStateR(std::istream& fin){	return	base::ReadStateR(fin);}	\
	bool ReadState(std::string fileName){	return	base::ReadState(fileName);}	\
	size_t GetStateSize()const{	return	base::GetStateSize();}	\
	void ConstructState(void* m)const{	return	base::ConstructState(m);}	\
	void DestructState(void* m)const{	return	base::DestructState(m);}	\
	void DumpObjectR(std::ostream& os, int level)const{	return	base::DumpObjectR(os, level);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_NamedObjectIf(base)	\
	const char* GetName()const{	return	base::GetName();}	\
	void SetName(const char* n){	return	base::SetName(n);}	\
	Spr::NameManagerIf* GetNameManager(){	return	base::GetNameManager();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_SceneObjectIf(base)	\
	Spr::SceneIf* GetScene(){	return	base::GetScene();}	\
	Spr::SceneObjectIf* CloneObject(){	return	base::CloneObject();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_ObjectStatesIf(base)	\
	void AllocateState(Spr::ObjectIf* o){	return	base::AllocateState(o);}	\
	void ReleaseState(Spr::ObjectIf* o){	return	base::ReleaseState(o);}	\
	size_t CalcStateSize(Spr::ObjectIf* o){	return	base::CalcStateSize(o);}	\
	void SaveState(Spr::ObjectIf* o){	return	base::SaveState(o);}	\
	void LoadState(Spr::ObjectIf* o){	return	base::LoadState(o);}	\
	void SingleSave(Spr::ObjectIf* o){	return	base::SingleSave(o);}	\
	void SingleLoad(Spr::ObjectIf* o){	return	base::SingleLoad(o);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_NameManagerIf(base)	\
	Spr::NamedObjectIf* FindObject(UTString name, UTString cls){	return	base::FindObject(name, cls);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_SceneIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_SdkIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_UTTimerIf(base)	\
	unsigned int GetID(){	return	base::GetID();}	\
	unsigned int GetResolution(){	return	base::GetResolution();}	\
	bool SetResolution(unsigned int r){	return	base::SetResolution(r);}	\
	unsigned int GetInterval(){	return	base::GetInterval();}	\
	bool SetInterval(unsigned int i){	return	base::SetInterval(i);}	\
	bool SetCallback(Spr::UTTimerIf::TimerFunc f, void* arg){	return	base::SetCallback(f, arg);}	\
	Spr::UTTimerIf::TimerFunc GetCallback(){	return	base::GetCallback();}	\
	void* GetCallbackArg(){	return	base::GetCallbackArg();}	\
	Spr::UTTimerIf::Mode GetMode(){	return	base::GetMode();}	\
	bool SetMode(Spr::UTTimerIf::Mode m){	return	base::SetMode(m);}	\
	bool IsStarted(){	return	base::IsStarted();}	\
	bool IsRunning(){	return	base::IsRunning();}	\
	bool Start(){	return	base::Start();}	\
	bool Stop(){	return	base::Stop();}	\
	void Call(){	base::Call();}	\

