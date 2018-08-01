/*
 *  Copyright (c) 2003-2008, Shoichi Hasegawa and Springhead development team 
 *  All rights reserved.
 *  This software is free software. You can freely use, distribute and modify this 
 *  software. Please deal with this software under one of the following licenses: 
 *  This license itself, Boost Software License, The MIT License, The BSD License.   
 */

/** @file SprUTTimer.h
	UTQPTimer
 */

#ifndef SPR_UTQPTIMER_H
#define SPR_UTQPTIMER_H

#include <Foundation/SprObject.h>

namespace Spr{;

typedef	long long	UTLongLong;	// 64-bit signed integer
class UTPerformanceMeasureIf:public ObjectIf{
public:
	SPR_IFDEF(UTPerformanceMeasure);
	static UTPerformanceMeasureIf* GetInstance(const char* name);		///<	Find or Create UTPerformanceMeasure with specified name.
	static UTPerformanceMeasureIf* CreateInstance(const char* name);	///<	Create UTPerformanceMeasure with specified name.
	static UTPerformanceMeasureIf* FindInstance(const char* name);		///<	Find UTPerformanceMeasure by name.
	static UTPerformanceMeasureIf* GetInstance(int i);					///<	Get i-th instance.
	static int NInstance();												///<	return the number of intstance.
	const char* GetName();
	int NCounter();
	const char* GetNameOfCounter(int id);
	std::string PrintAll();
	std::string Print(std::string name);
	std::string Print(int id);
	void SetUnit(double u);
	double GetUnit();
	double Time(int id);
	double Time(const char* name);
	UTLongLong& Count(const char* name);
	UTLongLong& Count(int id);
	int GetId(std::string name);
	int FindId(std::string name);
	int CreateId(std::string name);
	void ClearCounts();
};

}	//	namespace Spr
#endif
