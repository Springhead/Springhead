#include "../../include\EmbPython\SprEPUtility.h" 
#pragma warning(disable:4244) 
UTCriticalSection EPCriticalSection;

//{*********EPVec2f*******
PyObject* EPVec2f_clear( EPVec2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2f_norm( EPVec2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2f_resize( EPVec2f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2f_size( EPVec2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2f_square( EPVec2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2f_unit( EPVec2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2f_unitize( EPVec2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPVec2f_methods[] =
{
	{"clear",(PyCFunction)EPVec2f_clear,METH_NOARGS,"EPVec2f::clear"},
	{"norm",(PyCFunction)EPVec2f_norm,METH_NOARGS,"EPVec2f::norm"},
	{"resize",(PyCFunction)EPVec2f_resize,METH_O,"EPVec2f::resize"},
	{"size",(PyCFunction)EPVec2f_size,METH_NOARGS,"EPVec2f::size"},
	{"square",(PyCFunction)EPVec2f_square,METH_NOARGS,"EPVec2f::square"},
	{"unit",(PyCFunction)EPVec2f_unit,METH_NOARGS,"EPVec2f::unit"},
	{"unitize",(PyCFunction)EPVec2f_unitize,METH_NOARGS,"EPVec2f::unitize"},
	{NULL}
};
void EPVec2f_dealloc(PyObject* self)
{
	delete ((EPVec2f*)self)->ptr;
}
PyObject* EPVec2f_str()
{
	return Py_BuildValue("s","This is EPVec2f.");
}
int EPVec2f_init(EPVec2f* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPVec2f* EPVec2f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec2f *self;
	self = (EPVec2f*) type->tp_alloc(type,0);
	if ( self != NULL ) EPVec2f_init(self,args,kwds);
	return self;
}
PyTypeObject EPVec2fType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPVec2f",/*tp_name*/
	sizeof(EPVec2f),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPVec2f_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPVec2f_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Vec2f",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPVec2f_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPVec2f_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPVec2f_new,/*tp_new*/
};
void initEPVec2f(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPVec2fType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPVec2fType);
	PyModule_AddObject(m,"Vec2f",(PyObject *)&EPVec2fType);//モジュールに追加
}
PyObject* newEPVec2f(Vec2f org)
{
	EPVec2f *ret = EPVec2f_new(&EPVec2fType,NULL,NULL);
	ret->ptr = new Vec2f();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPVec2f(Vec2f* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPVec2f *ret = EPVec2f_new(&EPVec2fType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Vec2f

//{*********EPVec2d*******
PyObject* EPVec2d_clear( EPVec2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2d_norm( EPVec2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2d_resize( EPVec2d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2d_size( EPVec2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2d_square( EPVec2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2d_unit( EPVec2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec2d* ret = (EPVec2d*)newEPVec2d(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec2d_unitize( EPVec2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPVec2d_methods[] =
{
	{"clear",(PyCFunction)EPVec2d_clear,METH_NOARGS,"EPVec2d::clear"},
	{"norm",(PyCFunction)EPVec2d_norm,METH_NOARGS,"EPVec2d::norm"},
	{"resize",(PyCFunction)EPVec2d_resize,METH_O,"EPVec2d::resize"},
	{"size",(PyCFunction)EPVec2d_size,METH_NOARGS,"EPVec2d::size"},
	{"square",(PyCFunction)EPVec2d_square,METH_NOARGS,"EPVec2d::square"},
	{"unit",(PyCFunction)EPVec2d_unit,METH_NOARGS,"EPVec2d::unit"},
	{"unitize",(PyCFunction)EPVec2d_unitize,METH_NOARGS,"EPVec2d::unitize"},
	{NULL}
};
void EPVec2d_dealloc(PyObject* self)
{
	delete ((EPVec2d*)self)->ptr;
}
PyObject* EPVec2d_str()
{
	return Py_BuildValue("s","This is EPVec2d.");
}
int EPVec2d_init(EPVec2d* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPVec2d* EPVec2d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec2d *self;
	self = (EPVec2d*) type->tp_alloc(type,0);
	if ( self != NULL ) EPVec2d_init(self,args,kwds);
	return self;
}
PyTypeObject EPVec2dType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPVec2d",/*tp_name*/
	sizeof(EPVec2d),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPVec2d_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPVec2d_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Vec2d",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPVec2d_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPVec2d_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPVec2d_new,/*tp_new*/
};
void initEPVec2d(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPVec2dType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPVec2dType);
	PyModule_AddObject(m,"Vec2d",(PyObject *)&EPVec2dType);//モジュールに追加
}
PyObject* newEPVec2d(Vec2d org)
{
	EPVec2d *ret = EPVec2d_new(&EPVec2dType,NULL,NULL);
	ret->ptr = new Vec2d();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPVec2d(Vec2d* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPVec2d *ret = EPVec2d_new(&EPVec2dType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Vec2d

//{*********EPVec3f*******
PyObject* EPVec3f_clear( EPVec3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3f_norm( EPVec3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3f_resize( EPVec3f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3f_size( EPVec3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3f_square( EPVec3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3f_unit( EPVec3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3f_unitize( EPVec3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPVec3f_methods[] =
{
	{"clear",(PyCFunction)EPVec3f_clear,METH_NOARGS,"EPVec3f::clear"},
	{"norm",(PyCFunction)EPVec3f_norm,METH_NOARGS,"EPVec3f::norm"},
	{"resize",(PyCFunction)EPVec3f_resize,METH_O,"EPVec3f::resize"},
	{"size",(PyCFunction)EPVec3f_size,METH_NOARGS,"EPVec3f::size"},
	{"square",(PyCFunction)EPVec3f_square,METH_NOARGS,"EPVec3f::square"},
	{"unit",(PyCFunction)EPVec3f_unit,METH_NOARGS,"EPVec3f::unit"},
	{"unitize",(PyCFunction)EPVec3f_unitize,METH_NOARGS,"EPVec3f::unitize"},
	{NULL}
};
void EPVec3f_dealloc(PyObject* self)
{
	delete ((EPVec3f*)self)->ptr;
}
PyObject* EPVec3f_str()
{
	return Py_BuildValue("s","This is EPVec3f.");
}
int EPVec3f_init(EPVec3f* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPVec3f* EPVec3f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec3f *self;
	self = (EPVec3f*) type->tp_alloc(type,0);
	if ( self != NULL ) EPVec3f_init(self,args,kwds);
	return self;
}
PyTypeObject EPVec3fType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPVec3f",/*tp_name*/
	sizeof(EPVec3f),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPVec3f_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPVec3f_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Vec3f",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPVec3f_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPVec3f_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPVec3f_new,/*tp_new*/
};
void initEPVec3f(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPVec3fType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPVec3fType);
	PyModule_AddObject(m,"Vec3f",(PyObject *)&EPVec3fType);//モジュールに追加
}
PyObject* newEPVec3f(Vec3f org)
{
	EPVec3f *ret = EPVec3f_new(&EPVec3fType,NULL,NULL);
	ret->ptr = new Vec3f();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPVec3f(Vec3f* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPVec3f *ret = EPVec3f_new(&EPVec3fType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Vec3f

//{*********EPVec3d*******
PyObject* EPVec3d_clear( EPVec3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3d_norm( EPVec3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3d_resize( EPVec3d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3d_size( EPVec3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3d_square( EPVec3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3d_unit( EPVec3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec3d_unitize( EPVec3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPVec3d_methods[] =
{
	{"clear",(PyCFunction)EPVec3d_clear,METH_NOARGS,"EPVec3d::clear"},
	{"norm",(PyCFunction)EPVec3d_norm,METH_NOARGS,"EPVec3d::norm"},
	{"resize",(PyCFunction)EPVec3d_resize,METH_O,"EPVec3d::resize"},
	{"size",(PyCFunction)EPVec3d_size,METH_NOARGS,"EPVec3d::size"},
	{"square",(PyCFunction)EPVec3d_square,METH_NOARGS,"EPVec3d::square"},
	{"unit",(PyCFunction)EPVec3d_unit,METH_NOARGS,"EPVec3d::unit"},
	{"unitize",(PyCFunction)EPVec3d_unitize,METH_NOARGS,"EPVec3d::unitize"},
	{NULL}
};
void EPVec3d_dealloc(PyObject* self)
{
	delete ((EPVec3d*)self)->ptr;
}
PyObject* EPVec3d_str()
{
	return Py_BuildValue("s","This is EPVec3d.");
}
int EPVec3d_init(EPVec3d* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPVec3d* EPVec3d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec3d *self;
	self = (EPVec3d*) type->tp_alloc(type,0);
	if ( self != NULL ) EPVec3d_init(self,args,kwds);
	return self;
}
PyTypeObject EPVec3dType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPVec3d",/*tp_name*/
	sizeof(EPVec3d),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPVec3d_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPVec3d_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Vec3d",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPVec3d_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPVec3d_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPVec3d_new,/*tp_new*/
};
void initEPVec3d(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPVec3dType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPVec3dType);
	PyModule_AddObject(m,"Vec3d",(PyObject *)&EPVec3dType);//モジュールに追加
}
PyObject* newEPVec3d(Vec3d org)
{
	EPVec3d *ret = EPVec3d_new(&EPVec3dType,NULL,NULL);
	ret->ptr = new Vec3d();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPVec3d(Vec3d* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPVec3d *ret = EPVec3d_new(&EPVec3dType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Vec3d

//{*********EPVec4f*******
PyObject* EPVec4f_clear( EPVec4f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4f_norm( EPVec4f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4f_resize( EPVec4f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4f_size( EPVec4f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4f_square( EPVec4f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4f_unit( EPVec4f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec4f* ret = (EPVec4f*)newEPVec4f(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4f_unitize( EPVec4f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPVec4f_methods[] =
{
	{"clear",(PyCFunction)EPVec4f_clear,METH_NOARGS,"EPVec4f::clear"},
	{"norm",(PyCFunction)EPVec4f_norm,METH_NOARGS,"EPVec4f::norm"},
	{"resize",(PyCFunction)EPVec4f_resize,METH_O,"EPVec4f::resize"},
	{"size",(PyCFunction)EPVec4f_size,METH_NOARGS,"EPVec4f::size"},
	{"square",(PyCFunction)EPVec4f_square,METH_NOARGS,"EPVec4f::square"},
	{"unit",(PyCFunction)EPVec4f_unit,METH_NOARGS,"EPVec4f::unit"},
	{"unitize",(PyCFunction)EPVec4f_unitize,METH_NOARGS,"EPVec4f::unitize"},
	{NULL}
};
void EPVec4f_dealloc(PyObject* self)
{
	delete ((EPVec4f*)self)->ptr;
}
PyObject* EPVec4f_str()
{
	return Py_BuildValue("s","This is EPVec4f.");
}
int EPVec4f_init(EPVec4f* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPVec4f* EPVec4f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec4f *self;
	self = (EPVec4f*) type->tp_alloc(type,0);
	if ( self != NULL ) EPVec4f_init(self,args,kwds);
	return self;
}
PyTypeObject EPVec4fType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPVec4f",/*tp_name*/
	sizeof(EPVec4f),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPVec4f_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPVec4f_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Vec4f",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPVec4f_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPVec4f_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPVec4f_new,/*tp_new*/
};
void initEPVec4f(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPVec4fType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPVec4fType);
	PyModule_AddObject(m,"Vec4f",(PyObject *)&EPVec4fType);//モジュールに追加
}
PyObject* newEPVec4f(Vec4f org)
{
	EPVec4f *ret = EPVec4f_new(&EPVec4fType,NULL,NULL);
	ret->ptr = new Vec4f();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPVec4f(Vec4f* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPVec4f *ret = EPVec4f_new(&EPVec4fType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Vec4f

//{*********EPVec4d*******
PyObject* EPVec4d_clear( EPVec4d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4d_norm( EPVec4d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4d_resize( EPVec4d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4d_size( EPVec4d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4d_square( EPVec4d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4d_unit( EPVec4d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec4d* ret = (EPVec4d*)newEPVec4d(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPVec4d_unitize( EPVec4d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPVec4d_methods[] =
{
	{"clear",(PyCFunction)EPVec4d_clear,METH_NOARGS,"EPVec4d::clear"},
	{"norm",(PyCFunction)EPVec4d_norm,METH_NOARGS,"EPVec4d::norm"},
	{"resize",(PyCFunction)EPVec4d_resize,METH_O,"EPVec4d::resize"},
	{"size",(PyCFunction)EPVec4d_size,METH_NOARGS,"EPVec4d::size"},
	{"square",(PyCFunction)EPVec4d_square,METH_NOARGS,"EPVec4d::square"},
	{"unit",(PyCFunction)EPVec4d_unit,METH_NOARGS,"EPVec4d::unit"},
	{"unitize",(PyCFunction)EPVec4d_unitize,METH_NOARGS,"EPVec4d::unitize"},
	{NULL}
};
void EPVec4d_dealloc(PyObject* self)
{
	delete ((EPVec4d*)self)->ptr;
}
PyObject* EPVec4d_str()
{
	return Py_BuildValue("s","This is EPVec4d.");
}
int EPVec4d_init(EPVec4d* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPVec4d* EPVec4d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPVec4d *self;
	self = (EPVec4d*) type->tp_alloc(type,0);
	if ( self != NULL ) EPVec4d_init(self,args,kwds);
	return self;
}
PyTypeObject EPVec4dType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPVec4d",/*tp_name*/
	sizeof(EPVec4d),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPVec4d_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPVec4d_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Vec4d",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPVec4d_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPVec4d_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPVec4d_new,/*tp_new*/
};
void initEPVec4d(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPVec4dType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPVec4dType);
	PyModule_AddObject(m,"Vec4d",(PyObject *)&EPVec4dType);//モジュールに追加
}
PyObject* newEPVec4d(Vec4d org)
{
	EPVec4d *ret = EPVec4d_new(&EPVec4dType,NULL,NULL);
	ret->ptr = new Vec4d();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPVec4d(Vec4d* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPVec4d *ret = EPVec4d_new(&EPVec4dType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Vec4d

//{*********EPMatrix2f*******
PyObject* EPMatrix2f_Diag( EPMatrix2f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(Matrix2f::Diag(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_Rot( EPMatrix2f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))))
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(Matrix2f::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(Matrix2f::Rot((*(((EPVec2f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_Unit( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(Matrix2f::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_Zero( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(Matrix2f::Zero());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_angle( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->angle());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_clear( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_col( EPMatrix2f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_det( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_height( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_inv( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_resize( EPMatrix2f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_row( EPMatrix2f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_trans( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2f* ret = (EPMatrix2f*)newEPMatrix2f(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2f_width( EPMatrix2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPMatrix2f_methods[] =
{
	{"Diag",(PyCFunction)EPMatrix2f_Diag,METH_VARARGS,"EPMatrix2f::Diag"},
	{"Rot",(PyCFunction)EPMatrix2f_Rot,METH_VARARGS,"EPMatrix2f::Rot"},
	{"Unit",(PyCFunction)EPMatrix2f_Unit,METH_NOARGS,"EPMatrix2f::Unit"},
	{"Zero",(PyCFunction)EPMatrix2f_Zero,METH_NOARGS,"EPMatrix2f::Zero"},
	{"angle",(PyCFunction)EPMatrix2f_angle,METH_NOARGS,"EPMatrix2f::angle"},
	{"clear",(PyCFunction)EPMatrix2f_clear,METH_NOARGS,"EPMatrix2f::clear"},
	{"col",(PyCFunction)EPMatrix2f_col,METH_O,"EPMatrix2f::col"},
	{"det",(PyCFunction)EPMatrix2f_det,METH_NOARGS,"EPMatrix2f::det"},
	{"height",(PyCFunction)EPMatrix2f_height,METH_NOARGS,"EPMatrix2f::height"},
	{"inv",(PyCFunction)EPMatrix2f_inv,METH_NOARGS,"EPMatrix2f::inv"},
	{"resize",(PyCFunction)EPMatrix2f_resize,METH_VARARGS,"EPMatrix2f::resize"},
	{"row",(PyCFunction)EPMatrix2f_row,METH_O,"EPMatrix2f::row"},
	{"trans",(PyCFunction)EPMatrix2f_trans,METH_NOARGS,"EPMatrix2f::trans"},
	{"width",(PyCFunction)EPMatrix2f_width,METH_NOARGS,"EPMatrix2f::width"},
	{NULL}
};
void EPMatrix2f_dealloc(PyObject* self)
{
	delete ((EPMatrix2f*)self)->ptr;
}
PyObject* EPMatrix2f_str()
{
	return Py_BuildValue("s","This is EPMatrix2f.");
}
int EPMatrix2f_init(EPMatrix2f* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPMatrix2f* EPMatrix2f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPMatrix2f *self;
	self = (EPMatrix2f*) type->tp_alloc(type,0);
	if ( self != NULL ) EPMatrix2f_init(self,args,kwds);
	return self;
}
PyTypeObject EPMatrix2fType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPMatrix2f",/*tp_name*/
	sizeof(EPMatrix2f),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPMatrix2f_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPMatrix2f_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Matrix2f",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPMatrix2f_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPMatrix2f_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPMatrix2f_new,/*tp_new*/
};
void initEPMatrix2f(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPMatrix2fType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPMatrix2fType);
	PyModule_AddObject(m,"Matrix2f",(PyObject *)&EPMatrix2fType);//モジュールに追加
}
PyObject* newEPMatrix2f(Matrix2f org)
{
	EPMatrix2f *ret = EPMatrix2f_new(&EPMatrix2fType,NULL,NULL);
	ret->ptr = new Matrix2f();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPMatrix2f(Matrix2f* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPMatrix2f *ret = EPMatrix2f_new(&EPMatrix2fType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Matrix2f

//{*********EPMatrix2d*******
PyObject* EPMatrix2d_Diag( EPMatrix2d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(Matrix2d::Diag(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_Rot( EPMatrix2d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))))
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(Matrix2d::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(Matrix2d::Rot((*(((EPVec2d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_Unit( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(Matrix2d::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_Zero( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(Matrix2d::Zero());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_angle( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->angle());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_clear( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_col( EPMatrix2d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec2d* ret = (EPVec2d*)newEPVec2d(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_det( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_height( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_inv( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_resize( EPMatrix2d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_row( EPMatrix2d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec2d* ret = (EPVec2d*)newEPVec2d(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_trans( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix2d* ret = (EPMatrix2d*)newEPMatrix2d(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix2d_width( EPMatrix2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPMatrix2d_methods[] =
{
	{"Diag",(PyCFunction)EPMatrix2d_Diag,METH_VARARGS,"EPMatrix2d::Diag"},
	{"Rot",(PyCFunction)EPMatrix2d_Rot,METH_VARARGS,"EPMatrix2d::Rot"},
	{"Unit",(PyCFunction)EPMatrix2d_Unit,METH_NOARGS,"EPMatrix2d::Unit"},
	{"Zero",(PyCFunction)EPMatrix2d_Zero,METH_NOARGS,"EPMatrix2d::Zero"},
	{"angle",(PyCFunction)EPMatrix2d_angle,METH_NOARGS,"EPMatrix2d::angle"},
	{"clear",(PyCFunction)EPMatrix2d_clear,METH_NOARGS,"EPMatrix2d::clear"},
	{"col",(PyCFunction)EPMatrix2d_col,METH_O,"EPMatrix2d::col"},
	{"det",(PyCFunction)EPMatrix2d_det,METH_NOARGS,"EPMatrix2d::det"},
	{"height",(PyCFunction)EPMatrix2d_height,METH_NOARGS,"EPMatrix2d::height"},
	{"inv",(PyCFunction)EPMatrix2d_inv,METH_NOARGS,"EPMatrix2d::inv"},
	{"resize",(PyCFunction)EPMatrix2d_resize,METH_VARARGS,"EPMatrix2d::resize"},
	{"row",(PyCFunction)EPMatrix2d_row,METH_O,"EPMatrix2d::row"},
	{"trans",(PyCFunction)EPMatrix2d_trans,METH_NOARGS,"EPMatrix2d::trans"},
	{"width",(PyCFunction)EPMatrix2d_width,METH_NOARGS,"EPMatrix2d::width"},
	{NULL}
};
void EPMatrix2d_dealloc(PyObject* self)
{
	delete ((EPMatrix2d*)self)->ptr;
}
PyObject* EPMatrix2d_str()
{
	return Py_BuildValue("s","This is EPMatrix2d.");
}
int EPMatrix2d_init(EPMatrix2d* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPMatrix2d* EPMatrix2d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPMatrix2d *self;
	self = (EPMatrix2d*) type->tp_alloc(type,0);
	if ( self != NULL ) EPMatrix2d_init(self,args,kwds);
	return self;
}
PyTypeObject EPMatrix2dType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPMatrix2d",/*tp_name*/
	sizeof(EPMatrix2d),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPMatrix2d_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPMatrix2d_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Matrix2d",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPMatrix2d_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPMatrix2d_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPMatrix2d_new,/*tp_new*/
};
void initEPMatrix2d(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPMatrix2dType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPMatrix2dType);
	PyModule_AddObject(m,"Matrix2d",(PyObject *)&EPMatrix2dType);//モジュールに追加
}
PyObject* newEPMatrix2d(Matrix2d org)
{
	EPMatrix2d *ret = EPMatrix2d_new(&EPMatrix2dType,NULL,NULL);
	ret->ptr = new Matrix2d();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPMatrix2d(Matrix2d* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPMatrix2d *ret = EPMatrix2d_new(&EPMatrix2dType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Matrix2d

//{*********EPMatrix3f*******
PyObject* EPMatrix3f_Cross( EPMatrix3f* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Cross((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_Diag( EPMatrix3f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Diag(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_Rot( EPMatrix3f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,2))))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Rot((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)),(*(((EPchar*)PyTuple_GetItem(tuple,2))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Rot((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec4f_Check(((EPVec4f*)PyTuple_GetItem(tuple,0))))
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Rot((*(((EPVec4f*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_Unit( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_Zero( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(Matrix3f::Zero());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_clear( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_col( EPMatrix3f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_det( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_height( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_inv( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_resize( EPMatrix3f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_row( EPMatrix3f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_trans( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3f* ret = (EPMatrix3f*)newEPMatrix3f(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3f_width( EPMatrix3f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPMatrix3f_methods[] =
{
	{"Cross",(PyCFunction)EPMatrix3f_Cross,METH_O,"EPMatrix3f::Cross"},
	{"Diag",(PyCFunction)EPMatrix3f_Diag,METH_VARARGS,"EPMatrix3f::Diag"},
	{"Rot",(PyCFunction)EPMatrix3f_Rot,METH_VARARGS,"EPMatrix3f::Rot"},
	{"Unit",(PyCFunction)EPMatrix3f_Unit,METH_NOARGS,"EPMatrix3f::Unit"},
	{"Zero",(PyCFunction)EPMatrix3f_Zero,METH_NOARGS,"EPMatrix3f::Zero"},
	{"clear",(PyCFunction)EPMatrix3f_clear,METH_NOARGS,"EPMatrix3f::clear"},
	{"col",(PyCFunction)EPMatrix3f_col,METH_O,"EPMatrix3f::col"},
	{"det",(PyCFunction)EPMatrix3f_det,METH_NOARGS,"EPMatrix3f::det"},
	{"height",(PyCFunction)EPMatrix3f_height,METH_NOARGS,"EPMatrix3f::height"},
	{"inv",(PyCFunction)EPMatrix3f_inv,METH_NOARGS,"EPMatrix3f::inv"},
	{"resize",(PyCFunction)EPMatrix3f_resize,METH_VARARGS,"EPMatrix3f::resize"},
	{"row",(PyCFunction)EPMatrix3f_row,METH_O,"EPMatrix3f::row"},
	{"trans",(PyCFunction)EPMatrix3f_trans,METH_NOARGS,"EPMatrix3f::trans"},
	{"width",(PyCFunction)EPMatrix3f_width,METH_NOARGS,"EPMatrix3f::width"},
	{NULL}
};
void EPMatrix3f_dealloc(PyObject* self)
{
	delete ((EPMatrix3f*)self)->ptr;
}
PyObject* EPMatrix3f_str()
{
	return Py_BuildValue("s","This is EPMatrix3f.");
}
int EPMatrix3f_init(EPMatrix3f* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPMatrix3f* EPMatrix3f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPMatrix3f *self;
	self = (EPMatrix3f*) type->tp_alloc(type,0);
	if ( self != NULL ) EPMatrix3f_init(self,args,kwds);
	return self;
}
PyTypeObject EPMatrix3fType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPMatrix3f",/*tp_name*/
	sizeof(EPMatrix3f),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPMatrix3f_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPMatrix3f_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Matrix3f",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPMatrix3f_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPMatrix3f_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPMatrix3f_new,/*tp_new*/
};
void initEPMatrix3f(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPMatrix3fType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPMatrix3fType);
	PyModule_AddObject(m,"Matrix3f",(PyObject *)&EPMatrix3fType);//モジュールに追加
}
PyObject* newEPMatrix3f(Matrix3f org)
{
	EPMatrix3f *ret = EPMatrix3f_new(&EPMatrix3fType,NULL,NULL);
	ret->ptr = new Matrix3f();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPMatrix3f(Matrix3f* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPMatrix3f *ret = EPMatrix3f_new(&EPMatrix3fType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Matrix3f

//{*********EPMatrix3d*******
PyObject* EPMatrix3d_Cross( EPMatrix3d* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Cross((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_Diag( EPMatrix3d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Diag(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_Rot( EPMatrix3d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,2))))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Rot((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)),(*(((EPchar*)PyTuple_GetItem(tuple,2))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Rot((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec4d_Check(((EPVec4d*)PyTuple_GetItem(tuple,0))))
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Rot((*(((EPVec4d*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_Unit( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_Zero( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(Matrix3d::Zero());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_clear( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_col( EPMatrix3d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_det( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_height( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_inv( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_resize( EPMatrix3d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_row( EPMatrix3d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_trans( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPMatrix3d_width( EPMatrix3d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPMatrix3d_methods[] =
{
	{"Cross",(PyCFunction)EPMatrix3d_Cross,METH_O,"EPMatrix3d::Cross"},
	{"Diag",(PyCFunction)EPMatrix3d_Diag,METH_VARARGS,"EPMatrix3d::Diag"},
	{"Rot",(PyCFunction)EPMatrix3d_Rot,METH_VARARGS,"EPMatrix3d::Rot"},
	{"Unit",(PyCFunction)EPMatrix3d_Unit,METH_NOARGS,"EPMatrix3d::Unit"},
	{"Zero",(PyCFunction)EPMatrix3d_Zero,METH_NOARGS,"EPMatrix3d::Zero"},
	{"clear",(PyCFunction)EPMatrix3d_clear,METH_NOARGS,"EPMatrix3d::clear"},
	{"col",(PyCFunction)EPMatrix3d_col,METH_O,"EPMatrix3d::col"},
	{"det",(PyCFunction)EPMatrix3d_det,METH_NOARGS,"EPMatrix3d::det"},
	{"height",(PyCFunction)EPMatrix3d_height,METH_NOARGS,"EPMatrix3d::height"},
	{"inv",(PyCFunction)EPMatrix3d_inv,METH_NOARGS,"EPMatrix3d::inv"},
	{"resize",(PyCFunction)EPMatrix3d_resize,METH_VARARGS,"EPMatrix3d::resize"},
	{"row",(PyCFunction)EPMatrix3d_row,METH_O,"EPMatrix3d::row"},
	{"trans",(PyCFunction)EPMatrix3d_trans,METH_NOARGS,"EPMatrix3d::trans"},
	{"width",(PyCFunction)EPMatrix3d_width,METH_NOARGS,"EPMatrix3d::width"},
	{NULL}
};
void EPMatrix3d_dealloc(PyObject* self)
{
	delete ((EPMatrix3d*)self)->ptr;
}
PyObject* EPMatrix3d_str()
{
	return Py_BuildValue("s","This is EPMatrix3d.");
}
int EPMatrix3d_init(EPMatrix3d* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPMatrix3d* EPMatrix3d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPMatrix3d *self;
	self = (EPMatrix3d*) type->tp_alloc(type,0);
	if ( self != NULL ) EPMatrix3d_init(self,args,kwds);
	return self;
}
PyTypeObject EPMatrix3dType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPMatrix3d",/*tp_name*/
	sizeof(EPMatrix3d),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPMatrix3d_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPMatrix3d_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Matrix3d",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPMatrix3d_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPMatrix3d_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPMatrix3d_new,/*tp_new*/
};
void initEPMatrix3d(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPMatrix3dType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPMatrix3dType);
	PyModule_AddObject(m,"Matrix3d",(PyObject *)&EPMatrix3dType);//モジュールに追加
}
PyObject* newEPMatrix3d(Matrix3d org)
{
	EPMatrix3d *ret = EPMatrix3d_new(&EPMatrix3dType,NULL,NULL);
	ret->ptr = new Matrix3d();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPMatrix3d(Matrix3d* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPMatrix3d *ret = EPMatrix3d_new(&EPMatrix3dType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Matrix3d

//{*********EPAffine2f*******
PyObject* EPAffine2f_Rot( EPAffine2f* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		EPAffine2f* ret = (EPAffine2f*)newEPAffine2f(Affine2f::Rot(PyFloat_AS_DOUBLE((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_Scale( EPAffine2f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		EPAffine2f* ret = (EPAffine2f*)newEPAffine2f(Affine2f::Scale(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_Trn( EPAffine2f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		EPAffine2f* ret = (EPAffine2f*)newEPAffine2f(Affine2f::Trn(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_Unit( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffine2f* ret = (EPAffine2f*)newEPAffine2f(Affine2f::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_clear( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_col( EPAffine2f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_det( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_height( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_inv( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffine2f* ret = (EPAffine2f*)newEPAffine2f(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_resize( EPAffine2f* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_row( EPAffine2f* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_trans( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffine2f* ret = (EPAffine2f*)newEPAffine2f(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2f_width( EPAffine2f* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPAffine2f_methods[] =
{
	{"Rot",(PyCFunction)EPAffine2f_Rot,METH_O,"EPAffine2f::Rot"},
	{"Scale",(PyCFunction)EPAffine2f_Scale,METH_VARARGS,"EPAffine2f::Scale"},
	{"Trn",(PyCFunction)EPAffine2f_Trn,METH_VARARGS,"EPAffine2f::Trn"},
	{"Unit",(PyCFunction)EPAffine2f_Unit,METH_NOARGS,"EPAffine2f::Unit"},
	{"clear",(PyCFunction)EPAffine2f_clear,METH_NOARGS,"EPAffine2f::clear"},
	{"col",(PyCFunction)EPAffine2f_col,METH_O,"EPAffine2f::col"},
	{"det",(PyCFunction)EPAffine2f_det,METH_NOARGS,"EPAffine2f::det"},
	{"height",(PyCFunction)EPAffine2f_height,METH_NOARGS,"EPAffine2f::height"},
	{"inv",(PyCFunction)EPAffine2f_inv,METH_NOARGS,"EPAffine2f::inv"},
	{"resize",(PyCFunction)EPAffine2f_resize,METH_VARARGS,"EPAffine2f::resize"},
	{"row",(PyCFunction)EPAffine2f_row,METH_O,"EPAffine2f::row"},
	{"trans",(PyCFunction)EPAffine2f_trans,METH_NOARGS,"EPAffine2f::trans"},
	{"width",(PyCFunction)EPAffine2f_width,METH_NOARGS,"EPAffine2f::width"},
	{NULL}
};
void EPAffine2f_dealloc(PyObject* self)
{
	delete ((EPAffine2f*)self)->ptr;
}
PyObject* EPAffine2f_str()
{
	return Py_BuildValue("s","This is EPAffine2f.");
}
int EPAffine2f_init(EPAffine2f* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPAffine2f* EPAffine2f_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPAffine2f *self;
	self = (EPAffine2f*) type->tp_alloc(type,0);
	if ( self != NULL ) EPAffine2f_init(self,args,kwds);
	return self;
}
PyTypeObject EPAffine2fType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPAffine2f",/*tp_name*/
	sizeof(EPAffine2f),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPAffine2f_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPAffine2f_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Affine2f",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPAffine2f_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPAffine2f_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPAffine2f_new,/*tp_new*/
};
void initEPAffine2f(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPAffine2fType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPAffine2fType);
	PyModule_AddObject(m,"Affine2f",(PyObject *)&EPAffine2fType);//モジュールに追加
}
PyObject* newEPAffine2f(Affine2f org)
{
	EPAffine2f *ret = EPAffine2f_new(&EPAffine2fType,NULL,NULL);
	ret->ptr = new Affine2f();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPAffine2f(Affine2f* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPAffine2f *ret = EPAffine2f_new(&EPAffine2fType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Affine2f

//{*********EPAffinef*******
PyObject* EPAffinef_LookAt( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->LookAt((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->LookAt((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_LookAtGL( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->LookAtGL((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->LookAtGL((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_ProjectionD3D( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,3))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::ProjectionD3D((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,3)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::ProjectionD3D((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::ProjectionD3D((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_ProjectionGL( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,3))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::ProjectionGL((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,3)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::ProjectionGL((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::ProjectionGL((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_Rot( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_Scale( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::Scale(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_Trn( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::Trn(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_Unit( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(Affinef::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_clear( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_col( EPAffinef* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec4f* ret = (EPVec4f*)newEPVec4f(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_det( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_height( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_inv( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_resize( EPAffinef* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_row( EPAffinef* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec4f* ret = (EPVec4f*)newEPVec4f(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_trans( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffinef_width( EPAffinef* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPAffinef_methods[] =
{
	{"LookAt",(PyCFunction)EPAffinef_LookAt,METH_VARARGS,"EPAffinef::LookAt"},
	{"LookAtGL",(PyCFunction)EPAffinef_LookAtGL,METH_VARARGS,"EPAffinef::LookAtGL"},
	{"ProjectionD3D",(PyCFunction)EPAffinef_ProjectionD3D,METH_VARARGS,"EPAffinef::ProjectionD3D"},
	{"ProjectionGL",(PyCFunction)EPAffinef_ProjectionGL,METH_VARARGS,"EPAffinef::ProjectionGL"},
	{"Rot",(PyCFunction)EPAffinef_Rot,METH_VARARGS,"EPAffinef::Rot"},
	{"Scale",(PyCFunction)EPAffinef_Scale,METH_VARARGS,"EPAffinef::Scale"},
	{"Trn",(PyCFunction)EPAffinef_Trn,METH_VARARGS,"EPAffinef::Trn"},
	{"Unit",(PyCFunction)EPAffinef_Unit,METH_NOARGS,"EPAffinef::Unit"},
	{"clear",(PyCFunction)EPAffinef_clear,METH_NOARGS,"EPAffinef::clear"},
	{"col",(PyCFunction)EPAffinef_col,METH_O,"EPAffinef::col"},
	{"det",(PyCFunction)EPAffinef_det,METH_NOARGS,"EPAffinef::det"},
	{"height",(PyCFunction)EPAffinef_height,METH_NOARGS,"EPAffinef::height"},
	{"inv",(PyCFunction)EPAffinef_inv,METH_NOARGS,"EPAffinef::inv"},
	{"resize",(PyCFunction)EPAffinef_resize,METH_VARARGS,"EPAffinef::resize"},
	{"row",(PyCFunction)EPAffinef_row,METH_O,"EPAffinef::row"},
	{"trans",(PyCFunction)EPAffinef_trans,METH_NOARGS,"EPAffinef::trans"},
	{"width",(PyCFunction)EPAffinef_width,METH_NOARGS,"EPAffinef::width"},
	{NULL}
};
void EPAffinef_dealloc(PyObject* self)
{
	delete ((EPAffinef*)self)->ptr;
}
PyObject* EPAffinef_str()
{
	return Py_BuildValue("s","This is EPAffinef.");
}
int EPAffinef_init(EPAffinef* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPAffinef* EPAffinef_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPAffinef *self;
	self = (EPAffinef*) type->tp_alloc(type,0);
	if ( self != NULL ) EPAffinef_init(self,args,kwds);
	return self;
}
PyTypeObject EPAffinefType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPAffinef",/*tp_name*/
	sizeof(EPAffinef),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPAffinef_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPAffinef_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Affinef",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPAffinef_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPAffinef_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPAffinef_new,/*tp_new*/
};
void initEPAffinef(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPAffinefType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPAffinefType);
	PyModule_AddObject(m,"Affinef",(PyObject *)&EPAffinefType);//モジュールに追加
}
PyObject* newEPAffinef(Affinef org)
{
	EPAffinef *ret = EPAffinef_new(&EPAffinefType,NULL,NULL);
	ret->ptr = new Affinef();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPAffinef(Affinef* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPAffinef *ret = EPAffinef_new(&EPAffinefType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Affinef

//{*********EPAffine2d*******
PyObject* EPAffine2d_Rot( EPAffine2d* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		EPAffine2d* ret = (EPAffine2d*)newEPAffine2d(Affine2d::Rot(PyFloat_AS_DOUBLE((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_Scale( EPAffine2d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		EPAffine2d* ret = (EPAffine2d*)newEPAffine2d(Affine2d::Scale(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_Trn( EPAffine2d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		EPAffine2d* ret = (EPAffine2d*)newEPAffine2d(Affine2d::Trn(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_Unit( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffine2d* ret = (EPAffine2d*)newEPAffine2d(Affine2d::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_clear( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_col( EPAffine2d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_det( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_height( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_inv( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffine2d* ret = (EPAffine2d*)newEPAffine2d(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_resize( EPAffine2d* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_row( EPAffine2d* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_trans( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffine2d* ret = (EPAffine2d*)newEPAffine2d(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffine2d_width( EPAffine2d* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPAffine2d_methods[] =
{
	{"Rot",(PyCFunction)EPAffine2d_Rot,METH_O,"EPAffine2d::Rot"},
	{"Scale",(PyCFunction)EPAffine2d_Scale,METH_VARARGS,"EPAffine2d::Scale"},
	{"Trn",(PyCFunction)EPAffine2d_Trn,METH_VARARGS,"EPAffine2d::Trn"},
	{"Unit",(PyCFunction)EPAffine2d_Unit,METH_NOARGS,"EPAffine2d::Unit"},
	{"clear",(PyCFunction)EPAffine2d_clear,METH_NOARGS,"EPAffine2d::clear"},
	{"col",(PyCFunction)EPAffine2d_col,METH_O,"EPAffine2d::col"},
	{"det",(PyCFunction)EPAffine2d_det,METH_NOARGS,"EPAffine2d::det"},
	{"height",(PyCFunction)EPAffine2d_height,METH_NOARGS,"EPAffine2d::height"},
	{"inv",(PyCFunction)EPAffine2d_inv,METH_NOARGS,"EPAffine2d::inv"},
	{"resize",(PyCFunction)EPAffine2d_resize,METH_VARARGS,"EPAffine2d::resize"},
	{"row",(PyCFunction)EPAffine2d_row,METH_O,"EPAffine2d::row"},
	{"trans",(PyCFunction)EPAffine2d_trans,METH_NOARGS,"EPAffine2d::trans"},
	{"width",(PyCFunction)EPAffine2d_width,METH_NOARGS,"EPAffine2d::width"},
	{NULL}
};
void EPAffine2d_dealloc(PyObject* self)
{
	delete ((EPAffine2d*)self)->ptr;
}
PyObject* EPAffine2d_str()
{
	return Py_BuildValue("s","This is EPAffine2d.");
}
int EPAffine2d_init(EPAffine2d* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPAffine2d* EPAffine2d_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPAffine2d *self;
	self = (EPAffine2d*) type->tp_alloc(type,0);
	if ( self != NULL ) EPAffine2d_init(self,args,kwds);
	return self;
}
PyTypeObject EPAffine2dType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPAffine2d",/*tp_name*/
	sizeof(EPAffine2d),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPAffine2d_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPAffine2d_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Affine2d",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPAffine2d_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPAffine2d_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPAffine2d_new,/*tp_new*/
};
void initEPAffine2d(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPAffine2dType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPAffine2dType);
	PyModule_AddObject(m,"Affine2d",(PyObject *)&EPAffine2dType);//モジュールに追加
}
PyObject* newEPAffine2d(Affine2d org)
{
	EPAffine2d *ret = EPAffine2d_new(&EPAffine2dType,NULL,NULL);
	ret->ptr = new Affine2d();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPAffine2d(Affine2d* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPAffine2d *ret = EPAffine2d_new(&EPAffine2dType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Affine2d

//{*********EPAffined*******
PyObject* EPAffined_LookAt( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->LookAt((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->LookAt((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_LookAtGL( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->LookAtGL((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->LookAtGL((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_ProjectionD3D( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,3))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::ProjectionD3D((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2d*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,3)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::ProjectionD3D((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2d*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,1))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::ProjectionD3D((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_ProjectionGL( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,3))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::ProjectionGL((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2d*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,3)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::ProjectionGL((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2d*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec2d_Check(((EPVec2d*)PyTuple_GetItem(tuple,1))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::ProjectionGL((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_Rot( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_Scale( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::Scale(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_Trn( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::Trn(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_Unit( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffined* ret = (EPAffined*)newEPAffined(Affined::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_clear( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_col( EPAffined* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec4d* ret = (EPVec4d*)newEPVec4d(self->ptr->col(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_det( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->det());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_height( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->height());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_inv( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffined* ret = (EPAffined*)newEPAffined(self->ptr->inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_resize( EPAffined* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_row( EPAffined* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec4d* ret = (EPVec4d*)newEPVec4d(self->ptr->row(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_trans( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffined* ret = (EPAffined*)newEPAffined(self->ptr->trans());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPAffined_width( EPAffined* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->width());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPAffined_methods[] =
{
	{"LookAt",(PyCFunction)EPAffined_LookAt,METH_VARARGS,"EPAffined::LookAt"},
	{"LookAtGL",(PyCFunction)EPAffined_LookAtGL,METH_VARARGS,"EPAffined::LookAtGL"},
	{"ProjectionD3D",(PyCFunction)EPAffined_ProjectionD3D,METH_VARARGS,"EPAffined::ProjectionD3D"},
	{"ProjectionGL",(PyCFunction)EPAffined_ProjectionGL,METH_VARARGS,"EPAffined::ProjectionGL"},
	{"Rot",(PyCFunction)EPAffined_Rot,METH_VARARGS,"EPAffined::Rot"},
	{"Scale",(PyCFunction)EPAffined_Scale,METH_VARARGS,"EPAffined::Scale"},
	{"Trn",(PyCFunction)EPAffined_Trn,METH_VARARGS,"EPAffined::Trn"},
	{"Unit",(PyCFunction)EPAffined_Unit,METH_NOARGS,"EPAffined::Unit"},
	{"clear",(PyCFunction)EPAffined_clear,METH_NOARGS,"EPAffined::clear"},
	{"col",(PyCFunction)EPAffined_col,METH_O,"EPAffined::col"},
	{"det",(PyCFunction)EPAffined_det,METH_NOARGS,"EPAffined::det"},
	{"height",(PyCFunction)EPAffined_height,METH_NOARGS,"EPAffined::height"},
	{"inv",(PyCFunction)EPAffined_inv,METH_NOARGS,"EPAffined::inv"},
	{"resize",(PyCFunction)EPAffined_resize,METH_VARARGS,"EPAffined::resize"},
	{"row",(PyCFunction)EPAffined_row,METH_O,"EPAffined::row"},
	{"trans",(PyCFunction)EPAffined_trans,METH_NOARGS,"EPAffined::trans"},
	{"width",(PyCFunction)EPAffined_width,METH_NOARGS,"EPAffined::width"},
	{NULL}
};
void EPAffined_dealloc(PyObject* self)
{
	delete ((EPAffined*)self)->ptr;
}
PyObject* EPAffined_str()
{
	return Py_BuildValue("s","This is EPAffined.");
}
int EPAffined_init(EPAffined* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPAffined* EPAffined_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPAffined *self;
	self = (EPAffined*) type->tp_alloc(type,0);
	if ( self != NULL ) EPAffined_init(self,args,kwds);
	return self;
}
PyTypeObject EPAffinedType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPAffined",/*tp_name*/
	sizeof(EPAffined),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPAffined_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPAffined_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Affined",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPAffined_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPAffined_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPAffined_new,/*tp_new*/
};
void initEPAffined(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPAffinedType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPAffinedType);
	PyModule_AddObject(m,"Affined",(PyObject *)&EPAffinedType);//モジュールに追加
}
PyObject* newEPAffined(Affined org)
{
	EPAffined *ret = EPAffined_new(&EPAffinedType,NULL,NULL);
	ret->ptr = new Affined();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPAffined(Affined* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPAffined *ret = EPAffined_new(&EPAffinedType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Affined

//{*********EPQuaterniond*******
PyObject* EPQuaterniond_AngularVelocity( EPQuaterniond* self,EPQuaterniond* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPQuaterniond_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->AngularVelocity((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Axis( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->Axis());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Conjugate( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Conjugate();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Conjugated( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->Conjugated());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Derivative( EPQuaterniond* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->Derivative((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Euler( EPQuaterniond* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->Euler(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_FromEuler( EPQuaterniond* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->FromEuler((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_FromMatrix( EPQuaterniond* self,EPMatrix3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPMatrix3d_Check(var1))
	{
		self->ptr->FromMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Inv( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->Inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Rot( EPQuaterniond* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(Quaterniond::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(Quaterniond::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(Quaterniond::Rot((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Rotation( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->Rotation());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_RotationArc( EPQuaterniond* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->RotationArc((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_RotationHalf( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->RotationHalf());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_Theta( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->Theta());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_ToEuler( EPQuaterniond* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->ToEuler((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_ToMatrix( EPQuaterniond* self,EPMatrix3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPMatrix3d_Check(var1))
	{
		self->ptr->ToMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_clear( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_norm( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_resize( EPQuaterniond* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_size( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_square( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_unit( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaterniond_unitize( EPQuaterniond* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPQuaterniond_methods[] =
{
	{"AngularVelocity",(PyCFunction)EPQuaterniond_AngularVelocity,METH_O,"EPQuaterniond::AngularVelocity"},
	{"Axis",(PyCFunction)EPQuaterniond_Axis,METH_NOARGS,"EPQuaterniond::Axis"},
	{"Conjugate",(PyCFunction)EPQuaterniond_Conjugate,METH_NOARGS,"EPQuaterniond::Conjugate"},
	{"Conjugated",(PyCFunction)EPQuaterniond_Conjugated,METH_NOARGS,"EPQuaterniond::Conjugated"},
	{"Derivative",(PyCFunction)EPQuaterniond_Derivative,METH_O,"EPQuaterniond::Derivative"},
	{"Euler",(PyCFunction)EPQuaterniond_Euler,METH_VARARGS,"EPQuaterniond::Euler"},
	{"FromEuler",(PyCFunction)EPQuaterniond_FromEuler,METH_O,"EPQuaterniond::FromEuler"},
	{"FromMatrix",(PyCFunction)EPQuaterniond_FromMatrix,METH_O,"EPQuaterniond::FromMatrix"},
	{"Inv",(PyCFunction)EPQuaterniond_Inv,METH_NOARGS,"EPQuaterniond::Inv"},
	{"Rot",(PyCFunction)EPQuaterniond_Rot,METH_VARARGS,"EPQuaterniond::Rot"},
	{"Rotation",(PyCFunction)EPQuaterniond_Rotation,METH_NOARGS,"EPQuaterniond::Rotation"},
	{"RotationArc",(PyCFunction)EPQuaterniond_RotationArc,METH_VARARGS,"EPQuaterniond::RotationArc"},
	{"RotationHalf",(PyCFunction)EPQuaterniond_RotationHalf,METH_NOARGS,"EPQuaterniond::RotationHalf"},
	{"Theta",(PyCFunction)EPQuaterniond_Theta,METH_NOARGS,"EPQuaterniond::Theta"},
	{"ToEuler",(PyCFunction)EPQuaterniond_ToEuler,METH_O,"EPQuaterniond::ToEuler"},
	{"ToMatrix",(PyCFunction)EPQuaterniond_ToMatrix,METH_O,"EPQuaterniond::ToMatrix"},
	{"clear",(PyCFunction)EPQuaterniond_clear,METH_NOARGS,"EPQuaterniond::clear"},
	{"norm",(PyCFunction)EPQuaterniond_norm,METH_NOARGS,"EPQuaterniond::norm"},
	{"resize",(PyCFunction)EPQuaterniond_resize,METH_O,"EPQuaterniond::resize"},
	{"size",(PyCFunction)EPQuaterniond_size,METH_NOARGS,"EPQuaterniond::size"},
	{"square",(PyCFunction)EPQuaterniond_square,METH_NOARGS,"EPQuaterniond::square"},
	{"unit",(PyCFunction)EPQuaterniond_unit,METH_NOARGS,"EPQuaterniond::unit"},
	{"unitize",(PyCFunction)EPQuaterniond_unitize,METH_NOARGS,"EPQuaterniond::unitize"},
	{NULL}
};
void EPQuaterniond_dealloc(PyObject* self)
{
	delete ((EPQuaterniond*)self)->ptr;
}
PyObject* EPQuaterniond_str()
{
	return Py_BuildValue("s","This is EPQuaterniond.");
}
int EPQuaterniond_init(EPQuaterniond* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPQuaterniond* EPQuaterniond_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPQuaterniond *self;
	self = (EPQuaterniond*) type->tp_alloc(type,0);
	if ( self != NULL ) EPQuaterniond_init(self,args,kwds);
	return self;
}
PyTypeObject EPQuaterniondType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPQuaterniond",/*tp_name*/
	sizeof(EPQuaterniond),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPQuaterniond_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPQuaterniond_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Quaterniond",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPQuaterniond_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPQuaterniond_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPQuaterniond_new,/*tp_new*/
};
void initEPQuaterniond(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPQuaterniondType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPQuaterniondType);
	PyModule_AddObject(m,"Quaterniond",(PyObject *)&EPQuaterniondType);//モジュールに追加
}
PyObject* newEPQuaterniond(Quaterniond org)
{
	EPQuaterniond *ret = EPQuaterniond_new(&EPQuaterniondType,NULL,NULL);
	ret->ptr = new Quaterniond();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPQuaterniond(Quaterniond* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPQuaterniond *ret = EPQuaterniond_new(&EPQuaterniondType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Quaterniond

//{*********EPQuaternionf*******
PyObject* EPQuaternionf_AngularVelocity( EPQuaternionf* self,EPQuaternionf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPQuaternionf_Check(var1))
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->AngularVelocity((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Axis( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->Axis());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Conjugate( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Conjugate();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Conjugated( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(self->ptr->Conjugated());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Derivative( EPQuaternionf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(self->ptr->Derivative((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Euler( EPQuaternionf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->Euler(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_FromEuler( EPQuaternionf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->FromEuler((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_FromMatrix( EPQuaternionf* self,EPMatrix3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPMatrix3d_Check(var1))
	{
		self->ptr->FromMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Inv( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(self->ptr->Inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Rot( EPQuaternionf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(Quaternionf::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(Quaternionf::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))))
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(Quaternionf::Rot((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Rotation( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->Rotation());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_RotationArc( EPQuaternionf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->RotationArc((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_RotationHalf( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->RotationHalf());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_Theta( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->Theta());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_ToEuler( EPQuaternionf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->ToEuler((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_ToMatrix( EPQuaternionf* self,EPMatrix3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPMatrix3d_Check(var1))
	{
		self->ptr->ToMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_clear( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_norm( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_resize( EPQuaternionf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_size( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_square( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_unit( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPQuaternionf_unitize( EPQuaternionf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPQuaternionf_methods[] =
{
	{"AngularVelocity",(PyCFunction)EPQuaternionf_AngularVelocity,METH_O,"EPQuaternionf::AngularVelocity"},
	{"Axis",(PyCFunction)EPQuaternionf_Axis,METH_NOARGS,"EPQuaternionf::Axis"},
	{"Conjugate",(PyCFunction)EPQuaternionf_Conjugate,METH_NOARGS,"EPQuaternionf::Conjugate"},
	{"Conjugated",(PyCFunction)EPQuaternionf_Conjugated,METH_NOARGS,"EPQuaternionf::Conjugated"},
	{"Derivative",(PyCFunction)EPQuaternionf_Derivative,METH_O,"EPQuaternionf::Derivative"},
	{"Euler",(PyCFunction)EPQuaternionf_Euler,METH_VARARGS,"EPQuaternionf::Euler"},
	{"FromEuler",(PyCFunction)EPQuaternionf_FromEuler,METH_O,"EPQuaternionf::FromEuler"},
	{"FromMatrix",(PyCFunction)EPQuaternionf_FromMatrix,METH_O,"EPQuaternionf::FromMatrix"},
	{"Inv",(PyCFunction)EPQuaternionf_Inv,METH_NOARGS,"EPQuaternionf::Inv"},
	{"Rot",(PyCFunction)EPQuaternionf_Rot,METH_VARARGS,"EPQuaternionf::Rot"},
	{"Rotation",(PyCFunction)EPQuaternionf_Rotation,METH_NOARGS,"EPQuaternionf::Rotation"},
	{"RotationArc",(PyCFunction)EPQuaternionf_RotationArc,METH_VARARGS,"EPQuaternionf::RotationArc"},
	{"RotationHalf",(PyCFunction)EPQuaternionf_RotationHalf,METH_NOARGS,"EPQuaternionf::RotationHalf"},
	{"Theta",(PyCFunction)EPQuaternionf_Theta,METH_NOARGS,"EPQuaternionf::Theta"},
	{"ToEuler",(PyCFunction)EPQuaternionf_ToEuler,METH_O,"EPQuaternionf::ToEuler"},
	{"ToMatrix",(PyCFunction)EPQuaternionf_ToMatrix,METH_O,"EPQuaternionf::ToMatrix"},
	{"clear",(PyCFunction)EPQuaternionf_clear,METH_NOARGS,"EPQuaternionf::clear"},
	{"norm",(PyCFunction)EPQuaternionf_norm,METH_NOARGS,"EPQuaternionf::norm"},
	{"resize",(PyCFunction)EPQuaternionf_resize,METH_O,"EPQuaternionf::resize"},
	{"size",(PyCFunction)EPQuaternionf_size,METH_NOARGS,"EPQuaternionf::size"},
	{"square",(PyCFunction)EPQuaternionf_square,METH_NOARGS,"EPQuaternionf::square"},
	{"unit",(PyCFunction)EPQuaternionf_unit,METH_NOARGS,"EPQuaternionf::unit"},
	{"unitize",(PyCFunction)EPQuaternionf_unitize,METH_NOARGS,"EPQuaternionf::unitize"},
	{NULL}
};
void EPQuaternionf_dealloc(PyObject* self)
{
	delete ((EPQuaternionf*)self)->ptr;
}
PyObject* EPQuaternionf_str()
{
	return Py_BuildValue("s","This is EPQuaternionf.");
}
int EPQuaternionf_init(EPQuaternionf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPQuaternionf* EPQuaternionf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPQuaternionf *self;
	self = (EPQuaternionf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPQuaternionf_init(self,args,kwds);
	return self;
}
PyTypeObject EPQuaternionfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPQuaternionf",/*tp_name*/
	sizeof(EPQuaternionf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPQuaternionf_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPQuaternionf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Quaternionf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPQuaternionf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPQuaternionf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPQuaternionf_new,/*tp_new*/
};
void initEPQuaternionf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPQuaternionfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPQuaternionfType);
	PyModule_AddObject(m,"Quaternionf",(PyObject *)&EPQuaternionfType);//モジュールに追加
}
PyObject* newEPQuaternionf(Quaternionf org)
{
	EPQuaternionf *ret = EPQuaternionf_new(&EPQuaternionfType,NULL,NULL);
	ret->ptr = new Quaternionf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPQuaternionf(Quaternionf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPQuaternionf *ret = EPQuaternionf_new(&EPQuaternionfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Quaternionf

//{*********EPPosed*******
PyObject* EPPosed_FromAffine( EPPosed* self,EPAffined* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffined_Check(var1))
	{
		self->ptr->FromAffine((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_Inv( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPosed* ret = (EPPosed*)newEPPosed(self->ptr->Inv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_Rot( EPPosed* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,3))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,3)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,1))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Rot(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPchar*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Rot((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPQuaterniond_Check(((EPQuaterniond*)PyTuple_GetItem(tuple,0))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Rot((*(((EPQuaterniond*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_ToAffine( EPPosed* self,EPAffined* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffined_Check(var1))
	{
		self->ptr->ToAffine((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_Trn( EPPosed* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Trn(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Trn((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_Unit( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPosed* ret = (EPPosed*)newEPPosed(Posed::Unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_clear( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_norm( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->norm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_resize( EPPosed* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->resize(PyLong_AsSsize_t((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_size( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->size());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_square( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->square());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_unit( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPosed* ret = (EPPosed*)newEPPosed(self->ptr->unit());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPosed_unitize( EPPosed* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->unitize();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPosed_methods[] =
{
	{"FromAffine",(PyCFunction)EPPosed_FromAffine,METH_O,"EPPosed::FromAffine"},
	{"Inv",(PyCFunction)EPPosed_Inv,METH_NOARGS,"EPPosed::Inv"},
	{"Rot",(PyCFunction)EPPosed_Rot,METH_VARARGS,"EPPosed::Rot"},
	{"ToAffine",(PyCFunction)EPPosed_ToAffine,METH_O,"EPPosed::ToAffine"},
	{"Trn",(PyCFunction)EPPosed_Trn,METH_VARARGS,"EPPosed::Trn"},
	{"Unit",(PyCFunction)EPPosed_Unit,METH_NOARGS,"EPPosed::Unit"},
	{"clear",(PyCFunction)EPPosed_clear,METH_NOARGS,"EPPosed::clear"},
	{"norm",(PyCFunction)EPPosed_norm,METH_NOARGS,"EPPosed::norm"},
	{"resize",(PyCFunction)EPPosed_resize,METH_O,"EPPosed::resize"},
	{"size",(PyCFunction)EPPosed_size,METH_NOARGS,"EPPosed::size"},
	{"square",(PyCFunction)EPPosed_square,METH_NOARGS,"EPPosed::square"},
	{"unit",(PyCFunction)EPPosed_unit,METH_NOARGS,"EPPosed::unit"},
	{"unitize",(PyCFunction)EPPosed_unitize,METH_NOARGS,"EPPosed::unitize"},
	{NULL}
};
void EPPosed_dealloc(PyObject* self)
{
	delete ((EPPosed*)self)->ptr;
}
PyObject* EPPosed_str()
{
	return Py_BuildValue("s","This is EPPosed.");
}
int EPPosed_init(EPPosed* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPosed* EPPosed_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPosed *self;
	self = (EPPosed*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPosed_init(self,args,kwds);
	return self;
}
PyTypeObject EPPosedType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Utility.EPPosed",/*tp_name*/
	sizeof(EPPosed),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPosed_dealloc,/*tp_dealloc*/
	0,/*tp_print*/
	0,/*tp_getattr*/
	0,/*tp_setattr*/
	0,/*tp_reserved*/
	0,/*tp_repr*/
	0,/*tp_as_number*/
	0,/*tp_as_sequence*/
	0,/*tp_as_mapping*/
	0,/*tp_hash*/
	0,/*tp_call*/
	(reprfunc)EPPosed_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"Posed",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPosed_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPosed_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPosed_new,/*tp_new*/
};
void initEPPosed(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPosedType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Utility");
	Py_INCREF(&EPPosedType);
	PyModule_AddObject(m,"Posed",(PyObject *)&EPPosedType);//モジュールに追加
}
PyObject* newEPPosed(Posed org)
{
	EPPosed *ret = EPPosed_new(&EPPosedType,NULL,NULL);
	ret->ptr = new Posed();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPosed(Posed* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPosed *ret = EPPosed_new(&EPPosedType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}Posed
/**************** for Module ******************/
void initUtility(void)
{
	initEPVec2f();
	initEPVec2d();
	initEPVec3f();
	initEPVec3d();
	initEPVec4f();
	initEPVec4d();
	initEPMatrix2f();
	initEPMatrix2d();
	initEPMatrix3f();
	initEPMatrix3d();
	initEPAffine2f();
	initEPAffinef();
	initEPAffine2d();
	initEPAffined();
	initEPQuaterniond();
	initEPQuaternionf();
	initEPPosed();
}
