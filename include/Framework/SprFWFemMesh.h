/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWFEMMESHIF_H
#define SPR_FWFEMMESHIF_H

#include <Framework/SprFWObject.h>


namespace Spr{;

struct FWFemMeshDesc: public FWObjectDesc{
};

/** @brief Frameworkのオブジェクト
	剛体（PhysicsモジュールのPHSolid）とシーングラフのフレーム（GraphicsモジュールのGRFrame）を
	関連付けるオブジェクト．
	これによってシミュレーションによる剛体の位置の変化がシーングラフに自動的に反映される．
	Frameworkを利用する場合はPHSceneIf::CreateSolidやGRSceneIf::CreateFrameの代わりに
	FWSceneIf::CreateObjectを使用する．
 */
struct FWFemMeshIf : FWObjectIf {
	SPR_IFDEF(FWFemMesh);
};


}

#endif
