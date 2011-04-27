
#include "..\..\..\include\EmbPython\Utility\SprEPQuaternion.h"



/*********************************************************Quaterniond*****************************************************/
/////////////////////////////////////メソッド登録用
///Quaterniondのメソッドの定義

static PyObject*
EPQuaterniond_clear(EPQuaterniond* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPQuaterniond_str(EPQuaterniond* self)
{
	PyObject* val, *ret;
	val = EPQuaterniond_BuildValue(self);
	ret = PyObject_Str(val);
	Py_DECREF(val);
	return ret;
}

static PyObject*
EPQuaterniond_set(EPQuaterniond *self,PyObject* data)
{
	double dx,dy,dz;
	EPQuaterniond_ArgParseTuple(data);

	self->ptr->w = wi;
	self->ptr->x = xi;
	self->ptr->y = yi;
	self->ptr->z = zi;
	Py_RETURN_NONE;
}


static PyObject*
EPQuaterniond_assign(EPQuaterniond* self, EPQuaterniond* b)
{
	if ( !EPQuaterniond_Check(self)|| !EPQuaterniond_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPQuaterniond_add(EPQuaterniond* v1, EPQuaterniond*v2)
{
	if ( !EPQuaterniond_Check(v1)|| !EPQuaterniond_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPQuaterniond* ret = newEPQuaterniond();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPQuaterniond_iadd(EPQuaterniond* v1, EPQuaterniond*v2)
{
	if ( !EPQuaterniond_Check(v1 )|| !EPQuaterniond_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPQuaterniond_sub(EPQuaterniond* v1, EPQuaterniond*v2)
{
	if ( !EPQuaterniond_Check(v1 )|| !EPQuaterniond_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPQuaterniond* ret = newEPQuaterniond();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPQuaterniond_isub(EPQuaterniond* v1, EPQuaterniond*v2)
{
	if ( !EPQuaterniond_Check(v1 )|| !EPQuaterniond_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPQuaterniond_dot(EPQuaterniond* v1, EPQuaterniond*v2)
{
	if ( !EPQuaterniond_Check(v1 )|| !EPQuaterniond_Check(v2)) Return_ArgError;

	EPQuaterniond* retv = newEPQuaterniond();
	retv->ptr->assign( v1->ptr->exp() );
	double y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("d",y);
	return ret;
}
static PyObject* EPQuaterniond_idot(EPQuaterniond* v1, EPQuaterniond*v2)
{
	if ( !EPQuaterniond_Check(v1 )|| !EPQuaterniond_Check(v2)) Return_ArgError;

	double y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPQuaterniond_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPQuaterniond_Check( v1 ))
	{
		w1 = v1;

		if ( EPQuaterniond_Check( v2 ))
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
	else if ( EPQuaterniond_Check( v2 ))
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
		return EPQuaterniond_dot((EPQuaterniond*)w1,(EPQuaterniond*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPQuaterniond* ret = newEPQuaterniond();
		EPQuaterniond_assign(ret,(EPQuaterniond*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPQuaterniond_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPQuaterniond*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPQuaterniond_div(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPQuaterniond* ret = newEPQuaterniond();
	ret->ptr->assign(((EPQuaterniond*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPQuaterniond_idiv(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPQuaterniond*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPQuaterniond_equal(EPQuaterniond* v1, EPQuaterniond* v2)
{
	if ( !EPQuaterniond_Check(v1)|| !EPQuaterniond_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPQuaterniond_square(EPQuaterniond* self)
{
	double y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPQuaterniond_norm(EPQuaterniond* self)
{
	double y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}

static PyObject* EPQuaterniond_inv(EPQuaterniond* self)
{
	if( EPQuaterniond_Check( self))
	{
		EPQuaterniond* ret = newEPQuaterniond( self->ptr->Inv() );
		Py_XINCREF( ret );
		return (PyObject*)ret;
	}
	Py_RETURN_NONE;
}

static PyObject* EPQuaterniond_trn(EPQuaterniond* self)
{
	Return_NotImplementedError("Quaterniond::Trn");
}

static PyObject* EPQuaterniond_unit(EPQuaterniond* self)
{
	Return_NotImplementedError("Quaterniond::Unit");
}

static PyObject* EPQuaterniond_item(EPQuaterniond* self)
{
	Return_NotImplementedError("Quaterniond::Item");
}




///EPQuaterniond_method[]にメソッド情報を格納
static PyMethodDef EPQuaterniond_methods[] =
{
	{"Clear",(PyCFunction)EPQuaterniond_clear,METH_NOARGS,"spr Quaterniond clear"},
	{"Set",(PyCFunction)EPQuaterniond_set,METH_VARARGS,"spr Quaterniond set"},
	{"Add",(PyCFunction)EPQuaterniond_add,METH_O,"spr Quaterniond add"},
	{"Assign",(PyCFunction)EPQuaterniond_assign,METH_O,"spr Quaterniond assign"},
	{"Sub",(PyCFunction)EPQuaterniond_sub,METH_O,"spr Quaterniond sub"},
	{"Multi",(PyCFunction)EPQuaterniond_multi,METH_VARARGS,"spr Quaterniond multi"},
	{"Div",(PyCFunction)EPQuaterniond_div,METH_VARARGS,"spr Quaterniond div"},
	{"Dot",(PyCFunction)EPQuaterniond_dot,METH_O,"spr Quaterniond dot"},
	{"Equal",(PyCFunction)EPQuaterniond_equal,METH_O,"spr Quaterniond equal"},
	{"Square",(PyCFunction)EPQuaterniond_square,METH_NOARGS,"spr Quaterniond square"},
	{"Inv",(PyCFunction)EPQuaterniond_inv,METH_NOARGS,"spr Quaterniond inv"},
	{"Trn",(PyCFunction)EPQuaterniond_trn,METH_NOARGS,"spr Quaterniond trn"},
	{"Unit",(PyCFunction)EPQuaterniond_unit,METH_NOARGS,"spr Quaterniond unit"},
	{"Item",(PyCFunction)EPQuaterniond_item,METH_NOARGS,"spr Quaterniond item"},
	{NULL}
};

static PyNumberMethods EPQuaterniond_math_methods =
{
	(binaryfunc) EPQuaterniond_add,					/* __add__ */
	(binaryfunc) EPQuaterniond_sub,					/* __sub__ */
	(binaryfunc) EPQuaterniond_multi,					/* __mul__ */
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
	(binaryfunc) EPQuaterniond_iadd,					/*__iadd__*/
	(binaryfunc) EPQuaterniond_isub,					/*__isub__*/
	(binaryfunc) EPQuaterniond_imulti,				/*__imul__*/
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
	(binaryfunc)  EPQuaterniond_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPQuaterniond_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPQuaterniond,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPQuaterniond_init(EPQuaterniond* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Quaterniond();
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Quaterniond();
		break;
	case 4:
		EPQuaterniond_ArgParseTuple(args);
		self->ptr = new Quaterniond(wi,xi,yi,zi);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPQuaterniond_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPQuaterniond *self;
	self = ( EPQuaterniond*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPQuaterniond_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPQuaterniond_dealloc(EPQuaterniond* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPQuaterniondType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Quaterniond",             /*tp_name*/
	sizeof(EPQuaterniond),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPQuaterniond_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPQuaterniond_str,                         /*tp_repr*/
	&EPQuaterniond_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPQuaterniond_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Quaterniond",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPQuaterniond_methods,             /* tp_methods */
	0,//EPQuaterniond_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPQuaterniond_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPQuaterniond_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPQuaterniond(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPQuaterniondType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPQuaterniondType);
	PyModule_AddObject(m,"Quaterniond",(PyObject *)&EPQuaterniondType);
	return m;
}

EPQuaterniond* newEPQuaterniond()
{
	return (EPQuaterniond*)EPQuaterniond_new(&EPQuaterniondType,NULL,NULL);
}

EPQuaterniond* newEPQuaterniond(Quaterniond vec)
{
	EPQuaterniond* ret = (EPQuaterniond*)EPQuaterniond_new(&EPQuaterniondType,NULL,NULL);
	ret->ptr->assign(vec);

	return ret;
}

EPQuaterniond* newEPQuaterniond(Quaterniond* vec)
{
	EPQuaterniond* ret = (EPQuaterniond*)EPQuaterniond_new(&EPQuaterniondType,NULL,NULL);
	ret->ptr->assign(*vec);

	return ret;
}


/*********************************************************Quaternionf*****************************************************/
/////////////////////////////////////メソッド登録用
///Quaternionfのメソッドの定義

static PyObject*
EPQuaternionf_clear(EPQuaternionf* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPQuaternionf_str(EPQuaternionf* self)
{
	PyObject* val, *ret;
	val = EPQuaternionf_BuildValue(self);
	ret = PyObject_Str(val);
	Py_DECREF(val);
	return ret;
}

static PyObject*
EPQuaternionf_set(EPQuaternionf *self,PyObject* data)
{
	float dx,dy,dz;
	EPQuaternionf_ArgParseTuple(data);

	self->ptr->w = wi;
	self->ptr->x = xi;
	self->ptr->y = yi;
	self->ptr->z = zi;
	Py_RETURN_NONE;
}


static PyObject*
EPQuaternionf_assign(EPQuaternionf* self, EPQuaternionf* b)
{
	if ( !EPQuaternionf_Check(self)|| !EPQuaternionf_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPQuaternionf_add(EPQuaternionf* v1, EPQuaternionf*v2)
{
	if ( !EPQuaternionf_Check(v1)|| !EPQuaternionf_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPQuaternionf* ret = newEPQuaternionf();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPQuaternionf_iadd(EPQuaternionf* v1, EPQuaternionf*v2)
{
	if ( !EPQuaternionf_Check(v1 )|| !EPQuaternionf_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPQuaternionf_sub(EPQuaternionf* v1, EPQuaternionf*v2)
{
	if ( !EPQuaternionf_Check(v1 )|| !EPQuaternionf_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPQuaternionf* ret = newEPQuaternionf();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPQuaternionf_isub(EPQuaternionf* v1, EPQuaternionf*v2)
{
	if ( !EPQuaternionf_Check(v1 )|| !EPQuaternionf_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//内積
static PyObject* EPQuaternionf_dot(EPQuaternionf* v1, EPQuaternionf*v2)
{
	if ( !EPQuaternionf_Check(v1 )|| !EPQuaternionf_Check(v2)) Return_ArgError;

	EPQuaternionf* retv = newEPQuaternionf();
	retv->ptr->assign( v1->ptr->exp() );
	float y = retv->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	return ret;
}
static PyObject* EPQuaternionf_idot(EPQuaternionf* v1, EPQuaternionf*v2)
{
	if ( !EPQuaternionf_Check(v1 )|| !EPQuaternionf_Check(v2)) Return_ArgError;

	float y = v1->ptr->dot( *(v2->ptr) );
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
//乗算
static PyObject* EPQuaternionf_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	float x;

	if ( EPQuaternionf_Check( v1 ))
	{
		w1 = v1;

		if ( EPQuaternionf_Check( v2 ))
			w2 = v2;
		//int
		else if ( PyLong_Check(v2) )
			x = (float) PyLong_AS_LONG(v2);
		//float
		else if ( PyFloat_Check(v2) )
			x = PyFloat_AS_DOUBLE(v2);
		//入力エラー
		else
			Return_ArgError;
	}
	else if ( EPQuaternionf_Check( v2 ))
	{
		w1 = v2;
		//int
		if ( PyLong_Check(v1) )
			x = (float) PyLong_AS_LONG(v1);
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
		return EPQuaternionf_dot((EPQuaternionf*)w1,(EPQuaternionf*)w2);
	}
	//掛け算
	else
	{
		//w1のコピーを作る
		EPQuaternionf* ret = newEPQuaternionf();
		EPQuaternionf_assign(ret,(EPQuaternionf*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPQuaternionf_imulti(PyObject* v1, PyObject* v2)
{
	float x;
	if ( PyLong_Check(v2) )
		x = (float) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPQuaternionf*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPQuaternionf_div(PyObject* v1, PyObject* v2)
{
	float x;
	if ( PyLong_Check(v2) )
		x = (float) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	//v1のコピーを作る
	EPQuaternionf* ret = newEPQuaternionf();
	ret->ptr->assign(((EPQuaternionf*)v1)->ptr->exp());

	ret->ptr->div( x );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPQuaternionf_idiv(PyObject* v1, PyObject* v2)
{
	float x;
	if ( PyLong_Check(v2) )
		x = (float) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;

	((EPQuaternionf*)v1)->ptr->div( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}



static PyObject* EPQuaternionf_equal(EPQuaternionf* v1, EPQuaternionf* v2)
{
	if ( !EPQuaternionf_Check(v1)|| !EPQuaternionf_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPQuaternionf_square(EPQuaternionf* self)
{
	float y = self->ptr->square();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}
static PyObject* EPQuaternionf_norm(EPQuaternionf* self)
{
	float y = self->ptr->norm();
	PyObject* ret = Py_BuildValue("f",y);
	Py_INCREF(ret);
	return ret;
}

static PyObject* EPQuaternionf_inv(EPQuaternionf* self)
{
	if( EPQuaternionf_Check( self))
	{
		EPQuaternionf* ret = newEPQuaternionf( self->ptr->Inv() );
		Py_XINCREF( ret );
		return (PyObject*)ret;
	}
	Py_RETURN_NONE;
}

static PyObject* EPQuaternionf_trn(EPQuaternionf* self)
{
	Return_NotImplementedError("Quaternionf::Trn");
}

static PyObject* EPQuaternionf_unit(EPQuaternionf* self)
{
	Return_NotImplementedError("Quaternionf::Unit");
}

static PyObject* EPQuaternionf_item(EPQuaternionf* self)
{
	Return_NotImplementedError("Quaternionf::Item");
}




///EPQuaternionf_method[]にメソッド情報を格納
static PyMethodDef EPQuaternionf_methods[] =
{
	{"Clear",(PyCFunction)EPQuaternionf_clear,METH_NOARGS,"spr Quaternionf clear"},
	{"Set",(PyCFunction)EPQuaternionf_set,METH_VARARGS,"spr Quaternionf set"},
	{"Add",(PyCFunction)EPQuaternionf_add,METH_O,"spr Quaternionf add"},
	{"Assign",(PyCFunction)EPQuaternionf_assign,METH_O,"spr Quaternionf assign"},
	{"Sub",(PyCFunction)EPQuaternionf_sub,METH_O,"spr Quaternionf sub"},
	{"Multi",(PyCFunction)EPQuaternionf_multi,METH_VARARGS,"spr Quaternionf multi"},
	{"Div",(PyCFunction)EPQuaternionf_div,METH_VARARGS,"spr Quaternionf div"},
	{"Dot",(PyCFunction)EPQuaternionf_dot,METH_O,"spr Quaternionf dot"},
	{"Equal",(PyCFunction)EPQuaternionf_equal,METH_O,"spr Quaternionf equal"},
	{"Square",(PyCFunction)EPQuaternionf_square,METH_NOARGS,"spr Quaternionf square"},
	{"Inv",(PyCFunction)EPQuaternionf_inv,METH_NOARGS,"spr Quaternionf inv"},
	{"Trn",(PyCFunction)EPQuaternionf_trn,METH_NOARGS,"spr Quaternionf trn"},
	{"Unit",(PyCFunction)EPQuaternionf_unit,METH_NOARGS,"spr Quaternionf unit"},
	{"Item",(PyCFunction)EPQuaternionf_item,METH_NOARGS,"spr Quaternionf item"},
	{NULL}
};

static PyNumberMethods EPQuaternionf_math_methods =
{
	(binaryfunc) EPQuaternionf_add,					/* __add__ */
	(binaryfunc) EPQuaternionf_sub,					/* __sub__ */
	(binaryfunc) EPQuaternionf_multi,					/* __mul__ */
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
	(binaryfunc) EPQuaternionf_iadd,					/*__iadd__*/
	(binaryfunc) EPQuaternionf_isub,					/*__isub__*/
	(binaryfunc) EPQuaternionf_imulti,				/*__imul__*/
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
	(binaryfunc)  EPQuaternionf_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPQuaternionf_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPQuaternionf,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPQuaternionf_init(EPQuaternionf* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Quaternionf();
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Quaternionf();
		break;
	case 4:
		EPQuaternionf_ArgParseTuple(args);
		self->ptr = new Quaternionf(wi,xi,yi,zi);
		break;
	default:
		Return_ArgError_Int;
		break;
	}
	return 0;
}
//////////////////////////////////////////new
static PyObject*
EPQuaternionf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPQuaternionf *self;
	self = ( EPQuaternionf*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPQuaternionf_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPQuaternionf_dealloc(EPQuaternionf* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPQuaternionfType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Quaternionf",             /*tp_name*/
	sizeof(EPQuaternionf),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPQuaternionf_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPQuaternionf_str,                         /*tp_repr*/
	&EPQuaternionf_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPQuaternionf_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Quaternionf",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPQuaternionf_methods,             /* tp_methods */
	0,//EPQuaternionf_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPQuaternionf_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPQuaternionf_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPQuaternionf(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPQuaternionfType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPQuaternionfType);
	PyModule_AddObject(m,"Quaternionf",(PyObject *)&EPQuaternionfType);
	return m;
}

EPQuaternionf* newEPQuaternionf()
{
	return (EPQuaternionf*)EPQuaternionf_new(&EPQuaternionfType,NULL,NULL);
}

EPQuaternionf* newEPQuaternionf(Quaternionf vec)
{
	EPQuaternionf* ret = (EPQuaternionf*)EPQuaternionf_new(&EPQuaternionfType,NULL,NULL);
	ret->ptr->assign(vec);

	return ret;
}

EPQuaternionf* newEPQuaternionf(Quaternionf* vec)
{
	EPQuaternionf* ret = (EPQuaternionf*)EPQuaternionf_new(&EPQuaternionfType,NULL,NULL);
	ret->ptr->assign(*vec);

	return ret;
}



PyMODINIT_FUNC initEPQuaternion(void)
{
	initEPQuaternionf();
	initEPQuaterniond();
	return NULL;
}