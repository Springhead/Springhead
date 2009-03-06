/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWOBJECTIF_H
#define SPR_FWOBJECTIF_H

#include <Foundation/SprScene.h>
#include <Physics/SprPHSolid.h>
#include <Graphics/SprGRFrame.h>
#include <Base/Affine.h>
namespace Spr{

	struct FWSceneIf;

	struct FWObjectDesc{
	};

	/** @brief Frameworkのオブジェクト
		剛体（PhysicsモジュールのPHSolid）とシーングラフのフレーム（GraphicsモジュールのGRFrame）を
		関連付けるオブジェクト．
		これによってシミュレーションによる剛体の位置の変化がシーングラフに自動的に反映される．
		Frameworkを利用する場合はPHSceneIf::CreateSolidやGRSceneIf::CreateFrameの代わりに
		FWSceneIf::CreateObjectを使用する．
	 */
	struct FWObjectIf : SceneObjectIf {
		SPR_IFDEF(FWObject);
		/// PHSolidを取得する
		PHSolidIf* GetPHSolid();
		/// PHSolidを設定する
		void SetPHSolid(PHSolidIf* s);
		/// GRFrameを取得する
		GRFrameIf* GetGRFrame();
		/// GRFrameを設定する
		void SetGRFrame(GRFrameIf* f);
		/// PHParentSolidを取得する
		PHSolidIf* GetPHParentSolid();
		/// PHParentSolidを設定する
		void SetPHParentSolid(PHSolidIf* ps);
		/// PHAncestorSolidを取得する
		PHSolidIf* GetPHAncestorSolid();
		/// PHAncestorSolidを設定する
		void SetPHAncestorSolid(PHSolidIf* ps);
		/// PHAncestorSolid2を取得する
		PHSolidIf* GetPHAncestorSolid2();
		/// PHAncestorSolid2を設定する
		void SetPHAncestorSolid2(PHSolidIf* ps);
		/// GRParentAffineを取得する
		Affinef GetGRParentAffine();
		/// GRParentAffineを設定する
		void SetGRParentAffine(Affinef af);


	};

}

#endif
