
#include "..\..\..\include\EmbPython\Utility\SprEPAffine.h"



/**************************************************Affinef*************************************/
/////////////////////////////////////メソッド登録用
///Affinefのメソッドの定義

static PyObject*
EPAffinef_clear(EPAffinef* self)
{
	self->ptr->clear();
	Py_RETURN_NONE;
}


static PyObject*
EPAffinef_str(EPAffinef* self)
{
	PyObject* val, *ret;
	val = Py_BuildValue("ffffffffffffffff"
	                    ,self->ptr->ExX(),self->ptr->ExY(),self->ptr->ExZ(),self->ptr->ExW()
	                    ,self->ptr->EyX(),self->ptr->EyY(),self->ptr->EyZ(),self->ptr->EyW()
	                    ,self->ptr->EzX(),self->ptr->EzY(),self->ptr->EzZ(),self->ptr->EzW()
	                    ,self->ptr->PosX(),self->ptr->PosY(),self->ptr->PosZ(),self->ptr->PosW()
	                   );
	ret = PyObject_Str(val);
	Py_DECREF(val);

	return ret;
}

static PyObject*
EPAffinef_set(EPAffinef *self,PyObject* data)
{
	float xx,xy,xz,xw,yx,yy,yz,yw,zx,zy,zz,zw,px,py,pz,pw;
	PyArg_ParseTuple(data,"ffffffffffffffff",
	                 &xx,&xy,&xz,&xw,
	                 &yx,&yy,&yz,&yw,
	                 &zx,&zy,&zz,&zw,
	                 &px,&py,&pz,&pw);
	self->ptr->ExX() = xx;
	self->ptr->ExY() = xy;
	self->ptr->ExZ() = xz;
	self->ptr->ExW() = xw;

	self->ptr->EyX() = yx;
	self->ptr->EyY() = yy;
	self->ptr->EyZ() = yz;
	self->ptr->EyW() = yw;

	self->ptr->EzX() = zx;
	self->ptr->EzY() = zy;
	self->ptr->EzZ() = zz;
	self->ptr->EzW() = zw;

	self->ptr->PosX() = px;
	self->ptr->PosY() = py;
	self->ptr->PosZ() = pz;
	self->ptr->PosW() = pw;


	Py_RETURN_NONE;
}

//代入
static PyObject* EPAffinef_assign(EPAffinef* self, EPAffinef* b)
{
	if ( !EPAffinef_Check(self)|| !EPAffinef_Check(b)) Return_ArgError;

	self->ptr->assign( b->ptr->exp() );
	Py_RETURN_NONE;
}


//加算
static PyObject*
EPAffinef_add(EPAffinef* v1, EPAffinef*v2)
{
	if ( !EPAffinef_Check(v1)|| !EPAffinef_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPAffinef* ret = newEPAffinef();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->add( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject*
EPAffinef_iadd(EPAffinef* v1, EPAffinef*v2)
{
	if ( !EPAffinef_Check(v1 )|| !EPAffinef_Check(v2)) Return_ArgError;

	v1->ptr->add( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//減算
static PyObject* EPAffinef_sub(EPAffinef* v1, EPAffinef*v2)
{
	if ( !EPAffinef_Check(v1 )|| !EPAffinef_Check(v2)) Return_ArgError;

	//v1のコピーを作る
	EPAffinef* ret = newEPAffinef();
	ret->ptr->assign(v1->ptr->exp());

	ret->ptr->sub( *(v2->ptr) );
	Py_INCREF(ret);
	return (PyObject*)ret;
}
static PyObject* EPAffinef_isub(EPAffinef* v1, EPAffinef*v2)
{
	if ( !EPAffinef_Check(v1 )|| !EPAffinef_Check(v2)) Return_ArgError;

	v1->ptr->sub( *(v2->ptr) );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//外積
static PyObject* EPAffinef_cross(EPAffinef* v1, EPAffinef*v2)
{
	Return_NotImplementedError("Cross ha mada desu");

	//if ( !EPAffinef_Check(v1 )|| !EPAffinef_Check(v2)) Return_ArgError;

	//EPAffinef* retv = newEPAffinef();
	//retv->ptr->assign( v1->ptr->exp() );
	//double y = retv->ptr->dot( *(v2->ptr) );

	//PyObject* ret = Py_BuildValue("d",y);
	//return ret;
	//Py_RETURN_FALSE;
}

//乗算
static PyObject* EPAffinef_multi(PyObject* v1, PyObject* v2)
{
	PyObject* w1 = NULL;
	PyObject* w2 = NULL;
	double x;

	if ( EPAffinef_Check( v1 ))
	{
		w1 = v1;

		if ( EPAffinef_Check( v2 ))
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
	else if ( EPAffinef_Check( v2 ))
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
		//return EPAffinef_dot((EPAffinef*)w1,(EPAffinef*)w2);
	}
	//スカラー倍
	else
	{
		//w1のコピーを作る
		EPAffinef* ret = newEPAffinef();
		EPAffinef_assign(ret,(EPAffinef*)w1);

		ret->ptr->multi( x );
		Py_INCREF(ret);
		return (PyObject*)ret;
	}
}
static PyObject* EPAffinef_imulti(PyObject* v1, PyObject* v2)
{
	double x;
	if ( PyLong_Check(v2) )
		x = (double) PyLong_AS_LONG(v2);
	else if ( PyFloat_Check(v2) )
		x = PyFloat_AS_DOUBLE(v2);
	else
		Return_ArgError;
	((EPAffinef*)v1)->ptr->multi( x );
	Py_INCREF(v1);
	return (PyObject*)v1;
}
//除算
static PyObject* EPAffinef_div(PyObject* v1, PyObject* v2)
{
	//double x;
	//if ( PyLong_Check(v2) )
	//	x = (double) PyLong_AS_LONG(v2);
	//else if ( PyFloat_Check(v2) )
	//	x = PyFloat_AS_DOUBLE(v2);
	//else
	//	Return_ArgError;

	////v1のコピーを作る
	//EPAffinef* ret = newEPAffinef();
	//ret->ptr->assign(((EPAffinef*)v1)->ptr->exp());
	//
	//ret->ptr->div( x );
	//Py_INCREF(ret);
	//return (PyObject*)ret;
	Py_RETURN_FALSE;
}
static PyObject* EPAffinef_idiv(PyObject* v1, PyObject* v2)
{
	//double x;
	//if ( PyLong_Check(v2) )
	//	x = (double) PyLong_AS_LONG(v2);
	//else if ( PyFloat_Check(v2) )
	//	x = PyFloat_AS_DOUBLE(v2);
	//else
	//	Return_ArgError;

	//((EPAffinef*)v1)->ptr->div( x );
	//Py_INCREF(v1);
	//return (PyObject*)v1;
	Py_RETURN_FALSE;
}



static PyObject* EPAffinef_equal(EPAffinef* v1, EPAffinef* v2)
{
	if ( !EPAffinef_Check(v1)|| !EPAffinef_Check(v2))
	{
		if (v1->ptr->equal( v2->ptr->exp() ) ) Py_RETURN_TRUE;
		else Py_RETURN_FALSE;
	}

	Py_RETURN_FALSE;
}

static PyObject* EPAffinef_square(EPAffinef* self)
{
	//double y = self->ptr->square();
	//PyObject* ret = Py_BuildValue("f",y);
	//Py_INCREF(ret);
	//return ret;
	Py_RETURN_FALSE;
}
static PyObject* EPAffinef_norm(EPAffinef* self)
{
	//double y = self->ptr->norm();
	//PyObject* ret = Py_BuildValue("f",y);
	//Py_INCREF(ret);
	//return ret;
	Py_RETURN_FALSE;
}






///EPAffinef_method[]にメソッド情報を格納
static PyMethodDef EPAffinef_methods[] =
{
	{"Clear",(PyCFunction)EPAffinef_clear,METH_NOARGS,"spr Affinef clear"},
	{"Set",(PyCFunction)EPAffinef_set,METH_VARARGS,"spr Affinef set"},
	{"Add",(PyCFunction)EPAffinef_add,METH_O,"spr Affinef add"},
	{"Assign",(PyCFunction)EPAffinef_assign,METH_O,"spr Affinef assign"},
	{"Sub",(PyCFunction)EPAffinef_sub,METH_O,"spr Affinef sub"},
	{"Multi",(PyCFunction)EPAffinef_multi,METH_VARARGS,"spr Affinef multi"},
	{"Div",(PyCFunction)EPAffinef_div,METH_VARARGS,"spr Affinef div"},
	{"Cross",(PyCFunction)EPAffinef_cross,METH_O,"spr Affinef dot"},
	{"Equal",(PyCFunction)EPAffinef_equal,METH_O,"spr Affinef equal"},
	{"Square",(PyCFunction)EPAffinef_square,METH_NOARGS,"spr Affinef square"},
	{"Norm",(PyCFunction)EPAffinef_norm,METH_NOARGS,"spr Affinef norm"},
	{NULL}
};

static PyNumberMethods EPAffinef_math_methods =
{
	(binaryfunc) EPAffinef_add,					/* __add__ */
	(binaryfunc) EPAffinef_sub,					/* __sub__ */
	(binaryfunc) EPAffinef_multi,					/* __mul__ */
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
	(binaryfunc) EPAffinef_iadd,					/*__iadd__*/
	(binaryfunc) EPAffinef_isub,					/*__isub__*/
	(binaryfunc) EPAffinef_imulti,				/*__imul__*/
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
	(binaryfunc)  EPAffinef_idiv,				/* __div__*/
	(binaryfunc) NULL,							/*__ifloordiv__*/
	(binaryfunc) NULL,							/*__itruediv__*/
};

/////////////////////////////////////////メンバ変数登録用

//#ifndef T_OBJECT_EX
//#define T_OBJECT_EX 16
//#endif
//static PyMemberDef EPAffinef_members[] = {
//	{"Vars",T_OBJECT_EX, offsetof(EPAffinef,vars), 0, "Test Member"},
//	{NULL}
//};

//////////////////////////////////////////init
static int
EPAffinef_init(EPAffinef* self,PyObject *args, PyObject *kwds)
{
	if ( args == NULL )
	{
		self->ptr = new Affinef();
		return 0;
	}

	switch (PyTuple_Size(args))
	{
	case 0:
		self->ptr = new Affinef();
		break;
	case 16:
		self->ptr = new Affinef();
		EPAffinef_set(self,args);
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
EPAffinef_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPAffinef *self;
	self = ( EPAffinef*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		EPAffinef_init(self,args,kwds);
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void
EPAffinef_dealloc(EPAffinef* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

PyTypeObject EPAffinefType =
{
	PyVarObject_HEAD_INIT(NULL,0)

	"Test.Affinef",             /*tp_name*/
	sizeof(EPAffinef),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPAffinef_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	(reprfunc)EPAffinef_str,                         /*tp_repr*/
	&EPAffinef_math_methods,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	(reprfunc)EPAffinef_str,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
	"Affinef",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	0,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPAffinef_methods,             /* tp_methods */
	0,//EPAffinef_members,             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPAffinef_init,      /* tp_init */
	0,                         /* tp_alloc */
	EPAffinef_new,                 /* tp_new */

};

PyMODINIT_FUNC initEPAffinef(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPAffinefType ) < 0 ) return NULL;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPAffinefType);
	PyModule_AddObject(m,"Affinef",(PyObject *)&EPAffinefType);
	return m;
}

EPAffinef* newEPAffinef()
{
	return (EPAffinef*)EPAffinef_new(&EPAffinefType,NULL,NULL);
}

EPAffinef* newEPAffinef(Affinef vec)
{
	EPAffinef* ret = (EPAffinef*)EPAffinef_new(&EPAffinefType,NULL,NULL);
	ret->ptr->assign(vec);

	return ret;
}

EPAffinef* newEPAffinef(Affinef* vec)
{
	EPAffinef* ret = (EPAffinef*)EPAffinef_new(&EPAffinefType,NULL,NULL);
	ret->ptr->assign(*vec);

	return ret;
}



PyMODINIT_FUNC initEPAffine(void)
{
	initEPAffinef();
	return NULL;
}