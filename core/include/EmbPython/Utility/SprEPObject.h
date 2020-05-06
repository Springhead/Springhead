#ifndef SPREPOBJECT_H
#define SPREPOBJECT_H

#include <EmbPython/SprEPVersion.h>
#include PYTHON_H_PATH

#include <Springhead.h>

using namespace Spr;



enum MemoryManager{
	EP_MM_SPR = 0,
	EP_MM_PY
};



/********************************EPObject*************************************/
#define EPObject_Check(ob) PyObject_TypeCheck(ob,&EPObjectType)
#define EPObject_Ptr(ob) (((EPObject*)ob)->ptr)
#define EPObject_Cast(ob,dest) ((dest*)((EPObject*)ob)->ptr)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	const void *ptr;
	MemoryManager mm;
	
} EPObject;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* __PYDECL EPObject_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
PyObject* newEPObject();
PyObject* newEPObject(const void*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPObjectType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
void initEPObject(PyObject* dict =NULL) ;

#endif