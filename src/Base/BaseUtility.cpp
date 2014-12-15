/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#include "Base.h"
#ifdef USE_HDRSTOP
#pragma hdrstop
#endif

#include <iostream>
namespace Spr {
using namespace std;

#ifdef _DEBUG
int UTRefCount::nObject;
UTRefCount::UTRefCount(){
	refCount = 0;
	nObject++;
}
UTRefCount::UTRefCount(const UTRefCount&){
	refCount = 0;
	nObject++;
}
UTRefCount::~UTRefCount(){
	assert(refCount==0); 
	nObject--;
}
#else
UTRefCount::UTRefCount(){
	refCount = 0;
}
UTRefCount::UTRefCount(const UTRefCount&){
	refCount = 0;
}
UTRefCount::~UTRefCount(){
	assert(refCount==0);
}
#endif

std::ostream& operator << (std::ostream& os, UTPadding p){
	if (p.len == -1){
		p.len = os.width();
	}
	for(int i=0; i<p.len; i++) os << ' ';
	return os;
}
std::istream& operator >> (std::istream& is, const UTEatWhite& e){
	while(is.good()){
		char ch = is.peek();
		if (ch != ' ' && ch != '\t' && ch != '\r' && ch != '\n') break;
		is.get();
	}
	return is;
}

}	//	namespace Spr
