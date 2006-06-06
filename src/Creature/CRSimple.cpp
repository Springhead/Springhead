#include <Springhead.h>
#include "CRSimple.h"
#include <Foundation/Object.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

using namespace PTM;
namespace Spr{

///////////////////////////////////////////////////////////////////
//	CRSimple
IF_OBJECT_IMP(CRSimple, SceneObject);
//IF_IMP(CRSimple, SceneObject);

void CRSimple::Hoge(){
}

void CRSimple::Step(){
	std::cout << "Hoge!Hoge!Hoge!" << std::endl;
}
}