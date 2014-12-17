/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Graphics/GRFrame.h>
#include <Graphics/GRScene.h>
#include <Graphics/GRMesh.h>

using namespace std;

namespace Spr{;

//-----------------------------------------------------------------
//	GRFrame
//
GRFrame::GRFrame(const GRFrameDesc& desc):GRFrameDesc(desc){
	parent = NULL;

}
SceneObjectIf* GRFrame::CloneObject(){
	GRFrameDesc desc;
	GRFrameIf* origin = DCAST(GRFrameIf,this);
	origin ->GetDesc(&desc);
	GRScene* s = DCAST(GRScene, GetScene());
	GRFrameIf* clone = s->CreateVisual(desc.GetIfInfo(), desc)->Cast();
	for (unsigned int i=0; i < origin->NChildObject(); ++i) {
		clone->AddChildObject(origin->GetChildObject(i));
	}
	return clone;
}
void GRFrame::Render(GRRenderIf* r){
	if(!enabled)
		return;
	r->PushModelMatrix();
	r->MultModelMatrix(transform);
	
	/// light -> material -> mesh -> その他visual -> 子frame　の順にRenderを呼ぶ
	/// Renderedはその逆順で呼ぶ
	for(vector<GRLight*>::iterator it = lights.begin(); it != lights.end(); it++)
		(*it)->Render(r);
	for(vector<GRMaterial*>::iterator it = materials.begin(); it != materials.end(); it++)
		(*it)->Render(r);
	for(vector<GRMesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
		(*it)->Render(r);
	for(vector<GRVisual*>::iterator it = miscs.begin(); it != miscs.end(); it++)
		(*it)->Render(r);
	for(vector<GRFrame*>::iterator it = frames.begin(); it != frames.end(); it++)
		(*it)->Render(r);
	
	for(vector<GRFrame*>::iterator it = frames.begin(); it != frames.end(); it++)
		(*it)->Rendered(r);
	for(vector<GRVisual*>::iterator it = miscs.begin(); it != miscs.end(); it++)
		(*it)->Rendered(r);
	for(vector<GRMesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
		(*it)->Rendered(r);
	for(vector<GRMaterial*>::iterator it = materials.begin(); it != materials.end(); it++)
		(*it)->Rendered(r);
	for(vector<GRLight*>::iterator it = lights.begin(); it != lights.end(); it++)
		(*it)->Rendered(r);
	
	//for(GRVisualIfs::iterator it = children.begin(); it != children.end(); ++it){
	//	(*it)->Render(r);
	//}
	//for(GRVisualIfs::reverse_iterator it = children.rbegin(); it != children.rend(); ++it){
	//	(*it)->Rendered(r);
	//}
	
	r->PopModelMatrix();
}
void GRFrame::Rendered(GRRenderIf* r){
}
void GRFrame::SetParent(GRFrameIf* fr){
	if((GRFrameIf*)(parent->Cast()) == fr) return;
	UTRef<GRFrame> tmp = this->Cast();	// delete対策
	if (fr){
		fr->AddChildObject(this->Cast());
	}
}
bool GRFrame::AddChildObject(ObjectIf* o){
	GRVisual* v = o->Cast();
	if (v){
		v->SetNameManager(GetNameManager());
		children.push_back(v->Cast());
		GRFrame*	frame	= DCAST(GRFrame, v);
		GRMaterial* mat		= DCAST(GRMaterial, v);
		GRLight*	light	= DCAST(GRLight, v);
		GRMesh*		mesh	= DCAST(GRMesh, v);
		if (frame && frame->parent != this){
			//	ここで元の持ち主から削除するのはやりすぎでは？	by tazaki ?
			//	 -> いや、Frame は parentがひとつなので、複数のフレームの子になるのはだめです。 by hase
			if (frame->parent) frame->parent->DelChildObject(frame->Cast());
			frame->parent = this;
			frames.push_back(frame);
		}
		else if(mat)
			materials.push_back(mat);
		else if(light)
			lights.push_back(light);
		else if(mesh)
			meshes.push_back(mesh);
		else miscs.push_back(v);

		// デフォルトネーム設定
		if(strcmp(v->GetName(), "") == 0){
			char name[256]="";
			if(frame)
				sprintf(name, "%s_frame%d", GetName(), (int)frames.size()-1);
			else if(mat)
				sprintf(name, "%s_mat%d", GetName(), (int)materials.size()-1);
			else if(light)
				sprintf(name, "%s_light%d", GetName(), (int)lights.size()-1);
			else if(mesh)
				sprintf(name, "%s_mesh%d", GetName(), (int)meshes.size()-1);
			else sprintf(name, "%s_misc%d", GetName(), (int)miscs.size()-1);
			v->SetName(name);
		}
		return true;
	}
	return false;
}
bool GRFrame::DelChildObject(ObjectIf* o){
	
	GRVisualIfs::iterator it = find(children.begin(), children.end(), o);
	if(it != children.end()){

		// 種類別の参照を削除
		GRVisual*	v		= o->Cast();
		GRFrame*	frame	= DCAST(GRFrame, v);
		GRMaterial* mat		= DCAST(GRMaterial, v);
		GRLight*	light	= DCAST(GRLight, v);
		GRMesh*		mesh	= DCAST(GRMesh, v);

		if(frame)
			frames.erase(find(frames.begin(), frames.end(), frame));
		else if(mat)
			materials.erase(find(materials.begin(), materials.end(), mat));
		else if(light)
			lights.erase(find(lights.begin(), lights.end(), light));
		else if(mesh)
			meshes.erase(find(meshes.begin(), meshes.end(), mesh));
		else miscs.erase(find(miscs.begin(), miscs.end(), v));
	
		// 最後にオブジェクトを削除
		children.erase(it);

		return true;
	
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

void GRFrame::AddRBFKeyFrame(PTM::VVector<float> pos){
	Affinef aff = GetTransform();
	// std::cout << "Aff : " << aff << std::endl;

	// キーフレーム座標を追加
	kfPositions.push_back(pos);

	// 次元の取得
	int nKeys	= (int)kfPositions.size();
	int nKeyDim	= (int)pos.size();
	int nMatDim	= nKeys + 1 + nKeyDim;

	// 目標値ベクトルの次元・値設定と係数ベクトルの次元設定
	for (int i=0; i<4; ++i) {
		for (int j=0; j<4; ++j) {
			PTM::VVector<float> p; p.resize(kfAffines[i][j].size()); p = kfAffines[i][j];
			kfAffines[i][j].resize(nMatDim);
			for (unsigned q=0; q<p.size(); ++q) {
				kfAffines[i][j][q] = p[q];
			}
			kfAffines[i][j][nKeys-1] = aff[i][j];
			for (int k=nKeys+1; k<nMatDim; ++k) {
				kfAffines[i][j][k] = 0;
			}
			kfCoeffs[i][j].resize(nMatDim);
		}
	}

	// 計算用行列の用意
	PTM::VMatrixCol<float> A;	A.resize(nMatDim, nMatDim);
	/// 左上
	for (int i=0; i<nKeys; ++i) {
		for (int j=i; j<nKeys; ++j) {
			A[i][j] = A[j][i] = (kfPositions[i] - kfPositions[j]).norm();
		}
	}
	/// 右上中・左下中
	for (int i=nKeys; i<nKeys+1; ++i) {
		for (int j=0; j<nKeys; ++j) {
			A[i][j] = A[j][i] = 1;
		}
	}
	/// 右上・左下
	for (int i=nKeys+1; i<nMatDim; ++i) {
		for (int j=0; j<nKeys; ++j) {
			int m = i - (nKeys+1);
			A[i][j] = A[j][i] = kfPositions[j][m];
		}
	}
	/// 右下
	for (int i=nKeys; i<nMatDim; ++i) {
		for (int j=nKeys; j<nMatDim; ++j) {
			A[i][j] = 0;
		}
	}

	// 逆行列計算
	A = A.inv();

	// 係数ベクトルの計算
	for (int i=0; i<4; ++i) {
		for (int j=0; j<4; ++j) {
			kfCoeffs[i][j] = A * kfAffines[i][j];
			/*
			std::cout << "A : " << A << std::endl;
			std::cout << "kAf : " << kfAffines[i][j] << std::endl;
			std::cout << "kfC : " << kfCoeffs[i][j] << std::endl;
			*/
		}
	}

	// 子要素に再帰
	for (unsigned i=0; i<children.size(); ++i) {
		GRFrame* fr = children[i]->Cast();
		if (fr) {
			fr->AddRBFKeyFrame(pos);
		}
	}
}

void GRFrame::BlendRBF(PTM::VVector<float> pos){
	Affinef aff;

	// 次元の取得
	int nKeys	= (int)kfPositions.size();
	int nKeyDim	= (int)pos.size();
	int nMatDim	= nKeys + 1 + nKeyDim;

	// 計算用ベクトルの用意
	PTM::VVector<float> input; input.resize(nMatDim);
	/// 左
	for (int i=0; i<nKeys; ++i) {
		input[i] = (kfPositions[i] - pos).norm();
	}
	/// 中
	for (int i=nKeys; i<nKeys+1; ++i) {
		input[i] = 1;
	}
	/// 右
	for (int i=nKeys+1; i<nMatDim; ++i) {
		int m = i - (nKeys+1);
		input[i] = pos[m];
	}

	// ブレンド値の計算
	// std::cout << "inp : " << input << std::endl;
	for (int i=0; i<4; ++i) {
		for (int j=0; j<4; ++j) {
			aff[i][j] = PTM::dot(kfCoeffs[i][j], input);
			// std::cout << "kfC : " << kfCoeffs[i][j] << std::endl;
		}
	}

	// 正規直交化
	aff.Ex() /= aff.Ex().norm();
	aff.Ey() /= aff.Ey().norm();
	aff.Ez() /= aff.Ez().norm();
	for (int i=0; i<10; ++i) {
		Vec3f u = PTM::cross(aff.Ey(), aff.Ez());
		Vec3f v = PTM::cross(aff.Ez(), aff.Ex());
		Vec3f w = PTM::cross(aff.Ex(), aff.Ey());
		aff.Ex() = (aff.Ex()+u)/2.0f;
		aff.Ey() = (aff.Ey()+v)/2.0f;
		aff.Ez() = (aff.Ez()+w)/2.0f;
		aff.Ex() /= aff.Ex().norm();
		aff.Ey() /= aff.Ey().norm();
		aff.Ez() /= aff.Ez().norm();
		float r = 0.0f;
		r += PTM::dot(aff.Ex(), aff.Ey())*PTM::dot(aff.Ex(), aff.Ey());
		r += PTM::dot(aff.Ey(), aff.Ez())*PTM::dot(aff.Ey(), aff.Ez());
		r += PTM::dot(aff.Ez(), aff.Ex())*PTM::dot(aff.Ez(), aff.Ex());
		if (r < 0.000001) {
			break;
		}
	}

	// 変換行列のセット
	SetTransform(aff);
	// std::cout << this->GetName() << std::endl;
	// std::cout << "Aff : " << aff << std::endl;

	// 子要素に再帰
	for (unsigned  i=0; i<children.size(); ++i) {
		GRFrame* fr = children[i]->Cast();
		if (fr) {
			fr->BlendRBF(pos);
		}
	}
}

bool GRFrame::CalcBBox(Vec3f& bbmin, Vec3f& bbmax, const Affinef& aff){
	bool ok = false;
	Affinef a = aff * transform;
	for(unsigned i = 0; i < children.size(); i++){
		GRFrame* frame = children[i]->Cast();
		if(frame){
			ok |= frame->CalcBBox(bbmin, bbmax, a);
		}
		GRMesh* mesh = children[i]->Cast();
		if(mesh){
			mesh->CalcBBox(bbmin, bbmax, a);
			ok = true;
		}
	}
	return ok;
}

//-----------------------------------------------------------------
//	GRDummyFrame
//
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
void GRAnimation::BlendPose(float time, float weight){
	BlendPose(time, weight, false);
}
void GRAnimation::BlendPose(float time, float weight, bool add){
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
	transform.Orthonormalization(); //正規直交化
	for(Targets::iterator it = targets.begin(); it!= targets.end(); ++it){
		UTString name = it->target->GetName();
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

GRAnimationKey GRAnimation::GetAnimationKey(int n){
	if(n < (int)keys.size()){
		return keys[n];
	}else{
		DSTR<< "GetAnimationKey: index out of range" << std::endl;
		return GRAnimationKey();
	}
}
int GRAnimation::NAnimationKey(){
	return (int)keys.size();
}

void GRAnimation::SetCurrentPose(float t){
	//同一の時刻のKeyがある場合削除
	DeletePose(t);
	//GRAnimationKeyが存在する場合Keyを作成
	if(keys.size()>0){
		//ROTATIONの指定
			GRKey rotationKey;
			rotationKey.time = t;
			Affinef af = targets[0].target->GetTransform();
			af.Orthonormalization();
			Quaterniond q; q.FromMatrix(af.Rot());
			q.w *=-1;
			//クォータニオンに代入
			for(int i=0; i<4; i++){
				rotationKey.values.push_back(q[i]);
			}
			keys[0].keys.push_back(rotationKey);
		//SCALEの指定
			GRKey scaleKey;
			scaleKey.time = t;
			//3変数のベクトルを代入
			scaleKey.values =keys[1].keys[0].values; //0番目のKeyをコピー
			keys[1].keys.push_back(scaleKey);
		//POSITIONの指定(RootとなるKeyのみ位置が変わる)
			GRKey positionKey;
			positionKey.time = t;
			if(!(targets[0].target->GetParent()->GetParent()->GetParent()->GetParent())){
				//Rootの場合
				Vec3d trn = targets[0].target->GetTransform().Trn();
				for(int i=0; i<3; i++){
					positionKey.values.push_back(trn[i]);
				}
			}else{
				//3変数のベクトルを代入
				positionKey.values =keys[2].keys[0].values; //0番目のKeyをコピー
			}
			keys[2].keys.push_back(positionKey);
	}
	SortGRKey();
}
void GRAnimation::DeletePose(float t){
	//GRAnimationKeyが存在する場合Keyを作成
	if(keys.size()>0){
		for(int i=0; i<3 ; i++){
			std::vector<GRKey>::iterator it;
			std::vector<GRKey>::iterator eraseit;
			bool timeFlag = false;
			for(it =keys[i].keys.begin();it!= keys[i].keys.end(); ++it){
				if(it->time==t){
					eraseit = it;
					timeFlag = true;
					continue;
				}
			}
			if(timeFlag)keys[i].keys.erase(eraseit);
		}
	SortGRKey();
	}
}
//GRKeyのソート用関数
bool cmp(GRKey a, GRKey b){
	return a.time < b.time;
}

void GRAnimation::SortGRKey(){
	//keys[0].keys,keys[1].keys,keys[2].keysを時間でソート
	for(unsigned i=0; i<keys.size(); i++){
		std::sort( keys[i].keys.begin(), keys[i].keys.end() ,cmp) ;
	}
}

float GRAnimation::GetLastKeyTime(){
	SortGRKey();
	float lastKeyTime;
	if(keys[0].keys.size()>0){
		//ソート後の最後の時間を取得
		lastKeyTime =keys[0].keys.back().time;
	}
	return lastKeyTime;
}
//-----------------------------------------------------------------
//	GRAnimationSet
//
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
size_t GRAnimationSet::NChildObject() const{
	return animations.size();
}


ObjectIf* GRAnimationSet::GetChildObject(size_t p){
	return animations[p]->Cast();
}
void GRAnimationSet::BlendPose(float time, float weight){
	BlendPose(time, weight, false);
}
void GRAnimationSet::BlendPose(float time, float weight, bool add){
	for (Animations::iterator it = animations.begin(); it != animations.end(); ++it){
		(*it)->BlendPose(time, weight, add);
	}
	UpdateLastKeyTime();
}
void GRAnimationSet::ResetPose(){
	for (Animations::iterator it = animations.begin(); it != animations.end(); ++it){
		(*it)->ResetPose();
	}
	UpdateLastKeyTime();
}
void GRAnimationSet::LoadInitialPose(){
	for (Animations::iterator it = animations.begin(); it != animations.end(); ++it){
		(*it)->LoadInitialPose();
	}
	UpdateLastKeyTime();
}

void GRAnimationSet::SetCurrentAnimationPose(float t){
	for(size_t i = 0; i<animations.size(); i++){
		animations[i]->SetCurrentPose(t);
	}
	UpdateLastKeyTime();
}
void GRAnimationSet::DeleteAnimationPose(float t){
	for(size_t i = 0; i<animations.size(); i++){
		animations[i]->DeletePose(t);
	}
	UpdateLastKeyTime();
}

void GRAnimationSet::UpdateLastKeyTime(){
	//lastKeyTimeは全てのGRAnimationのLastKeyTimeが同一であると仮定して取得
	lastKeyTime = animations[0]->GetLastKeyTime();
}
float GRAnimationSet::GetLastKeyTime(){
	return lastKeyTime;
}
//-----------------------------------------------------------------
//	GRAnimationController
//
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
		if (it->second == ani){
			sets.erase(it);
			return true;
		}
	}
	return false;
}
size_t GRAnimationController::NChildObject() const{
	return sets.size();
}

ObjectIf* GRAnimationController::GetChildObject(size_t p){
	Sets::iterator it = sets.begin();
	for(unsigned i=0; i<p; ++i) ++it;
	return it->second->Cast();
}
GRAnimationSetIf* GRAnimationController::GetAnimationSet(size_t p){
	Sets::iterator it = sets.begin();
	for(unsigned i=0; i<p; ++i) ++it;
	return it->second->Cast();
}
void GRAnimationController::BlendPose(UTString name, float time, float weight){
	BlendPose(name, time, weight, false);
}
void GRAnimationController::BlendPose(UTString name, float time, float weight, bool add){
	Sets::iterator it = sets.find(name);
	if (it != sets.end()){
		it->second->BlendPose(time, weight, add);
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
