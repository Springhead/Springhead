/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/FWGLUI.h>
#include <Framework/SprFWApp.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;

FWGLUI* FWGLUI::GetInstance(){
	return (FWGLUI*)&*(FWGraphicsHandler::instance);
}

FWGLUI::~FWGLUI(){
}

void FWGLUI::GluiUpdateFunc(int id){
	FWApp::GetApp()->OnControlUpdate(FWGLUI::GetInstance()->ctrls[id]->Cast());
}

#ifdef USE_GLUI

void FWGLUI::EnableIdleFunc(bool on){
	idleFuncFlag = on;
	// GLUIに渡す
	GLUI_Master.set_glutIdleFunc(on ? FWGLUI::GlutIdleFunc : NULL);
}
void FWGLUI::GlutIdleFunc() {
	FWGLUT::GlutIdleFunc();
}
void GLUICALLBACK FWGLUI::GluiReshapeFunc(int w, int h) {
	FWGLUT::GlutReshapeFunc(w,h);
}
void GLUICALLBACK FWGLUI::GluiMouseFunc(int button, int state, int x, int y) {
	FWGLUT::GlutMouseFunc(button, state, x, y);
}
void GLUICALLBACK FWGLUI::GluiKeyFunc(unsigned char ch, int x, int y) {
	FWGLUT::GlutKeyFunc(ch, x, y);
}
void GLUICALLBACK FWGLUI::GluiSpecialKeyFunc(int ch, int x, int y){
	FWGLUT::GlutSpecialKeyFunc(ch, x, y);
}

void FWGLUI::CalcViewport(int& l, int& t, int& w, int& h){
	GLUI_Master.get_viewport_area(&l, &t, &w, &h);
}

void FWGLUI::RegisterCallbacks(){
	glutDisplayFunc		 (FWGLUT::GlutDisplayFunc);
	glutMotionFunc		 (FWGLUT::GlutMotionFunc);
	glutPassiveMotionFunc(FWGLUT::GlutPassiveMotionFunc);

	// 以下4つはGLUIが乗っ取る必要がある
	GLUI_Master.set_glutReshapeFunc	(FWGLUI::GluiReshapeFunc);
	GLUI_Master.set_glutMouseFunc	(FWGLUI::GluiMouseFunc);
	GLUI_Master.set_glutKeyboardFunc(FWGLUI::GluiKeyFunc);
	GLUI_Master.set_glutSpecialFunc	(FWGLUI::GluiSpecialKeyFunc);
}

FWDialog*	FWGLUI::CreateDialog(FWWin* owner, const FWDialogDesc& desc){
	GLUI* glui;
	if(desc.dock){
		int pos;
		switch(desc.dockPos){
		case FWDialogDesc::DOCK_LEFT:	pos = GLUI_SUBWINDOW_LEFT;   break;
		case FWDialogDesc::DOCK_RIGHT:  pos = GLUI_SUBWINDOW_RIGHT;  break;
		case FWDialogDesc::DOCK_BOTTOM: pos = GLUI_SUBWINDOW_BOTTOM; break;
		case FWDialogDesc::DOCK_TOP:
		default:						pos = GLUI_SUBWINDOW_TOP;    break;
		}
		glui = GLUI_Master.create_glui_subwindow(owner->GetID(), pos);
	}
	else{
		glui = GLUI_Master.create_glui(desc.title.c_str(), 0, desc.left, desc.top);
		glui->set_main_gfx_window(owner->GetID());
	}
	
	FWDialog* dlg = DBG_NEW FWDialog();
	dlg->SetDesc(&desc);
	dlg->handle = glui;
	return dlg;
}

FWControl* FWGLUI::CreateControl(FWDialog* owner, const IfInfo* ii, const FWControlDesc& desc, FWPanel* parent){
	GLUI*		glui		= (GLUI*)owner->handle;
	FWPanel*	par			= 0;
	GLUI_Panel* gluiPanel	= 0;
	void*		handle		= 0;

	int id = (int)ctrls.size();

	if(parent){
		par = parent->Cast();
		gluiPanel = (GLUI_Panel*)par->handle;
	}
	
	if(ii == FWPanelIf::GetIfInfoStatic()){
		const FWPanelDesc& panelDesc = (const FWPanelDesc&)desc;
		if(desc.style == FWPanelDesc::ROLLOUT){
			if(par)
				 handle = glui->add_rollout_to_panel(gluiPanel, panelDesc.label.c_str(), panelDesc.open);
			else handle = glui->add_rollout(panelDesc.label.c_str(), panelDesc.open);
		}
		else if(desc.style == FWPanelDesc::RADIOGROUP){
			if(par)
				 handle = glui->add_radiogroup_to_panel(gluiPanel);
			else handle = glui->add_radiogroup();
		}
		else{
			int style;
			switch(panelDesc.style){
			case FWPanelDesc::RAISED:	style = GLUI_PANEL_RAISED;	 break;
			case FWPanelDesc::EMBOSSED:	style = GLUI_PANEL_EMBOSSED; break;
			default:					style = GLUI_PANEL_NONE;	 break;
			}
			if(par)
				 handle = glui->add_panel_to_panel(gluiPanel, panelDesc.label.c_str(), style);
			else handle = glui->add_panel(panelDesc.label.c_str(), style);
		}
		ctrls.push_back(DBG_NEW FWPanel());
	}
	else if(ii == FWButtonIf::GetIfInfoStatic()){
		const FWButtonDesc& btnDesc = (const FWButtonDesc&)desc;
		if(btnDesc.style == FWButtonDesc::PUSH_BUTTON){
			if(par)
				 handle = glui->add_button_to_panel(gluiPanel, btnDesc.label.c_str(), id, GluiUpdateFunc);
			else handle = glui->add_button(btnDesc.label.c_str(), id, &GluiUpdateFunc);
		}
		else if(btnDesc.style == FWButtonDesc::CHECK_BUTTON){
			if(par)
				 handle = glui->add_checkbox_to_panel(gluiPanel, btnDesc.label.c_str(), 0, id, &GluiUpdateFunc);
			else handle = glui->add_checkbox(btnDesc.label.c_str(), 0, id, &GluiUpdateFunc);
		}
		else if(btnDesc.style == FWButtonDesc::RADIO_BUTTON){
			if(par && par->GetStyle() == FWPanelDesc::RADIOGROUP)
				 handle = glui->add_radiobutton_to_group((GLUI_RadioGroup*)gluiPanel, btnDesc.label.c_str());
			else return 0;
		}
		else return 0;

		ctrls.push_back(DBG_NEW FWButton());
	}
	else if(ii == FWStaticTextIf::GetIfInfoStatic()){
		if(par)
			 handle = glui->add_statictext_to_panel(gluiPanel, desc.label.c_str());
		else handle = glui->add_statictext(desc.label.c_str());
		ctrls.push_back(DBG_NEW FWStaticText());
	}
	else if(ii == FWTextBoxIf::GetIfInfoStatic()){
		const FWTextBoxDesc& textDesc = (const FWTextBoxDesc&)desc;
		int style;
		switch(desc.style){
		case FWTextBoxDesc::INT:	style = GLUI_EDITTEXT_INT;	 break;
		case FWTextBoxDesc::FLOAT:	style = GLUI_EDITTEXT_FLOAT; break;
		case FWTextBoxDesc::TEXT:
		default:					style = GLUI_EDITTEXT_TEXT;
		}
		if(par)
			 handle = glui->add_edittext_to_panel(gluiPanel, desc.label.c_str(), style, 0, id, &GluiUpdateFunc);
		else handle = glui->add_edittext(desc.label.c_str(), style, 0, id, &GluiUpdateFunc);
		FWTextBox* text = DBG_NEW FWTextBox();
		text->handle = handle;

		// 値域の設定
		if(desc.style == FWTextBoxDesc::INT)
			SetIntRange(text, textDesc.intMin, textDesc.intMax);
		if(desc.style == FWTextBoxDesc::FLOAT)
			SetFloatRange(text, textDesc.floatMin, textDesc.floatMax);

		ctrls.push_back(text);
	}
	else if(ii == FWRotationControlIf::GetIfInfoStatic()){
		if(par)
			 handle = glui->add_rotation_to_panel(gluiPanel, desc.label.c_str(), 0, id, &GluiUpdateFunc);
		else handle = glui->add_rotation(desc.label.c_str(), 0, id, &GluiUpdateFunc);
		ctrls.push_back(DBG_NEW FWRotationControl());
	}
	else if(ii == FWTranslationControlIf::GetIfInfoStatic()){
		if(par)
			 handle = glui->add_translation_to_panel(gluiPanel, desc.label.c_str(), desc.style, 0, id, &GluiUpdateFunc);
		else handle = glui->add_translation(desc.label.c_str(), desc.style, 0, id, &GluiUpdateFunc);
		ctrls.push_back(DBG_NEW FWTranslationControl());
	}
	else if(ii == FWListBoxIf::GetIfInfoStatic()){
		if(par)
			 handle = glui->add_listbox_to_panel(gluiPanel, desc.label.c_str(), 0, id, &GluiUpdateFunc);
		else handle = glui->add_listbox(desc.label.c_str(), 0, id, &GluiUpdateFunc);
		ctrls.push_back(DBG_NEW FWListBox());
	}
	else return 0;
	
	ctrls.back()->SetDesc(&desc);
	ctrls.back()->handle = handle;

	return ctrls.back();
}

void FWGLUI::CreateColumn(FWDialog* owner, bool sep, FWPanel* panel){
	GLUI* glui = (GLUI*)owner->handle;
	if(panel){
		GLUI_Panel* gluiPanel = (GLUI_Panel*)panel->handle;
		glui->add_column_to_panel(gluiPanel, sep);
	}
	else{
		glui->add_column(sep);
	}
}

void FWGLUI::CreateSeparator(FWDialog* owner, FWPanel* panel){
	GLUI* glui = (GLUI*)owner->handle;
	if(panel){
		GLUI_Panel* gluiPanel = (GLUI_Panel*)panel->handle;
		glui->add_separator_to_panel(gluiPanel);
	}
	else{
		glui->add_separator();
	}
}

void FWGLUI::SetSize(FWWinBase* win, int width, int height){
	FWControl* ctrl = win->Cast();
	if(ctrl){
		GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
		gluiCtrl->set_w(width);
		gluiCtrl->set_h(height);
	}
	else FWGLUT::SetSize(win, width, height);
}

void FWGLUI::SetLabel(FWControl* ctrl, UTString label){
	GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
	gluiCtrl->set_name(label.c_str());
}

void FWGLUI::SetAlign(FWControl* ctrl, int align){
	int gluiAlign = FWControlDesc::ALIGN_LEFT;
	switch(align){
	case FWControlDesc::ALIGN_LEFT:		gluiAlign = GLUI_ALIGN_LEFT;	break;
	case FWControlDesc::ALIGN_CENTER:	gluiAlign = GLUI_ALIGN_CENTER;	break;
	case FWControlDesc::ALIGN_RIGHT:	gluiAlign = GLUI_ALIGN_RIGHT;	break;
	}
	GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
	gluiCtrl->set_alignment(gluiAlign);
}

void FWGLUI::SetChecked(FWButton* btn, bool on){
	if(btn->GetStyle() != FWButtonDesc::CHECK_BUTTON)
		return;
	GLUI_Checkbox* gluiCheck = (GLUI_Checkbox*)btn->handle;
	gluiCheck->set_int_val((int)on);
}

bool FWGLUI::IsChecked(FWButton* btn){
	if(btn->GetStyle() != FWButtonDesc::CHECK_BUTTON)
		return false;
	GLUI_Checkbox* gluiCheck = (GLUI_Checkbox*)btn->handle;
	return !!(gluiCheck->get_int_val());
}

void FWGLUI::SetIntRange(FWTextBox* text, int rmin, int rmax){
	GLUI_EditText* gluiText = (GLUI_EditText*)text->handle;
	gluiText->set_int_limits(rmin, rmax);
}

void FWGLUI::SetFloatRange(FWTextBox* text, float rmin, float rmax){
	GLUI_EditText* gluiText = (GLUI_EditText*)text->handle;
	gluiText->set_float_limits(rmin, rmax);
}

const char* FWGLUI::GetString(FWTextBox* text){
	GLUI_EditText* gluiText = (GLUI_EditText*)text->handle;
	return gluiText->get_text();
}

void FWGLUI::SetString(FWTextBox* text, const char* str){
	GLUI_EditText* gluiText = (GLUI_EditText*)text->handle;
	gluiText->set_text(str);
}

int	FWGLUI::GetInt(FWControl* ctrl){
	GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
	return gluiCtrl->get_int_val();
}

void FWGLUI::SetInt(FWControl* ctrl, int val){
	GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
	gluiCtrl->set_int_val(val);
}

float FWGLUI::GetFloat(FWControl* ctrl){
	GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
	return gluiCtrl->get_float_val();
}

void FWGLUI::SetFloat(FWControl* ctrl, float val){
	GLUI_Control* gluiCtrl = (GLUI_Control*)ctrl->handle;
	gluiCtrl->set_int_val(val);
}

Matrix3f FWGLUI::GetRotation(FWRotationControl* rotCtrl){
	GLUI_Rotation* gluiRot = (GLUI_Rotation*)rotCtrl->handle;
	Affinef aff;
	gluiRot->get_float_array_val((float*)&aff);
	return aff.Rot();
}

void FWGLUI::SetRotation(FWRotationControl* rotCtrl, const Matrix3f& rot){
	GLUI_Rotation* gluiRot = (GLUI_Rotation*)rotCtrl->handle;
	Affinef aff;
	aff.Rot() = rot;
	gluiRot->set_float_array_val((float*)&aff);
}

void FWGLUI::SetDamping(FWRotationControl* rotCtrl, float d){
	GLUI_Rotation* gluiRot = (GLUI_Rotation*)rotCtrl->handle;
	gluiRot->set_spin(d);
}

void FWGLUI::Reset(FWRotationControl* rotCtrl){
	GLUI_Rotation* gluiRot = (GLUI_Rotation*)rotCtrl->handle;
	gluiRot->reset();
}

Vec3f FWGLUI::GetTranslation(FWTranslationControl* trnCtrl){
	GLUI_Translation* gluiTrn = (GLUI_Translation*)trnCtrl->handle;
	Vec3f p;
	p.x = gluiTrn->get_x();
	p.y = gluiTrn->get_y();
	p.z = gluiTrn->get_z();
	return p;
}

void FWGLUI::SetTranslation(FWTranslationControl* trnCtrl, Vec3f p){
	GLUI_Translation* gluiTrn = (GLUI_Translation*)trnCtrl->handle;
	gluiTrn->set_x(p.x);
	gluiTrn->set_y(p.y);
	gluiTrn->set_z(p.z);
}

void FWGLUI::SetSpeed(FWTranslationControl* trnCtrl, float sp){
	GLUI_Translation* gluiTrn = (GLUI_Translation*)trnCtrl->handle;
	gluiTrn->set_speed(sp);
}

void FWGLUI::AddItem(FWListBox*	listBox, UTString label){
	GLUI_Listbox* gluiList = (GLUI_Listbox*)listBox->handle;
	gluiList->add_item((int)listBox->items.size(), label.c_str());
}

#else

void FWGLUI::EnableIdleFunc(bool on){}
void FWGLUI::CalcViewport(int& l, int& t, int& w, int& h){}
void FWGLUI::RegisterCallbacks(){}

FWDialog* FWGLUI::CreateDialog(FWWin* owner, const FWDialogDesc& desc){ return 0; }
FWControl* FWGLUI::CreateControl(FWDialog* owner, const IfInfo* ii, const FWControlDesc& desc, FWPanel* parent){ return 0; }
void FWGLUI::CreateColumn(FWDialog* owner, bool sep, FWPanel* panel){}
void FWGLUI::CreateSeparator(FWDialog* owner, FWPanel* panel){}
void FWGLUI::SetSize(FWWinBase* win, int width, int height){}
void FWGLUI::SetLabel(FWControl* ctrl, UTString label){}
void FWGLUI::SetAlign(FWControl* ctrl, int align){}
void FWGLUI::SetChecked(FWButton* btn, bool on){}
bool FWGLUI::IsChecked(FWButton* btn){ return false; }
void FWGLUI::SetIntRange(FWTextBox* text, int rmin, int rmax){}
void FWGLUI::SetFloatRange(FWTextBox* text, float rmin, float rmax){}
const char* FWGLUI::GetString(FWTextBox* text){ return 0; }
void FWGLUI::SetString(FWTextBox* text, const char* str){}
int	FWGLUI::GetInt(FWControl* ctrl){ return 0; }
void FWGLUI::SetInt(FWControl* ctrl, int val){}
float FWGLUI::GetFloat(FWControl* ctrl){ return 0.0f; }
void FWGLUI::SetFloat(FWControl* ctrl, float val){}
Matrix3f FWGLUI::GetRotation(FWRotationControl* rotCtrl){ return Matrix3f(); }
void FWGLUI::SetRotation(FWRotationControl* rotCtrl, const Matrix3f& rot){}
void FWGLUI::SetDamping(FWRotationControl* rotCtrl, float d){}
void FWGLUI::Reset(FWRotationControl* rotCtrl){}
Vec3f FWGLUI::GetTranslation(FWTranslationControl* trnCtrl){ return Vec3f(); }
void FWGLUI::SetTranslation(FWTranslationControl* trnCtrl, Vec3f p){}
void FWGLUI::SetSpeed(FWTranslationControl* trnCtrl, float sp){}
void FWGLUI::AddItem(FWListBox*	listBox, UTString label){}

#endif



}
