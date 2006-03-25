#include <Springhead.h>
#include <Foundation/Object.h>
#include "Collision.h"
#include <set>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include "CDQuickHull3DImp.h"

#define CD_EPSILON	HULL_EPSILON
#define CD_INFINITE	HULL_INFINITE


namespace Spr{;


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
OBJECTIMP(CDConvexMesh, CDConvex);
IF_IMP(CDConvexMesh, CDConvex);

CDConvexMesh::CDConvexMesh(){
}
CDConvexMesh::CDConvexMesh(const CDConvexMeshDesc& desc){
	material = desc.material;
	base = desc.vertices;
	CalcFace();
}

void CDConvexMesh::FindCutRing(CDCutRing& r, const Affined& toW){	
#if 0
	Affined toL	= toW.inv();
	//	頂点がどっち側にあるか調べる．
	for(
	//	またがっている面の場合，交線を求める
	r.center
	
	r.local_inv *
#endif
}



void CalcBBox(Vec3f& bbmin, Vec3f& bbmax){
	
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
	int n = base.size();
//	CDQHPlanes<CDQhullVtx> planes(n*(n-1)*(n-2)/6);
	CDQHPlanes<CDQhullVtx> planes(n*10);
	planes.CreateConvexHull(&*pvtxs.begin(), &*pvtxs.end());
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
	int pos = base.size()-1;
	for(int i=vtxIds.size()-1; i>=0; --i){
		for(; pos>vtxIds[i] && pos>=0; --pos){
			base.erase(base.begin()+pos);
		}
		--pos;
	}
	//	面の頂点IDを振りなおす
	for(CDFaces::iterator it = faces.begin(); it != faces.end(); ++it){
		for(int i=0; i<3; ++i){
			it->vtxs[i] = vtxIds.FindPos(it->vtxs[i]);
		}
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
			Vec3f na =  ((base[a.vtxs[2]] - pa) ^ (base[a.vtxs[1]] - pa)).unit();
			Vec3f nb =  ((base[b.vtxs[2]] - pb) ^ (base[b.vtxs[1]] - pb)).unit();
			float len;
			len = pa*na - pb*na;
			if (na*nb > 0.998f && (len>0?len:-len) < 1e-5f){
				erased.push_back(faces[i]);
				faces.erase(faces.begin() + i);
				i--;
				break;
			}
		}
	}
	nPlanes = faces.size();
	faces.insert(faces.end(), erased.begin(), erased.end());
	//	DSTR << "Poly faces:" << nf << "->" << faces.size() << std::endl;
}
Vec3f CDConvexMesh::Support(const Vec3f& v) const {
	int lastPos = -1;
	float h = base[curPos] * v;
	float d=0;
	int count = 0;
	while (1) {
		const std::vector<int>& curNeighbor = neighbor[curPos];
		int i = 0;
		int n = curNeighbor.size();
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
	return base[curPos];
}

CDFaceIf* CDConvexMesh::GetFace(size_t i){
	return &faces.at(i);
}
size_t CDConvexMesh::NFace(){
	return faces.size();
}
Vec3f* CDConvexMesh::GetVertices(){
	return &*base.begin();
}
size_t CDConvexMesh::NVertex(){
	return base.size();
}

}
