#include <Springhead.h>
#include <Foundation/UTDllLoader.h>
#include <Scilab/SprScilab.h>

namespace Scilab{
static Spr::UTDllLoader dllLoader;
static Spr::UTDllLoader dllLoader2;
#define SWIGDLL_GETPROC(x) ((dllLoader.GetProc(x)) ? dllLoader.GetProc(x) : dllLoader2.GetProc(x))
#include "ScilabStubImpl.cxx"

int (SPR_CDECL *StartScilabOrg)(char *SCIpath, char *ScilabStartup,int *Stacksize);
StrCtx** ppvApiCtx;

bool StartScilab(char* SCIpath, char *ScilabStartup,int *Stacksize){
	const char* dllPath = "C:\\Program Files\\scilab-5.4.0\\bin;C:\\Program Files (x86)\\scilab-5.4.0\\bin";
	if(!dllLoader.Load("call_Scilab.dll", dllPath)) return false;
	if(!dllLoader2.Load("api_Scilab.dll", dllPath)) return false;

	InitScilabFunc();
	*(void **)&ppvApiCtx = SWIGDLL_GETPROC("pvApiCtx");
	*(void **)&StartScilabOrg = SWIGDLL_GETPROC("StartScilab");
	if (!StartScilabOrg) return false;

	return StartScilabOrg(SCIpath, ScilabStartup, Stacksize) != 0;
}

}
