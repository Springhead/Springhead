#include "Collision.h"
#pragma hdrstop

namespace Spr {;

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

	
//------------------------------------------------------------------------
SGOBJECTIMP(CDPolyhedron, CDConvex);
Vec3f CDPolyhedron::Support(const Vec3f& v) const {
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
