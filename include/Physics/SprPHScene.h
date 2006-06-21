/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/** \addtogroup gpPhysics 	*/
//@{

/**
 *	@file SprPHScene.h
 *	@brief シーン
*/
#ifndef SPR_PHSCENEIF_H
#define SPR_PHSCENEIF_H

namespace Spr{;

struct PHSdkIf;

struct PHSolidIf;
struct PHSolidDesc;

struct CDShapeIf;
struct CDShapeDesc;

struct PHJointIf;
struct PHConstraintDesc;
typedef PHConstraintDesc PHJointDesc;

struct PHPathIf;
struct PHPathDesc;

/// 物理エンジンのシーンの状態
struct PHSceneState{
	/// 積分ステップ
	double timeStep;
	/// 積分した回数
	unsigned count;
	PHSceneState(){Init();}
	void Init(){
		timeStep = 0.005;
		count = 0;
	}
};
/// \ingroup gpPhysicsNode
/// 物理エンジンのシーンのデスクリプタ
struct PHSceneDesc: public PHSceneState{
	///	接触のモード
	enum ContactMode{
		MODE_NONE,		///< 無視する
		MODE_PENALTY,	///< ペナルティ法で解く
		MODE_LCP		///< LCPで解く
	};
	
	/// 重力加速度ベクトル．デフォルト値は(0.0f, -9.8f, 0.0f)．
	Vec3f gravity;

	PHSceneDesc(){Init();}
	void Init(){
		gravity=Vec3f(0,-9.8f,0);
	}
};

///	シーン
struct PHSceneIf : public SceneIf{
public:
	IF_DEF(PHScene);

	/** @brief このSceneをもつSDKを返す
		@return SDKのインタフェース
	 */
	virtual PHSdkIf* GetSdk()=0;

	/** @brief Shapeを作成する
		@param desc 作成するShapeのディスクリプタ
		@return Shapeのインタフェース
	 */
	virtual CDShapeIf* CreateShape(const CDShapeDesc& desc)=0;
	
	/** @brief Solidを作成する
		@return Solidのインタフェース

		PHSolidDescのデフォルト値を用いてSolidを作成する．
	 */
	virtual PHSolidIf* CreateSolid()=0;
	
	/** @brief Solidを作成する
		@param desc 作成するSolidのディスクリプタ
		@return Solidのインタフェース
	*/
	virtual PHSolidIf* CreateSolid(const PHSolidDesc& desc)=0;
	
	/** @brief Solidの数を取得する
		@return Solidの数
	 */
	virtual int NSolids()=0;

	/** @brief Solidを取得する
		@return Solidのインタフェースの配列へのポインタ
	 */
	virtual PHSolidIf** GetSolids()=0;

	/** @brief 指定した剛体同士の接触のモードを設定する
		@param lhs 組の片方の剛体へのポインタ
		@param rhs 組のもう片方の剛体へのポインタ
		@param mode 接触のモード

		剛体lhsと剛体rhsとの接触のモードをmodeに設定する．
	  */
	virtual void SetContactMode(PHSolidIf* lhs,	PHSolidIf* rhs, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief 指定した集合に含まれる全ての剛体同士の接触のモードを設定する
		@param group 剛体へのポインタ配列の先頭アドレス
		@param size ポインタ配列の長さ
		@param mode 接触のモード

		group[0]からgroup[size-1]までの全ての剛体の組の接触のモードをmodeに設定する．
	 */
	virtual void SetContactMode(PHSolidIf** group, size_t length, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief 指定した剛体と他の剛体との全ての接触のモードを設定する
		@param mode 接触のモード
	 */
	virtual void SetContactMode(PHSolidIf* solid, PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief シーンが保有する全ての剛体同士の接触のモードを設定する
		@param mode 接触のモード
	 */
	virtual void SetContactMode(PHSceneDesc::ContactMode mode = PHSceneDesc::MODE_LCP)=0;

	/** @brief 関節を作成する
		@param desc 関節のディスクリプタ
		@return 関節のインタフェース
		Solid lhsとSolid rhsを連結する関節を作成する．
	 */
	virtual PHJointIf* CreateJoint(PHSolidIf* lhs, PHSolidIf* rhs, const PHJointDesc& desc)=0;

	/** @brief パスを作成する
		@param desc パスのディスクリプタ
	 */
	virtual PHPathIf* CreatePath(const PHPathDesc& desc)=0;

	/** @brief 積分ステップを取得する
		@return 積分ステップ
	 */
	virtual double GetTimeStep()const=0;
	
	/** @brief 積分ステップを設定する
		@param dt 積分ステップ
	 */
	virtual void SetTimeStep(double dt)=0;
	
	/** @brief カウント数を取得する
		@return カウント数
		シーンが作成されてから現在までにStep()を実行した回数を返す．
	 */
	virtual unsigned GetCount()const=0;

	/** @brief カウント数を設定する
		@param count カウント数
	 */
	virtual void SetCount(unsigned count)=0;
	
	/** @brief シーンの時刻を進める
	 */
	virtual void Step()=0;

	/** @brief シーンを空にする
	 */
	virtual void Clear()=0;

	/** @brief 重力を設定する
		@param accel 重力加速度ベクトル
	 */
	//ここに持たせるべきか要検討だが，Novodexはここ 
	virtual void SetGravity(Vec3f accel)=0;
	
	/** @brief 重力を取得する
		@return 重力加速度ベクトル
	 */
	virtual Vec3f GetGravity()=0;

};

//@}

}	//	namespace Spr
#endif
