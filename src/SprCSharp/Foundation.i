#	Do not edit. RunSwig.bat will update this file.
%ignore Spr::UTTimerIf::GetCallback;
%ignore Spr::UTTimerIf::SetCallback;
// %ignore Spr::ObjectIf::CreateObject;
%ignore Spr::ObjectIf::Print;
%ignore Spr::ObjectIf::PrintShort;
%ignore Spr::ObjectIf::WriteStateR;
%ignore Spr::ObjectIf::ReadStateR;
%ignore Spr::ObjectIf::DumpObjectR;
%ignore Spr::SdkIf::CreateSdk;

%feature("struct") Spr::IfInfo;

%feature("operator",
        macro="UNARY    (-,CC)
               BASIC_V  (+,CCC: -,CCC: *,CEC: *,CCE: /,CCE)
               BASIC_M  (+,CCC: -,CCC: *,CEC: *,CCE)
               S_PROD   (*,ECC)
               V_PROD2  (%,ECC: ^,ECC)
               V_PROD3  (%,CCC: ^,CCC)
               Q_PROD   (*,vCv: *,CCC: *,mCm)
               P_PROD   (*,vCv: *,CCC)
               M_PROD   (*,vCv: *,vvC)
               COMP     (==,SCC,,bool: !=,SCC,,bool)

               VECTOR2_OPERATOR(UNARY: BASIC_V: S_PROD: V_PROD2: COMP)
               VECTOR3_OPERATOR(UNARY: BASIC_V: S_PROD: V_PROD3: COMP)
               VECTOR4_OPERATOR(UNARY: BASIC_V: COMP)
               VECTOR6_OPERATOR(UNARY: BASIC_V: COMP)
               MATRIX_OPERATION(UNARY: BASIC_M: M_PROD)
");

%feature("operator", def="VECTOR2_OPERATOR") Spr::Vec2f;
%feature("operator", def="VECTOR2_OPERATOR") Spr::Vec2d;
%feature("operator", def="VECTOR3_OPERATOR") Spr::Vec3f;
%feature("operator", def="VECTOR3_OPERATOR") Spr::Vec3d;
%feature("operator", def="VECTOR4_OPERATOR") Spr::Vec4f;
%feature("operator", def="VECTOR4_OPERATOR") Spr::Vec4d;

%feature("operator", def="Q_PROD") Spr::Quaternionf;
%feature("operator", def="Q_PROD") Spr::Quaterniond;
%feature("operator", def="P_PROD") Spr::Posef;
%feature("operator", def="P_PROD") Spr::Posed;

%feature("operator", def="VECTOR2_OPERATOR") Spr::Vec2i;
%feature("operator", def="VECTOR3_OPERATOR") Spr::Vec3i;
%feature("operator", def="VECTOR4_OPERATOR") Spr::Vec4i;
%feature("operator", def="VECTOR6_OPERATOR") Spr::Vec6f;
%feature("operator", def="VECTOR6_OPERATOR") Spr::Vec6d;

%feature("operator", def="MATRIX_OPERATION") Spr::Matrix3f;
%feature("operator", def="MATRIX_OPERATION") Spr::Matrix3d;

%feature("withfunc") Spr::Posed;
%feature("withfunc") Spr::Vec3d;
%feature("withfunc") Spr::Vec3f;

%module Foundation
%include "../../include/SprBase.h"
%include "../../include/Base/Env.h"
%include "../../include/Foundation/SprObject.h"
%include "../../include/Foundation/SprScene.h"
%include "../../include/Foundation/SprUTTimer.h"
%include "../Foundation/UTBaseType.h"
