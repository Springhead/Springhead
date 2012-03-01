#ifndef SPREPInterpreter_H
#define SPREPInterpreter_H

#include <EmbPython\EmbPython.h>

namespace Spr{


class EPInterpreter{
public:
	enum EPState{
		RUN,
		STOP,
		UNSTARTED,
	};
	static EPInterpreter* Create();
	static void           Destroy();
	EPState State;
	EPInterpreter();
	~EPInterpreter();
	virtual void Initialize();
	virtual void Run(void*);
	virtual void Stop();
	virtual bool BindInt(int);
	
	void (*EPLoop)(void*);
	void (*EPLoopInit)(void*);

private:
	static EPInterpreter* instance;
	static void SPR_CDECL EPLoopLauncher(void *arg);
};

};	//namespace Spr

#endif