#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <SprPhysics.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>

namespace Spr{;

/// 1つの接触点
struct PHContactPoint{
	int	contact;				/// 属する接触
	Vec3d pos;					/// 接触点の位置
	Matrix3d Jlin[2], Jang[2];	/// J行列のブロック
	Matrix3d Tlin[2], Tang[2];	/// T行列のブロック
	Vec3d b;					/// bベクトルのブロック
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
	double mu;					/// 摩擦係数
	
	PHContact(int solid0, int solid1, int shape0, int shape1, Vec3d n, Vec3d c, double _mu){
		solid[0] = solid0;
		solid[1] = solid1;
		shape[0] = shape0;
		shape[1] = shape1;
		normal = n;
		center = c;
		mu = _mu;
	}
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
		Vec3d		dVlin_nc, dVang_nc;	/// 接触力が0の場合の速度変化量
		Vec3d		dVlin, dVang;		/// 接触力を考慮した速度変化量(LCPを解いて求める)
	};
	typedef std::vector<PHSolidAux> PHSolidAuxArray;

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

	/// LCPのA行列
	class PHLCPMatrix : public UTCombination<Matrix3d>{
	public:
		typedef UTCombination<Matrix3d> base_type;
		Matrix3d& item(int i, int j){return (*this)[i * width() + j];}
	};
	
protected:
	bool			bReady;		/// 
	PHSolids		solids;		/// 拘束力計算の対象となる剛体
	PHSolidAuxArray	solidAuxs;	/// 剛体の付加情報
	PHSolidPairs	solidPairs;
	PHContacts		contacts;	/// 剛体同士の接触の配列
	PHContactPoints	points;		///	接触点の配列
	PHLCPMatrix		A;			/// LCPのA行列

	void Detect();	/// 全体の交差の検知
								/// Solid組ごとの交差検知
	void SetupLCP();			/// LCPの準備
	void SetInitialValue();		/// LCPの決定変数の初期値を設定
	bool CheckConvergence();	/// 反復法における収束判定
	void UpdateLCP();			/// 反復法における一度の更新

public:
	void Add(PHSolid* s);
	void Remove(PHSolid* s);
	void Init();				/// 初期化
	///
	int GetPriority() const {return 0/*SGBP_CONSTRAINTENGINE*/;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s){ solids.clear(); }

	PHConstraintEngine();
	~PHConstraintEngine();
};

}	//	namespace Spr
#endif
