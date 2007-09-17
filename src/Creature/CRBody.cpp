/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRBody.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRBody, SceneObject);

PHJointIf* CRBody::CreateJoint(PHSolidIf* soChild, PHSolidIf* soParent, PHHingeJointDesc desc){
	PHJointIf* joint;
	if (jointOrder == PLUG_PARENT) {
		joint = phScene->CreateJoint(soChild, soParent, desc);
	} else { // SOCKET_PARENT
		Posed pp=desc.posePlug, ps=desc.poseSocket;
		desc.posePlug=ps; desc.poseSocket=pp;
		joint = phScene->CreateJoint(soParent, soChild, desc);
	}
	return joint;
}

void CRBody::Init(){
	// コンストラクタに移動
	/*
	phScene = DCAST(PHSceneIf, GetScene());
	phSdk   = DCAST(PHSdkIf, phScene->GetSdk());
	*/
}

int CRBody::NSolids(){
	return solids.size();
}

PHSolidIf* CRBody::GetSolid(int i){
	return solids[i];
}

int CRBody::NJoints(){
	return joints.size();
}

PHJointIf* CRBody::GetJoint(int i){
	return joints[i];
}
}
