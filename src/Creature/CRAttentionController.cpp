/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "CRAttentionController.h"

#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{
IF_OBJECT_IMP(CRAttentionController, SceneObject);

void CRAttentionController::Step(){
	attentionList->SortAttentionList();
	CRAttentionItem* att = attentionList->GetAttentionItem(0);
	gazeController->LookAt(att->solid->GetPose()*att->pos, att->solid->GetVelocity(), att->attractiveness);
}
}
