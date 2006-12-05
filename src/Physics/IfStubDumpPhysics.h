//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class OBJ>
struct PHEngineIfStubTemplate: public SceneObjectIfStubTemplate<IF, OBJ> {
};
struct PHEngineIf;	class PHEngine;
typedef PHEngineIfStubTemplate<PHEngineIf, PHEngine>	PHEngineIfStub;
typedef IfInitTemplate<PHEngineIfStub, PHEngine>	PHEngineIfInit;

template <class IF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class OBJ>
struct PHConstraintIfStubTemplate: public SceneObjectIfStubTemplate<IF, OBJ> {
	virtual void Enable(bool bEnable = true){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Enable(bEnable);
	}
	virtual bool IsEnabled(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::IsEnabled();
	}
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetConstraintType();
	}
	virtual void GetRelativePose(Posed &  p){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetRelativePose(p);
	}
	virtual void GetRelativeVelocity(Vec3d &  v, Vec3d &  w){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetRelativeVelocity(v, w);
	}
	virtual void GetConstraintForce(Vec3d &  f, Vec3d &  t){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetConstraintForce(f, t);
	}
};
struct PHConstraintIf;	class PHConstraint;
typedef PHConstraintIfStubTemplate<PHConstraintIf, PHConstraint>	PHConstraintIfStub;
typedef IfInitTemplate<PHConstraintIfStub, PHConstraint>	PHConstraintIfInit;

template <class IF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class OBJ>
struct PHContactPointIfStubTemplate: public PHConstraintIfStubTemplate<IF, OBJ> {
};
struct PHContactPointIf;	class PHContactPoint;
typedef PHContactPointIfStubTemplate<PHContactPointIf, PHContactPoint>	PHContactPointIfStub;
typedef IfInitTemplate<PHContactPointIfStub, PHContactPoint>	PHContactPointIfInit;

template <class IF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class OBJ>
struct PHJointIfStubTemplate: public PHConstraintIfStubTemplate<IF, OBJ> {
};
struct PHJointIf;	class PHJoint;
typedef PHJointIfStubTemplate<PHJointIf, PHJoint>	PHJointIfStub;
typedef IfInitTemplate<PHJointIfStub, PHJoint>	PHJointIfInit;

template <class IF, class OBJ> struct PHJointIfStubTemplate;
template <class IF, class OBJ>
struct PHJoint1DIfStubTemplate: public PHJointIfStubTemplate<IF, OBJ> {
	virtual void SetRange(double lower, double upper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetRange(lower, upper);
	}
	virtual void GetRange(double &  lower, double &  upper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetRange(lower, upper);
	}
	virtual void SetMotorTorque(double t){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetMotorTorque(t);
	}
	virtual double GetMotorTorque(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetMotorTorque();
	}
	virtual void SetDesiredVelocity(double v){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDesiredVelocity(v);
	}
	virtual double GetDesiredVelocity(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetDesiredVelocity();
	}
	virtual void SetSpring(double spring){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetSpring(spring);
	}
	virtual double GetSpring(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSpring();
	}
	virtual void SetSpringOrigin(double origin){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetSpringOrigin(origin);
	}
	virtual double GetSpringOrigin(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSpringOrigin();
	}
	virtual void SetDamper(double damper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDamper(damper);
	}
	virtual double GetDamper(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetDamper();
	}
	virtual double GetPosition(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetPosition();
	}
	virtual double GetVelocity(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetVelocity();
	}
};
struct PHJoint1DIf;	class PHJoint1D;
typedef PHJoint1DIfStubTemplate<PHJoint1DIf, PHJoint1D>	PHJoint1DIfStub;
typedef IfInitTemplate<PHJoint1DIfStub, PHJoint1D>	PHJoint1DIfInit;

template <class IF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class OBJ>
struct PHHingeJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, OBJ> {
};
struct PHHingeJointIf;	class PHHingeJoint;
typedef PHHingeJointIfStubTemplate<PHHingeJointIf, PHHingeJoint>	PHHingeJointIfStub;
typedef IfInitTemplate<PHHingeJointIfStub, PHHingeJoint>	PHHingeJointIfInit;

template <class IF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class OBJ>
struct PHSliderJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, OBJ> {
};
struct PHSliderJointIf;	class PHSliderJoint;
typedef PHSliderJointIfStubTemplate<PHSliderJointIf, PHSliderJoint>	PHSliderJointIfStub;
typedef IfInitTemplate<PHSliderJointIfStub, PHSliderJoint>	PHSliderJointIfInit;

template <class IF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class OBJ>
struct PHPathIfStubTemplate: public SceneObjectIfStubTemplate<IF, OBJ> {
	virtual void AddPoint(double s, const Posed &  pose){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddPoint(s, pose);
	}
	virtual void SetLoop(bool bOnOff = true){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetLoop(bOnOff);
	}
	virtual bool IsLoop(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::IsLoop();
	}
};
struct PHPathIf;	class PHPath;
typedef PHPathIfStubTemplate<PHPathIf, PHPath>	PHPathIfStub;
typedef IfInitTemplate<PHPathIfStub, PHPath>	PHPathIfInit;

template <class IF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class OBJ>
struct PHPathJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, OBJ> {
	virtual void SetPosition(double q){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetPosition(q);
	}
};
struct PHPathJointIf;	class PHPathJoint;
typedef PHPathJointIfStubTemplate<PHPathJointIf, PHPathJoint>	PHPathJointIfStub;
typedef IfInitTemplate<PHPathJointIfStub, PHPathJoint>	PHPathJointIfInit;

template <class IF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class OBJ>
struct PHBallJointIfStubTemplate: public PHConstraintIfStubTemplate<IF, OBJ> {
	virtual void SetSwingRange(double upper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetSwingRange(upper);
	}
	virtual double GetSwingRange(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSwingRange();
	}
	virtual void SetTwistRange(double lower, double upper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetTwistRange(lower, upper);
	}
	virtual void GetTwistRange(double &  lower, double &  upper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetTwistRange(lower, upper);
	}
	virtual void SetMotorTorque(const Vec3d &  torque){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetMotorTorque(torque);
	}
	virtual Vec3d GetMotorTorque(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetMotorTorque();
	}
	virtual Vec3d GetAngle(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetAngle();
	}
	virtual Vec3d GetVelocity(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetVelocity();
	}
};
struct PHBallJointIf;	class PHBallJoint;
typedef PHBallJointIfStubTemplate<PHBallJointIf, PHBallJoint>	PHBallJointIfStub;
typedef IfInitTemplate<PHBallJointIfStub, PHBallJoint>	PHBallJointIfInit;

template <class IF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class OBJ>
struct PHSpringIfStubTemplate: public PHConstraintIfStubTemplate<IF, OBJ> {
	virtual void SetSpring(const Vec3d &  spring){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetSpring(spring);
	}
	virtual Vec3d GetSpring(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSpring();
	}
	virtual void SetDamper(const Vec3d &  damper){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDamper(damper);
	}
	virtual Vec3d GetDamper(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetDamper();
	}
};
struct PHSpringIf;	class PHSpring;
typedef PHSpringIfStubTemplate<PHSpringIf, PHSpring>	PHSpringIfStub;
typedef IfInitTemplate<PHSpringIfStub, PHSpring>	PHSpringIfInit;

template <class IF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class OBJ>
struct PHTreeNodeIfStubTemplate: public SceneObjectIfStubTemplate<IF, OBJ> {
};
struct PHTreeNodeIf;	class PHTreeNode;
typedef PHTreeNodeIfStubTemplate<PHTreeNodeIf, PHTreeNode>	PHTreeNodeIfStub;
typedef IfInitTemplate<PHTreeNodeIfStub, PHTreeNode>	PHTreeNodeIfInit;

template <class IF, class OBJ> struct PHTreeNodeIfStubTemplate;
template <class IF, class OBJ>
struct PHRootNodeIfStubTemplate: public PHTreeNodeIfStubTemplate<IF, OBJ> {
};
struct PHRootNodeIf;	class PHRootNode;
typedef PHRootNodeIfStubTemplate<PHRootNodeIf, PHRootNode>	PHRootNodeIfStub;
typedef IfInitTemplate<PHRootNodeIfStub, PHRootNode>	PHRootNodeIfInit;

template <class IF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class OBJ>
struct PHSceneIfStubTemplate: public SceneIfStubTemplate<IF, OBJ> {
	virtual PHSdkIf *  GetSdk(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSdk();
	}
	virtual CDShapeIf *  CreateShape(const CDShapeDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShape(desc);
	}
	virtual PHSolidIf *  CreateSolid(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateSolid();
	}
	virtual PHSolidIf *  CreateSolid(const PHSolidDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateSolid(desc);
	}
	virtual int NSolids(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NSolids();
	}
	virtual PHSolidIf * *  GetSolids(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetSolids();
	}
	virtual void SetContactMode(PHSolidIf *  lhs, PHSolidIf *  rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(lhs, rhs, mode);
	}
	virtual void SetContactMode(PHSolidIf * *  group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(group, length, mode);
	}
	virtual void SetContactMode(PHSolidIf *  solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(solid, mode);
	}
	virtual void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetContactMode(mode);
	}
	virtual int GetNumIteration(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetNumIteration();
	}
	virtual void SetNumIteration(int n){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetNumIteration(n);
	}
	virtual PHJointIf *  CreateJoint(PHSolidIf *  lhs, PHSolidIf *  rhs, const PHJointDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateJoint(lhs, rhs, desc);
	}
	virtual PHTreeNodeIf *  CreateRootNode(PHSolidIf *  root){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateRootNode(root);
	}
	virtual PHTreeNodeIf *  CreateTreeNode(PHTreeNodeIf *  parent, PHSolidIf *  child){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateTreeNode(parent, child);
	}
	virtual double GetTimeStep() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetTimeStep();
	}
	virtual void SetTimeStep(double dt){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetTimeStep(dt);
	}
	virtual unsigned GetCount() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetCount();
	}
	virtual void SetCount(unsigned count){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetCount(count);
	}
	virtual void Step(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Step();
	}
	virtual void ClearForce(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::ClearForce();
	}
	virtual void GenerateForce(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GenerateForce();
	}
	virtual void Integrate(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Integrate();
	}
	virtual void Clear(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::Clear();
	}
	virtual void SetGravity(Vec3f accel){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetGravity(accel);
	}
	virtual Vec3f GetGravity(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetGravity();
	}
	virtual PHConstraintEngine *  GetConstraintEngine(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetConstraintEngine();
	}
};
struct PHSceneIf;	class PHScene;
typedef PHSceneIfStubTemplate<PHSceneIf, PHScene>	PHSceneIfStub;
typedef IfInitTemplate<PHSceneIfStub, PHScene>	PHSceneIfInit;

template <class IF, class OBJ> struct NameManagerIfStubTemplate;
template <class IF, class OBJ>
struct PHSdkIfStubTemplate: public NameManagerIfStubTemplate<IF, OBJ> {
	virtual PHSceneIf *  CreateScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene();
	}
	virtual PHSceneIf *  CreateScene(const PHSceneDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateScene(desc);
	}
	virtual int NScene(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NScene();
	}
	virtual PHSceneIf *  GetScene(size_t i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetScene(i);
	}
	virtual CDShapeIf *  CreateShape(const CDShapeDesc &  desc){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::CreateShape(desc);
	}
	virtual int NShape(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NShape();
	}
	virtual CDShapeIf *  GetShape(int i){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetShape(i);
	}
};
struct PHSdkIf;	class PHSdk;
typedef PHSdkIfStubTemplate<PHSdkIf, PHSdk>	PHSdkIfStub;
typedef IfInitTemplate<PHSdkIfStub, PHSdk>	PHSdkIfInit;

template <class IF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class OBJ>
struct PHSolidIfStubTemplate: public SceneObjectIfStubTemplate<IF, OBJ> {
	virtual void AddForce(Vec3d f){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddForce(f);
	}
	virtual void AddTorque(Vec3d t){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddTorque(t);
	}
	virtual void AddForce(Vec3d f, Vec3d r){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddForce(f, r);
	}
	virtual double GetMass(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetMass();
	}
	virtual double GetMassInv(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetMassInv();
	}
	virtual void SetMass(double m){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetMass(m);
	}
	virtual Vec3d GetCenterOfMass(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetCenterOfMass();
	}
	virtual void SetCenterOfMass(const Vec3d &  center){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetCenterOfMass(center);
	}
	virtual Matrix3d GetInertia(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetInertia();
	}
	virtual Matrix3d GetInertiaInv(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetInertiaInv();
	}
	virtual void SetInertia(const Matrix3d &  I){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetInertia(I);
	}
	virtual Vec3d GetFramePosition() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetFramePosition();
	}
	virtual void SetFramePosition(const Vec3d &  p){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetFramePosition(p);
	}
	virtual Vec3d GetCenterPosition() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetCenterPosition();
	}
	virtual void SetCenterPosition(const Vec3d &  p){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetCenterPosition(p);
	}
	virtual Quaterniond GetOrientation() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetOrientation();
	}
	virtual void SetOrientation(const Quaterniond &  q){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetOrientation(q);
	}
	virtual Posed GetPose() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetPose();
	}
	virtual void SetPose(const Posed &  p){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetPose(p);
	}
	virtual Vec3d GetVelocity() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetVelocity();
	}
	virtual void SetVelocity(const Vec3d &  v){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetVelocity(v);
	}
	virtual Vec3d GetAngularVelocity() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetAngularVelocity();
	}
	virtual void SetAngularVelocity(const Vec3d &  av){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetAngularVelocity(av);
	}
	virtual Vec3d GetForce() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetForce();
	}
	virtual Vec3d GetTorque() const {
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetTorque();
	}
	virtual void AddShape(CDShapeIf *  shape){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::AddShape(shape);
	}
	virtual int NShape(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::NShape();
	}
	virtual CDShapeIf *	 GetShape(int index){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetShape(index);
	}
	virtual Posed GetShapePose(int index){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::GetShapePose(index);
	}
	virtual void SetShapePose(int index, const Posed &  pose){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetShapePose(index, pose);
	}
	virtual void SetGravity(bool bOn){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetGravity(bOn);
	}
	virtual void SetDynamical(bool bOn){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::SetDynamical(bOn);
	}
	virtual bool IsDynamical(){
		const int offset = (int)((char*)(ObjectIfBuf*)(Object*)0x1000 - (char*)0x1000);
		return ((OBJ*)((char*)this-offset))->OBJ::IsDynamical();
	}
};
struct PHSolidIf;	class PHSolid;
typedef PHSolidIfStubTemplate<PHSolidIf, PHSolid>	PHSolidIfStub;
typedef IfInitTemplate<PHSolidIfStub, PHSolid>	PHSolidIfInit;
}	//	namespace Spr; 
