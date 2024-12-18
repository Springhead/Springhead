/*
*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team
*  All rights reserved.
*  This software is free software. You can freely use, distribute and modify this
*  software. Please deal with this software under one of the following licenses:
*  This license itself, Boost Software License, The MIT License, The BSD License.
*/

/*
Convex物体に対しての接触力を計算するためのプログラミングとなります
*/

/*
made by KIM
*/

#ifndef PHCONTACTENGINE_H
#define PHCONTACTENGINE_H

#include <Physics/PHContactPoint.h>

typedef long long int ll;
typedef unsigned int ui;

namespace Spr {

	class PHShapePairForLCP;
	enum CONTACT { UNKNOWN = -1, POINT = 1, LINE, FACE };
	class PHContactEngine : public PHContactPoint {
	public:
		SPR_OBJECTDEF(PHContactEngine);

		struct ContactPartInfo {
			/// サポートタグによる点の候補
			std::vector<Vec3d> vectorGroup;
			/// サポートタグによる値（どのところが接触しているかを示す変数：0,1,2で区分される）
			CONTACT supportTag;
			ContactPartInfo() : vectorGroup(NULL), supportTag(CONTACT::UNKNOWN) {};
		};

		struct ObjectInfo {
			PHSolid* PHSptr;
			CDConvex* CDCptr;
			Posed posed;
		};


		struct NecessaryInfo {
			/// ZMP更新前
			Vec3d OldCoP = Vec3d();
			/// ZMP更新後
			Vec3d NewCoP = Vec3d();

			double  frictionMargin;   ///< 最大摩擦力と作用している摩擦力とのマージン
			double  copMargin;        ///< CoPから接触多角形境界までのマージン
									  /// ZMP位置算出用
			Vec3d ContPoint;
			Matrix3d ContLocal;
			Quaterniond w2x;
		};

		struct Info {
			NecessaryInfo necessaryInfo;
			ContactPartInfo contactpartInfo[2];
			ObjectInfo objectInfo[2];
		};

		Info phceInfo;

		// GJKのバリエーション
		bool			GJKIteration(int st_0, int st_1, const Vec3f& z_0, const Vec3f& z_1, Vec3f& tmp);
		// Euclid 距離計算
		void			EuclideanProjection(CONTACT st_0, CONTACT st_1, const Vec3f& z_0, Vec3f& tmp);
		// どのように接触しているかを判断するものであり、Integer型としてその接触の形を分類する
		CONTACT			HowToContact();
		// 点Ptがc0に属しているかどうかを判断し、Boolean値を返す
		inline bool		isPointInObject0(const Vec3d& pt);
		// 点Ptがc1に属しているかどうかを判断し、Boolean値を返す
		inline bool		isPointInObject1(const Vec3d& pt);
		// CoPがc0に属しているかどうかを判断し、Boolean値を返す
		inline bool		isCoPInObject0();
		// CoPがc1に属しているかどうかを判断し、Boolean値を返す
		inline bool		isCoPInObject1();
		// 2D上で点が面に属しているかどうかを判別する関数であり、中にあればtrue、なければfalseを返す
		bool			IsInside2D(const std::vector<Vec3d>& c_vec, CDConvex* c, PHSolid* s, const Vec3d& pt);

	public:

		/// コンストラクタ
		PHContactEngine();
		PHContactEngine(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1);

		// ----- PHConstraintの機能をオーバーライド
		virtual bool Iterate();

		// Projection and Update
		void			updateWithProjection(unsigned s, unsigned e, bool& updated);

		// ----- このクラスで実装する機能
		/// 接触状態(cts)を判断し、ある点（old_cop）から一番近い点（cp）を登録し、更新されたらtrueで返す
		bool	FindClosestPoint(Vec3f& cp, const Vec3d& old_cop, CONTACT cts, bool copOnObject0, bool copOnObject1);
		void	Projection(SpatialVector& fnew, const int i, bool& updat);
		bool	GJK2D(const std::vector<Vec3d>& c_vec, CDConvex* c, PHSolid* s, Vec3f& tmp, const Vec3d& old_cop);
		/// ある物体情報とその物体上の点から(cdc0, phs0, pt0)別の物体(cdc1, phs1)に対して一番近い点を算出しpt1にセーブする
		bool	GJK3D(CDConvex* cdc0, PHSolid* phs0, Vec3d& pt0, CDConvex* cdc1, PHSolid* phs1, Vec3d& pt1);
		// 任意の2次元のConvex間の共通空間に対してある点から一番近い点を探す
		void	DykstraProjection(Vec3f& v, const Vec3d& pt);
		virtual void CompBias();
			
	};

	// Preprocess for InOut2D
	Vec3d			calculateMid3D(const std::vector<Vec3d>& vec_Group);
	// 符号付き面積（>0ならばpqrは反時計回り）
	inline double	area2D(const Vec2d& p, const Vec2d& q, const Vec2d& r);
	// 2DのConvex hullとある2Dのある点を入力とし、その点がConvex hullの中に入っているかどうかを判断しboolean値を返す
	bool			isPointIn2DConvexHull(const std::vector<Vec3d>& vec_group, const Vec3d& pt);
	Vec2d			calculateMid2D(const std::vector<Vec3d>& vec_Group);
	// 3次元の線に一番近い点を探すものであり、出力として３次元の座標となる
	Vec3d			ClosestPtPointLine(const Vec3d& old_ZMP, const std::vector<Vec3d>& vecGroup, double& t);
	// 3次元の線に一番近い点を探すものであり、出力として３次元の座標となる
	Vec3d			ClosestPtPointLine(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2, double& t);
	// ３次元の三角形に対して一番近い点を見つけ出し、出力として３次元の座標とする
	Vec3d			ClosestPtPointTriangle(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2, const Vec3d& tmp_3);
	// 3次元の線に一番近い点を探し、出力として、一番近い点から入力としていれた点までのベクトルを返す
	Vec3d			NormalPt2ClosestPt(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2);
	// 2DのConvex hullと、あるベクトルを入れることでサポートポイントの点の座標を取り出し、出力はその座標の番号となる
	ll				PointSupport(const std::vector<Vec3d>& tmp_vec, const Vec3d& normal);
	// 3D上の二つの点に対して2D上の座標に変換してからその距離を返す
	inline double	Norm2D(const Posed& pose_1, const Vec3d& tmp_1, const Posed& pose_2, const Vec3d& tmp_2);
	// 3D上の二つの点間距離
	inline double	Norm3D(const Vec3d& pt1, const Vec3d& pt2);
	// ある物体の点の候補たちから一番近い点を見つけ出し、その点を返す関数
	Vec3d			ClosestPtFromGroup(const Vec3d& old_ZMP, const std::vector<Vec3d>& c_vec);

}

#endif