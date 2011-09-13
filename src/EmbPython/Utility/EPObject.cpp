#include "..\..\..\include\EmbPython\Utility\SprEPObject.h"



static PyObject* __PYDECL EPObject_dir(PyObject* self)
{
	return PyObject_Dir(self);
}


static PyObject* __PYDECL EPObject_type(PyObject* self)
{
	return PyObject_Type(self);
}



/////////////////////////////////////メソッド登録用
///voidのメソッドの定義
static PyMethodDef EPObject_methods[] =
{
	{"dir",(PyCFunction)EPObject_dir,METH_NOARGS,"EPObject::dir"},
	{"type",(PyCFunction)EPObject_type,METH_NOARGS,"EPObject::type"},
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
	int c;

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
	0,                         /*tp_hash */
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

PyMODINIT_FUNC initEPObject(void)
{
	PyObject* m;

	//Pythonクラスの作成
	if ( PyType_Ready( &EPObjectType ) < 0 ) return NULL ;

	m = PyImport_AddModule("Utility");

	//モジュールに追加
	Py_INCREF(&EPObjectType);
	PyModule_AddObject(m,"EPObject",(PyObject *)&EPObjectType);
	return m;
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