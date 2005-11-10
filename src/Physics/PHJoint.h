#ifndef PH_JOINT_H
#define PH_JOINT_H


#include <Base/BaseUtility.h>
#include <Base/TinyVec.h>
#include <Base/TinyMat.h>
#include <FileIO/FIDocScene.h>
#include <SceneGraph/SGScene.h>
#include <Physics/PHSolid.h>
#include <WinBasis/WBPreciseTimer.h>
#include <float.h>

//	角速度の最大値．関節が高速に回りすぎて発散するのを防ぐ
#define PHJOINT_MAX_VELOCITY	(1000*M_PI)

/**	@file ３次元連結剛体系
	解説：
	Featherstoneのアルゴリズムを利用してツリー状に連結された剛体の
	運動をシミュレートするクラス

	開発責任者：田崎勇一 tazaki@cyb.mei.titech.ac.jp

	参考文献："Impulse-based Dynamic Simulation of Rigid Body Systems" Brian Mirtich 1996
*/
namespace Spr{;

/**@name	Featherstone's method で使われる"spatial matrix/vector"
	上3つが(力，角速度，角加速度など)をあらわし，
	下3つが(トルク，速度，加速度)をあらわす．(と思う) by hase
	力とトルクで回転と並進が逆なので注意．
*/
//@{
typedef PTM::TVector<6, double> SpVec6d;
typedef PTM::TSubVector<3, SpVec6d::desc> SpSubVec3d;
typedef PTM::TMatrixCol<6, 6, double> SpMatrix6d;
typedef PTM::TSubMatrixCol<3, 3, SpMatrix6d::desc> SpSubMatrix3d;
//@}

typedef Vec3f Vector;
typedef Matrix3f Matrix3x3;
DEF_RECORD(XJointBase, {
	GUID Guid(){ return WBGuid("23F6D545-8987-4aa6-BBE1-2FE32C096D5A"); }	
	Matrix3x3	pRj;
	Vector		prj;
	Matrix3x3	cRj;
	Vector		crj;
});

///	関節の基本クラス．ツリー構造を作る．PHJointEngineがツリーを持つ．
class PHJointBase : public UTTreeNode<PHJointBase>, public SGObject{
public:
	typedef std::vector<UTRef<PHJointBase> > array_type;
	friend class PHJointEngine;
	friend class PHJointClearForce;
	friend class PHJointLoader;
	friend class CRHuman;
public:
	SGOBJECTDEFABST(PHJointBase);
	///	積分の方法． Method of solver 
	enum IntType{
		SYMPLETIC,
		ANALYTIC,
	}intType;

	UTRef<PHSolid> solid;	///<	子Solid．関節は親Solidと子Solidをつなぐ
	UTRef<SGFrame> frame;	///<	ルートノードが固定の場合のFrame
	/** @name 座標系の説明
		関節フレームの説明
			関節の位置と傾きを表現するためのフレーム。
			関節フレーム原点は関節軸の位置を表す。
			軸と座標の関係は関節の種類(派生クラス)によって異なる．
			- PHJointHinge:	フレームのZ軸が回転軸の向きを表す。
							pRc = pRj * Rot(position, 'z') * cRj.trans()
							prc = cRp*(-parent->solid->center + prj) - (crj - solid->center)
			- PHJointSlider:	フレームのZ軸が直動軸の向きを表す。
							pRc = pRj * cRj.trans()
							prc = cRp*(-parent->solid->center + prj) + cRj*(0,0,position) - (crj - solid->center)  
			- PHJointUniv:	フレームのX,Y軸が，第1軸・第2軸を表す．
							pRc = pRj * Rot(position[0], 'x') * Rot(position[1], 'y') * cRj.trans()
							prc = cRp*(-parent->solid->center + prj) - (crj - solid->center)
			- PHJointBall:	3軸動く．可動域はZ軸が動く範囲を円錐で指定する．
							pRc = pRj * position * cRj.trans()
							prc = cRp*(-parent->solid->center + prj) - (crj - solid->center)
		以下のコメントで，
		- Fc := child solid's frame		子剛体の座標系
		- Fp := parent solid's frame	親剛体の座標系
		- Fj := joint's frame			ジョイントの座標系(親側Fjpと子側Fjcがある)
		を意味する
	*/
	//@{
	Matrix3d		pRj, cRj;			///<	3x3回転行列  rotation matrix(Fp to Fjp, Fc to Fjc)
	Vec3d			prj, crj;			///<	並進ベクトル radius vector(Fp to Fjp in Fp, Fc to Fjc in Fc)

protected:
	Vec3d			prc;				///<	並進ベクトル radius vector(Fp to Fc in Fc)
	Matrix3d		pRc, cRp;			///<	3x3回転行列  rotation matrix(Fp to Fc, Fc to Fp)
	Matrix3d		R;					///<	orientation matrix
	Quaterniond		quat;				///<	orientation quaternion
	Vec3d			p;					///<	position vector
	Vec3d			v, v_abs;			///<	velocity in Fc/world coord.
	Vec3d			w, w_abs;			///<	angular velocity in Fc/world coord.
	Vec3d			pvc, pwc;			///<	[angular]velocity relative to Fp in Fc coord. 
	SpMatrix6d		Ii;					///<	spatial isolated inertia
	SpMatrix6d		Ia;					///<	spatial articulated inertia
	SpVec6d			Za;					///<	zero accelaration force in Fc coord.
	SpVec6d			c;					///<	Coriolis vector in Fc coord.
	SpVec6d			a;					///<	spatial accelaration in Fc coord.
	//@}

	//	sを含まない変数のキャッシュ．
	Matrix3d	rcross, rpcross, rcross_cRp, rpcross_pRc;
	SpVec6d		a_p, Ia_c, Z_plus_Ia_c;
public:
	///	コンストラクタ
	PHJointBase();
	///	ロード後の初期化．再帰．
	void Loaded(SGScene* scene);
	///	指定したPHSolidを小に持つノードを検索する
	PHJointBase* Search(PHSolid*);
	///	親剛体のフレームから見た関節姿勢
	Affinef GetPostureFromParent(){ Affinef rv; rv.Pos()=prj; rv.Rot()=pRj; return rv; }
	///	子剛体のフレームから見た関節姿勢
	Affinef GetPostureFromChild(){ Affinef rv; rv.Pos()=crj; rv.Rot()=cRj; return rv; }
	///	子剛体の角加速度(World系)
	Vec3f GetSolidAngularAccel(){ return solid->GetRotation() * a.sub_vector(0, Vec3f()); }
	///	子剛体の加速度(World系)
	Vec3f GetSolidAccel(){ return solid->GetRotation() * a.sub_vector(3, Vec3f()); }
	///	子剛体の角加速度(World系)
	Vec3f GetSolidAngularVelocity(){ return w_abs; }
	///	子剛体の加速度(World系)
	Vec3f GetSolidVelocity(){ return v_abs; }
	/// 絶対座標での関節の向き
	Matrix3d GetOrientation(){return R;}
	
	///	関節の自由度
	virtual int GetJointDof()=0;
	///	関節位置の取得
	virtual double GetJointPosition(int i)=0;
	///	関節速度の取得
	virtual double GetJointVelocity(int i)=0;
	///	関節加速度の取得
	virtual double GetJointAccel(int i)=0;
	///	関節トルクの取得
	virtual double GetJointTorque(int i)=0;
	///	関節トルクの設定
	virtual void SetJointTorque(double v, int i)=0;
	///	関節トルクを加える
	virtual void AddJointTorque(double v, int i)=0;

	///@name	Featherstone's algorithm
	//@{
	///	コリオリの力による加速度の計算
	void CompCoriolisAccelRecursive(double dt);	
	///	articulated inertia & ZA-force
	virtual void CompArticulatedInertia(double dt);
	///	積分
	virtual void Integrate(SGScene* scene);
	/**	このノードの加速度を計算したい場合に呼ぶ．
		親ノードの加速度は副作用で求まる．子ノードについては計算しない．
		
		呼び出しかた．
		1. root->CalcCoriolisAccelRecusive(dt);
		2. 全ジョイントについて，PHJointBase::torque に値を直接代入する．
		3. root->CalcArticulatedInertia(dt);
		4. CalcAccel(dt)
		5. ジョイントの a から加速度を読み出す(座標系に注意)
		6. 2-5をトルクパターンを変えながら呼び出す．
		
		Compute accelaration of the child solid of this joint.
		For partial use of Featherstone's algorithm.
	*/
	virtual void CalcAccel(double dt)=0;
	//@}

	///	所有しているオブジェクトの数
	virtual size_t NChildObjects();
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i);
	///	参照しているオブジェクトの数
	virtual size_t NReferenceObjects();
	///	参照しているオブジェクト
	virtual SGObject* ReferenceObject(size_t i);
	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	子オブジェクトの削除
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates();

	void SaveX(XJointBase& x) const;
	void LoadX(const XJointBase& x);

	//	関節角を-π～πに制限する関数
	virtual void LimitAngle(double& d){}
protected:
	///	派生クラスが基本クラス型オブジェクトのメンバにアクセスするための手段
	template <class T> T& OfParent(T PHJointBase::* member){
		return GetParent()->*member;
	}

	inline SpMatrix6d pXc_Mat_cXp(SpMatrix6d& m);
	inline SpVec6d	pXc_Vec(SpVec6d& v);
	inline SpVec6d cXp_Vec(SpVec6d& v);

	virtual void CompJointAxis()=0;
	virtual void CompRelativePosition()=0;
	virtual void CompRelativeVelocity()=0;
	virtual void CompCoriolisAccel()=0;
	void UpdateSolid();
	void UpdateJointPosture();
	void PropagateState();				///<	位置・速度の伝播（非再帰関数）	
	virtual void ClearTorqueRecursive();
	virtual void ClearForce();
	/**	姿勢などの再設定	非再帰
		基本的な状態量(PHJointStateの内容)がLoadedやLoadStateでセットされた後に、
		従属変数を計算するための関数	*/
	virtual void Reset();
	virtual void LoadState(const SGBehaviorStates& states);
	virtual void SaveState(SGBehaviorStates& states) const;
	virtual double MassFactor(){ return 0; }
};

class PHJointRoot:public PHJointBase{
public:
	SGOBJECTDEF(PHJointRoot);
	virtual void Reset();
	virtual void CalcAccel(double dt);
	virtual void CompJointAxis(){}
	virtual void CompRelativePosition(){}
	virtual void CompRelativeVelocity(){}
	virtual void CompCoriolisAccel(){}
	virtual void Integrate(SGScene* scene);
	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;	
protected:
	virtual int GetJointDof(){ return 0; }
	virtual double GetJointPosition(int i){ return 0; }
	virtual double GetJointVelocity(int i){ return 0; }
	virtual double GetJointAccel(int i){ return 0; }
	virtual double GetJointTorque(int i){ return 0; }
	virtual void SetJointTorque(double v, int i){}
	virtual void AddJointTorque(double v, int i){}
};


/**	PHJointEngine
	関節エンジン．1つのArticulated Bodyに対応する．
*/
class PHJointEngine	: public PHSolverBase{
public:
	SGOBJECTDEF(PHJointEngine);
	typedef PHJointBase::array_type array_type;		///<	ジョイントの配列
	UTRef<PHJointBase> root;						///<	ルートノード．関節ではない．
	void ClearForce(){ root->ClearForce(); }
	typedef std::set< UTRef<PHSolid> > PHSolidSet;	///<	剛体のセット．
	PHSolidSet solids;

	//	Featherstone法にかかる時間の計測
	WBPreciseTimer timer;
	

	int GetPriority() const {return SGBP_JOINTENGINE;}
	void Step (SGScene* s);
	void Loaded(SGScene* scene);
	void Clear(SGScene* s){}
	PHJointEngine::PHJointEngine(){}

	///	所有しているオブジェクトの数
	virtual size_t NChildObjects();
	///	所有しているオブジェクト
	virtual SGObject* ChildObject(size_t i);
	///	子オブジェクトの追加
	virtual bool AddChildObject(SGObject* o, SGScene* s);
	///	子オブジェクトの削除
	virtual bool DelChildObject(SGObject* o, SGScene* s);
	///	子になりえるオブジェクトの型情報の配列
	virtual const UTTypeInfo** ChildCandidates();
	///	オブジェクトを子孫に持っているかどうか．
	bool Has(SGObject*);

	///	状態の読み出し
	virtual void LoadState(const SGBehaviorStates& states);
	///	状態の保存
	virtual void SaveState(SGBehaviorStates& states) const;

	///
	template <class CT> void EnumJoint(CT& ct){
		typedef void (PHJointEngine::*Func)(PHJointBase* b, CT& ct);
		Func func = &PHJointEngine::EnumJointFunc;
		root->MemberTraverse(this, func, ct);
	}
protected:
	template <class CT> void EnumJointFunc(PHJointBase* p, CT& ct){
		CT::value_type v = DCASTP(CT::value_type, p);
		if (v) ct.push_back(v);
	}
};

/**	ステップの最初でJointのトルクをクリアする．	*/
class PHJointClearForce:public SGBehaviorEngine{
	SGOBJECTDEF(PHJointClearForce);
public:
	UTRef<PHJointEngine> je;
	///	クリアする
	virtual void Step(SGScene* s);
	virtual int GetPriority() const { return SGBP_CLEARFORCE; }
};

//----------------------------------------------------------------------------
//	実装

///	要素アクセス
inline SpSubVec3d& svitem(SpVec6d& v, int i){
	return *(SpSubVec3d*)((double*)&v + i*3);
}
inline SpSubMatrix3d& smitem(SpMatrix6d& m, int i, int j){
	return m.sub_matrix(i * 3, j * 3, PTM::TMatDim<3, 3>());
}
///	独自の内積定義(v1^T * v2)
inline double svdot(const SpVec6d& v1, const SpVec6d& v2){
	return 
		v1[0] * v2[3] + v1[1] * v2[4] + v1[2] * v2[5] + 
		v1[3] * v2[0] + v1[4] * v2[1] + v1[5] * v2[2];
}

///	v1 * v2^T で得られる行列
inline Matrix3d mat(const Vec3d& v1, const Vec3d& v2){
	return Matrix3d(
		v1[0] * v2[0], v1[0] * v2[1], v1[0] * v2[2],
		v1[1] * v2[0], v1[1] * v2[1], v1[1] * v2[2],
		v1[2] * v2[0], v1[2] * v2[1], v1[2] * v2[2]);
}
inline SpMatrix6d svmat(const SpVec6d& v1, const SpVec6d& v2){
    PTM::TVecDim<3> dim3;
	const SpSubVec3d& v11 = v1.sub_vector(0, dim3), v12 = v1.sub_vector(3, PTM::TVecDim<3>());
	const SpSubVec3d& v21 = v2.sub_vector(0, dim3), v22 = v2.sub_vector(3, PTM::TVecDim<3>());
	SpMatrix6d y;
	y.sub_matrix(0, 0, PTM::TMatDim<3, 3>()) = mat(v11, v22);
	y.sub_matrix(0, 3, PTM::TMatDim<3, 3>()) = mat(v11, v21);
	y.sub_matrix(3, 0, PTM::TMatDim<3, 3>()) = mat(v12, v22);
	y.sub_matrix(3, 3, PTM::TMatDim<3, 3>()) = mat(v12, v21);
	return y;
}
SpMatrix6d PHJointBase::pXc_Mat_cXp(SpMatrix6d& m){
	static Matrix3d pRc_m11_cRp, pRc_m12_cRp, pRc_m21_cRp, pRc_m22_cRp, tmp;
	pRc_m11_cRp = pRc * smitem(m, 0, 0) * cRp;
	pRc_m12_cRp = pRc * smitem(m, 0, 1) * cRp;
	pRc_m21_cRp = pRc * smitem(m, 1, 0) * cRp;
	pRc_m22_cRp = pRc * smitem(m, 1, 1) * cRp;
	tmp = pRc_m11_cRp - pRc * smitem(m, 0, 1) * rcross_cRp;
	SpMatrix6d	y;
	smitem(y, 0, 0) = tmp;
	smitem(y, 0, 1) = pRc_m12_cRp;
	smitem(y, 1, 0) = rpcross * tmp + pRc_m21_cRp - pRc * smitem(m, 1, 1) * rcross_cRp;
	smitem(y, 1, 1) = rpcross * pRc_m12_cRp + pRc_m22_cRp;
	return y;
}

SpVec6d	PHJointBase::pXc_Vec(SpVec6d& v){
	Vec3d pRc_v1 = pRc * svitem(v, 0);
	SpVec6d y;
	svitem(y, 0) = pRc_v1;
	svitem(y, 1) = rpcross * pRc_v1 + pRc * svitem(v, 1);
	return y;
}

SpVec6d PHJointBase::cXp_Vec(SpVec6d& v){
	Vec3d cRp_v1 = cRp * svitem(v, 0);
	SpVec6d y;
	svitem(y, 0) = cRp_v1;
	svitem(y, 1) = -rcross * cRp_v1 + cRp * svitem(v, 1);
	return y;
}


}

#endif