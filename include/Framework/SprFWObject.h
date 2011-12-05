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
	/// PHFEMMeshを取得する

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
	void Sync(bool ph_to_gr);

	///ボーン付きXファイル専用
	/// solidLengthを取得する
	double GetSolidLength();
	/// solidLengthを設定する
	void SetSolidLength(double l);
};

struct FWBoneObjectDesc {
	bool modifyShapeLength;		///< 剛体のShapeの長さをBoneにあわせて変更する
	bool modifyShapeWidth;		///< 剛体のShapeの太さをスキンメッシュに合わせて変更する（未実装）
	bool modifyShapePose;		///< 剛体に対するShapeの位置を(0,0,0)に変更する（falseにするともともとのshapePoseを維持する）
	bool modifyPlugSocketPose;	///< 関節の取り付け位置をBoneにあわせて変更する

	enum FWBoneObjectAdaptType{
		GRFRAME_TO_PHSOLID = 0, //PHSolidの位置にGRFrameを合わせる
		PHSOLID_TO_GRFRAME = 1	//GRFrameの位置にPHSolidを合わせる
	}AdaptType; 
};

/** @brief ボーンモデルと剛体関節系をつなげるためのFrameworkオブジェクト
 */
struct FWBoneObjectIf : FWObjectIf {
	SPR_IFDEF(FWBoneObject);

	/// PHJointを取得する
	PHJointIf* GetPHJoint();
	/// PHJointを設定する
	void SetPHJoint(PHJointIf* j);

	/// ボーンの終端点を示すGRFrameを取得する
	GRFrameIf* GetEndFrame();
	/// ボーンの終端点を示すGRFrameを設定する
	void SetEndFrame(GRFrameIf* f);

	/// PHSolid,GRFrameのいずれの位置に合わせるかを設定する
	void SetAdaptType(int t);
};

struct FWStructureDesc {
};
/** @brief ボーンを集合体として管理するためのFrameworkオブジェクト
 */
struct FWStructureIf : SceneObjectIf {
	SPR_IFDEF(FWStructure);

	/// BoneObjectをナンバーで指定して取得する
	FWBoneObjectIf* GetBone(int n);
	/// BoneObjectを名前で指定して取得する
	FWBoneObjectIf* GetBone(const char* n);
	/// BoneObjectのサイズを取得する
	int GetBoneSize();
	/// FWBoneObjectの位置を設定する
	void SetPose(Posed p);	
	/// FWBoneObjectを追加する
	void AddBone(FWBoneObjectIf* o);
};

}

#endif
