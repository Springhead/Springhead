/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#include "GRSphere.h"
#include <gl/glut.h>


namespace Spr{;
IF_OBJECT_IMP(GRSphere, GRVisual);

GRSphere::GRSphere(const GRSphereDesc& desc):GRSphereDesc(desc){
	list = 0;
	render = NULL;
}
GRSphere::~GRSphere(){
	if (list) render->ReleaseList(list);		// ディスプレイリストの破棄
}
void GRSphere::CreateList(GRRenderIf* r){
	if (list) render->ReleaseList(list);
	list = 0;
	render = r;
}

void GRSphere::Render(GRRenderIf* r){
	if (r!=render || !list) CreateList(r);
	render->DrawList(list);
}
void GRSphere::Rendered(GRRenderIf* r){
}
bool GRSphere::AddChildObject(ObjectIf* o){			
	GRMaterial* m = DCAST(GRMaterial, o);
	if (m){
		material.push_back(m);
		return GetNameManager()->AddChildObject(m->GetIf());
	}
	return false;
}

}
