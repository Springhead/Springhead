/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Graphics.h"
#include "GRFrame.h"
#include "GRMesh.h"

namespace Spr{;
//-----------------------------------------------------------------
//	GRVisual
//
IF_OBJECT_IMP_ABST(GRVisual, SceneObject);

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
	GRVisualIf* v = o->Cast();
	if (v){
		children.push_back(v);
		GRFrame*	frame	= DCAST(GRFrame, v);
		GRMaterial* mat		= DCAST(GRMaterial, v);
		GRLight*	light	= DCAST(GRLight, v);
		GRMesh*		mesh	= DCAST(GRMesh, v);
		if (frame && frame->parent != this){
			//	ここで元の持ち主から削除するのはやりすぎでは？	by tazaki ?
			//	 -> いや、Frame は parentがひとつなので、複数のフレームの子になるのはだめです。 by hase
			if (frame->parent) frame->parent->DelChildObject(frame->Cast());
			frame->parent = this;
		}
		// デフォルトネーム設定
		if(strcmp(v->GetName(), "") == 0){
			char name[256]="";
			if(frame)
				sprintf(name, "%s_frame%d", GetName(), children.size()-1);
			if(mat)
				sprintf(name, "%s_mat%d", GetName(), children.size()-1);
			if(light)
				sprintf(name, "%s_light%d", GetName(), children.size()-1);
			if(mesh)
				sprintf(name, "%s_mesh%d", GetName(), children.size()-1);
			v->SetName(name);
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
//	GRDummyFrame
//
IF_OBJECT_IMP(GRDummyFrame, GRVisual);
GRDummyFrame::GRDummyFrame(const GRDummyFrameDesc& desc):GRDummyFrameDesc(desc){	
}

bool GRDummyFrame::AddChildObject(ObjectIf* o){
	GRVisualIf* v = o->Cast();
	if (v){
		children.push_back(v);
		return true;
	}
	return false;
}
bool GRDummyFrame::DelChildObject(ObjectIf* v){
	for(GRVisualIfs::iterator it = children.begin(); it != children.end(); ++it){
		if (*it == v){
			children.erase(it);
			return true;
		}
	}
	return false;
}
size_t GRDummyFrame::NChildObject() const{ return children.size(); }
ObjectIf* GRDummyFrame::GetChildObject(size_t pos){
	if (pos >= children.size()) return NULL; 
	return children[pos];
}

//-----------------------------------------------------------------
//	GRAnimation
//
IF_OBJECT_IMP(GRAnimation, SceneObject);
void GRAnimation::BlendPose(float time, float weight){
	//	ターゲットに変換を加える
	Affinef transform;
	for(std::vector<GRAnimationKey>::iterator it = keys.begin(); it != keys.end(); ++it){
		GRAnimationKey& anim = *it;
		//	時刻でキーを検索
		for(unsigned i=0; i < anim.keys.size(); ++i){
			float blended[16];
			if (anim.keys[i].time > time){	//	見つかったのでブレンドした変換を計算
				if (i==0){	//	i=0だけをセット
					for(unsigned v=0; v<anim.keys[i].values.size(); ++v){
						blended[v] = anim.keys[i].values[v];
					}
				}else{		//	i-1とiをブレンド
					float k = (anim.keys[i].time - time) / 
							(anim.keys[i].time - anim.keys[i-1].time);
					for(unsigned v=0; v<anim.keys[i].values.size(); ++v){
						blended[v] = (1-k) * anim.keys[i].values[v]
							+ k * anim.keys[i-1].values[v];
					}						
				}
				//	見つかった変換をtransformに適用
				switch(anim.keyType){
					case GRAnimationDesc::ROTATION:{
						Affinef mat;
						//((Quaternionf*)blended)->ToMatrix(mat);
						Quaternionf q = *((Quaternionf*) blended);
						q.w*=-1;
						q.ToMatrix(mat.Rot());
						transform = mat * transform;
						}break;
					case GRAnimationDesc::SCALE:
						transform.Ex() *= blended[0];
						transform.Ey() *= blended[1];
						transform.Ez() *= blended[2];
						break;
					case GRAnimationDesc::POSITION:
						transform.Pos()[0] += blended[0];
						transform.Pos()[1] += blended[1];
						transform.Pos()[2] += blended[2];
						break;
					case GRAnimationDesc::MATRIX:
//						transform =  *((Affinef*)blended) * transform ;
						break;
				}
				break;
			}
		}
	}
	//	transform をターゲットに適用
	for(Targets::iterator it = targets.begin(); it!= targets.end(); ++it){
		it->target->SetTransform(transform);
	}
}
void GRAnimation::ResetPose(){
	//	transform を初期値に戻す
	for(Targets::iterator it = targets.begin(); it!= targets.end(); ++it){
		it->target->SetTransform(it->initalTransform);
	}
}
void GRAnimation::LoadInitialPose(){
	//	フレームの変換に初期値を設定する
	for(Targets::iterator it = targets.begin(); it!= targets.end(); ++it){
		it->initalTransform = it->target->GetTransform();
	}
}
bool GRAnimation::AddChildObject(ObjectIf* o){
	GRFrame* fr = o->Cast();
	if (fr){
		targets.push_back(Target());
		targets.back().target = fr->Cast();
		targets.back().initalTransform = fr->GetTransform();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------
//	GRAnimationSet
//
IF_OBJECT_IMP(GRAnimationSet, SceneObject);
bool GRAnimationSet::AddChildObject(ObjectIf* o){
	GRAnimation* ani = o->Cast();
	if (ani){
		animations.push_back(ani);
		return true;
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
void GRAnimationSet::BlendPose(float time, float weight){
	for (Animations::iterator it = animations.begin(); it != animations.end(); ++it){
		(*it)->BlendPose(time, weight);
	}
}
void GRAnimationSet::ResetPose(){
	for (Animations::iterator it = animations.begin(); it != animations.end(); ++it){
		(*it)->ResetPose();
	}	
}
void GRAnimationSet::LoadInitialPose(){
	for (Animations::iterator it = animations.begin(); it != animations.end(); ++it){
		(*it)->LoadInitialPose();
	}
}


IF_OBJECT_IMP(GRAnimationController, SceneObject);
bool GRAnimationController::AddChildObject(ObjectIf* o){
	GRAnimationSet* ani = o->Cast();
	if (ani){
		sets.insert(std::make_pair(ani->GetName(), ani));
		return true;
	}
	return false;
}

bool GRAnimationController::DelChildObject(ObjectIf* o){
	GRAnimationSet* ani = o->Cast();
	if (ani){
		Sets::iterator it = sets.find(ani->GetName());
		if (it->second == o->Cast()){
			sets.erase(it);
			return true;
		}
	}
	return false;
}
int GRAnimationController::NChildObject(){
	return sets.size();
}

ObjectIf* GRAnimationController::GetChildObject(size_t p){
	Sets::iterator it = sets.begin();
	for(unsigned i=0; i<p; ++i) ++it;
	return it->second->Cast();
}
void GRAnimationController::BlendPose(UTString name, float time, float weight){
	Sets::iterator it = sets.find(name);
	if (it != sets.end()){
		it->second->BlendPose(time, weight);
	}
}
void GRAnimationController::ResetPose(){
	for(Sets::iterator it = sets.begin(); it != sets.end(); ++it){
		it->second->ResetPose();
	}
}
void GRAnimationController::LoadInitialPose(){
	for(Sets::iterator it = sets.begin(); it != sets.end(); ++it){
		it->second->LoadInitialPose();
	}
}


}
