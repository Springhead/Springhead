/*  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWBone.h"
#include <Graphics/GRFrame.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

BoneJoint::BoneJoint()
{
}

FWBone::FWBone()
{

}

void FWBone::Sync(){
	if (phSolid && grFrame){
		if(solidLength){
			//ボーン付きXファイルを使用する場合
			if(bonePositionFlag==1){
				//ボーン１　（並行行列のみのアフィン行列更新）
				Affinef af;
				Matrix3d matrix;
				phSolid->GetPose().ToAffine(af);
				af.Rot()=matrix;									//回転成分の初期化
				DCAST(GRFrame, grFrame)->SetTransform(af);
			}else if(bonePositionFlag==2){
				//ボーン２　（回転行列のみのアフィン行列更新）
				Affinef af,afParent,afd,afl,AF;
				phSolid->GetPose().ToAffine(af);
				afParent=grFrame->GetParent()->GetWorldTransform();
				afd=afParent.inv()*af;
				afl.PosZ()+=solidLength/2;							//剛体中心の位置から剛体の半長分だけずらし，ジョイント部分の位置にする
				AF=afd*afl;
				DCAST(GRFrame, grFrame)->SetTransform(AF);
			}else if(bonePositionFlag==3){
				//ボーン３
				Affinef af,afParent,afd,afl,AF;
				phSolid->GetPose().ToAffine(af);
				afParent=grFrame->GetParent()->GetWorldTransform();
				afd=afParent.inv()*af;
				afl.PosZ()+=solidLength/2;							//剛体中心の位置から剛体の半長分だけずらし，ジョイント部分の位置にする
				AF=afd*afl;
				DCAST(GRFrame, grFrame)->SetTransform(AF);
			}else if(bonePositionFlag==0){
				//ボーン１，２以外のボーン（アフィン行列の更新）
				Affinef af,afParent,afd,afl,AF;
				phSolid->GetPose().ToAffine(af);	
				afParent=grFrame->GetParent()->GetWorldTransform();	//親のWorld座標からみたFrameを取得
				afd=afParent.inv()*af;								//階層構造下のAffin行列に変換する
				afl.PosZ()+=solidLength/2;							//剛体中心の位置から剛体の半長分だけずらし，ジョイント部分の位置にする
				AF=afd*afl;
				DCAST(GRFrame, grFrame)->SetTransform(AF);
			}else{
				DSTR << "Warning: No solid or frame for " << GetName() << ":FWObject." << std::endl;
			}
		}
	}
}



void FWBoneCreate::BoneCreate(GRMesh* m, PHScene* s){
	mesh=m;
	phScene=s;
	SetFWBone();
}

void FWBoneCreate::SetFWBone(){
	if (mesh){
		for(int i=0 ;i<mesh->skinWeights.size(); ++i){
			GRFrameIf* grFrame =mesh->skinWeights[i].frame->Cast();
			GRFrameIf* grParentFrame=mesh->skinWeights[i].frame->GetParent();
		/*	GRFrameIf* grChildFrame=mesh->skinWeights[i].frame->GetChildren();*/
			std::string name	= grFrame->GetName();
			std::string nameP	= grParentFrame->GetName();
			std::string namePP	= grParentFrame->GetParent()->GetName();
			DSTR << "  Parent:"<< nameP << "now:"<<name << std::endl;
			Vec3d point			= grFrame->GetWorldTransform()*Vec3d(0.0,0.0,0.0);
			Vec3d parentPoint	= grParentFrame->GetWorldTransform()*Vec3d(0.0,0.0,0.0);
			if(!(nameP=="Root")){//Bone1以外
				if(!(namePP=="Root")){
					bone.push_back(bone_);
					bone[bone.size()-1].centerPoint = (point+parentPoint)*0.5;
				}else{}
			}
		}
	}
	//FWSdkの取得
	fwSdk = phScene->GetSdk()->GetNameManager()->Cast();
	//phSceneIfの取得
	phSceneIf =phScene->Cast();
}

void FWBoneCreate::SetWorldAffine(std::vector<Affinef> a){
	af.swap(a);
	//アフィン行列から3次元座標を算出
	Vec3d BonePoint;
	if (af.size()){
		for(int i=0 ;i<af.size(); ++i){
			BonePoint=af[i]*Vec3d(0.0,0.0,0.0);
			bonePoint.push_back(BonePoint);
			//DSTR << i << std::endl << bonePoint[i] << std::endl;
		}
	}

	//3次元座標からPHBoneの剛体情報を算出
	if (bonePoint.size()){
		for(int i=0 ;i<bonePoint.size()-2; ++i){
			bone.push_back(bone_);
			bone[i].centerPoint = (bonePoint[i+2]+bonePoint[i+1])*0.5;
			Vec3d length = bonePoint[i+2]-bonePoint[i+1];
			bone[i].length = length.norm();
			//DSTR<< i << std::endl << bone[i].centerPoint << bone[i].length << std::endl;
		}
	}
	//アフィン行列を剛体に保存
	if (af.size()){
		for(int i=0 ;i<bone.size(); ++i){
			bone[i].worldTransformAffine=af[i+1];
			//DSTR << i << std::endl << bone[i].grParentAffine << std::endl;
		}
	}
	////PHBoneの剛体情報からJointのソケット・プラグの位置を算出
	//if (bone.size()){
	//	for(int i=0 ;i<bone.size()-1; ++i){
	//		boneJoint.push_back(boneJointData);
	//		boneJoint[i].SocketPos = Vec3f(0.0,0.0, -bone[i].length/2);
	//		boneJoint[i].PlugPos = Vec3f(0.0,0.0,bone[i+1].length/2);
	//	}
	//}

}

void FWBoneCreate::FWPHBoneCreate(){
	//double wide =0.5;
	////desc
	//desc.mass = 0.05;
	//desc.inertia = 0.033 * Matrix3d::Unit();
	//CDBoxDesc dBox;

	//for(int i=0; i<bone.size(); ++i){
	//	Posed pose;
	//	//soBoneの作成
	//	soBone.push_back(phScene->CreateSolid(desc));
	//	bone[i].solid=soBone[i];
	//	{
	//		if(i==0){
	//			bone[i].solid->SetDynamical(false);
	//		}else{
	//			bone[i].solid->SetDynamical(true);
	//		}
	//		//bone[i].solid->SetFramePosition(bone[i].centerPoint);
	//		pose.FromAffine(bone[i].worldTransformAffine);
	//		pose.PosZ()+=bone[i].length/2;
	//		bone[i].solid->SetPose(pose);
	//	}
	//		if(i==0){
	//			ancestorBone.push_back(boneData);
	//			soAncestorBone.push_back(phScene->CreateSolid(desc));
	//			ancestorBone[0].solid=soAncestorBone[0];
	//			ancestorBone[0].solid->SetPose(pose);
	//		}
	//	//shapeBoneの作成	
	//	{
	//		dBox.boxsize=(Vec3d(wide,wide,bone[i].length));
	//		bone[i].shapeBone=XCAST(fwSdk->GetPHSdk()->CreateShape(dBox));
	//	}
	//	bone[i].solid->AddShape(bone[i].shapeBone);
	//}
	//FWJointCreate();
	//FWSkinMeshAdapt();
}
void FWBoneCreate::FWJointCreate(){
	//std::vector<PH3ElementBallJointDesc> d3Ball;
	//PH3ElementBallJointDesc _d3Ball;

	//if (boneJoint.size()){
	//	for(int i=0 ;i<boneJoint.size(); ++i){
	//		d3Ball.push_back(_d3Ball);
	//		{
	//			d3Ball[i].poseSocket.Pos()	= boneJoint[i].SocketPos;
	//			d3Ball[i].posePlug.Pos()	= boneJoint[i].PlugPos;
	//			d3Ball[i].spring			= boneJoint[i].K;
	//			d3Ball[i].damper			= boneJoint[i].D1;
	//			d3Ball[i].secondDamper		= boneJoint[i].D2;
	//			d3Ball[i].yieldStress		= boneJoint[i].yieldStress;
	//			d3Ball[i].hardnessRate		= boneJoint[i].hardnessRate;
	//		}
	//	}
	//	for(int i=0;i<boneJoint.size();i++){
	//		phScene->SetContactMode(bone[i].solid, bone[i+1].solid, PHSceneDesc::MODE_NONE);
	//		phScene->CreateJoint(bone[i].solid,bone[i+1].solid, d3Ball[i]);
	//		Joint.push_back( phScene->CreateJoint( soBone[i], soBone[i+1], d3Ball[i]) );
	//	}
	//}
}

void FWBoneCreate::FWSkinMeshAdapt(){

	//for(int i=0; i<bone.size(); ++i){
	//	//ボーン１の設定をする（並行行列のみのアフィン行列更新）
	//	if(i==0){
	//		fwoAncestorBone.push_back(fwSdk->GetScene()->CreateFWObject());
	//		ancestorBone[0].fwObject	= fwoAncestorBone[0];
	//		ancestorBone[0].grFrame		= grfBone[0];
	//		ancestorBone[0].length		= bone[0].length;

	//		ancestorBone[0].fwObject->SetPHSolid(ancestorBone[0].solid);
	//		ancestorBone[0].fwObject->SetbonePositionFlag(1);
	//		ancestorBone[0].fwObject->SetSolidLength(ancestorBone[0].length);
	//		ancestorBone[0].fwObject->SetGRFrame(ancestorBone[0].grFrame->Cast());
	//	}
	//	fwoBone.push_back(fwSdk->GetScene()->CreateFWObject());

	//	bone[i].fwObject=fwoBone[i];
	//	bone[i].grFrame=grfBone[i+1];
	//	//PHとGRを連動させる
	//	bone[i].fwObject->SetPHSolid(bone[i].solid);
	//	if(i==0){
	//		//ボーン２の設定をする（回転行列のみのアフィン行列更新）
	//		bone[i].fwObject->SetbonePositionFlag(2);
	//		bone[i].fwObject->SetSolidLength(bone[i].length);
	//	}else if(i==1){
	//		//ボーン３の設定（アフィン行列）
	//		bone[i].fwObject->SetbonePositionFlag(3);
	//		bone[i].fwObject->SetSolidLength(bone[i].length);
	//	}else{
	//		//ボーン１，２以外のボーンの設定（アフィン行列）
	//		bone[i].fwObject->SetbonePositionFlag(0);
	//		bone[i].fwObject->SetSolidLength(bone[i].length);
	//	}
	//	bone[i].fwObject->SetGRFrame(bone[i].grFrame->Cast());

	//	Affinef afs;
	//	bone[i].solid->GetPose().ToAffine(afs);
	//	bone[i].grFrame->GetObjectIf()->Print(std::cout);
	//}
}

void FWBoneCreate::DisplayBonePoint(){
	//CDSphereDesc dSphere;
	//CDSphereIf* shapeSphere;
	//std::vector<PHSolidIf*> soSphere;

	//for(int i=0; i<bonePoint.size(); ++i){
	//	//soSphereの作成
	//	soSphere.push_back(phScene->CreateSolid(desc));
	//	{
	//		soSphere[i]->SetDynamical(false);
	//		soSphere[i]->SetFramePosition(bonePoint[i]);
	//	}
	//	//shapeBoneの作成	
	//	{
	//		dSphere.radius=0.1;
	//		shapeSphere=XCAST(fwSdk->GetPHSdk()->CreateShape(dSphere));
	//	}
	//	soSphere[i]->AddShape(shapeSphere);
	//}
}

void FWBoneCreate::DisplayPHBoneCenter(){
	//CDBoxDesc dBoxD;
	//CDBoxIf* shapeBoxD;
	//std::vector<PHSolidIf*> soBoxD;
	//for(int i=0; i<bone.size(); ++i){
	//	//soBoxDの作成
	//	soBoxD.push_back(phScene->CreateSolid(desc));
	//	{
	//		soBoxD[i]->SetDynamical(false);
	//		soBoxD[i]->SetFramePosition(bone[i].centerPoint);
	//	}
	//	//shapeBoxDの作成	
	//	{
	//		dBoxD.boxsize=(Vec3d(0.1,0.1,0.1));
	//		shapeBoxD=XCAST(fwSdk->GetPHSdk()->CreateShape(dBoxD));
	//	}
	//	soBoxD[i]->AddShape(shapeBoxD);
	//}
}

void FWBoneCreate::SetAffine(std::vector<Affinef> a){
	//アフィン行列を剛体に保存
	if (af.size()){
		for(int i=0 ;i<bone.size(); ++i){
			bone[i].transformAffine=af[i+1];
			//DSTR << i << std::endl << bone[i].grParentAffine << std::endl;
		}
	}
}



}
