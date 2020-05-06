#include "App.h"

App app;

#if _MSC_VER <= 1600
#include <GL/glut.h>
#endif
int _cdecl main(int argc, char* argv[]){
	app.Init(argc, argv);
	app.StartMainLoop();
	return 0;
}
