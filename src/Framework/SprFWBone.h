/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWBONEIF_H
#define SPR_FWBONEIF_H

#include <Foundation/SprScene.h>
#include <Physics/SprPHSolid.h>
#include <Graphics/SprGRFrame.h>
#include <Base/Affine.h>
namespace Spr{

	struct FWSceneIf;

	struct FWBoneDesc{
	};

	/** @brief Frameworkのオブジェクト
		自動生成したボーンの剛体（PhysicsモジュールのPHSolid）とシーングラフのフレーム（GraphicsモジュールのGRFrame）を
		関連付けるオブジェクト．
		これによってシミュレーションによる剛体の位置の変化がシーングラフに自動的に反映される．
		Frameworkを利用する場合はPHSceneIf::CreateSolidやGRSceneIf::CreateFrameの代わりに
		FWSceneIf::CreateObjectを使用する．
	 */
	struct FWBoneIf : SceneObjectIf {
		SPR_IFDEF(FWBone);

		/// PHSolidを取得する
		PHSolidIf* GetPHSolid();
		/// PHSolidを設定する
		void SetPHSolid(PHSolidIf* s);
		/// GRFrameを取得する
		GRFrameIf* GetGRFrame();
		/// GRFrameを設定する
		void SetGRFrame(GRFrameIf* f);
		/// CDRoundConeIfを取得する
		CDRoundConeIf* GetShape();
		/// CDRoundConeIfのRadiusを設定する
		void		   SetShapeRadius(double r1,double r2);
		///　jointを取得する
		PHJointIf*	   GetJoint();
		///  jointのK,D,D2を設定する
		void		   SetJointKDD2(double K,double D, double D2);
	};

}

#endif
