#ifndef SPREPISTREAM_H
#define SPREPISTREAM_H

#include <Python/Python.h>
#include <iostream>
#include "SprEPErrors.h"



/********************************EPistream*************************************/
#define EPistream_Check(ob) (((PyObject*)ob)->ob_type == &EPistreamType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	std::istream *ptr;
} EPistream;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPistream_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPistream* newEPistream();
EPistream* newEPistream(std::istream*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPistreamType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPistream(void) ;


#endif