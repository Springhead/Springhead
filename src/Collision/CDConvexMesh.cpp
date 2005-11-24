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

void CDFace::Print(std::ostream& os) const {
	os << normal << "*" << dist;
}
Vec3f CDFace::CommonVtx(int i){
	/**car2.xでdualPlanes[i]->distが0.000000000になる**/
	double dist = dualPlanes[i]->dist;
#ifdef _DEBUG
	if (dist < CD_EPSILON){
		DSTR << "Error: dist " << dist << std::endl;
		DSTR << "normal:" << dualPlanes[i]->normal << std::endl;
		Vec3d posAv;
		for(int j=0; j<3; ++j){
			DSTR << "Point" << j << ":" << dualPlanes[i]->vtx[j]->GetPos() << std::endl;
			posAv += dualPlanes[i]->vtx[j]->GetPos();
		}
		DSTR << "PosAve:" << posAv << std::endl;
		Vec3d pos0 = dualPlanes[i]->vtx[0]->GetPos();
		Vec3d pos1 = dualPlanes[i]->vtx[1]->GetPos();
		Vec3d pos2 = dualPlanes[i]->vtx[2]->GetPos();
		Vec3d a = pos1 - pos0;
		Vec3d b = pos2 - pos0;
		DSTR << "a:" << a << "b:" << b << std::endl;
		Vec3d normal = a ^ b;
		assert(normal.norm());
		normal.unitize();
		dist = pos0 * normal;
	}
#endif
	return Vec3f(dualPlanes[i]->normal / dist);
}

bool CDFace::CalcDualVtx(Vec3f* base){
	normal = (base[vtxs[1]] - base[vtxs[0]]) ^ (base[vtxs[2]] - base[vtxs[0]]);
	DEBUG_EVAL( if ( normal.norm() < CD_EPSILON || !_finite(normal.norm()) ){ DSTR << "normal is too small." << std::endl; } )
	normal.unitize();
	dist = normal * base[vtxs[0]];
#if 1	//	debug	hase
	if (dist < -1e-3){
		DSTR << "Error: distance=" << dist << " < 0" << std::endl;
		return false; 
	}
#endif
	if (dist < CD_EPSILON) dist = CD_EPSILON;
	if (dist > CD_INFINITE) dist = CD_INFINITE;
	return true;
}



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

CDConvexMesh::CDConvexMesh(){
}
CDConvexMesh::CDConvexMesh(const CDConvexMeshDesc& desc){
	base = desc.vertices;
	for(unsigned i=0; i<base.size(); ++i) vtxIDs.push_back(i);
	CalcFace();
}
void CDConvexMesh::Print(std::ostream& os) const {
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


}
