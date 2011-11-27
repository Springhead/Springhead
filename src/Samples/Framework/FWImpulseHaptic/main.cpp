//#include "MyApp.h"
#include "dev.h"

MyApp app;

int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}