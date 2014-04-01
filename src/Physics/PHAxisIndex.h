/*
 *  Copyright (c) 2003-2012, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef PH_AXIS_INDEX_H
#define PH_AXIS_INDEX_H

namespace Spr{;

/// 拘束軸管理クラス．拘束の有無を保持し，拘束軸リストを作成する
template<int N>
class AxisIndex {

	/// 軸の有効・無効を保持するフラグ
	bool   bCurr[N], bPrev[N];

	/// 有効軸番号リスト
	int    idx[N];

	/// idxの要素数
	size_t sz;

	/// 拘束フラグから拘束軸番号リストを作成する
	void CreateList() {
		sz = 0;
		for (int i=0; i<N; ++i) { if (bCurr[i]) { idx[sz++] = i; } }
	}

public:

	/// 変化状態をあらわすenum
	enum Transition { IL_CONTINUE_ENABLED=0, IL_NEWLY_ENABLED, IL_DISABLED };
  
	/// コンストラクタ
	AxisIndex() {
		for (int i=0; i<N; ++i) { bPrev[i] = false; }
		Clear();
	}

	/// 軸番号iを拘束する
	void Enable(int i)  {
		bCurr[i] = true;
		CreateList();
	}

	/// 軸番号iの拘束を解除する
	void Disable(int i) {
		bCurr[i] = false;
		CreateList();
	}

	/// 軸番号iが拘束されているかどうかを取得する
	bool IsEnabled(int i) { return bCurr[i]; }

	/// 拘束軸番号リストのサイズを取得する
	int  size() { return (int)sz; }

	/// 拘束軸番号リストのn番目の軸番号を取得する
	int& operator[](int n){ return idx[n]; }

	/// 軸番号iの拘束が継続しているかどうかを返す
	bool IsContinued(int i) {
		return(bCurr[i] && bPrev[i]);
	}

	/// 拘束軸番号リストをクリア
	void Clear() {
		sz = 0;
		for (int i=0; i<N; ++i) {
			bPrev[i] = bCurr[i];
			bCurr[i] = false;
		}
	}
};

}

#endif//PH_AXIS_INDEX_H
