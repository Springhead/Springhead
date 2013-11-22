/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef SPR_FWOBJECTIF_H
#define SPR_FWOBJECTIF_H

#include <Foundation/SprObject.h>
#include <Collision/SprCDShape.h>

namespace Spr{;

struct CDConvexMeshIf;
struct PHSolidIf;
struct PHJointIf;
struct GRMeshIf;
struct GRFrameIf;
struct FWSceneIf;
struct PHIKEndEffectorIf;
struct PHIKActuatorIf;

/** \addtogroup gpFramework */
//@{

struct FWObjectDesc{
	/// ボーンの同期を絶対位置で行う（親フレームからの差分でなくワールド座標系に対する変換をセットする）
	bool bAbsolute;

	enum FWObjectSyncSource {
		PHYSICS=0,  // Physicsの状態にGraphicsが同期される
		GRAPHICS    // Graphicsの状態にPhysicsが同期される
	} syncSource;

	FWObjectDesc() {
		bAbsolute  = false;
		syncSource = PHYSICS;
	}
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

	/// PHJointを取得する
	PHJointIf* GetPHJoint();
	/// PHJointを設定する
	void SetPHJoint(PHJointIf* j);

	/// セットされた子Frameを取得する
	GRFrameIf* GetChildFrame();
	/// 子Frameをセットする（自Frameと子Frame間の距離がわかるのでSolidの大きさを自動設定可能になる）
	void SetChildFrame(GRFrameIf* f);

	/// PHSolid,GRFrameのいずれの位置に合わせるかを設定する
	void SetSyncSource(FWObjectDesc::FWObjectSyncSource syncSrc);
	/// PHSolid,GRFrameのいずれの位置に合わせるかを取得する
	FWObjectDesc::FWObjectSyncSource GetSyncSource();

	/// ボーンを絶対位置指定で同期するかを設定する
	void EnableAbsolute(bool bAbs);
	/// ボーンを絶対位置指定で同期するかを取得する
	bool IsAbsolute();

	/** @brief グラフィクス用メッシュをロードするヘルパ関数
		@param filename ファイル名
		@param ii		ファイルタイプを指定する場合のIfInfo
		@param frame	メッシュを保有するフレーム．NULLを指定するとルートフレームが保有する
		@return ロードに成功したらtrueを返す．失敗したらfalseを返す．

		メッシュをロードし，このFWObjectのGRFrameの下に加える
	 */
	bool LoadMesh(const char* filename, const IfInfo* ii = NULL, GRFrameIf* frame = NULL);

	/** @brief グラフィクス用メッシュから衝突判定用メッシュを自動生成する
		@param frame	変換対象のメッシュを保有するGRFrame．NULLを指定するとルートフレームが対象となる
		@param mat		物性値
		オブジェクトが保持するグラフィクスメッシュの凸包として衝突判定メッシュを生成する
	 */
	void GenerateCDMesh(GRFrameIf* frame = NULL, const PHMaterial& mat = PHMaterial());

	/// PHSolidとGRFrameの同期
	void Sync();
};
//@}
}

#endif
