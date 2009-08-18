#include "FWLDHapticSample.h"

FWLDHapticSample LDSample;

int _cdecl main(int argc, char* argv[]){
	LDSample.Init(argc, argv);
	LDSample.TimerStart();
	return 0;
}