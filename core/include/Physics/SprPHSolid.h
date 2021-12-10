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
#include <Collision/SprCDDetector.h>
#include <Base/Spatial.h>

/**	\addtogroup gpPhysics	*/
//@{
namespace Spr{;

///	形と位置を指定するための衝突判定用フレーム
struct PHFrameDesc{
	Posed pose;		///< 剛体に対する位置と向き
};
struct CDShapeIf;
struct PHFrameIf: public SceneObjectIf{
	SPR_IFDEF(PHFrame);

	Posed GetPose();
	void  SetPose(Posed p);
	CDShapeIf* GetShape();
};

struct PHBodyIf : SceneObjectIf {
	SPR_IFDEF(PHBody);
	/** @brief 剛体の位置と向きを取得する
	@return シーンに対する剛体の位置と向き
	*/
	Posed		GetPose() const;

	/** @brief 剛体の速度を取得する
	@return シーンに対する剛体の質量中心の速度
	*/
	Vec3d		GetVelocity() const;

	/** @brief 剛体の角速度を取得する
	@return シーンに対する剛体の角速度
	*/
	Vec3d		GetAngularVelocity() const;

	/** @brief 質量中心を取得する
	@return 質量中心の位置(剛体座標系)
	*/
	Vec3d		GetCenterOfMass();

	/** @brief 剛体の位置を取得する
	@return シーンに対する剛体フレーム原点の位置
	*/
	Vec3d		GetFramePosition();

	/** @brief 剛体の質量中心の位置を取得する
	@return シーンに対する剛体の質量中心の位置(World)
	*/
	Vec3d		GetCenterPosition();

	/// 動く物体かどうか
	bool IsDynamical();

	/// 速度が0になり、計算省略のために凍らせているかどうか
	bool IsFrozen();

	/** @brief 剛体に形状を登録する
	@param shape 形状へのポインタ

	剛体が保持する形状リストの末尾に新しく形状を追加する
	*/
	void AddShape(CDShapeIf* shape);
	void AddShapes(CDShapeIf** shBegin, CDShapeIf** shEnd);

	/** @brief 剛体から形状を取り外す
	@param index	形状インデックス

	形状リストからindex番目の形状を削除する．
	その結果，index+1番目以降の形状のインデックスは1つ先頭に向かってシフトするので注意が必要．
	*/
	void RemoveShape(int index);
	void RemoveShapes(int idxBegin, int idxEnd);

	/** @brief 剛体から形状を取り外す
	@param shape	形状へのポインタ

	形状リストの中からshapeを参照している要素をすべて削除する．
	その結果，削除された要素よりも後ろにある要素は先頭に向かってシフトするので注意が必要．
	*/
	void		RemoveShape(CDShapeIf* shape);

	/** @brief 登録されている形状の個数を取得する
	@return 形状の個数

	形状リストの要素数を返す．
	*/
	int			NShape();

	/**	@brief 登録されている形状を取得する
	@param index 形状インデックス
	@return 形状へのポインタ

	形状リストのindex番目の形状を返す．
	*/
	CDShapeIf*	GetShape(int index);

	/** @brief 形状の位置と向きを取得する
	@param index 対象とする形状のインデックス
	@return 剛体に対する形状の位置と向き

	形状リストのindex番目の形状の位置と向きを取得する．
	*/
	Posed		GetShapePose(int index);

	/** @brief 形状の位置と向きを設定する
	@param index 対象とする形状のインデックス
	@param pose 剛体に対する形状の位置と向き

	形状リストのindex番目の位置と向きを設定する．
	*/
	void		SetShapePose(int index, const Posed& pose);

	/** @brief 形状をClearする

	形状リストを空にする．
	*/
	void		ClearShape();

	/** @brief Bounding boxを取得．必要なら再計算
	@param bbmin  bboxの下限
	@param bbmax  bboxの上限
	@param world  trueならワールド座標，falseならローカル座標に平行なbbox
	*/
	void		GetBBox(Vec3d& bbmin, Vec3d& bbmax, bool world);

	/** @brief Bounding Box キャッシュの無効化. Shapeのパラメータを変えた場合には呼び出しが必要。
	*/
	bool InvalidateBbox();
};

///	剛体のステート
struct PHSolidState{
	Vec3d		velocity;		///<	質量中心の速度		(World系)
	Vec3d		angVelocity;	///<	角速度				(World系)
	Posed		pose;			///<	座標原点の位置と向き	(World系)
};

///	剛体のディスクリプタ
struct PHSolidDesc: public PHSolidState{
	double		mass;			///<	質量
	Matrix3d	inertia;		///<	慣性テンソル	(Local系)
	Vec3d		center;			///<	質量中心の位置	(Local系)
	bool		dynamical;		///<	物理法則に従うか(速度は積分される)
	bool        stationary;

	PHSolidDesc(){ Init(); }
	void Init(){
		mass       = 1.0f;
		inertia    = Matrix3d::Unit();
		dynamical  = true;
		stationary = false;
	}
};

struct CDShapeIf; 
struct PHTreeNodeIf;

///	剛体のインタフェース
struct PHSolidIf : public PHBodyIf{
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
		@param f 力（ワールド座標系）
		@param r 作用点の位置（ワールド座標系）
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
	
	/** @brief 質量中心を設定する
		@param center 質量中心の位置(剛体座標系)
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
	
	/** @brief 剛体の位置を設定する
		@param p シーンに対する剛体フレーム原点の位置
	 */
	void		SetFramePosition(const Vec3d& p);
	
	/** @brief 剛体の位置を設定する
		@param p シーンに対する剛体の質量中心の位置(World)
	 */
	void		SetCenterPosition(const Vec3d& p);
	
	/** @brief 剛体の前のステップからの並進移動量を返す
		@param pos 重心からの相対位置．この位置の並進移動量を返す．
		@return 前のステップからの並進移動量
	 */
	Vec3d GetDeltaPosition() const;
	Vec3d GetDeltaPosition(const Vec3d& p) const;

	/** @brief 剛体の向きを取得する
		@return シーンに対する剛体の向き
	 */
	Quaterniond GetOrientation() const;
	
	/** @brief 剛体の向きを設定する
		@param q シーンに対する剛体の向き
	 */
	void		SetOrientation(const Quaterniond& q);

	/** @brief 剛体の位置と向きを設定する
		@param p シーンに対する剛体の位置と向き
	 */
	void		SetPose(const Posed& p);

	/** @brief 剛体の速度を設定する
		@param v シーンに対する剛体の質量中心の速度
	 */
	void		SetVelocity(const Vec3d& v);

    /** @brief 剛体の角速度を設定する
		@param av シーンに対する剛体の角速度
	 */
	void		SetAngularVelocity(const Vec3d& av);

	/** @brief 最後のステップで、拘束力以外に剛体に加わった外力を取得する。
		拘束力は PHConstraint とその派生クラスから取得しなければならない。
		@return 剛体に加えられた力(World系)
	*/
	Vec3d GetForce() const;

	/** @brief 最後のステップで、拘束力以外に剛体に加わった外力トルクを取得する。
		拘束力は PHConstraint とその派生クラスから取得しなければならない。
		@return 剛体に加えられたトルク(World系、剛体の重心周り)
	*/
	Vec3d GetTorque() const;
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

	void        SetStationary(bool bOn);
	bool        IsStationary();


	/** @brief ツリーノードを取得する
		CreateRootNode，CreateTreeNodeによってツリーノードが割り当てられている場合，
		そのノードを返す．それ以外の場合はNULLを返す．
	 */
	PHTreeNodeIf* GetTreeNode();

	/** @brief 剛体の形状を描画するかどうかを指定する
		@param bOn true: 形状を描画 bOn false: 形状を描画しない
	*/
	//void		SetDrawing(bool bOn);

	/** @brief 剛体の形状を描画するかどうかを取得
		@return trueで描画、falseで描画しない
	*/
	//bool		IsDrawn();
};

struct PHShapePairForLCPIf : public CDShapePairIf{
	SPR_IFDEF(PHShapePairForLCP);

	/** @brief sectionの数を取得する
	*/
	int NSectionVertexes();

	/** @brief i番目の交差断面の頂点座標を取得する
		@param i:頂点座標の番号．
	*/
	Vec3d GetSectionVertex(int i);

	/** @brief 交差断面の面積を取得する
	*/
	double GetContactDimension();

	/** @brief 交差断面の単位法線ベクトルを取得する
	*/
	Vec3d GetNormalVector();

	/** @brief 最近接点のペアを取得する
	*/
	void GetClosestPoints(Vec3d& pa, Vec3d& pb);

	/// 形状を取得する
	CDShapeIf* GetShape(int i);
};

struct PHSolidPairIf;
class PHCollisionListener {
public:
	virtual void OnDetect(PHSolidPairIf* sop, CDShapePairIf* shp, unsigned ct, double dt) {}		///< 交差が検知されたときの処理
	virtual void OnContDetect(PHSolidPairIf* sop, CDShapePairIf* shp, unsigned ct, double dt) {}	///< 交差が検知されたときの処理
};

struct PHSolidPairIf : public ObjectIf {
	SPR_IFDEF(PHSolidPair);
	PHSolidIf* GetSolid(int i);
	int NListener();
	PHCollisionListener* GetListener(int i);
	void RemoveListener(int i);
	void AddListener(PHCollisionListener*l, int pos = -1);
};

struct PHSolidPairForLCPIf : public PHSolidPairIf{
	SPR_IFDEF(PHSolidPairForLCP);
	/** @brief 剛体同士の接触が有効かどうかを取得・設定する
	*/
	bool IsContactEnabled();
	void EnableContact(bool enable);

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
