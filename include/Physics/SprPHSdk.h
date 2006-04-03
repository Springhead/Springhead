#ifndef SPR_PHSDKIF_H
#define SPR_PHSDKIF_H
#include <Collision/SprCDShape.h>

namespace Spr{;

/** \addtogroup gpPhysics 物理エンジンの基礎	*/
//@{

struct PHSceneIf;
struct PHSceneDesc;

///	物理シミュレーションSDK
struct PHSdkIf : public ObjectIf{
	IF_DEF(PHSdk);
	///	Sceneの作成
	virtual PHSceneIf* CreateScene()=0;
	///	Sceneの作成
	virtual PHSceneIf* CreateScene(const PHSceneDesc& desc)=0;
	///	Sceneの数を取得
	virtual int NScene()=0;
	///	Sceneの取得
	virtual PHSceneIf** GetScenes()=0;
	
	///	Shape作成
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	///	Shapeの数
	virtual int NShape()=0;
	///	Shapeの取得
	virtual CDShapeIf** GetShapes()=0;
};
PHSdkIf* SPR_CDECL CreatePHSdk();

//@}

}	//	namespace Spr
#endif
