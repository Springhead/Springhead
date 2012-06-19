/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef BASE_KEYLIST_H
#define BASE_KEYLIST_H
#include "Env.h"
#include <list>
#include <set>

/**	@file KeyList.h キーワード検索機能つきリストの定義*/
namespace Spr {;

/**	set つきのリスト．キーで高速に操作ができる list．
	setと違って，順番が保持され，順番でアクセスすることもできる． */

template <class T, class Pred=std DOUBLECOLON less<T> >
class UTKeyList: std DOUBLECOLON list<T>{
public:
	typedef std::list<T> base;
	typedef base::iterator iterator;
	typedef base::const_iterator const_iterator;
	struct SetPred{
		typedef TYPENAME std::list<T>::iterator listIt;
		bool operator() (const listIt& i1, const listIt& i2) const {
			Pred pred;
			return pred(*i1, *i2);
		}
	};
	typedef std::set< TYPENAME std::list<T>::iterator, SetPred > Finder;
	Finder finder;
	UTKeyList(){}
	UTKeyList(const UTKeyList<T>& l):std::list<T>(l){
		for(iterator it = begin(); it != end(); ++it) finder.insert(it);
	}
	void push_back(const T& t){
		base::push_back(t);
		InsToFinder(--base::end());
	}
	void pop_back(){
		EraseFromFinder(--base::end());
		base::pop_back();
	}
	void push_front(const T& t){
		base::push_front(t);
		InsToFinder(base::begin());
	}
	void pop_front(){
		EraseFromFinder(base::begin());
		base::pop_front();
	}
	void insert(base::iterator it, T& t = T()){
		base::iterator ins = base::insert(it, t);
		InsToFinder(ins);
	}
	void erase(base::iterator it){
		EraseFromFinder(it);
		base::erase(it);
	}
	void erase(const T& t){
		base::push_back(t);
		Finder::iterator rv = finder.find(--end());
		base::pop_back();
		if (rv != finder.end()) erase(*rv);
	}
	base::iterator find(const T& t){
		base::push_back(t);
		Finder::iterator rv = finder.find(--base::end());
		base::pop_back();
		if (rv == finder.end()){
			return end();
		}else{
			return *rv;
		}
	}
	base::const_iterator find(const T& t) const {
		return ((UTKeyList<T, Pred>*)this)->find(t);
	}
	base::iterator begin(){ return base::begin(); }
	base::const_iterator begin() const { return base::begin(); }
	base::iterator end(){ return base::end(); }
	base::const_iterator end() const { return base::end(); }
	size_t size() const { return base::size(); }

private:
	void InsToFinder(base::iterator& it){
		std::pair<Finder::iterator, bool> rv = finder.insert(it);
		if (!rv.second){	//	既に同じキーが存在するため，追加できなかったとき
			base::erase(*rv.first);	//	リストから古い値を削除し，
			(base::iterator&)*rv.first = it;			//	新しい値に更新する．結果としてlistの要素数が増えない．
		}
	}
	void EraseFromFinder(base::iterator& it){
		int ne = finder.erase(it);
		assert(ne == 1);
	}
};

}

#endif
