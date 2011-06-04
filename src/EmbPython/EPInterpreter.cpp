
#include "..\..\include\EmbPython\SprEPInterpreter.h"

using namespace Spr;

//マルチスレッド用
#include <process.h>

#ifdef _WIN32
#define NOMINMAX //std::maxなどを使うのに必要(windows.hと競合するから)
#include <windows.h>
#endif

EPInterpreter* EPInterpreter::instance = 0;

EPInterpreter* EPInterpreter::Create()
{
	if( EPInterpreter::instance == 0)
		EPInterpreter::instance = new EPInterpreter();
	
	return EPInterpreter::instance;
}

EPInterpreter::EPInterpreter()
{
	this->State = UNSTARTED;
}

EPInterpreter::~EPInterpreter()
{
	this->State = STOP;
}


extern "C" { PyObject* PyInit__Base(void); }


void EPInterpreter::Initialize()
{
	//PYTHONPATHを変更する
	//SPRPYTHONPATHにSpringheadPythonで使うLIBのPATHを追加しておく
	std::string newPath;
	char buff[1024];
	if (!GetEnvironmentVariable("SPRPYTHONPATH",buff,1024) || !strlen(buff)){
		DSTR << "Warning: " << "Can not find environment variable of 'SPRPYTHONPATH'. " 
			<< "Embeded python may cause serious error." << std::endl;
	}
	newPath.append(buff);
	newPath.append(";");
	buff[0] = '\0';
	GetEnvironmentVariable("PYTHONPATH", buff,1024);
	newPath.append(buff);
	if (newPath.length() <= 1){
		DSTR << "Warning: " << "Can not find environment variable of 'PYTHONPATH' and 'SPRPYTHONPATH'. " 
			<< "Embeded python will causes buffer over run error." << std::endl;
	}
	
	SetEnvironmentVariable("PYTHONPATH", newPath.c_str());
	// SetEnvironmentVariable("PYTHONHOME", newPath.c_str());


	//パイソン初期化
	Py_Initialize();

	PyObject*m = PyInit__Base();
	PyObject *modules = PyImport_GetModuleDict();
	if (PyDict_SetItemString(modules, "_Base", m) != 0) {
		Py_DECREF(m);
	}



	////手作りクラスの初期化
	initUtility();

	////モジュール読み込み
	initFoundation();
	initCollision();
	initPhysics();
	//initGraphics();
	//initCreature();
}

void EPInterpreter::Run(void* arg)
{
	if( this->State == UNSTARTED || this->State == STOP)
	{
		_beginthread(EPLoopLauncher, 0 , arg);
		this->State = RUN;
	}
}

void EPInterpreter::Stop()
{
	if( this->State != UNSTARTED)
		this->State = STOP;
}

bool EPInterpreter::BindInt(int i)
{
	return false;
}

void EPInterpreter::EPLoopLauncher(void* arg)
{
	EPInterpreter::instance->EPLoopInit(arg);
	while(EPInterpreter::instance->State == RUN)
	{
		EPInterpreter::instance->EPLoop(arg);
	}
}
