#ifndef SprEPCollision_H
#define SprEPCollision_H
#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Python\Add_decl.h"
#include "..\..\include\Python\pyport.h"
#include "..\..\include\Python\patchlevel.h"
#include "..\..\include\Python\Python.h"
#include "..\..\include\Python\object.h"
#include "..\..\include\EmbPython\SprEPUtility.h"
#include "..\..\include\EmbPython\SprEPFoundation.h"
#include "..\..\include\Collision\SprCDShape.h"
typedef struct
{
	PyObject_HEAD
	UTRef<CDShapeIf> ptr;
} EPCDShapeIf;
typedef struct
{
	PyObject_HEAD
	UTRef<CDShapePairIf> ptr;
} EPCDShapePairIf;
typedef struct
{
	PyObject_HEAD
	UTRef<CDConvexIf> ptr;
} EPCDConvexIf;
typedef struct
{
	PyObject_HEAD
	UTRef<CDFaceIf> ptr;
} EPCDFaceIf;
typedef struct
{
	PyObject_HEAD
	UTRef<CDQuadFaceIf> ptr;
} EPCDQuadFaceIf;
typedef struct
{
	PyObject_HEAD
	struct PHMaterial* ptr;
} EPPHMaterial;
typedef struct
{
	PyObject_HEAD
	struct CDShapeDesc* ptr;
} EPCDShapeDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CDConvexMeshIf> ptr;
} EPCDConvexMeshIf;
typedef struct
{
	PyObject_HEAD
	struct CDConvexMeshDesc* ptr;
} EPCDConvexMeshDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CDConvexMeshInterpolateIf> ptr;
} EPCDConvexMeshInterpolateIf;
typedef struct
{
	PyObject_HEAD
	struct CDConvexMeshInterpolateDesc* ptr;
} EPCDConvexMeshInterpolateDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CDSphereIf> ptr;
} EPCDSphereIf;
typedef struct
{
	PyObject_HEAD
	struct CDSphereDesc* ptr;
} EPCDSphereDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CDCapsuleIf> ptr;
} EPCDCapsuleIf;
typedef struct
{
	PyObject_HEAD
	struct CDCapsuleDesc* ptr;
} EPCDCapsuleDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CDRoundConeIf> ptr;
} EPCDRoundConeIf;
typedef struct
{
	PyObject_HEAD
	struct CDRoundConeDesc* ptr;
} EPCDRoundConeDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<CDBoxIf> ptr;
} EPCDBoxIf;
typedef struct
{
	PyObject_HEAD
	struct CDBoxDesc* ptr;
} EPCDBoxDesc;

//{*********EPCDShapeIf*******
#define EPCDShapeIf_Check(ob) PyObject_TypeCheck(ob, &EPCDShapeIfType)
extern PyTypeObject EPCDShapeIfType;
PyObject* __EPDECL newEPCDShapeIf(struct CDShapeIf);
PyObject* __EPDECL newEPCDShapeIf(struct CDShapeIf*);
PyObject* EPCDShapeIf_CalcCenterOfMass( EPCDShapeIf* self );
PyObject* EPCDShapeIf_CalcMomentOfInertia( EPCDShapeIf* self );
PyObject* EPCDShapeIf_CalcVolume( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetDamperD( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetDensity( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetDynamicFriction( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetElasticity( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetMaterial( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetSpringK( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetStaticFriction( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetVibA( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetVibB( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetVibContact( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetVibT( EPCDShapeIf* self );
PyObject* EPCDShapeIf_GetVibW( EPCDShapeIf* self );
PyObject* EPCDShapeIf_SetDensity( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetDynamicFriction( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetElasticity( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetMaterial( EPCDShapeIf* self,EPPHMaterial* var1 );
PyObject* EPCDShapeIf_SetSpringDamper( EPCDShapeIf* self,PyObject* tuple );
PyObject* EPCDShapeIf_SetStaticFriction( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetVibA( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetVibB( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetVibContact( EPCDShapeIf* self,PyObject* var1 );
PyObject* EPCDShapeIf_SetVibT( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetVibW( EPCDShapeIf* self,PyFloatObject* var1 );
PyObject* EPCDShapeIf_SetVibration( EPCDShapeIf* self,PyObject* tuple );

//}EPCDShapeIf

//{*********EPCDShapePairIf*******
#define EPCDShapePairIf_Check(ob) PyObject_TypeCheck(ob, &EPCDShapePairIfType)
extern PyTypeObject EPCDShapePairIfType;
PyObject* __EPDECL newEPCDShapePairIf(struct CDShapePairIf);
PyObject* __EPDECL newEPCDShapePairIf(struct CDShapePairIf*);
PyObject* EPCDShapePairIf_GetShape( EPCDShapePairIf* self,PyLongObject* var1 );

//}EPCDShapePairIf

//{*********EPCDConvexIf*******
#define EPCDConvexIf_Check(ob) PyObject_TypeCheck(ob, &EPCDConvexIfType)
extern PyTypeObject EPCDConvexIfType;
PyObject* __EPDECL newEPCDConvexIf(struct CDConvexIf);
PyObject* __EPDECL newEPCDConvexIf(struct CDConvexIf*);
PyObject* EPCDConvexIf_CurvatureRadius( EPCDConvexIf* self,EPVec3d* var1 );
PyObject* EPCDConvexIf_Normal( EPCDConvexIf* self,EPVec3d* var1 );

//}EPCDConvexIf

//{*********EPCDFaceIf*******
#define EPCDFaceIf_Check(ob) PyObject_TypeCheck(ob, &EPCDFaceIfType)
extern PyTypeObject EPCDFaceIfType;
PyObject* __EPDECL newEPCDFaceIf(struct CDFaceIf);
PyObject* __EPDECL newEPCDFaceIf(struct CDFaceIf*);
PyObject* EPCDFaceIf_GetIndices( EPCDFaceIf* self );
PyObject* EPCDFaceIf_NIndex( EPCDFaceIf* self );

//}EPCDFaceIf

//{*********EPCDQuadFaceIf*******
#define EPCDQuadFaceIf_Check(ob) PyObject_TypeCheck(ob, &EPCDQuadFaceIfType)
extern PyTypeObject EPCDQuadFaceIfType;
PyObject* __EPDECL newEPCDQuadFaceIf(struct CDQuadFaceIf);
PyObject* __EPDECL newEPCDQuadFaceIf(struct CDQuadFaceIf*);
PyObject* EPCDQuadFaceIf_GetIndices( EPCDQuadFaceIf* self );
PyObject* EPCDQuadFaceIf_NIndex( EPCDQuadFaceIf* self );

//}EPCDQuadFaceIf

//{*********EPPHMaterial*******
#define EPPHMaterial_Check(ob) PyObject_TypeCheck(ob, &EPPHMaterialType)
extern PyTypeObject EPPHMaterialType;
PyObject* __EPDECL newEPPHMaterial(struct PHMaterial);
PyObject* __EPDECL newEPPHMaterial(struct PHMaterial*);

//}EPPHMaterial

//{*********EPCDShapeDesc*******
#define EPCDShapeDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDShapeDescType)
extern PyTypeObject EPCDShapeDescType;
PyObject* __EPDECL newEPCDShapeDesc(struct CDShapeDesc);
PyObject* __EPDECL newEPCDShapeDesc(struct CDShapeDesc*);

//}EPCDShapeDesc

//{*********EPCDConvexMeshIf*******
#define EPCDConvexMeshIf_Check(ob) PyObject_TypeCheck(ob, &EPCDConvexMeshIfType)
extern PyTypeObject EPCDConvexMeshIfType;
PyObject* __EPDECL newEPCDConvexMeshIf(struct CDConvexMeshIf);
PyObject* __EPDECL newEPCDConvexMeshIf(struct CDConvexMeshIf*);
PyObject* EPCDConvexMeshIf_GetFace( EPCDConvexMeshIf* self,PyLongObject* var1 );
PyObject* EPCDConvexMeshIf_GetVertices( EPCDConvexMeshIf* self );
PyObject* EPCDConvexMeshIf_NFace( EPCDConvexMeshIf* self );
PyObject* EPCDConvexMeshIf_NVertex( EPCDConvexMeshIf* self );

//}EPCDConvexMeshIf

//{*********EPCDConvexMeshDesc*******
#define EPCDConvexMeshDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDConvexMeshDescType)
extern PyTypeObject EPCDConvexMeshDescType;
PyObject* __EPDECL newEPCDConvexMeshDesc(struct CDConvexMeshDesc);
PyObject* __EPDECL newEPCDConvexMeshDesc(struct CDConvexMeshDesc*);

//}EPCDConvexMeshDesc

//{*********EPCDConvexMeshInterpolateIf*******
#define EPCDConvexMeshInterpolateIf_Check(ob) PyObject_TypeCheck(ob, &EPCDConvexMeshInterpolateIfType)
extern PyTypeObject EPCDConvexMeshInterpolateIfType;
PyObject* __EPDECL newEPCDConvexMeshInterpolateIf(struct CDConvexMeshInterpolateIf);
PyObject* __EPDECL newEPCDConvexMeshInterpolateIf(struct CDConvexMeshInterpolateIf*);

//}EPCDConvexMeshInterpolateIf

//{*********EPCDConvexMeshInterpolateDesc*******
#define EPCDConvexMeshInterpolateDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDConvexMeshInterpolateDescType)
extern PyTypeObject EPCDConvexMeshInterpolateDescType;
PyObject* __EPDECL newEPCDConvexMeshInterpolateDesc(struct CDConvexMeshInterpolateDesc);
PyObject* __EPDECL newEPCDConvexMeshInterpolateDesc(struct CDConvexMeshInterpolateDesc*);

//}EPCDConvexMeshInterpolateDesc

//{*********EPCDSphereIf*******
#define EPCDSphereIf_Check(ob) PyObject_TypeCheck(ob, &EPCDSphereIfType)
extern PyTypeObject EPCDSphereIfType;
PyObject* __EPDECL newEPCDSphereIf(struct CDSphereIf);
PyObject* __EPDECL newEPCDSphereIf(struct CDSphereIf*);
PyObject* EPCDSphereIf_GetRadius( EPCDSphereIf* self );

//}EPCDSphereIf

//{*********EPCDSphereDesc*******
#define EPCDSphereDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDSphereDescType)
extern PyTypeObject EPCDSphereDescType;
PyObject* __EPDECL newEPCDSphereDesc(struct CDSphereDesc);
PyObject* __EPDECL newEPCDSphereDesc(struct CDSphereDesc*);

//}EPCDSphereDesc

//{*********EPCDCapsuleIf*******
#define EPCDCapsuleIf_Check(ob) PyObject_TypeCheck(ob, &EPCDCapsuleIfType)
extern PyTypeObject EPCDCapsuleIfType;
PyObject* __EPDECL newEPCDCapsuleIf(struct CDCapsuleIf);
PyObject* __EPDECL newEPCDCapsuleIf(struct CDCapsuleIf*);
PyObject* EPCDCapsuleIf_GetLength( EPCDCapsuleIf* self );
PyObject* EPCDCapsuleIf_GetRadius( EPCDCapsuleIf* self );

//}EPCDCapsuleIf

//{*********EPCDCapsuleDesc*******
#define EPCDCapsuleDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDCapsuleDescType)
extern PyTypeObject EPCDCapsuleDescType;
PyObject* __EPDECL newEPCDCapsuleDesc(struct CDCapsuleDesc);
PyObject* __EPDECL newEPCDCapsuleDesc(struct CDCapsuleDesc*);

//}EPCDCapsuleDesc

//{*********EPCDRoundConeIf*******
#define EPCDRoundConeIf_Check(ob) PyObject_TypeCheck(ob, &EPCDRoundConeIfType)
extern PyTypeObject EPCDRoundConeIfType;
PyObject* __EPDECL newEPCDRoundConeIf(struct CDRoundConeIf);
PyObject* __EPDECL newEPCDRoundConeIf(struct CDRoundConeIf*);
PyObject* EPCDRoundConeIf_GetLength( EPCDRoundConeIf* self );
PyObject* EPCDRoundConeIf_GetRadius( EPCDRoundConeIf* self );
PyObject* EPCDRoundConeIf_SetLength( EPCDRoundConeIf* self,PyFloatObject* var1 );
PyObject* EPCDRoundConeIf_SetRadius( EPCDRoundConeIf* self,EPVec2f* var1 );
PyObject* EPCDRoundConeIf_SetWidth( EPCDRoundConeIf* self,EPVec2f* var1 );

//}EPCDRoundConeIf

//{*********EPCDRoundConeDesc*******
#define EPCDRoundConeDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDRoundConeDescType)
extern PyTypeObject EPCDRoundConeDescType;
PyObject* __EPDECL newEPCDRoundConeDesc(struct CDRoundConeDesc);
PyObject* __EPDECL newEPCDRoundConeDesc(struct CDRoundConeDesc*);

//}EPCDRoundConeDesc

//{*********EPCDBoxIf*******
#define EPCDBoxIf_Check(ob) PyObject_TypeCheck(ob, &EPCDBoxIfType)
extern PyTypeObject EPCDBoxIfType;
PyObject* __EPDECL newEPCDBoxIf(struct CDBoxIf);
PyObject* __EPDECL newEPCDBoxIf(struct CDBoxIf*);
PyObject* EPCDBoxIf_GetBoxSize( EPCDBoxIf* self );
PyObject* EPCDBoxIf_GetFace( EPCDBoxIf* self,PyLongObject* var1 );
PyObject* EPCDBoxIf_GetVertices( EPCDBoxIf* self );
PyObject* EPCDBoxIf_SetBoxSize( EPCDBoxIf* self,EPVec3f* var1 );

//}EPCDBoxIf

//{*********EPCDBoxDesc*******
#define EPCDBoxDesc_Check(ob) PyObject_TypeCheck(ob, &EPCDBoxDescType)
extern PyTypeObject EPCDBoxDescType;
PyObject* __EPDECL newEPCDBoxDesc(struct CDBoxDesc);
PyObject* __EPDECL newEPCDBoxDesc(struct CDBoxDesc*);

//}EPCDBoxDesc
void __EPDECL initCollision(void) ;
#endif
