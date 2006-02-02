#include "FileIO/FITypeDesc.h" 
#include "TypeDescDump.h"

namespace Spr{ 
static FITypeDesc* desc; 
static FITypeDesc::Field* field; 
FITypeDescDb typeDescDb;
static FITypeDescDb* db = &typeDescDb;
void RegisterTypes(){
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned int DWORD;
	db->RegisterDesc(new FITypeDescNumber<char>("char"));
	db->RegisterDesc(new FITypeDescNumber<short>("short"));
	db->RegisterDesc(new FITypeDescNumber<int>("int"));
	db->RegisterDesc(new FITypeDescNumber<BYTE>("BYTE"));
	db->RegisterDesc(new FITypeDescNumber<WORD>("WORD"));
	db->RegisterDesc(new FITypeDescNumber<DWORD>("DWORD"));
	db->RegisterDesc(new FITypeDescNumber<float>("float"));
	db->RegisterDesc(new FITypeDescNumber<double>("double"));

	#include "TypeDescDump.cpp"
	db->Link();
}
}
