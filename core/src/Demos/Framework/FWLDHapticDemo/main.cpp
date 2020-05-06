#include "FWLDHapticDemo.h"

FWLDHapticDemo app;

int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}