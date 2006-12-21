//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIBaseIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual bool BeforeCalibration(){
		return ((OBJ*)(MIF*)this)->OBJ::BeforeCalibration();
	}
	virtual bool Calibration(){
		return ((OBJ*)(MIF*)this)->OBJ::Calibration();
	}
	virtual bool AfterCalibration(){
		return ((OBJ*)(MIF*)this)->OBJ::AfterCalibration();
	}
	virtual void AddDeviceDependency(HIRealDeviceIf *  rd){
		return ((OBJ*)(MIF*)this)->OBJ::AddDeviceDependency(rd);
	}
	virtual void ClearDeviceDependency(){
		return ((OBJ*)(MIF*)this)->OBJ::ClearDeviceDependency();
	}
	virtual void Update(float dt){
		return ((OBJ*)(MIF*)this)->OBJ::Update(dt);
	}
	virtual bool IsGood(){
		return ((OBJ*)(MIF*)this)->OBJ::IsGood();
	}
};
struct HIBaseIf;	class HIBase;
typedef HIBaseIfStubTemplate<HIBaseIf, ObjectIfBuf, HIBase>	HIBaseIfStub;
typedef IfInitTemplate<HIBaseIfStub, HIBase>	HIBaseIfInit;

template <class IF, class MIF, class OBJ> struct HIBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIPositionIfStubTemplate: public HIBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual Vec3f GetPosition(){
		return ((OBJ*)(MIF*)this)->OBJ::GetPosition();
	}
};
struct HIPositionIf;	class HIPosition;
typedef HIPositionIfStubTemplate<HIPositionIf, ObjectIfBuf, HIPosition>	HIPositionIfStub;
typedef IfInitTemplate<HIPositionIfStub, HIPosition>	HIPositionIfInit;

template <class IF, class MIF, class OBJ> struct HIBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIOrientationIfStubTemplate: public HIBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual Quaternionf GetOrientation(){
		return ((OBJ*)(MIF*)this)->OBJ::GetOrientation();
	}
};
struct HIOrientationIf;	class HIOrientation;
typedef HIOrientationIfStubTemplate<HIOrientationIf, ObjectIfBuf, HIOrientation>	HIOrientationIfStub;
typedef IfInitTemplate<HIOrientationIfStub, HIOrientation>	HIOrientationIfInit;

template <class IF, class MIF, class OBJ> struct HIPositionIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIPoseIfStubTemplate: public HIPositionIfStubTemplate<IF, MIF, OBJ> {
	virtual Quaternionf GetOrientation(){
		return ((OBJ*)(MIF*)this)->OBJ::GetOrientation();
	}
	virtual Posef GetPose(){
		return ((OBJ*)(MIF*)this)->OBJ::GetPose();
	}
};
struct HIPoseIf;	class HIPose;
typedef HIPoseIfStubTemplate<HIPoseIf, ObjectIfBuf, HIPose>	HIPoseIfStub;
typedef IfInitTemplate<HIPoseIfStub, HIPose>	HIPoseIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIRealDeviceIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual const char *  Name(){
		return ((OBJ*)(MIF*)this)->OBJ::Name();
	}
	virtual bool Init(){
		return ((OBJ*)(MIF*)this)->OBJ::Init();
	}
	virtual void Register(HIDeviceManagerIf *  devMan){
		return ((OBJ*)(MIF*)this)->OBJ::Register(devMan);
	}
	virtual void Update(){
		return ((OBJ*)(MIF*)this)->OBJ::Update();
	}
};
struct HIRealDeviceIf;	class HIRealDevice;
typedef HIRealDeviceIfStubTemplate<HIRealDeviceIf, ObjectIfBuf, HIRealDevice>	HIRealDeviceIfStub;
typedef IfInitTemplate<HIRealDeviceIfStub, HIRealDevice>	HIRealDeviceIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIVirtualDeviceIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual const char *  Name() const {
		return ((OBJ*)(MIF*)this)->OBJ::Name();
	}
	virtual const char *  Type() const {
		return ((OBJ*)(MIF*)this)->OBJ::Type();
	}
	virtual HIRealDeviceIf *  RealDevice(){
		return ((OBJ*)(MIF*)this)->OBJ::RealDevice();
	}
	virtual void Print(std::ostream &  o) const {
		return ((OBJ*)(MIF*)this)->OBJ::Print(o);
	}
	virtual void Update(){
		return ((OBJ*)(MIF*)this)->OBJ::Update();
	}
};
struct HIVirtualDeviceIf;	class HIVirtualDevice;
typedef HIVirtualDeviceIfStubTemplate<HIVirtualDeviceIf, ObjectIfBuf, HIVirtualDevice>	HIVirtualDeviceIfStub;
typedef IfInitTemplate<HIVirtualDeviceIfStub, HIVirtualDevice>	HIVirtualDeviceIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIDeviceManagerIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->OBJ::Init();
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->OBJ::Clear();
	}
	virtual HIVirtualDeviceIf *  Rent(const char *  type, const char *  name = NULL){
		return ((OBJ*)(MIF*)this)->OBJ::Rent(type, name);
	}
	virtual bool Return(HIVirtualDeviceIf *  dev){
		return ((OBJ*)(MIF*)this)->OBJ::Return(dev);
	}
	virtual void RegisterRealDevice(HIRealDeviceIf *  dev){
		return ((OBJ*)(MIF*)this)->OBJ::RegisterRealDevice(dev);
	}
	virtual HIRealDeviceIf *  FindRealDevice(const char *  name = NULL){
		return ((OBJ*)(MIF*)this)->OBJ::FindRealDevice(name);
	}
};
struct HIDeviceManagerIf;	class HIDeviceManager;
typedef HIDeviceManagerIfStubTemplate<HIDeviceManagerIf, ObjectIfBuf, HIDeviceManager>	HIDeviceManagerIfStub;
typedef IfInitTemplate<HIDeviceManagerIfStub, HIDeviceManager>	HIDeviceManagerIfInit;

template <class IF, class MIF, class OBJ> struct NameManagerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HISdkIfStubTemplate: public NameManagerIfStubTemplate<IF, MIF, OBJ> {
	virtual HIBaseIf *  CreateHumanInterface(const char *  name){
		return ((OBJ*)(MIF*)this)->OBJ::CreateHumanInterface(name);
	}
	virtual void RegisterDevice(const char *  name){
		return ((OBJ*)(MIF*)this)->OBJ::RegisterDevice(name);
	}
};
struct HISdkIf;	class HISdk;
typedef HISdkIfStubTemplate<HISdkIf, ObjectIfBuf, HISdk>	HISdkIfStub;
typedef IfInitTemplate<HISdkIfStub, HISdk>	HISdkIfInit;
}	//	namespace Spr; 
