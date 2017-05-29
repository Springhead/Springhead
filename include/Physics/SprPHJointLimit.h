/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHJointLimit.h
 *	@brief 関節可動域制限
*/
#ifndef SPR_PHJOINTLIMITIf_H
#define SPR_PHJOINTLIMITIf_H

#include <Foundation/SprObject.h>

#include <float.h>  // FLT_MAX

namespace Spr{;

/** \addtogroup gpJoint */
//@{

/// 1自由度関節可動域拘束のインタフェース
struct PH1DJointLimitIf : public SceneObjectIf{
	SPR_IFDEF(PH1DJointLimit);

	/** @brief 可動範囲を設定する
		@param range 可動範囲
	 */
	void SetRange(Vec2d range);
	
	/** @brief 可動範囲を取得する
		@return 可動範囲
	 */
	void GetRange(Vec2d& range);

	/** @brief 可動域制限のためのバネ係数を設定する
		@param spring バネ係数
	 */
	void SetSpring(double spring);

	/** @brief 可動域制限のためのバネ係数を取得する
		@return バネ係数
	 */
	double GetSpring();

	/** @brief 可動域制限のためのダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void SetDamper(double damper);

	/** @brief 可動域制限のためのダンパ係数を取得する
		@return ダンパ係数
	 */
	double GetDamper();

	/** @brief 可動域にかかっているかどうかを取得する
		@return かかっていればtrue
	*/
	bool IsOnLimit();

	/** @brief 有効かどうかを設定する
	*/
	void    Enable(bool b);

	/** @brief 有効かどうかを取得する
	*/
	bool    IsEnabled();
};

///	1自由度関節の可動域拘束
struct PH1DJointLimitDesc{
	bool    bEnabled;	///< 有効かどうか
	double	spring;		///< 可動範囲バネ
	double	damper;		///< 可動範囲ダンパ
	Vec2d	range;		///< 可動範囲. range[0]:lower < range[1]:upper のときに有効

	PH1DJointLimitDesc() {
		bEnabled = true;
		spring   = 10000.0;
		damper   =   100.0;
		range    = Vec2d(1, -1); /// デフォルトでは拘束無効
	}
};

// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 

/// ボールジョイント可動域制限のインタフェース
struct PHBallJointLimitIf : public SceneObjectIf {
	SPR_IFDEF(PHBallJointLimit);

	/** @brief 可動域制限のためのバネ係数を設定する
		@param spring バネ係数
	 */
	void SetSpring(double rSpring);

	/** @brief 可動域制限のためのバネ係数を取得する
		@return バネ係数
	 */
	double GetSpring();

	/** @brief 可動域制限のためのダンパ係数を設定する
		@param damper ダンパ係数
	 */
	void SetDamper(double rDamper);

	/** @brief 可動域制限のためのダンパ係数を取得する
		@return ダンパ係数
	 */
	double GetDamper();

	/** @brief 可動域の基準方向を設定する
		@param limDir 可動域の基準方向
	 */
	void SetLimitDir(Vec3d limDir);

	/** @brief 可動域の基準方向を取得する
		@return 可動域基準方向
	 */
	Vec3d GetLimitDir();

	/** @brief 可動域にかかっているかどうかを取得する
		@return かかっていればtrue
	*/
	bool IsOnLimit();

	/** @brief 有効かどうかを設定する
	*/
	void    Enable(bool b);

	/** @brief 有効かどうかを取得する
	*/
	bool    IsEnabled();
};
	
/// ボールジョイントの可動域制限のステート
/*struct PHBallJointLimitState {
	Vec3d f;   ///< 拘束力の力積
};*/

/// ボールジョイントの可動域制限のデスクリプタ
struct PHBallJointLimitDesc{
	SPR_DESCDEF(PHBallJointLimit);
	bool    bEnabled;	///< 有効かどうか
	double	spring;			///< 可動範囲バネ
	double	damper;			///< 可動範囲ダンパ
	Vec3d   limitDir;		///< 可動域の基準方向（円錐可動域なら中心線，など）

	PHBallJointLimitDesc() {
		bEnabled  = false;
		spring    = 10000.0;
		damper    =   100.0;
		limitDir  = Vec3d(0,0,1);
	}
};

// -----  -----  -----  -----  -----

/// ボールジョイントの円錐可動域制限のインタフェース
struct PHBallJointConeLimitIf : public PHBallJointLimitIf{
	SPR_IFDEF(PHBallJointConeLimit);

	/** @brief スイング角の可動範囲を設定する
		@param lower 最小スイング角度
		@param upper 最大スイング角度
	 */
	void SetSwingRange(Vec2d range);

	/** @brief スイング角の可動範囲を取得する
		@param lower 最大スイング角度
		@param upper 最大スイング角度
	 */
	void GetSwingRange(Vec2d& range);

	/** @brief スイング方位角の可動範囲を設定する
	@param lower 最大スイング角度
	@param upper 最大スイング角度
	*/
	void SetSwingDirRange(Vec2d range);

	/** @brief スイング方位角の可動範囲を取得する
	@param lower 最大スイング角度
	@param upper 最大スイング角度
	*/
	void GetSwingDirRange(Vec2d& range);

	/** @brief ツイスト角の可動範囲を設定する
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
	 */
	void SetTwistRange(Vec2d range);

	/** @brief ツイスト角の可動範囲を取得する
		@param lower 最大ツイスト角度
		@param upper 最大ツイスト角度
	 */
	void GetTwistRange(Vec2d& range);
};

/// ボールジョイントの円錐可動域制限のデスクリプタ
struct PHBallJointConeLimitDesc : public PHBallJointLimitDesc {
	SPR_DESCDEF(PHBallJointConeLimit);
	Vec2d	limitSwing;		///< swing角の可動域（[0] or .lower, [1] or .upper）
	Vec2d	limitSwingDir;	///< swing方位角の可動域（[0] or .lower, [1] or .upper） <!!> いささか不安定
	Vec2d	limitTwist;		///< twist角の可動域（[0] or .lower, [1] or .upper）

	PHBallJointConeLimitDesc() {
		limitSwing    = Vec2d(-FLT_MAX, FLT_MAX);
		limitSwingDir = Vec2d(-FLT_MAX, FLT_MAX);
		limitTwist    = Vec2d(-FLT_MAX, FLT_MAX);
	}
};

// -----  -----  -----  -----  -----

/// スプライン制御点
/// スプライン閉曲線の制御点
struct PHSplineLimitNode {
	double swing;
	double swingDir;
	double dSwing;
	double dSwingDir;
	double twistMin;
	double twistMax;

	PHSplineLimitNode(){
		swing     =  0;       swingDir  =  0;
		dSwing    =  0;       dSwingDir =  0;
		twistMin  = -FLT_MAX; twistMax  =  FLT_MAX;
	};
	PHSplineLimitNode(double s,double sd,double ds, double dsd, double tmn, double tmx) {
		swing     = s;   swingDir  = sd;
		dSwing    = ds;  dSwingDir = dsd;
		twistMin  = tmn; twistMax  = tmx;
	}
};

/// ボールジョイントのスプライン可動域制限のインタフェース
struct PHBallJointSplineLimitIf : public PHBallJointLimitIf{
	SPR_IFDEF(PHBallJointSplineLimit);

	/** @brief 制御点を追加する
		@param node 追加する制御点
	 */
	void AddNode(PHSplineLimitNode node);
	void AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax);

	/** @brief 制御点を挿入する
		@param node 追加する制御点
		@param pos  挿入位置
	 */
	void AddNode(PHSplineLimitNode node, int pos);
	void AddNode(double S, double SD, double dS, double dSD, double tMin, double tMax, int pos);

	/** @brief 極におけるツイスト角の可動範囲を設定する
		@param lower 最小ツイスト角度
		@param upper 最大ツイスト角度
	 */
	void SetPoleTwistRange(Vec2d range);

	/** @brief 極におけるツイスト角の可動範囲を取得する
		@param lower 最大ツイスト角度
		@param upper 最大ツイスト角度
	 */
	void GetPoleTwistRange(Vec2d& range);
};

/// ボールジョイントのスプライン可動域制限のデスクリプタ
struct PHBallJointSplineLimitDesc : public PHBallJointLimitDesc {
	SPR_DESCDEF(PHBallJointSplineLimit);
	std::vector<PHSplineLimitNode> nodes;  ///< 可動域を表現するスプライン制御点の配列
	Vec2d poleTwist;                       ///< 極（原点）におけるTwistのリミット

	PHBallJointSplineLimitDesc() {
		poleTwist = Vec2d(-FLT_MAX, FLT_MAX);
	}
};

//@}

}

#endif//SPR_PHJOINTLIMITIf_H
