#ifndef SPREPCHAR_H
#define SPREPCHAR_H

#include <Python/Python.h>
#include "SprEPErrors.h"




/********************************EPchar*************************************/
#define EPchar_Check(ob) (((PyObject*)ob)->ob_type == &EPcharType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	char *ptr;
} EPchar;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPchar_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPchar* newEPchar();
EPchar* newEPchar(const char*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPcharType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPchar(void) ;


#endif