/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
/**
 *	@file SprPHSolid.h
 *	@brief 剛体
*/
#ifndef SPR_PHSolidIf_H
#define SPR_PHSolidIf_H
#include <Foundation/SprObject.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;
///	剛体のステート
struct PHSolidState{
	Vec3d		velocity;		///<	速度			(World系)
	Vec3d		angVelocity;	///<	角速度			(World系)
	Vec3d		center;			///<	質量中心の位置	(Local系)
	Posed		pose;			///<	位置と向き		(World系)

	Vec3d		force;			///<	前の積分でこの剛体に加わった力(World系)
	Vec3d		torque;			///<	前の積分でこの剛体に加わったトルク(World系)
	Vec3d		nextForce;		///<	次の積分でこの剛体に加わる力(World系)
	Vec3d		nextTorque;		///<	次の積分でこの剛体に加わるトルク(World系)
};
///	剛体のディスクリプタ
struct PHSolidDesc: public PHSolidState{
	double		mass;			///<	質量
	Matrix3d	inertia;		///<	慣性テンソル	(Local系)
	bool		gravity;		///<	重力の影響を受けるか

	PHSolidDesc(){ Init(); }
	void Init(){
		mass = 1.0f;
		inertia=Matrix3d::Unit();
		gravity=true;
	}
};

struct CDShapeIf; 
///	剛体のインタフェース
struct PHSolidIf : public SceneObjectIf{
	IF_DEF(PHSolid);

	/** @brief 力を質量中心に加える
		@param f 力
	 */
	virtual void AddForce(Vec3d f)=0;
	/** @brief トルクを加える
		@param t トルク
	 */
	virtual void AddTorque(Vec3d t)=0;
	/**	@brief 力を指定した作用点に加える
		@param f 力
		@param r シーン座標系で表わした作用点の位置
	 */
	virtual void AddForce(Vec3d f, Vec3d r)=0;

	/**	@brief 質量を取得する
		@return 質量
	 */
	virtual double		GetMass()=0;
	/** @brief 質量の逆数を取得する
		@return 質量の逆数
	 */
	virtual double		GetMassInv()=0;
	/** @brief 質量を設定する
		@param m 質量
	 */
	virtual void		SetMass(double m)=0;
	/** @brief 慣性テンソルを取得する
		@return 慣性テンソル
	 */
	virtual Matrix3d	GetInertia()=0;
	/** @brief 慣性テンソルを設定する
		@param I 慣性テンソル
	 */
	virtual void		SetInertia(const Matrix3d& I)=0;
	/** @brief 剛体の位置を取得する
		@return シーンに対する剛体フレーム原点の位置
	 */
	virtual Vec3d		GetFramePosition() const =0;
	/** @brief 剛体の位置を設定する
		@param p シーンに対する剛体フレーム原点の位置
	 */
	virtual void		SetFramePosition(const Vec3d& p) =0;
	/** @brief 剛体の質量中心の位置を取得する
		@return シーンに対する剛体の質量中心の位置
	 */
	virtual Vec3d		GetCenterPosition() const  =0;
	/** @brief 剛体の位置を設定する
		@param p シーンに対する剛体の質量中心の位置
	 */
	virtual void		SetCenterPosition(const Vec3d& p) =0;

	/** @brief 剛体の傾きを取得する
		@return シーンに対する剛体の傾き
	 */
	virtual Quaterniond GetOrientation() const =0;
	/** @brief 剛体の傾きを設定する
		@param q シーンに対する剛体の傾き
	 */
	virtual void		SetOrientation(const Quaterniond& q) =0;

	/** @brief 剛体の位置と傾きを取得する
		@return シーンに対する剛体の位置と傾き
	 */
	virtual Posed		GetPose() const = 0;
	/** @brief 剛体の位置と傾きを設定する
		@param p シーンに対する剛体の位置と傾き
	 */
	virtual void		SetPose(const Posed& p) = 0;

	/** @brief 剛体の速度を取得する
		@return シーンに対する剛体の質量中心の速度
	 */
	virtual Vec3d		GetVelocity() const  =0;
	/** @brief 剛体の速度を設定する
		@param v シーンに対する剛体の質量中心の速度
	 */
	virtual void		SetVelocity(const Vec3d& v) =0;
	/** @brief 剛体の角速度を取得する
		@return シーンに対する剛体の角速度
	 */
	virtual Vec3d		GetAngularVelocity() const =0;
    /** @brief 剛体の角速度を設定する
		@param av シーンに対する剛体の角速度
	 */
	virtual void		SetAngularVelocity(const Vec3d& av)=0;

	/** @brief 剛体に形状を登録する
		@param shape 形状へのポインタ
	 */
	virtual void		AddShape(CDShapeIf* shape)=0;

	/** @brief 形状の位置と傾きを取得する
		@param index 対象とする形状のインデックス
		@return 剛体に対する形状の位置と傾き
		最初に登録された形状のインデックスは0，その次に登録された形状のインデックスは1，
		以下同様．
	 */
	virtual Posed		GetShapePose(int index)=0;
	/** @brief 形状の位置と傾きを設定する
		@param index 対象とする形状のインデックス
		@param pose 剛体に対する形状の位置と傾き
		インデックスについてはGetShapePoseを参照．
	 */
	virtual void		SetShapePose(int index, const Posed& pose)=0;

	///	重力を有効/無効化する	ここでよいか疑問
	virtual void		SetGravity(bool bOn)=0;
	/// 動力学法則に従うかどうかを有効/無効化する
	virtual void		SetDynamical(bool bOn)=0;
	virtual bool		IsDynamical()=0;

	/// 登録されている形状の個数を取得
	virtual int			NShape()=0;

	///	形状を取得
	virtual CDShapeIf*	GetShape(int i)=0;

};

//@}

}	//	namespace Spr
#endif
