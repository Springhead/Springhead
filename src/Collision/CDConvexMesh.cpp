#include <Springhead.h>
#include <Foundation/Object.h>
#include "Collision.h"
#include <set>
#include "Collision.h"

#pragma hdrstop
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
	for(unsigned i=0; i<base.size(); ++i) vtxIDs.push_back(i);
	CalcFace();
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
	
	std::vector<CDQhullVtx> vtxs;
	std::vector<CDQhullVtx*> pvtxs;
	vtxs.resize(vtxIDs.size());
	pvtxs.resize(vtxIDs.size());
	neighbor.resize(vtxIDs.size());
	for(unsigned int i=0; i<vtxIDs.size(); ++i){
		vtxs[i].vtxID = vtxIDs[i];
		pvtxs[i] = &vtxs[i];
	}
	CDQhullVtx::base = &*base.begin();
	int n = vtxIDs.size();
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
	vtxIDs.clear();
	for(std::set<int>::iterator it = usedVtxs.begin(); it != usedVtxs.end(); ++it){
		vtxIDs.push_back(*it);
	}
	neighbor.resize(vtxIDs.size());
	for(CDFaces::iterator it = faces.begin(); it != faces.end(); ++it){
		for(int i=0; i<3; ++i){
			int pos = vtxIDs.FindPos(it->vtxs[i]);
			int next = vtxIDs.FindPos(it->vtxs[(i+1)%3]);
			neighbor[pos].push_back(next);
		}
	}
	MergeFace();
}

bool CDConvexMesh::VertexNear(int v1, int v2) const{
	if ((base[v1]-base[v2]).norm() < 1e-8) return true;
	return false;
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
	float h = Vertex(curPos) * v;
	float d;
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
				d = Vertex(curNeighbor[i])*v;
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
	return Vertex(curPos);
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
