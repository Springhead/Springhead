#ifndef UTCRITICALSECTION_H
#define UTCRITICALSECTION_H
#include <Base/BaseUtility.h>

namespace Spr
{

class SPR_DLL UTCriticalSection
{
	void* cr;
public:
	UTCriticalSection();
	~UTCriticalSection();
	void Enter();
	void Leave();
};


struct UTAutoLock
{
	UTCriticalSection* cr;
	UTAutoLock(UTCriticalSection& c);
	UTAutoLock();
	~UTAutoLock();
};

}	//	namespace Spr

#endif
