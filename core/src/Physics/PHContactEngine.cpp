/*
made by KIM
*/


#include <Physics/PHContactEngine.h>
#include <Physics/PHConstraintEngine.h>
#include <algorithm>
#include <math.h>

using namespace PTM;
using namespace std;

namespace Spr {
	;

	const double EPSILON_2 = 1.0e-2;
	const double EPSILON_4 = EPSILON_2 * EPSILON_2;
	const double EPSILON_8 = 1.0e-8;
	const double EPSILON_10 = 1.0e-10;
	const double EPSILON_16 = EPSILON_8 * EPSILON_8;
	const double EPSILON_20 = EPSILON_10 * EPSILON_10;

	// 内積を計算するための関数
	inline double Dot6(const double* v1, const double* v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3] + v1[4] * v2[4] + v1[5] * v2[5];
	}

	// tmp_1,tmp_2から構成される線分にold_ZMPから射影したときに一番近い点を見つけ出し、その点を返す
	Vec3d ClosestPtPointLine(const Vec3d& old_ZMP, const std::vector<Vec3d>& vecGroup, double& t)
	{
		assert(!vecGroup.empty());

		Vec3d tmp_1 = vecGroup[0];
		Vec3d tmp_2 = vecGroup[1];
		// 線分のベクトル
		Vec3d tmp_12 = tmp_2 - tmp_1;
		t = (old_ZMP - tmp_1).dot(tmp_12);

		// 線分の外にある(tmp_12の線分から垂直でtmp_1を通る直線を引いたときにその線より下にある)からこれは 
		if (t <= 0.0) {
			t = 0.0;
			return tmp_1;
		}
		else {
			double nom = tmp_12.dot(tmp_12);
			if (t >= nom) {
				t = 1.0;
				return tmp_2;
			}
			else {
				t = t / nom;
				return tmp_1 + t * tmp_12;
			}
		}
	}

	Vec3d ClosestPtPointLine(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2, double& t)
	{

		// 線分のベクトル
		Vec3d tmp_12 = tmp_2 - tmp_1;
		t = (old_ZMP - tmp_1).dot(tmp_12);

		// 線分の外にある(tmp_12の線分から垂直でtmp_1を通る直線を引いたときにその線より下にある)からこれは 
		if (t <= 0.0) {
			t = 0.0;
			return tmp_1;
		}
		else {
			double nom = tmp_12.dot(tmp_12);
			if (t >= nom) {
				t = 1.0;
				return tmp_2;
			}
			else {
				t = t / nom;
				return tmp_1 + t * tmp_12;
			}
		}
	}


	// Collision Detectionを参考にしました
	// p141～p142
	// 点三つ(tmp_1, tmp_2, tmp_3)に対してある点(old_ZMP)から一番近い点を見つけ出し、その点を返す
	Vec3d ClosestPtPointTriangle(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2, const Vec3d& tmp_3) {

		Vec3d tmp2_tmp1 = tmp_2 - tmp_1; // ab
		Vec3d tmp3_tmp1 = tmp_3 - tmp_1; // ac
		Vec3d zmp_tmp1 = old_ZMP - tmp_1; // ap

		float d1 = tmp2_tmp1 * zmp_tmp1;
		float d2 = tmp3_tmp1 * zmp_tmp1;
		// Voronoi領域により一番近い点となる
		if (d1 <= 0.0f && d2 <= 0.0f) return tmp_1;

		Vec3d tmp2_zmp = old_ZMP - tmp_2; // bp
		float d3 = tmp2_tmp1 * tmp2_zmp;
		float d4 = tmp3_tmp1 * tmp2_zmp;
		if (d3 >= 0.0f && d4 <= d3) return tmp_2;

		float vc = d1 * d4 - d3 * d2;
		if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
			float v = d1 / (d1 - d3);
			return tmp_1 + v * tmp2_tmp1;
		}

		Vec3d tmp3_zmp = old_ZMP - tmp_3;
		float d5 = tmp2_tmp1 * tmp3_zmp;
		float d6 = tmp3_tmp1 * tmp3_zmp;
		if (d6 >= 0.0f && d5 <= d6) return tmp_3;

		float vb = d5 * d2 - d1 * d6;
		if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
			float w = d2 / (d2 - d6);
			return tmp_1 + w * tmp3_tmp1;
		}

		float va = d3 * d6 - d5 * d4;
		if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
			float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
			return tmp_2 + w * (tmp_3 - tmp_2);
		}

		float denom = 1.0f / (va + vb + vc);
		float v = vb * denom;
		float w = vc * denom;
		return tmp_1 + tmp2_tmp1 * v + tmp3_tmp1 * w;

	}

	// 二点(tmp_1, tmp_2)に対してある点(old_ZMP)から一番近い点を見つけ出し、その点からold_ZMPまでのベクトルを返す
	Vec3d NormalPt2ClosestPt(const Vec3d& old_ZMP, const Vec3d& tmp_1, const Vec3d& tmp_2)
	{
		Vec3d tmp_12 = tmp_2 - tmp_1;
		double t = (old_ZMP - tmp_1).dot(tmp_12);

		if (t <= 0.0) {
			t = 0.0;
			return old_ZMP - tmp_1;
		}
		else {
			double nom = tmp_12.dot(tmp_12);
			if (t >= nom) {
				t = 1.0;
				return old_ZMP - tmp_2;
			}
			else {
				t = t / nom;
				return old_ZMP - (tmp_1 + t * tmp_12);
			}
		}
	}

#define YZ()	sub_vector( PTM::TSubVectorDim<1,2>() )

	// 2DConvex hullを作るために、3DのVerticesを入れる
	// 入れたVerticesはx方向に対して切って、2DのConvex hullを作る
	// ベクトル(normal)を基準に、2DConvex hullからサポートポイントに一番近い点を取り出し、その座標を返す。
	ll PointSupport(const std::vector<Vec3d>& vec_group, const Vec3d& normal) {
		ll n = vec_group.size();
		double d1 = 0.0;
		ll curPos = 0;
		Vec2d normal2D = normal.YZ();
		double maxValue = normal2D * vec_group[0].YZ();

		for (ll i = 1; i < n; i++) {
			d1 = normal.YZ() * vec_group[i].YZ();
			if (d1 > maxValue) {
				maxValue = d1;
				curPos = i;
			}
		}

		return curPos;
	}

	// 3D上の二点(tmp_1, tmp_2)に対してposeを掛けて(各々の物体から位置を計算して)2D上座標に変換する（YZ平面を作る）
	// へっかんさせてに転換の距離を返す(YZ平面上での距離)
	inline double Norm2D(const Posed& pose_1, const Vec3d& tmp_1, const Posed& pose_2, const Vec3d& tmp_2)
	{
		return ((pose_1 * tmp_1).YZ() - (pose_2 * tmp_2).YZ()).norm();
	}

	inline double Norm3D(const Vec3d& pt1, const Vec3d& pt2) {
		return (pt1 - pt2).norm();
	}

	// Preprocess for InOut2D
	Vec3d calculateMid3D(const std::vector<Vec3d>& vec_Group) {
		Vec3d result = Vec3d(0, 0, 0);

		// サイズが０ならば脱出
		assert(!vec_Group.empty());

		for (auto& iter : vec_Group) {
			result += iter;
		}

		return (result / vec_Group.size());
	}

	// Preprocess for InOut2D
	// 前処理されている（軸揃い）
	Vec2d calculateMid2D(const std::vector<Vec3d>& vec_Group) {
		Vec2d result;
		double X = 0;
		double Y = 0;

		// サイズが０ならば脱出
		assert(!vec_Group.empty());

		for (auto& iter : vec_Group) {
			X += (double)iter.Y();
			Y += (double)iter.Z();
		}

		X /= (double)vec_Group.size();
		Y /= (double)vec_Group.size();

		result.x = X;
		result.y = Y;

		return result;
	}

	inline double area2D(const Vec2d& p, const Vec2d& q, const Vec2d& r) {
		return (p.X() - r.X()) * (q.Y() - r.Y()) - (q.X() - r.X()) * (p.Y() - r.Y());
	}

	// 3Dの座標群に対して、x方向に対して切って2DのConvex Hullを作る。
	// 2DConvex Hullの中にある点(pt)があるかどうか内包判定する
	bool isPointIn2DConvexHull(const std::vector<Vec3d>& vec_group, const Vec3d& pt) {

		Vec2d MID = calculateMid2D(vec_group);

		size_t n = vec_group.size();
		int l = 0, r = n - 1, m;

		while (r - l > 1) {
			m = (r + l) / 2;
			if (area2D(vec_group[l].YZ(), MID, vec_group[m].YZ()) <= 0.0) {
				if (area2D(vec_group[l].YZ(), MID, pt.YZ()) <= 0.0 &&
					area2D(pt.YZ(), MID, vec_group[m].YZ()) <= 0.0) r = m;
				else l = m;
			}
			else
			{
				if (area2D(vec_group[l].YZ(), MID, pt.YZ()) >= 0.0 &&
					area2D(pt.YZ(), MID, vec_group[m].YZ()) >= 0.0) l = m;
				else r = m;
			}
		}

		return (area2D(vec_group[l].YZ(), vec_group[r].YZ(), pt.YZ()) >= 0.0) ? true : false;
	}

	// 点群（3DConvexHullから生成された点群であるvec_group）からある点（old_ZMP）に一番近い点（pp）を見つけ出し、その点を返す関数
	Vec3d ClosestPtFromGroup(const Vec3d& old_ZMP, const std::vector<Vec3d>& vec_group) {
		size_t size = vec_group.size();
		double d1 = 0.0, d2 = INFINITY;
		double t = 0.0;
		Vec3d pp;
		for (ui i = 0; i < size; i++) {
			Vec3d tmp = ClosestPtPointLine(old_ZMP, vec_group[i % size], vec_group[(i + 1) % size], t);
			d1 = (old_ZMP - tmp).norm();
			if (d2 > d1) {
				d2 = d1;
				pp = tmp;
			}
		}
		return pp;
	}

	// -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  -----  ----- 
	// PHContactEngine

	// Contactpointに調整が必要となる場合がある
	PHContactEngine::PHContactEngine() {
	}
	PHContactEngine::PHContactEngine(const Matrix3d& local, PHShapePairForLCP* sp, Vec3d p, PHSolid* s0, PHSolid* s1) :
		PHContactPoint(local, sp, p, s0, s1)
		{

		phceInfo.necessaryInfo.ContPoint = p;
		phceInfo.necessaryInfo.ContLocal = local;

		// 軸設定
		Vec3d u = local.Ex();	//	u: 物体ではなく原点の速度の向きなので - がつく．
		if (u.X() < -1 + EPSILON_16) {
			phceInfo.necessaryInfo.w2x = Quaterniond::Rot(Rad(180), 'z');
		}
		else if (u.X() < 1 - EPSILON_16) {
			Matrix3d matW2x = Matrix3d::Rot(u, Vec3f(1, 0, 0), 'x');
			phceInfo.necessaryInfo.w2x.FromMatrix(matW2x);
			phceInfo.necessaryInfo.w2x = phceInfo.necessaryInfo.w2x.Inv();
		}
		
		phceInfo.objectInfo[0].PHSptr = s0;
		phceInfo.objectInfo[0].CDCptr = DCAST(CDConvex, phceInfo.objectInfo[0].PHSptr->GetShape(0));
		phceInfo.objectInfo[0].posed.Ori() = phceInfo.necessaryInfo.w2x * phceInfo.objectInfo[0].PHSptr->GetOrientation();
		phceInfo.objectInfo[0].posed.Pos() = phceInfo.necessaryInfo.w2x * phceInfo.objectInfo[0].PHSptr->GetCenterPosition();

		phceInfo.objectInfo[1].PHSptr = s1;
		phceInfo.objectInfo[1].CDCptr = DCAST(CDConvex, phceInfo.objectInfo[1].PHSptr->GetShape(0));
		phceInfo.objectInfo[1].posed.Ori() = phceInfo.necessaryInfo.w2x * phceInfo.objectInfo[1].PHSptr->GetOrientation();
		phceInfo.objectInfo[1].posed.Pos() = phceInfo.necessaryInfo.w2x * phceInfo.objectInfo[1].PHSptr->GetCenterPosition();

		Vec3d rjabs[2];
		for (int i = 0; i < 2; i++) {
			rjabs[i] = phceInfo.necessaryInfo.ContPoint - solid[i]->GetCenterPosition();	//剛体の中心から接触点までのベクトル
		}

		// local: 接触点の関節フレーム は，x軸を法線, y,z軸を接線とする
		Quaterniond qlocal;
		qlocal.FromMatrix(local);

		for (int i = 0; i < 2; i++) {
			(i == 0 ? poseSocket : posePlug).Ori() = Xj[i].q = solid[i]->GetOrientation().Conjugated() * qlocal;
			(i == 0 ? poseSocket : posePlug).Pos() = Xj[i].r = solid[i]->GetOrientation().Conjugated() * rjabs[i];
		}

		// 6自由度拘束必要
		movableAxes.Clear();

	}

	void PHContactEngine::CompBias() {
		PHSceneIf* scene = GetScene();
		double dt = scene->GetTimeStep();
		double dtinv = scene->GetTimeStepInv();
		double tol = scene->GetContactTolerance();
		double vth = scene->GetImpactThreshold();
		double fth = scene->GetFrictionThreshold();

		// 位置（深さ）補正の計算, Positional correction for penetration depth
		double diff = std::max(shapePair->depth - tol, 0.0); // 補正が必要な貫通深さ, Penetration depth to correct
		if (vjrel[0] > -vth) { // 相対速度が閾値以下の場合

			if (spring == 0.0 && damper == 0.0) {
				// 粘弾性がない場合の補正
				db[0] = -engine->contactCorrectionRate * diff * dtinv;
			}
			else {
				// 粘弾性がある場合の補正
				double tmp = (shapePair->depth > tol) ? (1.0 / (damper + spring * dt)) : (1.0 / damper);
				dA[0] = tmp * dtinv;
				db[0] = -tmp * spring * diff;
			}
		}
		else {
			// 相対速度が閾値を超える場合、補正なし, If the relative velocity is above threshold, no correction
			db[0] = e * vjrel[0];
		}

		// 接線方向の補正, Tangential corrections
		db[1] = velField[1];
		db[2] = velField[2];

		// Rotational corrections for 6DoF (axes 4 and 5)
		Vec3d n0 = poseSocket.Ori() * Vec3d(1.0, 0.0, 0.0); // Normal vector for socket
		Vec3d n1 = posePlug.Ori() * Vec3d(1.0, 0.0, 0.0);   // Normal vector for plug

		// 外積を使用して角度の誤差を計算, Calculate angular error using cross product
		Vec3d angularError = n0 % n1; // Cross product gives the rotation axis and magnitude
		db[4] = angularError[1] * engine->contactCorrectionRate * dtinv; // Rotation around Y-axis
		db[5] = angularError[2] * engine->contactCorrectionRate * dtinv; // Rotation around Z-axis

		// Adjust static/dynamic friction state based on tangential relative velocity
		double vt = vjrel[1] + velField[1];
		isStatic = (-fth < vt&& vt < fth);
		DSTR << "CEngine::db:" << db << std::endl;
	}	

	inline void PHContactEngine::updateWithProjection(unsigned s, unsigned e, bool& updated) {
		
		for (unsigned i = s; i <= e; ++i) {
			dv[i] = Dot6((const double*)J[0].row(i), (const double*)solid[0]->dv)
				+ Dot6((const double*)J[1].row(i), (const double*)solid[1]->dv);
			res[i] = b[i] + dA[i] * f[i] + dv[i];
			fnew[i] = f[i] - engine->accelSOR * Ainv[i] * res[i];
		}

		bool temp = false;
		Projection(fnew, s, temp);

		for (unsigned i = s; i <= e; ++i) {
			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
				CompResponse(df[i], i);
			}
		}

		updated |= temp;
	}

	// hit count 10 X 9 O
	bool PHContactEngine::Iterate() {

		bool updated = false;
		/*
		for (int i = 0; i < 6; i++) {

			// Gauss-Seidel Update
			dv[i] = Dot6((const double*)J[0].row(i), (const double*)solid[0]->dv)
				+ Dot6((const double*)J[1].row(i), (const double*)solid[1]->dv);
			res[i] = b[i] + dA[i] * f[i] + dv[i];
			fnew[i] = f[i] - Ainv[i] * res[i];

			
			// Comp Response & Update f
			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
			CompResponse(df[i], i);

			}
			
		}

		// 法線での力
		Projection(fnew, 0, updated);
		// 接線でのトルク
		Projection(fnew, 4, updated);
		Projection(fnew, 5, updated);
		// 法線での力
		Projection(fnew, 1, updated);
		Projection(fnew, 2, updated);
		// 接線でのトルク
		Projection(fnew, 3, updated);

		return updated;
		*/

		updateWithProjection(0, 0, updated);
		updateWithProjection(1, 2, updated);
		updateWithProjection(4, 5, updated);
		updateWithProjection(3, 3, updated);

		return updated;
	}



	// hemi 80
	void PHContactEngine::Projection(SpatialVector & fnew, const int i, bool& updat)
	{
		switch (i) {

		case 0: // 垂直方向（接触法線方向）
		{
			if (shapePair->depth < GetScene()->GetContactTolerance() && vjrel[0] * GetScene()->GetTimeStep() < GetScene()->GetContactTolerance()) {
				fnew[0] = 0.0;
			}
			if (fnew[0] < EPSILON_10) {
				fnew[0] = 0.0;
			}
			return;
		}
		/*
			// 接触断面内での最大横速度
			double vmax = sqrt(vjrel[1]*vjrel[1] + vjrel[2]*vjrel[2]) + contactRadius*std::abs(vjrel[3]);
			double vth  = GetScene()->GetFrictionThreshold();
	
			double ftmax = (vmax < vth ? mu0 : mu) * fnew[0];
			double ft    = sqrt(fnew[1]*fnew[1] + fnew[2]*fnew[2]);

			frictionMargin = ftmax - ft;

			if(frictionMargin < 0.0){
			double k = ftmax / ft;
			fnew[1] *= k;
			fnew[2] *= k;
			frictionMargin = 0.0;
			}
		*/
		case 1: // 接触面方向（normalと垂直)
		{

			Vec3d vrelA = phceInfo.necessaryInfo.ContLocal.inv() * (phceInfo.objectInfo[0].PHSptr->GetAngularVelocity() ^ (phceInfo.necessaryInfo.ContPoint - phceInfo.objectInfo[0].PHSptr->GetCenterPosition()));
					- phceInfo.necessaryInfo.ContLocal.inv() * phceInfo.objectInfo[1].PHSptr->GetAngularVelocity() ^ (phceInfo.necessaryInfo.ContPoint - phceInfo.objectInfo[1].PHSptr->GetCenterPosition());
			double vmax2cont = sqrt(vjrel[1] * vjrel[1] + vjrel[2] * vjrel[2]) + sqrt(vrelA[1]*vrelA[1] + vrelA[2]*vrelA[2]);
			double vmax2ideal = GetScene()->GetFrictionThreshold();

			double ftmax = (vmax2cont < vmax2ideal ? mu0 : mu) * fnew[0];
			double ft = sqrt(fnew[1] * fnew[1] + fnew[2] * fnew[2]);

			phceInfo.necessaryInfo.frictionMargin = ftmax - ft;

			if (phceInfo.necessaryInfo.frictionMargin < 0.0) {
				double k = ftmax / ft;
				fnew[1] *= k;
				fnew[2] *= k;
				phceInfo.necessaryInfo.frictionMargin = 0.0;
			}
			return;
			/*
			if (vmax2cont < vmax2ideal) {
				if (ft > ftmax) {
					ft = ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
				else if (ft < -ftmax) {
					ft = -ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
			}
			else {
				if (ft > ftmax) {
					ft = ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
				else if (ft < -ftmax) {
					ft = -ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
			}
			*/
		}
		case 2: // 接触面方向（normalと垂直)
		{
			double vmax2cont = sqrt(vjrel[1] * vjrel[1] + vjrel[2] * vjrel[2]);
			double vmax2ideal = GetScene()->GetFrictionThreshold();

			double ftmax = (vmax2cont < vmax2ideal ? mu0 : mu) * fnew[0];
			double ft = sqrt(fnew[1] * fnew[1] + fnew[2] * fnew[2]);

			phceInfo.necessaryInfo.frictionMargin = ftmax - ft;

			if (vmax2cont < vmax2ideal) {
				if (ft > ftmax) {
					ft = ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
				else if (ft < -ftmax) {
					ft = -ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
			}
			else {
				if (ft > ftmax) {
					ft = ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
				else if (ft < -ftmax) {
					ft = -ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
			}
			return;
		}
		case 3:	//接触法線と垂直な方向のトルク
		{
			if (fnew[0] < EPSILON_10) {
				fnew[3] = 0.0;
				return;
			}

			// 接触法線垂直向きのfnew1, fnew2から生じるモーメント
			// 接触法線垂直向きから生まれる力静止摩擦力のように扱う
			// 拘束している力であると考えている

			double ncop = phceInfo.necessaryInfo.NewCoP.y*fnew[2] - phceInfo.necessaryInfo.NewCoP.z*fnew[1];

			// ここの2/3の意味とあとMarginらが必要な理由というのがまだ理解できない
			double margin = (2.0 / 3.0)*phceInfo.necessaryInfo.copMargin*phceInfo.necessaryInfo.frictionMargin;
			//double margin = (2.0/3.0)*contactRadius*frictionMargin;

			if (fnew[3] - ncop > margin) fnew[3] = ncop + margin;
			if (fnew[3] - ncop < -margin) fnew[3] = ncop - margin;

			return;
		}
		case 4:
		{

			phceInfo.necessaryInfo.copMargin = EPSILON_10;

			if (fnew[0] < EPSILON_10) {
				fnew[4] = fnew[5] = 0;
				return;
			}

			Vec3d CopFromContPoint = Vec3d(0.0, -fnew[5] / fnew[0], fnew[4] / fnew[0]);

			CONTACT htc = HowToContact();

			if (htc == CONTACT::POINT) {

				phceInfo.necessaryInfo.NewCoP = Vec3d();
				phceInfo.necessaryInfo.copMargin = 0.0;

				fnew[4] = phceInfo.necessaryInfo.NewCoP.Z() * fnew[0];
				fnew[5] = -phceInfo.necessaryInfo.NewCoP.Y() * fnew[0];

			}
			else {

				phceInfo.necessaryInfo.OldCoP = phceInfo.necessaryInfo.ContPoint + phceInfo.necessaryInfo.ContLocal * CopFromContPoint;

				// 点が中にあれば処理が必要ない
				bool isCopOnObject0 = isCoPInObject0(), isCopOnObject1 = isCoPInObject1();

				if (isCopOnObject0 && isCopOnObject1) {

					phceInfo.necessaryInfo.copMargin = 0.0;

				}

				else {

					Vec3f w = Vec3f();

					FindClosestPoint(w, phceInfo.necessaryInfo.OldCoP, htc, isCopOnObject0, isCopOnObject1);

					phceInfo.necessaryInfo.NewCoP = phceInfo.necessaryInfo.ContLocal.inv() * (w - phceInfo.necessaryInfo.ContPoint);
					phceInfo.necessaryInfo.copMargin = 0.0;

					fnew[4] = phceInfo.necessaryInfo.NewCoP.Z() * fnew[0];
					fnew[5] = -phceInfo.necessaryInfo.NewCoP.Y() * fnew[0];

				}
			}

			if (phceInfo.necessaryInfo.copMargin != 0.0) {
				Vec3d c = phceInfo.necessaryInfo.OldCoP - phceInfo.necessaryInfo.ContPoint;
				phceInfo.necessaryInfo.copMargin = std::min(phceInfo.necessaryInfo.copMargin, -c * phceInfo.necessaryInfo.ContLocal.Ex());
			}
			return;
		}
		case 5://接触法線垂直方向に作用するトルク
		{

			phceInfo.necessaryInfo.copMargin = EPSILON_10;

			if (fnew[0] < EPSILON_10) {

				fnew[4] = 0, fnew[5] = 0;
				return;
			}

			Vec3d CopFromContPoint = Vec3d(0.0, -fnew[5] / fnew[0], fnew[4] / fnew[0]);
			DSTR << "CopFromContPoint:" << CopFromContPoint << std::endl;

			CONTACT htc = HowToContact();

			if (htc == CONTACT::POINT) {

				phceInfo.necessaryInfo.NewCoP = Vec3d();
				phceInfo.necessaryInfo.copMargin = 0.0;

				fnew[4] = phceInfo.necessaryInfo.NewCoP.Z() * fnew[0];
				fnew[5] = -phceInfo.necessaryInfo.NewCoP.Y() * fnew[0];

			}
			else {

				phceInfo.necessaryInfo.OldCoP = phceInfo.necessaryInfo.ContPoint + phceInfo.necessaryInfo.ContLocal * CopFromContPoint;

				// 点が中にあれば処理が必要ない
				bool isCopOnObject0 = isCoPInObject0(), isCopOnObject1 = isCoPInObject1();

				if (isCopOnObject0 && isCopOnObject1) {

					phceInfo.necessaryInfo.copMargin = 0.0;

				}

				else {

					Vec3f w = Vec3f();

					FindClosestPoint(w, phceInfo.necessaryInfo.OldCoP, htc, isCopOnObject0, isCopOnObject1);

					phceInfo.necessaryInfo.NewCoP = phceInfo.necessaryInfo.ContLocal * (w - phceInfo.necessaryInfo.ContPoint);
					phceInfo.necessaryInfo.copMargin = 0.0;

					fnew[4] = phceInfo.necessaryInfo.NewCoP.Z() * fnew[0];
					fnew[5] = -phceInfo.necessaryInfo.NewCoP.Y() * fnew[0];

				}
			}

			if (phceInfo.necessaryInfo.copMargin != 0.0) {
				Vec3d c = phceInfo.necessaryInfo.OldCoP - phceInfo.necessaryInfo.ContPoint;
				phceInfo.necessaryInfo.copMargin = std::min(phceInfo.necessaryInfo.copMargin, -c * phceInfo.necessaryInfo.ContLocal.Ex());
			}
			return;
		}

		default:
		{
			return;
		}
		}
		/*
		switch (i) {

		case 0: // 垂直方向（接触法線方向）
		{
			if (fnew[0] < EPSILON_10) {
				fnew[0] = 0.0;
			}

			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
				updat = true;
				CompResponse(df[i], i);
			}

			break;

		}

		case 1: // 接触面方向（normalと垂直)
		{
			double vmax2cont = sqrt(vjrel[1] * vjrel[1] + vjrel[2] * vjrel[2]);
			double vmax2ideal = GetScene()->GetFrictionThreshold();

			double ftmax = (vmax2cont < vmax2ideal ? mu0 : mu) * fnew[0];
			double ft = sqrt(fnew[1] * fnew[1] + fnew[2] * fnew[2]);

			phceInfo.necessaryInfo.frictionMargin = ftmax - ft;

			if (vmax2cont < vmax2ideal) {
				if (ft > ftmax) {
					ft = ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
				else if (ft < -ftmax) {
					ft = -ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
			}
			else {
				if (ft > ftmax) {
					ft = ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
				else if (ft < -ftmax) {
					ft = -ftmax;
					phceInfo.necessaryInfo.frictionMargin = 0.0;
				}
			}

			
			//if (frictionMargin < 0.0) {
			//double k = ftmax / ft;
			//fnew[1] *= k;
			//fnew[2] *= k;
			//frictionMargin = 0.0;
			//}
			

			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
				updat = true;
				CompResponse(df[i], i);
			}

			break;
		}
		case 4://接触法線垂直方向に作用するトルク
		{

			phceInfo.necessaryInfo.copMargin = EPSILON_10;

			if (fnew[0] < EPSILON_10) {

				fnew[4] = 0, fnew[5] = 0;

				df[4] = fnew[4] - f[4];
				f[4] = fnew[4];

				if (std::abs(df[4]) > engine->dfEps) {
					updat = true;
					CompResponse(df[4], 4);
				}

				break;
			}

			Vec3d CopFromContPoint = Vec3d(0.0, -fnew[5] / fnew[0], fnew[4] / fnew[0]);
			
			CONTACT htc = HowToContact();

			// the first contact is occurred
			//if (Norm3D(phceInfo.necessaryInfo.OldCoP, phceInfo.necessaryInfo.ContPoint) < EPSILON_16){
				
			//}
			
			if (htc == CONTACT::POINT) {

				phceInfo.necessaryInfo.NewCoP = Vec3d();
				phceInfo.necessaryInfo.copMargin = 0.0;

				fnew[4] = phceInfo.necessaryInfo.NewCoP.Z() * fnew[0];
				fnew[5] = -phceInfo.necessaryInfo.NewCoP.Y() * fnew[0];

			}
			else {

				phceInfo.necessaryInfo.OldCoP = phceInfo.necessaryInfo.ContPoint + phceInfo.necessaryInfo.ContLocal * CopFromContPoint;

				// 点が中にあれば処理が必要ない
				bool isCopOnObject0 = isCoPInObject0(), isCopOnObject1 = isCoPInObject1();

				if (isCopOnObject0 && isCopOnObject1) {

					phceInfo.necessaryInfo.copMargin = 0.0;

				}

				else {

					Vec3f w = Vec3f();

					FindClosestPoint(w, phceInfo.necessaryInfo.OldCoP, htc, isCopOnObject0, isCopOnObject1);

					phceInfo.necessaryInfo.NewCoP = phceInfo.necessaryInfo.ContLocal * (w - phceInfo.necessaryInfo.ContPoint);
					phceInfo.necessaryInfo.copMargin = 0.0;

					fnew[4] = phceInfo.necessaryInfo.NewCoP.Z() * fnew[0];
					fnew[5] = -phceInfo.necessaryInfo.NewCoP.Y() * fnew[0];

				}
			}

			if (phceInfo.necessaryInfo.copMargin != 0.0) {
				Vec3d c = phceInfo.necessaryInfo.OldCoP - phceInfo.necessaryInfo.ContPoint;
				phceInfo.necessaryInfo.copMargin = std::min(phceInfo.necessaryInfo.copMargin, -c * phceInfo.necessaryInfo.ContLocal.Ex());
			}

			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
				updat = true;
				CompResponse(df[i], i);
			}

			break;
		}

		case 3:	//接触法線と垂直な方向のトルク
		{
			if (fnew[0] < EPSILON_10) {
				fnew[3] = 0.0;

				df[i] = fnew[i] - f[i];
				f[i] = fnew[i];

				if (std::abs(df[i]) > engine->dfEps) {
					updat = true;
					CompResponse(df[i], i);
				}

				break;
			}

			// 接触法線垂直向きのfnew1, fnew2から生じるモーメント
			// 接触法線垂直向きから生まれる力静止摩擦力のように扱う
			// 拘束している力であると考えている

			double ncop = phceInfo.necessaryInfo.NewCoP.y*fnew[2] - phceInfo.necessaryInfo.NewCoP.z*fnew[1];

			// ここの2/3の意味とあとMarginらが必要な理由というのがまだ理解できない
			double margin = (2.0 / 3.0)*phceInfo.necessaryInfo.copMargin*phceInfo.necessaryInfo.frictionMargin;
			//double margin = (2.0/3.0)*contactRadius*frictionMargin;

			if (fnew[3] - ncop > margin) fnew[3] = ncop + margin;
			else if (fnew[3] - ncop < -margin) fnew[3] = ncop - margin;

			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
				updat = true;
				CompResponse(df[i], i);
			}

			break;
		}

		default:
		{
			df[i] = fnew[i] - f[i];
			f[i] = fnew[i];

			if (std::abs(df[i]) > engine->dfEps) {
				updat = true;
				CompResponse(df[i], i);
			}

			break;
		}
		}
		*/

	}

	// OldCoPが外に出ているときに呼ばれる関数

	bool PHContactEngine::FindClosestPoint(Vec3f & cp, const Vec3d & OldCoP, CONTACT cts, bool copOnObject0, bool copOnObject1){

		double t = 0.0;

		if (cts == CONTACT::LINE) {

			if (phceInfo.contactpartInfo[0].supportTag == CONTACT::LINE && phceInfo.contactpartInfo[1].supportTag == CONTACT::LINE) {

				Vec3d zp_0 = ClosestPtPointLine(OldCoP, phceInfo.contactpartInfo[0].vectorGroup, t);
				Vec3d zp_1 = ClosestPtPointLine(OldCoP, phceInfo.contactpartInfo[1].vectorGroup, t);

				if (isPointInObject1(zp_0)) {
					cp = zp_0;
					return true;
				}
				else if (isPointInObject0(zp_1)) {
					cp = zp_1;
					return true;
				}
				else {
					//EuclideanProjection(SupportTag0, SupportTag1, zp_1, cp);
					//GJKIteration(SupportTag0, SupportTag1, zp_0, zp_1, cp);
					DykstraProjection(cp, OldCoP);
					return true;
				}
			}
			else if (phceInfo.contactpartInfo[0].supportTag == CONTACT::FACE && phceInfo.contactpartInfo[1].supportTag == CONTACT::LINE) {

				if (!copOnObject0 && copOnObject1) {
					GJK2D(phceInfo.contactpartInfo[0].vectorGroup,
						phceInfo.objectInfo[0].CDCptr,
						phceInfo.objectInfo[0].PHSptr, cp, OldCoP);
					return true;
				}
				else if (copOnObject0 && !copOnObject1) {
					cp = ClosestPtPointLine(OldCoP, phceInfo.contactpartInfo[1].vectorGroup, t);
					return true;
				}
				else {
					DykstraProjection(cp, OldCoP);
					return true;
				}
			}
			else if (phceInfo.contactpartInfo[0].supportTag == CONTACT::LINE && phceInfo.contactpartInfo[1].supportTag == CONTACT::FACE) {

				if (!copOnObject0 && copOnObject1) {
					cp = ClosestPtPointLine(OldCoP, phceInfo.contactpartInfo[0].vectorGroup, t);
					return true;
				}
				else if (copOnObject0 && !copOnObject1) {
					GJK2D(phceInfo.contactpartInfo[1].vectorGroup,
						phceInfo.objectInfo[1].CDCptr,
						phceInfo.objectInfo[1].PHSptr, cp, OldCoP);
					return true;
				}
				else {
					DykstraProjection(cp, OldCoP);
					return true;
				}
			}
		}
		else if (cts == CONTACT::FACE){		// 面と面の接触であれば
			if (!copOnObject0 && copOnObject1) {
				GJK2D(phceInfo.contactpartInfo[0].vectorGroup,
					phceInfo.objectInfo[0].CDCptr,
					phceInfo.objectInfo[0].PHSptr, cp, OldCoP);
				return true;
			}
			else if (copOnObject0 && !copOnObject1) {
				GJK2D(phceInfo.contactpartInfo[1].vectorGroup,
					phceInfo.objectInfo[1].CDCptr,
					phceInfo.objectInfo[1].PHSptr, cp, OldCoP);
				return true;
			}
			else {
				DykstraProjection(cp, OldCoP);
				return true;
			}
		}
		else {
			DSTR << "PHContactEngine::FindClosestPoint() cts = " << cts << std::endl;
		}

		// 失敗したらfalse
		cp = Vec3d();
		return false;

	}

	bool PHContactEngine::GJK2D(const std::vector<Vec3d>& c_vec, CDConvex* c, PHSolid* s, Vec3f& tmp, const Vec3d& OldCoP) {

		// 平面をなす点の数
		size_t vec_size = c_vec.size();
		
		// sizeが1で全体が曲面を持つ（円、楕円など）
		if (vec_size == 1) {
			c->Support(tmp, s->GetOrientation().Conjugated() * (OldCoP - s->GetCenterPosition()));
			tmp = s->GetOrientation() * tmp + s->GetCenterPosition();
			return true;
		}

		Posed pose_;
		pose_.Ori() = phceInfo.necessaryInfo.w2x * s->GetOrientation();
		pose_.Pos() = phceInfo.necessaryInfo.w2x * s->GetCenterPosition();

		Vec3d ct2x = pose_.Ori() * OldCoP;

		std::vector<Vec3d> tmp_vec(vec_size);

		auto iter_c_vec = c_vec.begin();
		auto iter_tmp_vec = tmp_vec.begin();

		for (; iter_tmp_vec != tmp_vec.end();) {
			*iter_tmp_vec = pose_.Ori() * (*iter_c_vec);
			++iter_tmp_vec;
			++iter_c_vec;
		}

		iter_tmp_vec = tmp_vec.begin();

		Vec3d center = ct2x - *iter_tmp_vec;

		if (isPointIn2DConvexHull(tmp_vec, ct2x)) {
			tmp = ClosestPtFromGroup(OldCoP, c_vec);
			return true;
		}

		else {
			// 面接触のときにだけ現れるので、もしc_vecの数が1個であるということは曲線の形を持つ2次元のものである

			std::vector<Vec3d> simplex;
			Vec3d tmp_0, tmp_1, tmp_2, tmp_3;
			while (1) {
				switch (simplex.size()) {
				case 0:
				{
					tmp_0 = tmp_vec[PointSupport(tmp_vec, center)];
					simplex.push_back(tmp_0);
					break;
				}
				case 1:
				{
					tmp_1 = ct2x - tmp_0;
					tmp_1 = tmp_vec[PointSupport(tmp_vec, tmp_1)];
					if (simplex[0] == tmp_1) {
						tmp = pose_.Ori().Conjugated() * tmp_1;
						return true;
					}
					simplex.push_back(tmp_1);
					break;
				}
				case 2:
				{
					double t = 0.0;
					Vec3d tmp__ = ClosestPtPointLine(ct2x, simplex[0], simplex[1], t);

					tmp_2 = ct2x - tmp__;
					tmp_2 = tmp_vec[PointSupport(tmp_vec, tmp_2)];

					if (tmp_2 == simplex[0] || tmp_2 == simplex[1]) {
						tmp = pose_.Ori().Conjugated() * tmp__;
						return true;
					}

					simplex.push_back(tmp_2);
					break;
				}
				default:
				{
					Vec3d tmp__ = ClosestPtPointTriangle(ct2x, simplex[0], simplex[1], simplex[2]);

					tmp_3 = ct2x - tmp__;
					tmp_3 = tmp_vec[PointSupport(tmp_vec, tmp_3)];

					if (tmp_3 == simplex[0] || tmp_3 == simplex[1] || tmp_3 == simplex[2]) {
						tmp = pose_.Ori().Conjugated() * tmp__;
						return true;
					}

					simplex.erase(simplex.begin());
					simplex.push_back(tmp_3);
					break;
				}
				}
			}
		}
	}

	bool PHContactEngine::GJK3D(CDConvex * cdc0, PHSolid * phs0, Vec3d & pt0, CDConvex * cdc1, PHSolid * phs1, Vec3d & pt1)
	{
		// support point
		// 初期化
		Vec3d init = phs1->GetCenterPosition() - phs0->GetCenterPosition();
		cdc0->Support((Vec3f)pt0, init);
		cdc1->Support((Vec3f)pt1, -init);
		return false;
	}

	// この関数は改良も必要だしそもそも精度が非常に重要
	// return 1 -> 点接触
	// return 2 -> 線接触
	// reteurn 3 -> 面接触
	/* 1に該当する関数 */

	// 重なり具合
	CONTACT PHContactEngine::HowToContact()
	{
		phceInfo.contactpartInfo[0].vectorGroup.clear();
		phceInfo.contactpartInfo[1].vectorGroup.clear();

		Vec3d tmp = phceInfo.necessaryInfo.ContPoint - phceInfo.objectInfo[1].PHSptr->GetCenterPosition();

		if (tmp.dot(phceInfo.necessaryInfo.ContLocal.Ex()) < 0) {
			Vec3d tmp_0 = phceInfo.objectInfo[0].PHSptr->GetOrientation().Conjugated() * phceInfo.necessaryInfo.ContLocal.Ex();
			phceInfo.contactpartInfo[0].supportTag = static_cast<CONTACT>(phceInfo.objectInfo[0].CDCptr->SupportTag(tmp_0, phceInfo.contactpartInfo[0].vectorGroup));
			for (unsigned int i = 0; i < phceInfo.contactpartInfo[0].vectorGroup.size(); i++)
				phceInfo.contactpartInfo[0].vectorGroup[i] = phceInfo.objectInfo[0].PHSptr->GetOrientation() * phceInfo.contactpartInfo[0].vectorGroup[i] + phceInfo.objectInfo[0].PHSptr->GetCenterPosition();

			Vec3d tmp_1 = phceInfo.objectInfo[1].PHSptr->GetOrientation().Conjugated() * -phceInfo.necessaryInfo.ContLocal.Ex();
			phceInfo.contactpartInfo[1].supportTag = static_cast<CONTACT>(phceInfo.objectInfo[1].CDCptr->SupportTag(tmp_1, phceInfo.contactpartInfo[1].vectorGroup));
			for (unsigned int i = 0; i < phceInfo.contactpartInfo[1].vectorGroup.size(); i++)
				phceInfo.contactpartInfo[1].vectorGroup[i] = phceInfo.objectInfo[1].PHSptr->GetOrientation() * phceInfo.contactpartInfo[1].vectorGroup[i] + phceInfo.objectInfo[1].PHSptr->GetCenterPosition();

		}
		else {
			Vec3d tmp_0 = phceInfo.objectInfo[0].PHSptr->GetOrientation().Conjugated() * -phceInfo.necessaryInfo.ContLocal.Ex();
			phceInfo.contactpartInfo[0].supportTag = static_cast<CONTACT>(phceInfo.objectInfo[0].CDCptr->SupportTag(tmp_0, phceInfo.contactpartInfo[0].vectorGroup));
			for (unsigned int i = 0; i < phceInfo.contactpartInfo[0].vectorGroup.size(); i++)
				phceInfo.contactpartInfo[0].vectorGroup[i] = phceInfo.objectInfo[0].PHSptr->GetOrientation() * phceInfo.contactpartInfo[0].vectorGroup[i] + phceInfo.objectInfo[0].PHSptr->GetCenterPosition();

			Vec3d tmp_1 = phceInfo.objectInfo[1].PHSptr->GetOrientation().Conjugated() * phceInfo.necessaryInfo.ContLocal.Ex();
			phceInfo.contactpartInfo[1].supportTag = static_cast<CONTACT>(phceInfo.objectInfo[1].CDCptr->SupportTag(tmp_1, phceInfo.contactpartInfo[1].vectorGroup));
			for (unsigned int i = 0; i < phceInfo.contactpartInfo[1].vectorGroup.size(); i++)
				phceInfo.contactpartInfo[1].vectorGroup[i] = phceInfo.objectInfo[1].PHSptr->GetOrientation() * phceInfo.contactpartInfo[1].vectorGroup[i] + phceInfo.objectInfo[1].PHSptr->GetCenterPosition();
		}

		//点接触
		if (phceInfo.contactpartInfo[0].supportTag == CONTACT::POINT || phceInfo.contactpartInfo[1].supportTag == CONTACT::POINT) {
			return CONTACT::POINT;
		}

		// 面とEDGEであるかEdgeとEdgeであるか
		else if (phceInfo.contactpartInfo[0].supportTag == CONTACT::LINE || phceInfo.contactpartInfo[1].supportTag == CONTACT::POINT) {
			return CONTACT::LINE;
		}

		// 面と面の接触だけ
		else if (phceInfo.contactpartInfo[0].supportTag == CONTACT::FACE && phceInfo.contactpartInfo[1].supportTag == CONTACT::FACE) {
			assert(phceInfo.contactpartInfo[0].vectorGroup.size() > 0);
			assert(phceInfo.contactpartInfo[1].vectorGroup.size() > 0);
			return CONTACT::FACE;
		}

		return CONTACT::UNKNOWN;
	}

	inline bool PHContactEngine::isPointInObject0(const Vec3d& pt)
	{
		return phceInfo.objectInfo[0].CDCptr->IsInside(phceInfo.objectInfo[0].PHSptr->GetOrientation().Conjugated() *
			(pt - phceInfo.objectInfo[0].PHSptr->GetCenterPosition()));
	}

	inline bool PHContactEngine::isPointInObject1(const Vec3d& pt)
	{
		return phceInfo.objectInfo[1].CDCptr->IsInside(phceInfo.objectInfo[1].PHSptr->GetOrientation().Conjugated() *
			(pt - phceInfo.objectInfo[1].PHSptr->GetCenterPosition()));
	}

	inline bool PHContactEngine::isCoPInObject0()
	{
		return phceInfo.objectInfo[0].CDCptr->IsInside(phceInfo.objectInfo[0].PHSptr->GetOrientation().Conjugated() *
			(phceInfo.necessaryInfo.OldCoP - phceInfo.objectInfo[0].PHSptr->GetCenterPosition()));
	}

	inline bool PHContactEngine::isCoPInObject1()
	{
		return phceInfo.objectInfo[1].CDCptr->IsInside(phceInfo.objectInfo[1].PHSptr->GetOrientation().Conjugated() *
			(phceInfo.necessaryInfo.OldCoP - phceInfo.objectInfo[1].PHSptr->GetCenterPosition()));
	}

	// 中にあればTrue
	// 外にあればFalse
	bool PHContactEngine::IsInside2D(const std::vector<Vec3d>& c_vec, CDConvex * c, PHSolid * s, const Vec3d & pt)
	{
		Posed pose_;
		pose_.Ori() = phceInfo.necessaryInfo.w2x * s->GetOrientation();
		pose_.Pos() = phceInfo.necessaryInfo.w2x * s->GetCenterPosition();

		Vec3d ct2x = pose_.Ori() * pt;

		std::vector<Vec3d> tmp_vec(c_vec.size());
		for (unsigned int i = 0; i < tmp_vec.size(); i++) {
			tmp_vec[i] = pose_.Ori() * c_vec[i];
		}

		if (tmp_vec.size() != 1) {
			return isPointIn2DConvexHull(tmp_vec, ct2x);
		}
		else {
			Vec3f w;
			c->Support(w, s->GetOrientation().Conjugated() * (pt - s->GetCenterPosition()));
			w = s->GetOrientation() * w + s->GetCenterPosition();
			return (((pose_.Ori() * pt).YZ()).norm() - ((pose_.Ori() * w).YZ()).norm() >= 0) ? false : true;
		}
	}

	/*
	bool PHContactEngine::GJKIteration(int SupportTag0, int SupportTag1, const Vec3f& z_0, const Vec3f& z_1, Vec3f& tmp) {

		Vec3d pt = OldCoP;
		Vec3d tmp_0 = z_0;
		Vec3d tmp_1 = z_1;
		Vec3f tmp_tmp, tmp_tmp_0, tmp_tmp_1;

		// 切り替えながら判定するための変数
		int cross = 0;
		double t = 0.0;

		double stop = Norm2D(Pose0, tmp_0, Pose1, tmp_1);
		if (SupportTag0 == 2 && SupportTag1 == 2) {

			tmp_tmp_0 = z_0;
			tmp_tmp_1 = z_1;

			while (stop > EPSILON_2) {
				tmp_tmp_0 = ClosestPtPointLine(tmp_1, VecGroup0[0], VecGroup0[1], t);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_0 = tmp_tmp_0;
					break;
				}
				tmp_tmp_1 = ClosestPtPointLine(tmp_0, VecGroup1[0], VecGroup1[1], t);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_1 = tmp_tmp_1;
					break;
				}
				tmp_0 = tmp_tmp_0;
				tmp_1 = tmp_tmp_1;
				stop = Norm2D(Pose0, tmp_0, Pose1, tmp_1);
			}

			tmp = (tmp_0 + tmp_1) / 2;
			return true;

		}
		else if (SupportTag0 == 3 && SupportTag1 == 2) {
			while (stop > EPSILON_2) {

				GJK2D(VecGroup0, CDC0, PHS0, tmp_tmp_0, tmp_1);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_0 = tmp_tmp_0;
					break;
				}
				tmp_tmp_1 = ClosestPtPointLine(tmp_0, VecGroup1[0], VecGroup1[1], t);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_1 = tmp_tmp_1;
					break;
				}
				tmp_0 = tmp_tmp_0;
				tmp_1 = tmp_tmp_1;
				stop = Norm2D(Pose0, tmp_0, Pose1, tmp_1);
			}

			tmp = (tmp_0 + tmp_1) / 2;
			return true;
		}
		else if (SupportTag0 == 2 && SupportTag1 == 3) {
			while (stop > EPSILON_2) {

				GJK2D(VecGroup1, CDC1, PHS1, tmp_tmp_1, tmp_0);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_1 = tmp_tmp_1;
					break;
				}
				tmp_tmp_0 = ClosestPtPointLine(tmp_1, VecGroup0[0], VecGroup0[1], t);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_0 = tmp_tmp_0;
					break;
				}
				tmp_0 = tmp_tmp_0;
				tmp_1 = tmp_tmp_1;
				stop = Norm2D(Pose0, tmp_0, Pose1, tmp_1);
			}

			tmp = (tmp_0 + tmp_1) / 2;
			return true;

		}
		else {
			while (stop > EPSILON_2) {

				GJK2D(VecGroup0, CDC0, PHS0, tmp_tmp_0, tmp_1);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_0 = tmp_tmp_0;
					break;
				}
				GJK2D(VecGroup1, CDC1, PHS1, tmp_tmp_1, tmp_0);
				if (Norm2D(Pose0, tmp_tmp_0, Pose1, tmp_tmp_1) > EPSILON_2) {
					tmp_1 = tmp_tmp_1;
					break;
				}
				tmp_0 = tmp_tmp_0;
				tmp_1 = tmp_tmp_1;
				stop = Norm2D(Pose0, tmp_0, Pose1, tmp_1);
			}

			tmp = (tmp_0 + tmp_1) / 2;
			return true;
		}
	}

	// Euclidean Projection
	void PHContactEngine::EuclideanProjection(CONTACT SupportTag0, CONTACT SupportTag1, const Vec3f& z_0, Vec3f& tmp) {

		// 移動前の点
		Vec3d prev_point = OldCoP;
		// 移動後の点
		Vec3d current_point = z_0;

		// 切り替えながら判定するための変数
		int point_on_where = 0;
		double t = 0.0;

		// 停止条件
		double stop = Norm3D(prev_point, current_point);

		if (SupportTag0 == CONTACT::LINE && SupportTag1 == CONTACT::LINE) {

			while (stop > EPSILON_2) {
				prev_point = current_point;
				current_point = point_on_where == 0 ? ClosestPtPointLine(current_point, VecGroup1[0], VecGroup1[1], t) :
					ClosestPtPointLine(current_point, VecGroup0[0], VecGroup0[1], t);
				// current_pointがどこの物体に対して引かれたをずっとビットを反対させるため
				point_on_where = point_on_where ^ 1;
				stop = Norm3D(prev_point, current_point);
			}

			tmp = current_point;

		}
		else if (SupportTag0 == CONTACT::FACE && SupportTag1 == CONTACT::LINE) {

			Vec3d tmp_current_point;

			while (stop > EPSILON_2) {
				prev_point = current_point;
				if (point_on_where == 0) current_point = ClosestPtPointLine(current_point, VecGroup1[0], VecGroup1[1], t);
				else {
					GJK2D(VecGroup0, CDC0, PHS0, (Vec3f)tmp_current_point, current_point);
					current_point = tmp_current_point;
				}
				point_on_where = point_on_where ^ 1;
				stop = Norm3D(prev_point, current_point);
			}

			tmp = current_point;
		}
		else if (SupportTag0 == CONTACT::LINE && SupportTag1 == CONTACT::FACE) {

			Vec3d tmp_current_point;

			while (stop > EPSILON_2) {
				prev_point = current_point;
				if (point_on_where == 1) current_point = ClosestPtPointLine(current_point, VecGroup0[0], VecGroup0[1], t);
				else {
					GJK2D(VecGroup1, CDC1, PHS1, (Vec3f)tmp_current_point, current_point);
					current_point = tmp_current_point;
				}
				point_on_where = point_on_where ^ 1;
				stop = Norm3D(prev_point, current_point);
			}

			tmp = current_point;

		}
		else
		{
			while (stop > EPSILON_2) {

				Vec3d tmp_current_point;

				while (stop > EPSILON_2) {
					prev_point = current_point;
					if (point_on_where == 1) {
						GJK2D(VecGroup0, CDC0, PHS0, (Vec3f)tmp_current_point, current_point);
						current_point = tmp_current_point;
					}
					else {
						GJK2D(VecGroup1, CDC1, PHS1, (Vec3f)tmp_current_point, current_point);
						current_point = tmp_current_point;
					}
					point_on_where = point_on_where ^ 1;
					stop = Norm3D(prev_point, current_point);
				}

				tmp = current_point;
			}
		}
	}
	*/


	// 任意の2次元のConvex間の共通空間に対してある点から一番近い点を探す
	/// Wikipedia
	void PHContactEngine::DykstraProjection(Vec3f& v, const Vec3d& pt) {
		
		Vec3d pk(0, 0, 0), qk(0, 0, 0);
		Vec3d xk = pt;
		Vec3d prevxk = pt;
		Vec3d yk(0, 0, 0);
		int obj = 0;
		int Limits = 15;
		while (Limits--) {
			GJK2D(phceInfo.contactpartInfo[obj].vectorGroup, 
				phceInfo.objectInfo[obj].CDCptr, 
				phceInfo.objectInfo[obj].PHSptr, 
				(Vec3f)yk, xk + pk);
			pk = xk + pk - yk;
			GJK2D(phceInfo.contactpartInfo[obj^1].vectorGroup,
				phceInfo.objectInfo[obj^1].CDCptr,
				phceInfo.objectInfo[obj^1].PHSptr,
				(Vec3f)xk, yk + qk);
			qk = yk + qk - xk;
			
			if (Norm3D(xk, prevxk) < EPSILON_8) break;
			
			prevxk = xk;
			obj ^= 1;
		}

		v = xk;
	}

}