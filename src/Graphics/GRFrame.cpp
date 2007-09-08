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
//-----------------------------------------------------------------
//	GRVisual
//
IF_OBJECT_IMP_ABST(GRVisual, NamedObject);

//-----------------------------------------------------------------
//	GRFrame
//
IF_OBJECT_IMP(GRFrame, GRVisual);
GRFrame::GRFrame(const GRFrameDesc& desc):GRFrameDesc(desc){
	parent = NULL;

}

void GRFrame::Render(GRRenderIf* r){
	r->PushModelMatrix();
	r->MultModelMatrix(transform);
	for(GRVisualIfs::iterator it = children.begin(); it != children.end(); ++it){
		(*it)->Render(r);
	}
	for(GRVisualIfs::reverse_iterator it = children.rbegin(); it != children.rend(); ++it){
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
	if((GRFrameIf*)(parent->Cast()) == fr) return;
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
	GRVisualIf* v = DCAST(GRVisualIf, o);
	if (v){
		children.push_back(v);
		GRFrame* f = DCAST(GRFrame, v);
		if (f && f->parent != this){
			//ここで元の持ち主から削除するのはやりすぎでは？
			//if (f->parent) f->parent->DelChildObject(f->Cast());
			f->parent = this;
		}
		return true;
	}
	return false;
}
bool GRFrame::DelChildObject(ObjectIf* v){
	for(GRVisualIfs::iterator it = children.begin(); it != children.end(); ++it){
		if (*it == v){
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


//-----------------------------------------------------------------
//	GRAnimation
//
IF_OBJECT_IMP(GRAnimation, SceneObject);
GRAnimation::GRAnimation(): activated(false){
	weight = 0;
	weightTo = 0;
	duration = 0;
};

void GRAnimation::SetTime(float time){
	/*
	for(vector<GRAnimationDesc::Animation>::iterator it = animations.begin(); 
		it != animations.end(); ++it){
		GRAnimationDesc::Animation& anim = *it;
		for(int i=0; i<	anim.keys.size(); ++i){
			if (anim.keys[i].time > time){
				if (i==0){	//	i=0だけをセット

				}

			}
		}
	}
	*/
}

//-----------------------------------------------------------------
//	GRAnimationSet
//
IF_OBJECT_IMP(GRAnimationSet, SceneObject);

bool GRAnimationSet::AddChildObject(ObjectIf* o){
	GRAnimation* ani = o->Cast();
	if (ani){
		animations.push_back(ani);
	}
	return false;
}
bool GRAnimationSet::DelChildObject(ObjectIf* o){
	GRAnimation* ani = o->Cast();
	if (ani){
		for(Animations::iterator it = animations.begin(); it != animations.end(); ++it){
			if(ani == *it){
				animations.erase(it);
				return true;
			}
		}
	}
	return false;
}
int GRAnimationSet::NChildObject(){
	return animations.size();
}
ObjectIf* GRAnimationSet::GetChildObject(size_t p){
	return animations[p]->Cast();
}

void GRAnimationSet::Activate(UTString name, float weightStart, float weightEnd, float duration){
	for(Animations::iterator it = animations.begin(); it!=animations.end(); ++it){
		if (name.compare((*it)->GetName())==0){
			(*it)->activated = true;

		}
	}

}

void GRAnimationSet::Deactivate(UTString name){
	
}


}
