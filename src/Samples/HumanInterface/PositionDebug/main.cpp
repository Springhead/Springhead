#include "PositionDebug.h"

FWLDHapticSample LDSample;

int _cdecl main(int argc, char* argv[]){
	LDSample.Init(argc, argv);
	LDSample.Start();
	return 0;
}