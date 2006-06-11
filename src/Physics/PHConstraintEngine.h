#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <Physics/PHContactDetector.h>
#include <Physics/PHConstraint.h>

namespace Spr{;

class PHSolid;
class PHJoint;
class PHConstraintEngine;

///	形状の組
class PHShapePairForLCP : public CDShapePair{
public:
	///	コンストラクタ
	//PHShapePairForLCP(){}
	//PHShapePair(CDShape* s0, CDShape* s1):CDShapePair(s0, s1){}
	///	接触判定．接触の中心(center 最侵入点の中点)と法線ベクトルまで求める
	//bool Detect(unsigned ct, PHSolid* s0, PHSolid* s1, int ishape0, int ishape1);
	void CalcNormal(PHSolid* solid0, PHSolid* solid1);
	///	接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．
	void EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolidInfoForLCP* solid0, PHSolidInfoForLCP* solid1);
	//void SetState(const PHShapePairState& s);
};

/// Solidの組
class PHConstraintEngine;
class PHSolidPairForLCP : public PHSolidPair<PHSolidInfoForLCP, PHShapePairForLCP, PHConstraintEngine>{
public:
	//virtual void Clear();
	//virtual void Init(PHSolidInfoFor* s0, PHSolidInfo* s1);	/// 初期化
	virtual void OnDetect(PHShapePairForLCP* cp, PHConstraintEngine* engine, unsigned ct, double dt);
};

class PHConstraintEngine : public PHContactDetectorImp<PHSolidInfoForLCP, PHShapePairForLCP, PHSolidPairForLCP, PHConstraintEngine>{
	typedef PHContactDetectorImp<PHSolidInfoForLCP, PHShapePairForLCP, PHSolidPairForLCP, PHConstraintEngine> base_type;
	/*friend class PHSolidPair;
	friend class PHSolid;*/
	friend class PHConstraint;

	OBJECT_DEF_NOIF(PHConstraintEngine);

	PHJoint* CreateJoint(const PHJointDesc& desc);
	
public:
	PHConstraintEngine();
	~PHConstraintEngine();
	//void Add(PHSolid* s);			///< Solid を登録する
	//void Remove(PHSolid* s);		///< 登録されているSolidを削除する
	PHJoint* AddJoint(const PHJointDesc& desc);	///< 関節を追加する（ファイルローダ用）
	PHJoint* AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc);	///< 関節の追加する
	
	virtual int GetPriority() const {return SGBP_CONSTRAINTENGINE;}
	virtual void Step();	///<	速度→位置、加速度→速度の積分
	virtual void Clear();

protected:
	//PHSolidAuxs		solids;			/// 剛体の配列
	//PHSolidPairs	solidPairs;	
	PHConstraints	points;			///	接触点の配列
	PHConstraints	joints;			///	関節の配列
	int max_iter_dynamics;			/// Dynamics()の反復回数
	int max_iter_correction;		/// Correction()の反復回数
	//double step_size;				/// LCPのステップ幅
	//double converge_criteria;		/// 収束判定の閾値
	double max_error;

	void SetupDynamics(double dt);	/// LCPの準備
	void SetupCorrection(double dt);/// 誤差の修正
	void IterateDynamics();			/// Correction()での一度の反復
	void IterateCorrection();		/// Correction()での一度の反復
	void UpdateSolids(double dt);	/// 結果をSolidに反映する
	//void SetInitialValue();		/// LCPの決定変数の初期値を設定
	
	friend class PHShapePairForLCP;
	//friend struct PHConstraintEngineState;
};


}	//	namespace Spr
#endif
