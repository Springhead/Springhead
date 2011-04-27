
#include "..\..\..\include\EmbPython\Utility\SprEPMatrix.h"



/**************************************************Matrix3d*************************************/
/////////////////////////////////////メソッド登録用
///Matrix3dのメソッドの定義

static PyObject*
EPMatrix3d_clear(EPMatrix3d* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPMatrix3d_str(EPMatrix3d* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("fffffffff"	,self->ptr->Ex().X(),self->ptr->Ey().X(),self->ptr->Ez().X()
	                    ,self->ptr->Ex().Y(),self->ptr->Ey().Y(),self->ptr->Ez().Y()
	                    ,self->ptr->Ex().Z(),self->ptr->Ey().Z(),self->ptr->Ez().Z()
	                   );
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPMatrix3d_set(EPMatrix3d *self,PyObject* data)
{
	double xx,xy,xz,yx,yy,yz,zx,zy,zz;
	PyArg_ParseTuple(data,"ddddddddd",&xx,&yx,&zx,&xy,&yy,&zy,&xz,&yz,&zz);
	self->ptr->xx = xx;
	self->ptr->xy = xy;
	self->ptr->xz = xz;
	self->ptr->yx = yx;
	self->ptr->yy = yy;
	self->ptr->yz = yz;
	self->ptr->zx = zx;
	self->ptr->zy = zy;
	self->ptr->zz = zz;
	Py_RETURN_NONE;
}

//代入
static PyObject* EPMatrix3d_assign(EPMatrix3d* self, EPMatrix3d* b)
{
	if ( !EPMatrix3d_Check(self)|| !EPMatrix3d_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPMatrix3d_add(EPMatrix3d* v1, EPMatrix3d*v2)
{
	if ( !EPMatrix3d_Check(v1)|| !EPMatrix3d_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPMatrix3d* ret = newEPMatrix3d();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPMatrix3d_iadd(EPMatrix3d* v1, EPMatrix3d*v2)
{
	if ( !EPMatrix3d_Check(v1 )|| !EPMatrix3d_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPMatrix3d_sub(EPMatrix3d* v1, EPMatrix3d*v2)
{
	if ( !EPMatrix3d_Check(v1 )|| !EPMatrix3d_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPMatrix3d* ret = newEPMatrix3d();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPMatrix3d_isub(EPMatrix3d* v1, EPMatrix3d*v2)
{
	if ( !EPMatrix3d_Check(v1 )|| !EPMatrix3d_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//外積
static PyObject* EPMatrix3d_cross(EPMatrix3d* v1, EPMatrix3d*v2)
{
	Return_NotImplementedError("Cross ha mada desu");

	//if ( !EPMatrix3d_Check(v1 )|| !EPMatrix3d_Check(v2)) Return_ArgError;

	//EPMatrix3d* retv = newEPMatrix3d();
	//retv->ptr->assign( v1->ptr->exp() );
	//double y = retv->ptr->dot( *(v2->ptr) );

	//PyObject* ret = Py_BuildValue("d",y);
	//return ret;
	//Py_RETURN_FALSE;
}

//乗算
static PyObject* EPMatrix3d_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPMatrix3d_Check( v1 ))
	{
		w1 = v1;

		if ( EPMatrix3d_Check( v2 ))
			w2 = v2;
		//int
		else if ( PyLong_Check(v2) )
			x = (double) PyLong_AS_LONG(v2);
		//float
		else if ( PyFloat_Check(v2) )
			x = PyFloat_AS_DOUBLE(v2);
		//入力エラー
		else
			Return_ArgError;
	}
	else if ( EPMatrix3d_Check( v2 ))
	{
		w1 = v2;
		//int
		if ( PyLong_Check(v1) )
			x = (double) PyLong_AS_LONG(v1);
		//float
		else if ( PyFloat_Check(v1) )
			x = PyFloat_AS_DOUBLE(v1);
		//入力エラー
		else
			Return_ArgError;
	}
	else Return_ArgError;


	//行列の掛け算の場合
	if ( w2 != NULL )
	{
		Return_NotImplementedError("あgyoretu no kakezan ha mada dame");
		//return EPMatrix3d_dot((EPMatrix3d*)w1,(EPMatrix3d*)w2);
	}
	//スカラー倍
	else
	{
		//w1のコピーを作る
		EPMatrix3d* ret = newEPMatrix3d();
		EPMatrix3d_assign(ret,(EPMatrix3d*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPMatrix3d_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;
	((EPMatrix3d*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPMatrix3d_div(PyObject* v1, PyObject* v2)
{
	//double x;
	//if ( PyLong_Check(v2) )
	//	x = (double) PyLong_AS_LONG(v2);
	//else if ( PyFloat_Check(v2) )
	//	x = PyFloat_AS_DOUBLE(v2);
	//else
	//	Return_ArgError;

	////v1のコピーを作る
	//EPMatrix3d* ret = newEPMatrix3d();
	//ret->ptr->assign(((EPMatrix3d*)v1)->ptr->exp());
	//
	//ret->ptr->div( x );
	//Py_INCREF(ret);
	//return (PyObject*)ret;
	Py_RETURN_FALSE;
}
static PyObject* EPMatrix3d_idiv(PyObject* v1, PyObject* v2)
{
	//double x;
	//if ( PyLong_Check(v2) )
	//	x = (double) PyLong_AS_LONG(v2);
	//else if ( PyFloat_Check(v2) )
	//	x = PyFloat_AS_DOUBLE(v2);
	//else
	//	Return_ArgError;

	//((EPMatrix3d*)v1)->ptr->div( x );
	//Py_INCREF(v1);
	//return (PyObject*)v1;
	Py_RETURN_FALSE;
}



static PyObject* EPMatrix3d_equal(EPMatrix3d* v1, EPMatrix3d* v2)
{
	if ( !EPMatrix3d_Check(v1)|| !EPMatrix3d_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPMatrix3d_square(EPMatrix3d* self)
{
	//double y = self->ptr->square();
	//PyObject* ret = Py_BuildValue("f",y);
	//Py_INCREF(ret);
	//return ret;
	Py_RETURN_FALSE;
}
static PyObject* EPMatrix3d_norm(EPMatrix3d* self)
{
	//double y = self->ptr->norm();
	//PyObject* ret = Py_BuildValue("f",y);
	//Py_INCREF(ret);
	//return ret;
	Py_RETURN_FALSE;
}






///EPMatrix3d_method[]にメソッド情報を格納
static PyMethodDef EPMatrix3d_methods[] =
{
	{"Clear",(PyCFunction)EPMatrix3d_clear,METH_NOARGS,"spr Matrix3d clear"},
	{"Set",(PyCFunction)EPMatrix3d_set,METH_VARARGS,"spr Matrix3d set"},
	{"Add",(PyCFunction)EPMatrix3d_add,METH_O,"spr Matrix3d add"},
	{"Assign",(PyCFunction)EPMatrix3d_assign,METH_O,"spr Matrix3d assign"},
	{"Sub",(PyCFunction)EPMatrix3d_sub,METH_O,"spr Matrix3d sub"},
	{"Multi",(PyCFunction)EPMatrix3d_multi,METH_VARARGS,"spr Matrix3d multi"},
	{"Div",(PyCFunction)EPMatrix3d_div,METH_VARARGS,"spr Matrix3d div"},
	{"Cross",(PyCFunction)EPMatrix3d_cross,METH_O,"spr Matrix3d dot"},
	{"Equal",(PyCFunction)EPMatrix3d_equal,METH_O,"spr Matrix3d equal"},
	{"Square",(PyCFunction)EPMatrix3d_square,METH_NOARGS,"spr Matrix3d square"},
	{"Norm",(PyCFunction)EPMatrix3d_norm,METH_NOARGS,"spr Matrix3d norm"},
	{NULL}
};

static PyNumberMethods EPMatrix3d_math_methods =
{
	(binaryfunc) EPMatrix3d_add,					/* __add__ */
	(binaryfunc) EPMatrix3d_sub,					/* __sub__ */
	(binaryfunc) EPMatrix3d_multi,					/* __mul__ */
	(binaryfunc) NULL,							/* __mod__ */
	(binaryfunc) NULL,							/* __divmod__ */
	(ternaryfunc) NULL,							/* __pow__ */
	(unaryfunc) NULL,							/* __neg__ */
	(unaryfunc) NULL,							/* __pos__ */
	(unaryfunc) NULL,							/* __abs__ */
	(inquiry) NULL,								/* __bool__ */
	(unaryfunc) NULL,							/* __invert__ */
	(binaryfunc) NULL,							/* __lshift__ */
	(binaryfunc) NULL,							/* __rshift__ */
	(binaryfunc) NULL,							/* __and__ */
	(binaryfunc) NULL,							/* __xor__ */
	(binaryfunc) NULL,							/* __or__ */
	(unaryfunc) NULL,							/* __int__ */
	(void *) NULL,		/* __reserved__ */
	(unaryfunc) NULL,							/* __float__ */

	/* Added in release 2.0 */
	(binaryfunc) EPMatrix3d_iadd,					/*__iadd__*/
	(binaryfunc) EPMatrix3d_isub,					/*__isub__*/
	(binaryfunc) EPMatrix3d_imulti,				/*__imul__*/
	(binaryfunc) NULL,							/*__imod__*/
	(ternaryfunc) NULL,							/*__ipow__*/
	(binaryfunc) NULL,							/*__ilshift__*/
	(binaryfunc) NULL,							/*__irshift__*/
	(binaryfunc) NULL,							/*__iand__*/
	(binaryfunc) NULL,							/*__ixor__*/
	(binaryfunc) NULL,							/*__ior__*/

	/* Added in release 2.2 */
	/* The following require the Py_TPFLAGS_HAVE_CLASS flag */
	(binaryfunc) NULL,							/*__floordiv__  __rfloordiv__*/
	(binaryfunc)  EPMatrix3d_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPMatrix3d_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPMatrix3d,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPMatrix3d_init(EPMatrix3d* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Matrix3d(0,0,0,0,0,0,0,0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Matrix3d(0,0,0,0,0,0,0,0,0);
		break;
	case 9:

		double xx,xy,xz,yx,yy,yz,zx,zy,zz;
		PyArg_ParseTuple(args,"ddddddddd",&xx,&yx,&zx,&xy,&yy,&zy,&xz,&yz,&zz);
		self->ptr = new Matrix3d(xx,yx,zx,xy,yy,zy,xz,yz,zz);
		break;
	default:
		printf("arg error");
		return -1;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPMatrix3d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPMatrix3d *self;
	self = ( EPMatrix3d*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPMatrix3d_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPMatrix3d_dealloc(EPMatrix3d* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPMatrix3dType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Matrix3d",             /*tp_name*/
	sizeof(EPMatrix3d),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPMatrix3d_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPMatrix3d_str,                         /*tp_repr*/
	&EPMatrix3d_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPMatrix3d_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Matrix3d",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPMatrix3d_methods,             /* tp_methods */
	0,//EPMatrix3d_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPMatrix3d_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPMatrix3d_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPMatrix3d(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPMatrix3dType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPMatrix3dType);
	PyModule_AddObject(m,"Matrix3d",(PyObject *)&EPMatrix3dType);
	return m;
}

EPMatrix3d* newEPMatrix3d()
{
	return (EPMatrix3d*)EPMatrix3d_new(&EPMatrix3dType,NULL,NULL);
}

EPMatrix3d* newEPMatrix3d(Matrix3d vec)
{
	EPMatrix3d* ret = (EPMatrix3d*)EPMatrix3d_new(&EPMatrix3dType,NULL,NULL);
	ret->ptr->assign(vec);

	return ret;
}

EPMatrix3d* newEPMatrix3d(Matrix3d* vec)
{
	EPMatrix3d* ret = (EPMatrix3d*)EPMatrix3d_new(&EPMatrix3dType,NULL,NULL);
	ret->ptr->assign(*vec);

	return ret;
}
/**************************************************Matrix3f*************************************/
/////////////////////////////////////メソッド登録用
///Matrix3fのメソッドの定義

static PyObject*
EPMatrix3f_clear(EPMatrix3f* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPMatrix3f_str(EPMatrix3f* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("fffffffff"	,self->ptr->Ex().X(),self->ptr->Ey().X(),self->ptr->Ez().X()
	                    ,self->ptr->Ex().Y(),self->ptr->Ey().Y(),self->ptr->Ez().Y()
	                    ,self->ptr->Ex().Z(),self->ptr->Ey().Z(),self->ptr->Ez().Z()
	                   );
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPMatrix3f_set(EPMatrix3f *self,PyObject* data)
{
	float xx,xy,xz,yx,yy,yz,zx,zy,zz;
	PyArg_ParseTuple(data,"fffffffff",&xx,&yx,&zx,&xy,&yy,&zy,&xz,&yz,&zz);
	self->ptr->xx = xx;
	self->ptr->xy = xy;
	self->ptr->xz = xz;
	self->ptr->yx = yx;
	self->ptr->yy = yy;
	self->ptr->yz = yz;
	self->ptr->zx = zx;
	self->ptr->zy = zy;
	self->ptr->zz = zz;
	Py_RETURN_NONE;
}

//代入
static PyObject* EPMatrix3f_assign(EPMatrix3f* self, EPMatrix3f* b)
{
	if ( !EPMatrix3f_Check(self)|| !EPMatrix3f_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPMatrix3f_add(EPMatrix3f* v1, EPMatrix3f*v2)
{
	if ( !EPMatrix3f_Check(v1)|| !EPMatrix3f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPMatrix3f* ret = newEPMatrix3f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPMatrix3f_iadd(EPMatrix3f* v1, EPMatrix3f*v2)
{
	if ( !EPMatrix3f_Check(v1 )|| !EPMatrix3f_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPMatrix3f_sub(EPMatrix3f* v1, EPMatrix3f*v2)
{
	if ( !EPMatrix3f_Check(v1 )|| !EPMatrix3f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPMatrix3f* ret = newEPMatrix3f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPMatrix3f_isub(EPMatrix3f* v1, EPMatrix3f*v2)
{
	if ( !EPMatrix3f_Check(v1 )|| !EPMatrix3f_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//外積
static PyObject* EPMatrix3f_cross(EPMatrix3f* v1, EPMatrix3f*v2)
{
	Return_NotImplementedError("Cross ha mada desu");

	//if ( !EPMatrix3f_Check(v1 )|| !EPMatrix3f_Check(v2)) Return_ArgError;

	//EPMatrix3f* retv = newEPMatrix3f();
	//retv->ptr->assign( v1->ptr->exp() );
	//double y = retv->ptr->dot( *(v2->ptr) );

	//PyObject* ret = Py_BuildValue("d",y);
	//return ret;
	//Py_RETURN_FALSE;
}

//乗算
static PyObject* EPMatrix3f_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPMatrix3f_Check( v1 ))
	{
		w1 = v1;

		if ( EPMatrix3f_Check( v2 ))
			w2 = v2;
		//int
		else if ( PyLong_Check(v2) )
			x = (double) PyLong_AS_LONG(v2);
		//float
		else if ( PyFloat_Check(v2) )
			x = PyFloat_AS_DOUBLE(v2);
		//入力エラー
		else
			Return_ArgError;
	}
	else if ( EPMatrix3f_Check( v2 ))
	{
		w1 = v2;
		//int
		if ( PyLong_Check(v1) )
			x = (double) PyLong_AS_LONG(v1);
		//float
		else if ( PyFloat_Check(v1) )
			x = PyFloat_AS_DOUBLE(v1);
		//入力エラー
		else
			Return_ArgError;
	}
	else Return_ArgError;


	//行列の掛け算の場合
	if ( w2 != NULL )
	{
		Return_NotImplementedError("あgyoretu no kakezan ha mada dame");
		//return EPMatrix3f_dot((EPMatrix3f*)w1,(EPMatrix3f*)w2);
	}
	//スカラー倍
	else
	{
		//w1のコピーを作る
		EPMatrix3f* ret = newEPMatrix3f();
		EPMatrix3f_assign(ret,(EPMatrix3f*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPMatrix3f_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;
	((EPMatrix3f*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPMatrix3f_div(PyObject* v1, PyObject* v2)
{
	//double x;
	//if ( PyLong_Check(v2) )
	//	x = (double) PyLong_AS_LONG(v2);
	//else if ( PyFloat_Check(v2) )
	//	x = PyFloat_AS_DOUBLE(v2);
	//else
	//	Return_ArgError;

	////v1のコピーを作る
	//EPMatrix3f* ret = newEPMatrix3f();
	//ret->ptr->assign(((EPMatrix3f*)v1)->ptr->exp());
	//
	//ret->ptr->div( x );
	//Py_INCREF(ret);
	//return (PyObject*)ret;
	Py_RETURN_FALSE;
}
static PyObject* EPMatrix3f_idiv(PyObject* v1, PyObject* v2)
{
	//double x;
	//if ( PyLong_Check(v2) )
	//	x = (double) PyLong_AS_LONG(v2);
	//else if ( PyFloat_Check(v2) )
	//	x = PyFloat_AS_DOUBLE(v2);
	//else
	//	Return_ArgError;

	//((EPMatrix3f*)v1)->ptr->div( x );
	//Py_INCREF(v1);
	//return (PyObject*)v1;
	Py_RETURN_FALSE;
}



static PyObject* EPMatrix3f_equal(EPMatrix3f* v1, EPMatrix3f* v2)
{
	if ( !EPMatrix3f_Check(v1)|| !EPMatrix3f_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPMatrix3f_square(EPMatrix3f* self)
{
	//double y = self->ptr->square();
	//PyObject* ret = Py_BuildValue("f",y);
	//Py_INCREF(ret);
	//return ret;
	Py_RETURN_FALSE;
}
static PyObject* EPMatrix3f_norm(EPMatrix3f* self)
{
	//double y = self->ptr->norm();
	//PyObject* ret = Py_BuildValue("f",y);
	//Py_INCREF(ret);
	//return ret;
	Py_RETURN_FALSE;
}






///EPMatrix3f_method[]にメソッド情報を格納
static PyMethodDef EPMatrix3f_methods[] =
{
	{"Clear",(PyCFunction)EPMatrix3f_clear,METH_NOARGS,"spr Matrix3f clear"},
	{"Set",(PyCFunction)EPMatrix3f_set,METH_VARARGS,"spr Matrix3f set"},
	{"Add",(PyCFunction)EPMatrix3f_add,METH_O,"spr Matrix3f add"},
	{"Assign",(PyCFunction)EPMatrix3f_assign,METH_O,"spr Matrix3f assign"},
	{"Sub",(PyCFunction)EPMatrix3f_sub,METH_O,"spr Matrix3f sub"},
	{"Multi",(PyCFunction)EPMatrix3f_multi,METH_VARARGS,"spr Matrix3f multi"},
	{"Div",(PyCFunction)EPMatrix3f_div,METH_VARARGS,"spr Matrix3f div"},
	{"Cross",(PyCFunction)EPMatrix3f_cross,METH_O,"spr Matrix3f dot"},
	{"Equal",(PyCFunction)EPMatrix3f_equal,METH_O,"spr Matrix3f equal"},
	{"Square",(PyCFunction)EPMatrix3f_square,METH_NOARGS,"spr Matrix3f square"},
	{"Norm",(PyCFunction)EPMatrix3f_norm,METH_NOARGS,"spr Matrix3f norm"},
	{NULL}
};

static PyNumberMethods EPMatrix3f_math_methods =
{
	(binaryfunc) EPMatrix3f_add,					/* __add__ */
	(binaryfunc) EPMatrix3f_sub,					/* __sub__ */
	(binaryfunc) EPMatrix3f_multi,					/* __mul__ */
	(binaryfunc) NULL,							/* __mod__ */
	(binaryfunc) NULL,							/* __divmod__ */
	(ternaryfunc) NULL,							/* __pow__ */
	(unaryfunc) NULL,							/* __neg__ */
	(unaryfunc) NULL,							/* __pos__ */
	(unaryfunc) NULL,							/* __abs__ */
	(inquiry) NULL,								/* __bool__ */
	(unaryfunc) NULL,							/* __invert__ */
	(binaryfunc) NULL,							/* __lshift__ */
	(binaryfunc) NULL,							/* __rshift__ */
	(binaryfunc) NULL,							/* __and__ */
	(binaryfunc) NULL,							/* __xor__ */
	(binaryfunc) NULL,							/* __or__ */
	(unaryfunc) NULL,							/* __int__ */
	(void *) NULL,		/* __reserved__ */
	(unaryfunc) NULL,							/* __float__ */

	/* Added in release 2.0 */
	(binaryfunc) EPMatrix3f_iadd,					/*__iadd__*/
	(binaryfunc) EPMatrix3f_isub,					/*__isub__*/
	(binaryfunc) EPMatrix3f_imulti,				/*__imul__*/
	(binaryfunc) NULL,							/*__imod__*/
	(ternaryfunc) NULL,							/*__ipow__*/
	(binaryfunc) NULL,							/*__ilshift__*/
	(binaryfunc) NULL,							/*__irshift__*/
	(binaryfunc) NULL,							/*__iand__*/
	(binaryfunc) NULL,							/*__ixor__*/
	(binaryfunc) NULL,							/*__ior__*/

	/* Added in release 2.2 */
	/* The following require the Py_TPFLAGS_HAVE_CLASS flag */
	(binaryfunc) NULL,							/*__floordiv__  __rfloordiv__*/
	(binaryfunc)  EPMatrix3f_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPMatrix3f_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPMatrix3f,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPMatrix3f_init(EPMatrix3f* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Matrix3f(0,0,0,0,0,0,0,0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Matrix3f(0,0,0,0,0,0,0,0,0);
		break;
	case 9:

		float xx,xy,xz,yx,yy,yz,zx,zy,zz;
		PyArg_ParseTuple(args,"fffffffff",&xx,&yx,&zx,&xy,&yy,&zy,&xz,&yz,&zz);
		self->ptr = new Matrix3f(xx,yx,zx,xy,yy,zy,xz,yz,zz);
		break;
	default:
		printf("arg error");
		return -1;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPMatrix3f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPMatrix3f *self;
	self = ( EPMatrix3f*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPMatrix3f_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPMatrix3f_dealloc(EPMatrix3f* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPMatrix3fType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Matrix3f",             /*tp_name*/
	sizeof(EPMatrix3f),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPMatrix3f_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPMatrix3f_str,                         /*tp_repr*/
	&EPMatrix3f_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPMatrix3f_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Matrix3f",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPMatrix3f_methods,             /* tp_methods */
	0,//EPMatrix3f_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPMatrix3f_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPMatrix3f_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPMatrix3f(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPMatrix3fType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPMatrix3fType);
	PyModule_AddObject(m,"Matrix3f",(PyObject *)&EPMatrix3fType);
	return m;
}

EPMatrix3f* newEPMatrix3f()
{
	return (EPMatrix3f*)EPMatrix3f_new(&EPMatrix3fType,NULL,NULL);
}

EPMatrix3f* newEPMatrix3f(Matrix3f vec)
{
	EPMatrix3f* ret = (EPMatrix3f*)EPMatrix3f_new(&EPMatrix3fType,NULL,NULL);
	ret->ptr->assign(vec);

	return ret;
}

EPMatrix3f* newEPMatrix3f(Matrix3f* vec)
{
	EPMatrix3f* ret = (EPMatrix3f*)EPMatrix3f_new(&EPMatrix3fType,NULL,NULL);
	ret->ptr->assign(*vec);

	return ret;
}


PyMODINIT_FUNC initEPMatrix(void)
{
	initEPMatrix3d();
	initEPMatrix3f();
	return NULL;
}
