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
		IF_DEF(FWScene);
		
		/// Physicsモジュールのシーンを取得する
		virtual PHSceneIf*	GetPHScene() = 0;
		/** @brief Physicsモジュールのシーンを設定する
			FWSceneの作成と同時に自動的に作成されるので、通常は呼び出す必要は無い．
		*/	
		virtual void		SetPHScene(PHSceneIf* s) = 0;

		/// Graphicsモジュールのシーンを取得する
		virtual GRSceneIf*	GetGRScene() = 0;
		/** @brief Graphicsモジュールのシーンを取得・設定する
			FWSceneの作成と同時に自動的に作成されるので、通常は呼び出す必要は無い．
		*/
		virtual void		SetGRScene(GRSceneIf* s) = 0;

		/** @brief Frameworkオブジェクトを作成する
			PHSolidとGRFrameを関連付けるオブジェクトを作成する．
			内部的には、まずdescにもとづいて新たなPHSolidおよびGRFrameが、それぞれ
			PHSceneIf::CreateSolidおよびGRSceneIf::CreateFrameにより作成される。
			次に両者を参照するFWObjectが作成されFWSceneに登録される。
		 */
		virtual FWObjectIf*	CreateObject(const PHSolidDesc& soliddesc, const GRFrameDesc& framedesc)=0;
		virtual int NObject()const=0;
		virtual FWObjectIf** GetObjects()=0;

		/** @brief オブジェクトを同期する
			各FWObjectに対して、PHSolidの位置をGRFrameに反映させる。
			通常はFWSceneIf::Stepによってシミュレーションが実行された後に呼ぶ。
		 */
		virtual void Sync() = 0;

		/** @brief シミュレーションを実行する
		 */
		virtual void Step() = 0;

		/** @brief 描画する
			@param grRender	レンダラ
			@param debug	デバッグ描画か
			debugがtrueの場合（デバッグ描画）、各剛体の衝突判定用形状データにより
			描画が行われる。一方、debugがfalseの場合はシーングラフの形状データや
			マテリアル、テクスチャを利用して描画が行われる。
		 */
		virtual void Draw(GRRenderIf* grRender, bool debug=false) = 0;

		virtual void AddHumanInterface(HIForceDevice6D* d)=0;
	};

	FWSceneIf* SPR_CDECL CreateFWScene();
	FWSceneIf* SPR_CDECL CreateFWScene(const void* desc);
}

#endif
