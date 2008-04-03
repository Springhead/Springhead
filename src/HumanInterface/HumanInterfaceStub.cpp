#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\HumanInterface\SprHIBase.h"
#include "..\..\include\HumanInterface\SprHIDevice.h"
#include "..\..\include\HumanInterface\SprHIDRUsb.h"
#include "..\..\include\HumanInterface\SprHIKeyMouse.h"
#include "..\..\include\HumanInterface\SprHIMouse6D.h"
#include "..\..\include\HumanInterface\SprHISdk.h"
#include "..\..\src\Base\BaseDebug.h"
#include "..\..\src\Foundation\UTTypeDesc.h"
#include "..\Foundation\Foundation.h"
#include "..\Foundation\Object.h"
#include "..\Foundation\Scene.h"
#include "..\Foundation\UTBaseType.h"
#include "..\Foundation\UTDllLoader.h"
#include "..\Foundation\UTDllLoaderImpl.h"
#include "..\Foundation\UTLoadContext.h"
#include "..\Foundation\UTLoadHandler.h"
#include "..\Foundation\UTMMTimer.h"
#include "..\Foundation\UTPath.h"
#include "..\Foundation\UTPreciseTimer.h"
#include "..\Foundation\UTTypeDesc.h"
#include "..\HumanInterface\Device.h"
#include "..\HumanInterface\DRADAPIO.h"
#include "..\HumanInterface\DRContecIsaCounter.h"
#include "..\HumanInterface\DRContecIsaDa.h"
#include "..\HumanInterface\DREzUSB.h"
#include "..\HumanInterface\DRKeyMouseWin32.h"
#include "..\HumanInterface\DRNittaForce.h"
#include "..\HumanInterface\DRTokin3D.h"
#include "..\HumanInterface\DRUPP.h"
#include "..\HumanInterface\DRUsb20Sh4.h"
#include "..\HumanInterface\DRUsb20Simple.h"
#include "..\HumanInterface\DRUsbH8Simple.h"
#include "..\HumanInterface\DRUsbH8SimpleDef.h"
#include "..\HumanInterface\DVADBase.h"
#include "..\HumanInterface\DVCounterBase.h"
#include "..\HumanInterface\DVDABase.h"
#include "..\HumanInterface\DVForceBase.h"
#include "..\HumanInterface\DVKeyMouse.h"
#include "..\HumanInterface\DVOrientationBase.h"
#include "..\HumanInterface\DVPIOBase.h"
#include "..\HumanInterface\HIBase.h"
#include "..\HumanInterface\HIMouse6D.h"
#include "..\HumanInterface\HIRealDevice.h"
#include "..\HumanInterface\HIRealDevicePool.h"
#include "..\HumanInterface\HISdk.h"
#include "..\HumanInterface\HISpidarCalc3DOF.h"
#include "..\HumanInterface\HISPidarCalc6DOF.h"
#include "..\HumanInterface\HISpidarCalcBase.h"
#include "..\HumanInterface\HIVirtualDevice.h"
#include "..\HumanInterface\HIVirtualDevicePool.h"
#include "..\HumanInterface\HumanInterface.h"
#include "..\HumanInterface\TKINPUT.H"
#include "..\HumanInterface\TKINPUTDEF.H"
#include "..\HumanInterface\Tuadapio.h"
namespace Spr{

void RegisterTypeDescHumanInterface(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::DRUsb20SimpleDesc* pDRUsb20SimpleDesc = NULL;
	desc = DBG_NEW UTTypeDesc("DRUsb20SimpleDesc");
	desc->size = sizeof(Spr::DRUsb20SimpleDesc);
	desc->ifInfo = DRUsb20SimpleIf::GetIfInfoStatic();
	((IfInfo*)DRUsb20SimpleIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::DRUsb20SimpleDesc>;
	field = desc->AddField("", "int", "number", "");
	field->offset = int((char*)&(pDRUsb20SimpleDesc->number) - (char*)pDRUsb20SimpleDesc);
	db->RegisterDesc(desc);
	Spr::DVKeyMouseHandler* pDVKeyMouseHandler = NULL;
	desc = DBG_NEW UTTypeDesc("DVKeyMouseHandler");
	desc->size = sizeof(Spr::DVKeyMouseHandler);
	desc->access = DBG_NEW UTAccess<Spr::DVKeyMouseHandler>;
	db->RegisterDesc(desc);
	Spr::HISdkDesc* pHISdkDesc = NULL;
	desc = DBG_NEW UTTypeDesc("HISdkDesc");
	desc->size = sizeof(Spr::HISdkDesc);
	desc->ifInfo = HISdkIf::GetIfInfoStatic();
	((IfInfo*)HISdkIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::HISdkDesc>;
	db->RegisterDesc(desc);
	Spr::BULK_TRANSFER_CONTROL* pBULK_TRANSFER_CONTROL = NULL;
	desc = DBG_NEW UTTypeDesc("BULK_TRANSFER_CONTROL");
	desc->size = sizeof(Spr::BULK_TRANSFER_CONTROL);
	desc->access = DBG_NEW UTAccess<Spr::BULK_TRANSFER_CONTROL>;
	field = desc->AddField("", "unsigned long", "pipeNum", "");
	field->offset = int((char*)&(pBULK_TRANSFER_CONTROL->pipeNum) - (char*)pBULK_TRANSFER_CONTROL);
	db->RegisterDesc(desc);
	Spr::USB_DEVICE_DESCRIPTOR* pUSB_DEVICE_DESCRIPTOR = NULL;
	desc = DBG_NEW UTTypeDesc("USB_DEVICE_DESCRIPTOR");
	desc->size = sizeof(Spr::USB_DEVICE_DESCRIPTOR);
	desc->access = DBG_NEW UTAccess<Spr::USB_DEVICE_DESCRIPTOR>;
	field = desc->AddField("", "unsigned char", "bLength", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bLength) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "bDescriptorType", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bDescriptorType) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned short", "bcdUSB", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bcdUSB) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "bDeviceClass", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bDeviceClass) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "bDeviceSubClass", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bDeviceSubClass) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "bDeviceProtocol", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bDeviceProtocol) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "bMaxPacketSize0", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bMaxPacketSize0) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned short", "idVendor", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->idVendor) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned short", "idProduct", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->idProduct) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned short", "bcdDevice", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bcdDevice) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "iManufacturer", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->iManufacturer) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "iProduct", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->iProduct) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "iSerialNumber", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->iSerialNumber) - (char*)pUSB_DEVICE_DESCRIPTOR);
	field = desc->AddField("", "unsigned char", "bNumConfigurations", "");
	field->offset = int((char*)&(pUSB_DEVICE_DESCRIPTOR->bNumConfigurations) - (char*)pUSB_DEVICE_DESCRIPTOR);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(HIBase, NamedObject);
bool Spr::HIBaseIf::BeforeCalibration(){
	return	((HIBase*)(Object*)(ObjectIf*)this)->BeforeCalibration();
}
bool Spr::HIBaseIf::Calibration(){
	return	((HIBase*)(Object*)(ObjectIf*)this)->Calibration();
}
bool Spr::HIBaseIf::AfterCalibration(){
	return	((HIBase*)(Object*)(ObjectIf*)this)->AfterCalibration();
}
void Spr::HIBaseIf::AddDeviceDependency(HIRealDeviceIf* rd){
	((HIBase*)(Object*)(ObjectIf*)this)->AddDeviceDependency(rd);
}
void Spr::HIBaseIf::ClearDeviceDependency(){
	((HIBase*)(Object*)(ObjectIf*)this)->ClearDeviceDependency();
}
void Spr::HIBaseIf::Update(float dt){
	((HIBase*)(Object*)(ObjectIf*)this)->Update(dt);
}
bool Spr::HIBaseIf::IsGood(){
	return	((HIBase*)(Object*)(ObjectIf*)this)->IsGood();
}
bool Spr::HIBaseIf::Init(HISdkIf* sdk, const void* desc){
	return	((HIBase*)(Object*)(ObjectIf*)this)->Init(sdk, desc);
}
SPR_IFIMP1(HIPosition, HIBase);
Vec3f Spr::HIPositionIf::GetPosition(){
	return	((HIPosition*)(Object*)(ObjectIf*)this)->GetPosition();
}
SPR_IFIMP1(HIOrientation, HIBase);
Quaternionf Spr::HIOrientationIf::GetOrientation(){
	return	((HIOrientation*)(Object*)(ObjectIf*)this)->GetOrientation();
}
SPR_IFIMP1(HIPose, HIPosition);
Quaternionf Spr::HIPoseIf::GetOrientation(){
	return	((HIPose*)(Object*)(ObjectIf*)this)->GetOrientation();
}
Posef Spr::HIPoseIf::GetPose(){
	return	((HIPose*)(Object*)(ObjectIf*)this)->GetPose();
}
SPR_IFIMP1(HIRealDevice, Object);
const char* Spr::HIRealDeviceIf::Name(){
	return	((HIRealDevice*)(Object*)(ObjectIf*)this)->Name();
}
bool Spr::HIRealDeviceIf::Init(){
	return	((HIRealDevice*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::HIRealDeviceIf::Register(Spr::HISdkIf* sdk){
	((HIRealDevice*)(Object*)(ObjectIf*)this)->Register(sdk);
}
void Spr::HIRealDeviceIf::Update(){
	((HIRealDevice*)(Object*)(ObjectIf*)this)->Update();
}
SPR_IFIMP1(HIVirtualDevice, Object);
const char* Spr::HIVirtualDeviceIf::Name()const{
	return	((HIVirtualDevice*)(Object*)(ObjectIf*)this)->Name();
}
const char* Spr::HIVirtualDeviceIf::Type()const{
	return	((HIVirtualDevice*)(Object*)(ObjectIf*)this)->Type();
}
Spr::HIRealDeviceIf* Spr::HIVirtualDeviceIf::RealDevice(){
	return	((HIVirtualDevice*)(Object*)(ObjectIf*)this)->RealDevice();
}
void Spr::HIVirtualDeviceIf::Print(std::ostream& o)const{
	((HIVirtualDevice*)(Object*)(ObjectIf*)this)->Print(o);
}
void Spr::HIVirtualDeviceIf::Update(){
	((HIVirtualDevice*)(Object*)(ObjectIf*)this)->Update();
}
SPR_IFIMP1(DRUsb20Simple, HIRealDevice);
SPR_IFIMP1(DVKeyMouse, HIVirtualDevice);
const char* Spr::DVKeyMouseIf::Name()const{
	return	((DVKeyMouse*)(Object*)(ObjectIf*)this)->Name();
}
void Spr::DVKeyMouseIf::SetHandler(Spr::DVKeyMouseHandler* h){
	((DVKeyMouse*)(Object*)(ObjectIf*)this)->SetHandler(h);
}
int Spr::DVKeyMouseIf::GetKeyState(int key){
	return	((DVKeyMouse*)(Object*)(ObjectIf*)this)->GetKeyState(key);
}
Spr::DVKeyMouseIf::DVMousePosition Spr::DVKeyMouseIf::GetMousePosition(int count){
	return	((DVKeyMouse*)(Object*)(ObjectIf*)this)->GetMousePosition(count);
}
SPR_IFIMP1(DRKeyMouseWin32, HIRealDevice);
SPR_IFIMP1(HIMouse6D, HIPose);
Spr::DVKeyMouseIf* Spr::HIMouse6DIf::GetKeyMouse(){
	return	((HIMouse6D*)(Object*)(ObjectIf*)this)->GetKeyMouse();
}
SPR_IFIMP1(HISdk, NameManager);
void Spr::HISdkIf::Init(){
	((HISdk*)(Object*)(ObjectIf*)this)->Init();
}
void Spr::HISdkIf::Clear(){
	((HISdk*)(Object*)(ObjectIf*)this)->Clear();
}
Spr::HIVirtualDeviceIf* Spr::HISdkIf::RentVirtualDevice(const char* type, const char* name){
	return	((HISdk*)(Object*)(ObjectIf*)this)->RentVirtualDevice(type, name);
}
bool Spr::HISdkIf::ReturnVirtualDevice(Spr::HIVirtualDeviceIf* dev){
	return	((HISdk*)(Object*)(ObjectIf*)this)->ReturnVirtualDevice(dev);
}
bool Spr::HISdkIf::AddRealDevice(const IfInfo* keyInfo, const void* desc){
	return	((HISdk*)(Object*)(ObjectIf*)this)->AddRealDevice(keyInfo, desc);
}
Spr::HIRealDeviceIf* Spr::HISdkIf::FindRealDevice(const char* name){
	return	((HISdk*)(Object*)(ObjectIf*)this)->FindRealDevice(name);
}
UTRef< Spr::HIBaseIf > Spr::HISdkIf::CreateHumanInterface(const IfInfo* info, const void* desc){
	return	((HISdk*)(Object*)(ObjectIf*)this)->CreateHumanInterface(info, desc);
}
UTRef< Spr::HIBaseIf > Spr::HISdkIf::CreateHumanInterface(const char* name, const char* desc){
	return	((HISdk*)(Object*)(ObjectIf*)this)->CreateHumanInterface(name, desc);
}
SPR_OBJECTIMP1(DRKeyMouseWin32, HIRealDevice);
SPR_OBJECTIMP1(DRUsb20Simple, HIRealDevice);
SPR_OBJECTIMP_ABST1(DVKeyMouse, HIVirtualDevice);
SPR_OBJECTIMP_ABST1(HIBase, NamedObject);
SPR_OBJECTIMP1(HIPosition, HIBase);
SPR_OBJECTIMP1(HIOrientation, HIBase);
SPR_OBJECTIMP2(HIPose, HIPosition, HIOrientation);
SPR_OBJECTIMP1(HIMouse6D, HIPose);
SPR_OBJECTIMP_ABST1(HIRealDevice, Object);
SPR_OBJECTIMP1(HISdk, Sdk);
SPR_OBJECTIMP_ABST1(HIVirtualDevice, Object);
}
