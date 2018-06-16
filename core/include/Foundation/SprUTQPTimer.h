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
class UTPerformanceMeasure{
public:
	static UTPerformanceMeasure* GetInstance(const char* name);		///<	Find or Create UTPerformanceMeasure with specified name.
	static UTPerformanceMeasure* CreateInstance(const char* name);	///<	Create UTPerformanceMeasure with specified name.
	static UTPerformanceMeasure* FindInstance(const char* name);	///<	Find UTPerformanceMeasure by name.
	static UTPerformanceMeasure* GetInstance(int i);				///<	Get i-th instance.
	static int NInstance();											///<	return the number of intstance.
	virtual const char* GetName() = 0;
	virtual int NCounter() = 0;
	virtual const char* GetNameOfCounter(int id) = 0;
	virtual std::string PrintAll() = 0;
	virtual std::string Print(std::string name) = 0;
	virtual std::string Print(int id) = 0;
	virtual void SetUnit(double u) = 0;
	virtual double GetUnit() = 0;
	virtual double Time(int id) = 0;
	virtual double Time(const char* name) = 0;
	virtual UTLongLong& Count(const char* name) = 0;
	virtual UTLongLong& Count(int id) = 0;
	virtual int GetId(std::string name) = 0;
	virtual int FindId(std::string name) = 0;
	virtual int CreateId(std::string name) = 0;
	virtual void ClearCounts() = 0;
};

}	//	namespace Spr
#endif
