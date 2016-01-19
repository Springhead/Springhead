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

%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        *,ECC: %,ECC: ^,ECC: ==,SCC,,bool: !=,SCC,,bool") Spr::Vec2f;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        *,ECC: %,ECC: ^,ECC: ==,SCC,,bool: !=,SCC,,bool") Spr::Vec2d;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        *,ECC: %,CCC: ^,CCC: ==,SCC,,bool: !=,SCC,,bool") Spr::Vec3f;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        *,ECC: %,CCC: ^,CCC: ==,SCC,,bool: !=,SCC,,bool") Spr::Vec3d;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        ==,SCC,,bool: !=,SCC,,bool") Spr::Vec4f;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        ==,SCC,,bool: !=,SCC,,bool") Spr::Vec4d;

%feature("operator", def="*,vCv:*,CCC:*,mCm") Spr::Quaternionf;
%feature("operator", def="*,vCv:*,CCC:*,mCm") Spr::Quaterniond;
%feature("operator", def="*,vCv:*,CCC") Spr::Posef;
%feature("operator", def="*,vCv:*,CCC") Spr::Posed;

%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        *,ECC: %,ECC: ^,ECC: ==,SCC,,bool: !=,SCC,,bool") Spr::Vec2i;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        *,ECC: %,CCC: ^,CCC: ==,SCC,,bool: !=,SCC,,bool") Spr::Vec3i;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        ==,SCC,,bool: !=,SCC,,bool") Spr::Vec4i;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        ==,SCC,,bool: !=,SCC,,bool") Spr::Vec6f;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: /,CCE:
        ==,SCC,,bool: !=,SCC,,bool") Spr::Vec6d;

%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: *,vCv: *,vvC") Spr::Matrix3f;
%feature("operator", def="-,CC: +,CCC: -,CCC: *,CEC: *,CCE: *,vCv: *,vvC") Spr::Matrix3d;

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
