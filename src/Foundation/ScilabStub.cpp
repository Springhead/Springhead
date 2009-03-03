#include <Springhead.h>
#include <Foundation/UTDllLoader.h>
#include <SciLab/SprSciLab.h>

namespace Spr{
static UTDllLoader dllLoader;
#define SWIGDLL_GETPROC(x)	dllLoader.GetProc(x)
#include "SciLabStubImpl.cxx"
int (*StartScilabOrg)(char *SCIpath,char *ScilabStartup,int *Stacksize);


bool StartScilab(char* SCIpath, char *ScilabStartup,int *Stacksize){
	if (!dllLoader.Load("LibScilab.dll", "C:\\Program Files\\scilab-4.1.2\\bin; D:\\Program Files\\scilab-4.1.2\\bin")) return false;
	InitScilabFunc();
	*(void **)&StartScilabOrg = SWIGDLL_GETPROC("StartScilab");
	if (!StartScilabOrg) return false;
	return StartScilabOrg(SCIpath, ScilabStartup, Stacksize) != 0;
}

}
