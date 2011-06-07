
#include "..\..\..\include\EmbPython\Utility\SprEPVector.h"



/*******************************************Vec3d*******************************************/
//Pythonの新しい型　PyObjectを作る

/////////////////////////////////////メソッド登録用
///Vec3dのメソッドの定義

static PyObject*
EPVec3d_clear(EPVec3d* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPVec3d_str(EPVec3d* self)
{
	//int _NUM =3;

	//for (int i = 0 ; i < _NUM ; i++)
	//{
	//	 self->ptr->data[i]
	//	 ret = PyObject_Str(PyFloat);
	//}
	
	
	//for (int i = 0 ; i < _NUM ; i++)
	//{
	//	PyObject* PyTuple_GetItem(	PyObject *p, Py_ssize_t pos)
	//	self->ptr->data
	//}


	PyObject* val, *ret;
	val = Py_BuildValue("fff",self->ptr->X(),self->ptr->Y(),self->ptr->Z());
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPVec3d_set(EPVec3d *self,PyObject* data)
{
	double dx,dy,dz;
	PyArg_ParseTuple(data,"ddd",&dx,&dy,&dz);
	self->ptr->x = dx;
	self->ptr->y = dy;
	self->ptr->z = dz;
	Py_RETURN_NONE;
}


static PyObject* EPVec3d_assign(EPVec3d* self, EPVec3d* b)
{
	if ( !EPVec3d_Check(self)|| !EPVec3d_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPVec3d_add(EPVec3d* v1, EPVec3d*v2)
{
	if ( !EPVec3d_Check(v1)|| !EPVec3d_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec3d* ret = newEPVec3d();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPVec3d_iadd(EPVec3d* v1, EPVec3d*v2)
{
	if ( !EPVec3d_Check(v1 )|| !EPVec3d_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPVec3d_sub(EPVec3d* v1, EPVec3d*v2)
{
	if ( !EPVec3d_Check(v1 )|| !EPVec3d_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec3d* ret = newEPVec3d();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec3d_isub(EPVec3d* v1, EPVec3d*v2)
{
	if ( !EPVec3d_Check(v1 )|| !EPVec3d_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPVec3d_dot(EPVec3d* v1, EPVec3d*v2)
{
	if ( !EPVec3d_Check(v1 )|| !EPVec3d_Check(v2)) Return_ArgError;

	EPVec3d* retv = newEPVec3d();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPVec3d_idot(EPVec3d* v1, EPVec3d*v2)
{
	if ( !EPVec3d_Check(v1 )|| !EPVec3d_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPVec3d_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPVec3d_Check( v1 ))
	{
		w1 = v1;

		if ( EPVec3d_Check( v2 ))
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
	else if ( EPVec3d_Check( v2 ))
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


	//内積の場合
	if ( w2 != NULL )
	{
		return EPVec3d_dot((EPVec3d*)w1,(EPVec3d*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPVec3d* ret = newEPVec3d();
		EPVec3d_assign(ret,(EPVec3d*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPVec3d_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec3d*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPVec3d_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPVec3d* ret = newEPVec3d();
	ret->ptr->assign(((EPVec3d*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec3d_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec3d*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPVec3d_equal(EPVec3d* v1, EPVec3d* v2)
{
	if ( !EPVec3d_Check(v1)|| !EPVec3d_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPVec3d_square(EPVec3d* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPVec3d_norm(EPVec3d* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}






///EPVec3d_method[]にメソッド情報を格納
static PyMethodDef EPVec3d_methods[] =
{
	{"Clear",(PyCFunction)EPVec3d_clear,METH_NOARGS,"spr vec3d clear"},
	{"Set",(PyCFunction)EPVec3d_set,METH_VARARGS,"spr vec3d set"},
	{"Add",(PyCFunction)EPVec3d_add,METH_O,"spr vec3d add"},
	{"Assign",(PyCFunction)EPVec3d_assign,METH_O,"spr vec3d assign"},
	{"Sub",(PyCFunction)EPVec3d_sub,METH_O,"spr vec3d sub"},
	{"Multi",(PyCFunction)EPVec3d_multi,METH_VARARGS,"spr vec3d multi"},
	{"Div",(PyCFunction)EPVec3d_div,METH_VARARGS,"spr vec3d div"},
	{"Dot",(PyCFunction)EPVec3d_dot,METH_O,"spr vec3d dot"},
	{"Equal",(PyCFunction)EPVec3d_equal,METH_O,"spr vec3d equal"},
	{"Square",(PyCFunction)EPVec3d_square,METH_NOARGS,"spr vec3d square"},
	{"Norm",(PyCFunction)EPVec3d_norm,METH_NOARGS,"spr vec3d norm"},
	{NULL}
};

static PyNumberMethods EPVec3d_math_methods =
{
	(binaryfunc) EPVec3d_add,					/* __add__ */
	(binaryfunc) EPVec3d_sub,					/* __sub__ */
	(binaryfunc) EPVec3d_multi,					/* __mul__ */
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
	(binaryfunc) EPVec3d_iadd,					/*__iadd__*/
	(binaryfunc) EPVec3d_isub,					/*__isub__*/
	(binaryfunc) EPVec3d_imulti,				/*__imul__*/
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
	(binaryfunc)  EPVec3d_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPVec3d_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPVec3d,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPVec3d_init(EPVec3d* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Vec3d(0,0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Vec3d(0,0,0);
		break;
	case 3:
		double x,y,z;
		PyArg_ParseTuple(args,"ddd",&x,&y,&z);
		self->ptr = new Vec3d(x,y,z);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPVec3d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec3d *self;
	self = ( EPVec3d*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPVec3d_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPVec3d_dealloc(EPVec3d* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPVec3dType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Vec3d",             /*tp_name*/
	sizeof(EPVec3d),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPVec3d_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPVec3d_str,                         /*tp_repr*/
	&EPVec3d_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPVec3d_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Vec3d",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPVec3d_methods,             /* tp_methods */
	0,//EPVec3d_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPVec3d_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPVec3d_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPVec3d(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPVec3dType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPVec3dType);
	PyModule_AddObject(m,"Vec3d",(PyObject *)&EPVec3dType);
	return m;
}

EPVec3d* newEPVec3d()
{
	return (EPVec3d*)EPVec3d_new(&EPVec3dType,NULL,NULL);
}

EPVec3d* newEPVec3d(Vec3d vec)
{
	EPVec3d* ret = (EPVec3d*)EPVec3d_new(&EPVec3dType,NULL,NULL);
	ret->ptr->assign(vec);
	return ret;
}

EPVec3d* newEPVec3d(Vec3d* vec)
{
	EPVec3d* ret = (EPVec3d*)EPVec3d_new(&EPVec3dType,NULL,NULL);
	ret->ptr = vec;
	return ret;
}



/*******************************************Vec2d*******************************************/

//Pythonの新しい型　PyObjectを作る

/////////////////////////////////////メソッド登録用
///Vec2dのメソッドの定義

static PyObject*
EPVec2d_clear(EPVec2d* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPVec2d_str(EPVec2d* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("ff",self->ptr->X(),self->ptr->Y());
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPVec2d_set(EPVec2d *self,PyObject* data)
{
	double dx,dy,dz;
	PyArg_ParseTuple(data,"dd",&dx,&dy);

	self->ptr->x = dx;
	self->ptr->y = dy;
	Py_RETURN_NONE;
}


static PyObject* EPVec2d_assign(EPVec2d* self, EPVec2d* b)
{
	if ( !EPVec2d_Check(self)|| !EPVec2d_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPVec2d_add(EPVec2d* v1, EPVec2d*v2)
{
	if ( !EPVec2d_Check(v1)|| !EPVec2d_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec2d* ret = newEPVec2d();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPVec2d_iadd(EPVec2d* v1, EPVec2d*v2)
{
	if ( !EPVec2d_Check(v1 )|| !EPVec2d_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPVec2d_sub(EPVec2d* v1, EPVec2d*v2)
{
	if ( !EPVec2d_Check(v1 )|| !EPVec2d_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec2d* ret = newEPVec2d();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec2d_isub(EPVec2d* v1, EPVec2d*v2)
{
	if ( !EPVec2d_Check(v1 )|| !EPVec2d_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPVec2d_dot(EPVec2d* v1, EPVec2d*v2)
{
	if ( !EPVec2d_Check(v1 )|| !EPVec2d_Check(v2)) Return_ArgError;

	EPVec2d* retv = newEPVec2d();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPVec2d_idot(EPVec2d* v1, EPVec2d*v2)
{
	if ( !EPVec2d_Check(v1 )|| !EPVec2d_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPVec2d_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPVec2d_Check( v1 ))
	{
		w1 = v1;

		if ( EPVec2d_Check( v2 ))
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
	else if ( EPVec2d_Check( v2 ))
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


	//内積の場合
	if ( w2 != NULL )
	{
		return EPVec2d_dot((EPVec2d*)w1,(EPVec2d*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPVec2d* ret = newEPVec2d();
		EPVec2d_assign(ret,(EPVec2d*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPVec2d_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec2d*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPVec2d_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPVec2d* ret = newEPVec2d();
	ret->ptr->assign(((EPVec2d*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec2d_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec2d*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPVec2d_equal(EPVec2d* v1, EPVec2d* v2)
{
	if ( !EPVec2d_Check(v1)|| !EPVec2d_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPVec2d_square(EPVec2d* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPVec2d_norm(EPVec2d* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}






///EPVec2d_method[]にメソッド情報を格納
static PyMethodDef EPVec2d_methods[] =
{
	{"Clear",(PyCFunction)EPVec2d_clear,METH_NOARGS,"spr Vec2d clear"},
	{"Set",(PyCFunction)EPVec2d_set,METH_VARARGS,"spr Vec2d set"},
	{"Add",(PyCFunction)EPVec2d_add,METH_O,"spr Vec2d add"},
	{"Assign",(PyCFunction)EPVec2d_assign,METH_O,"spr Vec2d assign"},
	{"Sub",(PyCFunction)EPVec2d_sub,METH_O,"spr Vec2d sub"},
	{"Multi",(PyCFunction)EPVec2d_multi,METH_VARARGS,"spr Vec2d multi"},
	{"Div",(PyCFunction)EPVec2d_div,METH_VARARGS,"spr Vec2d div"},
	{"Dot",(PyCFunction)EPVec2d_dot,METH_O,"spr Vec2d dot"},
	{"Equal",(PyCFunction)EPVec2d_equal,METH_O,"spr Vec2d equal"},
	{"Square",(PyCFunction)EPVec2d_square,METH_NOARGS,"spr Vec2d square"},
	{"Norm",(PyCFunction)EPVec2d_norm,METH_NOARGS,"spr Vec2d norm"},
	{NULL}
};

static PyNumberMethods EPVec2d_math_methods =
{
	(binaryfunc) EPVec2d_add,					/* __add__ */
	(binaryfunc) EPVec2d_sub,					/* __sub__ */
	(binaryfunc) EPVec2d_multi,					/* __mul__ */
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
	(binaryfunc) EPVec2d_iadd,					/*__iadd__*/
	(binaryfunc) EPVec2d_isub,					/*__isub__*/
	(binaryfunc) EPVec2d_imulti,				/*__imul__*/
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
	(binaryfunc)  EPVec2d_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPVec2d_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPVec2d,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPVec2d_init(EPVec2d* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Vec2d(0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Vec2d(0,0);
		break;
	case 2:
		double x,y,z;
		PyArg_ParseTuple(args,"dd",&x,&y);
		self->ptr = new Vec2d(x,y);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPVec2d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec2d *self;
	self = ( EPVec2d*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPVec2d_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPVec2d_dealloc(EPVec2d* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPVec2dType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Vec2d",             /*tp_name*/
	sizeof(EPVec2d),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPVec2d_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPVec2d_str,                         /*tp_repr*/
	&EPVec2d_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPVec2d_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Vec2d",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPVec2d_methods,             /* tp_methods */
	0,//EPVec2d_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPVec2d_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPVec2d_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPVec2d(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPVec2dType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPVec2dType);
	PyModule_AddObject(m,"Vec2d",(PyObject *)&EPVec2dType);
	return m;
}

EPVec2d* newEPVec2d()
{
	return (EPVec2d*)EPVec2d_new(&EPVec2dType,NULL,NULL);
}

EPVec2d* newEPVec2d(Vec2d vec)
{
	EPVec2d* ret = (EPVec2d*)EPVec2d_new(&EPVec2dType,NULL,NULL);
	ret->ptr->assign(vec);
	return ret;
}

EPVec2d* newEPVec2d(Vec2d* vec)
{
	EPVec2d* ret = (EPVec2d*)EPVec2d_new(&EPVec2dType,NULL,NULL);
	ret->ptr = vec;
	return ret;
}

/*******************************************Vec3f*******************************************/
//Pythonの新しい型　PyObjectを作る

/////////////////////////////////////メソッド登録用
///Vec3fのメソッドの定義

static PyObject*
EPVec3f_clear(EPVec3f* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPVec3f_str(EPVec3f* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("fff",self->ptr->X(),self->ptr->Y(),self->ptr->Z());
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPVec3f_set(EPVec3f *self,PyObject* data)
{
	double dx,dy,dz;
	PyArg_ParseTuple(data,"ddd",&dx,&dy,&dz);

	self->ptr->x = dx;
	self->ptr->y = dy;
	self->ptr->z = dz;
	Py_RETURN_NONE;
}


static PyObject* EPVec3f_assign(EPVec3f* self, EPVec3f* b)
{
	if ( !EPVec3f_Check(self)|| !EPVec3f_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPVec3f_add(EPVec3f* v1, EPVec3f*v2)
{
	if ( !EPVec3f_Check(v1)|| !EPVec3f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec3f* ret = newEPVec3f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPVec3f_iadd(EPVec3f* v1, EPVec3f*v2)
{
	if ( !EPVec3f_Check(v1 )|| !EPVec3f_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPVec3f_sub(EPVec3f* v1, EPVec3f*v2)
{
	if ( !EPVec3f_Check(v1 )|| !EPVec3f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec3f* ret = newEPVec3f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec3f_isub(EPVec3f* v1, EPVec3f*v2)
{
	if ( !EPVec3f_Check(v1 )|| !EPVec3f_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPVec3f_dot(EPVec3f* v1, EPVec3f*v2)
{
	if ( !EPVec3f_Check(v1 )|| !EPVec3f_Check(v2)) Return_ArgError;

	EPVec3f* retv = newEPVec3f();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPVec3f_idot(EPVec3f* v1, EPVec3f*v2)
{
	if ( !EPVec3f_Check(v1 )|| !EPVec3f_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPVec3f_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPVec3f_Check( v1 ))
	{
		w1 = v1;

		if ( EPVec3f_Check( v2 ))
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
	else if ( EPVec3f_Check( v2 ))
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


	//内積の場合
	if ( w2 != NULL )
	{
		return EPVec3f_dot((EPVec3f*)w1,(EPVec3f*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPVec3f* ret = newEPVec3f();
		EPVec3f_assign(ret,(EPVec3f*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPVec3f_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec3f*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPVec3f_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPVec3f* ret = newEPVec3f();
	ret->ptr->assign(((EPVec3f*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec3f_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec3f*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPVec3f_equal(EPVec3f* v1, EPVec3f* v2)
{
	if ( !EPVec3f_Check(v1)|| !EPVec3f_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPVec3f_square(EPVec3f* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPVec3f_norm(EPVec3f* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}






///EPVec3f_method[]にメソッド情報を格納
static PyMethodDef EPVec3f_methods[] =
{
	{"Clear",(PyCFunction)EPVec3f_clear,METH_NOARGS,"spr Vec3f clear"},
	{"Set",(PyCFunction)EPVec3f_set,METH_VARARGS,"spr Vec3f set"},
	{"Add",(PyCFunction)EPVec3f_add,METH_O,"spr Vec3f add"},
	{"Assign",(PyCFunction)EPVec3f_assign,METH_O,"spr Vec3f assign"},
	{"Sub",(PyCFunction)EPVec3f_sub,METH_O,"spr Vec3f sub"},
	{"Multi",(PyCFunction)EPVec3f_multi,METH_VARARGS,"spr Vec3f multi"},
	{"Div",(PyCFunction)EPVec3f_div,METH_VARARGS,"spr Vec3f div"},
	{"Dot",(PyCFunction)EPVec3f_dot,METH_O,"spr Vec3f dot"},
	{"Equal",(PyCFunction)EPVec3f_equal,METH_O,"spr Vec3f equal"},
	{"Square",(PyCFunction)EPVec3f_square,METH_NOARGS,"spr Vec3f square"},
	{"Norm",(PyCFunction)EPVec3f_norm,METH_NOARGS,"spr Vec3f norm"},
	{NULL}
};

static PyNumberMethods EPVec3f_math_methods =
{
	(binaryfunc) EPVec3f_add,					/* __add__ */
	(binaryfunc) EPVec3f_sub,					/* __sub__ */
	(binaryfunc) EPVec3f_multi,					/* __mul__ */
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
	(binaryfunc) EPVec3f_iadd,					/*__iadd__*/
	(binaryfunc) EPVec3f_isub,					/*__isub__*/
	(binaryfunc) EPVec3f_imulti,				/*__imul__*/
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
	(binaryfunc)  EPVec3f_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPVec3f_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPVec3f,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPVec3f_init(EPVec3f* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Vec3f(0,0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Vec3f(0,0,0);
		break;
	case 3:
		double x,y,z;
		PyArg_ParseTuple(args,"ddd",&x,&y,&z);
		self->ptr = new Vec3f(x,y,z);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPVec3f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec3f *self;
	self = ( EPVec3f*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPVec3f_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPVec3f_dealloc(EPVec3f* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPVec3fType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Vec3f",             /*tp_name*/
	sizeof(EPVec3f),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPVec3f_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPVec3f_str,                         /*tp_repr*/
	&EPVec3f_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPVec3f_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Vec3f",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPVec3f_methods,             /* tp_methods */
	0,//EPVec3f_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPVec3f_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPVec3f_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPVec3f(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPVec3fType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPVec3fType);
	PyModule_AddObject(m,"Vec3f",(PyObject *)&EPVec3fType);
	return m;
}

EPVec3f* newEPVec3f()
{
	return (EPVec3f*)EPVec3f_new(&EPVec3fType,NULL,NULL);
}

EPVec3f* newEPVec3f(Vec3f vec)
{
	EPVec3f* ret = (EPVec3f*)EPVec3f_new(&EPVec3fType,NULL,NULL);
	ret->ptr->assign(vec);
	return ret;
}
EPVec3f* newEPVec3f(Vec3f* vec)
{
	EPVec3f* ret = (EPVec3f*)EPVec3f_new(&EPVec3fType,NULL,NULL);
	ret->ptr = vec;
	return ret;
}


/*******************************************Vec2f*******************************************/

//Pythonの新しい型　PyObjectを作る

/////////////////////////////////////メソッド登録用
///Vec2fのメソッドの定義

static PyObject*
EPVec2f_clear(EPVec2f* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPVec2f_str(EPVec2f* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("ff",self->ptr->X(),self->ptr->Y());
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPVec2f_set(EPVec2f *self,PyObject* data)
{
	double dx,dy;
	PyArg_ParseTuple(data,"ddd",&dx,&dy);

	self->ptr->x = dx;
	self->ptr->y = dy;
	Py_RETURN_NONE;
}


static PyObject* EPVec2f_assign(EPVec2f* self, EPVec2f* b)
{
	if ( !EPVec2f_Check(self)|| !EPVec2f_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPVec2f_add(EPVec2f* v1, EPVec2f*v2)
{
	if ( !EPVec2f_Check(v1)|| !EPVec2f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec2f* ret = newEPVec2f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPVec2f_iadd(EPVec2f* v1, EPVec2f*v2)
{
	if ( !EPVec2f_Check(v1 )|| !EPVec2f_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPVec2f_sub(EPVec2f* v1, EPVec2f*v2)
{
	if ( !EPVec2f_Check(v1 )|| !EPVec2f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec2f* ret = newEPVec2f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec2f_isub(EPVec2f* v1, EPVec2f*v2)
{
	if ( !EPVec2f_Check(v1 )|| !EPVec2f_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPVec2f_dot(EPVec2f* v1, EPVec2f*v2)
{
	if ( !EPVec2f_Check(v1 )|| !EPVec2f_Check(v2)) Return_ArgError;

	EPVec2f* retv = newEPVec2f();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPVec2f_idot(EPVec2f* v1, EPVec2f*v2)
{
	if ( !EPVec2f_Check(v1 )|| !EPVec2f_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPVec2f_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPVec2f_Check( v1 ))
	{
		w1 = v1;

		if ( EPVec2f_Check( v2 ))
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
	else if ( EPVec2f_Check( v2 ))
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


	//内積の場合
	if ( w2 != NULL )
	{
		return EPVec2f_dot((EPVec2f*)w1,(EPVec2f*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPVec2f* ret = newEPVec2f();
		EPVec2f_assign(ret,(EPVec2f*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPVec2f_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec2f*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPVec2f_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPVec2f* ret = newEPVec2f();
	ret->ptr->assign(((EPVec2f*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec2f_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec2f*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPVec2f_equal(EPVec2f* v1, EPVec2f* v2)
{
	if ( !EPVec2f_Check(v1)|| !EPVec2f_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPVec2f_square(EPVec2f* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPVec2f_norm(EPVec2f* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}






///EPVec2f_method[]にメソッド情報を格納
static PyMethodDef EPVec2f_methods[] =
{
	{"Clear",(PyCFunction)EPVec2f_clear,METH_NOARGS,"spr Vec2f clear"},
	{"Set",(PyCFunction)EPVec2f_set,METH_VARARGS,"spr Vec2f set"},
	{"Add",(PyCFunction)EPVec2f_add,METH_O,"spr Vec2f add"},
	{"Assign",(PyCFunction)EPVec2f_assign,METH_O,"spr Vec2f assign"},
	{"Sub",(PyCFunction)EPVec2f_sub,METH_O,"spr Vec2f sub"},
	{"Multi",(PyCFunction)EPVec2f_multi,METH_VARARGS,"spr Vec2f multi"},
	{"Div",(PyCFunction)EPVec2f_div,METH_VARARGS,"spr Vec2f div"},
	{"Dot",(PyCFunction)EPVec2f_dot,METH_O,"spr Vec2f dot"},
	{"Equal",(PyCFunction)EPVec2f_equal,METH_O,"spr Vec2f equal"},
	{"Square",(PyCFunction)EPVec2f_square,METH_NOARGS,"spr Vec2f square"},
	{"Norm",(PyCFunction)EPVec2f_norm,METH_NOARGS,"spr Vec2f norm"},
	{NULL}
};

static PyNumberMethods EPVec2f_math_methods =
{
	(binaryfunc) EPVec2f_add,					/* __add__ */
	(binaryfunc) EPVec2f_sub,					/* __sub__ */
	(binaryfunc) EPVec2f_multi,					/* __mul__ */
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
	(binaryfunc) EPVec2f_iadd,					/*__iadd__*/
	(binaryfunc) EPVec2f_isub,					/*__isub__*/
	(binaryfunc) EPVec2f_imulti,				/*__imul__*/
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
	(binaryfunc)  EPVec2f_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPVec2f_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPVec2f,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPVec2f_init(EPVec2f* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Vec2f(0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Vec2f(0,0);
		break;
	case 2:
		double x,y,z;
		PyArg_ParseTuple(args,"dd",&x,&y);
		self->ptr = new Vec2f(x,y);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPVec2f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec2f *self;
	self = ( EPVec2f*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPVec2f_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPVec2f_dealloc(EPVec2f* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPVec2fType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Vec2f",             /*tp_name*/
	sizeof(EPVec2f),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPVec2f_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPVec2f_str,                         /*tp_repr*/
	&EPVec2f_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPVec2f_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Vec2f",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPVec2f_methods,             /* tp_methods */
	0,//EPVec2f_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPVec2f_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPVec2f_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPVec2f(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPVec2fType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPVec2fType);
	PyModule_AddObject(m,"Vec2f",(PyObject *)&EPVec2fType);
	return m;
}

EPVec2f* newEPVec2f()
{
	return (EPVec2f*)EPVec2f_new(&EPVec2fType,NULL,NULL);
}

EPVec2f* newEPVec2f(Vec2f vec)
{
	EPVec2f* ret = (EPVec2f*)EPVec2f_new(&EPVec2fType,NULL,NULL);
	ret->ptr->assign(vec);
	return ret;
}

EPVec2f* newEPVec2f(Vec2f* vec)
{
	EPVec2f* ret = (EPVec2f*)EPVec2f_new(&EPVec2fType,NULL,NULL);
	ret->ptr = vec;
	return ret;
}


/*******************************************Vec4f*******************************************/
//Pythonの新しい型　PyObjectを作る

/////////////////////////////////////メソッド登録用
///Vec4fのメソッドの定義

static PyObject*
EPVec4f_clear(EPVec4f* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPVec4f_str(EPVec4f* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("ffff",self->ptr->X(),self->ptr->Y(),self->ptr->Z(),self->ptr->W());
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPVec4f_set(EPVec4f *self,PyObject* data)
{
	double dx,dy,dz,dw;
	PyArg_ParseTuple(data,"ffff",&dx,&dy,&dz,&dw);

	self->ptr->x = dx;
	self->ptr->y = dy;
	self->ptr->z = dz;
	self->ptr->w = dw;
	Py_RETURN_NONE;
}


static PyObject* EPVec4f_assign(EPVec4f* self, EPVec4f* b)
{
	if ( !EPVec4f_Check(self)|| !EPVec4f_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPVec4f_add(EPVec4f* v1, EPVec4f*v2)
{
	if ( !EPVec4f_Check(v1)|| !EPVec4f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec4f* ret = newEPVec4f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPVec4f_iadd(EPVec4f* v1, EPVec4f*v2)
{
	if ( !EPVec4f_Check(v1 )|| !EPVec4f_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPVec4f_sub(EPVec4f* v1, EPVec4f*v2)
{
	if ( !EPVec4f_Check(v1 )|| !EPVec4f_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPVec4f* ret = newEPVec4f();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec4f_isub(EPVec4f* v1, EPVec4f*v2)
{
	if ( !EPVec4f_Check(v1 )|| !EPVec4f_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPVec4f_dot(EPVec4f* v1, EPVec4f*v2)
{
	if ( !EPVec4f_Check(v1 )|| !EPVec4f_Check(v2)) Return_ArgError;

	EPVec4f* retv = newEPVec4f();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPVec4f_idot(EPVec4f* v1, EPVec4f*v2)
{
	if ( !EPVec4f_Check(v1 )|| !EPVec4f_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPVec4f_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPVec4f_Check( v1 ))
	{
		w1 = v1;

		if ( EPVec4f_Check( v2 ))
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
	else if ( EPVec4f_Check( v2 ))
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


	//内積の場合
	if ( w2 != NULL )
	{
		return EPVec4f_dot((EPVec4f*)w1,(EPVec4f*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPVec4f* ret = newEPVec4f();
		EPVec4f_assign(ret,(EPVec4f*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPVec4f_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec4f*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPVec4f_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPVec4f* ret = newEPVec4f();
	ret->ptr->assign(((EPVec4f*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPVec4f_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPVec4f*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPVec4f_equal(EPVec4f* v1, EPVec4f* v2)
{
	if ( !EPVec4f_Check(v1)|| !EPVec4f_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPVec4f_square(EPVec4f* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPVec4f_norm(EPVec4f* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}






///EPVec4f_method[]にメソッド情報を格納
static PyMethodDef EPVec4f_methods[] =
{
	{"Clear",(PyCFunction)EPVec4f_clear,METH_NOARGS,"spr Vec4f clear"},
	{"Set",(PyCFunction)EPVec4f_set,METH_VARARGS,"spr Vec4f set"},
	{"Add",(PyCFunction)EPVec4f_add,METH_O,"spr Vec4f add"},
	{"Assign",(PyCFunction)EPVec4f_assign,METH_O,"spr Vec4f assign"},
	{"Sub",(PyCFunction)EPVec4f_sub,METH_O,"spr Vec4f sub"},
	{"Multi",(PyCFunction)EPVec4f_multi,METH_VARARGS,"spr Vec4f multi"},
	{"Div",(PyCFunction)EPVec4f_div,METH_VARARGS,"spr Vec4f div"},
	{"Dot",(PyCFunction)EPVec4f_dot,METH_O,"spr Vec4f dot"},
	{"Equal",(PyCFunction)EPVec4f_equal,METH_O,"spr Vec4f equal"},
	{"Square",(PyCFunction)EPVec4f_square,METH_NOARGS,"spr Vec4f square"},
	{"Norm",(PyCFunction)EPVec4f_norm,METH_NOARGS,"spr Vec4f norm"},
	{NULL}
};

static PyNumberMethods EPVec4f_math_methods =
{
	(binaryfunc) EPVec4f_add,					/* __add__ */
	(binaryfunc) EPVec4f_sub,					/* __sub__ */
	(binaryfunc) EPVec4f_multi,					/* __mul__ */
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
	(binaryfunc) EPVec4f_iadd,					/*__iadd__*/
	(binaryfunc) EPVec4f_isub,					/*__isub__*/
	(binaryfunc) EPVec4f_imulti,				/*__imul__*/
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
	(binaryfunc)  EPVec4f_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPVec4f_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPVec4f,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPVec4f_init(EPVec4f* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Vec4f(0,0,0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Vec4f(0,0,0,0);
		break;
	case 3:
		double x,y,z,w;
		PyArg_ParseTuple(args,"ffff",&x,&y,&z,&w);
		self->ptr = new Vec4f(x,y,z,w);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPVec4f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec4f *self;
	self = ( EPVec4f*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPVec4f_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPVec4f_dealloc(EPVec4f* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPVec4fType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Vec4f",             /*tp_name*/
	sizeof(EPVec4f),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPVec4f_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPVec4f_str,                         /*tp_repr*/
	&EPVec4f_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPVec4f_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Vec4f",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPVec4f_methods,             /* tp_methods */
	0,//EPVec4f_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPVec4f_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPVec4f_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPVec4f(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPVec4fType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPVec4fType);
	PyModule_AddObject(m,"Vec4f",(PyObject *)&EPVec4fType);
	return m;
}

EPVec4f* newEPVec4f()
{
	return (EPVec4f*)EPVec4f_new(&EPVec4fType,NULL,NULL);
}

EPVec4f* newEPVec4f(Vec4f vec)
{
	EPVec4f* ret = (EPVec4f*)EPVec4f_new(&EPVec4fType,NULL,NULL);
	ret->ptr->assign(vec);
	return ret;
}
EPVec4f* newEPVec4f(Vec4f* vec)
{
	EPVec4f* ret = (EPVec4f*)EPVec4f_new(&EPVec4fType,NULL,NULL);
	ret->ptr = vec;
	return ret;
}



PyMODINIT_FUNC initEPVector(void)
{
	initEPVec2f();
	initEPVec3f();
	initEPVec3d();
	initEPVec4f();
	return NULL;
}