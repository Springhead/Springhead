#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Collision/CDDetectorImp.h>
#include <vector>

namespace Spr{;

/// 1つの接触点
struct PHContactPoint{
	int	contact;				/// 属する接触
	Vec3d pos;					/// 接触点の位置
	double depth;				/// 交差深度
	Matrix3d Jlin[2], Jang[2];	/// J行列のブロック		接触している剛体の速度から接触点での速度を与えるヤコビ行列
	Matrix3d Tlin[2], Tang[2];	/// T行列のブロック		接触力から剛体の速度変化を与える行列
	Matrix3d A;					/// A行列対角ブロック	この接触点の慣性行列
	Vec3d b;					/// bベクトルのブロック	接触力を0とした場合のdt後の接触点での相対速度
	double B;					/// Bベクトルの要素		接触力を考慮したdt後の交差深度
	Vec3d f;					/// 接触力(LCPの相補変数)
	Vec3d f0;					/// 反復での初期値(もしあれば前回の解)
	Vec3d df;					/// 各反復での接触力の変化量(収束判定に使用)

	PHContactPoint(int c, Vec3d p){
		contact = c;
		pos = p;
	}
};
typedef std::vector<PHContactPoint> PHContactPoints;

/// Shape同士の接触
class PHContact{
public:
	int solid[2];				/// 接触している剛体
	int shape[2];				/// 接触している形状
	Vec3d normal;				/// 法線
	Vec3d center;				/// 交差形状の重心
	double depth;
	double mu;					/// 摩擦係数
	
	PHContact(){}
	/*PHContact(int solid0, int solid1, int shape0, int shape1, Vec3d n, Vec3d c, double _mu){
		solid[0] = solid0;
		solid[1] = solid1;
		shape[0] = shape0;
		shape[1] = shape1;
		normal = n;
		center = c;
		mu = _mu;
	}*/
};

/// 全ての接触を保持するコンテナ
class PHContacts : public std::vector<PHContact>{
public:
};

class PHConstraintEngine: public PHEngine{
	friend class PHSolidPair;

	OBJECTDEF(PHConstraintEngine);

	/// 接触に関与する剛体の情報
	struct PHSolidAux{
		double		minv;				/// 各剛体の質量の逆数
		Matrix3d	Iinv;				/// 各剛体の慣性行列の逆行列
		Vec3d		Vlin0, Vang0;		/// 接触力が0の場合のdt後の速度
		Vec3d		dVlin, dVang;		/// 接触力を考慮した速度変化量(LCPを解いて求める)
	};
	typedef std::vector<PHSolidAux> PHSolidAuxs;

	/// Solidの組み合わせの配列
	class PHSolidPair{
	public:
		/// Shapeの組み合わせの配列
		typedef UTCombination<CDShapePair> CDShapePairs;
		CDShapePairs	shapePairs;

		void Init(PHSolid* s0, PHSolid* s1);	/// 初期化
		bool Detect(int s0, int s1, PHConstraintEngine* engine);	
		
	};
	typedef UTCombination<PHSolidPair> PHSolidPairs;
	
protected:
	bool			ready;		/// 
	PHSolids		solids;		/// 拘束力計算の対象となる剛体
	PHSolidAuxs		solidAuxs;	/// 剛体の付加情報
	PHSolidPairs	solidPairs;
	PHContacts		contacts;	/// 剛体同士の接触の配列
	PHContactPoints	points;		///	接触点の配列
	int max_iter_dynamics;		/// Dynamics()の反復回数
	int max_iter_correction;	/// Correction()の反復回数

	bool Detect();				/// 全体の交差の検知
	void PrintContacts();
	void Setup(double dt);	/// LCPの準備
	//void SetInitialValue();		/// LCPの決定変数の初期値を設定
	bool CheckConvergence();	/// 反復法における収束判定
	void Dynamics(double dt);			/// 速度と位置の更新
	void Correction();			/// 誤差の修正
	void IterateDynamics();	/// Dynamics()での一度の反復		
	void IterateCorrection();	/// Correction()での一度の反復
	void UpdateSolids(double dt);		/// 結果をSolidに反映する

public:
	void Add(PHSolid* s);		/// Solid を登録する
	void Remove(PHSolid* s);	/// 登録されているSolidを削除する
	void Invalidate(){ready = false;}	/// readyフラグをリセット
	void Init();				/// 初期化し，readyフラグをセット
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
