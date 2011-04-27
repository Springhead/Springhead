#ifndef SPREPPOSE_H
#define SPREPPOSE_H

#include <Python/Python.h>
#include <Springhead.h>
#include "SprEPErrors.h"
using namespace Spr;



///マクロ
#define EPPosed_Check(ob) (((PyObject*)ob)->ob_type == &EPPosedType)
#define EPPosed_ArgParseTuple( arg ) double wi,xi,yi,zi,pxi,pyi,pzi; PyArg_ParseTuple( arg , "ddddddd",&wi,&xi,&yi,&zi,&pxi,&pyi,&pzi )
#define EPPosed_BuildValue(self) Py_BuildValue("ddddddd",self->ptr->W(),self->ptr->X(),self->ptr->Y(),self->ptr->Z(),self->ptr->Px(),self->ptr->Py(),self->ptr->Pz())

//Pythonの新しい型　PyObjectを作る

///////////////////////////////////オブジェクトの宣言////////////
typedef struct
{
	PyObject_HEAD
	///ここにクラスの変数を入れる
	///全てpublic?
	Posed *ptr;
} EPPosed;


/////////////////////////////////////メソッド登録用
///Posedのメソッドの定義


EPPosed* newEPPosed();
EPPosed* newEPPosed(Posed);
EPPosed* newEPPosed(Posed*);

//////////////////////////////////////オブジェクトのタイプ宣言
extern PyTypeObject EPPosedType;
//////////////////////////////////////オブジェクト（モジュールの）初期化関数
PyMODINIT_FUNC initEPPosed(void) ;

PyMODINIT_FUNC initEPPose(void) ;

#endif