
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



PyMODINIT_FUNC initEPVector(void)
{
	initEPVec2f();
	initEPVec3f();
	initEPVec3d();
	initEPVec4f();
	return NULL;
}