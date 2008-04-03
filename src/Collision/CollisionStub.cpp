#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Collision\SprCDShape.h"
#include "..\..\src\Base\BaseDebug.h"
#include "..\..\src\Foundation\UTTypeDesc.h"
#include "..\Foundation\AccessDescDumpFoundation.h"
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
#include "..\Collision\AccessDescDumpCollision.h"
#include "..\Collision\CDBox.h"
#include "..\Collision\CDCapsule.h"
#include "..\Collision\CDConvex.h"
#include "..\Collision\CDConvexMesh.h"
#include "..\Collision\CDCutRing.h"
#include "..\Collision\CDDetectorImp.h"
#include "..\Collision\CDQuickHull2D.h"
#include "..\Collision\CDQuickHull2DImp.h"
#include "..\Collision\CDQuickHull3D.h"
#include "..\Collision\CDQuickHull3DImp.h"
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
	field = desc->AddField("", "float", "mu", "");
	field->offset = int((char*)&(pPHMaterial->mu) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "mu0", "");
	field->offset = int((char*)&(pPHMaterial->mu0) - (char*)pPHMaterial);
	field = desc->AddField("", "float", "e", "");
	field->offset = int((char*)&(pPHMaterial->e) - (char*)pPHMaterial);
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
float Spr::CDShapeIf::CalcVolume(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->CalcVolume();
}
Matrix3f Spr::CDShapeIf::CalcMomentOfInertia(){
	return	((CDShape*)(Object*)(ObjectIf*)this)->CalcMomentOfInertia();
}
SPR_IFIMP1(CDConvex, CDShape);
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
SPR_OBJECTIMP_ABST1(CDShape, NamedObject);
SPR_OBJECTIMP1(CDSphere, CDConvex);
}
