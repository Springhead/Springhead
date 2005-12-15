#ifndef SPR_PHDEF_H
#define SPR_PHDEF_H

namespace Spr {;

inline bool approx(const double x, const double y){
	const double eps = 1e-6;
	return ((x==y)
			|| (fabs(x-y) < eps)
			|| (fabs(x/y - 1.0) < eps));
}

inline bool approx(const Vec3d v1, const Vec3d v2){
	return (approx(v1.x, v2.x) && approx(v1.y, v2.y) && approx(v1.z, v2.z));
}

}	//	namespace Spr

#endif	

