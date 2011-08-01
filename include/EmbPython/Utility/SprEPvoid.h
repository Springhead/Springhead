#ifndef SPREPVOID_H
#define SPREPVOID_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPFoundation.h"
#include "SprEPObject.h"

using namespace Spr;




/********************************EPvoidObject*************************************/
#define EPvoidObject_Check(ob) (((PyObject*)ob)->ob_type == &EPvoidObjectType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	const void *ptr;
	MemoryManager mm;
} EPvoidObject;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPvoidObject_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
PyObject* newEPvoid();
PyObject* newEPvoid(const void*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPvoidObjectType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPvoidObject(void) ;

#endif