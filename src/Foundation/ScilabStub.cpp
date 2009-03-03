#include <Springhead.h>
#include <Foundation/UTDllLoader.h>
#include <Scilab/SprScilab.h>

namespace Scilab{
static Spr::UTDllLoader dllLoader;
#define SWIGDLL_GETPROC(x)	dllLoader.GetProc(x)

#include "ScilabStubImpl.cxx"
int (*StartScilabOrg)(char *SCIpath,char *ScilabStartup,int *Stacksize);

#define SC_IMP_STACKVAR(x) SC##x* p##x
SC_IMP_STACKVAR(stack);
SC_IMP_STACKVAR(recu);
SC_IMP_STACKVAR(iop);
SC_IMP_STACKVAR(errgst);
SC_IMP_STACKVAR(com);
SC_IMP_STACKVAR(adre);
SC_IMP_STACKVAR(intersci);
SC_IMP_STACKVAR(cha1);
SC_IMP_STACKVAR(dbg);
#undef SC_IMP_STACKVAR

bool StartScilab(char* SCIpath, char *ScilabStartup,int *Stacksize){
	if (!dllLoader.Load("LibScilab.dll", "C:\\Program Files\\scilab-4.1.2\\bin; D:\\Program Files\\scilab-4.1.2\\bin")) return false;
	InitScilabFunc();
	*(void **)&StartScilabOrg = SWIGDLL_GETPROC("StartScilab");
	if (!StartScilabOrg) return false;

#define SC_INIT_STACKVAR(x) *(void**)&p##x = SWIGDLL_GETPROC(#x "_");
	SC_INIT_STACKPTR(stack);
	SC_INIT_STACKPTR(recu);
	SC_INIT_STACKPTR(iop);
	SC_INIT_STACKPTR(errgst);
	SC_INIT_STACKPTR(com);
	SC_INIT_STACKPTR(adre);
	SC_INIT_STACKPTR(intersci);
	SC_INIT_STACKPTR(cha1);
	SC_INIT_STACKPTR(dbg);
#undef SC_INIT_STACKPTR

	return StartScilabOrg(SCIpath, ScilabStartup, Stacksize) != 0;
}

}
