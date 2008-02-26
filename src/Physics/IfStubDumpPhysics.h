//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHEngineIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
};
struct PHEngineIf;	class PHEngine;
typedef PHEngineIfStubTemplate<PHEngineIf, ObjectIfBuf, PHEngine>	PHEngineIfStub;
typedef IfInitTemplate<PHEngineIfStub, PHEngine>	PHEngineIfInit;

template <class IF, class MIF, class OBJ> struct PHEngineIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHConstraintEngineIfStubTemplate: public PHEngineIfStubTemplate<IF, MIF, OBJ> {
	virtual PHConstraintsIf * GetContactPoints(){
		return ((OBJ*)(MIF*)this)->GetContactPoints();
	}
	virtual void SetVelCorrectionRate(double value){
		return ((OBJ*)(MIF*)this)->SetVelCorrectionRate(value);
	}
	virtual void SetPosCorrectionRate(double value){
		return ((OBJ*)(MIF*)this)->SetPosCorrectionRate(value);
	}
	virtual void SetContactCorrectionRate(double value){
		return ((OBJ*)(MIF*)this)->SetContactCorrectionRate(value);
	}
};
struct PHConstraintEngineIf;	class PHConstraintEngine;
typedef PHConstraintEngineIfStubTemplate<PHConstraintEngineIf, ObjectIfBuf, PHConstraintEngine>	PHConstraintEngineIfStub;
typedef IfInitTemplate<PHConstraintEngineIfStub, PHConstraintEngine>	PHConstraintEngineIfInit;

template <class IF, class MIF, class OBJ> struct PHEngineIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHGravityEngineIfStubTemplate: public PHEngineIfStubTemplate<IF, MIF, OBJ> {
};
struct PHGravityEngineIf;	class PHGravityEngine;
typedef PHGravityEngineIfStubTemplate<PHGravityEngineIf, ObjectIfBuf, PHGravityEngine>	PHGravityEngineIfStub;
typedef IfInitTemplate<PHGravityEngineIfStub, PHGravityEngine>	PHGravityEngineIfInit;

template <class IF, class MIF, class OBJ> struct PHEngineIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHPenaltyEngineIfStubTemplate: public PHEngineIfStubTemplate<IF, MIF, OBJ> {
};
struct PHPenaltyEngineIf;	class PHPenaltyEngine;
typedef PHPenaltyEngineIfStubTemplate<PHPenaltyEngineIf, ObjectIfBuf, PHPenaltyEngine>	PHPenaltyEngineIfStub;
typedef IfInitTemplate<PHPenaltyEngineIfStub, PHPenaltyEngine>	PHPenaltyEngineIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHConstraintIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Enable(bool bEnable = true){
		return ((OBJ*)(MIF*)this)->Enable(bEnable);
	}
	virtual bool IsEnabled(){
		return ((OBJ*)(MIF*)this)->IsEnabled();
	}
	virtual void GetSocketPose(Posed & pose){
		return ((OBJ*)(MIF*)this)->GetSocketPose(pose);
	}
	virtual void SetSocketPose(const Posed & pose){
		return ((OBJ*)(MIF*)this)->SetSocketPose(pose);
	}
	virtual void GetPlugPose(Posed & pose){
		return ((OBJ*)(MIF*)this)->GetPlugPose(pose);
	}
	virtual void SetPlugPose(const Posed & pose){
		return ((OBJ*)(MIF*)this)->SetPlugPose(pose);
	}
	virtual void GetRelativePose(Posed & p){
		return ((OBJ*)(MIF*)this)->GetRelativePose(p);
	}
	virtual Vec3d GetRelativePoseR(){
		return ((OBJ*)(MIF*)this)->GetRelativePoseR();
	}
	virtual Quaterniond GetRelativePoseQ(){
		return ((OBJ*)(MIF*)this)->GetRelativePoseQ();
	}
	virtual void GetRelativeVelocity(Vec3d & v, Vec3d & w){
		return ((OBJ*)(MIF*)this)->GetRelativeVelocity(v, w);
	}
	virtual void GetConstraintForce(Vec3d & f, Vec3d & t){
		return ((OBJ*)(MIF*)this)->GetConstraintForce(f, t);
	}
};
struct PHConstraintIf;	class PHConstraint;
typedef PHConstraintIfStubTemplate<PHConstraintIf, ObjectIfBuf, PHConstraint>	PHConstraintIfStub;
typedef IfInitTemplate<PHConstraintIfStub, PHConstraint>	PHConstraintIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHConstraintsIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual PHConstraintIf * FindBySolidPair(PHSolidIf * lhs, PHSolidIf * rhs){
		return ((OBJ*)(MIF*)this)->FindBySolidPair(lhs, rhs);
	}
};
struct PHConstraintsIf;	class PHConstraints;
typedef PHConstraintsIfStubTemplate<PHConstraintsIf, ObjectIfBuf, PHConstraints>	PHConstraintsIfStub;
typedef IfInitTemplate<PHConstraintsIfStub, PHConstraints>	PHConstraintsIfInit;

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
		return ((OBJ*)(MIF*)this)->SetRange(lower, upper);
	}
	virtual void GetRange(double & lower, double & upper){
		return ((OBJ*)(MIF*)this)->GetRange(lower, upper);
	}
	virtual void SetMotorTorque(double t){
		return ((OBJ*)(MIF*)this)->SetMotorTorque(t);
	}
	virtual double GetMotorTorque(){
		return ((OBJ*)(MIF*)this)->GetMotorTorque();
	}
	virtual void SetDesiredVelocity(double v){
		return ((OBJ*)(MIF*)this)->SetDesiredVelocity(v);
	}
	virtual double GetDesiredVelocity(){
		return ((OBJ*)(MIF*)this)->GetDesiredVelocity();
	}
	virtual void SetSpring(double spring){
		return ((OBJ*)(MIF*)this)->SetSpring(spring);
	}
	virtual double GetSpring(){
		return ((OBJ*)(MIF*)this)->GetSpring();
	}
	virtual void SetSpringOrigin(double origin){
		return ((OBJ*)(MIF*)this)->SetSpringOrigin(origin);
	}
	virtual double GetSpringOrigin(){
		return ((OBJ*)(MIF*)this)->GetSpringOrigin();
	}
	virtual void SetDamper(double damper){
		return ((OBJ*)(MIF*)this)->SetDamper(damper);
	}
	virtual double GetDamper(){
		return ((OBJ*)(MIF*)this)->GetDamper();
	}
	virtual double GetPosition(){
		return ((OBJ*)(MIF*)this)->GetPosition();
	}
	virtual double GetVelocity(){
		return ((OBJ*)(MIF*)this)->GetVelocity();
	}
	virtual void SetTorqueMax(double max){
		return ((OBJ*)(MIF*)this)->SetTorqueMax(max);
	}
	virtual double GetTorqueMax(){
		return ((OBJ*)(MIF*)this)->GetTorqueMax();
	}
	virtual void SetTorqueMin(double min){
		return ((OBJ*)(MIF*)this)->SetTorqueMin(min);
	}
	virtual double GetTorqueMin(){
		return ((OBJ*)(MIF*)this)->GetTorqueMin();
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
	virtual void AddPoint(double s, const Posed & pose){
		return ((OBJ*)(MIF*)this)->AddPoint(s, pose);
	}
	virtual void SetLoop(bool bOnOff = true){
		return ((OBJ*)(MIF*)this)->SetLoop(bOnOff);
	}
	virtual bool IsLoop(){
		return ((OBJ*)(MIF*)this)->IsLoop();
	}
};
struct PHPathIf;	class PHPath;
typedef PHPathIfStubTemplate<PHPathIf, ObjectIfBuf, PHPath>	PHPathIfStub;
typedef IfInitTemplate<PHPathIfStub, PHPath>	PHPathIfInit;

template <class IF, class MIF, class OBJ> struct PHJoint1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHPathJointIfStubTemplate: public PHJoint1DIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetPosition(double q){
		return ((OBJ*)(MIF*)this)->SetPosition(q);
	}
};
struct PHPathJointIf;	class PHPathJoint;
typedef PHPathJointIfStubTemplate<PHPathJointIf, ObjectIfBuf, PHPathJoint>	PHPathJointIfStub;
typedef IfInitTemplate<PHPathJointIfStub, PHPathJoint>	PHPathJointIfInit;

template <class IF, class MIF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHBallJointIfStubTemplate: public PHConstraintIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetSwingRange(Vec2d range){
		return ((OBJ*)(MIF*)this)->SetSwingRange(range);
	}
	virtual void GetSwingRange(Vec2d & range){
		return ((OBJ*)(MIF*)this)->GetSwingRange(range);
	}
	virtual void SetTwistRange(Vec2d range){
		return ((OBJ*)(MIF*)this)->SetTwistRange(range);
	}
	virtual void GetTwistRange(Vec2d & range){
		return ((OBJ*)(MIF*)this)->GetTwistRange(range);
	}
	virtual void SetMotorTorque(const Vec3d & torque){
		return ((OBJ*)(MIF*)this)->SetMotorTorque(torque);
	}
	virtual Vec3d GetMotorTorque(){
		return ((OBJ*)(MIF*)this)->GetMotorTorque();
	}
	virtual Vec3d GetAngle(){
		return ((OBJ*)(MIF*)this)->GetAngle();
	}
	virtual Vec3d GetVelocity(){
		return ((OBJ*)(MIF*)this)->GetVelocity();
	}
	virtual void SetTorqueMax(double max){
		return ((OBJ*)(MIF*)this)->SetTorqueMax(max);
	}
	virtual double GetTorqueMax(){
		return ((OBJ*)(MIF*)this)->GetTorqueMax();
	}
	virtual void SetTorqueMin(double min){
		return ((OBJ*)(MIF*)this)->SetTorqueMin(min);
	}
	virtual double GetTorqueMin(){
		return ((OBJ*)(MIF*)this)->GetTorqueMin();
	}
};
struct PHBallJointIf;	class PHBallJoint;
typedef PHBallJointIfStubTemplate<PHBallJointIf, ObjectIfBuf, PHBallJoint>	PHBallJointIfStub;
typedef IfInitTemplate<PHBallJointIfStub, PHBallJoint>	PHBallJointIfInit;

template <class IF, class MIF, class OBJ> struct PHConstraintIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSpringIfStubTemplate: public PHConstraintIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetSpring(const Vec3d & spring){
		return ((OBJ*)(MIF*)this)->SetSpring(spring);
	}
	virtual Vec3d GetSpring(){
		return ((OBJ*)(MIF*)this)->GetSpring();
	}
	virtual void SetDamper(const Vec3d & damper){
		return ((OBJ*)(MIF*)this)->SetDamper(damper);
	}
	virtual Vec3d GetDamper(){
		return ((OBJ*)(MIF*)this)->GetDamper();
	}
	virtual void SetSpringOri(const double spring){
		return ((OBJ*)(MIF*)this)->SetSpringOri(spring);
	}
	virtual double GetSpringOri(){
		return ((OBJ*)(MIF*)this)->GetSpringOri();
	}
	virtual void SetDamperOri(const double damper){
		return ((OBJ*)(MIF*)this)->SetDamperOri(damper);
	}
	virtual double GetDamperOri(){
		return ((OBJ*)(MIF*)this)->GetDamperOri();
	}
};
struct PHSpringIf;	class PHSpring;
typedef PHSpringIfStubTemplate<PHSpringIf, ObjectIfBuf, PHSpring>	PHSpringIfStub;
typedef IfInitTemplate<PHSpringIfStub, PHSpring>	PHSpringIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHTreeNodeIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Enable(bool bEnable = true){
		return ((OBJ*)(MIF*)this)->Enable(bEnable);
	}
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

template <class IF, class MIF, class OBJ> struct PHTreeNodeIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHTreeNode1DIfStubTemplate: public PHTreeNodeIfStubTemplate<IF, MIF, OBJ> {
};
struct PHTreeNode1DIf;	class PHTreeNode1D;
typedef PHTreeNode1DIfStubTemplate<PHTreeNode1DIf, ObjectIfBuf, PHTreeNode1D>	PHTreeNode1DIfStub;
typedef IfInitTemplate<PHTreeNode1DIfStub, PHTreeNode1D>	PHTreeNode1DIfInit;

template <class IF, class MIF, class OBJ> struct PHTreeNode1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHHingeJointNodeIfStubTemplate: public PHTreeNode1DIfStubTemplate<IF, MIF, OBJ> {
};
struct PHHingeJointNodeIf;	class PHHingeJointNode;
typedef PHHingeJointNodeIfStubTemplate<PHHingeJointNodeIf, ObjectIfBuf, PHHingeJointNode>	PHHingeJointNodeIfStub;
typedef IfInitTemplate<PHHingeJointNodeIfStub, PHHingeJointNode>	PHHingeJointNodeIfInit;

template <class IF, class MIF, class OBJ> struct PHTreeNode1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSliderJointNodeIfStubTemplate: public PHTreeNode1DIfStubTemplate<IF, MIF, OBJ> {
};
struct PHSliderJointNodeIf;	class PHSliderJointNode;
typedef PHSliderJointNodeIfStubTemplate<PHSliderJointNodeIf, ObjectIfBuf, PHSliderJointNode>	PHSliderJointNodeIfStub;
typedef IfInitTemplate<PHSliderJointNodeIfStub, PHSliderJointNode>	PHSliderJointNodeIfInit;

template <class IF, class MIF, class OBJ> struct PHTreeNode1DIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHPathJointNodeIfStubTemplate: public PHTreeNode1DIfStubTemplate<IF, MIF, OBJ> {
};
struct PHPathJointNodeIf;	class PHPathJointNode;
typedef PHPathJointNodeIfStubTemplate<PHPathJointNodeIf, ObjectIfBuf, PHPathJointNode>	PHPathJointNodeIfStub;
typedef IfInitTemplate<PHPathJointNodeIfStub, PHPathJointNode>	PHPathJointNodeIfInit;

template <class IF, class MIF, class OBJ> struct PHTreeNodeIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHBallJointNodeIfStubTemplate: public PHTreeNodeIfStubTemplate<IF, MIF, OBJ> {
};
struct PHBallJointNodeIf;	class PHBallJointNode;
typedef PHBallJointNodeIfStubTemplate<PHBallJointNodeIf, ObjectIfBuf, PHBallJointNode>	PHBallJointNodeIfStub;
typedef IfInitTemplate<PHBallJointNodeIfStub, PHBallJointNode>	PHBallJointNodeIfInit;

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
	virtual PHSdkIf * GetSdk(){
		return ((OBJ*)(MIF*)this)->GetSdk();
	}
	virtual PHSolidIf * CreateSolid(const PHSolidDesc & desc = PHSolidDesc()){
		return ((OBJ*)(MIF*)this)->CreateSolid(desc);
	}
	virtual int NSolids() const {
		return ((OBJ*)(MIF*)this)->NSolids();
	}
	virtual PHSolidIf * * GetSolids(){
		return ((OBJ*)(MIF*)this)->GetSolids();
	}
	virtual void SetContactMode(PHSolidIf * lhs, PHSolidIf * rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->SetContactMode(lhs, rhs, mode);
	}
	virtual void SetContactMode(PHSolidIf * * group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->SetContactMode(group, length, mode);
	}
	virtual void SetContactMode(PHSolidIf * solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->SetContactMode(solid, mode);
	}
	virtual void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP){
		return ((OBJ*)(MIF*)this)->SetContactMode(mode);
	}
	virtual int GetNumIteration(){
		return ((OBJ*)(MIF*)this)->GetNumIteration();
	}
	virtual void SetNumIteration(int n){
		return ((OBJ*)(MIF*)this)->SetNumIteration(n);
	}
	virtual PHJointIf * CreateJoint(PHSolidIf * lhs, PHSolidIf * rhs, const IfInfo * ii, const PHJointDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateJoint(lhs, rhs, ii, desc);
	}
	virtual int NJoints() const {
		return ((OBJ*)(MIF*)this)->NJoints();
	}
	virtual PHJointIf * GetJoint(int i){
		return ((OBJ*)(MIF*)this)->GetJoint(i);
	}
	virtual int NContacts() const {
		return ((OBJ*)(MIF*)this)->NContacts();
	}
	virtual PHContactPointIf * GetContact(int i){
		return ((OBJ*)(MIF*)this)->GetContact(i);
	}
	virtual PHRootNodeIf * CreateRootNode(PHSolidIf * root, const PHRootNodeDesc & desc = PHRootNodeDesc()){
		return ((OBJ*)(MIF*)this)->CreateRootNode(root, desc);
	}
	virtual int NRootNodes() const {
		return ((OBJ*)(MIF*)this)->NRootNodes();
	}
	virtual PHRootNodeIf * GetRootNode(int i){
		return ((OBJ*)(MIF*)this)->GetRootNode(i);
	}
	virtual PHTreeNodeIf * CreateTreeNode(PHTreeNodeIf * parent, PHSolidIf * child, const PHTreeNodeDesc & desc = PHTreeNodeDesc()){
		return ((OBJ*)(MIF*)this)->CreateTreeNode(parent, child, desc);
	}
	virtual PHGearIf * CreateGear(PHJoint1DIf * lhs, PHJoint1DIf * rhs, const PHGearDesc & desc = PHGearDesc()){
		return ((OBJ*)(MIF*)this)->CreateGear(lhs, rhs, desc);
	}
	virtual int NGears() const {
		return ((OBJ*)(MIF*)this)->NGears();
	}
	virtual PHGearIf * GetGear(int i){
		return ((OBJ*)(MIF*)this)->GetGear(i);
	}
	virtual PHPathIf * CreatePath(const PHPathDesc & desc = PHPathDesc()){
		return ((OBJ*)(MIF*)this)->CreatePath(desc);
	}
	virtual int NPaths(){
		return ((OBJ*)(MIF*)this)->NPaths();
	}
	virtual PHPathIf * GetPath(int i){
		return ((OBJ*)(MIF*)this)->GetPath(i);
	}
	virtual double GetTimeStep() const {
		return ((OBJ*)(MIF*)this)->GetTimeStep();
	}
	virtual void SetTimeStep(double dt){
		return ((OBJ*)(MIF*)this)->SetTimeStep(dt);
	}
	virtual unsigned GetCount() const {
		return ((OBJ*)(MIF*)this)->GetCount();
	}
	virtual void SetCount(unsigned count){
		return ((OBJ*)(MIF*)this)->SetCount(count);
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
	virtual void ClearForce(){
		return ((OBJ*)(MIF*)this)->ClearForce();
	}
	virtual void GenerateForce(){
		return ((OBJ*)(MIF*)this)->GenerateForce();
	}
	virtual void Integrate(){
		return ((OBJ*)(MIF*)this)->Integrate();
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->Clear();
	}
	virtual void SetGravity(const Vec3d & accel){
		return ((OBJ*)(MIF*)this)->SetGravity(accel);
	}
	virtual Vec3d GetGravity(){
		return ((OBJ*)(MIF*)this)->GetGravity();
	}
	virtual int NEngines(){
		return ((OBJ*)(MIF*)this)->NEngines();
	}
	virtual PHEngineIf * GetEngine(int i){
		return ((OBJ*)(MIF*)this)->GetEngine(i);
	}
	virtual PHConstraintEngineIf * GetConstraintEngine(){
		return ((OBJ*)(MIF*)this)->GetConstraintEngine();
	}
	virtual PHGravityEngineIf * GetGravityEngine(){
		return ((OBJ*)(MIF*)this)->GetGravityEngine();
	}
	virtual PHPenaltyEngineIf * GetPenaltyEngine(){
		return ((OBJ*)(MIF*)this)->GetPenaltyEngine();
	}
};
struct PHSceneIf;	class PHScene;
typedef PHSceneIfStubTemplate<PHSceneIf, ObjectIfBuf, PHScene>	PHSceneIfStub;
typedef IfInitTemplate<PHSceneIfStub, PHScene>	PHSceneIfInit;

template <class IF, class MIF, class OBJ> struct SdkIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSdkIfStubTemplate: public SdkIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSceneIf * CreateScene(){
		return ((OBJ*)(MIF*)this)->CreateScene();
	}
	virtual PHSceneIf * CreateScene(const PHSceneDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateScene(desc);
	}
	virtual int NScene(){
		return ((OBJ*)(MIF*)this)->NScene();
	}
	virtual PHSceneIf * GetScene(size_t i){
		return ((OBJ*)(MIF*)this)->GetScene(i);
	}
	virtual void MergeScene(PHSceneIf * scene0, PHSceneIf * scene1){
		return ((OBJ*)(MIF*)this)->MergeScene(scene0, scene1);
	}
	virtual CDShapeIf * CreateShape(const IfInfo * ii, const CDShapeDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateShape(ii, desc);
	}
	virtual int NShape(){
		return ((OBJ*)(MIF*)this)->NShape();
	}
	virtual CDShapeIf * GetShape(int i){
		return ((OBJ*)(MIF*)this)->GetShape(i);
	}
};
struct PHSdkIf;	class PHSdk;
typedef PHSdkIfStubTemplate<PHSdkIf, ObjectIfBuf, PHSdk>	PHSdkIfStub;
typedef IfInitTemplate<PHSdkIfStub, PHSdk>	PHSdkIfInit;

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHFrameIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual Posed GetPose(){
		return ((OBJ*)(MIF*)this)->GetPose();
	}
	virtual void SetPose(Posed p){
		return ((OBJ*)(MIF*)this)->SetPose(p);
	}
};
struct PHFrameIf;	class PHFrame;
typedef PHFrameIfStubTemplate<PHFrameIf, ObjectIfBuf, PHFrame>	PHFrameIfStub;
typedef IfInitTemplate<PHFrameIfStub, PHFrame>	PHFrameIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct PHSolidIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void AddForce(Vec3d f){
		return ((OBJ*)(MIF*)this)->AddForce(f);
	}
	virtual void AddTorque(Vec3d t){
		return ((OBJ*)(MIF*)this)->AddTorque(t);
	}
	virtual void AddForce(Vec3d f, Vec3d r){
		return ((OBJ*)(MIF*)this)->AddForce(f, r);
	}
	virtual double GetMass(){
		return ((OBJ*)(MIF*)this)->GetMass();
	}
	virtual double GetMassInv(){
		return ((OBJ*)(MIF*)this)->GetMassInv();
	}
	virtual void SetMass(double m){
		return ((OBJ*)(MIF*)this)->SetMass(m);
	}
	virtual Vec3d GetCenterOfMass(){
		return ((OBJ*)(MIF*)this)->GetCenterOfMass();
	}
	virtual void SetCenterOfMass(const Vec3d & center){
		return ((OBJ*)(MIF*)this)->SetCenterOfMass(center);
	}
	virtual Matrix3d GetInertia(){
		return ((OBJ*)(MIF*)this)->GetInertia();
	}
	virtual Matrix3d GetInertiaInv(){
		return ((OBJ*)(MIF*)this)->GetInertiaInv();
	}
	virtual void SetInertia(const Matrix3d & I){
		return ((OBJ*)(MIF*)this)->SetInertia(I);
	}
	virtual Vec3d GetFramePosition() const {
		return ((OBJ*)(MIF*)this)->GetFramePosition();
	}
	virtual void SetFramePosition(const Vec3d & p){
		return ((OBJ*)(MIF*)this)->SetFramePosition(p);
	}
	virtual Vec3d GetCenterPosition() const {
		return ((OBJ*)(MIF*)this)->GetCenterPosition();
	}
	virtual void SetCenterPosition(const Vec3d & p){
		return ((OBJ*)(MIF*)this)->SetCenterPosition(p);
	}
	virtual Vec3d GetDeltaPosition(const Vec3d & p) const {
		return ((OBJ*)(MIF*)this)->GetDeltaPosition(p);
	}
	virtual Vec3d GetDeltaPosition() const {
		return ((OBJ*)(MIF*)this)->GetDeltaPosition();
	}
	virtual Quaterniond GetOrientation() const {
		return ((OBJ*)(MIF*)this)->GetOrientation();
	}
	virtual void SetOrientation(const Quaterniond & q){
		return ((OBJ*)(MIF*)this)->SetOrientation(q);
	}
	virtual Posed GetPose() const {
		return ((OBJ*)(MIF*)this)->GetPose();
	}
	virtual Vec3d GetPoseR() const {
		return ((OBJ*)(MIF*)this)->GetPoseR();
	}
	virtual Quaterniond GetPoseQ() const {
		return ((OBJ*)(MIF*)this)->GetPoseQ();
	}
	virtual void SetPose(const Posed & p){
		return ((OBJ*)(MIF*)this)->SetPose(p);
	}
	virtual Vec3d GetVelocity() const {
		return ((OBJ*)(MIF*)this)->GetVelocity();
	}
	virtual void SetVelocity(const Vec3d & v){
		return ((OBJ*)(MIF*)this)->SetVelocity(v);
	}
	virtual Vec3d GetAngularVelocity() const {
		return ((OBJ*)(MIF*)this)->GetAngularVelocity();
	}
	virtual void SetAngularVelocity(const Vec3d & av){
		return ((OBJ*)(MIF*)this)->SetAngularVelocity(av);
	}
	virtual Vec3d GetForce() const {
		return ((OBJ*)(MIF*)this)->GetForce();
	}
	virtual Vec3d GetTorque() const {
		return ((OBJ*)(MIF*)this)->GetTorque();
	}
	virtual void AddShape(CDShapeIf * shape){
		return ((OBJ*)(MIF*)this)->AddShape(shape);
	}
	virtual int NShape(){
		return ((OBJ*)(MIF*)this)->NShape();
	}
	virtual CDShapeIf * GetShape(int index){
		return ((OBJ*)(MIF*)this)->GetShape(index);
	}
	virtual Posed GetShapePose(int index){
		return ((OBJ*)(MIF*)this)->GetShapePose(index);
	}
	virtual void SetShapePose(int index, const Posed & pose){
		return ((OBJ*)(MIF*)this)->SetShapePose(index, pose);
	}
	virtual void SetGravity(bool bOn){
		return ((OBJ*)(MIF*)this)->SetGravity(bOn);
	}
	virtual void SetDynamical(bool bOn){
		return ((OBJ*)(MIF*)this)->SetDynamical(bOn);
	}
	virtual bool IsDynamical(){
		return ((OBJ*)(MIF*)this)->IsDynamical();
	}
};
struct PHSolidIf;	class PHSolid;
typedef PHSolidIfStubTemplate<PHSolidIf, ObjectIfBuf, PHSolid>	PHSolidIfStub;
typedef IfInitTemplate<PHSolidIfStub, PHSolid>	PHSolidIfInit;
}	//	namespace Spr; 
