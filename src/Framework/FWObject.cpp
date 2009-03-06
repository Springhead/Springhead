/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FWObject.h"
#include <Graphics/GRFrame.h>

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWObject::FWObject(const FWObjectDesc& d/*=FWObjectDesc()*/)
: desc(d), phSolid(NULL), grFrame(NULL)
{
}

void FWObject::Sync(){
	if (phSolid && grFrame){
		if(phAncestorSolid){
			//ボーン付きXファイルの最初のBone(剛体に適応させるとき
			Affinef af;
			Matrix3d matrix;
			phSolid->GetPose().ToAffine(af);
			af.Rot()=matrix;
			DCAST(GRFrame, grFrame)->SetTransform(af);
		}else if(phAncestorSolid2){
			Affinef af;
			phSolid->GetPose().ToAffine(af);
			af.Trn()=Vec3d(0,0,0);
			DCAST(GRFrame, grFrame)->SetTransform(af);
		}else if(phParentSolid){
			//関節があるときのスキンメッシュの座標変換
			Affinef af,afParent,afd;
			phSolid->GetPose().ToAffine(af);
			phParentSolid->GetPose().ToAffine(afParent);
			afd=afParent.inv()*af;
			DCAST(GRFrame, grFrame)->SetTransform(afd);
		}else if(grParentAffine){
			Affinef af,afd,T;
			phSolid->GetPose().ToAffine(af);	//現在の剛体のアフィン行列afを取得
			//T=T.Rot(Rad(180), 'y');
			//afd=grParentAffine.inv()*af*T;
			//DSTR<<"Transform1"<<std::endl<<DCAST(GRFrame, grFrame)->GetTransform()<<std::endl;
			DCAST(GRFrame, grFrame)->SetTransform(afd);  //FwObjectに剛体と共にセットされているgrFrameのアフィン行列を現在の剛体のアフィン行列に書き換える
			DSTR<<"Transform2"<<std::endl<<DCAST(GRFrame, grFrame)->GetTransform()<<std::endl;
			DSTR<<"Solidaf"<<std::endl<<af<<std::endl;
			DSTR<<"grParentAffine"<<std::endl<<grParentAffine<<std::endl;
		}else{
			Affinef af;
			phSolid->GetPose().ToAffine(af);
			DCAST(GRFrame, grFrame)->SetTransform(af);
		}
	}else{
		//DSTR << "Warning: No solid or frame for " << GetName() << ":FWObject." << std::endl;
	}
}


bool FWObject::AddChildObject(ObjectIf* o){
	bool rv = false;
	if (!rv) {
		PHSolidIf* obj = DCAST(PHSolidIf, o);
		if (obj) {
			phSolid = obj;
			rv = true;
		}
	}
	if (!rv) {
		GRFrameIf* obj = DCAST(GRFrameIf, o);
		if (obj) {
			grFrame = obj;
			rv = true;
		}
	}
	return rv;
}

}
