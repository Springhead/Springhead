#ifndef SCILAB_H
#define SCILAB_H
//	ScilabCall For Springhead2
extern "C"{
#include "machine.h"
typedef  void (*S_fp) __PARAMS((char *,int *));
}
namespace Spr{
bool StartScilab(char* SCIpath, char *ScilabStartup,int *Stacksize);
#include "SciLabStub.hpp"
}
#endif
