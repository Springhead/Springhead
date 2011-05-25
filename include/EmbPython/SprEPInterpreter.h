#ifndef SPREPInterpreter_H
#define SPREPInterpreter_H

#include <EmbPython\EmbPython.h>

namespace Spr{

enum EPState{
	RUN,
	STOP,
	UNSTARTED,
};

class EPInterpreter{
public:
	static EPInterpreter* Create();
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
	static void EPLoopLauncher(void *arg);
};

};	//namespace Spr

#endif