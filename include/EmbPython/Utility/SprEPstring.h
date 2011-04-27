#ifndef SPREPSTRING_H
#define SPREPSTRING_H

#include <Python/Python.h>
#include <Springhead.h>
#include <string>
#include <fstream>
#include "SprEPErrors.h"

using namespace Spr;



/********************************EPstring*************************************/
#define EPstring_Check(ob) (((PyObject*)ob)->ob_type == &EPstringType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	std::string *ptr;
} EPstring;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPstring_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPstring* newEPstring();
EPstring* newEPstring(std::string);
EPstring* newEPstring(std::string*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPstringType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPstring(void) ;

#endif