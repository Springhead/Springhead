/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDBox.h>

namespace Spr{;
const double sqEpsilon = 1e-4;
const double epsilon = 1e-8;
const double epsilon2 = epsilon*epsilon;
const double support_epsilon = 1.0e-4;

//----------------------------------------------------------------------------
//	CDBox
CDQuadFaces CDBox::qfaces;		// 面（四角形:quadrangular face）
std::vector<int> CDBox::neighbor[8];	// 頂点の隣の点

CDBox::CDBox() {
}

CDBox::CDBox(const CDBoxDesc& desc) {
	material  = desc.material;
	boxsize   = desc.boxsize;
	Recalc();
}
bool CDBox::IsInside(const Vec3f& p){
	Vec3f half = 0.5f * boxsize;
	return -half.x < p.x && p.x < half.x &&
		   -half.y < p.y && p.y < half.y &&
		   -half.z < p.z && p.z < half.z;
}
float CDBox::CalcVolume(){
	return boxsize.x * boxsize.y * boxsize.z;
}
Matrix3f CDBox::CalcMomentOfInertia(){
	Vec3f sz = boxsize;
	Vec3f sz2 = Vec3f(sz.x*sz.x, sz.y*sz.y, sz.z*sz.z);
	return ((sz.x * sz.y * sz.z) / 12.0f) * Matrix3f::Diag(sz2.y + sz2.z, sz2.x + sz2.z, sz2.x + sz2.y);
}

void CDBox::Recalc(){
	// ローカル座標系で、boxの位置を設定
	Vec3f halfsize = Vec3f(boxsize.x/2.0, boxsize.y/2.0, boxsize.z/2.0);
	base.clear();
	base.push_back(Vec3f( halfsize.x,  halfsize.y, -halfsize.z));
	base.push_back(Vec3f( halfsize.x,  halfsize.y,  halfsize.z));
	base.push_back(Vec3f( halfsize.x, -halfsize.y,  halfsize.z));
	base.push_back(Vec3f( halfsize.x, -halfsize.y, -halfsize.z));
	base.push_back(Vec3f(-halfsize.x,  halfsize.y, -halfsize.z));
	base.push_back(Vec3f(-halfsize.x,  halfsize.y,  halfsize.z));
	base.push_back(Vec3f(-halfsize.x, -halfsize.y,  halfsize.z));
	base.push_back(Vec3f(-halfsize.x, -halfsize.y, -halfsize.z));
	//      ^z              ^z        
	//      |               |         
	//   6  |  5         2  |  1      
	//      |               |         
	//------o------>y ------+------>y 
	//      |x-             |x+       
	//   7  |  4         3  |  0      
	//      |               |         
	if (!neighbor[0].size()){
		for(int i=0; i<8; ++i) neighbor[i].resize(3);
		neighbor[0][0] = 3; neighbor[0][1] = 1; neighbor[0][2] = 4;
		neighbor[1][0] = 2; neighbor[1][1] = 0; neighbor[1][2] = 5;
		neighbor[2][0] = 1; neighbor[2][1] = 3; neighbor[2][2] = 6;
		neighbor[3][0] = 0; neighbor[3][1] = 2; neighbor[3][2] = 7;
		neighbor[4][0] = 7; neighbor[4][1] = 5; neighbor[4][2] = 0;
		neighbor[5][0] = 6; neighbor[5][1] = 4; neighbor[5][2] = 1;
		neighbor[6][0] = 5; neighbor[6][1] = 7; neighbor[6][2] = 2;
		neighbor[7][0] = 4; neighbor[7][1] = 6; neighbor[7][2] = 3;

		qfaces.clear();
		for (int nface=0; nface<6; ++nface){	// 立方体は6面
			qfaces.push_back(CDQuadFace());
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
		qfaces[2].normal  = Vec3f(-1.0, 0.0, 0.0);
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
	curPos = 0;
}

// サポートポイントを求める
int CDBox::Support(Vec3f& w, const Vec3f& v) const {
	// 与えられた方向pに一番遠い点（内積最大の点をサポートポイントとする）
	float d1=0.0, d2=0.0;
	for (unsigned int i=0; i<8; ++i){		// 8頂点
		d1 = base[i] * v;
		if (d1 > d2) { 
			d2 = d1;
			curPos = i;
		}
	}
	w = base[curPos];
	return curPos;
}
// どこにサポートポイントがあるかをint値で返す
// Vertex = 0, Edge = 1, Face = 2
// v は　local.Ex()が入ってくる
int CDBox::SupportTag(const Vec3f& v, std::vector<Vec3d>& c_vec) const {

	// 最大内積値中に同じサポートポイントはいくつあるか数える
	int count = 0;

	float d1 = 0.0, d2 = 0.0;
	for (unsigned int i = 0; i < 8; ++i) {		// 8頂点
		d1 = base[i] * v;
		if (d1 > d2) {
			d2 = d1;
		}
	}

	for (unsigned int i = 0; i < 4; i++) {
		d1 = base[i] * v;
		if (std::abs(d1 - d2) < support_epsilon) {
			count++;
			c_vec.push_back(base[i]);
		}
	}
	for (unsigned int i = 7; i > 3; i--) {
		d1 = base[i] * v;
		if (std::abs(d1 - d2) < support_epsilon) {
			count++;
			c_vec.push_back(base[i]);
		}
	}

	switch (count) {
	case 1:
		return 1; // box : VERTEX
	case 2:
		return 2; // box : EDGE
	default:
		return 3; // box : FACE
	}

}


std::vector<int>& CDBox::FindNeighbors(int vtx){
	return neighbor[vtx];
}

// 切り口を求める. 接触解析を行う.
IntersectionType CDBox::FindCutRing(CDCutRing& ring, const Posed& toW) {
	const int qfaces_size = 6;	// qfaces.size()=6面 
	const int base_size = 8;

	Posed toL	  = toW.Inv();
	// 頂点がどっち側にあるか調べる
	Vec3d planePosL = toL * ring.local.Pos();
	Vec3d planeNormalL = toL.Ori() * ring.local.Ori() * Vec3d(1, 0, 0);
	int sign[base_size];
	double d = planeNormalL * planePosL;
	for (int i=0; i<base_size; ++i){
		double vtxDist = planeNormalL * base[i];
		if (vtxDist > d + sqEpsilon) sign[i] = 1;
		else if (vtxDist < d - sqEpsilon) sign[i] = -1;
		else sign[i] = 0;
	}
	bool rv = false;
	//	またがっている面の場合，交線を求める
	for(int i=0; i<qfaces_size; ++i){		// face.size()=6面
		//　全頂点がplaneに対して同じ方向にある場合はパス
		if (sign[qfaces[i].vtxs[0]] == sign[qfaces[i].vtxs[1]] &&
			sign[qfaces[i].vtxs[0]] == sign[qfaces[i].vtxs[2]] &&
			sign[qfaces[i].vtxs[0]] == sign[qfaces[i].vtxs[3]]) continue;
		
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
		double l_ipip = 1-(ip*ip);
		if (l_ipip < epsilon2) continue;	//	平行な面は無視
		double a = -qfaceDist*ip / (1-(ip*ip));
		double b = qfaceDist / (1-(ip*ip));
		Vec3d lineOff = a*planeNormalL + b*qfaceNormal;
		Vec3d lineNormal = planeNormalL ^ lineDirection;
		double lineDist = lineNormal * lineOff;
		if (finite(lineDist)) {	
			// local -> world -> ring2次元系に変換
			Posed to2D = ring.localInv * toW;
			Vec2d lineNormal2D = (to2D.Ori() * lineNormal).sub_vector(1, Vec2d());
			//	線は内側を向かせたいので， normal, dist を反転して ring.lines に追加
			ring.lines.push_back(CDCutLine(-lineNormal2D, -lineDist));
			rv = true;
		} 
	}
	//bool 衝突の有無
	return SEC_POLYGON;
}

// 直方体のサイズを取得
Vec3f CDBox::GetBoxSize() {
	return boxsize;
}

Vec3f* CDBox::GetVertices(){
	return &*base.begin();
}

CDFaceIf* CDBox::GetFace(int i){
	return qfaces.at(i).Cast();
}

// 直方体のサイズを設定
Vec3f CDBox::SetBoxSize(Vec3f boxSize){
	boxsize   = boxSize;
	bboxReady = false;
	Recalc();
	CalcMetrics(); 
	return boxsize;
}

int CDBox::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset){
	const float eps = 1.0e-10f;
	Vec3f p;
	int num = 0;

	for(size_t i = 0; i < qfaces.size(); i++){
		const CDQuadFace& f = qfaces[i];
		const Vec3f& n = f.normal;	//面の法線
		float tmp = n * dir;		//面の法線とポインタのベクトルとの内積
		if(abs(tmp) < eps)	//内積が小さい場合は判定しない
			continue;

		float s = ((base[f.vtxs[0]] - origin) * n) / tmp; //カメラと面の距離 
		if(s < 0.0)
			continue;
		p = origin + dir * s;	//直線と面の交点p = カメラ座標系の原点+カメラ座標系から面へのベクトル*距離 (Shape座標系)

		// 4角形の内部にあるか
		Vec3d b =GetBoxSize()*0.5; //CDBoxのx,y,z軸に対する最大値
		if (abs(n.x) > 0.99) {
			if(-b.y<=p.y && p.y <=b.y && -b.z<=p.z && p.z <=b.z ){
				result[num] = p;
				offset[num] = s;
				num++;
			}
		} else if (abs(n.y) > 0.99) {
			if(-b.x<=p.x && p.x <=b.x && -b.z<=p.z && p.z <=b.z ){
				result[num] = p;
				offset[num] = s;
				num++;
			}
		} else if (abs(n.z) > 0.99) {
			if(-b.y<=p.y && p.y <=b.y && -b.x<=p.x && p.x <=b.x ){
				result[num] = p;
				offset[num] = s;
				num++;
			}
		}

		if(num == 2)		// 理屈上は3つ以上はあり得ないが念のため
			break;
	}
	return num;
}

}	//	namespace Spr
