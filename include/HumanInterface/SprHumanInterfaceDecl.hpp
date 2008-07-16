#define SPR_OVERRIDEMEMBERFUNCOF_HIBaseIf(base)	\
	bool BeforeCalibration(){	return	base::BeforeCalibration();}	\
	bool Calibration(){	return	base::Calibration();}	\
	bool AfterCalibration(){	return	base::AfterCalibration();}	\
	void AddDeviceDependency(HIRealDeviceIf* rd){	base::AddDeviceDependency(rd);}	\
	void ClearDeviceDependency(){	base::ClearDeviceDependency();}	\
	void Update(float dt){	base::Update(dt);}	\
	bool IsGood(){	return	base::IsGood();}	\
	bool Init(const void* desc){	return	base::Init(desc);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIRealDeviceIf(base)	\
	const char* Name(){	return	base::Name();}	\
	bool Init(){	return	base::Init();}	\
	void Register(Spr::HISdkIf* sdk){	base::Register(sdk);}	\
	void Update(){	base::Update();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIVirtualDeviceIf(base)	\
	const char* Name()const{	return	base::Name();}	\
	const char* Type()const{	return	base::Type();}	\
	Spr::HIRealDeviceIf* RealDevice(){	return	base::RealDevice();}	\
	void Print(std::ostream& o)const{	base::Print(o);}	\
	void Update(){	base::Update();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_DVAdBaseIf(base)	\
	int Digit(){	return	base::Digit();}	\
	float Voltage(){	return	base::Voltage();}	\
	const char* Name()const{	return	base::Name();}	\
	const char* Type()const{	return	base::Type();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_DVDaBaseIf(base)	\
	void Digit(int d){	base::Digit(d);}	\
	void Voltage(float volt){	base::Voltage(volt);}	\
	const char* Name()const{	return	base::Name();}	\
	const char* Type(){	return	base::Type();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_DVCounterBaseIf(base)	\
	void Count(long count){	base::Count(count);}	\
	long Count(){	return	base::Count();}	\
	const char* Name()const{	return	base::Name();}	\
	const char* Type()const{	return	base::Type();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_DVPioBaseIf(base)	\
	int Get(){	return	base::Get();}	\
	void Set(int l){	base::Set(l);}	\
	const char* Name()const{	return	base::Name();}	\
	const char* Type()const{	return	base::Type();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_DRUsb20SimpleIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_DRUsb20Sh4If(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_DVKeyMouseIf(base)	\
	const char* Name()const{	return	base::Name();}	\
	void SetHandler(Spr::DVKeyMouseHandler* h){	base::SetHandler(h);}	\
	int GetKeyState(int key){	return	base::GetKeyState(key);}	\
	Spr::DVKeyMouseIf::DVMousePosition GetMousePosition(int count){	return	base::GetMousePosition(count);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_DRKeyMouseWin32If(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_DRKeyMouseGLUTIf(base)	\

#define SPR_OVERRIDEMEMBERFUNCOF_HISdkIf(base)	\
	void Init(){	base::Init();}	\
	void Clear(){	base::Clear();}	\
	Spr::HIVirtualDeviceIf* RentVirtualDevice(const char* type, const char* name){	return	base::RentVirtualDevice(type, name);}	\
	bool ReturnVirtualDevice(Spr::HIVirtualDeviceIf* dev){	return	base::ReturnVirtualDevice(dev);}	\
	bool AddRealDevice(const IfInfo* keyInfo, const void* desc){	return	base::AddRealDevice(keyInfo, desc);}	\
	Spr::HIRealDeviceIf* FindRealDevice(const char* name){	return	base::FindRealDevice(name);}	\
	UTRef< Spr::HIBaseIf > CreateHumanInterface(const IfInfo* info){	return	base::CreateHumanInterface(info);}	\
	UTRef< Spr::HIBaseIf > CreateHumanInterface(const char* name){	return	base::CreateHumanInterface(name);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HISpidarMotorIf(base)	\
	void SetForce(float f){	base::SetForce(f);}	\
	float GetForce(){	return	base::GetForce();}	\
	float GetCurrent(){	return	base::GetCurrent();}	\
	void SetLength(float l){	base::SetLength(l);}	\
	float GetLength(){	return	base::GetLength();}	\
	float GetVelocity(){	return	base::GetVelocity();}	\
	void GetCalibrationData(float dt){	base::GetCalibrationData(dt);}	\
	void Calibrate(bool bUpdate){	base::Calibrate(bUpdate);}	\
	void GetVdd(){	base::GetVdd();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIPositionIf(base)	\
	Vec3f GetPosition(){	return	base::GetPosition();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIOrientationIf(base)	\
	Quaternionf GetOrientation(){	return	base::GetOrientation();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIPoseIf(base)	\
	Quaternionf GetOrientation(){	return	base::GetOrientation();}	\
	Posef GetPose(){	return	base::GetPose();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIForceInterface6DIf(base)	\
	Vec3f GetTorque(){	return	base::GetTorque();}	\
	Vec3f GetForce(){	return	base::GetForce();}	\
	void SetForce(const Vec3f& f, const Vec3f& t){	base::SetForce(f, t);}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HIMouse6DIf(base)	\
	Spr::DVKeyMouseIf* GetKeyMouse(){	return	base::GetKeyMouse();}	\

#define SPR_OVERRIDEMEMBERFUNCOF_HISpidarGIf(base)	\
	Vec3f GetTorque(){	return	base::GetTorque();}	\
	Vec3f GetForce(){	return	base::GetForce();}	\
	void SetForce(const Vec3f& f, const Vec3f& t){	base::SetForce(f, t);}	\
	Spr::HISpidarMotorIf* GetMotor(size_t i){	return	base::GetMotor(i);}	\
	size_t NMotor()const{	return	base::NMotor();}	\

