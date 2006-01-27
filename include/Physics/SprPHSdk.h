#ifndef SPR_PHSDKIF_H
#define SPR_PHSDKIF_H
#include <Collision/SprCDShape.h>

namespace Spr{;

struct PHSceneIf;
///	物理シミュレーションSDK
struct PHSdkIf : public ObjectIf{
	IF_DEF(PHSdk);
	///	Sceneの作成
	virtual PHSceneIf* CreateScene()=0;
	///	Sceneの数を取得
	virtual int GetNScene()=0;
	///	Sceneの取得
	virtual PHSceneIf** GetScenes()=0;
	
	///	Shape作成
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	///	Shapeの数
	virtual int GetNShape()=0;
	///	Shapeの取得
	virtual CDShapeIf** GetShapes()=0;
};
PHSdkIf* _cdecl CreatePHSdk();

}	//	namespace Spr
#endif
