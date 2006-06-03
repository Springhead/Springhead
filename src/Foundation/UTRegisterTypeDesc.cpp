#include <Foundation/Foundation.h>
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include <Base/TQuaternion.h>
#include <sstream>
#include "TypeDescDump.h"


namespace Spr{ 
static UTTypeDesc* desc; 
static UTTypeDesc::Field* field; 

void UTRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	if(!UTTypeDescDb::theTypeDescDb) UTTypeDescDb::theTypeDescDb = new UTTypeDescDb;
	UTTypeDescDb* db = UTTypeDescDb::theTypeDescDb;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef unsigned int DWORD;
	db->RegisterDesc(new UTTypeDescBool<bool>("bool"));
	db->RegisterDesc(new UTTypeDescBool<bool>("BOOL"));
	db->RegisterDesc(new UTTypeDescNumber<char>("char"));
	db->RegisterDesc(new UTTypeDescNumber<short>("short"));
	db->RegisterDesc(new UTTypeDescNumber<int>("int"));
	db->RegisterDesc(new UTTypeDescNumber<BYTE>("BYTE"));
	db->RegisterDesc(new UTTypeDescNumber<WORD>("WORD"));
	db->RegisterDesc(new UTTypeDescNumber<DWORD>("DWORD"));
	db->RegisterDesc(new UTTypeDescNumber<unsigned>("unsigned"));
	db->RegisterDesc(new UTTypeDescNumber<DWORD>("enum"));

	db->RegisterDesc(new UTTypeDescNumber<float>("float"));
	db->RegisterDesc(new UTTypeDescNumber<double>("double"));
	typedef float FLOAT;
	typedef double DOUBLE;
	db->RegisterDesc(new UTTypeDescNumber<float>("FLOAT"));
	db->RegisterDesc(new UTTypeDescNumber<double>("DOUBLE"));

	db->RegisterDesc(new UTTypeDescString("string"));
	#include "TypeDescDumpImp.h"
	db->Link();
}
}
