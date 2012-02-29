#ifndef DEV_H
#define DEV_H

#include <Springhead.h>
#include <Framework/SprFWApp.h>

using namespace Spr;

class MyApp : public FWApp{
public:
	PHSceneIf* phscene;
	PHHapticPointerIf* pointer, *pointer2;
	double range;
	int physicsTimerID;
	int hapticTimerID;
	UTRef<HIBaseIf> spg, spg2;
	UTRef<HISdkIf> hiSdk;
	UTTimerIf* timer;
	PHSolidIf* sobox;

	void InitInterface();
	void Init(int argc = 0, char* argv[] = 0);
	void UserFunc();
	void Keyboard(int key, int x, int y);
	virtual void TimerFunc(int id);
};


#endif