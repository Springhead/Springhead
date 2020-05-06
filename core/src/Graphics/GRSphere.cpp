/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRSphere.h>
#include <Graphics/GRRender.h>

namespace Spr{;
GRSphere::GRSphere(const GRSphereDesc& desc):GRSphereDesc(desc){
	list = 0;
	render = NULL;
	material = NULL;
}
GRSphere::~GRSphere(){
	// これがプログラム終了時に呼ばれる時にはrenderが消滅しているのでここで落ちる。
	// どうしたものか。
	// (mitake, 09/08/23)
	if (list) render->ReleaseList(list);		// ディスプレイリストの破棄
}
void GRSphere::CreateList(GRRenderIf* r){
	if (list) render->ReleaseList(list);
	render = r;	
	list = render->StartList();
	if (material) render->SetMaterial(material->Cast());
	render->DrawSphere(radius, slices, stacks);
	render->EndList();
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
		material = m;
		return GetNameManager()->AddChildObject(m->Cast());
	}
	return false;
}

}
