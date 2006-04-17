#include <Springhead.h>
#include <Foundation/Object.h>
#include "Collision.h"
#include <set>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDQuickHull3DImp.h"

namespace Spr{;

//----------------------------------------------------------------------------
//	CDBox
OBJECTIMP(CDBox, CDConvex);	
IF_IMP(CDBox, CDConvex);	

CDBox::CDBox() {
}

CDBox::CDBox(const CDBoxDesc& desc) {
	boxsize = desc.boxsize;
	/*
	Vec3f boxpos = GetPose().Pos();
	float minx = boxpos.x - boxsize.x / 2;
	float maxx = boxpos.x + boxsize.x / 2;
	float miny = boxpos.y - boxsize.y / 2;
	float maxy = boxpos.y + boxsize.y / 2;
	float minz = boxpos.z - boxsize.z / 2;
	float maxz = boxpos.z + boxsize.z / 2;
	*/
	// ローカル座標系で、boxの位置を設定
	Vec3f halfsize = Vec3f(boxsize.x/2.0, boxsize.y/2.0, boxsize.z/2.0);
	base.push_back(Vec3f( halfsize.x,  halfsize.y, -halfsize.z));
	base.push_back(Vec3f( halfsize.x,  halfsize.y,  halfsize.z));
	base.push_back(Vec3f( halfsize.x, -halfsize.y,  halfsize.z));
	base.push_back(Vec3f( halfsize.x, -halfsize.y, -halfsize.z));
	base.push_back(Vec3f(-halfsize.x,  halfsize.y, -halfsize.z));
	base.push_back(Vec3f(-halfsize.x,  halfsize.y,  halfsize.z));
	base.push_back(Vec3f(-halfsize.x, -halfsize.y,  halfsize.z));
	base.push_back(Vec3f(-halfsize.x, -halfsize.y, -halfsize.z));

	for (int nface=0; nface<6; ++nface){	// 立方体は8面
		qfaces.push_back(CDqface());
	}
	// boxの各面の法線を設定、boxの各面の頂点のインデックスを設定
	qfaces[0].normal  = Vec3f(1.0, 0.0, 0.0);
	qfaces[0].vtxs[0] = 0;		
	qfaces[0].vtxs[1] = 1;
	qfaces[0].vtxs[2] = 2;
	qfaces[0].vtxs[3] = 3;
	qfaces[1].normal  = Vec3f(0.0, 1.0, 0.0);
	qfaces[1].vtxs[0] = 0;		
	qfaces[1].vtxs[1] = 4;
	qfaces[1].vtxs[2] = 5;
	qfaces[1].vtxs[3] = 1;
	qfaces[2].normal  = Vec3f(0.0, 0.0, -1.0);
	qfaces[2].vtxs[0] = 4;		
	qfaces[2].vtxs[1] = 7;
	qfaces[2].vtxs[2] = 6;
	qfaces[2].vtxs[3] = 5;
	qfaces[3].normal  = Vec3f(0.0, -1.0, 0.0);
	qfaces[3].vtxs[0] = 3;		
	qfaces[3].vtxs[1] = 2;
	qfaces[3].vtxs[2] = 6;
	qfaces[3].vtxs[3] = 7;
	qfaces[4].normal  = Vec3f(0.0, 0.0, 1.0);
	qfaces[4].vtxs[0] = 1;		
	qfaces[4].vtxs[1] = 5;
	qfaces[4].vtxs[2] = 6;
	qfaces[4].vtxs[3] = 2;
	qfaces[5].normal  = Vec3f(0.0, 0.0, -1.0);
	qfaces[5].vtxs[0] = 0;		
	qfaces[5].vtxs[1] = 3;
	qfaces[5].vtxs[2] = 7;
	qfaces[5].vtxs[3] = 4;
}

// サポートポイントを求める
Vec3f CDBox::Support(const Vec3f& p) const {
	Vec3f boxpos = GetPose().Pos();		// 直方体の位置(Boxの中心位置)を取得
	/*return Vec3f(p.x < 0.0 ? boxpos.x+boxsize.x/2 : boxpos.x-boxsize.x/2,
				 p.y < 0.0 ? boxpos.y+boxsize.y/2 : boxpos.y-boxsize.y/2,
				 p.z < 0.0 ? boxpos.z+boxsize.z/2 : boxpos.z-boxsize.z/2);	*/
	Vec3f halfsize = Vec3f(boxsize.x/2.0, boxsize.y/2.0, boxsize.z/2.0);
	return Vec3f(p.x < 0.0 ? halfsize.x : -halfsize.x,
				p.y < 0.0 ? halfsize.y : -halfsize.y,
				p.z < 0.0 ? halfsize.z : -halfsize.z);
}

// 切り口を求める. 接触解析を行う.
bool CDBox::FindCutRing(CDCutRing& ring, const Posed& toW) {
	int base_size		= 8;	// base.size()=8頂点
	int qfaces_size	= 6;	// qfaces_size()=6面 
	
	Posed toL	  = toW.Inv();
	// 頂点がどっち側にあるか調べる
	Vec3d planePosL = toL * ring.local.Pos();
	Vec3d planeNormalL = toL.Ori() * ring.local.Ori() * Vec3d(1, 0, 0);
	std::vector<bool> inside;
	inside.resize(base_size);
	double d = planeNormalL * planePosL;
	for (unsigned i=0; i<base_size; ++i){
		double vtxDist = planeNormalL * base[i];
		inside[i] = vtxDist >= d;
	}
	bool rv = false;
	//	またがっている面の場合，交線を求める
	for(unsigned i=0; i<qfaces_size; ++i){		// face.size()=6面
		//　全頂点がplaneに対して同じ方向にある場合はパス
		if (inside[qfaces[i].vtxs[0]] == inside[qfaces[i].vtxs[1]] &&
			inside[qfaces[i].vtxs[0]] == inside[qfaces[i].vtxs[2]] &&
			inside[qfaces[i].vtxs[0]] == inside[qfaces[i].vtxs[3]]) continue;
		
		//	接触面(plane,面1)とboxの面(qface,面2)の交線を求める
		/*	直線をとおる1点を見つけるのは
						|面2
						|n2
						|d2
				   O	|
			-------+----+----面1 n1,d1=0
						|P
			P = a*n1 + b*n2;
				a = (d1 - d2*(n1*n2)) / (1-(n1*n2)^2)
				b = (d2 - d1*(n1*n2)) / (1-(n1*n2)^2)
			が面1(plane)と面2(qface)が作る直線を通る1点
			O:		planePosL
			n1,d1	planeの法線(planeNormalL)，Oからの距離=0
			n2,d2	qfaceの法線(qfaceNormal)，Oからの距離			
		*/
		Vec3d qfaceNormal = qfaces[i].normal;
		double qfaceDist = qfaceNormal * (base[qfaces[i].vtxs[0]] - planePosL);
		Vec3d lineDirection = (planeNormalL ^ qfaceNormal).unit();
		double ip = planeNormalL * qfaceNormal;
		double a = -qfaceDist*ip / (1-(ip*ip));
		double b = qfaceDist / (1-(ip*ip));
		Vec3d lineOff = a*planeNormalL + b*qfaceNormal;
		Vec3d lineNormal = planeNormalL ^ lineDirection;
		double lineDist = lineNormal * lineOff;

		// local -> world -> ring2次元系に変換
		Posed to2D = ring.localInv * toW;
		Vec2d lineNormal2D = (to2D.Ori() * lineNormal).sub_vector(1, Vec2d());
		//	線は内側を向かせたいので， normal, dist を反転して ring.lines に追加
		ring.lines.push_back(CDCutLine(-lineNormal2D, -lineDist));
		rv = true;
	}
	//bool 衝突の有無
	return rv;
}

// 直方体のサイズを取得
Vec3f CDBox::GetBoxSize() {
	return boxsize;
}

}	//	namespace Spr
