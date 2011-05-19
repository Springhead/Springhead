#ifndef SprEPGraphics_H
#define SprEPGraphics_H
#include "..\..\include\Springhead.h"
#include "..\..\include\base\Env.h"
#include "..\..\include\Python\Add_decl.h"
#include "..\..\include\Python\pyport.h"
#include "..\..\include\Python\patchlevel.h"
#include "..\..\include\Python\Python.h"
#include "..\..\include\Python\object.h"
#include "..\..\include\EmbPython\SprEPUtility.h"
#include "..\..\include\EmbPython\SprEPFoundation.h"
#include "..\..\include\EmbPython\SprEPPhysics.h"
#include "..\..\include\Graphics\SprGRBlendMesh.h"
#include "..\..\include\Graphics\SprGRFrame.h"
#include "..\..\include\Graphics\SprGRMesh.h"
#include "..\..\include\Graphics\SprGRRender.h"
#include "..\..\include\Graphics\SprGRScene.h"
#include "..\..\include\Graphics\SprGRSdk.h"
#include "..\..\include\Graphics\SprGRShader.h"
#include "..\..\include\Graphics\SprGRSphere.h"
#include "..\..\include\Graphics\SprGRVertex.h"
typedef struct
{
	PyObject_HEAD
	struct GRBlendMeshDesc* ptr;
} EPGRBlendMeshDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRBlendMeshIf> ptr;
} EPGRBlendMeshIf;
typedef struct
{
	PyObject_HEAD
	struct GRVisualDesc* ptr;
} EPGRVisualDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRVisualIf> ptr;
} EPGRVisualIf;
typedef struct
{
	PyObject_HEAD
	struct GRFrameTransformMatrix* ptr;
} EPGRFrameTransformMatrix;
typedef struct
{
	PyObject_HEAD
	UTRef<GRFrameIf> ptr;
} EPGRFrameIf;
typedef struct
{
	PyObject_HEAD
	struct GRFrameDesc* ptr;
} EPGRFrameDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRDummyFrameIf> ptr;
} EPGRDummyFrameIf;
typedef struct
{
	PyObject_HEAD
	struct GRDummyFrameDesc* ptr;
} EPGRDummyFrameDesc;
typedef struct
{
	PyObject_HEAD
	struct GRKey* ptr;
} EPGRKey;
typedef struct
{
	PyObject_HEAD
	struct GRAnimationKey* ptr;
} EPGRAnimationKey;
typedef struct
{
	PyObject_HEAD
	UTRef<GRAnimationIf> ptr;
} EPGRAnimationIf;
typedef struct
{
	PyObject_HEAD
	struct GRAnimationDesc* ptr;
} EPGRAnimationDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRAnimationSetIf> ptr;
} EPGRAnimationSetIf;
typedef struct
{
	PyObject_HEAD
	struct GRAnimationSetDesc* ptr;
} EPGRAnimationSetDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRAnimationControllerIf> ptr;
} EPGRAnimationControllerIf;
typedef struct
{
	PyObject_HEAD
	struct GRAnimationControllerDesc* ptr;
} EPGRAnimationControllerDesc;
typedef struct
{
	PyObject_HEAD
	struct GRMeshFace* ptr;
} EPGRMeshFace;
typedef struct
{
	PyObject_HEAD
	UTRef<GRSkinWeightIf> ptr;
} EPGRSkinWeightIf;
typedef struct
{
	PyObject_HEAD
	struct GRSkinWeightDesc* ptr;
} EPGRSkinWeightDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRMeshIf> ptr;
} EPGRMeshIf;
typedef struct
{
	PyObject_HEAD
	struct GRMeshDesc* ptr;
} EPGRMeshDesc;
typedef struct
{
	PyObject_HEAD
	GRFont* ptr;
} EPGRFont;
typedef struct
{
	PyObject_HEAD
	UTRef<GRLightIf> ptr;
} EPGRLightIf;
typedef struct
{
	PyObject_HEAD
	struct GRLightDesc* ptr;
} EPGRLightDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRMaterialIf> ptr;
} EPGRMaterialIf;
typedef struct
{
	PyObject_HEAD
	struct GRMaterialDesc* ptr;
} EPGRMaterialDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRCameraIf> ptr;
} EPGRCameraIf;
typedef struct
{
	PyObject_HEAD
	struct GRCameraDesc* ptr;
} EPGRCameraDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRRenderBaseIf> ptr;
} EPGRRenderBaseIf;
typedef struct
{
	PyObject_HEAD
	UTRef<GRRenderIf> ptr;
} EPGRRenderIf;
typedef struct
{
	PyObject_HEAD
	UTRef<GRDeviceIf> ptr;
} EPGRDeviceIf;
typedef struct
{
	PyObject_HEAD
	UTRef<GRDeviceGLIf> ptr;
} EPGRDeviceGLIf;
typedef struct
{
	PyObject_HEAD
	struct GRSceneDesc* ptr;
} EPGRSceneDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRSceneIf> ptr;
} EPGRSceneIf;
typedef struct
{
	PyObject_HEAD
	struct GRSdkDesc* ptr;
} EPGRSdkDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRSdkIf> ptr;
} EPGRSdkIf;
typedef struct
{
	PyObject_HEAD
	struct SFBlendLocation* ptr;
} EPSFBlendLocation;
typedef struct
{
	PyObject_HEAD
	struct GRShaderFormat* ptr;
} EPGRShaderFormat;
typedef struct
{
	PyObject_HEAD
	struct GRSphereDesc* ptr;
} EPGRSphereDesc;
typedef struct
{
	PyObject_HEAD
	UTRef<GRSphereIf> ptr;
} EPGRSphereIf;
typedef struct
{
	PyObject_HEAD
	struct GRVertexElement* ptr;
} EPGRVertexElement;
typedef struct
{
	PyObject_HEAD
	struct GRVertexArray* ptr;
} EPGRVertexArray;

//{*********EPGRBlendMeshDesc*******
#define EPGRBlendMeshDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRBlendMeshDescType)
extern PyTypeObject EPGRBlendMeshDescType;
PyObject* __EPDECL newEPGRBlendMeshDesc(struct GRBlendMeshDesc);
PyObject* __EPDECL newEPGRBlendMeshDesc(struct GRBlendMeshDesc*);

//}EPGRBlendMeshDesc

//{*********EPGRBlendMeshIf*******
#define EPGRBlendMeshIf_Check(ob) PyObject_TypeCheck(ob, &EPGRBlendMeshIfType)
extern PyTypeObject EPGRBlendMeshIfType;
PyObject* __EPDECL newEPGRBlendMeshIf(struct GRBlendMeshIf);
PyObject* __EPDECL newEPGRBlendMeshIf(struct GRBlendMeshIf*);

//}EPGRBlendMeshIf

//{*********EPGRVisualDesc*******
#define EPGRVisualDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRVisualDescType)
extern PyTypeObject EPGRVisualDescType;
PyObject* __EPDECL newEPGRVisualDesc(struct GRVisualDesc);
PyObject* __EPDECL newEPGRVisualDesc(struct GRVisualDesc*);

//}EPGRVisualDesc

//{*********EPGRVisualIf*******
#define EPGRVisualIf_Check(ob) PyObject_TypeCheck(ob, &EPGRVisualIfType)
extern PyTypeObject EPGRVisualIfType;
PyObject* __EPDECL newEPGRVisualIf(struct GRVisualIf);
PyObject* __EPDECL newEPGRVisualIf(struct GRVisualIf*);
PyObject* EPGRVisualIf_Enable( EPGRVisualIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRVisualIf_IsEnabled( EPGRVisualIf* self );
PyObject* EPGRVisualIf_Render( EPGRVisualIf* self,EPGRRenderIf* var1 );
PyObject* EPGRVisualIf_Rendered( EPGRVisualIf* self,EPGRRenderIf* var1 );

//}EPGRVisualIf

//{*********EPGRFrameTransformMatrix*******
#define EPGRFrameTransformMatrix_Check(ob) PyObject_TypeCheck(ob, &EPGRFrameTransformMatrixType)
extern PyTypeObject EPGRFrameTransformMatrixType;
PyObject* __EPDECL newEPGRFrameTransformMatrix(struct GRFrameTransformMatrix);
PyObject* __EPDECL newEPGRFrameTransformMatrix(struct GRFrameTransformMatrix*);

//}EPGRFrameTransformMatrix

//{*********EPGRFrameIf*******
#define EPGRFrameIf_Check(ob) PyObject_TypeCheck(ob, &EPGRFrameIfType)
extern PyTypeObject EPGRFrameIfType;
PyObject* __EPDECL newEPGRFrameIf(struct GRFrameIf);
PyObject* __EPDECL newEPGRFrameIf(struct GRFrameIf*);
PyObject* EPGRFrameIf_Clear( EPGRFrameIf* self );
PyObject* EPGRFrameIf_GetParent( EPGRFrameIf* self );
PyObject* EPGRFrameIf_GetTransform( EPGRFrameIf* self );
PyObject* EPGRFrameIf_GetWorldTransform( EPGRFrameIf* self );
PyObject* EPGRFrameIf_NChildren( EPGRFrameIf* self );
PyObject* EPGRFrameIf_Print( EPGRFrameIf* self,EPostream* var1 );
PyObject* EPGRFrameIf_SetParent( EPGRFrameIf* self,EPGRFrameIf* var1 );
PyObject* EPGRFrameIf_SetTransform( EPGRFrameIf* self,EPAffinef* var1 );

//}EPGRFrameIf

//{*********EPGRFrameDesc*******
#define EPGRFrameDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRFrameDescType)
extern PyTypeObject EPGRFrameDescType;
PyObject* __EPDECL newEPGRFrameDesc(struct GRFrameDesc);
PyObject* __EPDECL newEPGRFrameDesc(struct GRFrameDesc*);

//}EPGRFrameDesc

//{*********EPGRDummyFrameIf*******
#define EPGRDummyFrameIf_Check(ob) PyObject_TypeCheck(ob, &EPGRDummyFrameIfType)
extern PyTypeObject EPGRDummyFrameIfType;
PyObject* __EPDECL newEPGRDummyFrameIf(struct GRDummyFrameIf);
PyObject* __EPDECL newEPGRDummyFrameIf(struct GRDummyFrameIf*);

//}EPGRDummyFrameIf

//{*********EPGRDummyFrameDesc*******
#define EPGRDummyFrameDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRDummyFrameDescType)
extern PyTypeObject EPGRDummyFrameDescType;
PyObject* __EPDECL newEPGRDummyFrameDesc(struct GRDummyFrameDesc);
PyObject* __EPDECL newEPGRDummyFrameDesc(struct GRDummyFrameDesc*);

//}EPGRDummyFrameDesc

//{*********EPGRKey*******
#define EPGRKey_Check(ob) PyObject_TypeCheck(ob, &EPGRKeyType)
extern PyTypeObject EPGRKeyType;
PyObject* __EPDECL newEPGRKey(struct GRKey);
PyObject* __EPDECL newEPGRKey(struct GRKey*);

//}EPGRKey

//{*********EPGRAnimationKey*******
#define EPGRAnimationKey_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationKeyType)
extern PyTypeObject EPGRAnimationKeyType;
PyObject* __EPDECL newEPGRAnimationKey(struct GRAnimationKey);
PyObject* __EPDECL newEPGRAnimationKey(struct GRAnimationKey*);

//}EPGRAnimationKey

//{*********EPGRAnimationIf*******
#define EPGRAnimationIf_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationIfType)
extern PyTypeObject EPGRAnimationIfType;
PyObject* __EPDECL newEPGRAnimationIf(struct GRAnimationIf);
PyObject* __EPDECL newEPGRAnimationIf(struct GRAnimationIf*);
PyObject* EPGRAnimationIf_BlendPose( EPGRAnimationIf* self,PyObject* tuple );
PyObject* EPGRAnimationIf_DeletePose( EPGRAnimationIf* self,PyFloatObject* var1 );
PyObject* EPGRAnimationIf_GetAnimationKey( EPGRAnimationIf* self,PyLongObject* var1 );
PyObject* EPGRAnimationIf_LoadInitialPose( EPGRAnimationIf* self );
PyObject* EPGRAnimationIf_NAnimationKey( EPGRAnimationIf* self );
PyObject* EPGRAnimationIf_ResetPose( EPGRAnimationIf* self );

//}EPGRAnimationIf

//{*********EPGRAnimationDesc*******
#define EPGRAnimationDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationDescType)
extern PyTypeObject EPGRAnimationDescType;
PyObject* __EPDECL newEPGRAnimationDesc(struct GRAnimationDesc);
PyObject* __EPDECL newEPGRAnimationDesc(struct GRAnimationDesc*);

//}EPGRAnimationDesc

//{*********EPGRAnimationSetIf*******
#define EPGRAnimationSetIf_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationSetIfType)
extern PyTypeObject EPGRAnimationSetIfType;
PyObject* __EPDECL newEPGRAnimationSetIf(struct GRAnimationSetIf);
PyObject* __EPDECL newEPGRAnimationSetIf(struct GRAnimationSetIf*);
PyObject* EPGRAnimationSetIf_BlendPose( EPGRAnimationSetIf* self,PyObject* tuple );
PyObject* EPGRAnimationSetIf_DeleteAnimationPose( EPGRAnimationSetIf* self,PyFloatObject* var1 );
PyObject* EPGRAnimationSetIf_GetChildObject( EPGRAnimationSetIf* self,PyLongObject* var1 );
PyObject* EPGRAnimationSetIf_GetLastKeyTime( EPGRAnimationSetIf* self );
PyObject* EPGRAnimationSetIf_LoadInitialPose( EPGRAnimationSetIf* self );
PyObject* EPGRAnimationSetIf_ResetPose( EPGRAnimationSetIf* self );
PyObject* EPGRAnimationSetIf_SetCurrentAnimationPose( EPGRAnimationSetIf* self,PyFloatObject* var1 );

//}EPGRAnimationSetIf

//{*********EPGRAnimationSetDesc*******
#define EPGRAnimationSetDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationSetDescType)
extern PyTypeObject EPGRAnimationSetDescType;
PyObject* __EPDECL newEPGRAnimationSetDesc(struct GRAnimationSetDesc);
PyObject* __EPDECL newEPGRAnimationSetDesc(struct GRAnimationSetDesc*);

//}EPGRAnimationSetDesc

//{*********EPGRAnimationControllerIf*******
#define EPGRAnimationControllerIf_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationControllerIfType)
extern PyTypeObject EPGRAnimationControllerIfType;
PyObject* __EPDECL newEPGRAnimationControllerIf(struct GRAnimationControllerIf);
PyObject* __EPDECL newEPGRAnimationControllerIf(struct GRAnimationControllerIf*);
PyObject* EPGRAnimationControllerIf_AddChildObject( EPGRAnimationControllerIf* self,EPObjectIf* var1 );
PyObject* EPGRAnimationControllerIf_BlendPose( EPGRAnimationControllerIf* self,PyObject* tuple );
PyObject* EPGRAnimationControllerIf_DelChildObject( EPGRAnimationControllerIf* self,EPObjectIf* var1 );
PyObject* EPGRAnimationControllerIf_GetAnimationSet( EPGRAnimationControllerIf* self,PyLongObject* var1 );
PyObject* EPGRAnimationControllerIf_GetChildObject( EPGRAnimationControllerIf* self,PyLongObject* var1 );
PyObject* EPGRAnimationControllerIf_LoadInitialPose( EPGRAnimationControllerIf* self );
PyObject* EPGRAnimationControllerIf_NChildObject( EPGRAnimationControllerIf* self );
PyObject* EPGRAnimationControllerIf_ResetPose( EPGRAnimationControllerIf* self );

//}EPGRAnimationControllerIf

//{*********EPGRAnimationControllerDesc*******
#define EPGRAnimationControllerDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRAnimationControllerDescType)
extern PyTypeObject EPGRAnimationControllerDescType;
PyObject* __EPDECL newEPGRAnimationControllerDesc(struct GRAnimationControllerDesc);
PyObject* __EPDECL newEPGRAnimationControllerDesc(struct GRAnimationControllerDesc*);

//}EPGRAnimationControllerDesc

//{*********EPGRMeshFace*******
#define EPGRMeshFace_Check(ob) PyObject_TypeCheck(ob, &EPGRMeshFaceType)
extern PyTypeObject EPGRMeshFaceType;
PyObject* __EPDECL newEPGRMeshFace(struct GRMeshFace);
PyObject* __EPDECL newEPGRMeshFace(struct GRMeshFace*);

//}EPGRMeshFace

//{*********EPGRSkinWeightIf*******
#define EPGRSkinWeightIf_Check(ob) PyObject_TypeCheck(ob, &EPGRSkinWeightIfType)
extern PyTypeObject EPGRSkinWeightIfType;
PyObject* __EPDECL newEPGRSkinWeightIf(struct GRSkinWeightIf);
PyObject* __EPDECL newEPGRSkinWeightIf(struct GRSkinWeightIf*);

//}EPGRSkinWeightIf

//{*********EPGRSkinWeightDesc*******
#define EPGRSkinWeightDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRSkinWeightDescType)
extern PyTypeObject EPGRSkinWeightDescType;
PyObject* __EPDECL newEPGRSkinWeightDesc(struct GRSkinWeightDesc);
PyObject* __EPDECL newEPGRSkinWeightDesc(struct GRSkinWeightDesc*);

//}EPGRSkinWeightDesc

//{*********EPGRMeshIf*******
#define EPGRMeshIf_Check(ob) PyObject_TypeCheck(ob, &EPGRMeshIfType)
extern PyTypeObject EPGRMeshIfType;
PyObject* __EPDECL newEPGRMeshIf(struct GRMeshIf);
PyObject* __EPDECL newEPGRMeshIf(struct GRMeshIf*);
PyObject* EPGRMeshIf_CreateSkinWeight( EPGRMeshIf* self,EPGRSkinWeightDesc* var1 );
PyObject* EPGRMeshIf_EnableTex3D( EPGRMeshIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRMeshIf_GetColors( EPGRMeshIf* self );
PyObject* EPGRMeshIf_GetFaceNormals( EPGRMeshIf* self );
PyObject* EPGRMeshIf_GetFaces( EPGRMeshIf* self );
PyObject* EPGRMeshIf_GetMaterialIndices( EPGRMeshIf* self );
PyObject* EPGRMeshIf_GetNormals( EPGRMeshIf* self );
PyObject* EPGRMeshIf_GetTexCoords( EPGRMeshIf* self );
PyObject* EPGRMeshIf_GetVertices( EPGRMeshIf* self );
PyObject* EPGRMeshIf_IsTex3D( EPGRMeshIf* self );
PyObject* EPGRMeshIf_NFace( EPGRMeshIf* self );
PyObject* EPGRMeshIf_NTriangle( EPGRMeshIf* self );
PyObject* EPGRMeshIf_NVertex( EPGRMeshIf* self );
PyObject* EPGRMeshIf_SwitchCoordinate( EPGRMeshIf* self );

//}EPGRMeshIf

//{*********EPGRMeshDesc*******
#define EPGRMeshDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRMeshDescType)
extern PyTypeObject EPGRMeshDescType;
PyObject* __EPDECL newEPGRMeshDesc(struct GRMeshDesc);
PyObject* __EPDECL newEPGRMeshDesc(struct GRMeshDesc*);

//}EPGRMeshDesc

//{*********EPGRFont*******
#define EPGRFont_Check(ob) PyObject_TypeCheck(ob, &EPGRFontType)
extern PyTypeObject EPGRFontType;
PyObject* __EPDECL newEPGRFont(GRFont);
PyObject* __EPDECL newEPGRFont(GRFont*);
PyObject* EPGRFont_SetColor( EPGRFont* self,EPunsignedlong* var1 );

//}EPGRFont

//{*********EPGRLightIf*******
#define EPGRLightIf_Check(ob) PyObject_TypeCheck(ob, &EPGRLightIfType)
extern PyTypeObject EPGRLightIfType;
PyObject* __EPDECL newEPGRLightIf(struct GRLightIf);
PyObject* __EPDECL newEPGRLightIf(struct GRLightIf*);

//}EPGRLightIf

//{*********EPGRLightDesc*******
#define EPGRLightDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRLightDescType)
extern PyTypeObject EPGRLightDescType;
PyObject* __EPDECL newEPGRLightDesc(struct GRLightDesc);
PyObject* __EPDECL newEPGRLightDesc(struct GRLightDesc*);

//}EPGRLightDesc

//{*********EPGRMaterialIf*******
#define EPGRMaterialIf_Check(ob) PyObject_TypeCheck(ob, &EPGRMaterialIfType)
extern PyTypeObject EPGRMaterialIfType;
PyObject* __EPDECL newEPGRMaterialIf(struct GRMaterialIf);
PyObject* __EPDECL newEPGRMaterialIf(struct GRMaterialIf*);
PyObject* EPGRMaterialIf_IsOpaque( EPGRMaterialIf* self );

//}EPGRMaterialIf

//{*********EPGRMaterialDesc*******
#define EPGRMaterialDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRMaterialDescType)
extern PyTypeObject EPGRMaterialDescType;
PyObject* __EPDECL newEPGRMaterialDesc(struct GRMaterialDesc);
PyObject* __EPDECL newEPGRMaterialDesc(struct GRMaterialDesc*);
PyObject* EPGRMaterialDesc_Is3D( EPGRMaterialDesc* self );
PyObject* EPGRMaterialDesc_IsOpaque( EPGRMaterialDesc* self );

//}EPGRMaterialDesc

//{*********EPGRCameraIf*******
#define EPGRCameraIf_Check(ob) PyObject_TypeCheck(ob, &EPGRCameraIfType)
extern PyTypeObject EPGRCameraIfType;
PyObject* __EPDECL newEPGRCameraIf(struct GRCameraIf);
PyObject* __EPDECL newEPGRCameraIf(struct GRCameraIf*);
PyObject* EPGRCameraIf_GetFrame( EPGRCameraIf* self );
PyObject* EPGRCameraIf_SetFrame( EPGRCameraIf* self,EPGRFrameIf* var1 );

//}EPGRCameraIf

//{*********EPGRCameraDesc*******
#define EPGRCameraDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRCameraDescType)
extern PyTypeObject EPGRCameraDescType;
PyObject* __EPDECL newEPGRCameraDesc(struct GRCameraDesc);
PyObject* __EPDECL newEPGRCameraDesc(struct GRCameraDesc*);

//}EPGRCameraDesc

//{*********EPGRRenderBaseIf*******
#define EPGRRenderBaseIf_Check(ob) PyObject_TypeCheck(ob, &EPGRRenderBaseIfType)
extern PyTypeObject EPGRRenderBaseIfType;
PyObject* __EPDECL newEPGRRenderBaseIf(struct GRRenderBaseIf);
PyObject* __EPDECL newEPGRRenderBaseIf(struct GRRenderBaseIf*);
PyObject* EPGRRenderBaseIf_BeginScene( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_ClearBlendMatrix( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_ClearBuffer( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_DrawArrow( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_DrawBox( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawCapsule( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawCone( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawCylinder( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawDirect( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawFont( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawGrid( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawLine( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_DrawList( EPGRRenderBaseIf* self,PyLongObject* var1 );
PyObject* EPGRRenderBaseIf_DrawRoundCone( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_DrawSphere( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_EndList( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_EndScene( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_GetClearColor( EPGRRenderBaseIf* self,EPVec4f* var1 );
PyObject* EPGRRenderBaseIf_GetModelMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_GetProjectionMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_GetShaderLocation( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_GetViewMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_InitShader( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_LoadTexture( EPGRRenderBaseIf* self,EPstring* var1 );
PyObject* EPGRRenderBaseIf_MultModelMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_NLights( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_PopLight( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_PopModelMatrix( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_PushLight( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_PushModelMatrix( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_ReadShaderSource( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_ReleaseList( EPGRRenderBaseIf* self,PyLongObject* var1 );
PyObject* EPGRRenderBaseIf_SetAlphaMode( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_SetAlphaTest( EPGRRenderBaseIf* self,PyObject* var1 );
PyObject* EPGRRenderBaseIf_SetBlendMatrix( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_SetClearColor( EPGRRenderBaseIf* self,EPVec4f* var1 );
PyObject* EPGRRenderBaseIf_SetDepthFunc( EPGRRenderBaseIf* self,PyLongObject* var1 );
PyObject* EPGRRenderBaseIf_SetDepthTest( EPGRRenderBaseIf* self,PyObject* var1 );
PyObject* EPGRRenderBaseIf_SetDepthWrite( EPGRRenderBaseIf* self,PyObject* var1 );
PyObject* EPGRRenderBaseIf_SetFont( EPGRRenderBaseIf* self,EPGRFont* var1 );
PyObject* EPGRRenderBaseIf_SetLighting( EPGRRenderBaseIf* self,PyObject* var1 );
PyObject* EPGRRenderBaseIf_SetLineWidth( EPGRRenderBaseIf* self,PyFloatObject* var1 );
PyObject* EPGRRenderBaseIf_SetMaterial( EPGRRenderBaseIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderBaseIf_SetModelMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_SetProjectionMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_SetShaderFormat( EPGRRenderBaseIf* self,PyLongObject* var1 );
PyObject* EPGRRenderBaseIf_SetTextureImage( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_SetVertexFormat( EPGRRenderBaseIf* self,EPGRVertexElement* var1 );
PyObject* EPGRRenderBaseIf_SetVertexShader( EPGRRenderBaseIf* self,EPvoid* var1 );
PyObject* EPGRRenderBaseIf_SetViewMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 );
PyObject* EPGRRenderBaseIf_SetViewport( EPGRRenderBaseIf* self,PyObject* tuple );
PyObject* EPGRRenderBaseIf_StartList( EPGRRenderBaseIf* self );
PyObject* EPGRRenderBaseIf_SwapBuffers( EPGRRenderBaseIf* self );

//}EPGRRenderBaseIf

//{*********EPGRRenderIf*******
#define EPGRRenderIf_Check(ob) PyObject_TypeCheck(ob, &EPGRRenderIfType)
extern PyTypeObject EPGRRenderIfType;
PyObject* __EPDECL newEPGRRenderIf(struct GRRenderIf);
PyObject* __EPDECL newEPGRRenderIf(struct GRRenderIf*);
PyObject* EPGRRenderIf_EnterScreenCoordinate( EPGRRenderIf* self );
PyObject* EPGRRenderIf_GetCamera( EPGRRenderIf* self );
PyObject* EPGRRenderIf_GetDevice( EPGRRenderIf* self );
PyObject* EPGRRenderIf_GetPixelSize( EPGRRenderIf* self );
PyObject* EPGRRenderIf_GetViewportPos( EPGRRenderIf* self );
PyObject* EPGRRenderIf_GetViewportSize( EPGRRenderIf* self );
PyObject* EPGRRenderIf_LeaveScreenCoordinate( EPGRRenderIf* self );
PyObject* EPGRRenderIf_Reshape( EPGRRenderIf* self,PyObject* tuple );
PyObject* EPGRRenderIf_ScreenToCamera( EPGRRenderIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRRenderIf_SetCamera( EPGRRenderIf* self,EPGRCameraDesc* var1 );
PyObject* EPGRRenderIf_SetDevice( EPGRRenderIf* self,EPGRDeviceIf* var1 );

//}EPGRRenderIf

//{*********EPGRDeviceIf*******
#define EPGRDeviceIf_Check(ob) PyObject_TypeCheck(ob, &EPGRDeviceIfType)
extern PyTypeObject EPGRDeviceIfType;
PyObject* __EPDECL newEPGRDeviceIf(struct GRDeviceIf);
PyObject* __EPDECL newEPGRDeviceIf(struct GRDeviceIf*);
PyObject* EPGRDeviceIf_Init( EPGRDeviceIf* self );
PyObject* EPGRDeviceIf_Print( EPGRDeviceIf* self,EPostream* var1 );

//}EPGRDeviceIf

//{*********EPGRDeviceGLIf*******
#define EPGRDeviceGLIf_Check(ob) PyObject_TypeCheck(ob, &EPGRDeviceGLIfType)
extern PyTypeObject EPGRDeviceGLIfType;
PyObject* __EPDECL newEPGRDeviceGLIf(struct GRDeviceGLIf);
PyObject* __EPDECL newEPGRDeviceGLIf(struct GRDeviceGLIf*);

//}EPGRDeviceGLIf

//{*********EPGRSceneDesc*******
#define EPGRSceneDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRSceneDescType)
extern PyTypeObject EPGRSceneDescType;
PyObject* __EPDECL newEPGRSceneDesc(struct GRSceneDesc);
PyObject* __EPDECL newEPGRSceneDesc(struct GRSceneDesc*);

//}EPGRSceneDesc

//{*********EPGRSceneIf*******
#define EPGRSceneIf_Check(ob) PyObject_TypeCheck(ob, &EPGRSceneIfType)
extern PyTypeObject EPGRSceneIfType;
PyObject* __EPDECL newEPGRSceneIf(struct GRSceneIf);
PyObject* __EPDECL newEPGRSceneIf(struct GRSceneIf*);
PyObject* EPGRSceneIf_CreateVisual( EPGRSceneIf* self,PyObject* tuple );//Overloaded
PyObject* EPGRSceneIf_GetAnimationController( EPGRSceneIf* self );
PyObject* EPGRSceneIf_GetCamera( EPGRSceneIf* self );
PyObject* EPGRSceneIf_GetSdk( EPGRSceneIf* self );
PyObject* EPGRSceneIf_GetWorld( EPGRSceneIf* self );
PyObject* EPGRSceneIf_Render( EPGRSceneIf* self,EPGRRenderIf* var1 );
PyObject* EPGRSceneIf_SetCamera( EPGRSceneIf* self,EPGRCameraDesc* var1 );

//}EPGRSceneIf

//{*********EPGRSdkDesc*******
#define EPGRSdkDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRSdkDescType)
extern PyTypeObject EPGRSdkDescType;
PyObject* __EPDECL newEPGRSdkDesc(struct GRSdkDesc);
PyObject* __EPDECL newEPGRSdkDesc(struct GRSdkDesc*);

//}EPGRSdkDesc

//{*********EPGRSdkIf*******
#define EPGRSdkIf_Check(ob) PyObject_TypeCheck(ob, &EPGRSdkIfType)
extern PyTypeObject EPGRSdkIfType;
PyObject* __EPDECL newEPGRSdkIf(struct GRSdkIf);
PyObject* __EPDECL newEPGRSdkIf(struct GRSdkIf*);
PyObject* EPGRSdkIf_CreateDeviceGL( EPGRSdkIf* self );
PyObject* EPGRSdkIf_CreateRender( EPGRSdkIf* self );
PyObject* EPGRSdkIf_CreateScene( EPGRSdkIf* self,EPGRSceneDesc* var1 );
PyObject* EPGRSdkIf_CreateSdk( EPGRSdkIf* self );
PyObject* EPGRSdkIf_GetScene( EPGRSdkIf* self,PyLongObject* var1 );
PyObject* EPGRSdkIf_MergeScene( EPGRSdkIf* self,PyObject* tuple );
PyObject* EPGRSdkIf_NScene( EPGRSdkIf* self );
PyObject* EPGRSdkIf_RegisterSdk( EPGRSdkIf* self );

//}EPGRSdkIf

//{*********EPSFBlendLocation*******
#define EPSFBlendLocation_Check(ob) PyObject_TypeCheck(ob, &EPSFBlendLocationType)
extern PyTypeObject EPSFBlendLocationType;
PyObject* __EPDECL newEPSFBlendLocation(struct SFBlendLocation);
PyObject* __EPDECL newEPSFBlendLocation(struct SFBlendLocation*);

//}EPSFBlendLocation

//{*********EPGRShaderFormat*******
#define EPGRShaderFormat_Check(ob) PyObject_TypeCheck(ob, &EPGRShaderFormatType)
extern PyTypeObject EPGRShaderFormatType;
PyObject* __EPDECL newEPGRShaderFormat(struct GRShaderFormat);
PyObject* __EPDECL newEPGRShaderFormat(struct GRShaderFormat*);

//}EPGRShaderFormat

//{*********EPGRSphereDesc*******
#define EPGRSphereDesc_Check(ob) PyObject_TypeCheck(ob, &EPGRSphereDescType)
extern PyTypeObject EPGRSphereDescType;
PyObject* __EPDECL newEPGRSphereDesc(struct GRSphereDesc);
PyObject* __EPDECL newEPGRSphereDesc(struct GRSphereDesc*);

//}EPGRSphereDesc

//{*********EPGRSphereIf*******
#define EPGRSphereIf_Check(ob) PyObject_TypeCheck(ob, &EPGRSphereIfType)
extern PyTypeObject EPGRSphereIfType;
PyObject* __EPDECL newEPGRSphereIf(struct GRSphereIf);
PyObject* __EPDECL newEPGRSphereIf(struct GRSphereIf*);

//}EPGRSphereIf

//{*********EPGRVertexElement*******
#define EPGRVertexElement_Check(ob) PyObject_TypeCheck(ob, &EPGRVertexElementType)
extern PyTypeObject EPGRVertexElementType;
PyObject* __EPDECL newEPGRVertexElement(struct GRVertexElement);
PyObject* __EPDECL newEPGRVertexElement(struct GRVertexElement*);
PyObject* EPGRVertexElement_Blend1f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_Blend2f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_Blend3f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_Blend4f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_BlendNf( EPGRVertexElement* self,PyObject* tuple );
PyObject* EPGRVertexElement_Color3f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_Color4b( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_Color4f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_MatrixIndices4f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_MatrixIndicesNf( EPGRVertexElement* self,PyObject* tuple );
PyObject* EPGRVertexElement_Normal3f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_NumBones4f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_NumBonesNf( EPGRVertexElement* self,PyObject* tuple );
PyObject* EPGRVertexElement_Position3f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_Position4f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_TexCoords2f( EPGRVertexElement* self,PyLongObject* var1 );
PyObject* EPGRVertexElement_TexCoords4f( EPGRVertexElement* self,PyLongObject* var1 );

//}EPGRVertexElement

//{*********EPGRVertexArray*******
#define EPGRVertexArray_Check(ob) PyObject_TypeCheck(ob, &EPGRVertexArrayType)
extern PyTypeObject EPGRVertexArrayType;
PyObject* __EPDECL newEPGRVertexArray(struct GRVertexArray);
PyObject* __EPDECL newEPGRVertexArray(struct GRVertexArray*);

//}EPGRVertexArray
void __EPDECL initGraphics(void) ;
#endif
