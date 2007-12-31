//	Do not edit. MakeTypeDesc.bat will update this file.
namespace Spr{; 

template <class IF, class MIF, class OBJ> struct NamedObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIBaseIfStubTemplate: public NamedObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual bool BeforeCalibration(){
		return ((OBJ*)(MIF*)this)->BeforeCalibration();
	}
	virtual bool Calibration(){
		return ((OBJ*)(MIF*)this)->Calibration();
	}
	virtual bool AfterCalibration(){
		return ((OBJ*)(MIF*)this)->AfterCalibration();
	}
	virtual void AddDeviceDependency(HIRealDeviceIf * rd){
		return ((OBJ*)(MIF*)this)->AddDeviceDependency(rd);
	}
	virtual void ClearDeviceDependency(){
		return ((OBJ*)(MIF*)this)->ClearDeviceDependency();
	}
	virtual void Update(float dt){
		return ((OBJ*)(MIF*)this)->Update(dt);
	}
	virtual bool IsGood(){
		return ((OBJ*)(MIF*)this)->IsGood();
	}
	virtual bool Init(HISdkIf * sdk, const void * desc){
		return ((OBJ*)(MIF*)this)->Init(sdk, desc);
	}
};
struct HIBaseIf;	class HIBase;
typedef HIBaseIfStubTemplate<HIBaseIf, ObjectIfBuf, HIBase>	HIBaseIfStub;
typedef IfInitTemplate<HIBaseIfStub, HIBase>	HIBaseIfInit;

template <class IF, class MIF, class OBJ> struct HIBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIPositionIfStubTemplate: public HIBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual Vec3f GetPosition(){
		return ((OBJ*)(MIF*)this)->GetPosition();
	}
};
struct HIPositionIf;	class HIPosition;
typedef HIPositionIfStubTemplate<HIPositionIf, ObjectIfBuf, HIPosition>	HIPositionIfStub;
typedef IfInitTemplate<HIPositionIfStub, HIPosition>	HIPositionIfInit;

template <class IF, class MIF, class OBJ> struct HIBaseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIOrientationIfStubTemplate: public HIBaseIfStubTemplate<IF, MIF, OBJ> {
	virtual Quaternionf GetOrientation(){
		return ((OBJ*)(MIF*)this)->GetOrientation();
	}
};
struct HIOrientationIf;	class HIOrientation;
typedef HIOrientationIfStubTemplate<HIOrientationIf, ObjectIfBuf, HIOrientation>	HIOrientationIfStub;
typedef IfInitTemplate<HIOrientationIfStub, HIOrientation>	HIOrientationIfInit;

template <class IF, class MIF, class OBJ> struct HIPositionIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIPoseIfStubTemplate: public HIPositionIfStubTemplate<IF, MIF, OBJ> {
	virtual Quaternionf GetOrientation(){
		return ((OBJ*)(MIF*)this)->GetOrientation();
	}
	virtual Posef GetPose(){
		return ((OBJ*)(MIF*)this)->GetPose();
	}
};
struct HIPoseIf;	class HIPose;
typedef HIPoseIfStubTemplate<HIPoseIf, ObjectIfBuf, HIPose>	HIPoseIfStub;
typedef IfInitTemplate<HIPoseIfStub, HIPose>	HIPoseIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIRealDeviceIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual const char * Name(){
		return ((OBJ*)(MIF*)this)->Name();
	}
	virtual bool Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual void Register(HISdkIf * sdk){
		return ((OBJ*)(MIF*)this)->Register(sdk);
	}
	virtual void Update(){
		return ((OBJ*)(MIF*)this)->Update();
	}
};
struct HIRealDeviceIf;	class HIRealDevice;
typedef HIRealDeviceIfStubTemplate<HIRealDeviceIf, ObjectIfBuf, HIRealDevice>	HIRealDeviceIfStub;
typedef IfInitTemplate<HIRealDeviceIfStub, HIRealDevice>	HIRealDeviceIfInit;

template <class IF, class MIF, class OBJ> struct ObjectIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIVirtualDeviceIfStubTemplate: public ObjectIfStubTemplate<IF, MIF, OBJ> {
	virtual const char * Name() const {
		return ((OBJ*)(MIF*)this)->Name();
	}
	virtual const char * Type() const {
		return ((OBJ*)(MIF*)this)->Type();
	}
	virtual HIRealDeviceIf * RealDevice(){
		return ((OBJ*)(MIF*)this)->RealDevice();
	}
	virtual void Print(std::ostream & o) const {
		return ((OBJ*)(MIF*)this)->Print(o);
	}
	virtual void Update(){
		return ((OBJ*)(MIF*)this)->Update();
	}
};
struct HIVirtualDeviceIf;	class HIVirtualDevice;
typedef HIVirtualDeviceIfStubTemplate<HIVirtualDeviceIf, ObjectIfBuf, HIVirtualDevice>	HIVirtualDeviceIfStub;
typedef IfInitTemplate<HIVirtualDeviceIfStub, HIVirtualDevice>	HIVirtualDeviceIfInit;

template <class IF, class MIF, class OBJ> struct HIRealDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct DRUsb20SimpleIfStubTemplate: public HIRealDeviceIfStubTemplate<IF, MIF, OBJ> {
};
struct DRUsb20SimpleIf;	class DRUsb20Simple;
typedef DRUsb20SimpleIfStubTemplate<DRUsb20SimpleIf, ObjectIfBuf, DRUsb20Simple>	DRUsb20SimpleIfStub;
typedef IfInitTemplate<DRUsb20SimpleIfStub, DRUsb20Simple>	DRUsb20SimpleIfInit;

template <class IF, class MIF, class OBJ> struct HIVirtualDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct DVKeyMouseIfStubTemplate: public HIVirtualDeviceIfStubTemplate<IF, MIF, OBJ> {
	virtual const char * Name() const {
		return ((OBJ*)(MIF*)this)->Name();
	}
	virtual void SetHandler(DVKeyMouseHandler * h){
		return ((OBJ*)(MIF*)this)->SetHandler(h);
	}
	virtual int GetKeyState(int key){
		return ((OBJ*)(MIF*)this)->GetKeyState(key);
	}
	virtual DVKeyMouseIf::DVMousePosition GetMousePosition(int count = 0){
		return ((OBJ*)(MIF*)this)->GetMousePosition(count);
	}
};
struct DVKeyMouseIf;	class DVKeyMouse;
typedef DVKeyMouseIfStubTemplate<DVKeyMouseIf, ObjectIfBuf, DVKeyMouse>	DVKeyMouseIfStub;
typedef IfInitTemplate<DVKeyMouseIfStub, DVKeyMouse>	DVKeyMouseIfInit;

template <class IF, class MIF, class OBJ> struct HIRealDeviceIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct DRKeyMouseWin32IfStubTemplate: public HIRealDeviceIfStubTemplate<IF, MIF, OBJ> {
};
struct DRKeyMouseWin32If;	class DRKeyMouseWin32;
typedef DRKeyMouseWin32IfStubTemplate<DRKeyMouseWin32If, ObjectIfBuf, DRKeyMouseWin32>	DRKeyMouseWin32IfStub;
typedef IfInitTemplate<DRKeyMouseWin32IfStub, DRKeyMouseWin32>	DRKeyMouseWin32IfInit;

template <class IF, class MIF, class OBJ> struct HIPoseIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HIMouse6DIfStubTemplate: public HIPoseIfStubTemplate<IF, MIF, OBJ> {
	virtual DVKeyMouseIf * GetKeyMouse(){
		return ((OBJ*)(MIF*)this)->GetKeyMouse();
	}
};
struct HIMouse6DIf;	class HIMouse6D;
typedef HIMouse6DIfStubTemplate<HIMouse6DIf, ObjectIfBuf, HIMouse6D>	HIMouse6DIfStub;
typedef IfInitTemplate<HIMouse6DIfStub, HIMouse6D>	HIMouse6DIfInit;

template <class IF, class MIF, class OBJ> struct NameManagerIfStubTemplate;
template <class IF, class MIF, class OBJ>
struct HISdkIfStubTemplate: public NameManagerIfStubTemplate<IF, MIF, OBJ> {
	virtual void Init(){
		return ((OBJ*)(MIF*)this)->Init();
	}
	virtual void Clear(){
		return ((OBJ*)(MIF*)this)->Clear();
	}
	virtual HIVirtualDeviceIf * RentVirtualDevice(const char * type, const char * name = NULL){
		return ((OBJ*)(MIF*)this)->RentVirtualDevice(type, name);
	}
	virtual bool ReturnVirtualDevice(HIVirtualDeviceIf * dev){
		return ((OBJ*)(MIF*)this)->ReturnVirtualDevice(dev);
	}
	virtual bool AddRealDevice(const IfInfo * keyInfo, const void * desc = NULL){
		return ((OBJ*)(MIF*)this)->AddRealDevice(keyInfo, desc);
	}
	virtual HIRealDeviceIf * FindRealDevice(const char * name = NULL){
		return ((OBJ*)(MIF*)this)->FindRealDevice(name);
	}
	virtual UTRef <HIBaseIf> CreateHumanInterface(const IfInfo * info, const void * desc){
		return ((OBJ*)(MIF*)this)->CreateHumanInterface(info, desc);
	}
	virtual UTRef <HIBaseIf> CreateHumanInterface(const char * name, const char * desc){
		return ((OBJ*)(MIF*)this)->CreateHumanInterface(name, desc);
	}
};
struct HISdkIf;	class HISdk;
typedef HISdkIfStubTemplate<HISdkIf, ObjectIfBuf, HISdk>	HISdkIfStub;
typedef IfInitTemplate<HISdkIfStub, HISdk>	HISdkIfInit;
}	//	namespace Spr; 
