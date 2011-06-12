#ifndef SprEPUtility_H
#define SprEPUtility_H

#include "Base/TQuaternion.h"
#include "Base/Affine.h"

#ifdef SWIG
#define __EPDECL
#else
#if _DEBUG
#define __EPDECL __cdecl
#else
#define __EPDECL __fastcall
#endif
#endif

//springhead
#include "Utility/SprEPIfInfo.h"
#include "Utility/SprEPUTString.h"
//swig
#include "Utility/SprEPErrors.h"

//std
#include "Utility/SprEPvoid.h"
#include "Utility/SprEPostream.h"
#include "Utility/SprEPistream.h"
#include "Utility/SprEPstring.h"
#include "Utility/SprEPchar.h"
#include "Utility/SprEPunsignedint.h"
#include "Utility/SprEPunsignedlong.h"

void __EPDECL initUtility(void);

//CriticalSection—p
#include "Utility/UTCriticalSection.h"
extern UTCriticalSection EPCriticalSection;
typedef struct
{
	PyObject_HEAD
	Vec2f* ptr;
} EPVec2f;
typedef struct
{
	PyObject_HEAD
	Vec2d* ptr;
} EPVec2d;
typedef struct
{
	PyObject_HEAD
	Vec3f* ptr;
} EPVec3f;
typedef struct
{
	PyObject_HEAD
	Vec3d* ptr;
} EPVec3d;
typedef struct
{
	PyObject_HEAD
	Vec4f* ptr;
} EPVec4f;
typedef struct
{
	PyObject_HEAD
	Vec4d* ptr;
} EPVec4d;
typedef struct
{
	PyObject_HEAD
	Matrix2f* ptr;
} EPMatrix2f;
typedef struct
{
	PyObject_HEAD
	Matrix2d* ptr;
} EPMatrix2d;
typedef struct
{
	PyObject_HEAD
	Matrix3f* ptr;
} EPMatrix3f;
typedef struct
{
	PyObject_HEAD
	Matrix3d* ptr;
} EPMatrix3d;
typedef struct
{
	PyObject_HEAD
	Affine2f* ptr;
} EPAffine2f;
typedef struct
{
	PyObject_HEAD
	Affinef* ptr;
} EPAffinef;
typedef struct
{
	PyObject_HEAD
	Affine2d* ptr;
} EPAffine2d;
typedef struct
{
	PyObject_HEAD
	Affined* ptr;
} EPAffined;
typedef struct
{
	PyObject_HEAD
	Quaterniond* ptr;
} EPQuaterniond;
typedef struct
{
	PyObject_HEAD
	Quaternionf* ptr;
} EPQuaternionf;
typedef struct
{
	PyObject_HEAD
	Posed* ptr;
} EPPosed;

//{*********EPVec2f*******
#define EPVec2f_Check(ob) PyObject_TypeCheck(ob, &EPVec2fType)
extern PyTypeObject EPVec2fType;
PyObject* __EPDECL newEPVec2f(Vec2f);
PyObject* __EPDECL newEPVec2f(Vec2f*);
PyObject* EPVec2f_clear( EPVec2f* self );
PyObject* EPVec2f_norm( EPVec2f* self );
PyObject* EPVec2f_resize( EPVec2f* self,PyLongObject* var1 );
PyObject* EPVec2f_size( EPVec2f* self );
PyObject* EPVec2f_square( EPVec2f* self );
PyObject* EPVec2f_unit( EPVec2f* self );
PyObject* EPVec2f_unitize( EPVec2f* self );

//}EPVec2f

//{*********EPVec2d*******
#define EPVec2d_Check(ob) PyObject_TypeCheck(ob, &EPVec2dType)
extern PyTypeObject EPVec2dType;
PyObject* __EPDECL newEPVec2d(Vec2d);
PyObject* __EPDECL newEPVec2d(Vec2d*);
PyObject* EPVec2d_clear( EPVec2d* self );
PyObject* EPVec2d_norm( EPVec2d* self );
PyObject* EPVec2d_resize( EPVec2d* self,PyLongObject* var1 );
PyObject* EPVec2d_size( EPVec2d* self );
PyObject* EPVec2d_square( EPVec2d* self );
PyObject* EPVec2d_unit( EPVec2d* self );
PyObject* EPVec2d_unitize( EPVec2d* self );

//}EPVec2d

//{*********EPVec3f*******
#define EPVec3f_Check(ob) PyObject_TypeCheck(ob, &EPVec3fType)
extern PyTypeObject EPVec3fType;
PyObject* __EPDECL newEPVec3f(Vec3f);
PyObject* __EPDECL newEPVec3f(Vec3f*);
PyObject* EPVec3f_clear( EPVec3f* self );
PyObject* EPVec3f_norm( EPVec3f* self );
PyObject* EPVec3f_resize( EPVec3f* self,PyLongObject* var1 );
PyObject* EPVec3f_size( EPVec3f* self );
PyObject* EPVec3f_square( EPVec3f* self );
PyObject* EPVec3f_unit( EPVec3f* self );
PyObject* EPVec3f_unitize( EPVec3f* self );

//}EPVec3f

//{*********EPVec3d*******
#define EPVec3d_Check(ob) PyObject_TypeCheck(ob, &EPVec3dType)
extern PyTypeObject EPVec3dType;
PyObject* __EPDECL newEPVec3d(Vec3d);
PyObject* __EPDECL newEPVec3d(Vec3d*);
PyObject* EPVec3d_clear( EPVec3d* self );
PyObject* EPVec3d_norm( EPVec3d* self );
PyObject* EPVec3d_resize( EPVec3d* self,PyLongObject* var1 );
PyObject* EPVec3d_size( EPVec3d* self );
PyObject* EPVec3d_square( EPVec3d* self );
PyObject* EPVec3d_unit( EPVec3d* self );
PyObject* EPVec3d_unitize( EPVec3d* self );

//}EPVec3d

//{*********EPVec4f*******
#define EPVec4f_Check(ob) PyObject_TypeCheck(ob, &EPVec4fType)
extern PyTypeObject EPVec4fType;
PyObject* __EPDECL newEPVec4f(Vec4f);
PyObject* __EPDECL newEPVec4f(Vec4f*);
PyObject* EPVec4f_clear( EPVec4f* self );
PyObject* EPVec4f_norm( EPVec4f* self );
PyObject* EPVec4f_resize( EPVec4f* self,PyLongObject* var1 );
PyObject* EPVec4f_size( EPVec4f* self );
PyObject* EPVec4f_square( EPVec4f* self );
PyObject* EPVec4f_unit( EPVec4f* self );
PyObject* EPVec4f_unitize( EPVec4f* self );

//}EPVec4f

//{*********EPVec4d*******
#define EPVec4d_Check(ob) PyObject_TypeCheck(ob, &EPVec4dType)
extern PyTypeObject EPVec4dType;
PyObject* __EPDECL newEPVec4d(Vec4d);
PyObject* __EPDECL newEPVec4d(Vec4d*);
PyObject* EPVec4d_clear( EPVec4d* self );
PyObject* EPVec4d_norm( EPVec4d* self );
PyObject* EPVec4d_resize( EPVec4d* self,PyLongObject* var1 );
PyObject* EPVec4d_size( EPVec4d* self );
PyObject* EPVec4d_square( EPVec4d* self );
PyObject* EPVec4d_unit( EPVec4d* self );
PyObject* EPVec4d_unitize( EPVec4d* self );

//}EPVec4d

//{*********EPMatrix2f*******
#define EPMatrix2f_Check(ob) PyObject_TypeCheck(ob, &EPMatrix2fType)
extern PyTypeObject EPMatrix2fType;
PyObject* __EPDECL newEPMatrix2f(Matrix2f);
PyObject* __EPDECL newEPMatrix2f(Matrix2f*);
PyObject* EPMatrix2f_Diag( EPMatrix2f* self,PyObject* tuple );
PyObject* EPMatrix2f_Rot( EPMatrix2f* self,PyObject* tuple );//Overloaded
PyObject* EPMatrix2f_Unit( EPMatrix2f* self );
PyObject* EPMatrix2f_Zero( EPMatrix2f* self );
PyObject* EPMatrix2f_angle( EPMatrix2f* self );
PyObject* EPMatrix2f_clear( EPMatrix2f* self );
PyObject* EPMatrix2f_col( EPMatrix2f* self,PyLongObject* var1 );
PyObject* EPMatrix2f_det( EPMatrix2f* self );
PyObject* EPMatrix2f_height( EPMatrix2f* self );
PyObject* EPMatrix2f_inv( EPMatrix2f* self );
PyObject* EPMatrix2f_resize( EPMatrix2f* self,PyObject* tuple );
PyObject* EPMatrix2f_row( EPMatrix2f* self,PyLongObject* var1 );
PyObject* EPMatrix2f_trans( EPMatrix2f* self );
PyObject* EPMatrix2f_width( EPMatrix2f* self );

//}EPMatrix2f

//{*********EPMatrix2d*******
#define EPMatrix2d_Check(ob) PyObject_TypeCheck(ob, &EPMatrix2dType)
extern PyTypeObject EPMatrix2dType;
PyObject* __EPDECL newEPMatrix2d(Matrix2d);
PyObject* __EPDECL newEPMatrix2d(Matrix2d*);
PyObject* EPMatrix2d_Diag( EPMatrix2d* self,PyObject* tuple );
PyObject* EPMatrix2d_Rot( EPMatrix2d* self,PyObject* tuple );//Overloaded
PyObject* EPMatrix2d_Unit( EPMatrix2d* self );
PyObject* EPMatrix2d_Zero( EPMatrix2d* self );
PyObject* EPMatrix2d_angle( EPMatrix2d* self );
PyObject* EPMatrix2d_clear( EPMatrix2d* self );
PyObject* EPMatrix2d_col( EPMatrix2d* self,PyLongObject* var1 );
PyObject* EPMatrix2d_det( EPMatrix2d* self );
PyObject* EPMatrix2d_height( EPMatrix2d* self );
PyObject* EPMatrix2d_inv( EPMatrix2d* self );
PyObject* EPMatrix2d_resize( EPMatrix2d* self,PyObject* tuple );
PyObject* EPMatrix2d_row( EPMatrix2d* self,PyLongObject* var1 );
PyObject* EPMatrix2d_trans( EPMatrix2d* self );
PyObject* EPMatrix2d_width( EPMatrix2d* self );

//}EPMatrix2d

//{*********EPMatrix3f*******
#define EPMatrix3f_Check(ob) PyObject_TypeCheck(ob, &EPMatrix3fType)
extern PyTypeObject EPMatrix3fType;
PyObject* __EPDECL newEPMatrix3f(Matrix3f);
PyObject* __EPDECL newEPMatrix3f(Matrix3f*);
PyObject* EPMatrix3f_Cross( EPMatrix3f* self,EPVec3f* var1 );
PyObject* EPMatrix3f_Diag( EPMatrix3f* self,PyObject* tuple );
PyObject* EPMatrix3f_Rot( EPMatrix3f* self,PyObject* tuple );//Overloaded
PyObject* EPMatrix3f_Unit( EPMatrix3f* self );
PyObject* EPMatrix3f_Zero( EPMatrix3f* self );
PyObject* EPMatrix3f_clear( EPMatrix3f* self );
PyObject* EPMatrix3f_col( EPMatrix3f* self,PyLongObject* var1 );
PyObject* EPMatrix3f_det( EPMatrix3f* self );
PyObject* EPMatrix3f_height( EPMatrix3f* self );
PyObject* EPMatrix3f_inv( EPMatrix3f* self );
PyObject* EPMatrix3f_resize( EPMatrix3f* self,PyObject* tuple );
PyObject* EPMatrix3f_row( EPMatrix3f* self,PyLongObject* var1 );
PyObject* EPMatrix3f_trans( EPMatrix3f* self );
PyObject* EPMatrix3f_width( EPMatrix3f* self );

//}EPMatrix3f

//{*********EPMatrix3d*******
#define EPMatrix3d_Check(ob) PyObject_TypeCheck(ob, &EPMatrix3dType)
extern PyTypeObject EPMatrix3dType;
PyObject* __EPDECL newEPMatrix3d(Matrix3d);
PyObject* __EPDECL newEPMatrix3d(Matrix3d*);
PyObject* EPMatrix3d_Cross( EPMatrix3d* self,EPVec3d* var1 );
PyObject* EPMatrix3d_Diag( EPMatrix3d* self,PyObject* tuple );
PyObject* EPMatrix3d_Rot( EPMatrix3d* self,PyObject* tuple );//Overloaded
PyObject* EPMatrix3d_Unit( EPMatrix3d* self );
PyObject* EPMatrix3d_Zero( EPMatrix3d* self );
PyObject* EPMatrix3d_clear( EPMatrix3d* self );
PyObject* EPMatrix3d_col( EPMatrix3d* self,PyLongObject* var1 );
PyObject* EPMatrix3d_det( EPMatrix3d* self );
PyObject* EPMatrix3d_height( EPMatrix3d* self );
PyObject* EPMatrix3d_inv( EPMatrix3d* self );
PyObject* EPMatrix3d_resize( EPMatrix3d* self,PyObject* tuple );
PyObject* EPMatrix3d_row( EPMatrix3d* self,PyLongObject* var1 );
PyObject* EPMatrix3d_trans( EPMatrix3d* self );
PyObject* EPMatrix3d_width( EPMatrix3d* self );

//}EPMatrix3d

//{*********EPAffine2f*******
#define EPAffine2f_Check(ob) PyObject_TypeCheck(ob, &EPAffine2fType)
extern PyTypeObject EPAffine2fType;
PyObject* __EPDECL newEPAffine2f(Affine2f);
PyObject* __EPDECL newEPAffine2f(Affine2f*);
PyObject* EPAffine2f_Rot( EPAffine2f* self,PyFloatObject* var1 );
PyObject* EPAffine2f_Scale( EPAffine2f* self,PyObject* tuple );
PyObject* EPAffine2f_Trn( EPAffine2f* self,PyObject* tuple );
PyObject* EPAffine2f_Unit( EPAffine2f* self );
PyObject* EPAffine2f_clear( EPAffine2f* self );
PyObject* EPAffine2f_col( EPAffine2f* self,PyLongObject* var1 );
PyObject* EPAffine2f_det( EPAffine2f* self );
PyObject* EPAffine2f_height( EPAffine2f* self );
PyObject* EPAffine2f_inv( EPAffine2f* self );
PyObject* EPAffine2f_resize( EPAffine2f* self,PyObject* tuple );
PyObject* EPAffine2f_row( EPAffine2f* self,PyLongObject* var1 );
PyObject* EPAffine2f_trans( EPAffine2f* self );
PyObject* EPAffine2f_width( EPAffine2f* self );

//}EPAffine2f

//{*********EPAffinef*******
#define EPAffinef_Check(ob) PyObject_TypeCheck(ob, &EPAffinefType)
extern PyTypeObject EPAffinefType;
PyObject* __EPDECL newEPAffinef(Affinef);
PyObject* __EPDECL newEPAffinef(Affinef*);
PyObject* EPAffinef_LookAt( EPAffinef* self,PyObject* tuple );//Overloaded
PyObject* EPAffinef_LookAtGL( EPAffinef* self,PyObject* tuple );//Overloaded
PyObject* EPAffinef_ProjectionD3D( EPAffinef* self,PyObject* tuple );//Overloaded
PyObject* EPAffinef_ProjectionGL( EPAffinef* self,PyObject* tuple );//Overloaded
PyObject* EPAffinef_Rot( EPAffinef* self,PyObject* tuple );//Overloaded
PyObject* EPAffinef_Scale( EPAffinef* self,PyObject* tuple );
PyObject* EPAffinef_Trn( EPAffinef* self,PyObject* tuple );
PyObject* EPAffinef_Unit( EPAffinef* self );
PyObject* EPAffinef_clear( EPAffinef* self );
PyObject* EPAffinef_col( EPAffinef* self,PyLongObject* var1 );
PyObject* EPAffinef_det( EPAffinef* self );
PyObject* EPAffinef_height( EPAffinef* self );
PyObject* EPAffinef_inv( EPAffinef* self );
PyObject* EPAffinef_resize( EPAffinef* self,PyObject* tuple );
PyObject* EPAffinef_row( EPAffinef* self,PyLongObject* var1 );
PyObject* EPAffinef_trans( EPAffinef* self );
PyObject* EPAffinef_width( EPAffinef* self );

//}EPAffinef

//{*********EPAffine2d*******
#define EPAffine2d_Check(ob) PyObject_TypeCheck(ob, &EPAffine2dType)
extern PyTypeObject EPAffine2dType;
PyObject* __EPDECL newEPAffine2d(Affine2d);
PyObject* __EPDECL newEPAffine2d(Affine2d*);
PyObject* EPAffine2d_Rot( EPAffine2d* self,PyFloatObject* var1 );
PyObject* EPAffine2d_Scale( EPAffine2d* self,PyObject* tuple );
PyObject* EPAffine2d_Trn( EPAffine2d* self,PyObject* tuple );
PyObject* EPAffine2d_Unit( EPAffine2d* self );
PyObject* EPAffine2d_clear( EPAffine2d* self );
PyObject* EPAffine2d_col( EPAffine2d* self,PyLongObject* var1 );
PyObject* EPAffine2d_det( EPAffine2d* self );
PyObject* EPAffine2d_height( EPAffine2d* self );
PyObject* EPAffine2d_inv( EPAffine2d* self );
PyObject* EPAffine2d_resize( EPAffine2d* self,PyObject* tuple );
PyObject* EPAffine2d_row( EPAffine2d* self,PyLongObject* var1 );
PyObject* EPAffine2d_trans( EPAffine2d* self );
PyObject* EPAffine2d_width( EPAffine2d* self );

//}EPAffine2d

//{*********EPAffined*******
#define EPAffined_Check(ob) PyObject_TypeCheck(ob, &EPAffinedType)
extern PyTypeObject EPAffinedType;
PyObject* __EPDECL newEPAffined(Affined);
PyObject* __EPDECL newEPAffined(Affined*);
PyObject* EPAffined_LookAt( EPAffined* self,PyObject* tuple );//Overloaded
PyObject* EPAffined_LookAtGL( EPAffined* self,PyObject* tuple );//Overloaded
PyObject* EPAffined_ProjectionD3D( EPAffined* self,PyObject* tuple );//Overloaded
PyObject* EPAffined_ProjectionGL( EPAffined* self,PyObject* tuple );//Overloaded
PyObject* EPAffined_Rot( EPAffined* self,PyObject* tuple );//Overloaded
PyObject* EPAffined_Scale( EPAffined* self,PyObject* tuple );
PyObject* EPAffined_Trn( EPAffined* self,PyObject* tuple );
PyObject* EPAffined_Unit( EPAffined* self );
PyObject* EPAffined_clear( EPAffined* self );
PyObject* EPAffined_col( EPAffined* self,PyLongObject* var1 );
PyObject* EPAffined_det( EPAffined* self );
PyObject* EPAffined_height( EPAffined* self );
PyObject* EPAffined_inv( EPAffined* self );
PyObject* EPAffined_resize( EPAffined* self,PyObject* tuple );
PyObject* EPAffined_row( EPAffined* self,PyLongObject* var1 );
PyObject* EPAffined_trans( EPAffined* self );
PyObject* EPAffined_width( EPAffined* self );

//}EPAffined

//{*********EPQuaterniond*******
#define EPQuaterniond_Check(ob) PyObject_TypeCheck(ob, &EPQuaterniondType)
extern PyTypeObject EPQuaterniondType;
PyObject* __EPDECL newEPQuaterniond(Quaterniond);
PyObject* __EPDECL newEPQuaterniond(Quaterniond*);
PyObject* EPQuaterniond_AngularVelocity( EPQuaterniond* self,EPQuaterniond* var1 );
PyObject* EPQuaterniond_Axis( EPQuaterniond* self );
PyObject* EPQuaterniond_Conjugate( EPQuaterniond* self );
PyObject* EPQuaterniond_Conjugated( EPQuaterniond* self );
PyObject* EPQuaterniond_Derivative( EPQuaterniond* self,EPVec3d* var1 );
PyObject* EPQuaterniond_Euler( EPQuaterniond* self,PyObject* tuple );
PyObject* EPQuaterniond_FromEuler( EPQuaterniond* self,EPVec3d* var1 );
PyObject* EPQuaterniond_FromMatrix( EPQuaterniond* self,EPMatrix3d* var1 );
PyObject* EPQuaterniond_Inv( EPQuaterniond* self );
PyObject* EPQuaterniond_Rot( EPQuaterniond* self,PyObject* tuple );//Overloaded
PyObject* EPQuaterniond_Rotation( EPQuaterniond* self );
PyObject* EPQuaterniond_RotationArc( EPQuaterniond* self,PyObject* tuple );
PyObject* EPQuaterniond_RotationHalf( EPQuaterniond* self );
PyObject* EPQuaterniond_Theta( EPQuaterniond* self );
PyObject* EPQuaterniond_ToEuler( EPQuaterniond* self,EPVec3d* var1 );
PyObject* EPQuaterniond_ToMatrix( EPQuaterniond* self,EPMatrix3d* var1 );
PyObject* EPQuaterniond_clear( EPQuaterniond* self );
PyObject* EPQuaterniond_norm( EPQuaterniond* self );
PyObject* EPQuaterniond_resize( EPQuaterniond* self,PyLongObject* var1 );
PyObject* EPQuaterniond_size( EPQuaterniond* self );
PyObject* EPQuaterniond_square( EPQuaterniond* self );
PyObject* EPQuaterniond_unit( EPQuaterniond* self );
PyObject* EPQuaterniond_unitize( EPQuaterniond* self );

//}EPQuaterniond

//{*********EPQuaternionf*******
#define EPQuaternionf_Check(ob) PyObject_TypeCheck(ob, &EPQuaternionfType)
extern PyTypeObject EPQuaternionfType;
PyObject* __EPDECL newEPQuaternionf(Quaternionf);
PyObject* __EPDECL newEPQuaternionf(Quaternionf*);
PyObject* EPQuaternionf_AngularVelocity( EPQuaternionf* self,EPQuaternionf* var1 );
PyObject* EPQuaternionf_Axis( EPQuaternionf* self );
PyObject* EPQuaternionf_Conjugate( EPQuaternionf* self );
PyObject* EPQuaternionf_Conjugated( EPQuaternionf* self );
PyObject* EPQuaternionf_Derivative( EPQuaternionf* self,EPVec3f* var1 );
PyObject* EPQuaternionf_Euler( EPQuaternionf* self,PyObject* tuple );
PyObject* EPQuaternionf_FromEuler( EPQuaternionf* self,EPVec3f* var1 );
PyObject* EPQuaternionf_FromMatrix( EPQuaternionf* self,EPMatrix3d* var1 );
PyObject* EPQuaternionf_Inv( EPQuaternionf* self );
PyObject* EPQuaternionf_Rot( EPQuaternionf* self,PyObject* tuple );//Overloaded
PyObject* EPQuaternionf_Rotation( EPQuaternionf* self );
PyObject* EPQuaternionf_RotationArc( EPQuaternionf* self,PyObject* tuple );
PyObject* EPQuaternionf_RotationHalf( EPQuaternionf* self );
PyObject* EPQuaternionf_Theta( EPQuaternionf* self );
PyObject* EPQuaternionf_ToEuler( EPQuaternionf* self,EPVec3f* var1 );
PyObject* EPQuaternionf_ToMatrix( EPQuaternionf* self,EPMatrix3d* var1 );
PyObject* EPQuaternionf_clear( EPQuaternionf* self );
PyObject* EPQuaternionf_norm( EPQuaternionf* self );
PyObject* EPQuaternionf_resize( EPQuaternionf* self,PyLongObject* var1 );
PyObject* EPQuaternionf_size( EPQuaternionf* self );
PyObject* EPQuaternionf_square( EPQuaternionf* self );
PyObject* EPQuaternionf_unit( EPQuaternionf* self );
PyObject* EPQuaternionf_unitize( EPQuaternionf* self );

//}EPQuaternionf

//{*********EPPosed*******
#define EPPosed_Check(ob) PyObject_TypeCheck(ob, &EPPosedType)
extern PyTypeObject EPPosedType;
PyObject* __EPDECL newEPPosed(Posed);
PyObject* __EPDECL newEPPosed(Posed*);
PyObject* EPPosed_FromAffine( EPPosed* self,EPAffined* var1 );
PyObject* EPPosed_Inv( EPPosed* self );
PyObject* EPPosed_Rot( EPPosed* self,PyObject* tuple );//Overloaded
PyObject* EPPosed_ToAffine( EPPosed* self,EPAffined* var1 );
PyObject* EPPosed_Trn( EPPosed* self,PyObject* tuple );//Overloaded
PyObject* EPPosed_Unit( EPPosed* self,PyObject* tuple );//Overloaded
PyObject* EPPosed_clear( EPPosed* self );
PyObject* EPPosed_norm( EPPosed* self );
PyObject* EPPosed_resize( EPPosed* self,PyLongObject* var1 );
PyObject* EPPosed_size( EPPosed* self );
PyObject* EPPosed_square( EPPosed* self );
PyObject* EPPosed_unit( EPPosed* self );
PyObject* EPPosed_unitize( EPPosed* self );

//}EPPosed
void __EPDECL initUtility(void) ;
#endif
