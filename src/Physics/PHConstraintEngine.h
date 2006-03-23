#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Collision/CDDetectorImp.h>
#include <vector>

namespace Spr{;

/// 剛体の情報
struct PHSolidAux{
	PHSolid*	solid;		/// PHSolidへの参照
	double		minv;		/// 質量の逆数
	Matrix3d	Iinv;		/// 慣性行列の逆行列
	Vec3d		v0, w0;		/// 接触力が0の場合のdt後の速度
	Vec3d		dv, dw;		/// 接触力を考慮した速度変化量
	Vec3d		dV, dW;		/// Correctionによる移動量，回転量
	void SetupDynamics(double dt);
	void SetupCorrection();
};
class PHSolidAuxs : public std::vector<PHSolidAux>{
public:
	iterator Find(PHSolid* s){
		iterator is;
		for(is = begin(); is != end(); is++)
			if(is->solid == s)
				break;
		return is;
	};
	void SetupDynamics(double dt){
		for(iterator is = begin(); is != end(); is++)
			is->SetupDynamics(dt);
	}
	void SetupCorrection(){
		for(iterator is = begin(); is != end(); is++)
			is->SetupCorrection();
	}
};

///
class PHConstraint : public InheritSceneObject<PHConstraintIf, SceneObject>{
public:
	PHJointDesc::JointType	type;
	int			dim_v, dim_w, dim_q;
	int			idx_v[3], idx_w[3], idx_q[3];
	bool		bFeasible;			/// 両方の剛体がundynamicalな場合true

	PHSolidAux* solid[2];
	Matrix3d	Rj[2];				/// 各剛体に張り付いた関節フレーム
	Vec3d		rj[2];
	Matrix3d	Rjrel;				/// 関節フレーム間の位置関係
	Vec3d		rjrel;
			/// 各剛体の速度，角速度から相対速度へのヤコビ行列
			/// 各剛体の速度，角速度から相対角速度へのヤコビ行列
			/// 各剛体の速度，角速度から相対quaternionの時間微分へのヤコビ行列
	/**
				|	  v[0]	  w[0]	  v[1]	  w[1]
	  ----------+---------------------------------
		vrel	|	Jvv[0]	Jvw[0]	Jvv[1]	Jvw[1]
		wrel	|	Jwv[0]	Jww[0]	Jwv[1]	Jww[1]
		qdrel	|	Jqv[0]	Jqw[0]	Jqv[1]	Jqw[1]
	*/
	Matrix3d	Jvv[2], Jvw[2], Jwv[2], Jww[2], Jqv[2], Jqw[2];
	Matrix3d	Tvv[2], Tvw[2], Twv[2], Tww[2], Tqv[2], Tqw[2];
	Vec3d		fv, fw, Fv, Fq, bv, bw, Bv, Bq;
	Vec3d		Av, Aw, Aq;
	
	void Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc){
		solid[0] = lhs, solid[1] = rhs;
		for(int i = 0; i < 2; i++){
			desc.poseJoint[i].Ori().ToMatrix(Rj[i]);
			rj[i] = desc.poseJoint[i].Pos();
		}
	}
	void CompJacobian(bool bCompAngular);
	void SetupDynamics(double dt);
	void SetupCorrection(double dt);
	void IterateDynamics();
	void IterateCorrection();
	virtual void Projectionfv(double& f, int k){}
	virtual void Projectionfw(double& f, int k){}
	virtual void ProjectionFv(double& F, int k){}
	virtual void ProjectionFq(double& F, int k){}
	virtual void CompError() = 0;

	PHConstraint(PHJointDesc::JointType t);
};
class PHConstraints : public std::vector< UTRef<PHConstraint> >{
public:
	void SetupDynamics(double dt){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupDynamics(dt);
	}
	void SetupCorrection(double dt){
		for(iterator it = begin(); it != end(); it++)
			(*it)->SetupCorrection(dt);
	}
	void IterateDynamics(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateDynamics();
	}
	void IterateCorrection(){
		for(iterator it = begin(); it != end(); it++)
			(*it)->IterateCorrection();
	}
};

class PHContactPoint : public PHConstraint{
public:
	CDShapePair* shapePair;
	Vec3d pos;
	virtual void CompError();
	virtual void Projectionfv(double& f, int k);
	virtual void ProjectionFv(double& F, int k);
	PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1);
};
	
class PHHingeJoint : public PHConstraint{
public:
	virtual void CompError();
	PHHingeJoint();
};
class PHSliderJoint : public PHConstraint{
	virtual void CompError();
	PHSliderJoint();

};
class PHBallJoint : public PHConstraint{
	virtual void CompError();
	PHBallJoint();
};

class PHConstraintEngine: public PHEngine{
	friend class PHSolidPair;

	OBJECTDEF(PHConstraintEngine);

	/// Solidの組み合わせの配列
	class PHSolidPair{
	public:
		PHSolidAux* solid[2];
		/// Shapeの組み合わせの配列
		typedef UTCombination<CDShapePair> CDShapePairs;
		CDShapePairs	shapePairs;

		void Init(PHSolidAux* s0, PHSolidAux* s1);	/// 初期化
		bool Detect(PHConstraintEngine* engine);	
		
	};
	typedef UTCombination<PHSolidPair> PHSolidPairs;
	
protected:
	bool			ready;			/// 
	PHSolidAuxs		solids;			/// 剛体の配列
	PHSolidPairs	solidPairs;	
	PHConstraints	points;			///	接触点の配列
	PHConstraints	joints;			///	関節の配列
	int max_iter_dynamics;			/// Dynamics()の反復回数
	int max_iter_correction;		/// Correction()の反復回数
	double step_size;				/// LCPのステップ幅
	double converge_criteria;		/// 収束判定の閾値

	bool DetectPenetration();		/// 全体の交差の検知
	void PrintContacts();
	void SetupDynamics(double dt);	/// LCPの準備
	void SetupCorrection(double dt);/// 誤差の修正
	void IterateDynamics();			/// Correction()での一度の反復
	void IterateCorrection();		/// Correction()での一度の反復
	void UpdateSolids(double dt);	/// 結果をSolidに反映する
	//void SetInitialValue();		/// LCPの決定変数の初期値を設定
	
public:
	void Add(PHSolid* s);			/// Solid を登録する
	void Remove(PHSolid* s);		/// 登録されているSolidを削除する
	void AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc);	/// 関節の追加する
	void EnableContact(PHSolid* lhs, PHSolid* rhs, bool bEnable);
	void Invalidate(){ready = false;}	/// readyフラグをリセット
	void Init();						/// 初期化し，readyフラグをセット
	///
	int GetPriority() const {return SGBP_CONSTRAINTENGINE;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s){ solids.clear(); }

	PHConstraintEngine();
	~PHConstraintEngine();
};

}	//	namespace Spr
#endif
