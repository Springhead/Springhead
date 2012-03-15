#ifndef MYAPP_H
#define MYAPP_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class MyApp : public FWApp{
public:
	PHSceneIf* phscene;
	PHHapticPointerIf* pointer;
	int physicsTimerID;
	int hapticTimerID;
	UTRef<HIBaseIf> spg;

	void InitInterface();
	void Init(int argc = 0, char* argv[] = 0);
	void Keyboard(int key, int x, int y);
	virtual void TimerFunc(int id);
};


#endif