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
%feature("operator") Spr::Vec3d;

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
