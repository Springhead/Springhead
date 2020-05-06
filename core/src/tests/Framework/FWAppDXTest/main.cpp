#include <Springhead.h>
#include <windows.h>
#include <string>
#include <cstdlib>
#include "MyApp.h"
MyApp app;
INT _stdcall WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow){
	app.Init(hInst, hPrevInstance, lpCmdLine, nCmdShow);
	app.dx9MainLoop();
}