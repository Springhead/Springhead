/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWSCENEIF_H
#define SPR_FWSCENEIF_H

#include <Foundation/SprScene.h>
#include <Physics/SprPHScene.h>
#include <Graphics/SprGRScene.h>
#include <Graphics/SprGRRender.h>

namespace Spr{

	struct FWObjectDesc;
	struct FWObjectIf;

	struct FWSceneDesc{
	};

	class HIForceDevice6D;

	struct FWSceneIf : SceneIf {
		SPR_IFDEF(FWScene);
		
		/// Physicsモジュールのシーンを取得する
		PHSceneIf*	GetPHScene();
		/** @brief Physicsモジュールのシーンを設定する
			FWSceneの作成と同時に自動的に作成されるので、通常は呼び出す必要は無い．
		*/	
		void		SetPHScene(PHSceneIf* s);

		/// Graphicsモジュールのシーンを取得する
		GRSceneIf*	GetGRScene();
		/** @brief Graphicsモジュールのシーンを取得・設定する
			FWSceneの作成と同時に自動的に作成されるので、通常は呼び出す必要は無い．
		*/
		void		SetGRScene(GRSceneIf* s);

		/** @brief Frameworkオブジェクトを作成する
			PHSolidとGRFrameを関連付けるオブジェクトを作成する．
			内部的には、まずdescにもとづいて新たなPHSolidおよびGRFrameが、それぞれ
			PHSceneIf::CreateSolidおよびGRSceneIf::CreateFrameにより作成される。
			次に両者を参照するFWObjectが作成されFWSceneに登録される。
		 */
		FWObjectIf*	CreateObject(const PHSolidDesc& soliddesc = PHSolidDesc(), const GRFrameDesc& framedesc = GRFrameDesc());
		int NObject()const;
		FWObjectIf** GetObjects();

		/** @brief オブジェクトを同期する
			各FWObjectに対して、PHSolidの位置をGRFrameに反映させる。
			通常はFWSceneIf::Stepによってシミュレーションが実行された後に呼ぶ。
		 */
		void Sync();

		/** @brief シミュレーションを実行する
		 */
		void Step();

		/** @brief 描画する
			@param grRender	レンダラ
			@param debug	デバッグ描画か
			debugがtrueの場合（デバッグ描画）、各剛体の衝突判定用形状データにより
			描画が行われる。一方、debugがfalseの場合はシーングラフの形状データや
			マテリアル、テクスチャを利用して描画が行われる。
		 */
		void Draw(GRRenderIf* grRender, bool debug=false);

		void AddHumanInterface(HIForceDevice6D* d);
	};

	FWSceneIf* SPR_CDECL CreateFWScene();
	FWSceneIf* SPR_CDECL CreateFWScene(const void* desc);
}

#endif
