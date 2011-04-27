#include "..\..\include\EmbPython\UTCriticalSection.h"

#include <windows.h>

namespace Spr
{

UTCriticalSection::UTCriticalSection()
{
	cr = new CRITICAL_SECTION();
	InitializeCriticalSection((LPCRITICAL_SECTION)cr);
}

UTCriticalSection::~UTCriticalSection()
{
	DeleteCriticalSection((LPCRITICAL_SECTION)cr);
	delete (LPCRITICAL_SECTION)cr;
}
void UTCriticalSection:: Enter()
{
	EnterCriticalSection((LPCRITICAL_SECTION)cr);
}
void UTCriticalSection::Leave()
{
	LeaveCriticalSection((LPCRITICAL_SECTION)cr);
}


UTAutoLock::UTAutoLock(UTCriticalSection& c):cr(c)
{
	cr.Enter();
}

UTAutoLock::~UTAutoLock()
{
	cr.Leave();
}

} //namespace Spr


