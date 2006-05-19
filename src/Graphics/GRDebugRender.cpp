#include "Graphics.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif
#include <GL/glut.h>

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDebugRender
OBJECTIMP(GRDebugRender, GRRender);
IF_IMP(GRDebugRender, GRRender);

/// コンストラクタ
GRDebugRender::GRDebugRender(){
	matSample.resize(24);
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
}

///	Viewportと射影行列を設定
void GRDebugRender::Reshape(Vec2f screen){
	glViewport(0, 0, static_cast<GLsizei>(screen.x), static_cast<GLsizei>(screen.y));
	Affinef afProj = Affinef::ProjectionGL(Vec3f(camera.center.x, camera.center.y, camera.size.y/2.0), // 視野角90度
											Vec2f(camera.size.x, camera.size.x*screen.y/screen.x), 
											camera.front, camera.back);
	SetProjectionMatrix(afProj);
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
	
	CDShapeIf** shapes = so->GetShapes();
	for(int s=0; s<so->NShape(); ++s){
		Affinef af;
		shapes[s]->GetPose().ToAffine(af);
		this->PushModelMatrix();
		this->MultModelMatrix(af);
		
		CDConvexMeshIf* mesh = ICAST(CDConvexMeshIf, shapes[s]);
		if (mesh){
			Vec3f* base = mesh->GetVertices();
			for (size_t f=0; f<mesh->NFace(); ++f) {	
				CDFaceIf* face = mesh->GetFace(f);
				this->DrawFace(face, base);
			}
		}
		CDSphereIf* sphere = ICAST(CDSphereIf, shapes[s]);
		if (sphere){
			float r = sphere->GetRadius();
			glutSolidSphere(r, 20, 20);
		}
		CDBoxIf* box = ICAST(CDBoxIf, shapes[s]);
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
	Vec3f *vtx = new Vec3f[numIndices];
#if 1
	// DrawDirect版
	for (int v=0; v<numIndices; ++v)
		vtx[v] = base[face->GetIndices()[v]].data;
	Vec3f normal, edge0, edge1;
	edge0 = vtx[1] - vtx[0];
	edge1 = vtx[2] - vtx[0];
	normal = edge0^edge1;
	normal.unitize();
	glNormal3fv(normal);
	DrawDirect(TRIANGLES, vtx, vtx + numIndices);
#else
	// DrawIndexed版
	size_t *vtxIndex = new size_t[numIndices];
	for (int v=0; v<numIndices; ++v) {
		vtx[v] = base[face->GetIndices()[v]].data;
		vtxIndex[v] = v;
	}
	DrawIndexed(TRIANGLES, vtxIndex, vtxIndex + numIndices, vtx);
	
	delete[] vtxIndex;
#endif
	delete[] vtx;
}

void GRDebugRender::SetMaterialSample(MaterialSample matname){
	this->SetMaterial(matSample[matname]);
}

}	//	Spr

