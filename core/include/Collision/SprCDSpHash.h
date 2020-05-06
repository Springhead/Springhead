#ifndef SPRCDSPHASH_H
#define SPRCDSPHASH_H

#include <Foundation/SprObject.h>

namespace Spr{;

struct CDBounds
{
public:
	CDBounds() {}
	inline CDBounds(const Vec3d &min0, const Vec3d &max0) {
		min = min0; max = max0;

	}

	inline void set(const Vec3d &min0, const Vec3d &max0) { min = min0; max = max0; }

	void clamp(Vec3f &pos) {//‹«ŠE‚ð’´‚¦‚½“_‚ð‚»‚Ì‚Ü‚Ü‹«ŠE‚Ì‚Ç‚±‚ëÝ’è‚·‚é
		if (isEmpty()) return;
		//pos.maximum(min);
		maximum(pos, min);
		minimum(pos, max);
	};
	inline bool isEmpty() const {
		if (min.x > max.x&&min.y > max.y&&min.z > max.z) return true;
		return false;
	}
	inline void minimum(Vec3f &self, Vec3f &other) {
		if (other.x < self.x) self.x = other.x;
		if (other.y < self.y) self.y = other.y;
		if (other.z < self.z) self.z = other.z;

	}
	inline void maximum(Vec3f &self, Vec3f &other) {
		if (other.x > self.x) self.x = other.x;
		if (other.y > self.y) self.y = other.y;
		if (other.z > self.z) self.z = other.z;
	}
	Vec3f min, max;
};

struct CDSpatialHashTableIf : ObjectIf{
	SPR_IFDEF(CDSpatialHashTable);

	void Init(float cellSize, CDBounds bounds);
	bool ReducedSelectPointsToAdd(Vec3f &pCtr, float radius, int objInd, int pInd, int timestamp);
};

struct CDSpatialHashTableDesc{
	
	
};

}
#endif
