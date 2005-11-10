#ifndef BASE_COMBINATION_H
#define BASE_COMBINATION_H
#include "Env.h"
#include <list>
#include <set>

/**	@file Combination.h キーワード検索機能つきリストの定義*/
namespace Spr {;

/**	組み合わせ配列（入れ替え不可，4角形になる）*/
template <class T>
class UTCombination:public std::vector<T>{
	int height_;
	int width_;
public:
	typedef std::vector<T> base_type;
	UTCombination(){ height_ = 0; width_ = 0; }
	///	サイズの設定	
	void resize(int h, int w){
		if (h*w > height_*width_){	//	大きくなるなら，先にリサイズ
			base_type::resize(h * w);
		}
		iterator b = begin();
		if (h < height_){	//	使わなくなった行をクリア
			for(int l=h; l<height_; ++l){
				for(int x=0; x<width_; ++x) b[width_*l+x] = T();
			}
		}
		if (w > width_){	//	幅が増える場合，後ろから移動
			for(int l=height_-1; l>=0; --l){
				for(int x=w; x > width_-1; --x) b[w*l+x]=T();
				for(int x=width_-1; x>=0; --x) b[w*l+x] = b[width_*l+x];
			}
		}else if (w < width_){	//	幅が減る場合，前から移動
			for(int l=1; l<height_; ++l){
				for(int x=0; x<width_; ++x) b[w*l+x] = b[width_*l+x];
			}
		}
		if (h*w > height_*width_){	//	小さくなるなら最後にリサイズ
			base_type::resize(h * w);
		}
		height_ = h;
		width_ = w;
	}
	int height(){ return height_; };
	int width(){ return width_; };
	T& item(int i, int j){
		assert(i < height());
		assert(j < width());
		return begin()[i*width() + j];
	}
	void clear(){
		base_type::clear();
		height_ = 0;
		width_ = 0;
	}
	const T& item(int i, int j) const { return ((UTCombination<T>*)this)->item(i,j); }
//	T& operator [] (int i, int j){ item(i,j); }
//	const T& operator [] (int i, int j) const { item(i,j); }
};

}

#endif
