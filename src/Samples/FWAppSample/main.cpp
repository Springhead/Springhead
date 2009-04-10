#include "FWAppSample.h"

FWAppSample app;
int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.Start();
	return 0;
}