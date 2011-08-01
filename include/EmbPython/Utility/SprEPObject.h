#ifndef SPREPOBJECT_H
#define SPREPOBJECT_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPFoundation.h"

using namespace Spr;




/********************************EPObject*************************************/
#define EPObject_Check(ob) (((PyObject*)ob)->ob_type == &EPObjectType)
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

PyObject* EPObject_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPObject* newEPObject();
EPObject* newEPObject(const void*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPObjectType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPObject(void) ;

#endif