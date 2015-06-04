#	Do not edit. RunSwig.bat will update this file.
%ignore Spr::UTTimerIf::GetCallback;
%ignore Spr::UTTimerIf::SetCallback;
%ignore Spr::ObjectIf::CreateObject;
%ignore Spr::ObjectIf::Print;
%ignore Spr::ObjectIf::PrintShort;
%ignore Spr::ObjectIf::WriteStateR;
%ignore Spr::ObjectIf::ReadStateR;
%ignore Spr::ObjectIf::DumpObjectR;
%ignore Spr::SdkIf::CreateSdk;
%module Foundation
%include "../../include/SprBase.h"
%include "../../include/Base/Env.h"
%include "../../include/Foundation/SprObject.h"
%include "../../include/Foundation/SprScene.h"
%include "../../include/Foundation/SprUTTimer.h"
%include "../Foundation/UTBaseType.h"
/*
#define DOUBLECOLON :: 
%include "../../include/Springhead.h"
%include "../../include/base/Env.h"
%include "../../include/Foundation/SprObject.h"
%include "../../include/Foundation/SprScene.h"
%include "../../include/Foundation/SprUTTimer.h"
%include "../../include/Base/BaseDebug.h"
%include "../../src/Foundation/UTTypeDesc.h"
%include "../Foundation/Foundation.h"
%include "../Foundation/Object.h"
%include "../Foundation/Scene.h"
%include "../Foundation/UTBaseType.h"
%include "../Foundation/UTClapack.h"
%include "../Foundation/UTDllLoader.h"
%include "../Foundation/UTDllLoaderImpl.h"
%include "../Foundation/UTLoadContext.h"
%include "../Foundation/UTLoadHandler.h"
%include "../Foundation/UTMMTimer.h"
%include "../Foundation/UTPath.h"
%include "../Foundation/UTPreciseTimer.h"
%include "../Foundation/UTQPTimer.h"
%include "../Foundation/UTSocket.h"
%include "../Foundation/UTTimer.h"
%include "../Foundation/UTTypeDesc.h"
*/
