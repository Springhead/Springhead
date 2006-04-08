#include "FileIO.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include <Base/TQuaternion.h> 
#include <FileIO/FITypeDesc.h> 
#include <FileIO/FIBaseType.h> 
#include <sstream>
#include "TypeDescDump.h"


namespace Spr{ 
static FITypeDesc* desc; 
static FITypeDesc::Field* field; 

void RegisterTypes(){
	if(!FITypeDescDb::theTypeDescDb) FITypeDescDb::theTypeDescDb = new FITypeDescDb;
	FITypeDescDb* db = FITypeDescDb::theTypeDescDb;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned int DWORD;
	db->RegisterDesc(new FITypeDescBool<bool>("bool"));
	db->RegisterDesc(new FITypeDescBool<bool>("BOOL"));
	db->RegisterDesc(new FITypeDescNumber<char>("char"));
	db->RegisterDesc(new FITypeDescNumber<short>("short"));
	db->RegisterDesc(new FITypeDescNumber<int>("int"));
	db->RegisterDesc(new FITypeDescNumber<BYTE>("BYTE"));
	db->RegisterDesc(new FITypeDescNumber<WORD>("WORD"));
	db->RegisterDesc(new FITypeDescNumber<DWORD>("DWORD"));
	db->RegisterDesc(new FITypeDescNumber<unsigned>("unsigned"));
	db->RegisterDesc(new FITypeDescNumber<DWORD>("enum"));

	db->RegisterDesc(new FITypeDescNumber<float>("float"));
	db->RegisterDesc(new FITypeDescNumber<double>("double"));
	typedef float FLOAT;
	typedef double DOUBLE;
	db->RegisterDesc(new FITypeDescNumber<float>("FLOAT"));
	db->RegisterDesc(new FITypeDescNumber<double>("DOUBLE"));

	db->RegisterDesc(new FITypeDescString("string"));
	#include "TypeDescDumpImp.h"
	db->Link();
}
}
