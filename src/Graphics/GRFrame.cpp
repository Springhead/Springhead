/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#include "GRFrame.h"

namespace Spr{;
IF_OBJECT_IMP_ABST(GRVisual, NamedObject);

IF_OBJECT_IMP(GRFrame, GRVisual);
GRFrame::GRFrame(const GRFrameDesc& desc):GRFrameDesc(desc){
	parent = NULL;

}

void GRFrame::Render(GRRenderIf* r){
	r->PushModelMatrix();
	r->MultModelMatrix(transform);
	for(GRVisuals::iterator it = children.begin(); it != children.end(); ++it){
		(*it)->Render(r);
	}
	for(GRVisuals::reverse_iterator it = children.rbegin(); it != children.rend(); ++it){
		(*it)->Rendered(r);
	}
	r->PopModelMatrix();
}
void GRFrame::Rendered(GRRenderIf* r){
}
void GRFrame::SetNameManager(NameManager* m){
	assert(DCAST(GRScene, m));
	GRVisual::SetNameManager(m->Cast());
}
GRSceneIf* GRFrame::GetScene(){
	return DCAST(GRSceneIf, GetNameManager());
}
void GRFrame::SetParent(GRFrameIf* fr){
	if(parent->Cast() == fr) return;
	if(parent){
		parent->DelChildObject(this->Cast());
		parent=NULL;
	}
	if (fr){
		parent = DCAST(GRFrame, fr);
		fr->AddChildObject(this->Cast());
	}
}
bool GRFrame::AddChildObject(ObjectIf* o){
	GRVisual* v = DCAST(GRVisual, o);
	if (v){
		children.push_back(v);
		GRFrame* f = DCAST(GRFrame, v);
		if (f && f->parent != this){
			if (f->parent) f->parent->DelChildObject(f->Cast());
			f->parent = this;
		}
		return true;
	}
	return false;
}
bool GRFrame::DelChildObject(ObjectIf* v){
	for(GRVisuals::iterator it = children.begin(); it != children.end(); ++it){
		if (*it == DCAST(Object, v)){
			children.erase(it);
			return true;
		}
	}
	return false;
}
size_t GRFrame::NChildObject() const {
	return children.size();
}
ObjectIf* GRFrame::GetChildObject(size_t pos){
	return children[pos]->Cast();
}

void GRFrame::Print(std::ostream& os) const {
	GRVisual::PrintHeader(os, false);
	os.width(os.width()+2);
	os << transform;
	os.width(os.width()-2);
	GRVisual::PrintChildren(os);
	GRVisual::PrintFooter(os);
}

}
