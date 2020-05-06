/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef FILOADCONTEXT_H
#define FILOADCONTEXT_H

#include <Foundation/UTLoadContext.h>
#include <Foundation/UTLoadHandler.h>

namespace Spr{;

class FISdk;
class Import;

///	FILoadContext
class FILoadContext: public UTLoadContext{
public:
	UTStack<Import*>		importStack;	///< インポートスタック
	FISdk*					sdk;			///< FISdkへの参照（インポート時のFIFileオブジェクト作成用）

	virtual void PushFileMap(const UTString fn, bool binary);
	virtual void PopFileMap();
	virtual ~FILoadContext(){}
};

}
#endif
