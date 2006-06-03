#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include "FISaveContext.h"
#include "FINodeHandler.h"
#include <fstream>
#include <sstream>

namespace Spr{;

//---------------------------------------------------------------------------
//	FISaveContext
FISaveContext::FISaveContext(){
	errorStream = &DSTR;
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
	UTString rv(OCAST(Object, objects.back())->GetTypeInfo()->ClassName());
	return rv;
}
UTString FISaveContext::GetNodeName(){
	NamedObjectIf* n = ICAST(NamedObjectIf, objects.back());
	UTString rv;
	if (n && n->GetName()) rv = n->GetName();
	return rv;
}

};

