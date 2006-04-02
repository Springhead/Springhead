#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <SprPhysics.h>
//#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Collision/CDDetectorImp.h>
//#include <vector>
#include "PHConstraint.h"

namespace Spr{;

class PHSolid;
class PHJoint;
class PHConstraintEngine;

///	Shapeの組合わせ
class PHShapePair: public CDShapePair{
public:
	///	
	PHShapePair(){}
	PHShapePair(CDShape* s0, CDShape* s1):CDShapePair(s0, s1){}
	///	接触判定．接触の中心(center 最侵入点の中点)と法線ベクトルまで求める
	bool Detect(unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1);
	///	接触解析．接触部分の切り口を求めて，切り口を構成する凸多角形の頂点をengineに拘束として追加する．
	void EnumVertex(PHConstraintEngine* engine, unsigned ct, PHSolidAux* solid0, PHSolidAux* solid1);
};
class PHConstraintEngine: public PHEngine{
	friend class PHSolidPair;
	friend class PHSolid;

	OBJECTDEF(PHConstraintEngine);

	/// Solidの組み合わせの配列
	class PHSolidPair{
	public:
		bool bEnabled;
		UTRef<PHSolidAux> solid[2];
		/// Shapeの組み合わせの配列
		typedef UTCombination<PHShapePair> PHShapePairs;
		PHShapePairs	shapePairs;

		void Init(PHSolidAux* s0, PHSolidAux* s1);	/// 初期化
		bool Detect(PHConstraintEngine* engine);	

	};
	typedef UTCombination<PHSolidPair> PHSolidPairs;
	
protected:
	PHSolidAuxs		solids;			/// 剛体の配列
	PHSolidPairs	solidPairs;	
	PHConstraints	points;			///	接触点の配列
	PHConstraints	joints;			///	関節の配列
	int max_iter_dynamics;			/// Dynamics()の反復回数
	int max_iter_correction;		/// Correction()の反復回数
	//double step_size;				/// LCPのステップ幅
	//double converge_criteria;		/// 収束判定の閾値
	double max_error;

	bool DetectPenetration();		/// 全体の交差の検知
	void PrintContacts();
	void SetupDynamics(double dt);	/// LCPの準備
	void SetupCorrection(double dt);/// 誤差の修正
	void IterateDynamics();			/// Correction()での一度の反復
	void IterateCorrection();		/// Correction()での一度の反復
	void UpdateSolids(double dt);	/// 結果をSolidに反映する
	//void SetInitialValue();		/// LCPの決定変数の初期値を設定
	
	/// SolidにShapeが追加されたときにSolidから呼ばれる
	void UpdateShapePairs(PHSolid* solid); 
	
public:
	void Add(PHSolid* s);			/// Solid を登録する
	void Remove(PHSolid* s);		/// 登録されているSolidを削除する
	PHJoint* AddJoint(PHSolid* lhs, PHSolid* rhs, const PHJointDesc& desc);	/// 関節の追加する
	void EnableContact(PHSolid* lhs, PHSolid* rhs, bool bEnable);
	///
	int GetPriority() const {return SGBP_CONSTRAINTENGINE;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s);

	PHConstraintEngine();
	~PHConstraintEngine();

	friend class PHShapePair;
};


}	//	namespace Spr
#endif
