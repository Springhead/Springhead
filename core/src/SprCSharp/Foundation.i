#	Do not edit. RunSwig.bat will update this file.
%ignore Spr::UTTimerIf::GetCallback;
%ignore Spr::UTTimerIf::SetCallback;
%ignore Spr::UTTypeDescIf::PrintPool;
%ignore Spr::UTTypeDescIf::Print;
%ignore Spr::UTTypeDescIf::GetFieldType;
%ignore Spr::UTTypeDescIf::Write;
%ignore Spr::UTTypeDescIf::Read;
// %ignore Spr::ObjectIf::CreateObject;
%ignore Spr::ObjectIf::Print;
%ignore Spr::ObjectIf::PrintShort;
%ignore Spr::ObjectIf::WriteStateR;
%ignore Spr::ObjectIf::ReadStateR;
%ignore Spr::ObjectIf::DumpObjectR;
%ignore Spr::SdkIf::CreateSdk;
%ignore Spr::UTPerformanceMeasureIf;

%ignore Spr::UTTypeDescDbIf::PrintPool;

%feature("struct") Spr::IfInfo;

#ifdef  DESCRIPTION
// HOW TO DEFINE CLASS OPERATOR.
//      Operators are defined using '%feature' facility of swig.
//        %feature("operator", def="<op-spec-list>");
//      Definition may accross several lines (spaces, tabs and new-lines are ignored).
//
//      <op-spec-list>          a ':' separated list of { <op-spec> | <op-macro> }
//      <op-spec>               <op-sym>,<type-cat>[,[<func-name>][,<type>[,<type>]]]
//      <op-sym>                operator symbol
//      <type-cat>              Two or three letters indicating type category.
//                                1st letter  type category of operation result
//                                2nd letter  type category of 1st operand
//                                3rd letter  type category of 2nd operand if exist
//                              Type category letter is one of follows.
//                              　C  indicates this class (defining the operator)
//                              　E  indicates the element type of this class
//                              　v  indicates Vec** class
//                              　m  indicates Matrix** class
//                              　q  indicates Quaternion* class
//                              　p  indicates Pose* class
//                              　S  indicates scalar type
//                                     type itself is specifeid in <type> field
//                              　O  indicates other class
//                                     specify name of that class in <type> field
//      <func-name>             name part assigning to interface function (optional).
//                                should be unique in combination with <type-cat>
//      <type>                  Class name or scalar type name
//                                if S or O appeares in <type-cat> field N times,
//                                <type> should be specified N times in thar order.
//      <op-macro>              operator macro
//
// HOW TO DEFINE OPERATOR MACRO.
//      Operator macros are also defined using '%feature' facility of swig.
//        %feature("operator", macro="<macro-list>");
//      Definition may accross several lines (spaces, tabs and new-lines are ignored).
//      Other macro names may be referred in dfining macro.
//
//      <op-macro>              a list of <macro-name>(<op-spec-list>)
//      <macro-name>            name assigned to this macro
//      <op-spec-list>          see above
#endif

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
               SUBS_V   ([],ES,,int)
               SUBS_M   ([],vS,,int)

               VECTOR2_OPERATOR(UNARY: BASIC_V: S_PROD: V_PROD2: COMP: SUBS_V)
               VECTOR3_OPERATOR(UNARY: BASIC_V: S_PROD: V_PROD3: COMP: SUBS_V)
               VECTOR4_OPERATOR(UNARY: BASIC_V: COMP: SUBS_V)
               VECTOR6_OPERATOR(UNARY: BASIC_V: COMP: SUBS_V)
               QUATERNION_OPERATOR(BASIC_V: Q_PROD)
               MATRIX_OPERATOR(UNARY: BASIC_M: M_PROD: SUBS_M)
");

%feature("operator", def="VECTOR2_OPERATOR") Spr::Vec2f;
%feature("operator", def="VECTOR2_OPERATOR") Spr::Vec2d;
%feature("operator", def="VECTOR3_OPERATOR") Spr::Vec3f;
%feature("operator", def="VECTOR3_OPERATOR") Spr::Vec3d;
%feature("operator", def="VECTOR4_OPERATOR") Spr::Vec4f;
%feature("operator", def="VECTOR4_OPERATOR") Spr::Vec4d;
//%feature("operator", def="Q_PROD") Spr::Quaternionf;
//%feature("operator", def="Q_PROD") Spr::Quaterniond;
%feature("operator", def="QUATERNION_OPERATOR") Spr::Quaternionf;
%feature("operator", def="QUATERNION_OPERATOR") Spr::Quaterniond;
%feature("operator", def="P_PROD") Spr::Posef;
%feature("operator", def="P_PROD") Spr::Posed;
%feature("operator", def="VECTOR2_OPERATOR") Spr::Vec2i;
%feature("operator", def="VECTOR3_OPERATOR") Spr::Vec3i;
%feature("operator", def="VECTOR4_OPERATOR") Spr::Vec4i;
%feature("operator", def="VECTOR6_OPERATOR") Spr::Vec6f;
%feature("operator", def="VECTOR6_OPERATOR") Spr::Vec6d;
%feature("operator", def="MATRIX_OPERATOR") Spr::Matrix3f;
%feature("operator", def="MATRIX_OPERATOR") Spr::Matrix3d;
%feature("virtual_function") Spr::ObjectIf::SetDesc;
%feature("virtual_function") Spr::ObjectIf::GetDesc;
%feature("virtual_function") Spr::ObjectIf::DelChildObject;
%feature("virtual_function") Spr::NamedObjectIf::SetName;
%feature("only_cs_ignore") Spr::ObjectIf::GetIfInfo;

//%feature("withfunc") Spr::Posed;
//%feature("withfunc") Spr::Vec3d;
//%feature("withfunc") Spr::Vec3f;

%module Foundation
%include "../../include/SprBase.h"
%include "../../include/Base/Env.h"
%include "../../include/Foundation/SprObject.h"
%include "../../include/Foundation/SprScene.h"
%include "../../include/Foundation/SprUTTimer.h"
%include "../Foundation/UTBaseType.h"

//%include "../../include/Base/TVector.h"
//%include "../../include/Base/TMatrix.h"
//%include "../../include/Base/TQuaternion.h"
