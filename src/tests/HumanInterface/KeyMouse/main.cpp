#include <conio.h>
#include <Springhead.h>
#include <HumanInterface/SprHIKeyMouse.h>
#include <iomanip>
#include <gl/glut.h>
#include <Windows.h>

Spr::DRKeyMouseWin32If* win32KeyMouse;
WNDPROC OldWndProc;
LRESULT CALLBACK NewWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	MSG m;
	m.hwnd		= hWnd;
	m.message	= msg;
	m.wParam	= wParam;
	m.lParam	= lParam;
	if (win32KeyMouse->PreviewMessage(&m)) {
		return 0L;
	}
	return CallWindowProc(OldWndProc, hWnd, msg, wParam, lParam);
}

using namespace Spr;
UTRef<HISdkIf> sdk;
UTRef<HITrackballIf> trackball;
void _cdecl Disp(){
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,1);
	glRasterPos2f(-0.95f, 0.3f);
	char* p = "Drag this area to change the pose.";
	while (*p != '\0') glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
	glRasterPos2f(-0.95f, -0.3f);
	p = "To quit, press 'Q'.";
	while (*p != '\0') glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p++);
	glFlush();
}


//	メインの処理
void _cdecl Idle(){
	//	初期化処理
	if (!OldWndProc){
		//	PreviewMesssage()を呼び出すためのプロシージャの入れ替え
		//	hWndの取得
		HWND hWnd = FindWindow(NULL, "KeyMouse Test");
		// ウィンドウプロシージャを置き換え
		OldWndProc = (WNDPROC)(GetWindowLongPtr(hWnd, GWLP_WNDPROC));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)(NewWndProc));

		//	KeyMouseとTrackballの作成
		HISdkIf::RegisterSdk();
		sdk = HISdkIf::CreateSdk();
		sdk->AddRealDevice(DRKeyMouseWin32If::GetIfInfoStatic());

		//	実デバイスの取得
		win32KeyMouse = sdk->FindRealDevice(DRKeyMouseWin32If::GetIfInfoStatic())->Cast();
		sdk->Print(DSTR);
		trackball = sdk->CreateHumanInterface("HITrackball")->Cast();
		trackball->SetKeyMouse(sdk->RentVirtualDevice("DVKeyMouse", NULL, (int)hWnd)->Cast());
	}

	//	キーの読み出しなど
	if (trackball->GetKeyMouse()->GetKeyState('Q') & DVKeySt::PRESSED)
		exit(0);
	for(int i=0; i<200; ++i){
		if (trackball->GetKeyMouse()->GetKeyState(i) & DVKeySt::PRESSED){
			std::cout << i << " '"<< (char)i << "' " << std::endl;
		}
	}
	std::cout << "pose:" << trackball->GetPose() << std::endl;
}

int __cdecl main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("KeyMouse Test");
	glutIdleFunc(Idle);
	glutDisplayFunc(Disp);
	glutMainLoop();
}
