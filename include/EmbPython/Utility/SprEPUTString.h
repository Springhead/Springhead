#ifndef SPREPUTSTRING_H
#define SPREPUTSTRING_H

#include <Python/Python.h>
#include <Springhead.h>
#include <iosfwd>
#include <iostream>
#include <fstream>
#include "SprEPErrors.h"

using namespace Spr;



/********************************EPUTString*************************************/
#define EPUTString_Check(ob) (((PyObject*)ob)->ob_type == &EPUTStringType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	UTString *ptr;
} EPUTString;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPUTString_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPUTString* newEPUTString();
EPUTString* newEPUTString(UTString);
EPUTString* newEPUTString(UTString*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPUTStringType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPUTString(void) ;

#endif