#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <Framework/SprFWSdk.h>
#include <Framework/SprFWScene.h>

namespace Spr{;

class FWWindowDesc{	//	hase	TypeDescができないようにクラスにしてある。TypeDesc側での対応が望ましい。
public:
	int width;
	int height;
	int left;
	int top;
	bool fullscreen;
	FWWindowDesc(int w=640, int h=480, int l=-1, int t=-1, bool f=false):width(w), height(h), left(l), top(t), fullscreen(f){}
};

class FWVFuncBridge : public UTRefCount{
public:
	virtual void Link(void* pObj) = 0;
	virtual bool Display() = 0;
	virtual bool Reshape(int w, int h) = 0;
	virtual bool Keyboard(unsigned char key, int x, int y) = 0;
	virtual bool MouseButton(int button, int state, int x, int y) = 0;
	virtual bool MouseMove(int x, int y) = 0;
	virtual bool Step() = 0;
};

class FWApp{
protected:
	UTRef<FWSdkIf> fwSdk;
	UTRef<FWSceneIf> fwScene;
	UTRef<GRDebugRenderIf> grRender;
	UTRef<GRDeviceIf> grDevice;
	bool isRunning;

	void CallDisplay(){
#ifdef HLANGPORT
		if(!vfBridge || !vfBridge->Display())
#endif
			Display();
	}
	void CallReshape(int w, int h){
#ifdef HLANGPORT
		if(!vfBridge || !vfBridge->Reshape(w, h))
#endif
			Reshape(w, h);
	}
	void CallKeyboard(unsigned char key, int x, int y){
#ifdef HLANGPORT
		if(!vfBridge || !vfBridge->Keyboard(key, x, y))
#endif
			Keyboard(key, x, y);
	}
	void CallMouseButton(int button, int state, int x, int y){
#ifdef HLANGPORT
		if(!vfBridge || !vfBridge->MouseButton(button, state, x, y))
#endif
			MouseButton(button, state, x, y);
	}
	void CallMouseMove(int x, int y){
#ifdef HLANGPORT
		if(!vfBridge || !vfBridge->MouseMove(x, y))
#endif
			MouseMove(x, y);
	}
	void CallStep(){
#ifdef HLANGPORT
		if(!vfBridge || !vfBridge->Step())
#endif
			Step();
	}
public:

	UTRef<FWVFuncBridge>	vfBridge;

	enum DebugMode{
		DM_NONE,
		DM_DEBUG,
	} debugMode;
	FWApp();
	virtual ~FWApp();
	virtual void Init(int argc, char* argv[]);
	virtual void ProcessArguments(int argc, char* argv[]);
	virtual void LoadScene(UTString filename);
	virtual void Start(){}
	virtual void Display();
	virtual void Reshape(int w, int h);
	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void MouseButton(int button, int state, int x, int y);
	virtual void MouseMove(int x, int y);
	virtual void Step();
	virtual int CreateWindow(const FWWindowDesc d=FWWindowDesc())=0;
	virtual DebugMode GetDebugMode(){ return debugMode; }
	virtual void SetDebugMode(DebugMode m){ debugMode = m; }

	FWSceneIf* GetFWScene(){ return fwScene; }
};

}
#endif
