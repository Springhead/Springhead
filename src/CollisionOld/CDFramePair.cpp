#include "Collision.h"
#pragma hdrstop

#define NO_DEBUG_EVAL	//	DEBUG_EVALの無効化
#include "CDQuickHull3DImp.h"
#include <float.h>
#pragma hdrstop


namespace Spr {;

CDFramePair::CDFramePair():lastContactCount(-2){
}
void CDFramePair::Set(CDFrame* f1, CDFrame* f2){
	geometryPairs.clear();
	frame[0] = f1;
	frame[1] = f2;
	
	CDGeometries geoms[2];
	SGFrames frames[2];
	EnumGeometry(frames[0], geoms[0], f1->frame);
	EnumGeometry(frames[1], geoms[1], f2->frame);
	geometryPairs.resize(geoms[0].size(), geoms[1].size());
	for(unsigned int i=0; i<geoms[0].size(); ++i){
		for(unsigned int j=0; j<geoms[1].size(); ++j){
			geometryPairs.item(i, j) = CreateGeometryPair();
			geometryPairs.item(i, j)->Set(this, geoms[0][i], frames[0][i], geoms[1][j], frames[1][j]);
		}
	}
}
void CDFramePair::EnumGeometry(SGFrames& frames, CDGeometries& geoms, SGFrame* f){
	for(SGObjects::iterator g = f->contents.begin(); g != f->contents.end(); ++g){
		if (DCAST(CDGeometry, *g)){
			geoms.push_back(*g);
			frames.push_back(f);
		}
	}
	for(unsigned int i=0; i<f->Children().size(); i++){
		EnumGeometry(frames, geoms, (SGFrame*)f->Children()[i]);
	}
}
bool CDFramePair::Detect(int ct){
	intersections.clear();
	bool rv = false;
//	9.14	BBoxIntersectionは，中心から外れているときに，うまく動作しないようだ．
//	2005.7.28直ったかも．
//	if (BBoxIntersection(frame[0]->frame->GetPosture(), frame[0]->frame->bbox.GetBBoxCenter(), frame[0]->frame->bbox.GetBBoxExtent(),
//		frame[1]->frame->GetPosture(), frame[1]->frame->bbox.GetBBoxCenter(), frame[1]->frame->bbox.GetBBoxExtent()) ){
		for(CDGeometryPairs::iterator it = geometryPairs.begin(); it != geometryPairs.end(); ++it){
			CDGeometryPair& gp = **it;
			SGFrame* fr[2];
			for(int i=0; i<2; ++i){
				fr[i] = gp.frame[i];
				gp.posture[i] = fr[i]->GetPosture();
				if (!_finite(gp.posture[i].Pos().norm())){
					DSTR << "Strange Posture:"  << gp.frame[0]->GetName() << " - " << gp.frame[1]->GetName();
					DSTR <<  gp.posture[i] << std::endl;
				}
				while (fr[i] != frame[i]->frame){
					fr[i] = fr[i]->GetParent();
					gp.posture[i] = fr[i]->GetPosture() * gp.posture[i];
				}
			}
			for(CDConvexPairs::iterator itc = gp.convexPairs.begin(); itc!= gp.convexPairs.end(); ++itc){
				CDConvexPair& cp = **itc;
				Vec3d sep;
				bool r = FindCommonPoint(*cp.convex[0], *cp.convex[1],
					gp.posture[0], gp.posture[1], sep, cp.closestPoint[0], cp.closestPoint[1]);
				if (r){
					cp.commonPoint = gp.posture[0] * cp.closestPoint[0];
					rv = true;
					if (cp.lastContactCount == unsigned(ct-1)){
						cp.state = cp.CONTINUE;
					}else{
						cp.state = cp.NEW;
					}
					cp.lastContactCount = ct;
					Found(cp, gp);
				}
			}
		}
//	}
	if (rv) lastContactCount = ct;
	return rv;
}
bool CDFramePair::IsContact(SGScene* scene){
	if ((unsigned)lastContactCount == scene->GetCount()) return true;
	return false;
}


void CDGeometryPair::Set(CDFramePair* fp, CDGeometry* g1, SGFrame* f1, CDGeometry* g2, SGFrame* f2){
	geometry[0] = g1;
	geometry[1] = g2;
	frame[0] = f1;
	frame[1] = f2;
	CDGeometries conveces[2];
	for(int i=0; i<2; ++i){
		CDMesh* mesh = DCAST(CDMesh, geometry[i]);
		if (mesh){
			mesh->CreateTree();
			conveces[i] = mesh->conveces;
			continue;
		}
		CDConvex* conv = DCAST(CDConvex, geometry[i]);
		if (conv){
			conveces[i].push_back(conv);
			continue;
		}
	}
	convexPairs.resize(conveces[0].size(), conveces[1].size());
	for(unsigned int i=0; i < conveces[0].size(); ++i){
		for(unsigned int j=0; j < conveces[1].size(); ++j){
			convexPairs.item(i, j) = fp->CreateConvexPair();
			convexPairs.item(i, j)->Set((CDConvex*)&*conveces[0][i], (CDConvex*)&*conveces[1][j]);
		}
	}
}

}
