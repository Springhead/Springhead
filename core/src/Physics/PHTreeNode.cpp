/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Physics/PHTreeNode.h>
#include <Physics/PHConstraintEngine.h>
#include <Physics/PHBallJoint.h>

using namespace std;
using namespace PTM;
namespace Spr{;

//-----------------------------------------------------------------------------

PHTreeNode::PHTreeNode(){
	joint  = 0;
	solid  = 0;
}

bool PHTreeNode::AddChildObject(ObjectIf* o){
	PHJointIf* j = DCAST(PHJointIf, o);
	if(j){
		joint = j->Cast();
		joint->treeNode = this;
		solid = joint->solid[1];
		solid->treeNode = this;
		return true;
	}
	PHTreeNode* n = DCAST(PHTreeNode, o);
	if(n){
		n->SetScene(GetScene());
		n->root = root;
		root->bReady = false;
		
		char name[256];
		if(strcmp(n->GetName(), "") == 0){
			sprintf(name, "%s%d", GetName(), (int)Children().size());
			n->SetName(name);
		}
		
		AddChild(n);
		
		// 有効/無効状態を親に合わせる
		n->Enable(IsEnabled());
		
		return true;
	}
	return false;
}

size_t PHTreeNode::NChildObject() const{
	return (joint ? 1 : 0) + Children().size();
}

ObjectIf* PHTreeNode::GetChildObject(size_t i){
	if(joint){
		if(i == 0)
			return joint->Cast();
		i--;
	}
	return Children()[i]->Cast();
}

void PHTreeNode::Enable(bool on){
	bEnabled = on;

	// 切り替え時にばたつかないように拘束力をクリア
	if(joint)
		joint->f.clear();

	// 子ノードも同時に切り替える
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->Enable(on);
}

PHRootNodeIf* PHTreeNode::GetRootNode(){
	return root->Cast();
}

bool PHTreeNode::Includes(PHTreeNode* node){
	if(node == this)
		return true;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		if((*it)->Includes(node))
			return true;
	return false;
}

PHTreeNode*	PHTreeNode::FindBySolid(PHSolid* s){
	if(solid == s)
		return this;
	PHTreeNode* node;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++){
		node = (*it)->FindBySolid(s);
		if(node)
			return node;
	}
	return NULL;
}

PHTreeNode*	PHTreeNode::FindByJoint(PHJoint* j){
	if(joint == j)
		return this;
	PHTreeNode* node;
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++){
		node = (*it)->FindByJoint(j);
		if(node)
			return node;
	}
	return NULL;
}

void PHTreeNode::AssignID(std::vector<PHTreeNode*>& n){
	id = (int)n.size();
	n.push_back(this);
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->AssignID(n);
}

PHScene* PHTreeNode::GetPHScene(){
	return DCAST(PHScene, GetScene());
}

void PHTreeNode::Prepare(){
	// 配列のサイズ確保
	int n = (int)root->nodes.size();
	dZdv_map.resize(n);
	
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->Prepare();
}

void PHTreeNode::PrepareGear(PHGears& gears){
	int n = (int)Children().size();

	// 子ノード初期化
	for(int i = 0; i < n; i++)
		Children()[i]->ResetGear();

	// 親ノード（つまり自分）とギアでつながる子ノードを列挙
	for(int i = 0; i < n; i++){
		PHTreeNode* child = Children()[i];
		PHGear* g = gears.FindByJointPair(
			DCAST(PH1DJoint, joint),
			DCAST(PH1DJoint, child->joint)
			);
		if(g)
			child->AddToGearTrain(g, this, true);
	}

	// 子ノード間のギアを順次列挙
	PHTreeNode *c0, *c1;
	while(true){
		// すでにギアトレインに含まれているノードとギアでつながっているノードを順次列挙
		bool found = true;
		while(found){
			found = false;
			for(int i0 = 0; i0 < n-1; i0++)for(int i1 = i0+1; i1 < n; i1++){
				c0 = Children()[i0];
				c1 = Children()[i1];
				if(c0->GetGearRoot() && c1->GetGearRoot())
					continue;
				if(!c0->GetGearRoot() && !c1->GetGearRoot())
					continue;
			
				PHGear* g = gears.FindByJointPair(
					DCAST(PH1DJoint, c0->joint),
					DCAST(PH1DJoint, c1->joint)
					);
				if(!g)
					continue;
				found = true;
				if(c0->GetGearRoot())
					 c1->AddToGearTrain(g, c0, false);
				else c0->AddToGearTrain(g, c1, false);
			}
		}
		
		// まだつながっていないノードから新しいギアトレインの起点を探す
		found = false;
		for(int i0 = 0; i0 < n-1; i0++)for(int i1 = i0+1; i1 < n; i1++){
			c0 = Children()[i0];
			c1 = Children()[i1];
			if(c0->GetGearRoot() || c1->GetGearRoot())
				continue;
			PHGear* g = gears.FindByJointPair(
				DCAST(PH1DJoint, c0->joint),
				DCAST(PH1DJoint, c1->joint)
				);
			if(!g)
				continue;
			found = true;
			c1->AddToGearTrain(g, c0, false);
			
			// 起点が一つ見つかったらbreak
			i0 = n-1; break;
		}
		if(!found)
			break;
	}

	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->PrepareGear(gears);
}

void PHTreeNode::PrepareDepth(){
	PHTreeNode* par = GetParentOfGearTrain();
	if(!par)
		 depth = 0;
	else depth = par->depth + 1;

	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->PrepareDepth();
}

void PHTreeNode::CompSpatialTransform(){
	Xcp = joint->Xj[1].inv() * joint->Xjrel * joint->Xj[0];
	Xcj = joint->Xj[1].inv() * SpatialTransform(Vec3d(), joint->Xjrel.q);
	Xcp_mat = Xcp;
	Xcj_mat = Xcj;
}

void PHTreeNode::CompCoriolisAccel(){
	PHSolid*          sp    = parent->solid;
	PHSolid*          s     = solid;
	SpatialVector&    vp    = sp->v;
	SpatialVector&    v     = s->v;
	SpatialVector&    vjrel = joint->vjrel;
	SpatialTransform& Xj0   = joint->Xj[0];
	SpatialTransform& Xj1   = joint->Xj[1];
	SpatialTransform& Xjrel = joint->Xjrel;
	SpatialVector	  vj    = joint->Xj[0] * vp;

	SpatialVector cpj, cjj, ccj;
	Vec3d vpw = vp.w();
	cpj.v() = (vpw * Xj0.r) * vpw - vpw.square() * Xj0.r;
	cpj.w() = Vec3d();

	Vec3d vjw = vj.w();
	cjj.v() = (vjw * Xjrel.r) * vjw - vjw.square() * Xjrel.r + 2 * vjw % vjrel.v();
	cjj.w() = vjw % vjrel.w();

	Vec3d vw = v.w();
	ccj.v() = (vw * Xj1.r) * vw - vw.square() * Xj1.r;
	ccj.w() = Vec3d();

	CompJointCoriolisAccel();
	c = Xj1.inv() * (
		  Xjrel * SpatialTransform(Vec3d(), Xj0  .q) * cpj
		        + SpatialTransform(Vec3d(), Xjrel.q) * cjj
		        - SpatialTransform(Vec3d(), Xj1  .q) * ccj);
	c += Xcj * cj;
	c *= GetPHScene()->GetTimeStep();
}

void PHTreeNode::InitArticulatedInertia(){
	double m = solid->GetMass();
	I.vv() = Matrix3d::Diag(m, m, m);
	I.vw() = Matrix3d::Zero();
	I.wv() = Matrix3d::Zero();
	I.ww() = solid->GetInertia();
}

void PHTreeNode::CompArticulatedInertia(){
	CompSpatialTransform();
	CompJointJacobian   ();
	CompCoriolisAccel   ();
	
	//子ノードにIaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のIaを積み上げる
	//＊ギアの並列連動においてギアトレインの先頭ノード（最もbegin寄りにいる）を最後に呼ぶ必要があるため
	//  reverse_iteratorを用いる
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->InitArticulatedInertia();
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->CompArticulatedInertia();

	Ic = I * c;

	//親ノードにIaを積む
	AccumulateInertia();
}

void PHTreeNode::InitArticulatedBiasForce(){
	PHSolid* s = solid;
	if(s->IsDynamical()){
		Z.v() = -1.0 * s->f.v();
		Z.w() = -1.0 * s->f.w() + s->v.w() % (s->GetInertia() * s->v.w());
		Z *= GetPHScene()->GetTimeStep();
	}
	else Z.clear();
}

void PHTreeNode::CompArticulatedBiasForce(){
	//子ノードにZaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のZaを積み上げる
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->InitArticulatedBiasForce();
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->CompArticulatedBiasForce();

	ZplusIc = Z + Ic;
	cout << "ZplusIc " << solid->GetName() << " " << ZplusIc << endl;
	//親ノードにZaを積む
	AccumulateBiasForce();
}

void PHTreeNode::CompResponseMatrix(){
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompResponseMatrix();
}

void PHTreeNode::CompResponse(const SpatialVector& df){
	for(int i = 0; i < (int)root->nodes.size(); i++)
		root->nodes[i]->CompResponse(this, df);
}

void PHTreeNode::CompResponseCorrection(const SpatialVector& dF){
	for(int i = 0; i < (int)root->nodes.size(); i++)
		root->nodes[i]->CompResponseCorrection(this, dF);
}

double PHTreeNode::GetResponse(PHTreeNode* src, const SpatialVector& J, const SpatialVector& df){
	return - QuadForm(J, dZdv_map[src->id], df);
}

void PHTreeNode::CompResponse(PHTreeNode* src, const SpatialVector& df){
	if(solid->IsDynamical())
		solid->dv += dZdv_map[src->id] * (-df);
}

void PHTreeNode::CompResponseCorrection(PHTreeNode* src, const SpatialVector& dF){
	if(solid->IsDynamical())
		solid->dV += dZdv_map[src->id] * (-dF);
}

void PHTreeNode::UpdateVelocity(double* dt){
	PHSolid* sp = parent->solid;
	PHSolid* s  = solid;
	UpdateJointVelocity ();
	CompRelativeVelocity();
	s->v = Xcp * sp->v + Xcj * joint->vjrel;
	s->SetVelocity       (s->GetOrientation() * s->v.v());
	s->SetAngularVelocity(s->GetOrientation() * s->v.w());
	
	// 位置更新のステップ幅を計算
	double dpmax = GetPHScene()->GetMaxDeltaPosition();
	double dqmax = GetPHScene()->GetMaxDeltaOrientation();
	if(s->velocityNorm * (*dt) > dpmax)
		*dt = dpmax/s->velocityNorm;
	if(s->angVelocityNorm * (*dt) > dqmax)
		*dt = dqmax/s->angVelocityNorm;


	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdateVelocity(dt);
}

void PHTreeNode::UpdatePosition(double dt){
	PHSolid* sp = parent->solid;
	PHSolid* s  = solid;
	UpdateJointPosition(dt);
	CompRelativePosition();
	CompSpatialTransform();
	SpatialTransform Xp(sp->GetCenterPosition(), sp->GetOrientation());
	SpatialTransform Xc = Xcp * Xp;
	Xc.q.unitize();
	// SetOrientation -> SetCenterPositionの順に呼ぶ必要がある．逆だとSetOrientationによって重心位置がずれてしまう tazz
	s->SetOrientation(Xc.q);
	s->SetCenterPosition(Xc.r);

	s->SetVelocity       (s->GetOrientation() * s->v.v());
	s->SetAngularVelocity(s->GetOrientation() * s->v.w());

	s->SetUpdated(true);
	
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdatePosition(dt);
}

//-----------------------------------------------------------------------------

PHRootNode::PHRootNode(const PHRootNodeDesc& desc){
	root   = this;
	bReady = false;
}

bool PHRootNode::AddChildObject(ObjectIf* o){
	if(PHTreeNode::AddChildObject(o))
		return true;
	PHSolidIf* s = DCAST(PHSolidIf, o);
	if(s){
		solid = s->Cast();
		solid->treeNode = this;
		return true;
	}
	return false;
}

size_t PHRootNode::NChildObject() const{
	return (solid ? 1 : 0) + Children().size();
}

ObjectIf* PHRootNode::GetChildObject(size_t i){
	if(solid){
		if(i == 0)
			return solid->Cast();
		i--;
	}
	return Children()[i]->Cast();
}

void PHRootNode::Setup(){
	if(!bReady){
		nodes.clear ();
		AssignID    (nodes);
		Prepare     ();
		PrepareGear (engine->gears);

		//for(int i = 0; i < (int)nodes.size(); i++){
		//	DSTR << nodes[i]->id;
		//	if(nodes[i]->GetParent())
		//		DSTR << " -> " << nodes[i]->GetParent()->id;
		//	PHTreeNode1D* node1D = nodes[i]->Cast();
		//	if(node1D){
		//		if(node1D->gearRoot)
		//			DSTR << " gr: " << node1D->gearRoot->id;
		//		if(node1D->gearUpper)
		//			DSTR << " gu: " << node1D->gearUpper->id;
		//	}
		//	DSTR << endl;
		//}

		PrepareDepth();
		bReady = true;
	}

	// articulated inertiaを計算
	InitArticulatedInertia();
	CompArticulatedInertia();

	// コールバック
	if (compControlForce != NULL) {
		compControlForce(this->GetObjectIf()->Cast(), arg);
	}


	double dt = engine->GetScene()->GetTimeStep();

	// 親のコリオリ加速度をsolid->dv経由で子に入れてる部分を
	SpatialVector cForce2 = SpatialVector(Vec3d(0,0,0),Vec3d(0,0,0));
	for (container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++) {
		PHTreeNode* treeNode = (*it);
		PHBallJointNode* ballJointNode = (PHBallJointNode*)(treeNode);
		SpatialVector cForce = SpatialVector(Vec3d(0,0,0),Vec3d(0,0,0));

		for (container_t::reverse_iterator it2 = ballJointNode->Children().rbegin(); it2 != ballJointNode->Children().rend(); it2++) {
			PHTreeNode* treeNode2 = (*it2);
			PHBallJointNode* ballJointNode2 = (PHBallJointNode*)(treeNode2);
			cout << "ballJointNode2->I * ballJointNode2->Xcp * ballJointNode->c " << ballJointNode2->I * ballJointNode2->Xcp * ballJointNode->c << endl;
			cForce2 += ballJointNode2->I * ballJointNode2->Xcp* ballJointNode->c / dt;
			//cForce2 += ballJointNode2->ConvertParentC(ballJointNode->c);
			//ballJointNode2->solid->AddForce(ballJointNode2->solid->GetOrientation() * cForce2.v());
			//ballJointNode2->solid->AddTorque(ballJointNode2->solid->GetOrientation() * cForce2.w());

			SpatialVector PlusedChildIc2 = SpatialVector(Vec3d(0, 0, 0), Vec3d(0, 0, 0));
			PlusedChildIc2 += -(ballJointNode2->Xcp_mat.trans() * (cForce2)+ballJointNode2->XtrIJ_JIJinv * (-ballJointNode2->J.trans() * (cForce2))); //ここ違う気がする
			//PlusedChildIc2 += ballJointNode2->ConvertChild2Parent(cForce2);
			cout << ballJointNode2->solid->GetName() << " PlusedChildIc2 " << PlusedChildIc2 << endl;
			PHSolid* parentSolid = ballJointNode2->GetParent()->solid;
			//parentSolid->AddForce(parentSolid->GetOrientation() * PlusedChildIc2.v());
			//parentSolid->AddTorque(parentSolid->GetOrientation() * PlusedChildIc2.w());
		}
	}

	for (container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++) {
		PHTreeNode* treeNode = (*it);
		PHBallJointNode* ballJointNode = (PHBallJointNode*)(treeNode);

		SpatialVector PlusedChildIc = SpatialVector(Vec3d(0,0,0),Vec3d(0,0,0));
		PlusedChildIc += (ballJointNode->Xcp_mat.trans() * ballJointNode->Ic + ballJointNode->XtrIJ_JIJinv * (-ballJointNode->J.trans() * ballJointNode->Ic))/dt;
		cout << ballJointNode->solid->GetName() << " PlusedChildIc " << PlusedChildIc << endl;
		//ballJointNode->GetParent()->solid->AddForce(PlusedChildIc.v());
		//ballJointNode->GetParent()->solid->AddTorque(PlusedChildIc.w());

		for (container_t::reverse_iterator it2 = ballJointNode->Children().rbegin(); it2 != ballJointNode->Children().rend(); it2++) {
			PHTreeNode* treeNode2 = (*it2);
			PHBallJointNode* ballJointNode2 = (PHBallJointNode*)(treeNode2);
			SpatialVector PlusedChildIc2 = SpatialVector(Vec3d(0,0,0),Vec3d(0,0,0));
			PlusedChildIc2 += -(ballJointNode2->Xcp_mat.trans() * (cForce2) + ballJointNode2->XtrIJ_JIJinv * (-ballJointNode2->J.trans() * (cForce2))); //ここ違う気がする
			cout << ballJointNode2->solid->GetName() << " PlusedChildIc2 " << PlusedChildIc2 << endl;
			PHSolid* parentSolid = ballJointNode2->GetParent()->solid;
			//parentSolid->AddForce(parentSolid->GetOrientation() * PlusedChildIc2.v());
			//parentSolid->AddTorque(parentSolid->GetOrientation() * PlusedChildIc2.w());
		}
	}


	// addforceやaddtorqueした力をfに書き込む<!!>実行順があっているか不明
	solid->UpdateCacheLCP(dt);
	for (container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++) {
		(*it)->solid->UpdateCacheLCP(dt);
		std::cout << "UpdateCacheLCP " << (*it)->solid->GetName() << endl;
		for (container_t::reverse_iterator it2 = (*it)->Children().rbegin(); it2 != (*it)->Children().rend(); it2++) {
			(*it2)->solid->UpdateCacheLCP(dt);
			std::cout << "UpdateCacheLCP " << (*it2)->solid->GetName() << endl;
		}
	}

	// articulated bias forceを計算
	InitArticulatedBiasForce();
	CompArticulatedBiasForce();


	// 慣性力と外力による加速度を計算
	CompAccel();
	// 拘束力変化に対する加速度変化のマップを作成
	CompResponseMatrix();
	CompResponseMap();

	// Correction用変数ここでクリア
	for(int i = 0; i < (int)nodes.size(); i++){
		nodes[i]->solid->dV.clear();
	}
}

void PHRootNode::SetupCorrection() {
	for (int i = 0; i < (int)nodes.size(); i++) {
		nodes[i]->ClearCorrection();
	}
}

void PHRootNode::CompArticulatedInertia(){
	//子ノードにIaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のIaを積み上げる
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->InitArticulatedInertia();
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->CompArticulatedInertia();

	//逆行列を計算
	(Matrix6d&)Iinv = I.inv();
}

void PHRootNode::CompArticulatedBiasForce(){
	//子ノードにZaを計算させる．
	//子ノード達は親ノード（つまりこのノード）に自分のZaを積み上げる
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->InitArticulatedBiasForce();
	for(container_t::reverse_iterator it = Children().rbegin(); it != Children().rend(); it++)
		(*it)->CompArticulatedBiasForce();
}

void PHRootNode::CompResponseMap(){
	int n = (int)nodes.size();
	
	if(solid->IsDynamical())
		 (Matrix6d&)dZdv_map[0] = -Iinv;
	else dZdv_map[0].clear();

	for(int i = 1; i < n; i++)
		(Matrix6d&)dZdv_map[i] = dZdv_map[nodes[i]->GetParentOfGearTrain()->id] * nodes[i]->Tcp.trans();
		
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompResponseMap();
}

void PHRootNode::CompAccel(){
	if(solid->IsDynamical()){
		(Vec6d&)solid->dv = - (Iinv * Z);
	}
	else{
		solid->dv.clear();
	}
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccel();
}

void PHRootNode::UpdateVelocity(double* dt){
	if(!bEnabled)
		return;
	solid->UpdateVelocity(dt);
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdateVelocity(dt);
}

void PHRootNode::UpdatePosition(double dt){
	if(!bEnabled)
		return;
	solid->UpdatePosition(dt);
	solid->SetUpdated(true);
	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->UpdatePosition(dt);
}

void PHRootNode::SetCompControlForceCallback(PHRootNodeIf::CompControlForce f, void* a) {
	compControlForce = f;
	arg = a;
}
//-----------------------------------------------------------------------------

template<int NDOF>
PHTreeNodeND<NDOF>::PHTreeNodeND(){
	dvel.clear();
	vel .clear();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::ResetGear(){
	gear        = 0;
	gearRoot    = 0;
	gearParent  = 0;
	gearUpper   = 0;
}	

template<int NDOF>
void PHTreeNodeND<NDOF>::AddToGearTrain(PHGear* gear, PHTreeNode* _upper, bool isParent){
	PHTreeNodeND<NDOF>* upper = (PHTreeNodeND<NDOF>*)_upper;
	if(!upper->gearRoot){
		upper->gearRoot     = upper;
		upper->gearParent   = 0;
		upper->gearUpper    = 0;
		upper->gearRatio    = 1.0;
		upper->gearOffset   = 0.0;
		upper->gearRatioAbs = 1.0;
	}
	this->gear   = gear;
	gearRoot     = upper->gearRoot;
	gearParent   = (isParent ? upper : upper->gearParent);
	gearUpper    = upper;
	// ギアの極性を考慮してギア比を記憶
	double ratio  = gear->GetRatio ();
	double offset = gear->GetOffset();
	if(gear->joint[1] == joint){
		gearRatio  = ratio;
		gearOffset = offset;
	}
	else{
		gearRatio  = 1.0/ratio;
		gearOffset = -offset/ratio;
	}
	gearRatioAbs = upper->gearRatioAbs * gearRatio;

	gear->bArticulated = true;
}

template<int NDOF>
void PHTreeNodeND<NDOF>::Prepare(){
	int n = (int)root->nodes.size();
	dZdvel_map.resize(n);
	
	PHTreeNode::Prepare();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::InitArticulatedInertia(){
	PHTreeNode::InitArticulatedInertia();
	if(gearRoot == this){
		sumXIX            .clear();
		sumXtrIJ          .clear();
		sumXtrIJ_sumJIJinv.clear();
		sumXtrZplusIc     .clear();
		sumJIJ            .clear();
		sumJIJinv         .clear();
		sumJtrZplusIc     .clear();
	}
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompSpatialTransform(){
	PHTreeNode::CompSpatialTransform();

	// 親ノードと連動している場合
	if(gearParent)
		 Xcg = Xcp * gearParent->Xcg;
	else Xcg = Xcp;
	Xcg_mat = Xcg;
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompJointJacobian(){
	J = Xcj_mat * J;

	if(gear){
		// ヤコビアンをギアトレインのルートに関するものに変換
		//  ギアトレインのギア比をかける
		J *= gearRatioAbs;
		
		//  親ノードと連動している場合はその分を合成
		//  ただし親ノードと直接ギアでつながっている（gearParent == parent）とは限らないので注意
		if(gearParent){
			J += Xcp_mat * gearParent->J;
		}
	}
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompCoriolisAccel(){
	PHTreeNode::CompCoriolisAccel();
	
	//直列ギア連動している場合は上段のコリオリ項を足す
	if(gearParent)
		c += Xcp * gearParent->c;
}

template<int NDOF>
void PHTreeNodeND<NDOF>::AccumulateInertia(){
	(Matrix6d&)XIX = Xcg_mat.trans() * I * Xcg_mat;

	IJ    = I * J;
	XtrIJ = Xcp_mat.trans() * IJ;

	JIJ                           = J.trans() * IJ;
	JIJinv                        = JIJ.inv();
	J_JIJinv                      = J            * JIJinv;
	IJ_JIJinv                     = I            * J_JIJinv;
	XtrIJ_JIJinv                  = XtrIJ        * JIJinv;
	(Matrix6d&)J_JIJinv_Jtr       = J_JIJinv     * J.trans();
	(Matrix6d&)XtrIJ_JIJinv_Jtr   = XtrIJ_JIJinv * J.trans();
	(Matrix6d&)XtrIJ_JIJinv_JtrIX = XtrIJ_JIJinv * XtrIJ.trans();
	
	if(gearRoot){
		gearRoot->sumXIX   += XIX;
		gearRoot->sumXtrIJ += XtrIJ;
		gearRoot->sumJIJ   += JIJ;

		if(gearRoot == this){
			sumJIJinv          = sumJIJ.inv();
			sumXtrIJ_sumJIJinv = sumXtrIJ * sumJIJinv;
			parent->I += (sumXIX - sumXtrIJ * sumXtrIJ_sumJIJinv.trans());
		}
	}
	else{
		parent->I += (XIX - XtrIJ_JIJinv_JtrIX);
	}
}

template<int NDOF>
void PHTreeNodeND<NDOF>::AccumulateBiasForce(){
	JtrZplusIc         = J      .trans() * ZplusIc;
	(Vec6d&)XtrZplusIc = Xcp_mat.trans() * ZplusIc;

	if(gearRoot){
		gearRoot->sumXtrZplusIc += XtrZplusIc;
		gearRoot->sumJtrZplusIc += JtrZplusIc;
	
		if(gearRoot == this)
			parent->Z += sumXtrZplusIc + sumXtrIJ_sumJIJinv * (- sumJtrZplusIc);
	}
	else {
		//SpatialVector test = SpatialVector(Vec3d(0, 0, 0), Vec3d(0, 0, 0));
		//test += XtrZplusIc + XtrIJ_JIJinv * (-JtrZplusIc);
		//cout << solid->GetName() << " ZplusIc " << test << endl;
		parent->Z += XtrZplusIc + XtrIJ_JIJinv * (-JtrZplusIc); 
	}
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompResponseMatrix(){
	if(gearRoot)
		 (Matrix6d&)Tcp = Xcg_mat - J * gearRoot->sumJIJinv * gearRoot->sumXtrIJ.trans();
	else (Matrix6d&)Tcp = Xcp_mat - XtrIJ_JIJinv_Jtr.trans();
	PHTreeNode::CompResponseMatrix();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompResponseMap(){
	int n = (int)root->nodes.size();
	for(int i = 0; i < n; i++){
		PHTreeNode* node = root->nodes[i];
		
		if(!solid->IsDynamical()){
			dZdv_map  [i].clear();
			dZdvel_map[i].clear();
		}
		// 同じノード
		else if(i == id && !gearRoot){
			Matrix6d tmp = parent->dZdv_map[parent->id] * Tcp.trans();
			(Matrix6d&)dZdv_map  [i] =   Tcp * tmp - J_JIJinv_Jtr;
			dZdvel_map[i]            = - (XtrIJ_JIJinv.trans() * tmp) - J_JIJinv.trans();
		}
		// 同じギアトレインに属するノード
		else if(gearRoot && gearRoot == node->GetGearRoot()){
			PHTreeNodeND<NDOF>* nodeND = (PHTreeNodeND<NDOF>*)node;
			PHTreeNode* par = gearRoot->parent;
			Matrix6d    tmp = par->dZdv_map[par->id] * nodeND->Tcp.trans();
			(Matrix6d&)dZdv_map  [i] =   Tcp * tmp - J * gearRoot->sumJIJinv * nodeND->J.trans();
			dZdvel_map[i] = - gearRatioAbs * gearRoot->sumJIJinv * ( gearRoot->sumXtrIJ.trans() * tmp + nodeND->J.trans() );
		}
		// 自分より下層
		else if(depth < node->depth){
			PHTreeNode* par = node->GetParentOfGearTrain();
			(Matrix6d&)dZdv_map  [i] = dZdv_map  [par->id] * node->Tcp.trans();
			dZdvel_map[i] = dZdvel_map[par->id] * node->Tcp.trans();
		}
		// 自分より上層
		else{
			PHTreeNode* par = GetParentOfGearTrain();
			dZdv_map  [i] = Tcp * par->dZdv_map[node->id];
			if(gearRoot)
				 dZdvel_map[i] = - gearRatioAbs * (gearRoot->sumJIJinv * gearRoot->sumXtrIJ.trans() * par->dZdv_map[node->id]);
			else dZdvel_map[i] = - (XtrIJ_JIJinv.trans() * par->dZdv_map[node->id]);
		}
	}

	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompResponseMap();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompResponse(PHTreeNode* src, const SpatialVector& df){
	solid->dv += dZdv_map  [src->id] * (-df);
	dvel      += dZdvel_map[src->id] * (-df);
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompResponseCorrection(PHTreeNode* src, const SpatialVector& dF){
	solid->dV += dZdv_map  [src->id] * (-dF);
	vel       += dZdvel_map[src->id] * (-dF);
}

template<int NDOF>
void PHTreeNodeND<NDOF>::CompAccel(){
	if(gear){
		dvel              = gearRatioAbs * gearRoot->dvel;
		(Vec6d&)solid->dv = Xcg_mat * gearRoot->parent->solid->dv + c + J * gearRoot->dvel;
	}
	else if(gearRoot == this){
		dvel              = - sumJIJinv * ((sumXtrIJ.trans() * parent->solid->dv) + sumJtrZplusIc);
		(Vec6d&)solid->dv = Xcp_mat * parent->solid->dv + c + J * dvel;
	}
	else{
		dvel              = -JIJinv * ((XtrIJ.trans() * parent->solid->dv) + JtrZplusIc);
		(Vec6d&)solid->dv = Xcp_mat * parent->solid->dv + c + J * dvel;
	}

	for(container_t::iterator it = Children().begin(); it != Children().end(); it++)
		(*it)->CompAccel();
}

template<int NDOF>
void PHTreeNodeND<NDOF>::UpdateJointVelocity(){
	if(gear)
		 GetJoint()->velocity = gearRatioAbs * gearRoot->GetJoint()->velocity;
	else GetJoint()->velocity += dvel;
}

template<int NDOF>
void PHTreeNodeND<NDOF>::UpdateJointPosition(double dt){
	GetJoint()->position += GetJoint()->velocity * dt + vel;
}

template<int NDOF>
SpatialVector   PHTreeNodeND<NDOF>::ConvertChild2Parent(SpatialVector cf) { 
	SpatialVector pf = SpatialVector(Vec3d(0, 0, 0), Vec3d(0, 0, 0));
	pf += -(Xcp_mat.trans() * cf + XtrIJ_JIJinv * (-J.trans() * (cf)));
	return pf;
}
//-----------------------------------------------------------------------------

PHTreeNode1D::PHTreeNode1D(){
}

void PHTreeNode1D::UpdateJointPosition(double dt){
	if(gear && gear->GetMode() == PHGearDesc::MODE_POS)
		 GetJoint()->position[0] = gearRatio * gearUpper->GetJoint()->position[0] + gearOffset;
	else GetJoint()->position[0] += GetJoint()->velocity[0] * dt + vel[0];
}

//-----------------------------------------------------------------------------

// explicit instantiation
template class PHTreeNodeND<1>;
template class PHTreeNodeND<2>;
template class PHTreeNodeND<3>;
	
}
