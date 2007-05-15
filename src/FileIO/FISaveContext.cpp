/*
 *  Copyright (c) 2003-2006, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FISaveContext.h"
#include <fstream>
#include <sstream>

namespace Spr{;

//---------------------------------------------------------------------------
//	FISaveContext
FISaveContext::FISaveContext(){
	errorStream = &DSTR;
	typeDbs.Push(DBG_NEW UTTypeDescDb);
	handlerDbs.Push(DBG_NEW UTLoadHandlerDb);
}

void FISaveContext::Open(const char* fn){
	file.open(fn);
}
void FISaveContext::Message(const char* msg){
	*errorStream << msg << std::endl;
}
void FISaveContext::ErrorMessage(const char* msg){
	*errorStream << "error: " << msg << std::endl;
}
UTString FISaveContext::GetNodeTypeName(){
	UTString rv(DCAST(Object, objects.back())->GetTypeInfo()->ClassName());
	return rv;
}
UTString FISaveContext::GetNodeName(){
	NamedObjectIf* n = DCAST(NamedObjectIf, objects.back());
	UTString rv;
	if (n && n->GetName()) rv = n->GetName();
	return rv;
}

void FISaveContext::RegisterGroupToDb(const char* gp){
	const char* p = gp;
	while(1){
		const char* end = strchr(p, ' ');
		if (!end) end = gp + strlen(gp);
		if (p < end){
			UTString group(p, end);
			p = end+1;
			*handlerDbs.Top() += *UTLoadHandlerDbPool::Get(group.c_str());
			*typeDbs.Top() += *UTTypeDescDbPool::Get(group.c_str());
		}else{
			break;
		}
	}
}

};

