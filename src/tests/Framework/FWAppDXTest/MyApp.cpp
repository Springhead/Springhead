#include "MyApp.h"
#include <windows.h>
#include <d3d9.h>

MyApp::MyApp(){
	hWnd			= NULL;
	winClassName	= "SprFWAppDXTest";
	windowName		= "Test Window";
	pD3D			= NULL;
	pD3DDevice		= NULL;
	
	winClass.cbSize			= sizeof(WNDCLASSEX);
	winClass.style			= CS_CLASSDC;
	winClass.lpfnWndProc	= CallMsgProc;
	winClass.cbClsExtra		= 0L;
	winClass.cbWndExtra		= 0L;
	winClass.hInstance		= GetModuleHandle(NULL);
	winClass.hIcon			= NULL;
	winClass.hCursor		= NULL;
	winClass.hbrBackground	= NULL;
	winClass.lpszMenuName	= NULL;
	winClass.lpszClassName	= winClassName;
	winClass.hIconSm		= NULL ;

	top						= 10;
	left					= 10;
	width					= 800;
	height					= 600;
}

MyApp::~MyApp(){
	UnregisterClass(winClassName, winClass.hInstance);
}

MyApp* MyApp::instance;
bool MyApp::Init(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow){
    instance = this;
	RegisterClassEx( &winClass );
	// Create the application's window
    hWnd = CreateWindow(winClassName,
						windowName,
						WS_OVERLAPPEDWINDOW,
						top,
						left,
						width,
						height,
						NULL, 
						NULL, 
						winClass.hInstance, 
						NULL );
	if(SUCCEEDED(InitD3D(hWnd))){
		// Show the window
		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
	}
	return true;
}

HRESULT MyApp::InitD3D(const HWND hWnd ){
    // Create the D3D object, which is needed to create the D3DDevice.
    if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Most parameters are
    // zeroed out. We set Windowed to TRUE, since we want to do D3D in a
    // window, and then set the SwapEffect to "discard", which is the most
    // efficient method of presenting the back buffer to the display.  And 
    // we request a back buffer format that matches the current desktop display 
    // format.
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the Direct3D device. Here we are using the default adapter (most
    // systems only have one, unless they have multiple graphics hardware cards
    // installed) and requesting the HAL (which is saying we want the hardware
    // device rather than a software one). Software vertex processing is 
    // specified since we know it will work on all cards. On cards that support 
    // hardware vertex processing, though, we would see a big performance gain 
    // by specifying hardware vertex processing.
    if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &pD3DDevice ) ) )
    {
        return E_FAIL;
    }

    // Device state would normally be set here

    return S_OK;
}

VOID MyApp::Render()
{
    if( NULL == pD3DDevice )
        return;

    // Clear the backbuffer to a blue color
    pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,255), 1.0f, 0 );
    
    // Begin the scene
    if( SUCCEEDED( pD3DDevice->BeginScene() ) )
    {
        // Rendering of scene objects can happen here
    
        // End the scene
        pD3DDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

VOID MyApp::CleanUp()
{
    if( pD3DDevice != NULL) 
        pD3DDevice->Release();

    if( pD3D != NULL)
        pD3D->Release();
}

LRESULT MyApp::CallMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam){
	return instance->MsgProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK MyApp::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            CleanUp();
            PostQuitMessage( 0 );
            return 0;

        case WM_PAINT:
            Render();
            ValidateRect( hWnd, NULL );
            return 0;
    }
    return DefWindowProc( hWnd, msg, wParam, lParam );
}

#define	DAILY_BUILD

int exitCount = 0;
void MyApp::dx9MainLoop(){
	exitCount ++;
	if (exitCount > 60*60) exit(0);
	
	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );
	
	while( GetMessage( &msg, NULL, 0, 0 ) ){
            TranslateMessage( &msg );
            DispatchMessage( &msg );
			
#ifdef	DAILY_BUILD
		if (++exitCount > 80) exit(0);
#endif
	}
}
