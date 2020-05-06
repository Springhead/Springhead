/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FIIMPORT_H
#define FIIMPORT_H

#include <SprFileIO.h>
#include <Foundation/UTLoadHandler.h>
#include <Foundation/UTPath.h>

namespace Spr{;

/// Importのハンドラ
//  ハンドラの登録はFISdkが行う
class ImportHandler : public UTLoadHandler{
public:	
	virtual void AfterLoadData(UTLoadedData* ld, UTLoadContext* lc);
	virtual void AfterCreateObject(UTLoadedData* ld, UTLoadContext* lc);
	virtual void AfterCreateChild(UTLoadedData* ld, ObjectIf* child, UTLoadContext* lc);
	virtual void AfterCreateChildren(UTLoadedData* ld, UTLoadContext* lc);
	ImportHandler();
};

/// インポート関係を保持するDB
class Import : public Object, public UTTreeNode<Import>{
public:
	SPR_OBJECTDEF(Import);

	/// DOMノードとオブジェクトの対
	/*struct ObjectDataPair{
		UTLoadedData*	data;
		ObjectIf*		obj;

		ObjectDataPair():data(NULL), obj(NULL){}
	};
	typedef std::vector<ObjectDataPair>	ObjectDataPairs;*/

	//ObjectDataPair		ownerObj;		///< インポートするオブジェクトの親
	//ObjectDataPairs		childObjs;		///< インポートするオブジェクト
	ObjectIf*			ownerObj;
	ObjectIfs			childObjs;
	UTPath				path;			///< インポートするファイルのパス（相対）
	bool				loadOnly;

	virtual void Clear();
	virtual bool GetDesc(void* desc) const { ((ImportDesc*)desc)->path = path; return true; }
	
	void SetLoadOnly(bool on = true){ loadOnly = on; }

	// 子インポートエントリの中からオーナーオブジェクトがobjであるものを列挙
	void EnumByOwner(std::vector<Import*>& imports, ObjectIf* obj);

	// 子インポートエントリの中からDOMノードがdataであるObjectDataPairを列挙
	//void EnumByData(ObjectIfs& objs, UTLoadedData* data);

	Import(const ImportDesc& desc = ImportDesc()){ loadOnly = false; }
};

}

#endif
