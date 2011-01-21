#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Foundation\UTTimer.h"
#include "..\..\include\Collision\SprCDShape.h"
#include "..\..\include\Base\BaseDebug.h"
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
#include "..\Foundation\UTQPTimer.h"
#include "..\Foundation\UTTypeDesc.h"
#include "..\Collision\CDBox.h"
#include "..\Collision\CDCapsule.h"
#include "..\Collision\CDConvex.h"
#include "..\Collision\CDConvexMesh.h"
#include "..\Collision\CDConvexMeshInterpolate.h"
#include "..\Collision\CDCutRing.h"
#include "..\Collision\CDDetectorImp.h"
#include "..\Collision\CDQuickHull2D.h"
#include "..\Collision\CDQuickHull2DImp.h"
#include "..\Collision\CDQuickHull3D.h"
#include "..\Collision\CDQuickHull3DImp.h"
#include "..\Collision\CDRoundCone.h"
#include "..\Collision\CDShape.h"
#include "..\Collision\CDSphere.h"
#include "..\Collision\Collision.h"
namespace Spr{

void RegisterTypeDescCollision(UTTypeDescDb* db){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDesc* desc;
	UTTypeDesc::Field* field;
	Spr::PHMaterial* pPHMaterial = NULL;
	desc = DBG_NEW UTTypeDesc("PHMaterial");
	desc->size = sizeof(Spr::PHMaterial);
	desc->access = DBG_NEW UTAccess<Spr::PHMaterial>;
	field = desc->AddField("", "float", "density", "");
	field->offset = int((char*)&(pPHMaterial->density) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "mu", "");
	field->offset = int((char*)&(pPHMaterial->mu) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "mu0", "");
	field->offset = int((char*)&(pPHMaterial->mu0) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "e", "");
	field->offset = int((char*)&(pPHMaterial->e) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "reflexSpringK", "");
	field->offset = int((char*)&(pPHMaterial->reflexSpringK) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "reflexDamperD", "");
	field->offset = int((char*)&(pPHMaterial->reflexDamperD) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "frictionSpringK", "");
	field->offset = int((char*)&(pPHMaterial->frictionSpringK) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "frictionDamperD", "");
	field->offset = int((char*)&(pPHMaterial->frictionDamperD) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "vibA", "");
	field->offset = int((char*)&(pPHMaterial->vibA) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "vibB", "");
	field->offset = int((char*)&(pPHMaterial->vibB) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "vibW", "");
	field->offset = int((char*)&(pPHMaterial->vibW) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "vibT", "");
	field->offset = int((char*)&(pPHMaterial->vibT) - (char*)pPHMaterial);
	field = desc->AddField("", "bool", "vibContact", "");
	field->offset = int((char*)&(pPHMaterial->vibContact) - (char*)pPHMaterial);
	db->RegisterDesc(desc);
	Spr::CDShapeDesc* pCDShapeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDShapeDesc");
	desc->size = sizeof(Spr::CDShapeDesc);
	desc->ifInfo = CDShapeIf::GetIfInfoStatic();
	((IfInfo*)CDShapeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDShapeDesc>;
	field = desc->AddField("", "PHMaterial", "material", "");
	field->offset = int((char*)&(pCDShapeDesc->material) - (char*)pCDShapeDesc);
	db->RegisterDesc(desc);
	Spr::CDConvexMeshDesc* pCDConvexMeshDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDConvexMeshDesc");
	desc->size = sizeof(Spr::CDConvexMeshDesc);
	desc->ifInfo = CDConvexMeshIf::GetIfInfoStatic();
	((IfInfo*)CDConvexMeshIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDConvexMeshDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = int((char*)(CDShapeDesc*)pCDConvexMeshDesc - (char*)pCDConvexMeshDesc);
	field = desc->AddField("vector", "Vec3f", "vertices", "");
	field->offset = int((char*)&(pCDConvexMeshDesc->vertices) - (char*)pCDConvexMeshDesc);
	db->RegisterDesc(desc);
	Spr::CDConvexMeshInterpolateDesc* pCDConvexMeshInterpolateDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDConvexMeshInterpolateDesc");
	desc->size = sizeof(Spr::CDConvexMeshInterpolateDesc);
	desc->ifInfo = CDConvexMeshInterpolateIf::GetIfInfoStatic();
	((IfInfo*)CDConvexMeshInterpolateIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDConvexMeshInterpolateDesc>;
	field = desc->AddBase("CDConvexMeshDesc");
	field->offset = int((char*)(CDConvexMeshDesc*)pCDConvexMeshInterpolateDesc - (char*)pCDConvexMeshInterpolateDesc);
	db->RegisterDesc(desc);
	Spr::CDSphereDesc* pCDSphereDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDSphereDesc");
	desc->size = sizeof(Spr::CDSphereDesc);
	desc->ifInfo = CDSphereIf::GetIfInfoStatic();
	((IfInfo*)CDSphereIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDSphereDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = int((char*)(CDShapeDesc*)pCDSphereDesc - (char*)pCDSphereDesc);
	field = desc->AddField("", "float", "radius", "");
	field->offset = int((char*)&(pCDSphereDesc->radius) - (char*)pCDSphereDesc);
	db->RegisterDesc(desc);
	Spr::CDCapsuleDesc* pCDCapsuleDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDCapsuleDesc");
	desc->size = sizeof(Spr::CDCapsuleDesc);
	desc->ifInfo = CDCapsuleIf::GetIfInfoStatic();
	((IfInfo*)CDCapsuleIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDCapsuleDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = int((char*)(CDShapeDesc*)pCDCapsuleDesc - (char*)pCDCapsuleDesc);
	field = desc->AddField("", "float", "radius", "");
	field->offset = int((char*)&(pCDCapsuleDesc->radius) - (char*)pCDCapsuleDesc);
	field = desc->AddField("", "float", "length", "");
	field->offset = int((char*)&(pCDCapsuleDesc->length) - (char*)pCDCapsuleDesc);
	db->RegisterDesc(desc);
	Spr::CDRoundConeDesc* pCDRoundConeDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDRoundConeDesc");
	desc->size = sizeof(Spr::CDRoundConeDesc);
	desc->ifInfo = CDRoundConeIf::GetIfInfoStatic();
	((IfInfo*)CDRoundConeIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDRoundConeDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = int((char*)(CDShapeDesc*)pCDRoundConeDesc - (char*)pCDRoundConeDesc);
	field = desc->AddField("", "Vec2f", "radius", "");
	field->offset = int((char*)&(pCDRoundConeDesc->radius) - (char*)pCDRoundConeDesc);
	field = desc->AddField("", "float", "length", "");
	field->offset = int((char*)&(pCDRoundConeDesc->length) - (char*)pCDRoundConeDesc);
	db->RegisterDesc(desc);
	Spr::CDBoxDesc* pCDBoxDesc = NULL;
	desc = DBG_NEW UTTypeDesc("CDBoxDesc");
	desc->size = sizeof(Spr::CDBoxDesc);
	desc->ifInfo = CDBoxIf::GetIfInfoStatic();
	((IfInfo*)CDBoxIf::GetIfInfoStatic())->desc = desc;
	desc->access = DBG_NEW UTAccess<Spr::CDBoxDesc>;
	field = desc->AddBase("CDShapeDesc");
	field->offset = int((char*)(CDShapeDesc*)pCDBoxDesc - (char*)pCDBoxDesc);
	field = desc->AddField("", "Vec3f", "boxsize", "");
	field->offset = int((char*)&(pCDBoxDesc->boxsize) - (char*)pCDBoxDesc);
	db->RegisterDesc(desc);
	Spr::CDShapePairSt* pCDShapePairSt = NULL;
	desc = DBG_NEW UTTypeDesc("CDShapePairSt");
	desc->size = sizeof(Spr::CDShapePairSt);
	desc->access = DBG_NEW UTAccess<Spr::CDShapePairSt>;
	field = desc->AddField("", "Vec3d", "normal", "");
	field->offset = int((char*)&(pCDShapePairSt->normal) - (char*)pCDShapePairSt);
	field = desc->AddField("", "double", "depth", "");
	field->offset = int((char*)&(pCDShapePairSt->depth) - (char*)pCDShapePairSt);
	field = desc->AddField("", "unsigned", "lastContactCount", "");
	field->offset = int((char*)&(pCDShapePairSt->lastContactCount) - (char*)pCDShapePairSt);
	db->RegisterDesc(desc);
}

SPR_IFIMP1(CDShape, NamedObject);
void Spr::CDShapeIf::SetStaticFriction(float mu0){
	((CDShape*)(Object*)(ObjectIf*)this)->SetStaticFriction(mu0);
}
float Spr::CDShapeIf::GetStaticFriction(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetStaticFriction();
}
void Spr::CDShapeIf::SetDynamicFriction(float mu){
	((CDShape*)(Object*)(ObjectIf*)this)->SetDynamicFriction(mu);
}
float Spr::CDShapeIf::GetDynamicFriction(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetDynamicFriction();
}
void Spr::CDShapeIf::SetElasticity(float e){
	((CDShape*)(Object*)(ObjectIf*)this)->SetElasticity(e);
}
float Spr::CDShapeIf::GetElasticity(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetElasticity();
}
void Spr::CDShapeIf::SetDensity(float d){
	((CDShape*)(Object*)(ObjectIf*)this)->SetDensity(d);
}
float Spr::CDShapeIf::GetDensity(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetDensity();
}
void Spr::CDShapeIf::SetVibration(float vibA, float vibB, float vibW){
	((CDShape*)(Object*)(ObjectIf*)this)->SetVibration(vibA, vibB, vibW);
}
void Spr::CDShapeIf::SetVibA(float vibA){
	((CDShape*)(Object*)(ObjectIf*)this)->SetVibA(vibA);
}
float Spr::CDShapeIf::GetVibA(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetVibA();
}
void Spr::CDShapeIf::SetVibB(float vibB){
	((CDShape*)(Object*)(ObjectIf*)this)->SetVibB(vibB);
}
float Spr::CDShapeIf::GetVibB(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetVibB();
}
void Spr::CDShapeIf::SetVibW(float vibW){
	((CDShape*)(Object*)(ObjectIf*)this)->SetVibW(vibW);
}
float Spr::CDShapeIf::GetVibW(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetVibW();
}
void Spr::CDShapeIf::SetVibT(float vibT){
	((CDShape*)(Object*)(ObjectIf*)this)->SetVibT(vibT);
}
float Spr::CDShapeIf::GetVibT(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetVibT();
}
void Spr::CDShapeIf::SetVibContact(bool vibContact){
	((CDShape*)(Object*)(ObjectIf*)this)->SetVibContact(vibContact);
}
bool Spr::CDShapeIf::GetVibContact(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetVibContact();
}
void Spr::CDShapeIf::SetSpringDamper(float spring, float damper){
	((CDShape*)(Object*)(ObjectIf*)this)->SetSpringDamper(spring, damper);
}
float Spr::CDShapeIf::GetSpringK(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetSpringK();
}
float Spr::CDShapeIf::GetDamperD(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetDamperD();
}
float Spr::CDShapeIf::CalcVolume(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->CalcVolume();
}
Vec3f Spr::CDShapeIf::CalcCenterOfMass(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->CalcCenterOfMass();
}
Matrix3f Spr::CDShapeIf::CalcMomentOfInertia(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->CalcMomentOfInertia();
}
Spr::PHMaterial Spr::CDShapeIf::GetMaterial(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->GetMaterial();
}
void Spr::CDShapeIf::SetMaterial(Spr::PHMaterial mat){
	((CDShape*)(Object*)(ObjectIf*)this)->SetMaterial(mat);
}
SPR_IFIMP1(CDShapePair, Object);
Spr::CDShapeIf* Spr::CDShapePairIf::GetShape(int i){
	return	((CDShapePair*)(Object*)(ObjectIf*)this)->GetShape(i);
}
SPR_IFIMP1(CDConvex, CDShape);
double Spr::CDConvexIf::CurvatureRadius(Vec3d p){
	return	((CDConvex*)(Object*)(ObjectIf*)this)->CurvatureRadius(p);
}
Vec3d Spr::CDConvexIf::Normal(Vec3d p){
	return	((CDConvex*)(Object*)(ObjectIf*)this)->Normal(p);
}
SPR_IFIMP1(CDFace, Object);
int Spr::CDFaceIf::NIndex(){
	return	((CDFace*)(Object*)(ObjectIf*)this)->NIndex();
}
int* Spr::CDFaceIf::GetIndices(){
	return	((CDFace*)(Object*)(ObjectIf*)this)->GetIndices();
}
SPR_IFIMP1(CDQuadFace, Object);
int Spr::CDQuadFaceIf::NIndex(){
	return	((CDQuadFace*)(Object*)(ObjectIf*)this)->NIndex();
}
int* Spr::CDQuadFaceIf::GetIndices(){
	return	((CDQuadFace*)(Object*)(ObjectIf*)this)->GetIndices();
}
SPR_IFIMP1(CDConvexMesh, CDConvex);
Spr::CDFaceIf* Spr::CDConvexMeshIf::GetFace(size_t i){
	return	((CDConvexMesh*)(Object*)(ObjectIf*)this)->GetFace(i);
}
size_t Spr::CDConvexMeshIf::NFace(){
	return	((CDConvexMesh*)(Object*)(ObjectIf*)this)->NFace();
}
Vec3f* Spr::CDConvexMeshIf::GetVertices(){
	return	((CDConvexMesh*)(Object*)(ObjectIf*)this)->GetVertices();
}
size_t Spr::CDConvexMeshIf::NVertex(){
	return	((CDConvexMesh*)(Object*)(ObjectIf*)this)->NVertex();
}
SPR_IFIMP1(CDConvexMeshInterpolate, CDConvexMesh);
SPR_IFIMP1(CDSphere, CDConvex);
float Spr::CDSphereIf::GetRadius(){
	return	((CDSphere*)(Object*)(ObjectIf*)this)->GetRadius();
}
SPR_IFIMP1(CDCapsule, CDConvex);
float Spr::CDCapsuleIf::GetRadius(){
	return	((CDCapsule*)(Object*)(ObjectIf*)this)->GetRadius();
}
float Spr::CDCapsuleIf::GetLength(){
	return	((CDCapsule*)(Object*)(ObjectIf*)this)->GetLength();
}
SPR_IFIMP1(CDRoundCone, CDConvex);
Vec2f Spr::CDRoundConeIf::GetRadius(){
	return	((CDRoundCone*)(Object*)(ObjectIf*)this)->GetRadius();
}
float Spr::CDRoundConeIf::GetLength(){
	return	((CDRoundCone*)(Object*)(ObjectIf*)this)->GetLength();
}
void Spr::CDRoundConeIf::SetRadius(Vec2f r){
	((CDRoundCone*)(Object*)(ObjectIf*)this)->SetRadius(r);
}
void Spr::CDRoundConeIf::SetLength(float l){
	((CDRoundCone*)(Object*)(ObjectIf*)this)->SetLength(l);
}
void Spr::CDRoundConeIf::SetWidth(Vec2f r){
	((CDRoundCone*)(Object*)(ObjectIf*)this)->SetWidth(r);
}
SPR_IFIMP1(CDBox, CDConvex);
Vec3f Spr::CDBoxIf::GetBoxSize(){
	return	((CDBox*)(Object*)(ObjectIf*)this)->GetBoxSize();
}
Vec3f* Spr::CDBoxIf::GetVertices(){
	return	((CDBox*)(Object*)(ObjectIf*)this)->GetVertices();
}
Spr::CDFaceIf* Spr::CDBoxIf::GetFace(size_t i){
	return	((CDBox*)(Object*)(ObjectIf*)this)->GetFace(i);
}
Vec3f Spr::CDBoxIf::SetBoxSize(Vec3f boxsize){
	return	((CDBox*)(Object*)(ObjectIf*)this)->SetBoxSize(boxsize);
}
SPR_OBJECTIMP1(CDQuadFace, Object);
SPR_OBJECTIMP1(CDBox, CDConvex);
SPR_OBJECTIMP1(CDCapsule, CDConvex);
SPR_OBJECTIMP_ABST1(CDConvex, CDShape);
SPR_OBJECTIMP1(CDFace, Object);
SPR_OBJECTIMP1(CDConvexMesh, CDConvex);
SPR_OBJECTIMP1(CDConvexMeshInterpolate, CDConvexMesh);
SPR_OBJECTIMP1(CDShapePair, Object);
SPR_OBJECTIMP1(CDRoundCone, CDConvex);
SPR_OBJECTIMP_ABST1(CDShape, NamedObject);
SPR_OBJECTIMP1(CDSphere, CDConvex);
}
