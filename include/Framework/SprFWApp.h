#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <Framework/SprFWSdk.h>
#include <Framework/SprFWScene.h>

namespace Spr{;

class FWVFuncBridge : public UTRefCount{
public:
	virtual void Link(void* pObj) = 0;
	virtual bool Display() = 0;
	virtual bool Reshape(int w, int h) = 0;
	virtual bool Keyboard(unsigned char key, int x, int y) = 0;
	virtual bool MouseButton(int button, int state, int x, int y) = 0;
	virtual bool MouseMove(int x, int y) = 0;
	virtual bool Step() = 0;
	virtual void AtExit() = 0;
};

class FWApp{
protected:
	UTRef<FWSdkIf> fwSdk;
	UTRef<FWSceneIf> fwScene;
	UTRef<GRDebugRenderIf> grRender;
	UTRef<GRDeviceIf> grDevice;
	bool isRunning;

	void CallDisplay(){
		if(!vfBridge || !vfBridge->Display())
			Display();
	}
	void CallReshape(int w, int h){
		if(!vfBridge || !vfBridge->Reshape(w, h))
			Reshape(w, h);
	}
	void CallKeyboard(unsigned char key, int x, int y){
		if(!vfBridge || !vfBridge->Keyboard(key, x, y))
			Keyboard(key, x, y);
	}
	void CallMouseButton(int button, int state, int x, int y){
		if(!vfBridge || !vfBridge->MouseButton(button, state, x, y))
			MouseButton(button, state, x, y);
	}
	void CallMouseMove(int x, int y){
		if(!vfBridge || !vfBridge->MouseMove(x, y))
			MouseMove(x, y);
	}
	void CallStep(){
		if(!vfBridge || !vfBridge->Step())
			Step();
	}
public:

	UTRef<FWVFuncBridge>	vfBridge;

	enum DebugMode{
		DM_NONE,
		DM_DEBUG,
	} debugMode;
	FWApp();

	/// ƒŒƒ“ƒ_ƒ‰‚ğæ“¾
	GRRenderIf* GetRender(){return grRender;}
	/// ƒV[ƒ“‚ğæ“¾
	FWSceneIf* GetScene(){return fwScene;}

	virtual ~FWApp();
	virtual void Init(int argc, char* argv[]);
	virtual void ProcessArguments(int argc, char* argv[]);
	virtual void LoadScene(UTString filename);
	virtual void Display();
	virtual void Reshape(int w, int h);
	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void MouseButton(int button, int state, int x, int y);
	virtual void MouseMove(int x, int y);
	virtual void Step();
	virtual DebugMode GetDebugMode(){ return debugMode; }
	virtual void SetDebugMode(DebugMode m){ debugMode = m; }

	FWSceneIf* GetFWScene(){ return fwScene; }
};

}
#endif
