/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <GL/glut.h>

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDebugRender
IF_OBJECT_IMP(GRDebugRender, GRRender);

/// コンストラクタ
GRDebugRender::GRDebugRender(){
	matSampleCount = 24;		// 24種類
	matSample.resize(matSampleCount);
	std::vector<GRMaterialDesc>::iterator itr;
	itr = matSample.begin();
	(itr++)->diffuse = Vec4f(1.0, 0.0, 0.0, 1.0);			// red
	(itr++)->diffuse = Vec4f(0.0, 1.0, 0.0, 1.0);			// green
	(itr++)->diffuse = Vec4f(0.0, 0.0, 1.0, 1.0);			// blue
	(itr++)->diffuse = Vec4f(1.0, 1.0, 0.0, 1.0);			// yellow
	(itr++)->diffuse = Vec4f(0.0, 1.0, 1.0, 1.0);			// cyan
	(itr++)->diffuse = Vec4f(1.0, 0.0, 1.0, 1.0);			// magenta
	(itr++)->diffuse = Vec4f(1.0, 1.0, 1.0, 1.0);			// white
	(itr++)->diffuse = Vec4f(0.5, 0.5, 0.5, 1.0);			// gray
	(itr++)->diffuse = Vec4f(1.0, 0.27, 0.0, 1.0);			// orange
	(itr++)->diffuse = Vec4f(0.198, 0.0, 0.0, 1.0);			// blown
	(itr++)->diffuse = Vec4f(0.676, 0.844, 0.898, 1.0);		// light blue
	(itr++)->diffuse = Vec4f(0.574, 0.438, 0.855, 1.0);		// midium purple
	(itr++)->diffuse = Vec4f(0.0, 0.391, 0.0, 1.0);			// dark green
	(itr++)->diffuse = Vec4f(0.578, 0.0, 0.824, 1.0);		// dark violet
	(itr++)->diffuse = Vec4f(0.0, 0.543, 0.543, 1.0);		// dark cyan
	(itr++)->diffuse = Vec4f(0.676, 1.0, 0.184, 1.0);		// green yellow
	(itr++)->diffuse = Vec4f(0.195, 0.801, 0.195, 1.0);		// lime green
	(itr++)->diffuse = Vec4f(0.801, 0.359, 0.359, 1.0);		// indian red
	(itr++)->diffuse = Vec4f(0.293, 0.0, 0.508, 1.0);		// indigo
	(itr++)->diffuse = Vec4f(0.0, 0.198, 0.198, 1.0);		// green indigo
	(itr++)->diffuse = Vec4f(0.198, 0.398, 0.0, 1.0);		// olive green
	(itr++)->diffuse = Vec4f(0.198, 0.398, 0.797, 1.0);		// navy blue
	(itr++)->diffuse = Vec4f(0.398, 1.0, 0.797, 1.0);		// turquoise blue
	(itr++)->diffuse = Vec4f(0.598, 1.0, 0.398, 1.0);		// emerald green
	for(std::vector<GRMaterialDesc>::iterator it=matSample.begin(); it!= matSample.end(); ++it){
		it->diffuse += Vec4f(0.5,0.5,0.5,1);
		it->diffuse /= 2;
	}
}

/// シーン内の全てのオブジェクトをレンダリングする
void GRDebugRender::DrawScene(PHSceneIf* scene){
	PHSolidIf **solids = scene->GetSolids();
	for (int num=0; num < scene->NSolids(); ++num){
		this->SetMaterialSample((MaterialSample)num);
		this->DrawSolid(solids[num]);
	}	
}

/// 剛体をレンダリングする
void GRDebugRender::DrawSolid(PHSolidIf* so){
	Affinef soaf;
	so->GetPose().ToAffine(soaf);
	this->PushModelMatrix();
	this->MultModelMatrix(soaf);
	
	for(int s=0; s<so->NShape(); ++s){
		CDShapeIf* shape = so->GetShape(s);

		Affinef af;
		so->GetShapePose(s).ToAffine(af);
		this->PushModelMatrix();
		this->MultModelMatrix(af);
		
		CDConvexMeshIf* mesh = DCAST(CDConvexMeshIf, shape);
		if (mesh){
			Vec3f* base = mesh->GetVertices();
			for (size_t f=0; f<mesh->NFace(); ++f) {	
				CDFaceIf* face = mesh->GetFace(f);
				this->DrawFace(face, base);
			}
		}
		CDSphereIf* sphere = DCAST(CDSphereIf, shape);
		if (sphere){
			float r = sphere->GetRadius();
			glutSolidSphere(r, 16, 16);
		}
		CDCapsuleIf* cap = DCAST(CDCapsuleIf, shape);
		if (cap){
			float r = cap->GetRadius();
			glPushMatrix();
			glTranslatef(0,0,-cap->GetLength()/2);
			glutSolidSphere(r, 20, 20);
			glTranslatef(0,0,cap->GetLength());
			glutSolidSphere(r, 20, 20);
			glPopMatrix();
		}
		CDBoxIf* box = DCAST(CDBoxIf, shape);
		if (box){
			Vec3f boxsize = box->GetBoxSize();
			glScalef(boxsize.x, boxsize.y, boxsize.z);	
			glutSolidCube(1.0);		
		}
		this->PopModelMatrix();
	}

	this->PopModelMatrix();
}
		
/// 面をレンダリングする
void GRDebugRender::DrawFace(CDFaceIf* face, Vec3f * base){
	int numIndices = face->NIndex();			// (=3 :三角形なので3頂点)
	struct Vtx{
		Vec3f n;
		Vec3f p;
	} vtxs[10];
	assert(numIndices < sizeof(vtxs)/sizeof(vtxs[0]));
	for (int v=0; v<numIndices; ++v) vtxs[v].p = base[face->GetIndices()[v]].data;
	Vec3f edge0, edge1;
	edge0 = vtxs[1].p - vtxs[0].p;
	edge1 = vtxs[2].p - vtxs[0].p;
	Vec3f n = (edge0^edge1).unit();
	for(int v=0; v<numIndices; ++v) vtxs[v].n = n;
	SetVertexFormat(GRVertexElement::vfN3fP3f);
	DrawDirect(TRIANGLE_FAN, vtxs, numIndices);
}

void GRDebugRender::SetMaterialSample(MaterialSample matname){
	int matID = 0;
	if (matname > EMERALD_GREEN) {
		matID = (int)matname;
		matID = matID % matSampleCount;
	} else {
		matID = (int)matname;
	}
	this->SetMaterial(matSample[matID]);
}

}	//	Spr

