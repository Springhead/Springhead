/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Framework/SprFWEditor.h>
#include <Foundation/UTTypeDesc.h>
#include <Graphics/SprGRRender.h>
#include <iomanip>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

namespace Spr{;
using namespace std;

///////////////////////////////////////////////////////////////////////////
// FWEditor

FWEditor::FieldInfo::FieldInfo(const void* f){
	field = f;
	string tn = ((UTTypeDesc::Field*)f)->typeName;
	if(tn == "bool"){
		nElements = 1;
		type = TYPE_BOOL;
	}
	else if(tn == "short"){
		nElements = 1;
		type = TYPE_INT16;
	}
	else if(tn == "unsigned short"){
		nElements = 1;
		type = TYPE_UINT16;
	}
	else if(tn == "int" || tn == "long"){
		nElements = 1;
		type = TYPE_INT32;
	}
	else if(tn == "unsigned" || tn == "unsigned int" || tn == "unsigned long"){
		nElements = 1;
		type = TYPE_UINT32;
	}
	else if(tn == "float"){
		nElements = 1;
		type = TYPE_FLOAT;
	}
	else if(tn == "double"){
		nElements = 1;
		type = TYPE_DOUBLE;
	}
	else if(tn == "Vec3f"){
		nElements = 3;
		type = TYPE_FLOAT;
	}
	else if(tn == "Vec3d"){
		nElements = 3;
		type = TYPE_DOUBLE;
	}
	else if(tn == "Quaterniond"){
		nElements = 4;
		type = TYPE_DOUBLE;
	}
	else if(tn == "Posed"){
		nElements = 7;
		type = TYPE_DOUBLE;
	}
	else if(tn == "Matrix3f"){
		nElements = 9;
		type = TYPE_FLOAT;
	}
	else if(tn == "Matrix3d"){
		nElements = 9;
		type = TYPE_DOUBLE;
	}
	else if(tn == "Affinef"){
		nElements = 16;
		type = TYPE_FLOAT;
	}
	else{
		nElements = 1;
		type = TYPE_UNKNOWN;
	}
}

FWEditor::FWEditor(){
	SetObject(NULL);
}

void FWEditor::SetObject(NamedObjectIf* obj, bool clearHist){
	curObj = obj;
	curChild	=  0;
	curField	= -1;
	curElement	= -1;

	if(curObj){
		// child objectの取得
		childObjects.clear();
		size_t nChildren = curObj->NChildObject();
		for(size_t i = 0; i < nChildren; i++){
			NamedObjectIf* named = DCAST(NamedObjectIf, curObj->GetChildObject(i));
			if(named)
				childObjects.push_back(named);
		}

		// typedescの取得
		const IfInfo*	ii = curObj->GetIfInfo();
		typeDesc = ii->desc;
		fieldInfos.clear();
		for(size_t i = 0; i < typeDesc->composit.size(); i++)
			FlattenTypeDesc(&typeDesc->composit[i]);
		
		Update();
	}
	if(clearHist)
		hist.clear();
}

void FWEditor::FlattenTypeDesc(const void* field){
	// collect primitive fields into an array
	UTTypeDesc::Field* f = (UTTypeDesc::Field*)field;
	if(!f->type || f->type->bPrimitive){
		fieldInfos.push_back(FieldInfo(field));
	}
	else{
		for(size_t i = 0; i < f->type->composit.size(); i++)
			FlattenTypeDesc(&f->type->composit[i]);
	}
}

double FWEditor::Increase(double val, bool mode){
	if(mode)
		 return val + 1.0;
	else return val * 1.1;
}
double FWEditor::Decrease(double val, bool mode){
	if(mode)
		 return val - 1.0;
	else return val / 1.1;
}
void FWEditor::Increment(bool mode){
	UTTypeDesc::Field* f = (UTTypeDesc::Field*)fieldInfos[curField].field;
	char* head = &descData[f->offset];
	PrimitiveType type = fieldInfos[curField].type;
	if(type == TYPE_BOOL)
		((bool*)head)[curElement] = true;
	else if(type == TYPE_UINT8)
		((unsigned char*)head)[curElement]++;
	else if(type == TYPE_INT8)
		((char*)head)[curElement]++;
	else if(type == TYPE_UINT16)
		((unsigned short*)head)[curElement]++;
	else if(type == TYPE_INT16)
		((short*)head)[curElement]++;
	else if(type == TYPE_UINT32)
		((unsigned int*)head)[curElement]++;
	else if(type == TYPE_INT32)
		((int*)head)[curElement]++;
	else if(type == TYPE_FLOAT)
		((float*)head)[curElement] = (float)Increase(((float*)head)[curElement], mode);
	else if(type == TYPE_DOUBLE)
		((double*)head)[curElement] = Increase(((double*)head)[curElement], mode);
	curObj->SetDesc((void*)&descData[0]);
}

void FWEditor::Decrement(bool mode){
	UTTypeDesc::Field* f = (UTTypeDesc::Field*)fieldInfos[curField].field;
	char* head = &descData[f->offset];
	PrimitiveType type = fieldInfos[curField].type;
	if(type == TYPE_BOOL)
		((bool*)head)[curElement] = false;
	else if(type == TYPE_UINT8){
		unsigned char& c = ((unsigned char*)head)[curElement];
		if(c) c--;
	}
	else if(type == TYPE_INT8)
		((char*)head)[curElement]--;
	else if(type == TYPE_UINT16){
		unsigned short& s = ((unsigned short*)head)[curElement];
		if(s) s--;
	}
	else if(type == TYPE_INT16)
		((short*)head)[curElement]--;
	else if(type == TYPE_UINT32){
		unsigned int& i = ((unsigned int*)head)[curElement];
		if(i) i--;
	}
	else if(type == TYPE_INT32)
		((int*)head)[curElement]--;
	else if(type == TYPE_FLOAT)
		((float*)head)[curElement] = (float)Decrease(((float*)head)[curElement], mode);
	else if(type == TYPE_DOUBLE)
		((double*)head)[curElement] = Decrease(((double*)head)[curElement], mode);
	curObj->SetDesc((void*)&descData[0]);
}

void FWEditor::Update(){
	// descriptorの取得
	size_t sz = curObj->GetDescSize();
	if(sz > descData.size())
		descData.resize(sz);
	curObj->GetDesc((void*)&descData[0]);
}

///////////////////////////////////////////////////////////////////////////
// FWEditorOverlay

FWEditorOverlay::FWEditorOverlay(){
	margin = Vec2f(5, 20);
	columnWidth = 8*20;	// 20 characters
	lineSpace = 15;
}

enum{
	// virtual key code
	/*VK_ENTER	= 10,
	VK_LEFT		= 37,
	VK_UP		= 38,
	VK_RIGHT	= 39,
	VK_DOWN		= 40,*/
	// GLUT special key code
	VK_BACKSPACE = 8,
	VK_ENTER	= 13,
	VK_LEFT		= 100,
	VK_UP		= 101,
	VK_RIGHT	= 102,
	VK_DOWN		= 103,
	VK_PAGEUP	= 104,
	VK_PAGEDOWN	= 105,
};

bool FWEditorOverlay::Key(int key){
	// glutの特殊キーコードは0x100足されて渡されるFWAppの仕様
	int spKey = key - 0x100;

	if(key == VK_ENTER){
		if(curField == -1){
			// 選択されたchild objectへ移動
			hist.push_back(curObj);
			SetObject(childObjects[curChild], false);
		}
		return true;
	}
	if(key == VK_BACKSPACE){
		// 履歴を1つ前へ戻る
		if(!hist.empty()){
			SetObject(hist.back(), false);
			hist.pop_back();
		}
		return true;
	}

	//
	if(spKey == VK_LEFT){
		if(curField < 0){
			if(curChild > 0)
				--curChild;
		}
		else{
			if(curElement >= 0)
				--curElement;
		}
		return true;
	}
	if(spKey == VK_RIGHT){
		if(curField < 0){
			if(curChild < (int)(childObjects.size()-1))
				++curChild;
		}
		else{
			if(curElement < (int)(fieldInfos[curField].nElements-1))
				++curElement;
		}
		return true;
	}
	if(spKey == VK_UP){
		if(curElement >= 0)
			Increment(true);
		else if(curField >= 0)
			--curField;
		return true;
	}
	if(spKey == VK_DOWN){
		if(curElement >= 0)
			Decrement(true);
		else if(curField < (int)fieldInfos.size()-1)
			++curField;
		return true;
	}
	if(spKey == VK_PAGEUP){
		if(curElement >= 0)
			Increment(false);
		return true;
	}
	if(spKey == VK_PAGEDOWN){
		if(curElement >= 0)
			Decrement(false);
		return true;
	}
	return false;
}

void FWEditorOverlay::Draw(GRRenderIf* render){
	render->EnterScreenCoordinate();

	// オブジェクトの型と名前
	drawPos.x = margin.x;
	drawPos.y = margin.y;
	render->DrawFont(drawPos, typeDesc->GetTypeName());
	drawPos.x += columnWidth;
	render->DrawFont(drawPos, curObj->GetName());
	drawPos.y += lineSpace;
	drawPos.y += lineSpace;

	// child objects
	drawPos.x = margin.x;
	render->DrawFont(drawPos, "child obj:");
	drawPos.x += columnWidth;
	oss.str("");
	if(!childObjects.empty()){
		if(curField == -1)
			oss << '[';
		string name = childObjects[curChild]->GetName();
		oss << (name.empty() ? "no name" : name);
		if(curField == -1)
			oss << ']';
	}
	oss << " (" << curChild << '/' << childObjects.size() << ')';
	render->DrawFont(drawPos, oss.str());
	drawPos.y += lineSpace;
	drawPos.y += lineSpace;

	// draw each field
	drawPos.x = margin.x;
	render->DrawFont(drawPos, "desc/state:");
	drawPos.y += lineSpace;
	for(size_t i = 0; i < fieldInfos.size(); i++){
		DrawField(render, i);
	}

	render->LeaveScreenCoordinate();
}

void FWEditorOverlay::DrawField(GRRenderIf* render, size_t index){
	const UTTypeDesc::Field* field = (const UTTypeDesc::Field*)fieldInfos[index].field;
	size_t nElem = fieldInfos[index].nElements;
	PrimitiveType type = fieldInfos[index].type;

	// draw field name
	// selected field is highlighted
	drawPos.x = margin.x;
	oss.str("");
	if(index == curField)
		oss << '[';
	oss << field->name;
	if(index == curField)
		oss << ']';
	render->DrawFont(drawPos, oss.str());
	drawPos.x += columnWidth;
	
	oss.str("");

	// for array field, only number of elements are displayed
	if(field->length > 1){
		oss << '[' << field->length << ']';
		render->DrawFont(drawPos, oss.str());
	}
	else{
		char* data = &descData[field->offset];
		oss << setprecision(5);
		if(nElem > 1)
			oss << '[';
		for(size_t i = 0; i < nElem; i++){
			if(index == curField && i == curElement)
				oss << '[';
			if(type == TYPE_BOOL)
				oss << ((bool*)data)[i];
			else if(type == TYPE_INT16)
				oss << ((short*)data)[i];
			else if(type == TYPE_UINT16)
				oss << ((unsigned short*)data)[i];
			else if(type == TYPE_INT32)
				oss << ((int*)data)[i];
			else if(type == TYPE_UINT32)
				oss << ((unsigned int*)data)[i];
			else if(type == TYPE_FLOAT)
				oss << ((float*)data)[i];
			else if(type == TYPE_DOUBLE)
				oss << ((double*)data)[i];
			else oss << "[unknown type " << field->typeName << "]";
			if(index == curField && i == curElement)
				oss << ']';

			if(i < nElem-1)
				oss << ' ';
		}
		if(nElem > 1)
			oss << ']';
		render->DrawFont(drawPos, oss.str());

		drawPos.y += lineSpace;
	}
}

}