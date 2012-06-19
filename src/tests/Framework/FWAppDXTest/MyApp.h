#ifndef MYAPP_H
#define MYAPP_H
#include <windows.h>
#include <d3d9.h>

class MyApp{
	
private:
	HWND				hWnd;			// windowへのハンドル
	MSG					msg;
	LPCSTR				winClassName;	// winClassの識別子
	LPCTSTR				windowName;		// 表示されるWindowの名前
	LPDIRECT3D9         pD3D;			// Used to create the D3DDevice
	LPDIRECT3DDEVICE9   pD3DDevice;		// Our rendering device
	WNDCLASSEX			winClass;		// 色々関数を登録する構造体
	int					top;			// ウィンドウのモニタ上端からの位置
	int					left;			// ウィンドウのモニタ左端からの位置
	int					width;			// ウィンドウの幅
	int					height;			// ウィンドウの高さ
	static MyApp*		instance;
public:

	//コンストラクタ
	MyApp();
	//デストラクタ
	~MyApp();
	//このクラスの関数
	bool					Init(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow);
	HRESULT					InitD3D(const HWND hWnd);
	VOID					Render();
	VOID					CleanUp();
	static LRESULT CALLBACK	CallMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WINAPI			MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void					dx9MainLoop();
};

#endif