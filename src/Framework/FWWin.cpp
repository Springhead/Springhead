/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

#include <Framework/SprFWApp.h>
#include <Framework/FWWin.h>
#include <Framework/FWGraphicsHandler.h>
#include <Framework/FWScene.h>
#include <Graphics/SprGRScene.h>
#include <Graphics/SprGRRender.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

//-------------------------------------------------------------------------------------------------
// FWWinBase

Vec2i FWWinBase::GetPosition(){
	Vec2i pos = FWGraphicsHandler::instance->GetPosition(this);
	left = pos.x;
	top  = pos.y;
	return pos;
}

void FWWinBase::SetPosition(int l, int t){
	FWGraphicsHandler::instance->SetPosition(this, l, t);
	left = l;
	top  = t;
}

Vec2i FWWinBase::GetSize(){
	Vec2i sz = FWGraphicsHandler::instance->GetSize(this);
	width  = sz.x;
	height = sz.y;
	return sz;
}

void FWWinBase::SetSize(int w, int h){
	FWGraphicsHandler::instance->SetSize(this, w, h);
	width  = w;
	height = h;
}
void FWWinBase::SetTitle(UTString t){
	FWGraphicsHandler::instance->SetTitle(this, t);
	title = t;
}

//-------------------------------------------------------------------------------------------------
// FWWin

bool FWWin::AddChildObject(ObjectIf* o){
	FWDialogIf* dlg = o->Cast();
	if(dlg){
		childDlg.push_back(dlg);
		return true;
	}
	return false;
}

size_t	FWWin::NChildObject() const{
	return childDlg.size();
}
	
void FWWin::Display(){
	if(!scene)
		return;

	// GRSceneにカメラフレームが無い場合はトラックボールを直接ビュー変換に反映する
	if(!scene->GetGRScene() || !scene->GetGRScene()->GetCamera() || !scene->GetGRScene()->GetCamera()->GetFrame()){
		render->SetViewMatrix(trackball->GetAffine().inv());
	}
	scene->Draw(render, debugMode);
	render->SwapBuffers();
}

FWDialogIf*	FWWin::CreateDialog(const FWDialogDesc& desc){
	FWDialogIf* dlg = FWGraphicsHandler::instance->CreateDialog(this, desc)->Cast();
	if(dlg){
		AddChildObject(dlg);
	}
	return dlg;
}

void FWWin::CalcViewport(int& l, int& t, int& w, int& h){
	FWGraphicsHandler::instance->CalcViewport(l, t, w, h);
}

void FWWin::SetFullScreen(){
	FWGraphicsHandler::instance->SetFullScreen(this);
	fullscreen = true;
}

void FWWin::SetPosition(int left, int top){
	FWWinBase::SetPosition(left, top);
	fullscreen = false;
}

void FWWin::SetSize(int width, int height){
	FWWinBase::SetSize(width, height);
	fullscreen = false;
}

void FWWin::OnUpdatePose(HITrackballIf*){
	if(!GetScene() || !GetScene()->GetGRScene())
		return;

	// トラックボールとGRSceneのカメラフレームの同期
	GRCameraIf* cam = GetScene()->GetGRScene()->GetCamera();
	if(!cam)
		return;
	GRFrameIf* camFrame = cam->GetFrame();
	if(!camFrame)
		return;

	camFrame->SetTransform(trackball->GetAffine());
}

//-------------------------------------------------------------------------------------------------
// FWDialog

FWControlIf* FWDialog::CreateControl(const IfInfo* ii, const FWControlDesc& desc, FWPanelIf* parent){
	return FWGraphicsHandler::instance->CreateControl(this, ii, desc, parent->Cast())->Cast();
}

FWStaticTextIf* FWDialog::CreateStaticText(UTString text, FWPanelIf* parent){
	FWStaticTextDesc desc;
	desc.label = text;
	return CreateControl(FWStaticTextIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWButtonIf* FWDialog::CreatePushButton(UTString label, FWPanelIf* parent){
	FWButtonDesc desc;
	desc.style = FWButtonDesc::PUSH_BUTTON;
	desc.label = label;
	return CreateControl(FWButtonIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWButtonIf* FWDialog::CreateCheckButton(UTString label, bool checked, FWPanelIf* parent){
	FWButtonDesc desc;
	desc.style = FWButtonDesc::CHECK_BUTTON;
	desc.label = label;
	desc.checked = checked;
	return CreateControl(FWButtonIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWButtonIf* FWDialog::CreateRadioButton(UTString label, FWPanelIf* parent){
	FWButtonDesc desc;
	desc.style = FWButtonDesc::RADIO_BUTTON;
	desc.label = label;
	return CreateControl(FWButtonIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWTextBoxIf* FWDialog::CreateTextBox(UTString label, UTString text, int style, FWPanelIf* parent){
	FWTextBoxDesc desc;
	desc.style = style;
	desc.label = label;
	desc.text = text;
	return CreateControl(FWTextBoxIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWPanelIf* FWDialog::CreatePanel(UTString label, int style, FWPanelIf* parent){
	FWPanelDesc desc;
	desc.style = style;
	desc.label = label;
	return CreateControl(FWPanelIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWPanelIf* FWDialog::CreateRadioGroup(FWPanelIf* parent){
	FWPanelDesc desc;
	desc.style = FWPanelDesc::RADIOGROUP;
	return CreateControl(FWPanelIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWRotationControlIf* FWDialog::CreateRotationControl(UTString label, FWPanelIf* parent){
	FWRotationControlDesc desc;
	desc.label = label;
	return CreateControl(FWRotationControlIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWTranslationControlIf* FWDialog::CreateTranslationControl(UTString label, int style, FWPanelIf* parent){
	FWTranslationControlDesc desc;
	desc.style = style;
	desc.label = label;
	return CreateControl(FWTranslationControlIf::GetIfInfoStatic(), desc, parent)->Cast();
}

FWListBoxIf* FWDialog::CreateListBox(UTString label, FWPanelIf* parent){
	FWListBoxDesc desc;
	desc.label = label;
	return CreateControl(FWListBoxIf::GetIfInfoStatic(), desc, parent)->Cast();	
}

void FWDialog::CreateColumn(bool sep, FWPanelIf* parent){
	FWGraphicsHandler::instance->CreateColumn(this, sep, parent->Cast());
}

void FWDialog::CreateSeparator(FWPanelIf* parent){
	FWGraphicsHandler::instance->CreateSeparator(this, parent->Cast());
}


//-------------------------------------------------------------------------------------------------
// FWControl

void FWControl::SetLabel(UTString l){
	FWGraphicsHandler::instance->SetLabel(this, l);
	label = l;
}

void FWControl::SetAlign(int a){
	FWGraphicsHandler::instance->SetAlign(this, align);
	align = a;
}

void FWControl::SetStyle(int s){
	FWGraphicsHandler::instance->SetStyle(this, s);
	style = s;
}

int	FWControl::GetInt(){
	return FWGraphicsHandler::instance->GetInt(this);
}

void FWControl::SetInt(int val){
	FWGraphicsHandler::instance->SetInt(this, val);
}

float FWControl::GetFloat(){
	return FWGraphicsHandler::instance->GetFloat(this);
}

void FWControl::SetFloat(float val){
	FWGraphicsHandler::instance->SetFloat(this, val);
}

//-------------------------------------------------------------------------------------------------
// FWPanel

//-------------------------------------------------------------------------------------------------
// FWButton

void FWButton::SetChecked(bool on){
	FWGraphicsHandler::instance->SetChecked(this, on);
}

bool FWButton::IsChecked(){
	return FWGraphicsHandler::instance->IsChecked(this);
}

//-------------------------------------------------------------------------------------------------
// FWTextBox

void FWTextBox::SetIntRange(int rmin, int rmax){
	return FWGraphicsHandler::instance->SetIntRange(this, rmin, rmax);
}

void FWTextBox::SetFloatRange(float rmin, float rmax){
	return FWGraphicsHandler::instance->SetFloatRange(this, rmin, rmax);
}

const char* FWTextBox::GetString(){
	return FWGraphicsHandler::instance->GetString(this);
}

void FWTextBox::SetString(char* str){
	FWGraphicsHandler::instance->SetString(this, str);
}

//-------------------------------------------------------------------------------------------------
// FWRotationControl

Matrix3f FWRotationControl::GetRotation(){
	return FWGraphicsHandler::instance->GetRotation(this);
}

void FWRotationControl::SetRotation(const Matrix3f& rot){
	FWGraphicsHandler::instance->SetRotation(this, rot);
}

void FWRotationControl::SetDamping(float d){
	FWGraphicsHandler::instance->SetDamping(this, d);
}

void FWRotationControl::Reset(){
	FWGraphicsHandler::instance->Reset(this);
}

//-------------------------------------------------------------------------------------------------
// FWTranslationControl

Vec3f FWTranslationControl::GetTranslation(){
	return FWGraphicsHandler::instance->GetTranslation(this);
}

void FWTranslationControl::SetTranslation(Vec3f p){
	FWGraphicsHandler::instance->SetTranslation(this, p);
}

void FWTranslationControl::SetSpeed(float sp){
	FWGraphicsHandler::instance->SetSpeed(this, sp);
}

//-------------------------------------------------------------------------------------------------
// FWListBox

void FWListBox::AddItem(UTString label){
	FWGraphicsHandler::instance->AddItem(this, label);
	items.push_back(label);
}

}
