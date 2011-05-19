#ifndef SprEPFoundation_H
#define SprEPFoundation_H
#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Python\Add_decl.h"
#include "..\..\include\Python\pyport.h"
#include "..\..\include\Python\patchlevel.h"
#include "..\..\include\Python\Python.h"
#include "..\..\include\Python\object.h"
#include "..\..\include\EmbPython\SprEPUtility.h"
#include "..\..\include\Foundation\SprObject.h"
#include "..\..\include\Foundation\SprScene.h"
#include "..\..\include\Foundation\SprUTTimer.h"
typedef struct
{
	PyObject_HEAD
	UTRef<ObjectIf> ptr;
} EPObjectIf;
typedef struct
{
	PyObject_HEAD
	struct ObjectIfs* ptr;
} EPObjectIfs;
typedef struct
{
	PyObject_HEAD
	UTRef<NamedObjectIf> ptr;
} EPNamedObjectIf;
typedef struct
{
	PyObject_HEAD
	UTRef<SceneObjectIf> ptr;
} EPSceneObjectIf;
typedef struct
{
	PyObject_HEAD
	UTRef<ObjectStatesIf> ptr;
} EPObjectStatesIf;
typedef struct
{
	PyObject_HEAD
	UTRef<NameManagerIf> ptr;
} EPNameManagerIf;
typedef struct
{
	PyObject_HEAD
	UTRef<SceneIf> ptr;
} EPSceneIf;
typedef struct
{
	PyObject_HEAD
	UTRef<SdkIf> ptr;
} EPSdkIf;

//{*********EPObjectIf*******
#define EPObjectIf_Check(ob) PyObject_TypeCheck(ob, &EPObjectIfType)
extern PyTypeObject EPObjectIfType;
PyObject* __EPDECL newEPObjectIf(struct ObjectIf);
PyObject* __EPDECL newEPObjectIf(struct ObjectIf*);
PyObject* EPObjectIf_AddChildObject( EPObjectIf* self,EPObjectIf* var1 );
PyObject* EPObjectIf_AddRef( EPObjectIf* self );
PyObject* EPObjectIf_Cast( EPObjectIf* self,EPObjectIf* var1 );
PyObject* EPObjectIf_Clear( EPObjectIf* self );
PyObject* EPObjectIf_ConstructState( EPObjectIf* self,EPvoid* var1 );
PyObject* EPObjectIf_CreateObject( EPObjectIf* self,PyObject* tuple );
PyObject* EPObjectIf_DelChildObject( EPObjectIf* self,EPObjectIf* var1 );
PyObject* EPObjectIf_DelRef( EPObjectIf* self );
PyObject* EPObjectIf_DestructState( EPObjectIf* self,EPvoid* var1 );
PyObject* EPObjectIf_DumpObjectR( EPObjectIf* self,PyObject* tuple );//Overloaded
PyObject* EPObjectIf_GetChildObject( EPObjectIf* self,PyObject* tuple );//Overloaded
PyObject* EPObjectIf_GetDesc( EPObjectIf* self,EPvoid* var1 );
PyObject* EPObjectIf_GetDescAddress( EPObjectIf* self );
PyObject* EPObjectIf_GetDescSize( EPObjectIf* self );
PyObject* EPObjectIf_GetObjectIf( EPObjectIf* self,PyObject* tuple );//Overloaded
PyObject* EPObjectIf_GetState( EPObjectIf* self,EPvoid* var1 );
PyObject* EPObjectIf_GetStateAddress( EPObjectIf* self );
PyObject* EPObjectIf_GetStateSize( EPObjectIf* self );
PyObject* EPObjectIf_NChildObject( EPObjectIf* self );
PyObject* EPObjectIf_Print( EPObjectIf* self,EPostream* var1 );
PyObject* EPObjectIf_PrintShort( EPObjectIf* self,EPostream* var1 );
PyObject* EPObjectIf_ReadState( EPObjectIf* self,EPstring* var1 );
PyObject* EPObjectIf_ReadStateR( EPObjectIf* self,EPistream* var1 );
PyObject* EPObjectIf_RefCount( EPObjectIf* self );
PyObject* EPObjectIf_SetDesc( EPObjectIf* self,EPvoid* var1 );
PyObject* EPObjectIf_SetState( EPObjectIf* self,EPvoid* var1 );
PyObject* EPObjectIf_WriteState( EPObjectIf* self,EPstring* var1 );
PyObject* EPObjectIf_WriteStateR( EPObjectIf* self,EPostream* var1 );

//}EPObjectIf

//{*********EPObjectIfs*******
#define EPObjectIfs_Check(ob) PyObject_TypeCheck(ob, &EPObjectIfsType)
extern PyTypeObject EPObjectIfsType;
PyObject* __EPDECL newEPObjectIfs(struct ObjectIfs);
PyObject* __EPDECL newEPObjectIfs(struct ObjectIfs*);
PyObject* EPObjectIfs_Pop( EPObjectIfs* self );
PyObject* EPObjectIfs_Push( EPObjectIfs* self,EPObjectIf* var1 );
PyObject* EPObjectIfs_Top( EPObjectIfs* self );

//}EPObjectIfs

//{*********EPNamedObjectIf*******
#define EPNamedObjectIf_Check(ob) PyObject_TypeCheck(ob, &EPNamedObjectIfType)
extern PyTypeObject EPNamedObjectIfType;
PyObject* __EPDECL newEPNamedObjectIf(struct NamedObjectIf);
PyObject* __EPDECL newEPNamedObjectIf(struct NamedObjectIf*);
PyObject* EPNamedObjectIf_Cast( EPNamedObjectIf* self,EPObjectIf* var1 );
PyObject* EPNamedObjectIf_GetName( EPNamedObjectIf* self );
PyObject* EPNamedObjectIf_GetNameManager( EPNamedObjectIf* self );
PyObject* EPNamedObjectIf_SetName( EPNamedObjectIf* self,EPchar* var1 );

//}EPNamedObjectIf

//{*********EPSceneObjectIf*******
#define EPSceneObjectIf_Check(ob) PyObject_TypeCheck(ob, &EPSceneObjectIfType)
extern PyTypeObject EPSceneObjectIfType;
PyObject* __EPDECL newEPSceneObjectIf(struct SceneObjectIf);
PyObject* __EPDECL newEPSceneObjectIf(struct SceneObjectIf*);
PyObject* EPSceneObjectIf_Cast( EPSceneObjectIf* self,EPObjectIf* var1 );
PyObject* EPSceneObjectIf_CloneObject( EPSceneObjectIf* self );
PyObject* EPSceneObjectIf_GetScene( EPSceneObjectIf* self );

//}EPSceneObjectIf

//{*********EPObjectStatesIf*******
#define EPObjectStatesIf_Check(ob) PyObject_TypeCheck(ob, &EPObjectStatesIfType)
extern PyTypeObject EPObjectStatesIfType;
PyObject* __EPDECL newEPObjectStatesIf(struct ObjectStatesIf);
PyObject* __EPDECL newEPObjectStatesIf(struct ObjectStatesIf*);
PyObject* EPObjectStatesIf_AllocateState( EPObjectStatesIf* self,EPObjectIf* var1 );
PyObject* EPObjectStatesIf_CalcStateSize( EPObjectStatesIf* self,EPObjectIf* var1 );
PyObject* EPObjectStatesIf_Cast( EPObjectStatesIf* self,EPObjectIf* var1 );
PyObject* EPObjectStatesIf_Create( EPObjectStatesIf* self );
PyObject* EPObjectStatesIf_LoadState( EPObjectStatesIf* self,EPObjectIf* var1 );
PyObject* EPObjectStatesIf_ReleaseState( EPObjectStatesIf* self,EPObjectIf* var1 );
PyObject* EPObjectStatesIf_SaveState( EPObjectStatesIf* self,EPObjectIf* var1 );

//}EPObjectStatesIf

//{*********EPNameManagerIf*******
#define EPNameManagerIf_Check(ob) PyObject_TypeCheck(ob, &EPNameManagerIfType)
extern PyTypeObject EPNameManagerIfType;
PyObject* __EPDECL newEPNameManagerIf(struct NameManagerIf);
PyObject* __EPDECL newEPNameManagerIf(struct NameManagerIf*);
PyObject* EPNameManagerIf_Cast( EPNameManagerIf* self,EPObjectIf* var1 );
PyObject* EPNameManagerIf_FindObject( EPNameManagerIf* self,PyObject* tuple );//Overloaded

//}EPNameManagerIf

//{*********EPSceneIf*******
#define EPSceneIf_Check(ob) PyObject_TypeCheck(ob, &EPSceneIfType)
extern PyTypeObject EPSceneIfType;
PyObject* __EPDECL newEPSceneIf(struct SceneIf);
PyObject* __EPDECL newEPSceneIf(struct SceneIf*);
PyObject* EPSceneIf_Cast( EPSceneIf* self,EPObjectIf* var1 );

//}EPSceneIf

//{*********EPSdkIf*******
#define EPSdkIf_Check(ob) PyObject_TypeCheck(ob, &EPSdkIfType)
extern PyTypeObject EPSdkIfType;
PyObject* __EPDECL newEPSdkIf(struct SdkIf);
PyObject* __EPDECL newEPSdkIf(struct SdkIf*);
PyObject* EPSdkIf_Cast( EPSdkIf* self,EPObjectIf* var1 );
PyObject* EPSdkIf_CreateSdk( EPSdkIf* self,PyObject* tuple );

//}EPSdkIf
void __EPDECL initFoundation(void) ;
#endif
