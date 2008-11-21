/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
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

///	形と位置を指定するための衝突判定用フレーム
struct PHFrameDesc{
	Posed pose;
};
struct PHFrameIf: public SceneObjectIf{
	SPR_IFDEF(PHFrame);
	Posed GetPose();
	void SetPose(Posed p);
};



///	剛体のステート
struct PHSolidState{
	Vec3d		velocity;		///<	速度			(World系)
	Vec3d		angVelocity;	///<	角速度			(World系)
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
	Vec3d		center;			///<	質量中心の位置	(Local系)
	bool		dynamical;		///<	物理法則に従うか

	PHSolidDesc(){ Init(); }
	void Init(){
		mass = 1.0f;
		inertia = Matrix3d::Unit();
		dynamical = true;
	}
};

struct CDShapeIf; 
///	剛体のインタフェース
struct PHSolidIf : public SceneObjectIf{
	SPR_IFDEF(PHSolid);

	/** @brief 力を質量中心に加える
		@param f 力
	 */
	void AddForce(Vec3d f);
	
	/** @brief トルクを加える
		@param t トルク
	 */
	void AddTorque(Vec3d t);
	
	/**	@brief 力を指定した作用点に加える
		@param f 力
		@param r シーン座標系で表わした作用点の位置
	 */
	void AddForce(Vec3d f, Vec3d r);

	/**	@brief 質量を取得する
		@return 質量
	 */
	double		GetMass();
	
	/** @brief 質量の逆数を取得する
		@return 質量の逆数
	 */
	double		GetMassInv();
	
	/** @brief 質量を設定する
		@param m 質量
	 */
	void		SetMass(double m);
	
	/** @brief 質量中心を取得する
		@return 質量中心の位置
	 */
	Vec3d		GetCenterOfMass();
	
	/** @brief 質量中心を設定する
		@param center 質量中心の位置
	 */
	void		SetCenterOfMass(const Vec3d& center);
	
	/** @brief 慣性テンソルを取得する
		@return 慣性テンソル
	 */
	Matrix3d	GetInertia();
	
	/** @brief 慣性テンソルの逆数を取得する
		@return 慣性テンソルの逆数
	 */
	Matrix3d	GetInertiaInv();
	
	/** @brief 慣性テンソルを設定する
		@param I 慣性テンソル
	 */
	void		SetInertia(const Matrix3d& I);

	/** @brief 形状と密度から重心，質量，慣性テンソルを自動的に設定する
	 */
	void		CompInertia();
	
	/** @brief 剛体の位置を取得する
		@return シーンに対する剛体フレーム原点の位置
	 */
	Vec3d		GetFramePosition() const;
	
	/** @brief 剛体の位置を設定する
		@param p シーンに対する剛体フレーム原点の位置
	 */
	void		SetFramePosition(const Vec3d& p);
	
	/** @brief 剛体の質量中心の位置を取得する
		@return シーンに対する剛体の質量中心の位置
	 */
	Vec3d		GetCenterPosition() const ;
	
	/** @brief 剛体の位置を設定する
		@param p シーンに対する剛体の質量中心の位置
	 */
	void		SetCenterPosition(const Vec3d& p);
	
	/** @brief 剛体の前のステップからの並進移動量を返す
		@return 前のステップからの並進移動量
	 */
	Vec3d GetDeltaPosition(const Vec3d& p) const;

	/** @brief 剛体の前のステップからの並進移動量を返す．
		@param pos 重心からの相対位置．この位置の並進移動量を返す．
		@return 前のステップからの並進移動量
	*/
	Vec3d GetDeltaPosition() const;

	/** @brief 剛体の向きを取得する
		@return シーンに対する剛体の向き
	 */
	Quaterniond GetOrientation() const;
	
	/** @brief 剛体の向きを設定する
		@param q シーンに対する剛体の向き
	 */
	void		SetOrientation(const Quaterniond& q);

	/** @brief 剛体の位置と向きを取得する
		@return シーンに対する剛体の位置と向き
	 */
	Posed		GetPose() const;
	
	/** @brief 剛体の位置を取得する
		@return シーンに対する剛体の位置
	*/
	Vec3d		GetPoseR() const;

	/** @brief	剛体の向きを取得する
	*/
	Quaterniond GetPoseQ() const;

	/** @brief 剛体の位置と向きを設定する
		@param p シーンに対する剛体の位置と向き
	 */
	void		SetPose(const Posed& p);

	/** @brief 剛体の速度を取得する
		@return シーンに対する剛体の質量中心の速度
	 */
	Vec3d		GetVelocity() const ;

	/** @brief 剛体の速度を設定する
		@param v シーンに対する剛体の質量中心の速度
	 */
	void		SetVelocity(const Vec3d& v);
	/** @brief 剛体の角速度を取得する
		@return シーンに対する剛体の角速度
	 */
	Vec3d		GetAngularVelocity() const;
	
    /** @brief 剛体の角速度を設定する
		@param av シーンに対する剛体の角速度
	 */
	void		SetAngularVelocity(const Vec3d& av);
    /** @brief 最後のステップで剛体に加わった力を取得する。
		@return 剛体に加えられた力(World系)
	*/
	Vec3d GetForce() const;
    /** @brief 最後のステップで剛体に加わったトルクを取得する。
		@return 剛体に加えられたトルク(World系、剛体の重心周り)
	*/
	Vec3d GetTorque() const;
	/** @brief 剛体に形状を登録する
		@param shape 形状へのポインタ
	 */
	void		AddShape(CDShapeIf* shape);
	/** @brief 登録されている形状の個数を取得する
		@return 形状の個数
	 */
	int			NShape();
	/**	@brief 登録されている形状を取得する
		@param index 形状のインデックス
		@return 形状へのポインタ
		最初に登録された形状のインデックスは0，その次に登録された形状のインデックスは1，
		以下同様．
	 */
	CDShapeIf*	GetShape(int index);
	/** @brief 形状の位置と向きを取得する
		@param index 対象とする形状のインデックス
		@return 剛体に対する形状の位置と向き
		インデックスについてはGetShapeを参照．
	 */
	Posed		GetShapePose(int index);
	/** @brief 形状の位置と向きを設定する
		@param index 対象とする形状のインデックス
		@param pose 剛体に対する形状の位置と向き
		インデックスについてはGetShapeを参照．
	 */
	void		SetShapePose(int index, const Posed& pose);
	
	/** @brief 重力を有効/無効化する
		@param bOn trueならば剛体に重力が加わる．falseならば加わらない．
	 */
	void		SetGravity(bool bOn);
	/** @brief 物理法則に従うかどうかを有効/無効化する
		@param bOn trueならば剛体は物理法則にしたがって運動する．
		物理法則に従わない剛体も，SetPose，SetVelocityなどの関数による位置や速度の更新は
		可能です．
	 */
	void		SetDynamical(bool bOn);
	/** @brief 物理法則に従うかどうかを取得する
		@return trueならば剛体は物理法則にしたがって運動する．
	 */
	bool		IsDynamical();
	/** @brief 位置，速度を更新するかどうかを有効/無効化する
		@param bOn trueならば位置，速度の更新を無効化する．
	 */
	void		SetFrozen(bool bOn);
	/** @brief 位置，速度を更新するかどうかを取得する
		@param bOn trueならば位置，速度の更新を無効化する．
	*/	
	bool		IsFrozen();
};

struct PHShapePairForLCPIf : public ObjectIf{
	SPR_IFDEF(PHShapePairForLCP);

	/** @brief sectionの数を取得する
	*/
	int NSectionVertexes();

	/** @brief i番目の交差断面の頂点座標を取得する
		@param i:頂点座標の番号．
	*/
	Vec3d GetSectionVertex(int i);
};

struct PHSolidPairForLCPIf : public ObjectIf{
	SPR_IFDEF(PHSolidPairForLCP);
	
	PHSolidIf* GetSolid(int i);

	/** @brief 剛体同士の接触状態を取得する
		@param i,j:Shapeの番号．
		ただしFindBySolidPairとセットで使うこと.
	*/
	int	GetContactState(int i, int j);

	/** @brief 剛体同士の接触座標（内点）を取得する
		@param i,j:Shapeの番号．
		ただしFindBySolidPairとセットで使うこと.
	*/
	Vec3d GetCommonPoint(int i, int j);

	/** @brief 最後に接触したときのカウントを取得する
		@param i,j:Shapeの番号．
		ただしscene.countと比較する際に一定のズレがあるので注意.
	*/
	unsigned GetLastContactCount(int i, int j);

	/** @brief 接触したときのめり込み量を取得する
		@param i,j:Shapeの番号．
	*/
	double GetContactDepth(int i, int j);

	/** @brief 接触したときの形状ペアを取得する
		@param i,j:Shapeの番号．
	*/
	PHShapePairForLCPIf* GetShapePair(int i, int j);
};


//@}

}	//	namespace Spr
#endif
