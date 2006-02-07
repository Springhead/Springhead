#include <Base/TQuaternion.h> 
#include <FileIO/FITypeDesc.h> 
#include <sstream>
#include "TypeDescDump.h"

namespace Spr{ 
static FITypeDesc* desc; 
static FITypeDesc::Field* field; 
UTRef<FITypeDescDb> typeDescDb;
void RegisterTypes(){
	if(!typeDescDb) typeDescDb = new FITypeDescDb;
	FITypeDescDb* db = typeDescDb;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned int DWORD;
	db->RegisterDesc(new FITypeDescNumber<char>("char"));
	db->RegisterDesc(new FITypeDescNumber<short>("short"));
	db->RegisterDesc(new FITypeDescNumber<int>("int"));
	db->RegisterDesc(new FITypeDescNumber<BYTE>("BYTE"));
	db->RegisterDesc(new FITypeDescNumber<WORD>("WORD"));
	db->RegisterDesc(new FITypeDescNumber<DWORD>("DWORD"));
	db->RegisterDesc(new FITypeDescNumber<DWORD>("enum"));

	db->RegisterDesc(new FITypeDescNumber<float>("float"));
	db->RegisterDesc(new FITypeDescNumber<double>("double"));
	typedef float FLOAT;
	typedef double DOUBLE;
	db->RegisterDesc(new FITypeDescNumber<float>("FLOAT"));
	db->RegisterDesc(new FITypeDescNumber<double>("DOUBLE"));

	#include "TypeDescDump.cpp"
	db->Link();
}
}
