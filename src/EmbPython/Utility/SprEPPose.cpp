
#include "..\..\..\include\EmbPython\Utility\SprEPPose.h"




//Pythonの新しい型　PyObjectを作る

/////////////////////////////////////メソッド登録用
///Posedのメソッドの定義

static PyObject*
EPPosed_clear(EPPosed* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPPosed_str(EPPosed* self)
{
	PyObject* val, *ret;
	val = EPPosed_BuildValue(self);
	ret = PyObject_Str(val);
	Py_DECREF(val);
	return ret;
}

static PyObject*
EPPosed_set(EPPosed *self,PyObject* data)
{
	double dx,dy,dz;
	EPPosed_ArgParseTuple(data);

	self->ptr->w = wi;
	self->ptr->x = xi;
	self->ptr->y = yi;
	self->ptr->z = zi;
	self->ptr->px = pxi;
	self->ptr->py = pyi;
	self->ptr->pz = pzi;
	Py_RETURN_NONE;
}


static PyObject*
EPPosed_assign(EPPosed* self, EPPosed* b)
{
	if ( !EPPosed_Check(self)|| !EPPosed_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPPosed_add(EPPosed* v1, EPPosed*v2)
{
	if ( !EPPosed_Check(v1)|| !EPPosed_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPPosed* ret = newEPPosed();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPPosed_iadd(EPPosed* v1, EPPosed*v2)
{
	if ( !EPPosed_Check(v1 )|| !EPPosed_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPPosed_sub(EPPosed* v1, EPPosed*v2)
{
	if ( !EPPosed_Check(v1 )|| !EPPosed_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPPosed* ret = newEPPosed();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPPosed_isub(EPPosed* v1, EPPosed*v2)
{
	if ( !EPPosed_Check(v1 )|| !EPPosed_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPPosed_dot(EPPosed* v1, EPPosed*v2)
{
	if ( !EPPosed_Check(v1 )|| !EPPosed_Check(v2)) Return_ArgError;

	EPPosed* retv = newEPPosed();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPPosed_idot(EPPosed* v1, EPPosed*v2)
{
	if ( !EPPosed_Check(v1 )|| !EPPosed_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPPosed_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPPosed_Check( v1 ))
	{
		w1 = v1;

		if ( EPPosed_Check( v2 ))
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
	else if ( EPPosed_Check( v2 ))
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
		return EPPosed_dot((EPPosed*)w1,(EPPosed*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPPosed* ret = newEPPosed();
		EPPosed_assign(ret,(EPPosed*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPPosed_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPPosed*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPPosed_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPPosed* ret = newEPPosed();
	ret->ptr->assign(((EPPosed*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPPosed_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPPosed*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPPosed_equal(EPPosed* v1, EPPosed* v2)
{
	if ( !EPPosed_Check(v1)|| !EPPosed_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPPosed_square(EPPosed* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPPosed_norm(EPPosed* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}

static PyObject* EPPosed_inv(EPPosed* self)
{
	if( EPPosed_Check( self))
	{
		EPPosed* ret = newEPPosed( self->ptr->Inv() );
		Py_XINCREF( ret );
		return (PyObject*)ret;
	}
	Py_RETURN_NONE;
}

static PyObject* EPPosed_trn(EPPosed* self)
{
	Return_NotImplementedError("Posed::Trn");
}

static PyObject* EPPosed_unit(EPPosed* self)
{
	Return_NotImplementedError("Posed::Unit");
}

static PyObject* EPPosed_item(EPPosed* self)
{
	Return_NotImplementedError("Posed::Item");
}




///EPPosed_method[]にメソッド情報を格納
static PyMethodDef EPPosed_methods[] =
{
	{"Clear",(PyCFunction)EPPosed_clear,METH_NOARGS,"spr Posed clear"},
	{"Set",(PyCFunction)EPPosed_set,METH_VARARGS,"spr Posed set"},
	{"Add",(PyCFunction)EPPosed_add,METH_O,"spr Posed add"},
	{"Assign",(PyCFunction)EPPosed_assign,METH_O,"spr Posed assign"},
	{"Sub",(PyCFunction)EPPosed_sub,METH_O,"spr Posed sub"},
	{"Multi",(PyCFunction)EPPosed_multi,METH_VARARGS,"spr Posed multi"},
	{"Div",(PyCFunction)EPPosed_div,METH_VARARGS,"spr Posed div"},
	{"Dot",(PyCFunction)EPPosed_dot,METH_O,"spr Posed dot"},
	{"Equal",(PyCFunction)EPPosed_equal,METH_O,"spr Posed equal"},
	{"Square",(PyCFunction)EPPosed_square,METH_NOARGS,"spr Posed square"},
	{"Inv",(PyCFunction)EPPosed_inv,METH_NOARGS,"spr Posed inv"},
	{"Trn",(PyCFunction)EPPosed_trn,METH_NOARGS,"spr Posed trn"},
	{"Unit",(PyCFunction)EPPosed_unit,METH_NOARGS,"spr Posed unit"},
	{"Item",(PyCFunction)EPPosed_item,METH_NOARGS,"spr Posed item"},
	{NULL}
};

static PyNumberMethods EPPosed_math_methods =
{
	(binaryfunc) EPPosed_add,					/* __add__ */
	(binaryfunc) EPPosed_sub,					/* __sub__ */
	(binaryfunc) EPPosed_multi,					/* __mul__ */
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
	(binaryfunc) EPPosed_iadd,					/*__iadd__*/
	(binaryfunc) EPPosed_isub,					/*__isub__*/
	(binaryfunc) EPPosed_imulti,				/*__imul__*/
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
	(binaryfunc)  EPPosed_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPPosed_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPPosed,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPPosed_init(EPPosed* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Posed(0,0,0,0,0,0,0);
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Posed(0,0,0,0,0,0,0);
		break;
	case 7:
		EPPosed_ArgParseTuple(args);
		self->ptr = new Posed(wi,xi,yi,zi,pxi,pyi,pzi);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPPosed_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPosed *self;
	self = ( EPPosed*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPPosed_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPPosed_dealloc(EPPosed* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPPosedType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Posed",             /*tp_name*/
	sizeof(EPPosed),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPPosed_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPPosed_str,                         /*tp_repr*/
	&EPPosed_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPPosed_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Posed",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPPosed_methods,             /* tp_methods */
	0,//EPPosed_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPPosed_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPPosed_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPPosed(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPPosedType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPPosedType);
	PyModule_AddObject(m,"Posed",(PyObject *)&EPPosedType);
	return m;
}

EPPosed* newEPPosed()
{
	return (EPPosed*)EPPosed_new(&EPPosedType,NULL,NULL);
}

EPPosed* newEPPosed(Posed vec)
{
	EPPosed* ret = (EPPosed*)EPPosed_new(&EPPosedType,NULL,NULL);
	ret->ptr->assign(vec);

	return ret;
}

EPPosed* newEPPosed(Posed* vec)
{
	EPPosed* ret = (EPPosed*)EPPosed_new(&EPPosedType,NULL,NULL);
	ret->ptr->assign(*vec);

	return ret;
}

PyMODINIT_FUNC initEPPose(void)
{
	initEPPosed();
	return NULL;
}