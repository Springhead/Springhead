#include "..\..\..\include\EmbPython\Utility\SprEPObject.h"
#include "..\..\..\include\EmbPython\Utility\SprEPCast.h"
#include "..\..\..\include\EmbPython\SprEPFoundation.h"
#include "..\..\..\include\EmbPython\SprEPUtility.h"
#include "..\..\..\include\EmbPython\SprEPBase.h"

static PyObject* __PYDECL EPObject_dir(PyObject* self)
{
	return PyObject_Dir(self);
}


static PyObject* __PYDECL EPObject_type(PyObject* self)
{
	return PyObject_Type(self);
}

static PyObject* __PYDECL EPObject_cast(PyObject* self, PyObject* to)
{
	if( PyUnicode_Check(to) )
	{
		EPObject_RuntimeDCast((EPObject*)self, ConvertStr( to ).GetBuffer() );
	}
	else if( EPIfInfo_Check(to) )
	{
		EPObject_RuntimeDCast((EPObject*)self, EPObject_Cast(to,IfInfo));
	}
	else
	{
		PyErr_BadArgument();
		return NULL;
	}
	Py_RETURN_NONE;
}



/////////////////////////////////////メソッド登録用
///voidのメソッドの定義
static PyMethodDef EPObject_methods[] =
{
	{"dir",(PyCFunction)EPObject_dir,METH_NOARGS|METH_CLASS,"EPObject::dir"},
	{"type",(PyCFunction)EPObject_type,METH_NOARGS,"EPObject::type"},
	{"cast",(PyCFunction)EPObject_cast,METH_O,"EPObject::cast"},
	{NULL}
};

//////////////////////////////////////////init
static int __PYDECL EPObject_init(EPObject* self,PyObject *args, PyObject *kwds)
{
	self->ptr = NULL;
	return 0;
}
//////////////////////////////////////////new
static PyObject* __PYDECL EPObject_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPObject *self;
	self = ( EPObject*) type->tp_alloc(type,0);
	if ( self != NULL)
	{
		//特にやることなし
	}
	return (PyObject *)self;
}
////////////////////////////////////////////dealloc
static void __PYDECL EPObject_dealloc(EPObject* self)
{
	//PythonでいらなくてもSpringheadで使ってるかもしれない
	//delete(self->ptr);
	self->ob_base.ob_type->tp_free((PyObject*)self);
}

//比較演算子オーバーロード
static PyObject* __PYDECL EPObject_richcmp(PyObject *obj1, PyObject *obj2, int op)
{
	//EPObjectは == で比較したとき、同じSpringheadのインスタンスへのポインタの場合はTrue,それ以外はFalseを返す
    PyObject *result;
	int c = 0;

	if ( !EPObject_Check(obj1) || !EPObject_Check(obj2) ) c = 0;
	else
	{
		switch (op) {
		//case Py_LT: c = size1 <  size2; break;
		//case Py_LE: c = size1 <= size2; break;
		case Py_EQ: c = EPObject_Ptr(obj1) == EPObject_Ptr(obj2)  ; break;
		case Py_NE: c = EPObject_Ptr(obj1) != EPObject_Ptr(obj2); break;
		//case Py_GT: c = size1 >  size2; break;
		//case Py_GE: c = size1 >= size2; break;
		}
	}

    result = c ? Py_True : Py_False;
    Py_INCREF(result);
    return result;
}

//辞書のキーにするために必要
static long __PYDECL EPObject_hash(PyObject *obj)
{
	//EPObjectのptrをハッシュ値として使う 
	if ( EPObject_Ptr(obj) == 0 ) {
		PyErr_SetString( PyErr_Spr_NullReference , "Null Reference in EPObject.__hash__");
		return -1;
	}
	else 
		return (long)EPObject_Ptr(obj);
}

PyTypeObject EPObjectType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPObject",             /*tp_name*/
	sizeof(EPObject),             /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	(destructor)EPObject_dealloc,                        /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	(hashfunc)EPObject_hash,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE , /*tp_flags*/
	"EPObject",           /* tp_doc */
	0,		               /* tp_traverse */
	0,		               /* tp_clear */
	EPObject_richcmp,		               /* tp_richcompare */
	0,		               /* tp_weaklistoffset */
	0,		               /* tp_iter */
	0,		               /* tp_iternext */
	EPObject_methods,             /* tp_methods */
	0,			             /* tp_members */
	0,                         /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	(initproc)EPObject_init,      /* tp_init */
	0,                         /* tp_alloc */
	(newfunc)EPObject_new,                 /* tp_new */

};

void initEPObject(PyObject *rootModule)
{
	//Pythonクラスの作成
	if ( PyType_Ready( &EPObjectType ) < 0 ) return ;

	string package;

	if(rootModule) package = PyModule_GetName(rootModule);
	else
	{
#ifdef EP_MODULE_NAME
		package = EP_MODULE_NAME ".";
		rootModule = PyImport_AddModule( EP_MODULE_NAME );
#else
		package = "";
		rootModule = PyImport_AddModule("__main__");
#endif
	}
#ifdef EP_USE_SUBMODULE
	PyObject *subModule = PyImport_AddModule( (package+"Utility").c_str() );
	Py_INCREF(subModule);
	PyModule_AddObject(rootModule,"Utility",subModule);
#else
	PyObject *subModule = rootModule;
#endif

	Py_INCREF(&EPObjectType);
	PyModule_AddObject(subModule,"EPObject",(PyObject*)&EPObjectType);
}

PyObject* newEPObject()
{
	return EPObject_new(&EPObjectType,NULL,NULL);
}

PyObject* newEPObject(const void* ptr)
{
	PyObject* ret = (EPObject_new(&EPObjectType,NULL,NULL));
	EPObject_Ptr(ret) = ptr;

	return ret;
}