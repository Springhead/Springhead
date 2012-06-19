/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "GraphicsD3D.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr {;
//----------------------------------------------------------------------------
//	GRDebugRenderD3D
IF_SPR_OBJECTIMP(GRDebugRenderD3D, GRDebugRender);

/// シーン内の全てのオブジェクトをレンダリングする
void GRDebugRenderD3D::DrawScene(PHSceneIf* scene){
	PHSolidIf **solids = scene->GetSolids();
	for (int num=0; num < scene->NSolids(); ++num){
		this->SetMaterialSample((GRDebugRenderIf::TMaterialSample)num);
		this->DrawSolid(solids[num]);
	}	
}

/// 剛体をレンダリングする
void GRDebugRenderD3D::DrawSolid(PHSolidIf* so){
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
			static CComPtr<ID3DXMesh> mesh;
			if(!mesh) D3DXCreateSphere(GRDeviceD3D::GetD3DDevice(), 1, 16, 16, &mesh, NULL);
			float r = sphere->GetRadius();
			this->MultModelMatrix(Affinef::Scale(r, r, r));
			mesh->DrawSubset(0);
		}
		CDCapsuleIf* cap = DCAST(CDCapsuleIf, shape);
		if (cap){
			/*
			float r = cap->GetRadius();
			this->PushModelMatrix();
			glTranslatef(0,0,-cap->GetLength()/2);
			glutSolidSphere(r, 20, 20);
			glTranslatef(0,0,cap->GetLength());
			glutSolidSphere(r, 20, 20);
			this->PopModelMatrix();
			*/
			DSTR << "GRDebugRenderD3D: Rendering CDCapsuleIf is not implemented." << std::endl;
		}
		CDBoxIf* box = DCAST(CDBoxIf, shape);
		if (box){
			static CComPtr<ID3DXMesh> mesh;
			if(!mesh) D3DXCreateBox(GRDeviceD3D::GetD3DDevice(), 1, 1, 1, &mesh, NULL);
			Vec3f boxsize = box->GetBoxSize();
			this->MultModelMatrix(Affinef::Scale(boxsize.x, boxsize.y, boxsize.z));
			mesh->DrawSubset(0);
		}
		this->PopModelMatrix();
	}

	this->PopModelMatrix();
}

}	//	Spr

