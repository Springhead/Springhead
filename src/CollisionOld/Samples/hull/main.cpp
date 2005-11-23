#include "../CDQuickHull3DImp.h"
#include <iostream>

using namespace Spr;

void main(){
	int id = 0;
	std::vector<TVtx> vtxBufs;
	vtxBufs.push_back(TVtx());
	vtxBufs.back().SetPos(Vec3f(0,0,1e8));
	vtxBufs.back().id_ = id++;
	for(float theta=0; theta < 2*M_PI-0.0001; theta += 2*M_PI/10){
		vtxBufs.push_back(TVtx());
		vtxBufs.back().SetPos(Vec3f(cos(theta), sin(theta),0));
		vtxBufs.back().id_ = id++;
	}

	for(float theta=0; theta < 0.5*M_PI-0.0001; theta += 2*M_PI/10){
		vtxBufs.push_back(TVtx());
		vtxBufs.back().SetPos(1e-8*Vec3f(cos(theta), sin(theta),0));
		vtxBufs.back().id_ = id++;
	}


	
	vtxBufs.push_back(TVtx());
	vtxBufs.back().SetPos(Vec3f(0,0,-1));
	vtxBufs.back().id_ = id++;

	for(float theta=0; theta < 2*M_PI-0.0001; theta += 2*M_PI/1000){
		vtxBufs.push_back(TVtx());
		vtxBufs.back().SetPos( Vec3f(cos(theta),sin(theta), 0) * 0.1f );
		vtxBufs.back().id_ = id++;
	}


	CDQHPlanes<TVtx>::TVtxs vtxs;
	for(int i=0; i<vtxBufs.size(); ++i){
		vtxs.push_back(vtxBufs.begin()+i);
	}

	CDQHPlanes<TVtx> planes(1000);
	planes.CreateConvexHull(vtxs.begin(), vtxs.end());
	//	‚Å‚«‚½–Ê‚ð•\Ž¦
	std::cout << planes;
}
