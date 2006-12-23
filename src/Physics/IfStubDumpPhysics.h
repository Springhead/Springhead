//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHEngineIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
};
struct PHEngineIf;	class PHEngine;
typedef PHEngineIfStubTemplate<PHEngineIf, ObjectIfBuf, PHEngine>	PHEngineIfStub;
typedef IfInitTemplate<PHEngineIfStub, PHEngine>	PHEngineIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHConstraintIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Enable(bool bEnable = true){
		return ((OBJ*)(MIF*)this)->OBJ::Enable(bEnable);
	}
	virtual bool IsEnabled(){
		return ((OBJ*)(MIF*)this)->OBJ::IsEnabled();
	}
	virtual PHConstraintDesc::ConstraintType GetConstraintType(){
		return ((OBJ*)(MIF*)this)->OBJ::GetConstraintType();
	}
	virtual void GetRelativePose(Posed &  p){
		return ((OBJ*)(MIF*)this)->OBJ::GetRelativePose(p);
	}
	virtual void GetRelativeVelocity(Vec3d &  v, Vec3d &  w){
		return ((OBJ*)(MIF*)this)->OBJ::GetRelativeVelocity(v, w);
	}
	virtual void GetConstraintForce(Vec3d &  f, Vec3d &  t){
		return ((OBJ*)(MIF*)this)->OBJ::GetConstraintForce(f, t);
	}
};
struct PHConstraintIf;	class PHConstraint;
typedef PHConstraintIfStubTemplate<PHConstraintIf, ObjectIfBuf, PHConstraint>	PHConstraintIfStub;
typedef IfInitTemplate<PHConstraintIfStub, PHConstraint>	PHConstraintIfInit;

template <class IF, class MIF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHContactPointIfStubTemplate: public PHConstraintIfStubTemplate<IF, MIF, OBJ> {
};
struct PHContactPointIf;	class PHContactPoint;
typedef PHContactPointIfStubTemplate<PHContactPointIf, ObjectIfBuf, PHContactPoint>	PHContactPointIfStub;
typedef IfInitTemplate<PHContactPointIfStub, PHContactPoint>	PHContactPointIfInit;

template <class IF, class MIF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHJointIfStubTemplate: public PHConstraintIfStubTemplate<IF, MIF, OBJ> {
};
struct PHJointIf;	class PHJoint;
typedef PHJointIfStubTemplate<PHJointIf, ObjectIfBuf, PHJoint>	PHJointIfStub;
typedef IfInitTemplate<PHJointIfStub, PHJoint>	PHJointIfInit;

template <class IF, class MIF, class OBJ> struct PHJointIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHJoint1DIfStubTemplate: public PHJointIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetRange(double lower, double upper){
		return ((OBJ*)(MIF*)this)->OBJ::SetRange(lower, upper);
	}
	virtual void GetRange(double &  lower, double &  upper){
		return ((OBJ*)(MIF*)this)->OBJ::GetRange(lower, upper);
	}
	virtual void SetMotorTorque(double t){
		return ((OBJ*)(MIF*)this)->OBJ::SetMotorTorque(t);
	}
	virtual double GetMotorTorque(){
		return ((OBJ*)(MIF*)this)->OBJ::GetMotorTorque();
	}
	virtual void SetDesiredVelocity(double v){
		return ((OBJ*)(MIF*)this)->OBJ::SetDesiredVelocity(v);
	}
	virtual double GetDesiredVelocity(){
		return ((OBJ*)(MIF*)this)->OBJ::GetDesiredVelocity();
	}
	virtual void SetSpring(double spring){
		return ((OBJ*)(MIF*)this)->OBJ::SetSpring(spring);
	}
	virtual double GetSpring(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSpring();
	}
	virtual void SetSpringOrigin(double origin){
		return ((OBJ*)(MIF*)this)->OBJ::SetSpringOrigin(origin);
	}
	virtual double GetSpringOrigin(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSpringOrigin();
	}
	virtual void SetDamper(double damper){
		return ((OBJ*)(MIF*)this)->OBJ::SetDamper(damper);
	}
	virtual double GetDamper(){
		return ((OBJ*)(MIF*)this)->OBJ::GetDamper();
	}
	virtual double GetPosition(){
		return ((OBJ*)(MIF*)this)->OBJ::GetPosition();
	}
	virtual double GetVelocity(){
		return ((OBJ*)(MIF*)this)->OBJ::GetVelocity();
	}
};
struct PHJoint1DIf;	class PHJoint1D;
typedef PHJoint1DIfStubTemplate<PHJoint1DIf, ObjectIfBuf, PHJoint1D>	PHJoint1DIfStub;
typedef IfInitTemplate<PHJoint1DIfStub, PHJoint1D>	PHJoint1DIfInit;

template <class IF, class MIF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHHingeJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, MIF, OBJ> {
};
struct PHHingeJointIf;	class PHHingeJoint;
typedef PHHingeJointIfStubTemplate<PHHingeJointIf, ObjectIfBuf, PHHingeJoint>	PHHingeJointIfStub;
typedef IfInitTemplate<PHHingeJointIfStub, PHHingeJoint>	PHHingeJointIfInit;

template <class IF, class MIF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSliderJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, MIF, OBJ> {
};
struct PHSliderJointIf;	class PHSliderJoint;
typedef PHSliderJointIfStubTemplate<PHSliderJointIf, ObjectIfBuf, PHSliderJoint>	PHSliderJointIfStub;
typedef IfInitTemplate<PHSliderJointIfStub, PHSliderJoint>	PHSliderJointIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHPathIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void AddPoint(double s, const Posed &  pose){
		return ((OBJ*)(MIF*)this)->OBJ::AddPoint(s, pose);
	}
	virtual void SetLoop(bool bOnOff = true){
		return ((OBJ*)(MIF*)this)->OBJ::SetLoop(bOnOff);
	}
	virtual bool IsLoop(){
		return ((OBJ*)(MIF*)this)->OBJ::IsLoop();
	}
};
struct PHPathIf;	class PHPath;
typedef PHPathIfStubTemplate<PHPathIf, ObjectIfBuf, PHPath>	PHPathIfStub;
typedef IfInitTemplate<PHPathIfStub, PHPath>	PHPathIfInit;

template <class IF, class MIF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHPathJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetPosition(double q){
		return ((OBJ*)(MIF*)this)->OBJ::SetPosition(q);
	}
};
struct PHPathJointIf;	class PHPathJoint;
typedef PHPathJointIfStubTemplate<PHPathJointIf, ObjectIfBuf, PHPathJoint>	PHPathJointIfStub;
typedef IfInitTemplate<PHPathJointIfStub, PHPathJoint>	PHPathJointIfInit;

template <class IF, class MIF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHBallJointIfStubTemplate: public PHConstraintIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetSwingRange(double upper){
		return ((OBJ*)(MIF*)this)->OBJ::SetSwingRange(upper);
	}
	virtual double GetSwingRange(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSwingRange();
	}
	virtual void SetTwistRange(double lower, double upper){
		return ((OBJ*)(MIF*)this)->OBJ::SetTwistRange(lower, upper);
	}
	virtual void GetTwistRange(double &  lower, double &  upper){
		return ((OBJ*)(MIF*)this)->OBJ::GetTwistRange(lower, upper);
	}
	virtual void SetMotorTorque(const Vec3d &  torque){
		return ((OBJ*)(MIF*)this)->OBJ::SetMotorTorque(torque);
	}
	virtual Vec3d GetMotorTorque(){
		return ((OBJ*)(MIF*)this)->OBJ::GetMotorTorque();
	}
	virtual Vec3d GetAngle(){
		return ((OBJ*)(MIF*)this)->OBJ::GetAngle();
	}
	virtual Vec3d GetVelocity(){
		return ((OBJ*)(MIF*)this)->OBJ::GetVelocity();
	}
};
struct PHBallJointIf;	class PHBallJoint;
typedef PHBallJointIfStubTemplate<PHBallJointIf, ObjectIfBuf, PHBallJoint>	PHBallJointIfStub;
typedef IfInitTemplate<PHBallJointIfStub, PHBallJoint>	PHBallJointIfInit;

template <class IF, class MIF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSpringIfStubTemplate: public PHConstraintIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetSpring(const Vec3d &  spring){
		return ((OBJ*)(MIF*)this)->OBJ::SetSpring(spring);
	}
	virtual Vec3d GetSpring(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSpring();
	}
	virtual void SetDamper(const Vec3d &  damper){
		return ((OBJ*)(MIF*)this)->OBJ::SetDamper(damper);
	}
	virtual Vec3d GetDamper(){
		return ((OBJ*)(MIF*)this)->OBJ::GetDamper();
	}
};
struct PHSpringIf;	class PHSpring;
typedef PHSpringIfStubTemplate<PHSpringIf, ObjectIfBuf, PHSpring>	PHSpringIfStub;
typedef IfInitTemplate<PHSpringIfStub, PHSpring>	PHSpringIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHTreeNodeIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
};
struct PHTreeNodeIf;	class PHTreeNode;
typedef PHTreeNodeIfStubTemplate<PHTreeNodeIf, ObjectIfBuf, PHTreeNode>	PHTreeNodeIfStub;
typedef IfInitTemplate<PHTreeNodeIfStub, PHTreeNode>	PHTreeNodeIfInit;

template <class IF, class MIF, class OBJ> struct PHTreeNodeIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHRootNodeIfStubTemplate: public PHTreeNodeIfStubTemplate<IF, MIF, OBJ> {
};
struct PHRootNodeIf;	class PHRootNode;
typedef PHRootNodeIfStubTemplate<PHRootNodeIf, ObjectIfBuf, PHRootNode>	PHRootNodeIfStub;
typedef IfInitTemplate<PHRootNodeIfStub, PHRootNode>	PHRootNodeIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHGearIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
};
struct PHGearIf;	class PHGear;
typedef PHGearIfStubTemplate<PHGearIf, ObjectIfBuf, PHGear>	PHGearIfStub;
typedef IfInitTemplate<PHGearIfStub, PHGear>	PHGearIfInit;

template <class IF, class MIF, class OBJ> struct SceneIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSceneIfStubTemplate: public SceneIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSdkIf *  GetSdk(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSdk();
	}
	virtual CDShapeIf *  CreateShape(const CDShapeDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateShape(desc);
	}
	virtual PHSolidIf *  CreateSolid(){
		return ((OBJ*)(MIF*)this)->OBJ::CreateSolid();
	}
	virtual PHSolidIf *  CreateSolid(const PHSolidDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateSolid(desc);
	}
	virtual int NSolids(){
		return ((OBJ*)(MIF*)this)->OBJ::NSolids();
	}
	virtual PHSolidIf * *  GetSolids(){
		return ((OBJ*)(MIF*)this)->OBJ::GetSolids();
	}
	virtual void SetContactMode(PHSolidIf *  lhs, PHSolidIf *  rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->OBJ::SetContactMode(lhs, rhs, mode);
	}
	virtual void SetContactMode(PHSolidIf * *  group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->OBJ::SetContactMode(group, length, mode);
	}
	virtual void SetContactMode(PHSolidIf *  solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->OBJ::SetContactMode(solid, mode);
	}
	virtual void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->OBJ::SetContactMode(mode);
	}
	virtual int GetNumIteration(){
		return ((OBJ*)(MIF*)this)->OBJ::GetNumIteration();
	}
	virtual void SetNumIteration(int n){
		return ((OBJ*)(MIF*)this)->OBJ::SetNumIteration(n);
	}
	virtual PHJointIf *  CreateJoint(PHSolidIf *  lhs, PHSolidIf *  rhs, const PHJointDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateJoint(lhs, rhs, desc);
	}
	virtual PHTreeNodeIf *  CreateRootNode(PHSolidIf *  root){
		return ((OBJ*)(MIF*)this)->OBJ::CreateRootNode(root);
	}
	virtual PHTreeNodeIf *  CreateTreeNode(PHTreeNodeIf *  parent, PHSolidIf *  child){
		return ((OBJ*)(MIF*)this)->OBJ::CreateTreeNode(parent, child);
	}
	virtual PHGearIf *  CreateGear(PHJoint1DIf *  lhs, PHJoint1DIf *  rhs, const PHGearDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateGear(lhs, rhs, desc);
	}
	virtual PHPathIf *  CreatePath(const PHPathDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreatePath(desc);
	}
	virtual double GetTimeStep() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetTimeStep();
	}
	virtual void SetTimeStep(double dt){
		return ((OBJ*)(MIF*)this)->OBJ::SetTimeStep(dt);
	}
	virtual unsigned GetCount() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetCount();
	}
	virtual void SetCount(unsigned count){
		return ((OBJ*)(MIF*)this)->OBJ::SetCount(count);
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->OBJ::Step();
	}
	virtual void ClearForce(){
		return ((OBJ*)(MIF*)this)->OBJ::ClearForce();
	}
	virtual void GenerateForce(){
		return ((OBJ*)(MIF*)this)->OBJ::GenerateForce();
	}
	virtual void Integrate(){
		return ((OBJ*)(MIF*)this)->OBJ::Integrate();
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->OBJ::Clear();
	}
	virtual void SetGravity(Vec3f accel){
		return ((OBJ*)(MIF*)this)->OBJ::SetGravity(accel);
	}
	virtual Vec3f GetGravity(){
		return ((OBJ*)(MIF*)this)->OBJ::GetGravity();
	}
	virtual PHConstraintEngine *  GetConstraintEngine(){
		return ((OBJ*)(MIF*)this)->OBJ::GetConstraintEngine();
	}
};
struct PHSceneIf;	class PHScene;
typedef PHSceneIfStubTemplate<PHSceneIf, ObjectIfBuf, PHScene>	PHSceneIfStub;
typedef IfInitTemplate<PHSceneIfStub, PHScene>	PHSceneIfInit;

template <class IF, class MIF, class OBJ> struct NameManagerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSdkIfStubTemplate: public NameManagerIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSceneIf *  CreateScene(){
		return ((OBJ*)(MIF*)this)->OBJ::CreateScene();
	}
	virtual PHSceneIf *  CreateScene(const PHSceneDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateScene(desc);
	}
	virtual int NScene(){
		return ((OBJ*)(MIF*)this)->OBJ::NScene();
	}
	virtual PHSceneIf *  GetScene(size_t i){
		return ((OBJ*)(MIF*)this)->OBJ::GetScene(i);
	}
	virtual CDShapeIf *  CreateShape(const CDShapeDesc &  desc){
		return ((OBJ*)(MIF*)this)->OBJ::CreateShape(desc);
	}
	virtual int NShape(){
		return ((OBJ*)(MIF*)this)->OBJ::NShape();
	}
	virtual CDShapeIf *  GetShape(int i){
		return ((OBJ*)(MIF*)this)->OBJ::GetShape(i);
	}
};
struct PHSdkIf;	class PHSdk;
typedef PHSdkIfStubTemplate<PHSdkIf, ObjectIfBuf, PHSdk>	PHSdkIfStub;
typedef IfInitTemplate<PHSdkIfStub, PHSdk>	PHSdkIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSolidIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void AddForce(Vec3d f){
		return ((OBJ*)(MIF*)this)->OBJ::AddForce(f);
	}
	virtual void AddTorque(Vec3d t){
		return ((OBJ*)(MIF*)this)->OBJ::AddTorque(t);
	}
	virtual void AddForce(Vec3d f, Vec3d r){
		return ((OBJ*)(MIF*)this)->OBJ::AddForce(f, r);
	}
	virtual double GetMass(){
		return ((OBJ*)(MIF*)this)->OBJ::GetMass();
	}
	virtual double GetMassInv(){
		return ((OBJ*)(MIF*)this)->OBJ::GetMassInv();
	}
	virtual void SetMass(double m){
		return ((OBJ*)(MIF*)this)->OBJ::SetMass(m);
	}
	virtual Vec3d GetCenterOfMass(){
		return ((OBJ*)(MIF*)this)->OBJ::GetCenterOfMass();
	}
	virtual void SetCenterOfMass(const Vec3d &  center){
		return ((OBJ*)(MIF*)this)->OBJ::SetCenterOfMass(center);
	}
	virtual Matrix3d GetInertia(){
		return ((OBJ*)(MIF*)this)->OBJ::GetInertia();
	}
	virtual Matrix3d GetInertiaInv(){
		return ((OBJ*)(MIF*)this)->OBJ::GetInertiaInv();
	}
	virtual void SetInertia(const Matrix3d &  I){
		return ((OBJ*)(MIF*)this)->OBJ::SetInertia(I);
	}
	virtual Vec3d GetFramePosition() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetFramePosition();
	}
	virtual void SetFramePosition(const Vec3d &  p){
		return ((OBJ*)(MIF*)this)->OBJ::SetFramePosition(p);
	}
	virtual Vec3d GetCenterPosition() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetCenterPosition();
	}
	virtual void SetCenterPosition(const Vec3d &  p){
		return ((OBJ*)(MIF*)this)->OBJ::SetCenterPosition(p);
	}
	virtual Quaterniond GetOrientation() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetOrientation();
	}
	virtual void SetOrientation(const Quaterniond &  q){
		return ((OBJ*)(MIF*)this)->OBJ::SetOrientation(q);
	}
	virtual Posed GetPose() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetPose();
	}
	virtual void SetPose(const Posed &  p){
		return ((OBJ*)(MIF*)this)->OBJ::SetPose(p);
	}
	virtual Vec3d GetVelocity() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetVelocity();
	}
	virtual void SetVelocity(const Vec3d &  v){
		return ((OBJ*)(MIF*)this)->OBJ::SetVelocity(v);
	}
	virtual Vec3d GetAngularVelocity() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetAngularVelocity();
	}
	virtual void SetAngularVelocity(const Vec3d &  av){
		return ((OBJ*)(MIF*)this)->OBJ::SetAngularVelocity(av);
	}
	virtual Vec3d GetForce() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetForce();
	}
	virtual Vec3d GetTorque() const {
		return ((OBJ*)(MIF*)this)->OBJ::GetTorque();
	}
	virtual void AddShape(CDShapeIf *  shape){
		return ((OBJ*)(MIF*)this)->OBJ::AddShape(shape);
	}
	virtual int NShape(){
		return ((OBJ*)(MIF*)this)->OBJ::NShape();
	}
	virtual CDShapeIf *	 GetShape(int index){
		return ((OBJ*)(MIF*)this)->OBJ::GetShape(index);
	}
	virtual Posed GetShapePose(int index){
		return ((OBJ*)(MIF*)this)->OBJ::GetShapePose(index);
	}
	virtual void SetShapePose(int index, const Posed &  pose){
		return ((OBJ*)(MIF*)this)->OBJ::SetShapePose(index, pose);
	}
	virtual void SetGravity(bool bOn){
		return ((OBJ*)(MIF*)this)->OBJ::SetGravity(bOn);
	}
	virtual void SetDynamical(bool bOn){
		return ((OBJ*)(MIF*)this)->OBJ::SetDynamical(bOn);
	}
	virtual bool IsDynamical(){
		return ((OBJ*)(MIF*)this)->OBJ::IsDynamical();
	}
};
struct PHSolidIf;	class PHSolid;
typedef PHSolidIfStubTemplate<PHSolidIf, ObjectIfBuf, PHSolid>	PHSolidIfStub;
typedef IfInitTemplate<PHSolidIfStub, PHSolid>	PHSolidIfInit;
}	//	namespace Spr; 
