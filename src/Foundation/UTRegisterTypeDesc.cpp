/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include <Foundation/Foundation.h>
#pragma hdrstop

#include <Base/TQuaternion.h>
#include <sstream>


namespace Spr{ 
//static UTTypeDesc* desc; 
//static UTTypeDesc::Field* field; 
extern void RegisterTypeDescFoundation(UTTypeDescDb* db);

void SPR_CDECL UTRegisterTypeDescs(){
	static bool bFirst=true;
	if (!bFirst) return;
	bFirst = false;

	UTTypeDescDb* db = UTTypeDescDbPool::Get("Foundation");
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
	db->RegisterDesc(new UTTypeDescNumber<DWORD>("size_t"));
	db->RegisterDesc(new UTTypeDescNumber<unsigned>("unsigned"));
	db->RegisterDesc(new UTTypeDescNumber<unsigned>("unsigned int"));
	db->RegisterDesc(new UTTypeDescNumber<DWORD>("enum"));

	db->RegisterDesc(new UTTypeDescNumber<float>("float"));
	db->RegisterDesc(new UTTypeDescNumber<double>("double"));
	typedef float FLOAT;
	typedef double DOUBLE;
	db->RegisterDesc(new UTTypeDescNumber<float>("FLOAT"));
	db->RegisterDesc(new UTTypeDescNumber<double>("DOUBLE"));
	db->RegisterDesc(new UTTypeDescString("string"));

	RegisterTypeDescFoundation(db);

	db->Link();
	db->LinkCheck();
}
}
