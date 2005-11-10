#ifndef VR_BASE_RINGBUF_H
#define VR_BASE_RINGBUF_H
#include "Env.h"
#include <vector>
/**	@file RingBuf.h リングバッファ(FIFOを実現するための環状バッファ)クラスの定義*/
namespace Spr {


template <class T>
class UTRingBuffer:private std::vector<T>{
public:
	typedef std::vector<T> container;
	typedef std::vector<T> base_class;
protected:
	int cur;
public:
	T zero;
	UTRingBuffer(int sz=0){
		cur = 0;
		size(sz);
	}
	int size() const{ return container::size(); }
	void size(int sz){
		container::resize(sz);
		if (cur >= size()) cur = 0;
	}
	void clear(){
		for(int i=0; i<size(); i++) begin()[i] = zero;
	}
	void inc(){
		cur ++;
		if (cur>=size()) cur = 0;
	}
	void dec(){
		cur --;
		if (cur<0) cur = size()-1;
	}
	T& operator[] (int i){
		i += cur + size();
		i = i % size();
		return begin()[i];
	}
};

}	//	namespace Spr

#endif
