#ifndef SPREPOSTREAM_H
#define SPREPOSTREAM_H

#include <Python/Python.h>
#include <Springhead.h>
#include <iosfwd>
#include <iostream>
#include <fstream>
#include "SprEPErrors.h"

using namespace Spr;



/********************************EPostream*************************************/
#define EPostream_Check(ob) (((PyObject*)ob)->ob_type == &EPostreamType)
///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	std::ostream *ptr;
} EPostream;

/////////////////////////////////////メソッド登録用
///voidのメソッドの定義

PyObject* EPostream_new(PyTypeObject *type,PyObject *args, PyObject *kwds);
EPostream* newEPostream();
EPostream* newEPostream(std::ostream*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPostreamType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPostream(void) ;


#endif