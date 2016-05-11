/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/FWObject.h>
#include <Framework/FWScene.h>
#include <Framework/FWSdk.h>
#include <Physics/PHConstraint.h>
#include <Physics/PHSdk.h>
#include <Graphics/GRFrame.h>
#include <Graphics/GRMesh.h>
#include <FileIO/FISdk.h>
#include <FileIO/FIFileX.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

// --- --- --- --- --- --- --- --- --- ---
// FWObject

FWObject::FWObject(const FWObjectDesc& d)
: phSolid(NULL), grFrame(NULL), phJoint(NULL), childFrame(NULL), FWObjectDesc(d){
}

SceneObjectIf* FWObject::CloneObject(){
	FWObjectIf* origin = DCAST(FWObjectIf,this);
	FWSceneIf* s = DCAST(FWSceneIf, GetScene());
	FWObjectIf* clone = s->CreateFWObject();

	// Boneに相当するFWObjectの複製には未対応 <!!>

	if(origin->GetPHSolid())
		clone->SetPHSolid(origin->GetPHSolid()->CloneObject()->Cast());
	if(origin->GetGRFrame())
		clone->SetGRFrame(origin->GetGRFrame()->CloneObject()->Cast());

	return clone;
}

void FWObject::Sync(){
	if(!phSolid || !grFrame){
		return;
	}

	if (phJoint==NULL || bAbsolute) {
		if(syncSource==FWObjectDesc::PHYSICS){
			// 剛体をフレームへ
			Affinef aff;
			phSolid->GetPose().ToAffine(aff);
			grFrame->SetTransform(aff);

		} else {
			// フレームを剛体へ
			Affinef af;
			af = grFrame->GetTransform();

			// ボーンのルートフレームだった場合。必要性が不明のため残しとくが、後ほど検証のこと (12/05/17, mitake) <!!>
			// af = grFrame->GetWorldTransform();
			// af.Orthonormalization();

			Posed pose; pose.FromAffine(af);
			phSolid->SetPose(pose);
		}

	} else {
		if(syncSource==FWObjectDesc::PHYSICS){
			// 関節をフレームへ
			Posed jointPosition;
			jointPosition.Ori() = phJoint->GetRelativePoseQ() * sockOffset.Ori().Inv();
			Posed poseSocket; phJoint->GetSocketPose(poseSocket);
			Posed pose = poseSocket * jointPosition;

			Affinef af; pose.ToAffine(af);
			DCAST(GRFrame, grFrame)->SetTransform(af);

			PHSolidIf *so1 = phJoint->GetSocketSolid(), *so2 = phJoint->GetPlugSolid();
			if (so1 && so2) {
				DCAST(FWSceneIf,GetScene())->GetPHScene()->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE);
			}

		} else {
			// フレームを関節（を構成する剛体）へ
			Affinef af = grFrame->GetWorldTransform();
			af.Orthonormalization(); //正規直交化
			Posed pose; pose.FromAffine(af);

			//アフィン行列→クォータニオンの変換誤差が大きい場合のエラー表示
			Affinef af2; pose.ToAffine(af2);
			Matrix3d mat=af.Rot(), mat2=af2.Rot();
			double epsilon = 0.1;
			bool   bErr    = false;
			for(int i=0;i<2;i++){
				for(int j=0;j<2;j++){
					if(fabs(mat[i][j]-mat2[i][j]) > epsilon){ bErr = true; }
				}
			}
			if(bErr){
				DSTR << "in FWObject[" << GetName() << "] : ";
				DSTR << mat << " <=> " << mat2 << " has error larger than " << epsilon << std::endl;
			}
			
			phSolid->SetPose(pose);
		}
	}
}

void FWObject::Sync2(){ //小野原追加 デバック用（肉を別表示）
	if(!phSolid || !grFrame){
		return;
	}

	if (phJoint==NULL || bAbsolute) {
		if(syncSource==FWObjectDesc::PHYSICS){
			// 剛体をフレームへ
			Affinef aff;
			phSolid->GetPose().ToAffine(aff);
			grFrame->SetTransform(aff);
			//Affinef trn = Affinef::Trn(0.0, -0.01, -0.1);
			//Affinef rot = Affinef::Rot(M_PI, 'x');
			//grFrame->SetTransform(trn*rot);
#if 0
			grFrame->SetTransform(Affinef::Trn(0.0, 0.05, 0.0));
#else
			aff = Affinef::Trn(0.0f, 0.05f, -0.03f)* Affinef::Rot((float)(-M_PI /2.0), 'x') * aff;
			grFrame->SetTransform(aff);
#endif		
			

		} else {
			// フレームを剛体へ
			Affinef af;
			af = grFrame->GetTransform();

			// ボーンのルートフレームだった場合。必要性が不明のため残しとくが、後ほど検証のこと (12/05/17, mitake) <!!>
			// af = grFrame->GetWorldTransform();
			// af.Orthonormalization();

			Posed pose; pose.FromAffine(af);
			phSolid->SetPose(pose);
		}

	} else {
		if(syncSource==FWObjectDesc::PHYSICS){
			// 関節をフレームへ
			Posed jointPosition;
			jointPosition.Ori() = phJoint->GetRelativePoseQ() * sockOffset.Ori().Inv();
			Posed poseSocket; phJoint->GetSocketPose(poseSocket);
			Posed pose = poseSocket * jointPosition;

			Affinef af; pose.ToAffine(af);
			DCAST(GRFrame, grFrame)->SetTransform(af);

			PHSolidIf *so1 = phJoint->GetSocketSolid(), *so2 = phJoint->GetPlugSolid();
			if (so1 && so2) {
				DCAST(FWSceneIf,GetScene())->GetPHScene()->SetContactMode(so1, so2, PHSceneDesc::MODE_NONE);
			}

		} else {
			// フレームを関節（を構成する剛体）へ
			Affinef af = grFrame->GetWorldTransform();
			af.Orthonormalization(); //正規直交化
			Posed pose; pose.FromAffine(af);

			//アフィン行列→クォータニオンの変換誤差が大きい場合のエラー表示
			Affinef af2; pose.ToAffine(af2);
			Matrix3d mat=af.Rot(), mat2=af2.Rot();
			double epsilon = 0.1;
			bool   bErr    = false;
			for(int i=0;i<2;i++){
				for(int j=0;j<2;j++){
					if(fabs(mat[i][j]-mat2[i][j]) > epsilon){ bErr = true; }
				}
			}
			if(bErr){
				DSTR << "in FWObject[" << GetName() << "] : ";
				DSTR << mat << " <=> " << mat2 << " has error larger than " << epsilon << std::endl;
			}
			
			phSolid->SetPose(pose);
		}
	}
}

bool FWObject::AddChildObject(ObjectIf* o){
	bool bAdded = false;

	if (!bAdded) {
		GRMeshIf* obj = DCAST(GRMeshIf, o);
		if(obj){
			FWSceneIf* s=GetScene()->Cast();
			GRSceneIf* gs = s->GetGRScene();
			grFrame = gs->CreateVisual(GRFrameIf::GetIfInfoStatic(), GRFrameDesc())->Cast();
			grFrame->SetName("newFrameForMesh");
			grFrame->AddChildObject(obj);
			bAdded = true;
		}
	}

	if (!bAdded) {
		PHSolidIf* obj = DCAST(PHSolidIf, o);
		if (obj) {
			phSolid = obj;
			bAdded = true;
		}
	}

	if (!bAdded) {
		GRFrameIf* obj = DCAST(GRFrameIf, o);
		if (obj) {
			if (!grFrame) {
				grFrame = obj;
				bAdded = true;
			} else {
				childFrame = obj;
				bAdded = true;
			}
		}
	}

	if (!bAdded) {
		PHJointIf* obj = DCAST(PHJointIf, o);
		if (obj) {
			phJoint = obj;
			bAdded = true;
		}
	}

	if (bAdded && phSolid && grFrame && phJoint && childFrame) {
		Modify();
	}

	return bAdded;
}

ObjectIf* FWObject::GetChildObject(size_t pos){
	bool objs[] = {phSolid!=NULL, grFrame!=NULL, phJoint!=NULL, childFrame!=NULL};
	int cnt = -1;
	int i=0;
	for (; i<4; ++i) {
		if (objs[i]) { cnt++; }
		if (cnt==pos) { break; }
	}
	if (i == 0) { return phSolid;  }
	if (i == 1) { return grFrame;  }
	if (i == 2) { return phJoint;  }
	if (i == 3) { return childFrame; }
	return NULL;
}

size_t FWObject::NChildObject() const {
	bool objs[] = {phSolid!=NULL, grFrame!=NULL, phJoint!=NULL, childFrame!=NULL};
	int cnt = 0;
	for (int i=0; i<4; ++i) {
		if (objs[i]) { cnt++; }
	}
	return cnt;
}

bool FWObject::LoadMesh(const char* filename, const IfInfo* ii, GRFrameIf* frame){
	FWScene* scene = DCAST(FWScene, GetScene());
	FISdkIf* fiSdk = scene->sdk->GetFISdk();

	ObjectIfs objs;
	objs.Push(scene->GetGRScene());					///< GRSceneが作成し，
	objs.Push(frame ? frame : GetGRFrame());		///< GRFrameが持つ

	FIFileIf* file = (ii ? fiSdk->CreateFile(ii) : fiSdk->CreateFileFromExt(filename));
	if(!file)
		file = fiSdk->CreateFileX();

	return file->Load(objs, filename);
}

void FWObject::GenerateCDMesh(GRFrameIf* frame, const PHMaterial& mat){
	// フレームをスキャン
	for(int i = 0; i < frame->NChildren(); i++){
		GRMeshIf* mesh	 = DCAST(GRMeshIf,  frame->GetChildren()[i]);
		GRFrameIf* child = DCAST(GRFrameIf, frame->GetChildren()[i]);
		
		// 子メッシュ
		if(mesh){
			// descレベルで頂点座標をコピー
			GRMeshDesc		 grdesc;
			CDConvexMeshDesc cddesc;

			mesh->GetDesc(&grdesc);
			for(int i = 0; i < (int)grdesc.vertices.size(); i++)
				cddesc.vertices.push_back(grdesc.vertices[i]);

			// CDConvexMeshを作成
			FWScene* scene = DCAST(FWScene, GetScene());
			CDShapeIf* shape = DCAST(CDConvexMeshIf, scene->sdk->GetPHSdk()->CreateShape(cddesc));

			// メッシュを保有するフレームのposeを反映
			Posed pose;
			pose.FromAffine(frame->GetWorldTransform());

			PHSolidIf* solid = GetPHSolid();
			solid->AddShape(shape);
			solid->SetShapePose(solid->NShape()-1, pose);
		}

		// 子フレームに対して再帰呼び出し
		if(child)
			GenerateCDMesh(child, mat);
	}

}

void FWObject::Modify() {
	Posed poseSock, posePlug;
	poseSock.FromAffine( grFrame->GetTransform() );

	posePlug.FromAffine( Affinef() );
	GRFrameIf* fr = grFrame;
	Affinef af = Affinef();
	while (fr->GetParent()) {
		af = fr->GetTransform() * af;
		fr = fr->GetParent();
	}
	Posed absPose; absPose.FromAffine(af);

	PHBallJointIf *bj = phJoint->Cast();
	if (bj) {
		PHBallJointDesc d; bj->GetDesc(&d);
		sockOffset = d.poseSocket;
		d.poseSocket = poseSock * d.poseSocket; d.posePlug = posePlug * d.posePlug;
		d.poseSocket.Ori().unitize();
		d.posePlug.Ori().unitize();
		bj->SetDesc(&d);
	}
	PHHingeJointIf *hj = phJoint->Cast();
	if (hj) {
		PHHingeJointDesc d; hj->GetDesc(&d);
		sockOffset = d.poseSocket;
		d.poseSocket = poseSock * d.poseSocket; d.posePlug = posePlug * d.posePlug;
		d.poseSocket.Ori().unitize();
		d.posePlug.Ori().unitize();
		hj->SetDesc(&d);
	}

	double boneLength = childFrame->GetTransform().Trn().norm();

	for (int i=0; i<phSolid->NShape(); ++i) {
		CDRoundConeIf* rc = phSolid->GetShape(i)->Cast();
		if (rc) {
			CDRoundConeDesc rd;
			rc->GetDesc(&rd);
			rd.length = (float)boneLength;
			rc->SetDesc(&rd);

			Posed pose;
			pose.Pos() = Vec3d(0,0,-boneLength/2.0);
			phSolid->SetShapePose(i, pose);
			phSolid->SetCenterOfMass(Vec3d(0,0,-boneLength/2.0));
			phSolid->SetPose(absPose);
		}
	}
}

}
