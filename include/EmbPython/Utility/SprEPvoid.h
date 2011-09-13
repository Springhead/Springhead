#ifndef SPREPVOID_H
#define SPREPVOID_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPObject.h"

using namespace Spr;




/********************************EPvoid*************************************/
#define EPvoid_Check(ob) (((PyObject*)ob)->ob_type == &EPvoidType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	const void *ptr;
	MemoryManager mm;
} EPvoid;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPvoid_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
PyObject* newEPvoid();
PyObject* newEPvoid(const void*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPvoidType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPvoid(void) ;

#endif