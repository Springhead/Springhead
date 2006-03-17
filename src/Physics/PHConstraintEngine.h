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
	PHSolidAux* solid[2];
	Matrix3d	Rj[2];		/// 各剛体に張り付いた関節フレーム
	Vec3d		rj[2];
	Matrix3d	Rjrel;		/// 関節フレーム間の位置関係
	Vec3d		rjrel;
	Matrix3d	Jvrel_v[2], Jvrel_w[2], Jwrel_v[2], Jwrel_w[2];
	void Init(PHSolidAux* lhs, PHSolidAux* rhs, const PHJointDesc& desc){
		solid[0] = lhs, solid[1] = rhs;
		for(int i = 0; i < 2; i++){
			desc.poseJoint[i].Ori().ToMatrix(Rj[i]);
			rj[i] = desc.poseJoint[i].Pos();
		}
	}
	void CompRelativeVelJacobian();
	virtual void SetupDynamics(double dt) = 0;
	virtual void SetupCorrection(double dt) = 0;
	virtual void IterateDynamics() = 0;
	virtual void IterateCorrection() = 0;
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

template<int N>
class PHConstraintND : public PHConstraint{
public:
	typedef PTM::TVector<N, double> VecNd;
	VecNd	f, F, b, B;
	PTM::TMatrixCol<N, 3, double>	Jvrel, Jwrel;
	PTM::TMatrixCol<N, 3, double>	Jv[2], Jw[2];
	PTM::TMatrixCol<3, N, double>	Tv[2], Tw[2];
	PTM::TMatrixCol<N, N, double>	A, Ainv;
	PTM::TVector<6-N, double>		u;
	virtual void CompJacobian();
	virtual void SetupDynamics(double dt);
	virtual void SetupCorrection(double dt);
	virtual void IterateDynamics();
	virtual void IterateCorrection();
	virtual void CompJointJacobian(){}
	virtual void ProjectionDynamics(VecNd& f){}
	virtual void ProjectionCorrection(VecNd& F){}
	virtual void CompError() = 0;
};

class PHContactPoint : public PHConstraintND<3>{
public:
	CDShapePair* shapePair;
	Vec3d pos;
	virtual void CompJacobian();
	virtual void CompError();
	virtual void ProjectionDynamics(VecNd& f);
	virtual void ProjectionCorrection(VecNd& F);
	PHContactPoint(CDShapePair* sp, Vec3d p, PHSolidAux* s0, PHSolidAux* s1):shapePair(sp), pos(p){
		solid[0] = s0, solid[1] = s1;
	}
};
	
class PHHingeJoint : public PHConstraintND<5>{
public:
	virtual void CompJointJacobian();
	virtual void CompError();
	PHHingeJoint();
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
