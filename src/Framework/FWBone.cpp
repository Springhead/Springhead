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

using namespace std;
namespace Spr{;

BoneJoint::BoneJoint()
{
	K				= 10;
	D1				= 2;
	D2				= 0.1;
	yieldStress		= 0.1;
	hardnessRate	= 1e3;
	SocketPos		=Vec3f(0.0,0.0,0.0);
	PlugPos			=Vec3f(0.0,0.0,0.0);
}

FWBone::FWBone(const FWBoneDesc& d/*=FWObjectDesc()*/)
: desc(d)
{
}
void FWBone::SetJointKDD2(double K,double D, double D2){
	PH3ElementBallJointIf* ball=joint->Cast();
	ball->SetSpring(K);
	ball->SetDamper(D);
	ball->SetSecondDamper(D2);
}

//Bootを呼べばすべての処理が自動で行われる-------------------------------------------------------
/*NodeHandlerでBoneCreateを呼んでボーン作成し適合させる*/
void FWBoneCreate::Boot(GRMesh* mesh, PHScene* phScene){
	/*MeshとPHSceneを最初に設定,以降の関数は代入されている事を前提とする*/
	SetMesh(mesh);
	SetPHScene(phScene);
	//FWSdkの取得
	fwSdk = phScene->GetSdk()->GetNameManager()->Cast();
	//phSceneIfの取得
	phSceneIf =phScene->Cast();

	SetFWBone();		//Xファイルの情報をFWBoneデータ構造に変換
	SetBoneJoint();		//FWBoneのデータから親に対するジョイントのプラグ，ソケットの位置測りFWBoneに代入
	GenerateBone();		//シーンに剛体を作成
	FWJointCreate();	//シーンにジョイントを作成
	ContactCanceler();	//連なる剛体の接触を切る
	FWSkinMeshAdapt();	//FWObjectに代入し，スキンメッシュを適合する
}

//Xファイルの情報をFWBoneデータ構造に変換--------------------------------------------------------
/*FWBoneを作成し数値を代入*/
void FWBoneCreate::SetFWBone(){
	if (mesh){
		for(unsigned int i=0 ;i<mesh->skinWeights.size()-1; ++i){
			GRFrameIf* frame1 =mesh->skinWeights[i].frame->Cast();
			GRFrameIf* frame2 =mesh->skinWeights[i+1].frame->Cast();
			if(BoneDetector(frame1,frame2)==true){
				bone_= new FWBone;
				bone.push_back(bone_);
				bone.back()->centerPoint			= BonePosition(frame1,frame2);
				bone.back()->length					= BoneLength(frame1,frame2);
				bone.back()->shapeBone				= BoneShapeCone(frame1,frame2);
				bone.back()->parentBone				= ParentBone(frame1);
				bone.back()->grFrame				= frame1;
				bone.back()->worldTransformAffine	= frame1->GetWorldTransform();
			}
		}
	}
}

/*2つのgrFrameがつながっているかどうかを検出し，FWBoneを作成する場所を探す*/
bool FWBoneCreate::BoneDetector(GRFrameIf* frame1,GRFrameIf* frame2){
	bool Flag=false;
	std::string name1		= frame1->GetName();
	std::string name2Parent	= frame2->GetParent()->GetName();
	if(!(name1=="Root")){//Rootは作成しない
		if(name1==name2Parent){//frame1とframe2がつながっている時true
			Flag=true;
		}
	}
	return Flag;
}
/*2つのgrFrameから中間地点を算出*/
Vec3d FWBoneCreate::BonePosition(GRFrameIf* frame1,GRFrameIf* frame2){
	Vec3d point1	= frame1->GetWorldTransform()*Vec3d(0.0,0.0,0.0);
	Vec3d point2	= frame2->GetWorldTransform()*Vec3d(0.0,0.0,0.0);
	return (point1+point2)*0.5;
}
/*2つのgrFrameから距離を作成*/
double FWBoneCreate::BoneLength(GRFrameIf* frame1,GRFrameIf* frame2){
	Vec3d point1= frame1->GetWorldTransform()*Vec3d(0.0,0.0,0.0);
	Vec3d point2= frame2->GetWorldTransform()*Vec3d(0.0,0.0,0.0);
	Vec3d length= point2-point1;
	return length.norm();
}
/*2つのgrFrameからshape(ラウンドコーン）を作成*/
CDRoundConeIf* FWBoneCreate::BoneShapeCone(GRFrameIf* frame1,GRFrameIf* frame2){
	double wide=0.5;
	double lengthRate=1;
	double length=BoneLength(frame1,frame2);
	CDRoundConeDesc desc;
	desc.length=((float)length-(float)wide*2.0f)*((float)lengthRate);
	desc.radius=Vec2f(wide,wide);
	shapeBone.push_back(XCAST(fwSdk->GetPHSdk()->CreateShape(desc)));
	return shapeBone[shapeBone.size()-1];
}
/*親boneを設定*/
FWBone* FWBoneCreate::ParentBone(GRFrameIf* frame1){
	FWBone* parentBone=NULL;
	std::string parentName=frame1->GetParent()->GetName();
	for(unsigned int i=0 ;i<bone.size()-1; ++i){
		DSTR<<bone[bone.size()-2-i]->grFrame->GetName()<<endl;
		if(parentName==bone[bone.size()-2-i]->grFrame->GetName()){
			parentBone=bone[bone.size()-2-i];
			break;
		}
	}
	return parentBone;
}
/*親boneに対するBoneJointデータを設定*/
void FWBoneCreate::SetBoneJoint(){
	if (bone.size()){
		for(unsigned int i=0 ;i<bone.size(); ++i){
			if(!(bone[i]->parentBone==NULL)){
				bone[i]->jointData.SocketPos=Vec3f(0.0,0.0, -bone[i]->parentBone->length/2);
				bone[i]->jointData.PlugPos=Vec3f(0.0,0.0, bone[i]->length/2);
			}
		}
	}
}
//ボーンの作成-----------------------------------------------------------------------------------
/*FWBoneの情報を使ってシーンを作成*/
void FWBoneCreate::GenerateBone(){
	for(unsigned int i=0; i<bone.size(); ++i){
		//soBoneの作成
		PHSolidDesc	desc;
		desc.mass = 0.0005;
		desc.inertia = 0.033 * Matrix3d::Unit();
		soBone.push_back(phScene->CreateSolid(desc));
		bone[i]->phSolid=soBone[i];
		{
			if(i==0){
				bone[i]->phSolid->SetDynamical(true);
			}else{
				bone[i]->phSolid->SetDynamical(true);
			}
			//ボーンの初期位置を設定
			Posed pose,pose2;
			pose.PosZ()=-bone[i]->length/2;
			pose2.FromAffine(bone[i]->worldTransformAffine);
			pose=pose2*pose;
			bone[i]->phSolid->SetPose(pose);
		}
		//shapeBoneの作成
		if(i>0){
			bone[i]->phSolid->AddShape(bone[i]->shapeBone);
		}
	}
}
/*ジョイントを作成*/
void FWBoneCreate::FWJointCreate(){
	if (bone.size()){
		for(size_t i=0 ;i<bone.size(); ++i){
			if(!(bone[i]->parentBone==NULL)){
				PH3ElementBallJointDesc d3Ball;
				{
					d3Ball.poseSocket.Pos()	= bone[i]->jointData.SocketPos;
					d3Ball.posePlug.Pos()	= bone[i]->jointData.PlugPos;
					d3Ball.spring			= bone[i]->jointData.K;
					d3Ball.damper			= bone[i]->jointData.D1;
					d3Ball.secondDamper		= bone[i]->jointData.D2;
					d3Ball.yieldStress		= bone[i]->jointData.yieldStress;
					d3Ball.hardnessRate		= bone[i]->jointData.hardnessRate;
				}
				joint.push_back( phSceneIf->CreateJoint(bone[i]->parentBone->phSolid, bone[i]->phSolid, d3Ball));
				bone[i]->joint=joint.back();
			}
		}
	}
}
/*接触判定の設定(隣合う剛体の接触を切る)*/
void FWBoneCreate::ContactCanceler(){
	if (bone.size()){
		for(size_t i=0 ;i<bone.size(); ++i){
			if(!(bone[i]->parentBone==NULL)){
				//親子の接触を切る
				phSceneIf->SetContactMode(bone[i]->parentBone->phSolid, bone[i]->phSolid, PHSceneDesc::MODE_NONE);
				//同じ親をもつ剛体の接触を切る
				for(size_t j=0 ;j<bone.size(); ++j){
					if(!(bone[j]->parentBone==NULL)){
						if(!(j==i)){
							if(bone[i]->parentBone->grFrame->GetName()==bone[j]->parentBone->grFrame->GetName()){
								phSceneIf->SetContactMode(bone[i]->phSolid, bone[j]->phSolid, PHSceneDesc::MODE_NONE);
							}
						}
					}
				}
			}
		}
	}
}

void FWBoneCreate::FWSkinMeshAdapt(){
	for(size_t i=0; i<bone.size(); ++i){
		//ボーン１の設定をする（並行行列のみのアフィン行列更新）
		fwObject.push_back(fwSdk->GetScene()->CreateFWObject());
		fwObject[i]->SetPHSolid(bone[i]->phSolid);
		fwObject[i]->SetGRFrame(bone[i]->grFrame);
		fwObject[i]->SetSolidLength(bone[i]->length);
		fwSdk->GetScene()->SetFWBones(DCAST(FWBoneIf,bone[i]));
	}
}

}
