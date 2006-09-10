#ifndef SPR_FWAPP_H
#define SPR_FWAPP_H

#include <Framework/SprFWSdk.h>
#include <Framework/SprFWScene.h>

namespace Spr{;

class FWApp{
protected:
	UTRef<FWSdkIf> fwSdk;
	UTRef<FWSceneIf> fwScene;
	UTRef<GRDebugRenderIf> grRender;
	UTRef<GRDeviceIf> grDevice;
	bool isRunning;
public:
	enum DebugMode{
		DM_NONE,
		DM_DEBUG,
	} debugMode;
	FWApp();
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

	virtual int CreateWindow(int arg=0)=0;
	virtual DebugMode GetDebugMode(){ return debugMode; }
	virtual void SetDebugMode(DebugMode m){ debugMode = m; }

	FWSceneIf* GetFWScene(){ return fwScene; }
};

}
#endif
