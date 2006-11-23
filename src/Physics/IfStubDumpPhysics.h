//	Do not edit. MakeTypeDesc.bat will update this file.

template <class IF, class OBJ>
struct PHEngineIfStub: public SceneObjectIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHConstraintIfStub: public SceneObjectIfStub<IF, OBJ> {
	virtual void Enable(bool bEnable = true){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Enable(bEnable);
	}
	virtual bool IsEnabled(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::IsEnabled();
	}
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetConstraintType();
	}
};

template <class IF, class OBJ>
struct PHContactPointIfStub: public PHConstraintIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHJointIfStub: public PHConstraintIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHJoint1DIfStub: public PHJointIfStub<IF, OBJ> {
	virtual void SetRange(double lower, double upper){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetRange(lower, upper);
	}
	virtual void GetRange(double &  lower, double &  upper){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetRange(lower, upper);
	}
	virtual void SetMotorTorque(double t){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMotorTorque(t);
	}
	virtual double GetMotorTorque(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetMotorTorque();
	}
	virtual void SetDesiredVelocity(double v){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDesiredVelocity(v);
	}
	virtual double GetDesiredVelocity(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetDesiredVelocity();
	}
	virtual void SetSpring(double spring){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetSpring(spring);
	}
	virtual double GetSpring(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetSpring();
	}
	virtual void SetSpringOrigin(double origin){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetSpringOrigin(origin);
	}
	virtual double GetSpringOrigin(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetSpringOrigin();
	}
	virtual void SetDamper(double damper){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDamper(damper);
	}
	virtual double GetDamper(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetDamper();
	}
	virtual double GetPosition(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetPosition();
	}
	virtual double GetVelocity(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetVelocity();
	}
};

template <class IF, class OBJ>
struct PHHingeJointIfStub: public PHJoint1DIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHSliderJointIfStub: public PHJoint1DIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHPathIfStub: public SceneObjectIfStub<IF, OBJ> {
	virtual void AddPoint(double s, const Posed &  pose){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::AddPoint(s, pose);
	}
	virtual void SetLoop(bool bOnOff = true){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetLoop(bOnOff);
	}
	virtual bool IsLoop(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::IsLoop();
	}
};

template <class IF, class OBJ>
struct PHPathJointIfStub: public PHJoint1DIfStub<IF, OBJ> {
	virtual void SetPosition(double q){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetPosition(q);
	}
};

template <class IF, class OBJ>
struct PHBallJointIfStub: public PHConstraintIfStub<IF, OBJ> {
	virtual void SetMaxAngle(double angle){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMaxAngle(angle);
	}
	virtual double GetMaxAngle(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetMaxAngle();
	}
	virtual void SetMotorTorque(const Vec3d &  torque){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMotorTorque(torque);
	}
	virtual Vec3d GetMotorTorque(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetMotorTorque();
	}
	virtual Quaterniond GetPosition(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetPosition();
	}
	virtual Vec3d GetVelocity(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetVelocity();
	}
};

template <class IF, class OBJ>
struct PHUniversalJointIfStub: public PHConstraintIfStub<IF, OBJ> {
	virtual void SetMotorTorque(const Vec2d &  torque){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMotorTorque(torque);
	}
	virtual Vec2d GetMotorTorque(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetMotorTorque();
	}
};

template <class IF, class OBJ>
struct PHSpringIfStub: public PHConstraintIfStub<IF, OBJ> {
	virtual void SetSpring(const Vec3d &  spring){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetSpring(spring);
	}
	virtual Vec3d GetSpring(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetSpring();
	}
	virtual void SetDamper(const Vec3d &  damper){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDamper(damper);
	}
	virtual Vec3d GetDamper(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetDamper();
	}
};

template <class IF, class OBJ>
struct PHTreeNodeIfStub: public SceneObjectIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHRootNodeIfStub: public PHTreeNodeIfStub<IF, OBJ> {
};

template <class IF, class OBJ>
struct PHSceneIfStub: public SceneIfStub<IF, OBJ> {
	virtual PHSdkIf *  GetSdk(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetSdk();
	}
	virtual CDShapeIf *  CreateShape(const CDShapeDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShape(desc);
	}
	virtual PHSolidIf *  CreateSolid(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateSolid();
	}
	virtual PHSolidIf *  CreateSolid(const PHSolidDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateSolid(desc);
	}
	virtual int NSolids(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NSolids();
	}
	virtual PHSolidIf * *  GetSolids(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetSolids();
	}
	virtual void SetContactMode(PHSolidIf *  lhs, PHSolidIf *  rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(lhs, rhs, mode);
	}
	virtual void SetContactMode(PHSolidIf * *  group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(group, length, mode);
	}
	virtual void SetContactMode(PHSolidIf *  solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(solid, mode);
	}
	virtual void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(mode);
	}
	virtual int GetNumIteration(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetNumIteration();
	}
	virtual void SetNumIteration(int n){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetNumIteration(n);
	}
	virtual PHJointIf *  CreateJoint(PHSolidIf *  lhs, PHSolidIf *  rhs, const PHJointDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateJoint(lhs, rhs, desc);
	}
	virtual PHTreeNodeIf *  CreateRootNode(PHSolidIf *  root){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateRootNode(root);
	}
	virtual PHTreeNodeIf *  CreateTreeNode(PHTreeNodeIf *  parent, PHSolidIf *  child){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateTreeNode(parent, child);
	}
	virtual double GetTimeStep() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetTimeStep();
	}
	virtual void SetTimeStep(double dt){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetTimeStep(dt);
	}
	virtual unsigned GetCount() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetCount();
	}
	virtual void SetCount(unsigned count){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetCount(count);
	}
	virtual void Step(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Step();
	}
	virtual void ClearForce(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::ClearForce();
	}
	virtual void GenerateForce(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GenerateForce();
	}
	virtual void Integrate(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Integrate();
	}
	virtual void Clear(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::Clear();
	}
	virtual void SetGravity(Vec3f accel){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetGravity(accel);
	}
	virtual Vec3f GetGravity(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetGravity();
	}
	virtual PHConstraintEngine *  GetConstraintEngine(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetConstraintEngine();
	}
};

template <class IF, class OBJ>
struct PHSdkIfStub: public NameManagerIfStub<IF, OBJ> {
	virtual PHSceneIf *  CreateScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene();
	}
	virtual PHSceneIf *  CreateScene(const PHSceneDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene(desc);
	}
	virtual int NScene(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NScene();
	}
	virtual PHSceneIf *  GetScene(size_t i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene(i);
	}
	virtual CDShapeIf *  CreateShape(const CDShapeDesc &  desc){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShape(desc);
	}
	virtual int NShape(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NShape();
	}
	virtual CDShapeIf *  GetShape(int i){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetShape(i);
	}
};

template <class IF, class OBJ>
struct PHSolidIfStub: public SceneObjectIfStub<IF, OBJ> {
	virtual void AddForce(Vec3d f){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::AddForce(f);
	}
	virtual void AddTorque(Vec3d t){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::AddTorque(t);
	}
	virtual void AddForce(Vec3d f, Vec3d r){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::AddForce(f, r);
	}
	virtual double GetMass(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetMass();
	}
	virtual double GetMassInv(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetMassInv();
	}
	virtual void SetMass(double m){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetMass(m);
	}
	virtual Vec3d GetCenterOfMass(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetCenterOfMass();
	}
	virtual void SetCenterOfMass(const Vec3d &  center){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetCenterOfMass(center);
	}
	virtual Matrix3d GetInertia(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetInertia();
	}
	virtual Matrix3d GetInertiaInv(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetInertiaInv();
	}
	virtual void SetInertia(const Matrix3d &  I){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetInertia(I);
	}
	virtual Vec3d GetFramePosition() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetFramePosition();
	}
	virtual void SetFramePosition(const Vec3d &  p){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetFramePosition(p);
	}
	virtual Vec3d GetCenterPosition() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetCenterPosition();
	}
	virtual void SetCenterPosition(const Vec3d &  p){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetCenterPosition(p);
	}
	virtual Quaterniond GetOrientation() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetOrientation();
	}
	virtual void SetOrientation(const Quaterniond &  q){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetOrientation(q);
	}
	virtual Posed GetPose() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetPose();
	}
	virtual void SetPose(const Posed &  p){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetPose(p);
	}
	virtual Vec3d GetVelocity() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetVelocity();
	}
	virtual void SetVelocity(const Vec3d &  v){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetVelocity(v);
	}
	virtual Vec3d GetAngularVelocity() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetAngularVelocity();
	}
	virtual void SetAngularVelocity(const Vec3d &  av){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetAngularVelocity(av);
	}
	virtual Vec3d GetForce() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetForce();
	}
	virtual Vec3d GetTorque() const {
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetTorque();
	}
	virtual void AddShape(CDShapeIf *  shape){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::AddShape(shape);
	}
	virtual int NShape(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::NShape();
	}
	virtual CDShapeIf *	 GetShape(int index){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetShape(index);
	}
	virtual Posed GetShapePose(int index){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::GetShapePose(index);
	}
	virtual void SetShapePose(int index, const Posed &  pose){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetShapePose(index, pose);
	}
	virtual void SetGravity(bool bOn){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetGravity(bOn);
	}
	virtual void SetDynamical(bool bOn){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::SetDynamical(bOn);
	}
	virtual bool IsDynamical(){
		const int offset = (unsigned)((ObjectIfBuf*)(Object*)0x1000) - 0x1000;
		return ((OBJ*)((char*)this-offset))->OBJ::IsDynamical();
	}
};
