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

FWBone::FWBone()
{

}

void FWObject::Sync(){
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


}
