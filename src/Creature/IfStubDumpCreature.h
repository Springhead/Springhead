//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRBodyIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual int NSolids(){
		return ((OBJ*)(MIF*)this)->NSolids();
	}
	virtual PHSolidIf * GetSolid(int i){
		return ((OBJ*)(MIF*)this)->GetSolid(i);
	}
	virtual int NJoints(){
		return ((OBJ*)(MIF*)this)->NJoints();
	}
	virtual PHJointIf * GetJoint(int i){
		return ((OBJ*)(MIF*)this)->GetJoint(i);
	}
};
struct CRBodyIf;	class CRBody;
typedef CRBodyIfStubTemplate<CRBodyIf, ObjectIfBuf, CRBody>	CRBodyIfStub;
typedef IfInitTemplate<CRBodyIfStub, CRBody>	CRBodyIfInit;

template <class IF, class MIF, class OBJ> struct CRBodyIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRHingeHumanBodyIfStubTemplate: public CRBodyIfStubTemplate<IF, MIF, OBJ> {
};
struct CRHingeHumanBodyIf;	class CRHingeHumanBody;
typedef CRHingeHumanBodyIfStubTemplate<CRHingeHumanBodyIf, ObjectIfBuf, CRHingeHumanBody>	CRHingeHumanBodyIfStub;
typedef IfInitTemplate<CRHingeHumanBodyIfStub, CRHingeHumanBody>	CRHingeHumanBodyIfInit;

template <class IF, class MIF, class OBJ> struct CRBodyIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRTrunkFootHumanBodyIfStubTemplate: public CRBodyIfStubTemplate<IF, MIF, OBJ> {
};
struct CRTrunkFootHumanBodyIf;	class CRTrunkFootHumanBody;
typedef CRTrunkFootHumanBodyIfStubTemplate<CRTrunkFootHumanBodyIf, ObjectIfBuf, CRTrunkFootHumanBody>	CRTrunkFootHumanBodyIfStub;
typedef IfInitTemplate<CRTrunkFootHumanBodyIfStub, CRTrunkFootHumanBody>	CRTrunkFootHumanBodyIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRControllerIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
	virtual void SetEnable(bool enable){
		return ((OBJ*)(MIF*)this)->SetEnable(enable);
	}
	virtual bool IsEnabled(){
		return ((OBJ*)(MIF*)this)->IsEnabled();
	}
};
struct CRControllerIf;	class CRController;
typedef CRControllerIfStubTemplate<CRControllerIf, ObjectIfBuf, CRController>	CRControllerIfStub;
typedef IfInitTemplate<CRControllerIfStub, CRController>	CRControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CREyeControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual void LookAt(Vec3f pos, Vec3f vel){
		return ((OBJ*)(MIF*)this)->LookAt(pos, vel);
	}
	virtual bool IsSaccading(){
		return ((OBJ*)(MIF*)this)->IsSaccading();
	}
};
struct CREyeControllerIf;	class CREyeController;
typedef CREyeControllerIfStubTemplate<CREyeControllerIf, ObjectIfBuf, CREyeController>	CREyeControllerIfStub;
typedef IfInitTemplate<CREyeControllerIfStub, CREyeController>	CREyeControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRNeckControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness){
		return ((OBJ*)(MIF*)this)->LookAt(pos, vel, attractiveness);
	}
};
struct CRNeckControllerIf;	class CRNeckController;
typedef CRNeckControllerIfStubTemplate<CRNeckControllerIf, ObjectIfBuf, CRNeckController>	CRNeckControllerIfStub;
typedef IfInitTemplate<CRNeckControllerIfStub, CRNeckController>	CRNeckControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRReachingControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual PHSolidIf * GetSolid(){
		return ((OBJ*)(MIF*)this)->GetSolid();
	}
	virtual void SetTarget(Vec3f p, Vec3f v, float t, float o){
		return ((OBJ*)(MIF*)this)->SetTarget(p, v, t, o);
	}
	virtual void SetTarget(Vec3f p, Vec3f v, Quaterniond q, Vec3f av, float t, float o){
		return ((OBJ*)(MIF*)this)->SetTarget(p, v, q, av, t, o);
	}
	virtual bool IsMoving(){
		return ((OBJ*)(MIF*)this)->IsMoving();
	}
	virtual bool IsReached(){
		return ((OBJ*)(MIF*)this)->IsReached();
	}
	virtual bool IsActive(){
		return ((OBJ*)(MIF*)this)->IsActive();
	}
	virtual void Reset(){
		return ((OBJ*)(MIF*)this)->Reset();
	}
};
struct CRReachingControllerIf;	class CRReachingController;
typedef CRReachingControllerIfStubTemplate<CRReachingControllerIf, ObjectIfBuf, CRReachingController>	CRReachingControllerIfStub;
typedef IfInitTemplate<CRReachingControllerIfStub, CRReachingController>	CRReachingControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRGazeControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual void LookAt(Vec3f pos, Vec3f vel, float attractiveness){
		return ((OBJ*)(MIF*)this)->LookAt(pos, vel, attractiveness);
	}
};
struct CRGazeControllerIf;	class CRGazeController;
typedef CRGazeControllerIfStubTemplate<CRGazeControllerIf, ObjectIfBuf, CRGazeController>	CRGazeControllerIfStub;
typedef IfInitTemplate<CRGazeControllerIfStub, CRGazeController>	CRGazeControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRAttentionControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
};
struct CRAttentionControllerIf;	class CRAttentionController;
typedef CRAttentionControllerIfStubTemplate<CRAttentionControllerIf, ObjectIfBuf, CRAttentionController>	CRAttentionControllerIfStub;
typedef IfInitTemplate<CRAttentionControllerIfStub, CRAttentionController>	CRAttentionControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRWalkingControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetSpeed(float speed){
		return ((OBJ*)(MIF*)this)->SetSpeed(speed);
	}
	virtual void SetRotationAngle(float rot){
		return ((OBJ*)(MIF*)this)->SetRotationAngle(rot);
	}
	virtual void SetRotationWorldCoordinate(double r){
		return ((OBJ*)(MIF*)this)->SetRotationWorldCoordinate(r);
	}
	virtual void Stop(){
		return ((OBJ*)(MIF*)this)->Stop();
	}
	virtual void Reverse(){
		return ((OBJ*)(MIF*)this)->Reverse();
	}
	virtual void SetPos(Vec3f pos){
		return ((OBJ*)(MIF*)this)->SetPos(pos);
	}
};
struct CRWalkingControllerIf;	class CRWalkingController;
typedef CRWalkingControllerIfStubTemplate<CRWalkingControllerIf, ObjectIfBuf, CRWalkingController>	CRWalkingControllerIfStub;
typedef IfInitTemplate<CRWalkingControllerIfStub, CRWalkingController>	CRWalkingControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CREseWalkingControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetSpeed(float speed){
		return ((OBJ*)(MIF*)this)->SetSpeed(speed);
	}
	virtual void SetRotationAngle(float rot){
		return ((OBJ*)(MIF*)this)->SetRotationAngle(rot);
	}
	virtual void SetPos(Vec3f pos){
		return ((OBJ*)(MIF*)this)->SetPos(pos);
	}
};
struct CREseWalkingControllerIf;	class CREseWalkingController;
typedef CREseWalkingControllerIfStubTemplate<CREseWalkingControllerIf, ObjectIfBuf, CREseWalkingController>	CREseWalkingControllerIfStub;
typedef IfInitTemplate<CREseWalkingControllerIfStub, CREseWalkingController>	CREseWalkingControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRTravelControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual void SetGoal(Vec3f goal){
		return ((OBJ*)(MIF*)this)->SetGoal(goal);
	}
};
struct CRTravelControllerIf;	class CRTravelController;
typedef CRTravelControllerIfStubTemplate<CRTravelControllerIf, ObjectIfBuf, CRTravelController>	CRTravelControllerIfStub;
typedef IfInitTemplate<CRTravelControllerIfStub, CRTravelController>	CRTravelControllerIfInit;

template <class IF, class MIF, class OBJ> struct CRControllerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRGrabControllerIfStubTemplate: public CRControllerIfStubTemplate<IF, MIF, OBJ> {
	virtual bool Reach(PHSolidIf * solid, float radius){
		return ((OBJ*)(MIF*)this)->Reach(solid, radius);
	}
	virtual bool IsReachable(PHSolidIf * solid){
		return ((OBJ*)(MIF*)this)->IsReachable(solid);
	}
	virtual bool IsReachable(PHSolidIf * solid, float safety){
		return ((OBJ*)(MIF*)this)->IsReachable(solid, safety);
	}
	virtual bool IsReachComplete(){
		return ((OBJ*)(MIF*)this)->IsReachComplete();
	}
	virtual bool Uphold(){
		return ((OBJ*)(MIF*)this)->Uphold();
	}
	virtual bool IsUpholdable(){
		return ((OBJ*)(MIF*)this)->IsUpholdable();
	}
	virtual bool IsUpholdComplete(){
		return ((OBJ*)(MIF*)this)->IsUpholdComplete();
	}
	virtual bool Place(Vec3d pos){
		return ((OBJ*)(MIF*)this)->Place(pos);
	}
	virtual bool IsPlaceable(Vec3d pos){
		return ((OBJ*)(MIF*)this)->IsPlaceable(pos);
	}
	virtual bool IsPlaceable(Vec3d pos, float safety){
		return ((OBJ*)(MIF*)this)->IsPlaceable(pos, safety);
	}
	virtual bool IsPlaceComplete(){
		return ((OBJ*)(MIF*)this)->IsPlaceComplete();
	}
	virtual void Abort(){
		return ((OBJ*)(MIF*)this)->Abort();
	}
	virtual void AbortAll(){
		return ((OBJ*)(MIF*)this)->AbortAll();
	}
	virtual CRGrabControllerIf::CRGCControlState GetControlState(){
		return ((OBJ*)(MIF*)this)->GetControlState();
	}
};
struct CRGrabControllerIf;	class CRGrabController;
typedef CRGrabControllerIfStubTemplate<CRGrabControllerIf, ObjectIfBuf, CRGrabController>	CRGrabControllerIfStub;
typedef IfInitTemplate<CRGrabControllerIfStub, CRGrabController>	CRGrabControllerIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRCreatureIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
	virtual void ClearInternalScene(){
		return ((OBJ*)(MIF*)this)->ClearInternalScene();
	}
	virtual void SensorStep(){
		return ((OBJ*)(MIF*)this)->SensorStep();
	}
	virtual void InternalSceneStep(){
		return ((OBJ*)(MIF*)this)->InternalSceneStep();
	}
	virtual void ControllerStep(){
		return ((OBJ*)(MIF*)this)->ControllerStep();
	}
	virtual CRBodyIf * CreateBody(const IfInfo * ii, const CRBodyDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateBody(ii, desc);
	}
	virtual CRBodyIf * GetBody(int i){
		return ((OBJ*)(MIF*)this)->GetBody(i);
	}
	virtual int NBodies(){
		return ((OBJ*)(MIF*)this)->NBodies();
	}
	virtual CRSensorIf * CreateSensor(const IfInfo * ii, const CRSensorDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateSensor(ii, desc);
	}
	virtual CRSensorIf * GetSensor(int i){
		return ((OBJ*)(MIF*)this)->GetSensor(i);
	}
	virtual int NSensors(){
		return ((OBJ*)(MIF*)this)->NSensors();
	}
	virtual CRControllerIf * CreateController(const IfInfo * ii, const CRControllerDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateController(ii, desc);
	}
	virtual CRControllerIf * GetController(int i){
		return ((OBJ*)(MIF*)this)->GetController(i);
	}
	virtual int NControllers(){
		return ((OBJ*)(MIF*)this)->NControllers();
	}
	virtual CRInternalSceneIf * CreateInternalScene(const CRInternalSceneDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateInternalScene(desc);
	}
	virtual CRInternalSceneIf * GetInternalScene(){
		return ((OBJ*)(MIF*)this)->GetInternalScene();
	}
};
struct CRCreatureIf;	class CRCreature;
typedef CRCreatureIfStubTemplate<CRCreatureIf, ObjectIfBuf, CRCreature>	CRCreatureIfStub;
typedef IfInitTemplate<CRCreatureIfStub, CRCreature>	CRCreatureIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRInternalSceneObjectIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual bool IsA(const char * typestr){
		return ((OBJ*)(MIF*)this)->IsA(typestr);
	}
	virtual const char * GetISObjType(){
		return ((OBJ*)(MIF*)this)->GetISObjType();
	}
	virtual PHSolidIf * GetSolid(){
		return ((OBJ*)(MIF*)this)->GetSolid();
	}
	virtual Vec3f GetPos(){
		return ((OBJ*)(MIF*)this)->GetPos();
	}
	virtual void SetPos(Vec3d pos){
		return ((OBJ*)(MIF*)this)->SetPos(pos);
	}
};
struct CRInternalSceneObjectIf;	class CRInternalSceneObject;
typedef CRInternalSceneObjectIfStubTemplate<CRInternalSceneObjectIf, ObjectIfBuf, CRInternalSceneObject>	CRInternalSceneObjectIfStub;
typedef IfInitTemplate<CRInternalSceneObjectIfStub, CRInternalSceneObject>	CRInternalSceneObjectIfInit;

template <class IF, class MIF, class OBJ> struct CRInternalSceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRISAttractiveObjectIfStubTemplate: public CRInternalSceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual float GetTotalAttractiveness(){
		return ((OBJ*)(MIF*)this)->GetTotalAttractiveness();
	}
	virtual void AddBottomupAttr(float attr){
		return ((OBJ*)(MIF*)this)->AddBottomupAttr(attr);
	}
	virtual void ClearBottomupAttr(){
		return ((OBJ*)(MIF*)this)->ClearBottomupAttr();
	}
	virtual void SetTopdownAttr(float attr){
		return ((OBJ*)(MIF*)this)->SetTopdownAttr(attr);
	}
	virtual void IncUncertainty(){
		return ((OBJ*)(MIF*)this)->IncUncertainty();
	}
	virtual void DecUncertainty(){
		return ((OBJ*)(MIF*)this)->DecUncertainty();
	}
	virtual float GetUncertainty(){
		return ((OBJ*)(MIF*)this)->GetUncertainty();
	}
};
struct CRISAttractiveObjectIf;	class CRISAttractiveObject;
typedef CRISAttractiveObjectIfStubTemplate<CRISAttractiveObjectIf, ObjectIfBuf, CRISAttractiveObject>	CRISAttractiveObjectIfStub;
typedef IfInitTemplate<CRISAttractiveObjectIfStub, CRISAttractiveObject>	CRISAttractiveObjectIfInit;

template <class IF, class MIF, class OBJ> struct CRInternalSceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRISTravelPotentialObjectIfStubTemplate: public CRInternalSceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual Vec2f GetStrengthCoeff(){
		return ((OBJ*)(MIF*)this)->GetStrengthCoeff();
	}
	virtual void SetStrengthCoeff(Vec2f strength){
		return ((OBJ*)(MIF*)this)->SetStrengthCoeff(strength);
	}
	virtual Vec2f GetDecayCoeff(){
		return ((OBJ*)(MIF*)this)->GetDecayCoeff();
	}
	virtual void SetDecayCoeff(Vec2f decay){
		return ((OBJ*)(MIF*)this)->SetDecayCoeff(decay);
	}
	virtual Vec2f GetPotential(Vec2f currPos){
		return ((OBJ*)(MIF*)this)->GetPotential(currPos);
	}
};
struct CRISTravelPotentialObjectIf;	class CRISTravelPotentialObject;
typedef CRISTravelPotentialObjectIfStubTemplate<CRISTravelPotentialObjectIf, ObjectIfBuf, CRISTravelPotentialObject>	CRISTravelPotentialObjectIfStub;
typedef IfInitTemplate<CRISTravelPotentialObjectIfStub, CRISTravelPotentialObject>	CRISTravelPotentialObjectIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRInternalSceneIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
	virtual void ClearAttractiveness(){
		return ((OBJ*)(MIF*)this)->ClearAttractiveness();
	}
	virtual void SortByAttractiveness(){
		return ((OBJ*)(MIF*)this)->SortByAttractiveness();
	}
	virtual CRInternalSceneObjectIf * FindObject(PHSolidIf * solid, Vec3f pos){
		return ((OBJ*)(MIF*)this)->FindObject(solid, pos);
	}
	virtual CRInternalSceneObjectIf * GetISObject(int i){
		return ((OBJ*)(MIF*)this)->GetISObject(i);
	}
	virtual int NObjects(){
		return ((OBJ*)(MIF*)this)->NObjects();
	}
	virtual CRInternalSceneObjectIf * CreateInternalSceneObject(const IfInfo * ii, const CRInternalSceneObjectDesc & desc){
		return ((OBJ*)(MIF*)this)->CreateInternalSceneObject(ii, desc);
	}
};
struct CRInternalSceneIf;	class CRInternalScene;
typedef CRInternalSceneIfStubTemplate<CRInternalSceneIf, ObjectIfBuf, CRInternalScene>	CRInternalSceneIfStub;
typedef IfInitTemplate<CRInternalSceneIfStub, CRInternalScene>	CRInternalSceneIfInit;

template <class IF, class MIF, class OBJ> struct SceneObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CRSensorIfStubTemplate: public SceneObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual void Step(){
		return ((OBJ*)(MIF*)this)->Step();
	}
};
struct CRSensorIf;	class CRSensor;
typedef CRSensorIfStubTemplate<CRSensorIf, ObjectIfBuf, CRSensor>	CRSensorIfStub;
typedef IfInitTemplate<CRSensorIfStub, CRSensor>	CRSensorIfInit;

template <class IF, class MIF, class OBJ> struct CRSensorIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct CROpticalSensorIfStubTemplate: public CRSensorIfStubTemplate<IF, MIF, OBJ> {
	virtual bool IsVisible(PHSolidIf * solid){
		return ((OBJ*)(MIF*)this)->IsVisible(solid);
	}
	virtual bool IsInCenter(PHSolidIf * solid){
		return ((OBJ*)(MIF*)this)->IsInCenter(solid);
	}
	virtual bool IsVisible(Vec3f pos){
		return ((OBJ*)(MIF*)this)->IsVisible(pos);
	}
	virtual bool IsInCenter(Vec3f pos){
		return ((OBJ*)(MIF*)this)->IsInCenter(pos);
	}
	virtual bool IsSelfSolid(PHSolidIf * solid){
		return ((OBJ*)(MIF*)this)->IsSelfSolid(solid);
	}
};
struct CROpticalSensorIf;	class CROpticalSensor;
typedef CROpticalSensorIfStubTemplate<CROpticalSensorIf, ObjectIfBuf, CROpticalSensor>	CROpticalSensorIfStub;
typedef IfInitTemplate<CROpticalSensorIfStub, CROpticalSensor>	CROpticalSensorIfInit;
}	//	namespace Spr; 
