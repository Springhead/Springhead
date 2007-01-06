%module rbPhysics

%{
// win32/win32.hにwriteとreadがマクロ定義されていて，これがstdと衝突する
#undef write
#undef read
#include <SprPhysics.h>
using namespace Spr;
%}

// typemaps
%include <sprtypemaps.i>

//boolのデフォルト引数true or false
//enum型が引数or戻り値
//Posed&
//Vec3d&
//double&
//const Vec3d&
//戻り値でVec3d
//GetSolidsの戻り値でPHSolid**
//enum contactmodeのデフォルト引数
//setcontactmodeの引数でPHSolid**
//引数でVec3f
//Vec3dなどのメンバ変数のget/set

%import	 <Base/Env.h>			// SPR_CDECL

%include <Foundation/SprObject.h>
%include <Foundation/SprScene.h>
%include "Physics/SprPHSdk.h"
%include "Physics/SprPHScene.h"
%include "Physics/SprPHSolid.h"
%include "Physics/SprPHJoint.h"
%include "Physics/SprPHEngine.h"
