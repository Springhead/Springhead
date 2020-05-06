/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */
#ifndef BASE_COMBINATION_H
#define BASE_COMBINATION_H

#include <assert.h>

/**	@file Combination.h 組み合わせ配列の定義*/
namespace Spr {;

/**	組み合わせ配列（入れ替え不可，4角形になる）*/
template <class T>
class UTCombination:public std::vector<T>{
	int height_;
	int width_;
public:
	typedef std::vector<T> base_type;
	UTCombination(){ height_ = 0; width_ = 0; }
	
	///	サイズの変更
	/// [0, min(height(), hnew))行，[0, min(width(), wnew)列の要素は保存される
	/// 新しくできる要素にはT()が代入される
	void resize(int hnew, int wnew){
		if (hnew * wnew > height_ * width_){	//	大きくなるなら，先にリサイズ
			base_type::resize(hnew * wnew);
		}
		typename base_type::iterator b = this->begin();
		int hmin = (std::min)(hnew, height_);
		int r, c;
		if (wnew > width_){	//	幅が増える場合，後ろから移動
			for(r = hmin-1; r >= 0; --r){
				for(c = wnew-1; c >= width_; --c)
					b[wnew * r + c] = T();
				for(; c >= 0; --c)
					b[wnew * r + c] = b[width_ * r + c];
			}
		}
		else if (wnew < width_){	//	幅が減る場合，前から移動
			for(r = 1; r < hmin; ++r){
				for(c = 0; c < wnew; ++c)
					b[wnew * r + c] = b[width_ * r + c];
			}
		}
		if (hnew * wnew < height_ * width_)	// 小さくなるなら最後にリサイズ
			base_type::resize(hnew * wnew);

		height_ = hnew;
		width_ = wnew;
	}
	/// r番目の行を削除し，[r+1, height())番目の行を1つ上に詰める
	void erase_row(int row){
		assert(0 <= row && row < height_);
		int r, c;
		for(r = row+1; r < height_; r++)
			for(c = 0; c < width_; c++)
				item(r-1, c) = item(r, c);
		height_--;
		base_type::resize(height_ * width_);
	}
	/// c番目の列を削除し，[c+1, width())番目の列を1つ左に詰める
	void erase_col(int col){
		assert(0 <= col && col < width_);
		int wnew = width_ - 1;
		/*if(col != width_ - 1){
			int r, c;
			for(c = col+1; c < width_; c++)
				for(r = 0; r < height_; r++)
					item(r, c-1) = item(r, c);
		}*/
		int r, c;
		typename base_type::iterator b = this->begin();
		for(r = 0; r < height_; r++){
			for(c = 0; c < width_; c++){
				if(c < col)
					b[wnew * r + c] = b[width_ * r + c];
				if(c > col)
					b[wnew * r + c-1] = b[width_ * r + c];
			}
		}
		width_ = wnew;
		base_type::resize(height_ * width_);
	}
	int height() const { return height_; };
	int width () const { return width_; };
	T& item(int i, int j){
		assert(i < height());
		assert(j < width());
		return this->begin()[i*width() + j];
	}
	void clear(){
		base_type::clear();
		height_ = 0;
		width_ = 0;
	}
	const T& item(int i, int j) const { return ((UTCombination<T>*)this)->item(i,j); }
};

}

#endif
