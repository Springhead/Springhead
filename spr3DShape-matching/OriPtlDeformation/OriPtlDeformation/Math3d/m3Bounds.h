#ifndef M3_BOUNDS_H
#define M3_BOUNDS_H


#include "m2Real.h"


using namespace Spr;
class m3Bounds
{	
public:
	m3Bounds(){}
	inline m3Bounds(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }

	inline void set(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }
	
	void clamp(Vec3f &pos){//‹«ŠE‚ð’´‚¦‚½“_‚ð‚»‚Ì‚Ü‚Ü‹«ŠE‚Ì‚Ç‚±‚ëÝ’è‚·‚é
		if (isEmpty()) return;
		//pos.maximum(min);
		maximum(pos,min);
		minimum(pos,max);
	};
	inline bool isEmpty() const { 
		if (min.x > max.x&&min.y > max.y&&min.z > max.z) return true;
		return false;
	}
	inline void minimum(Vec3f &self,Vec3f &other) {
		if (other.x < self.x) self.x = other.x;
		if (other.y < self.y) self.y = other.y;
		if (other.z < self.z) self.z = other.z;

	}
	inline void maximum(Vec3f &self,Vec3f &other) {
		if (other.x > self.x) self.x = other.x;
		if (other.y > self.y) self.y = other.y;
		if (other.z > self.z) self.z = other.z;
	}
	Vec3f min, max;
};



#endif