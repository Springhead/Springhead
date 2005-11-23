#ifndef CDSPHERE_H
#define CDSPHERE_H
#include <Collision/CDGeometry.h>
#include <Collision/CDConvex.h>

namespace Spr {;
/**	球体	*/
class CDSphere:public CDConvex{
	mutable Vec3f curPos;
public:
	///	中心の座標
	Vec3f center;
	///	半径
	float radius;

	SGOBJECTDEF(CDSphere);
	///
	CDSphere(float r = 0.0f, Vec3f c = Vec3f()):radius(r), center(c){}
	///	GeomtryID
	virtual int GeometryID(){ return CONVEX; }
	
	///	サポートポイントを求める．
	virtual Vec3f Support(const Vec3f& p) const;
	///	BBoxの計算
	void CalcBBox(Vec3f& bbMin, Vec3f& bbMax);
};

}
#endif
