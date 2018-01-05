/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Collision/CDConvexMesh.h>
#include <Collision/CDQuickHull3DImp.h>
#include <set>

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE

namespace Spr{;
const double epsilon = 1e-8;
const double epsilon2 = epsilon*epsilon;


int CDVertexIDs::FindPos(int id) const {
	const_iterator lower = begin();
	const_iterator upper = end();
	while(lower != upper){
		const_iterator middle = lower + (upper-lower)/2;
		if (*middle < id){
			lower = middle;
		}else if (*middle > id){
			upper = middle;
		}else{
			return middle - begin();
		}
	}
	return -1;
}



//----------------------------------------------------------------------------
//	CDConvexMesh
CDConvexMesh::CDConvexMesh(){
}
CDConvexMesh::CDConvexMesh(const CDConvexMeshDesc& desc){
	SetDesc(&desc);
}
void CDConvexMesh::SetDesc(const void *ptr){
	CDShape::SetDesc(ptr);
	const CDConvexMeshDesc* desc = (const CDConvexMeshDesc*)ptr;
	base = desc->vertices;

	CalcFace();
	CalcMetric();

	bboxReady = false;
}

bool CDConvexMesh::GetDesc(void *ptr) const {
	CDConvexMeshDesc* desc = (CDConvexMeshDesc*)ptr;
	desc->vertices = base;
	return CDShape::GetDesc(ptr);
}

bool CDConvexMesh::FindCutRing(CDCutRing& ring, const Posed& toW){
	Posed toL	= toW.Inv();
	//	頂点がどっち側にあるか調べる．
	Vec3d planePosL = toL * ring.local.Pos();
	Vec3d planeNormalL = toL.Ori() * ring.local.Ori() * Vec3d(1,0,0);
	std::vector<int> sign;
	sign.resize(base.size());
	double d = planeNormalL * planePosL;
	for(unsigned i=0; i<base.size(); ++i){
		double vtxDist = planeNormalL * base[i];
		if (vtxDist > d + epsilon) sign[i] = 1;
		else if (vtxDist < d - epsilon) sign[i] = -1;
		else sign[i] = 0;
	}
	bool rv = false;
	//	またがっている面の場合，交線を求める
	for(unsigned i=0; i<faces.size(); ++i){
		if (sign[faces[i].vtxs[0]] == sign[faces[i].vtxs[1]] && //	全部同じ側のときは，
			sign[faces[i].vtxs[0]] == sign[faces[i].vtxs[2]]) continue;	//	パス
		//	接触面(plane,面1)と多面体の面(face,面2)の交線を求める
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
			が面1(plane)と面2(face)が作る直線を通る1点
			O:		planePosL
			n1,d1	planeの法線(planeNormalL)，Oからの距離=0
			n2,d2	faceの法線(faceNormal)，Oからの距離			
		*/
		Vec3d faceNormal = ((base[faces[i].vtxs[1]] - base[faces[i].vtxs[0]]) ^ (base[faces[i].vtxs[2]] - base[faces[i].vtxs[0]])).unit();
		double faceDist = faceNormal * (base[faces[i].vtxs[0]] - planePosL);
		Vec3d lineDirection = (planeNormalL ^ faceNormal).unit();
		double ip = planeNormalL * faceNormal;
		if ((ip < 1.0-epsilon2) && (ip > -1.0+epsilon2)){	//	平行な面は無視
			double a = -faceDist*ip / (1.0-(ip*ip));
			double b = faceDist / (1.0-(ip*ip));
			Vec3d lineOff = a*planeNormalL + b*faceNormal;
			Vec3d lineNormal = planeNormalL ^ lineDirection;
			double lineDist = lineNormal * lineOff;
			if (finite(lineDist)) {	
				//	local -> world -> ring2次元系に変換
				Posed to2D = ring.localInv * toW;
				Vec2d lineNormal2D = (to2D.Ori() * lineNormal).sub_vector(1, Vec2d());
				assert(finite(lineNormal2D.x));
				assert(finite(lineNormal2D.y));

				//	線は内側を向かせたいので， normal, dist を反転して ring.lines に追加
				ring.lines.push_back(CDCutLine(-lineNormal2D, -lineDist));
				rv = true;
			}
		}
	}
	return rv;
}


class CDQhullVtx{
public:
	static Vec3f* base;
	int vtxID;
	int VtxID() const { return vtxID; }
	Vec3f GetPos() const { return base[VtxID()]; }
	CDQHPlane<CDQhullVtx>* horizon;
};

Vec3f* CDQhullVtx::base;

void CDConvexMesh::CalcFace(){
	maxSurfArea = 0;
	curPos = 0;
	faces.clear();
	neighbor.clear();
	
	//	baseの点から凸多面体を作る．
	std::vector<CDQhullVtx> vtxs;
	std::vector<CDQhullVtx*> pvtxs;
	vtxs.resize(base.size());
	pvtxs.resize(base.size());
	neighbor.resize(base.size());
	for(unsigned int i=0; i<base.size(); ++i){
		vtxs[i].vtxID = i;
		pvtxs[i] = &vtxs[i];
	}
	CDQhullVtx::base = &*base.begin();
	int n = (int)base.size();
//	CDQHPlanes<CDQhullVtx> planes(n*(n-1)*(n-2)/6);
	CDQHPlanes<CDQhullVtx> planes(n*10);
	planes.CreateConvexHull(&*pvtxs.begin(), &*pvtxs.begin() + pvtxs.size());
	std::set<int> usedVtxs;
	for(CDQHPlane<CDQhullVtx>* plane =planes.begin; plane != planes.end; ++plane){
		if (plane->deleted) continue;
		faces.push_back(CDFace());
		for(int i=0; i<3; ++i){
			faces.back().vtxs[i] = plane->vtx[i]->VtxID();
			usedVtxs.insert(plane->vtx[i]->VtxID());
		}
	}
	//	凸多面体に使われた頂点だけを列挙
	CDVertexIDs vtxIds;
	for(std::set<int>::iterator it = usedVtxs.begin(); it != usedVtxs.end(); ++it){
		vtxIds.push_back(*it);
	}
	//	baseから不要な頂点を削除
	int pos = (int)base.size()-1;
	int i = (int)vtxIds.size()-1;
	while(true){
		while(pos >= 0 && (i < 0 || pos>vtxIds[i])){
			base.erase(base.begin()+pos);
			 --pos;
		}
		if(pos < 0)break;
		--i;
		--pos;
	}

	// 平均座標の計算
	CalcAverage();

	//	面の頂点IDを振りなおす / 法線を計算
	for(CDFaces::iterator it = faces.begin(); it != faces.end(); ++it){
		for(int i=0; i<3; ++i){
			it->vtxs[i] = vtxIds.FindPos(it->vtxs[i]);
		}
		it->normal = (base[it->vtxs[2]] - base[it->vtxs[0]]) % (base[it->vtxs[1]] - base[it->vtxs[0]]);
		it->normal.unitize();
		if(it->normal * (base[it->vtxs[0]] - average) < 0.0f)
			it->normal *= -1.0f;
		float area = ((base[it->vtxs[2]] - base[it->vtxs[0]]) % (base[it->vtxs[1]] - base[it->vtxs[0]])).norm()/2.f;
		if (maxSurfArea < area) maxSurfArea = area;
	}
	//	隣の頂点リストを作る．(GJKのSupportに使用)
	neighbor.resize(vtxIds.size());
	for(CDFaces::iterator it = faces.begin(); it != faces.end(); ++it){
		//	各辺は2つの面に逆向きに使われるので，全部の面を巡回すると，
		//	ちょうど隣の頂点リストが完成する．
		for(int i=0; i<3; ++i){
			int pos = it->vtxs[i];
			int next = it->vtxs[(i+1)%3];
			neighbor[pos].push_back(next);
		}
	}

	//	凸多面体の面のうち，半平面表現に必要な面だけを前半に集める．
	MergeFace();
}

void CDConvexMesh::MergeFace(){
	//int nf = faces.size();
	CDFaces erased;
	for(unsigned int i=0; i<faces.size(); ++i){
		for(unsigned int j=i+1; j<faces.size(); ++j){
			CDFace& a = faces[i];
			CDFace& b = faces[j];
			Vec3f& pa = base[a.vtxs[0]];
			Vec3f& pb = base[b.vtxs[0]];
			//Vec3f na =  ((base[a.vtxs[2]] - pa) ^ (base[a.vtxs[1]] - pa)).unit();
			//Vec3f nb =  ((base[b.vtxs[2]] - pb) ^ (base[b.vtxs[1]] - pb)).unit();
			float len;
			//len = pa*na - pb*na;
			len = (pa - pb) * a.normal;
			if (/*na*nb*/a.normal * b.normal > 0.998f && (len>0?len:-len) < 1e-5f){
				erased.push_back(faces[i]);
				faces.erase(faces.begin() + i);
				i--;
				break;
			}
		}
	}
	nPlanes = (int)faces.size();
	faces.insert(faces.end(), erased.begin(), erased.end());
	//	DSTR << "Poly faces:" << nf << "->" << faces.size() << std::endl;
}

void CDConvexMesh::CalcAverage(){
	average.clear();
	for(unsigned i=0; i<base.size(); ++i){
		average += base[i];
	}
	average /= base.size();
}

float CDConvexMesh::CalcVolume(){
	return volume;
}

Vec3f CDConvexMesh::CalcCenterOfMass(){
	return center;
}

Matrix3f CDConvexMesh::CalcMomentOfInertia(){
	return inertia;
}

void CDConvexMesh::CalcMetric(){
	volume  = 0.0f;
	center  = Vec3f();
	inertia = Matrix3f::Zero();

	// 各面と原点からなる四面体の体積，重心，慣性行列を計算
	for(int i = 0; i < nPlanes; i++){
		CDFace& f = faces[i];
		Vec3f& v0 = base[f.vtxs[0]];
		Vec3f& v1 = base[f.vtxs[1]];
		Vec3f& v2 = base[f.vtxs[2]];
		float v = CDShape::CalcTetrahedronVolume(v0, v1, v2);
		float sign = (f.normal * v0 > 0.0f ? 1.0f : -1.0f);
		volume  += sign * v;
		center  += sign * v * CDShape::CalcTetrahedronCoM(v0, v1, v2);
		inertia += sign * CDShape::CalcTetrahedronInertia(v0, v1, v2);
	}
		
	// 重心は体積による重み平均
	center /= volume;

	// 慣性行列を原点基準から重心基準へ変換
	Matrix3f cross = Matrix3f::Cross(center);
	Matrix3f cross2 = cross*cross;
	inertia += volume * (cross2);
}

bool CDConvexMesh::IsInside(const Vec3f& p){
	for(int i = 0; i < nPlanes; i++){
		if(faces[i].normal * (base[faces[i].vtxs[0]] - p) <= 0.0f)
			return false;
	}
	return true;
}

int CDConvexMesh::Support(Vec3f& w, const Vec3f& v) const {
	int lastPos = -1;
	float h = base[curPos] * v;
	float d=0;
	int count = 0;
	while (1) {
		const std::vector<int>& curNeighbor = neighbor[curPos];
		int i = 0;
		int n = (int)curNeighbor.size();
		while(i!=n){
			++ count;
			if (curNeighbor[i] == lastPos){
				++i;
			}else{
				d = base[curNeighbor[i]]*v;
				if (count > 1000){	//hase	この処理をなくすと，VC7.1では，最適化がおかしくなって，無限ループになる．なぞ．
					DSTR << "d:" << d << " h:" << h;
					DSTR << " CN:" << curNeighbor[i] << " i:" <<i << " n:" << n << std::endl;
				}
				if (d>h) break;
				++i;
			}
		}
		if (i == n) break;
		lastPos = curPos;
		curPos = curNeighbor[i];
		h = d;
	}
	w = base[curPos];
	return curPos;
}
std::vector<int>& CDConvexMesh::FindNeighbors(int vtx){
	return neighbor[vtx];
}

CDFaceIf* CDConvexMesh::GetFace(int i){
	return faces.at(i).Cast();
}
int CDConvexMesh::NFace(){
	return (int)faces.size();
}
Vec3f* CDConvexMesh::GetVertices(){
	return &*base.begin();
}
int CDConvexMesh::NVertex(){
	return (int)base.size();
}

int CDConvexMesh::LineIntersect(const Vec3f& origin, const Vec3f& dir, Vec3f* result, float* offset){
	// 全ての面との交差を調べる安易な実装
	const float eps = 1.0e-10f;
	Matrix2f A;
	Vec2f b, x;
	Vec3f p, u1, u2, diff;
	int num = 0;

	for(int i = 0; i < nPlanes; i++){
		const CDFace& f = faces[i];
		const Vec3f& n = f.normal;
		float tmp = n * dir;
		if(abs(tmp) < eps)
			continue;
		// 直線と面の交点p
		float s = ((base[f.vtxs[0]] - origin) * n) / tmp;
		if(s < 0.0)
			continue;
		p = origin + dir * s;
		// 3角形の内部にあるか
		u1 = base[f.vtxs[1]] - base[f.vtxs[0]];
		u2 = base[f.vtxs[2]] - base[f.vtxs[0]];
		A[0][0] = u1.square();
		A[0][1] = A[1][0] = u1 * u2;
		A[1][1] = u2.square();
		if(A.det() < eps)
			continue;
		diff = p - base[f.vtxs[0]];
		b[0] = diff * u1;
		b[1] = diff * u2;
		x = A.inv() * b;	// 2次元だしいいか
		if(0.0 <= x[0] && x[0] <= 1.0 && 0.0 <= x[1] && x[1] <= 1.0 && x[0] + x[1] <= 1.0){
			result[num] = p;
			offset[num] = s;
			num++;
		}
		if(num == 2)		// 理屈上は3つ以上はあり得ないが念のため
			break;
	}
	return num;
}
void CDConvexMesh::Print(std::ostream& os) const{
	PrintHeader(os, false);
	int w = os.width();
	os.width(0);

	os << UTPadding(w+2) << "vtxs: " << base.size() << std::endl;
	os << UTPadding(w+2) << "faces:" << faces.size() << std::endl;

	os.width(w);
	PrintFooter(os);
}

}
