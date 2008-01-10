/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#ifndef SETDATAFROMGUI_H
#define SETDATAFROMGUI_H

#include <Springhead.h>
#include <iostream>
#include <gl/glui.h>
#include "MyGlobalValue.h"
/// Setting Data -------------------------------------------------------------------------
void guiSetDynamical(int control){
	animalBody1->GetSolid(CRFourLegsAnimalBodyDesc::SO_WAIST)->SetDynamical((bool)dynamicalValue);
	glui->sync_live();

}

void guiSetBoxsize(int control){
	box[nowSolid]->SetBoxSize(boxsize);
	//DSTR << "nowSolid : " << solids[nowSolid]->GetIfInfo()->ClassName() << " " << solids[nowSolid]->GetName() << endl;
	for(int i = 0; i<JO_NJOINTS; i++){
		if(animalBody1->GetJoint(i) != NULL){
			PHJointIf* jInfo = animalBody1->GetJoint(i);

			// はまりメモ--------------------------------------------------
			// PHJointDesc dをjInfo->GetDesc(&d);って呼べばいいと思っていたが、
			// それだとPHBallJointDescやPHHingeJointDescの方がテンプレートの大きさが大きいので
			// PHJointDescで宣言した領域よりもはみ出して書きこんでしまう。
			// そのためスタック破壊になり、更新しようとしてもエラーになる。
			// この問題を回避するために以下のようにする。↓
			//-------------------------------------------------------------

			PHBallJointIf* ball = XCAST(jInfo);
			PHHingeJointIf* hinge = XCAST(jInfo);

			if (ball){
				PHBallJointDesc d;
				jInfo->GetDesc(&d);
				if(solids[nowSolid] == animalBody1->GetJoint(i)->GetChildObject(0)){
					//現在のソリッドがソケット（親）側にくっついていた場合のジョイントの相対位置関係の更新処理
					if((nowSolid == CRFourLegsAnimalBodyDesc::SO_WAIST) && (i == CRFourLegsAnimalBodyDesc::JO_WAIST_CHEST)){
						d.poseSocket.Pos() = Vec3f(0.0, boxsize[1]/2.0, 0.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_CHEST) && (i == CRFourLegsAnimalBodyDesc::JO_LEFT_SHOULDER)){
						d.poseSocket.Pos() = Vec3f(-boxsize[0]/2.0, boxsize[1]/2.2, -boxsize[2]/3.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_CHEST) && (i == CRFourLegsAnimalBodyDesc::JO_RIGHT_SHOULDER)){
						d.poseSocket.Pos() = Vec3f(boxsize[0]/2.0, boxsize[1]/2.2, -boxsize[2]/3.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_WAIST) && (i == CRFourLegsAnimalBodyDesc::JO_LEFT_HIP)){
						d.poseSocket.Pos() = Vec3f(-boxsize[0]/2.0, -boxsize[1]/2.0, -boxsize[2]/3.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_WAIST) && (i == CRFourLegsAnimalBodyDesc::JO_RIGHT_HIP)){
						d.poseSocket.Pos() = Vec3f(boxsize[0] /2.0, -boxsize[1]/2.0, -boxsize[2]/3.0);
					}										
					else if(((nowSolid == CRFourLegsAnimalBodyDesc::SO_WAIST) && (i == CRFourLegsAnimalBodyDesc::JO_WAIST_TAIL)) || 
							((nowSolid == CRFourLegsAnimalBodyDesc::SO_TAIL1) && (i == CRFourLegsAnimalBodyDesc::JO_TAIL_12)) ||
							((nowSolid == CRFourLegsAnimalBodyDesc::SO_TAIL2) && (i == CRFourLegsAnimalBodyDesc::JO_TAIL_23))){
						d.poseSocket.Pos() = Vec3f(0.0, -boxsize[1] / 2.0, 0.0);					
					}
					else{
						d.poseSocket.Pos() = Vec3f(0, boxsize[1]/2.0, 0);
					}
					jInfo->SetDesc(&d);
				}
				else if(solids[nowSolid] == animalBody1->GetJoint(i)->GetChildObject(1)){				
					//現在のソリッドがプラグ（子）側にくっついていた場合のジョイントの相対位置関係の更新処理
					if((nowSolid == CRFourLegsAnimalBodyDesc::SO_CHEST) && (i == CRFourLegsAnimalBodyDesc::JO_WAIST_CHEST)){
						d.posePlug.Pos() = Vec3f(0.0, -boxsize[1] / 2.0, 0.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_LEFT_BREASTBONE) && (i == CRFourLegsAnimalBodyDesc::JO_LEFT_SHOULDER)){
						d.posePlug.Pos() = Vec3f(boxsize[0]/2.0, -boxsize[1]/2.0, 0.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_RIGHT_BREASTBONE) && (i == CRFourLegsAnimalBodyDesc::JO_RIGHT_SHOULDER)){
						d.posePlug.Pos() = Vec3f(-boxsize[0]/2.0, -boxsize[1]/2.0, 0.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_LEFT_FEMUR) && (i == CRFourLegsAnimalBodyDesc::JO_LEFT_HIP)){
						d.posePlug.Pos() = Vec3f(boxsize[0]/2.0, -boxsize[1]/2.0, 0.0);
					}
					else if((nowSolid == CRFourLegsAnimalBodyDesc::SO_RIGHT_FEMUR) && (i == CRFourLegsAnimalBodyDesc::JO_RIGHT_HIP)){
						d.posePlug.Pos() = Vec3f(-boxsize[0]/2.0, -boxsize[1]/2.0, 0.0);
					}
					else if(((nowSolid == CRFourLegsAnimalBodyDesc::SO_TAIL1) && (i == CRFourLegsAnimalBodyDesc::JO_WAIST_TAIL)) || 
							((nowSolid == CRFourLegsAnimalBodyDesc::SO_TAIL2) && (i == CRFourLegsAnimalBodyDesc::JO_TAIL_12)) ||
							((nowSolid == CRFourLegsAnimalBodyDesc::SO_TAIL3) && (i == CRFourLegsAnimalBodyDesc::JO_TAIL_23))){
						d.posePlug.Pos() = Vec3f(0.0, boxsize[1]/2.0, 0.0);					
					}
					else{
						d.posePlug.Pos() = Vec3f(0, -boxsize[1]/2.0 ,0);
					}
					jInfo->SetDesc(&d);
				}
			}
			if(hinge){
				PHHingeJointDesc d;
				jInfo->GetDesc(&d);
				if(solids[nowSolid] == animalBody1->GetJoint(i)->GetChildObject(0)){			
					// ソリッドがソケット（親）側だった場合
					d.poseSocket.Pos() = Vec3f(0.0, boxsize[1] / 2.0, 0.0);
					jInfo->SetDesc(&d);
				}
				else if(solids[nowSolid] == animalBody1->GetJoint(i)->GetChildObject(1)){
					// ソケットがプラグ（子）側だった場合
					d.posePlug.Pos() = Vec3f(0.0, -boxsize[1] / 2.0, 0.0);
					jInfo->SetDesc(&d);
				}
			}
		}
	}
	glui->sync_live();
}

void guiSetMass(int control){
	//DSTR << "nowSolid : " << solids[nowSolid]->GetIfInfo()->ClassName() << " " << solids[nowSolid]->GetName() << endl;
	solids[nowSolid]->SetMass(mass);
	glui->sync_live();
}


void guiSetDataBallJoint(int control){
	
	if(control == 0){				//< spinnerが更新された場合の処理
		if(DCAST(PHBallJointIf, animalBody1->GetJoint(nowBallJoint))){
			PHBallJointDesc ballDesc;
			animalBody1->GetJoint(nowBallJoint)->GetDesc(&ballDesc);
			ballDesc.spring			  = (double)ballJointData[0];
			ballDesc.damper			  = (double)ballJointData[1];
			animalBody1->GetJoint(nowBallJoint)->SetDesc(&ballDesc);
		}
		else DSTR << "【error】: Failed Getting The Joint Desc" << std::endl;
	}
	else if(control == 1){			//< RotationControlsが更新された場合の処理
		if(DCAST(PHBallJointIf, animalBody1->GetJoint(nowBallJoint))){
			goalEulerMatrix = *(Affinef*)rotationBallJoint * matMadeByQuaterinon;
			PHBallJointDesc ballDesc;		
			animalBody1->GetJoint(nowBallJoint)->GetDesc(&ballDesc);						
			ballDesc.goal.FromMatrix(goalEulerMatrix);
			animalBody1->GetJoint(nowBallJoint)->SetDesc(&ballDesc);
			///////////////////////
			// Affinef a;		 //
			// float* p = a;	 //
			// a = *(Affinef*)p; //
			///////////////////////
		}
	}
	else{
		DSTR << "【error】: Invalid control value　(SetBallJoint() is failed. )" << endl;
	}	
	glui->sync_live();
}

void guiSetDataHingeJoint(int control){

	if(DCAST(PHHingeJointIf, animalBody1->GetJoint(nowHingeJoint))){
		PHHingeJointDesc hingeDesc;
		animalBody1->GetJoint(nowHingeJoint)->GetDesc(&hingeDesc);
		hingeDesc.spring = (double)hingeJointData[0];
		hingeDesc.damper = (double)hingeJointData[1];
		hingeDesc.lower  = (double)hingeJointData[2];
		hingeDesc.upper  = (double)hingeJointData[3];
		hingeDesc.origin = (double)hingeJointGoal;
		animalBody1->GetJoint(nowHingeJoint)->SetDesc(&hingeDesc);
	}

	else{
		DSTR << "SetHingeJoint() is failed." << endl;
	}
	glui->sync_live();
}

#endif