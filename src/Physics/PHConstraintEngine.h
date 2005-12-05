#ifndef PHCONSTRAINTENGINE_H
#define PHCONSTRAINTENGINE_H

#include <Springhead.h>
#include <Foundation/Object.h>
#include <Physics/PHScene.h>
#include <Physics/PHEngine.h>
#include <Base/Combination.h>
#include <Collision/CDDetectorImp.h>

namespace Spr{;

class PHConstraintEngine: public PHEngine{
	OBJECTDEF(PHConstraintEngine);

	/// 接触に関与する剛体の情報
	struct PHSolidAux{
		double		minv;				/// 各剛体の質量の逆数
		Matrix3d	Iinv;				/// 各剛体の慣性行列の逆行列
		Vec3d		dVlin_nc, dVang_nc;
	};
	typedef std::vector<PHSolidAux> PHSolidAuxArray;

	/// 1つの接触点
	struct PHContactPoint{
		Vec3d pos;					/// 接触点の位置
		Matrix3d Jlin[2], Jang[2];	/// J行列のブロック
		Matrix3d Tlin[2], Tang[2];	/// T行列のブロック
		Vec3d b;					/// bベクトルのブロック

	};
	typedef std::vector<PHContactPoint> PHContactPoints;

	/// Solid同士の接触
	struct PHContact{
		bool	bValid;				/// true => 有効, false => 無効
		bool	bNew;				/// true => 新規, false => 継続
		int solids[2];				/// 接触している剛体
		int shapes[2];				/// 接触している形状
		CDShape* intersection;		/// 交差形状
		Vec3d normal;				/// 法線
		Vec3d center;				/// 交差形状の重心
		PHContactPoints	points;		/// normalに直交する平面へ射影した接触多面体の頂点配列
	};

	/// 全ての接触を保持するコンテナ
	/** ・新しく接触が生じた場合，先頭からスキャンしてbValid == falseの要素に
		　新たな接触が上書きされる．このときbNew = trueとなる
		・同じ接触が2ステップ以上継続した場合bNew = falseとなる
		・接触が解消された場合，該当する要素はbValid == falseとなり無効化される

		＊＊上の方式は予定です。やめるかも。＊＊
	  */
	class PHContacts : public std::vector<PHContact>{
	public:
		int	ncon;	/// 接触点の総数
	};

	/// LCPを与える行列
	class PHLCPMatrix : public std::vector<Matrix3d>{
		int ncon;	/// 接触の数
	public:
		typedef std::vector<Matrix3d> base_type;
		void resize(size_t K){
			base_type::resize(K * K);
			ncon = K;
		}
		Matrix3d& item(int i, int j){return (*this)[i * ncon + j];}
	};
	
protected:
	PHSolids		solids;		/// 拘束力計算の対象となる剛体
	PHContacts		contacts;	/// 剛体同士の接触情報
	PHSolidAuxArray	solidauxs;
	PHLCPMatrix		A;

	void Init();	/// 初期化

public:
	void AddSolid(PHSolid* s);
	void RemoveSolid(PHSolid* s);
	///
	int GetPriority() const {return 0/*SGBP_CONSTRAINTENGINE*/;}
	///	速度→位置、加速度→速度の積分
	virtual void Step();
	virtual void Clear(PHScene* s){ solids.clear(); }
};

}	//	namespace Spr
#endif
