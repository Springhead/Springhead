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

#include <Framework/SprFWObject.h>
#include <Framework/SprFWSkeletonSensor.h>

namespace Spr{;

struct CDShapeIf;
struct PHSolidIf;
struct PHConstraintIf;
struct PHContactPointIf;
struct PHIKEngineIf;
struct PHHapticEngineIf;
struct PHFemEngineIf;
struct PHSceneIf;
struct GRSceneIf;
struct GRRenderIf;

/** \addtogroup gpFramework */
//@{

struct FWHapticPointerIf;

struct FWSceneDesc{
};

class HIForceDevice6D;
struct FWSdkIf;

struct FWSceneIf : SceneIf {
	SPR_IFDEF(FWScene);

	enum AxisStyle{
		AXIS_LINES,
		AXIS_ARROWS,
	};

public:
	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	 */
	FWSdkIf* GetSdk();
		
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

	/** @brief 空のFrameworkオブジェクトを作成する
		作成された時点でPHSolidとGRFrameは割り当てられていないので，
		SetPHSolid, SetGRFrameで割り当てる必要がある．
	 */
	FWObjectIf*	CreateFWObject();

	/// オブジェクトの数を取得する
	int NObject()const;

	/// オブジェクトを取得する
	FWObjectIf* GetObject(int i);

	/// オブジェクトの配列を取得する
	FWObjectIf** GetObjects();

	/** @brief オブジェクトを同期する
		各FWObjectに対して、それぞれ
		・PHSolidの位置をGRFrameに反映させる
		・GRFrameの位置をPHSolidに反映させる
		のどちらかを行う(どちらを行うかはFWObject::syncSourceで設定する)。
		通常はFWSceneIf::Stepによってシミュレーションが実行された後に呼ぶ。
	 */
	void Sync();

	/** @brief シミュレーションを実行する
	 */
	void Step();

	/** @brief 描画する
		@param grRender	レンダラ
		@param debug	デバッグ描画か

	 */
	void Draw(GRRenderIf* grRender, bool debug);
	void Draw(GRRenderIf* grRender);

	/** @brief	Physicsシーンをレンダリングする
		@param	render	レンダラ
		@param  scene	シーン
		シーンに含まれるオブジェクト（剛体，拘束，接触，IKなど）をすべて描画する．
	 */
	void DrawPHScene(GRRenderIf* render);

	/** @brief 剛体をレンダリングする
	    @param	render			レンダラ
		@param	solid			剛体
		@param	solid_or_wire
		指定した剛体の情報（座標軸，衝突判定形状，力，モーメント）を描画する
	 */
	void DrawSolid(GRRenderIf* render, PHSolidIf* solid, bool solid_or_wire);

	/** @brief 衝突判定用形状を描画する
		@param	render	レンダラ
		@param	shape	形状
		@param	solid	trueならソリッド描画，falseならワイヤフレーム描画
		shapeの種類を判別して形状を描画する．
	 */
	void DrawShape(GRRenderIf* render, CDShapeIf* shape, bool solid);

	/** @brief 拘束を描画する
		@param	render	レンダラ
		@param	con		拘束
		指定した拘束の情報（ソケットとプラグの座標軸，拘束力，モーメント）を描画する
	 */
	void DrawConstraint(GRRenderIf* render, PHConstraintIf* con);

	/**	@brief 接触を描画する
		@param	render	レンダラ
		@param	con		接触
		指定した接触の接触断面を描画する
	 */
	void DrawContact(GRRenderIf* render, PHContactPointIf* con);

	/**	@brief	IK情報を描画する
		@param	render		レンダラ
		@param	ikEngine	IKエンジン
	 */
	void DrawIK(GRRenderIf* render, PHIKEngineIf* ikEngine);

	/** @brief 関節可動域を描画する
		@param	render	レンダラ
		@param	con		関節
	 */
	void DrawLimit(GRRenderIf* render, PHConstraintIf* con);

	/**	@brief	Haptic情報を描画する
		@param	render			レンダラ
		@param	hapticEngine	Hapticエンジン
	 */
	void DrawHaptic(GRRenderIf* render, PHHapticEngineIf* hapticEngine);

	/**	@brief 描画モードの設定
		@param ph_or_gr	PHSceneを描画するかGRSceneを描画するか
		@param solid	ソリッド描画のオン/オフ
		@param wire		ワイヤフレーム描画のオン/オフ
		solidとwireはPHSceenを描画するときのみ有効．
	 */
	void SetRenderMode(bool solid = true, bool wire = false);

	/** @brief	オブジェクト単位の描画のオン/オフ
	 */
	void EnableRender(ObjectIf* obj, bool enable = true);
	
	/** @brief 描画マテリアルの設定
		@param	mat			マテリアル
		@param	solid		設定対象の剛体
		デバッグ描画におけるマテリアルを設定する．
		matにはGRRenderIf::TMaterialSampleの値が指定できる．
		solidにNULLを指定すると全ての剛体が対象となる．
	 */
	void SetSolidMaterial(int mat, PHSolidIf* solid = 0);
	void SetWireMaterial (int mat, PHSolidIf* solid = 0);

	/// 座標軸の描画切り替え
	void EnableRenderAxis(bool world = true, bool solid = true, bool con = true);
	/// 座標軸の描画オプション
	void SetAxisMaterial(int matX, int matY, int matZ);
	void SetAxisScale(float scaleWorld, float scaleSolid, float scaleCon);
	void SetAxisStyle(int style);

	/// 力の描画切り替え
	void EnableRenderForce(bool solid = true, bool constraint = true);
	/// 力の描画オプション
	void SetForceMaterial(int matForce, int matMoment);
	void SetForceScale(float scaleForce, float scaleMoment);

	/**  @brief PHSceneの描画 */
	void EnableRenderPHScene(bool enable = true);
	/**  @brief GRSceneの描画 */
	void EnableRenderGRScene(bool enable = true);
	/**  @brief 接触面の描画 */
	void EnableRenderContact(bool enable = true);
	void SetContactMaterial(int mat);

	/** @brief bboxの描画 */
	void EnableRenderBBox(bool enable = true);
	void SetBBoxMaterial(int mat);

	/**  @brief グリッドの描画 */
	void EnableRenderGrid(bool x = true, bool y = true, bool z = true);
	/** @brief	グリッドの設定
		@param	axis	設定する軸を'x', 'y', 'z'で指定
		@param	offset	オフセット
		@param	size	グリッドの幅
		@param	slice	分割数
	 */
	void SetGridOption(char axis, float offset, float size, int slice);
	void SetGridMaterial(int matX, int matY, int matZ);

	/**  @brief IKの計算結果の描画 */
	void EnableRenderIK(bool enable = true);
	void SetIKMaterial(int mat);
	void SetIKScale(float scale);

	/**  @brief 関節可動域の描画 */
	void EnableRenderLimit(bool enable = true);

	/** @brief Hapticの描画 */
	void EnableRenderHaptic(bool enable = true);

	/** @brief FEMの描画 */
	void EnableRenderFem(bool enable = true);
	
	/** @brief SkeletonSensorの描画 */
	void EnableRenderSkeletonSensor(bool enable = true);

	/** @brief Opの描画 */
	void EnableRenderOp(bool enable = true);

	void AddHumanInterface(HIForceDevice6D* d);

	/** @brief 力覚ポインタを作る
	*/
	FWHapticPointerIf* CreateHapticPointer();

	/** @breif HumanInterfaceの状態を取得し、力覚ポインタを更新する
	*/
	void UpdateHapticPointers();

	/** @brief スケルトンセンサ（Kinect, Leapmotionなど）を作る
	*/
	FWSkeletonSensorIf* CreateSkeletonSensor(const FWSkeletonSensorDesc& desc = FWSkeletonSensorDesc());

	/** @breif スケルトンセンサからの情報を反映する
	*/
	void UpdateSkeletonSensors();

};

FWSceneIf* SPR_CDECL CreateFWScene();
FWSceneIf* SPR_CDECL CreateFWScene(const void* desc);
//@}
}

#endif
