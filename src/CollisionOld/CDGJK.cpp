#include "Collision.h"
#pragma hdrstop

namespace Spr{;
double relError = 1e-6;
double absError = 1e-10;

inline bool approxZero(const Vec3d& v) {
	return v.square() < 1e-10f;
}

inline void setMax(double& x, double y) {
	if (x < y) x = y;
}

static Vec3d p[4];			// Support points of object A in local coordinates 
static Vec3d q[4];			// Support points of object B in local coordinates 
static Vec3d p_q[4];		// Support points of A - B in world coordinates

static int usedPoints;		// identifies current simplex
static int lastPoint;		// identifies last found support vector
static int lastUsed;		// lastUsed = 1<<lastPoint
static int allUsedPoints;	// allUsedPoints = usedPoints|lastUsed 
static double det[16][4];	// cached sub-determinants

inline void computeDet() {
	static double dotp[4][4];

	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<=1) 
		if (usedPoints & curPoint) dotp[i][lastPoint] = dotp[lastPoint][i] = p_q[i] * p_q[lastPoint];
	dotp[lastPoint][lastPoint] = p_q[lastPoint] * p_q[lastPoint];

	det[lastUsed][lastPoint] = 1;
	for (int j = 0, sj = 1; j < 4; ++j, sj <<= 1) {
		if (usedPoints & sj) {
			int s2 = sj|lastUsed;
			det[s2][j] = dotp[lastPoint][lastPoint] - dotp[lastPoint][j]; 
			det[s2][lastPoint] = dotp[j][j] - dotp[j][lastPoint];
			for (int k = 0, sk = 1; k < j; ++k, sk <<= 1) {
				if (usedPoints & sk) {
					int s3 = sk|s2;
					det[s3][k] = det[s2][j] * (dotp[j][j] - dotp[j][k]) + 
						det[s2][lastPoint] * (dotp[lastPoint][j] - dotp[lastPoint][k]);
					det[s3][j] = det[sk|lastUsed][k] * (dotp[k][k] - dotp[k][j]) + 
						det[sk|lastUsed][lastPoint] * (dotp[lastPoint][k] - dotp[lastPoint][j]);
					det[s3][lastPoint] = det[sk|sj][k] * (dotp[k][k] - dotp[k][lastPoint]) + 
						det[sk|sj][j] * (dotp[j][k] - dotp[j][lastPoint]);
				}
			}
		}
	}
	if (allUsedPoints == 15) {
		det[15][0] =	det[14][1] * (dotp[1][1] - dotp[1][0]) + 
						det[14][2] * (dotp[2][1] - dotp[2][0]) + 
						det[14][3] * (dotp[3][1] - dotp[3][0]);
		det[15][1] =	det[13][0] * (dotp[0][0] - dotp[0][1]) + 
						det[13][2] * (dotp[2][0] - dotp[2][1]) + 
						det[13][3] * (dotp[3][0] - dotp[3][1]);
		det[15][2] =	det[11][0] * (dotp[0][0] - dotp[0][2]) + 
						det[11][1] * (dotp[1][0] - dotp[1][2]) +	
						det[11][3] * (dotp[3][0] - dotp[3][2]);
		det[15][3] =	det[ 7][0] * (dotp[0][0] - dotp[0][3]) + 
						det[ 7][1] * (dotp[1][0] - dotp[1][3]) + 
						det[ 7][2] * (dotp[2][0] - dotp[2][3]);
	}	
}

inline bool IsVaildPoint(int s) {
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (allUsedPoints & curPoint) {
			if (s & curPoint) { if (det[s][i] <= 0) return false; }
			else if (det[s|curPoint][i] > 0) return false;
		}
	}
	return true;
}

inline void CalcVector(int usedPoints, Vec3d& v) {
	double sum = 0;
	v = Vec3d(0, 0, 0);
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (usedPoints & curPoint) {
			sum += det[usedPoints][i];
			v += p_q[i] * det[usedPoints][i];
		}
	}
	v *= 1 / sum;
}

inline void CalcPoints(int usedPoints, Vec3d& p1, Vec3d& p2) {
	double sum = 0;
	p1 = Vec3d(0.0, 0.0, 0.0);
	p2 = Vec3d(0.0, 0.0, 0.0);
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1) {
		if (usedPoints & curPoint) {
			sum += det[usedPoints][i];
			p1 += p[i] * det[usedPoints][i];
			p2 += q[i] * det[usedPoints][i];
		}
	}
	if (sum){
		double s = 1 / sum;
		p1 *= s;
		p2 *= s;
	}else{
		p1 = p[0];
		p2 = q[0];
	}
}

inline bool IsClosest(Vec3d& v) {
	computeDet();
	for (int s = usedPoints; s; --s) {
		if ((s & usedPoints) == s) {
			if (IsVaildPoint(s|lastUsed)) {
				usedPoints = s|lastUsed;
				CalcVector(usedPoints, v);
				return true;
			}
		}
	}
	if (IsVaildPoint(lastUsed)) {
		usedPoints = lastUsed;
		v = p_q[lastPoint];
		return true;
	}
	return false;
}

// The next function is used for detecting degenerate cases that cause 
// termination problems due to rounding errors.	
	 
inline bool IsDegenerate(const Vec3d& w) {
	for (int i = 0, curPoint = 1; i < 4; ++i, curPoint <<= 1){
//	hase	2003.10.24
//		if ((allUsedPoints & curPoint) && p_q[i] == w) return true;
		if ((allUsedPoints & curPoint) && (p_q[i]-w).square() < 1e-6){
//			DSTR << "T:" << (p_q[i]-w).norm() << p_q[i] << w << std::endl;
			return true;
		}
	}
	return false;
}

bool FindCommonPoint(const CDConvex& a, const CDConvex& b,
					 const Affinef& a2w, const Affinef& b2w,
					 Vec3d& v, Vec3d& pa, Vec3d& pb) {
	Vec3d w;

	usedPoints = 0;
	allUsedPoints = 0;

	do {
		lastPoint = 0;
		lastUsed = 1;
		while (usedPoints & lastUsed) { ++lastPoint; lastUsed <<= 1; }
		p[lastPoint] = a.Support(a2w.Rot().trans() * (-v));
		q[lastPoint] = b.Support(b2w.Rot().trans() * v);
		w = a2w * p[lastPoint]  -  b2w * q[lastPoint];
		if (v*w > 0) return false;
		if (IsDegenerate(w)) return false;
		p_q[lastPoint] = w;
		allUsedPoints = usedPoints|lastUsed;
		if (!IsClosest(v)) return false;
	}while (usedPoints < 15 && !approxZero(v) ) ;
	CalcPoints(usedPoints, pa, pb);
	return true;
}

void FindClosestPoints(const CDConvex& a, const CDConvex& b,
					  const Affinef& a2w, const Affinef& b2w,
					  Vec3d& pa, Vec3d& pb) {
	Vec3d v = a2w * a.Support(Vec3d()) - b2w * b.Support(Vec3d());
	double len = v.norm();
	double mu = 0.0f;
	Vec3d w;

	usedPoints = 0;
	allUsedPoints = 0;

	while (usedPoints < 15 && len > absError) {
		lastPoint = 0;
		lastUsed = 1;
		while (usedPoints & lastUsed) { ++lastPoint; lastUsed <<= 1; }
		p[lastPoint] = a.Support(a2w.Rot().trans() * (-v));
		q[lastPoint] = b.Support(b2w.Rot().trans() * v);
		w = a2w * p[lastPoint]  -  b2w * q[lastPoint];
		setMax(mu, v*w/len);
		if (len - mu <= len * relError) break;
		if (IsDegenerate(w)) break;
		p_q[lastPoint] = w;
		allUsedPoints = usedPoints|lastUsed;
		if (!IsClosest(v)) break;
		len = v.norm();
	}
	CalcPoints(usedPoints, pa, pb);
}

}
