#include <Foundation/SprUTCriticalSection.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Spr
{

	UTCriticalSection::UTCriticalSection()
{
#ifdef _WIN32
		cr = new CRITICAL_SECTION();
	InitializeCriticalSection((LPCRITICAL_SECTION)cr);
#endif
}

UTCriticalSection::~UTCriticalSection()
{
#ifdef _WIN32
	DeleteCriticalSection((LPCRITICAL_SECTION)cr);
	delete (LPCRITICAL_SECTION)cr;
#endif
}
void UTCriticalSection:: Enter()
{
#ifdef _WIN32
	EnterCriticalSection((LPCRITICAL_SECTION)cr);
#endif
}
void UTCriticalSection::Leave()
{
#ifdef _WIN32
	LeaveCriticalSection((LPCRITICAL_SECTION)cr);
#endif
}


UTAutoLock::UTAutoLock(UTCriticalSection& c):cr(&c)
{
	cr->Enter();
}
static UTCriticalSection defaultCr;
UTAutoLock::UTAutoLock():cr(&defaultCr){
}
UTAutoLock::~UTAutoLock()
{
	cr->Leave();
}

} //namespace Spr


