#include "../../include\EmbPython\SprEPGraphics.h" 

//{*********EPGRBlendMeshDesc*******
static PyMethodDef EPGRBlendMeshDesc_methods[] =
{
	{NULL}
};
void EPGRBlendMeshDesc_dealloc(PyObject* self)
{
	delete ((EPGRBlendMeshDesc*)self)->ptr;
}
PyObject* EPGRBlendMeshDesc_str()
{
	return Py_BuildValue("s","This is EPGRBlendMeshDesc.");
}
int EPGRBlendMeshDesc_init(EPGRBlendMeshDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRBlendMeshDesc* EPGRBlendMeshDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRBlendMeshDesc *self;
	self = (EPGRBlendMeshDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRBlendMeshDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRBlendMeshDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRBlendMeshDesc",/*tp_name*/
	sizeof(EPGRBlendMeshDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRBlendMeshDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRBlendMeshDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRBlendMeshDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRBlendMeshDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRBlendMeshDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRBlendMeshDesc_new,/*tp_new*/
};
void initEPGRBlendMeshDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRBlendMeshDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRBlendMeshDescType);
	PyModule_AddObject(m,"GRBlendMeshDesc",(PyObject *)&EPGRBlendMeshDescType);//モジュールに追加
}
PyObject* newEPGRBlendMeshDesc(struct GRBlendMeshDesc org)
{
	EPGRBlendMeshDesc *ret = EPGRBlendMeshDesc_new(&EPGRBlendMeshDescType,NULL,NULL);
	ret->ptr = new GRBlendMeshDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRBlendMeshDesc(struct GRBlendMeshDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRBlendMeshDesc *ret = EPGRBlendMeshDesc_new(&EPGRBlendMeshDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRBlendMeshDesc

//{*********EPGRBlendMeshIf*******
static PyMethodDef EPGRBlendMeshIf_methods[] =
{
	{NULL}
};
void EPGRBlendMeshIf_dealloc(PyObject* self)
{
}
PyObject* EPGRBlendMeshIf_str()
{
	return Py_BuildValue("s","This is EPGRBlendMeshIf.");
}
int EPGRBlendMeshIf_init(EPGRBlendMeshIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRBlendMeshIf* EPGRBlendMeshIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRBlendMeshIf *self;
	self = (EPGRBlendMeshIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRBlendMeshIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRBlendMeshIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRBlendMeshIf",/*tp_name*/
	sizeof(EPGRBlendMeshIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRBlendMeshIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRBlendMeshIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRBlendMeshIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRBlendMeshIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRBlendMeshIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRBlendMeshIf_new,/*tp_new*/
};
void initEPGRBlendMeshIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRBlendMeshIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRBlendMeshIfType);
	PyModule_AddObject(m,"GRBlendMeshIf",(PyObject *)&EPGRBlendMeshIfType);//モジュールに追加
}
PyObject* newEPGRBlendMeshIf(struct GRBlendMeshIf org)
{
	EPGRBlendMeshIf *ret = EPGRBlendMeshIf_new(&EPGRBlendMeshIfType,NULL,NULL);
	ret->ptr = new GRBlendMeshIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRBlendMeshIf(struct GRBlendMeshIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRBlendMeshIf *ret = EPGRBlendMeshIf_new(&EPGRBlendMeshIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRBlendMeshIf

//{*********EPGRVisualDesc*******
static PyMethodDef EPGRVisualDesc_methods[] =
{
	{NULL}
};
void EPGRVisualDesc_dealloc(PyObject* self)
{
	delete ((EPGRVisualDesc*)self)->ptr;
}
PyObject* EPGRVisualDesc_str()
{
	return Py_BuildValue("s","This is EPGRVisualDesc.");
}
int EPGRVisualDesc_init(EPGRVisualDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRVisualDesc* EPGRVisualDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRVisualDesc *self;
	self = (EPGRVisualDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRVisualDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRVisualDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRVisualDesc",/*tp_name*/
	sizeof(EPGRVisualDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRVisualDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRVisualDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRVisualDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRVisualDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRVisualDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRVisualDesc_new,/*tp_new*/
};
void initEPGRVisualDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRVisualDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRVisualDescType);
	PyModule_AddObject(m,"GRVisualDesc",(PyObject *)&EPGRVisualDescType);//モジュールに追加
}
PyObject* newEPGRVisualDesc(struct GRVisualDesc org)
{
	EPGRVisualDesc *ret = EPGRVisualDesc_new(&EPGRVisualDescType,NULL,NULL);
	ret->ptr = new GRVisualDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRVisualDesc(struct GRVisualDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRVisualDesc *ret = EPGRVisualDesc_new(&EPGRVisualDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRVisualDesc

//{*********EPGRVisualIf*******
PyObject* EPGRVisualIf_Render( EPGRVisualIf* self,EPGRRenderIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRRenderIf_Check(var1))
	{
		self->ptr->Render((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVisualIf_Rendered( EPGRVisualIf* self,EPGRRenderIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRRenderIf_Check(var1))
	{
		self->ptr->Rendered((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRVisualIf_methods[] =
{
	{"Render",(PyCFunction)EPGRVisualIf_Render,METH_O,"EPGRVisualIf::Render"},
	{"Rendered",(PyCFunction)EPGRVisualIf_Rendered,METH_O,"EPGRVisualIf::Rendered"},
	{NULL}
};
void EPGRVisualIf_dealloc(PyObject* self)
{
}
PyObject* EPGRVisualIf_str()
{
	return Py_BuildValue("s","This is EPGRVisualIf.");
}
int EPGRVisualIf_init(EPGRVisualIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRVisualIf* EPGRVisualIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRVisualIf *self;
	self = (EPGRVisualIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRVisualIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRVisualIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRVisualIf",/*tp_name*/
	sizeof(EPGRVisualIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRVisualIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRVisualIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRVisualIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRVisualIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRVisualIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRVisualIf_new,/*tp_new*/
};
void initEPGRVisualIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRVisualIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRVisualIfType);
	PyModule_AddObject(m,"GRVisualIf",(PyObject *)&EPGRVisualIfType);//モジュールに追加
}
PyObject* newEPGRVisualIf(struct GRVisualIf org)
{
	EPGRVisualIf *ret = EPGRVisualIf_new(&EPGRVisualIfType,NULL,NULL);
	ret->ptr = new GRVisualIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRVisualIf(struct GRVisualIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRVisualIf *ret = EPGRVisualIf_new(&EPGRVisualIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRVisualIf

//{*********EPGRFrameTransformMatrix*******
static PyMethodDef EPGRFrameTransformMatrix_methods[] =
{
	{NULL}
};
void EPGRFrameTransformMatrix_dealloc(PyObject* self)
{
	delete ((EPGRFrameTransformMatrix*)self)->ptr;
}
PyObject* EPGRFrameTransformMatrix_str()
{
	return Py_BuildValue("s","This is EPGRFrameTransformMatrix.");
}
int EPGRFrameTransformMatrix_init(EPGRFrameTransformMatrix* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRFrameTransformMatrix* EPGRFrameTransformMatrix_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRFrameTransformMatrix *self;
	self = (EPGRFrameTransformMatrix*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRFrameTransformMatrix_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRFrameTransformMatrixType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRFrameTransformMatrix",/*tp_name*/
	sizeof(EPGRFrameTransformMatrix),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRFrameTransformMatrix_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRFrameTransformMatrix_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRFrameTransformMatrix",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRFrameTransformMatrix_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRFrameTransformMatrix_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRFrameTransformMatrix_new,/*tp_new*/
};
void initEPGRFrameTransformMatrix(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRFrameTransformMatrixType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRFrameTransformMatrixType);
	PyModule_AddObject(m,"GRFrameTransformMatrix",(PyObject *)&EPGRFrameTransformMatrixType);//モジュールに追加
}
PyObject* newEPGRFrameTransformMatrix(struct GRFrameTransformMatrix org)
{
	EPGRFrameTransformMatrix *ret = EPGRFrameTransformMatrix_new(&EPGRFrameTransformMatrixType,NULL,NULL);
	ret->ptr = new GRFrameTransformMatrix();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRFrameTransformMatrix(struct GRFrameTransformMatrix* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRFrameTransformMatrix *ret = EPGRFrameTransformMatrix_new(&EPGRFrameTransformMatrixType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRFrameTransformMatrix

//{*********EPGRFrameIf*******
PyObject* EPGRFrameIf_Clear( EPGRFrameIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_GetParent( EPGRFrameIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRFrameIf* ret = (EPGRFrameIf*)newEPGRFrameIf(self->ptr->GetParent());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_GetTransform( EPGRFrameIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(self->ptr->GetTransform());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_GetWorldTransform( EPGRFrameIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPAffinef* ret = (EPAffinef*)newEPAffinef(self->ptr->GetWorldTransform());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_NChildren( EPGRFrameIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NChildren());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_Print( EPGRFrameIf* self,EPostream* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPostream_Check(var1))
	{
		self->ptr->Print((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_SetParent( EPGRFrameIf* self,EPGRFrameIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRFrameIf_Check(var1))
	{
		self->ptr->SetParent((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRFrameIf_SetTransform( EPGRFrameIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->SetTransform((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRFrameIf_methods[] =
{
	{"Clear",(PyCFunction)EPGRFrameIf_Clear,METH_NOARGS,"EPGRFrameIf::Clear"},
	{"GetParent",(PyCFunction)EPGRFrameIf_GetParent,METH_NOARGS,"EPGRFrameIf::GetParent"},
	{"GetTransform",(PyCFunction)EPGRFrameIf_GetTransform,METH_NOARGS,"EPGRFrameIf::GetTransform"},
	{"GetWorldTransform",(PyCFunction)EPGRFrameIf_GetWorldTransform,METH_NOARGS,"EPGRFrameIf::GetWorldTransform"},
	{"NChildren",(PyCFunction)EPGRFrameIf_NChildren,METH_NOARGS,"EPGRFrameIf::NChildren"},
	{"Print",(PyCFunction)EPGRFrameIf_Print,METH_O,"EPGRFrameIf::Print"},
	{"SetParent",(PyCFunction)EPGRFrameIf_SetParent,METH_O,"EPGRFrameIf::SetParent"},
	{"SetTransform",(PyCFunction)EPGRFrameIf_SetTransform,METH_O,"EPGRFrameIf::SetTransform"},
	{NULL}
};
void EPGRFrameIf_dealloc(PyObject* self)
{
}
PyObject* EPGRFrameIf_str()
{
	return Py_BuildValue("s","This is EPGRFrameIf.");
}
int EPGRFrameIf_init(EPGRFrameIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRFrameIf* EPGRFrameIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRFrameIf *self;
	self = (EPGRFrameIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRFrameIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRFrameIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRFrameIf",/*tp_name*/
	sizeof(EPGRFrameIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRFrameIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRFrameIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRFrameIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRFrameIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRFrameIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRFrameIf_new,/*tp_new*/
};
void initEPGRFrameIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRFrameIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRFrameIfType);
	PyModule_AddObject(m,"GRFrameIf",(PyObject *)&EPGRFrameIfType);//モジュールに追加
}
PyObject* newEPGRFrameIf(struct GRFrameIf org)
{
	EPGRFrameIf *ret = EPGRFrameIf_new(&EPGRFrameIfType,NULL,NULL);
	ret->ptr = new GRFrameIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRFrameIf(struct GRFrameIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRFrameIf *ret = EPGRFrameIf_new(&EPGRFrameIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRFrameIf

//{*********EPGRFrameDesc*******
static PyMethodDef EPGRFrameDesc_methods[] =
{
	{NULL}
};
void EPGRFrameDesc_dealloc(PyObject* self)
{
	delete ((EPGRFrameDesc*)self)->ptr;
}
PyObject* EPGRFrameDesc_str()
{
	return Py_BuildValue("s","This is EPGRFrameDesc.");
}
int EPGRFrameDesc_init(EPGRFrameDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRFrameDesc* EPGRFrameDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRFrameDesc *self;
	self = (EPGRFrameDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRFrameDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRFrameDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRFrameDesc",/*tp_name*/
	sizeof(EPGRFrameDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRFrameDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRFrameDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRFrameDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRFrameDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRFrameDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRFrameDesc_new,/*tp_new*/
};
void initEPGRFrameDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRFrameDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRFrameDescType);
	PyModule_AddObject(m,"GRFrameDesc",(PyObject *)&EPGRFrameDescType);//モジュールに追加
}
PyObject* newEPGRFrameDesc(struct GRFrameDesc org)
{
	EPGRFrameDesc *ret = EPGRFrameDesc_new(&EPGRFrameDescType,NULL,NULL);
	ret->ptr = new GRFrameDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRFrameDesc(struct GRFrameDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRFrameDesc *ret = EPGRFrameDesc_new(&EPGRFrameDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRFrameDesc

//{*********EPGRDummyFrameIf*******
static PyMethodDef EPGRDummyFrameIf_methods[] =
{
	{NULL}
};
void EPGRDummyFrameIf_dealloc(PyObject* self)
{
}
PyObject* EPGRDummyFrameIf_str()
{
	return Py_BuildValue("s","This is EPGRDummyFrameIf.");
}
int EPGRDummyFrameIf_init(EPGRDummyFrameIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRDummyFrameIf* EPGRDummyFrameIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRDummyFrameIf *self;
	self = (EPGRDummyFrameIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRDummyFrameIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRDummyFrameIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRDummyFrameIf",/*tp_name*/
	sizeof(EPGRDummyFrameIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRDummyFrameIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRDummyFrameIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRDummyFrameIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRDummyFrameIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRDummyFrameIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRDummyFrameIf_new,/*tp_new*/
};
void initEPGRDummyFrameIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRDummyFrameIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRDummyFrameIfType);
	PyModule_AddObject(m,"GRDummyFrameIf",(PyObject *)&EPGRDummyFrameIfType);//モジュールに追加
}
PyObject* newEPGRDummyFrameIf(struct GRDummyFrameIf org)
{
	EPGRDummyFrameIf *ret = EPGRDummyFrameIf_new(&EPGRDummyFrameIfType,NULL,NULL);
	ret->ptr = new GRDummyFrameIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRDummyFrameIf(struct GRDummyFrameIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRDummyFrameIf *ret = EPGRDummyFrameIf_new(&EPGRDummyFrameIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRDummyFrameIf

//{*********EPGRDummyFrameDesc*******
static PyMethodDef EPGRDummyFrameDesc_methods[] =
{
	{NULL}
};
void EPGRDummyFrameDesc_dealloc(PyObject* self)
{
	delete ((EPGRDummyFrameDesc*)self)->ptr;
}
PyObject* EPGRDummyFrameDesc_str()
{
	return Py_BuildValue("s","This is EPGRDummyFrameDesc.");
}
int EPGRDummyFrameDesc_init(EPGRDummyFrameDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRDummyFrameDesc* EPGRDummyFrameDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRDummyFrameDesc *self;
	self = (EPGRDummyFrameDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRDummyFrameDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRDummyFrameDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRDummyFrameDesc",/*tp_name*/
	sizeof(EPGRDummyFrameDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRDummyFrameDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRDummyFrameDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRDummyFrameDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRDummyFrameDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRDummyFrameDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRDummyFrameDesc_new,/*tp_new*/
};
void initEPGRDummyFrameDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRDummyFrameDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRDummyFrameDescType);
	PyModule_AddObject(m,"GRDummyFrameDesc",(PyObject *)&EPGRDummyFrameDescType);//モジュールに追加
}
PyObject* newEPGRDummyFrameDesc(struct GRDummyFrameDesc org)
{
	EPGRDummyFrameDesc *ret = EPGRDummyFrameDesc_new(&EPGRDummyFrameDescType,NULL,NULL);
	ret->ptr = new GRDummyFrameDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRDummyFrameDesc(struct GRDummyFrameDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRDummyFrameDesc *ret = EPGRDummyFrameDesc_new(&EPGRDummyFrameDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRDummyFrameDesc

//{*********EPGRKey*******
static PyMethodDef EPGRKey_methods[] =
{
	{NULL}
};
void EPGRKey_dealloc(PyObject* self)
{
	delete ((EPGRKey*)self)->ptr;
}
PyObject* EPGRKey_str()
{
	return Py_BuildValue("s","This is EPGRKey.");
}
int EPGRKey_init(EPGRKey* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRKey* EPGRKey_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRKey *self;
	self = (EPGRKey*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRKey_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRKeyType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRKey",/*tp_name*/
	sizeof(EPGRKey),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRKey_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRKey_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRKey",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRKey_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRKey_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRKey_new,/*tp_new*/
};
void initEPGRKey(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRKeyType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRKeyType);
	PyModule_AddObject(m,"GRKey",(PyObject *)&EPGRKeyType);//モジュールに追加
}
PyObject* newEPGRKey(struct GRKey org)
{
	EPGRKey *ret = EPGRKey_new(&EPGRKeyType,NULL,NULL);
	ret->ptr = new GRKey();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRKey(struct GRKey* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRKey *ret = EPGRKey_new(&EPGRKeyType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRKey

//{*********EPGRAnimationKey*******
static PyMethodDef EPGRAnimationKey_methods[] =
{
	{NULL}
};
void EPGRAnimationKey_dealloc(PyObject* self)
{
	delete ((EPGRAnimationKey*)self)->ptr;
}
PyObject* EPGRAnimationKey_str()
{
	return Py_BuildValue("s","This is EPGRAnimationKey.");
}
int EPGRAnimationKey_init(EPGRAnimationKey* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationKey* EPGRAnimationKey_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationKey *self;
	self = (EPGRAnimationKey*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationKey_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationKeyType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationKey",/*tp_name*/
	sizeof(EPGRAnimationKey),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationKey_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationKey_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationKey",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationKey_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationKey_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationKey_new,/*tp_new*/
};
void initEPGRAnimationKey(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationKeyType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationKeyType);
	PyModule_AddObject(m,"GRAnimationKey",(PyObject *)&EPGRAnimationKeyType);//モジュールに追加
}
PyObject* newEPGRAnimationKey(struct GRAnimationKey org)
{
	EPGRAnimationKey *ret = EPGRAnimationKey_new(&EPGRAnimationKeyType,NULL,NULL);
	ret->ptr = new GRAnimationKey();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationKey(struct GRAnimationKey* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationKey *ret = EPGRAnimationKey_new(&EPGRAnimationKeyType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationKey

//{*********EPGRAnimationIf*******
PyObject* EPGRAnimationIf_BlendPose( EPGRAnimationIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->BlendPose(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationIf_DeletePose( EPGRAnimationIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->DeletePose(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationIf_GetAnimationKey( EPGRAnimationIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRAnimationKey* ret = (EPGRAnimationKey*)newEPGRAnimationKey(self->ptr->GetAnimationKey(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationIf_LoadInitialPose( EPGRAnimationIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->LoadInitialPose();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationIf_NAnimationKey( EPGRAnimationIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NAnimationKey());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationIf_ResetPose( EPGRAnimationIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ResetPose();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRAnimationIf_methods[] =
{
	{"BlendPose",(PyCFunction)EPGRAnimationIf_BlendPose,METH_VARARGS,"EPGRAnimationIf::BlendPose"},
	{"DeletePose",(PyCFunction)EPGRAnimationIf_DeletePose,METH_O,"EPGRAnimationIf::DeletePose"},
	{"GetAnimationKey",(PyCFunction)EPGRAnimationIf_GetAnimationKey,METH_O,"EPGRAnimationIf::GetAnimationKey"},
	{"LoadInitialPose",(PyCFunction)EPGRAnimationIf_LoadInitialPose,METH_NOARGS,"EPGRAnimationIf::LoadInitialPose"},
	{"NAnimationKey",(PyCFunction)EPGRAnimationIf_NAnimationKey,METH_NOARGS,"EPGRAnimationIf::NAnimationKey"},
	{"ResetPose",(PyCFunction)EPGRAnimationIf_ResetPose,METH_NOARGS,"EPGRAnimationIf::ResetPose"},
	{NULL}
};
void EPGRAnimationIf_dealloc(PyObject* self)
{
}
PyObject* EPGRAnimationIf_str()
{
	return Py_BuildValue("s","This is EPGRAnimationIf.");
}
int EPGRAnimationIf_init(EPGRAnimationIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationIf* EPGRAnimationIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationIf *self;
	self = (EPGRAnimationIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationIf",/*tp_name*/
	sizeof(EPGRAnimationIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationIf_new,/*tp_new*/
};
void initEPGRAnimationIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationIfType);
	PyModule_AddObject(m,"GRAnimationIf",(PyObject *)&EPGRAnimationIfType);//モジュールに追加
}
PyObject* newEPGRAnimationIf(struct GRAnimationIf org)
{
	EPGRAnimationIf *ret = EPGRAnimationIf_new(&EPGRAnimationIfType,NULL,NULL);
	ret->ptr = new GRAnimationIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationIf(struct GRAnimationIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationIf *ret = EPGRAnimationIf_new(&EPGRAnimationIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationIf

//{*********EPGRAnimationDesc*******
static PyMethodDef EPGRAnimationDesc_methods[] =
{
	{NULL}
};
void EPGRAnimationDesc_dealloc(PyObject* self)
{
	delete ((EPGRAnimationDesc*)self)->ptr;
}
PyObject* EPGRAnimationDesc_str()
{
	return Py_BuildValue("s","This is EPGRAnimationDesc.");
}
int EPGRAnimationDesc_init(EPGRAnimationDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationDesc* EPGRAnimationDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationDesc *self;
	self = (EPGRAnimationDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationDesc",/*tp_name*/
	sizeof(EPGRAnimationDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationDesc_new,/*tp_new*/
};
void initEPGRAnimationDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationDescType);
	PyModule_AddObject(m,"GRAnimationDesc",(PyObject *)&EPGRAnimationDescType);//モジュールに追加
}
PyObject* newEPGRAnimationDesc(struct GRAnimationDesc org)
{
	EPGRAnimationDesc *ret = EPGRAnimationDesc_new(&EPGRAnimationDescType,NULL,NULL);
	ret->ptr = new GRAnimationDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationDesc(struct GRAnimationDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationDesc *ret = EPGRAnimationDesc_new(&EPGRAnimationDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationDesc

//{*********EPGRAnimationSetIf*******
PyObject* EPGRAnimationSetIf_BlendPose( EPGRAnimationSetIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->BlendPose(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationSetIf_DeleteAnimationPose( EPGRAnimationSetIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->DeleteAnimationPose(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationSetIf_GetChildObject( EPGRAnimationSetIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->GetChildObject(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationSetIf_GetLastKeyTime( EPGRAnimationSetIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetLastKeyTime());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationSetIf_LoadInitialPose( EPGRAnimationSetIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->LoadInitialPose();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationSetIf_ResetPose( EPGRAnimationSetIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ResetPose();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationSetIf_SetCurrentAnimationPose( EPGRAnimationSetIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetCurrentAnimationPose(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRAnimationSetIf_methods[] =
{
	{"BlendPose",(PyCFunction)EPGRAnimationSetIf_BlendPose,METH_VARARGS,"EPGRAnimationSetIf::BlendPose"},
	{"DeleteAnimationPose",(PyCFunction)EPGRAnimationSetIf_DeleteAnimationPose,METH_O,"EPGRAnimationSetIf::DeleteAnimationPose"},
	{"GetChildObject",(PyCFunction)EPGRAnimationSetIf_GetChildObject,METH_O,"EPGRAnimationSetIf::GetChildObject"},
	{"GetLastKeyTime",(PyCFunction)EPGRAnimationSetIf_GetLastKeyTime,METH_NOARGS,"EPGRAnimationSetIf::GetLastKeyTime"},
	{"LoadInitialPose",(PyCFunction)EPGRAnimationSetIf_LoadInitialPose,METH_NOARGS,"EPGRAnimationSetIf::LoadInitialPose"},
	{"ResetPose",(PyCFunction)EPGRAnimationSetIf_ResetPose,METH_NOARGS,"EPGRAnimationSetIf::ResetPose"},
	{"SetCurrentAnimationPose",(PyCFunction)EPGRAnimationSetIf_SetCurrentAnimationPose,METH_O,"EPGRAnimationSetIf::SetCurrentAnimationPose"},
	{NULL}
};
void EPGRAnimationSetIf_dealloc(PyObject* self)
{
}
PyObject* EPGRAnimationSetIf_str()
{
	return Py_BuildValue("s","This is EPGRAnimationSetIf.");
}
int EPGRAnimationSetIf_init(EPGRAnimationSetIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationSetIf* EPGRAnimationSetIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationSetIf *self;
	self = (EPGRAnimationSetIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationSetIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationSetIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationSetIf",/*tp_name*/
	sizeof(EPGRAnimationSetIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationSetIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationSetIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationSetIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationSetIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationSetIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationSetIf_new,/*tp_new*/
};
void initEPGRAnimationSetIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationSetIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationSetIfType);
	PyModule_AddObject(m,"GRAnimationSetIf",(PyObject *)&EPGRAnimationSetIfType);//モジュールに追加
}
PyObject* newEPGRAnimationSetIf(struct GRAnimationSetIf org)
{
	EPGRAnimationSetIf *ret = EPGRAnimationSetIf_new(&EPGRAnimationSetIfType,NULL,NULL);
	ret->ptr = new GRAnimationSetIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationSetIf(struct GRAnimationSetIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationSetIf *ret = EPGRAnimationSetIf_new(&EPGRAnimationSetIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationSetIf

//{*********EPGRAnimationSetDesc*******
static PyMethodDef EPGRAnimationSetDesc_methods[] =
{
	{NULL}
};
void EPGRAnimationSetDesc_dealloc(PyObject* self)
{
	delete ((EPGRAnimationSetDesc*)self)->ptr;
}
PyObject* EPGRAnimationSetDesc_str()
{
	return Py_BuildValue("s","This is EPGRAnimationSetDesc.");
}
int EPGRAnimationSetDesc_init(EPGRAnimationSetDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationSetDesc* EPGRAnimationSetDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationSetDesc *self;
	self = (EPGRAnimationSetDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationSetDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationSetDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationSetDesc",/*tp_name*/
	sizeof(EPGRAnimationSetDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationSetDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationSetDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationSetDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationSetDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationSetDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationSetDesc_new,/*tp_new*/
};
void initEPGRAnimationSetDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationSetDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationSetDescType);
	PyModule_AddObject(m,"GRAnimationSetDesc",(PyObject *)&EPGRAnimationSetDescType);//モジュールに追加
}
PyObject* newEPGRAnimationSetDesc(struct GRAnimationSetDesc org)
{
	EPGRAnimationSetDesc *ret = EPGRAnimationSetDesc_new(&EPGRAnimationSetDescType,NULL,NULL);
	ret->ptr = new GRAnimationSetDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationSetDesc(struct GRAnimationSetDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationSetDesc *ret = EPGRAnimationSetDesc_new(&EPGRAnimationSetDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationSetDesc

//{*********EPGRAnimationControllerIf*******
PyObject* EPGRAnimationControllerIf_AddChildObject( EPGRAnimationControllerIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->AddChildObject((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_BlendPose( EPGRAnimationControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPUTString_Check(((EPUTString*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->BlendPose((*(((EPUTString*)PyTuple_GetItem(tuple,0))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_DelChildObject( EPGRAnimationControllerIf* self,EPObjectIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPObjectIf_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->DelChildObject((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_GetAnimationSet( EPGRAnimationControllerIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRAnimationSetIf* ret = (EPGRAnimationSetIf*)newEPGRAnimationSetIf(self->ptr->GetAnimationSet(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_GetChildObject( EPGRAnimationControllerIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPObjectIf* ret = (EPObjectIf*)newEPObjectIf(self->ptr->GetChildObject(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_LoadInitialPose( EPGRAnimationControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->LoadInitialPose();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_NChildObject( EPGRAnimationControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NChildObject());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRAnimationControllerIf_ResetPose( EPGRAnimationControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ResetPose();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRAnimationControllerIf_methods[] =
{
	{"AddChildObject",(PyCFunction)EPGRAnimationControllerIf_AddChildObject,METH_O,"EPGRAnimationControllerIf::AddChildObject"},
	{"BlendPose",(PyCFunction)EPGRAnimationControllerIf_BlendPose,METH_VARARGS,"EPGRAnimationControllerIf::BlendPose"},
	{"DelChildObject",(PyCFunction)EPGRAnimationControllerIf_DelChildObject,METH_O,"EPGRAnimationControllerIf::DelChildObject"},
	{"GetAnimationSet",(PyCFunction)EPGRAnimationControllerIf_GetAnimationSet,METH_O,"EPGRAnimationControllerIf::GetAnimationSet"},
	{"GetChildObject",(PyCFunction)EPGRAnimationControllerIf_GetChildObject,METH_O,"EPGRAnimationControllerIf::GetChildObject"},
	{"LoadInitialPose",(PyCFunction)EPGRAnimationControllerIf_LoadInitialPose,METH_NOARGS,"EPGRAnimationControllerIf::LoadInitialPose"},
	{"NChildObject",(PyCFunction)EPGRAnimationControllerIf_NChildObject,METH_NOARGS,"EPGRAnimationControllerIf::NChildObject"},
	{"ResetPose",(PyCFunction)EPGRAnimationControllerIf_ResetPose,METH_NOARGS,"EPGRAnimationControllerIf::ResetPose"},
	{NULL}
};
void EPGRAnimationControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPGRAnimationControllerIf_str()
{
	return Py_BuildValue("s","This is EPGRAnimationControllerIf.");
}
int EPGRAnimationControllerIf_init(EPGRAnimationControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationControllerIf* EPGRAnimationControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationControllerIf *self;
	self = (EPGRAnimationControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationControllerIf",/*tp_name*/
	sizeof(EPGRAnimationControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationControllerIf_new,/*tp_new*/
};
void initEPGRAnimationControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationControllerIfType);
	PyModule_AddObject(m,"GRAnimationControllerIf",(PyObject *)&EPGRAnimationControllerIfType);//モジュールに追加
}
PyObject* newEPGRAnimationControllerIf(struct GRAnimationControllerIf org)
{
	EPGRAnimationControllerIf *ret = EPGRAnimationControllerIf_new(&EPGRAnimationControllerIfType,NULL,NULL);
	ret->ptr = new GRAnimationControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationControllerIf(struct GRAnimationControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationControllerIf *ret = EPGRAnimationControllerIf_new(&EPGRAnimationControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationControllerIf

//{*********EPGRAnimationControllerDesc*******
static PyMethodDef EPGRAnimationControllerDesc_methods[] =
{
	{NULL}
};
void EPGRAnimationControllerDesc_dealloc(PyObject* self)
{
	delete ((EPGRAnimationControllerDesc*)self)->ptr;
}
PyObject* EPGRAnimationControllerDesc_str()
{
	return Py_BuildValue("s","This is EPGRAnimationControllerDesc.");
}
int EPGRAnimationControllerDesc_init(EPGRAnimationControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRAnimationControllerDesc* EPGRAnimationControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRAnimationControllerDesc *self;
	self = (EPGRAnimationControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRAnimationControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRAnimationControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRAnimationControllerDesc",/*tp_name*/
	sizeof(EPGRAnimationControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRAnimationControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRAnimationControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRAnimationControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRAnimationControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRAnimationControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRAnimationControllerDesc_new,/*tp_new*/
};
void initEPGRAnimationControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRAnimationControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRAnimationControllerDescType);
	PyModule_AddObject(m,"GRAnimationControllerDesc",(PyObject *)&EPGRAnimationControllerDescType);//モジュールに追加
}
PyObject* newEPGRAnimationControllerDesc(struct GRAnimationControllerDesc org)
{
	EPGRAnimationControllerDesc *ret = EPGRAnimationControllerDesc_new(&EPGRAnimationControllerDescType,NULL,NULL);
	ret->ptr = new GRAnimationControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRAnimationControllerDesc(struct GRAnimationControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRAnimationControllerDesc *ret = EPGRAnimationControllerDesc_new(&EPGRAnimationControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRAnimationControllerDesc

//{*********EPGRMeshFace*******
static PyMethodDef EPGRMeshFace_methods[] =
{
	{NULL}
};
void EPGRMeshFace_dealloc(PyObject* self)
{
	delete ((EPGRMeshFace*)self)->ptr;
}
PyObject* EPGRMeshFace_str()
{
	return Py_BuildValue("s","This is EPGRMeshFace.");
}
int EPGRMeshFace_init(EPGRMeshFace* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRMeshFace* EPGRMeshFace_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRMeshFace *self;
	self = (EPGRMeshFace*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRMeshFace_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRMeshFaceType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRMeshFace",/*tp_name*/
	sizeof(EPGRMeshFace),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRMeshFace_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRMeshFace_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRMeshFace",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRMeshFace_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRMeshFace_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRMeshFace_new,/*tp_new*/
};
void initEPGRMeshFace(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRMeshFaceType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRMeshFaceType);
	PyModule_AddObject(m,"GRMeshFace",(PyObject *)&EPGRMeshFaceType);//モジュールに追加
}
PyObject* newEPGRMeshFace(struct GRMeshFace org)
{
	EPGRMeshFace *ret = EPGRMeshFace_new(&EPGRMeshFaceType,NULL,NULL);
	ret->ptr = new GRMeshFace();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRMeshFace(struct GRMeshFace* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRMeshFace *ret = EPGRMeshFace_new(&EPGRMeshFaceType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRMeshFace

//{*********EPGRSkinWeightIf*******
static PyMethodDef EPGRSkinWeightIf_methods[] =
{
	{NULL}
};
void EPGRSkinWeightIf_dealloc(PyObject* self)
{
}
PyObject* EPGRSkinWeightIf_str()
{
	return Py_BuildValue("s","This is EPGRSkinWeightIf.");
}
int EPGRSkinWeightIf_init(EPGRSkinWeightIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSkinWeightIf* EPGRSkinWeightIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSkinWeightIf *self;
	self = (EPGRSkinWeightIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSkinWeightIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSkinWeightIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSkinWeightIf",/*tp_name*/
	sizeof(EPGRSkinWeightIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSkinWeightIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSkinWeightIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSkinWeightIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSkinWeightIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSkinWeightIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSkinWeightIf_new,/*tp_new*/
};
void initEPGRSkinWeightIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSkinWeightIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSkinWeightIfType);
	PyModule_AddObject(m,"GRSkinWeightIf",(PyObject *)&EPGRSkinWeightIfType);//モジュールに追加
}
PyObject* newEPGRSkinWeightIf(struct GRSkinWeightIf org)
{
	EPGRSkinWeightIf *ret = EPGRSkinWeightIf_new(&EPGRSkinWeightIfType,NULL,NULL);
	ret->ptr = new GRSkinWeightIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSkinWeightIf(struct GRSkinWeightIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSkinWeightIf *ret = EPGRSkinWeightIf_new(&EPGRSkinWeightIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSkinWeightIf

//{*********EPGRSkinWeightDesc*******
static PyMethodDef EPGRSkinWeightDesc_methods[] =
{
	{NULL}
};
void EPGRSkinWeightDesc_dealloc(PyObject* self)
{
	delete ((EPGRSkinWeightDesc*)self)->ptr;
}
PyObject* EPGRSkinWeightDesc_str()
{
	return Py_BuildValue("s","This is EPGRSkinWeightDesc.");
}
int EPGRSkinWeightDesc_init(EPGRSkinWeightDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSkinWeightDesc* EPGRSkinWeightDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSkinWeightDesc *self;
	self = (EPGRSkinWeightDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSkinWeightDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSkinWeightDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSkinWeightDesc",/*tp_name*/
	sizeof(EPGRSkinWeightDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSkinWeightDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSkinWeightDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSkinWeightDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSkinWeightDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSkinWeightDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSkinWeightDesc_new,/*tp_new*/
};
void initEPGRSkinWeightDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSkinWeightDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSkinWeightDescType);
	PyModule_AddObject(m,"GRSkinWeightDesc",(PyObject *)&EPGRSkinWeightDescType);//モジュールに追加
}
PyObject* newEPGRSkinWeightDesc(struct GRSkinWeightDesc org)
{
	EPGRSkinWeightDesc *ret = EPGRSkinWeightDesc_new(&EPGRSkinWeightDescType,NULL,NULL);
	ret->ptr = new GRSkinWeightDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSkinWeightDesc(struct GRSkinWeightDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSkinWeightDesc *ret = EPGRSkinWeightDesc_new(&EPGRSkinWeightDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSkinWeightDesc

//{*********EPGRMeshIf*******
PyObject* EPGRMeshIf_CreateSkinWeight( EPGRMeshIf* self,EPGRSkinWeightDesc* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRSkinWeightDesc_Check(var1))
	{
		EPGRSkinWeightIf* ret = (EPGRSkinWeightIf*)newEPGRSkinWeightIf(self->ptr->CreateSkinWeight((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_EnableTex3D( EPGRMeshIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableTex3D((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableTex3D();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetColors( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec4f* ret = (EPVec4f*)newEPVec4f(self->ptr->GetColors());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetFaceNormals( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRMeshFace* ret = (EPGRMeshFace*)newEPGRMeshFace(self->ptr->GetFaceNormals());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetFaces( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRMeshFace* ret = (EPGRMeshFace*)newEPGRMeshFace(self->ptr->GetFaces());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetMaterialIndices( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetMaterialIndices());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetNormals( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetNormals());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetTexCoords( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->GetTexCoords());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_GetVertices( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetVertices());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_IsTex3D( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsTex3D()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_NFace( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NFace());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_NTriangle( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NTriangle());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_NVertex( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NVertex());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMeshIf_SwitchCoordinate( EPGRMeshIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->SwitchCoordinate();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRMeshIf_methods[] =
{
	{"CreateSkinWeight",(PyCFunction)EPGRMeshIf_CreateSkinWeight,METH_O,"EPGRMeshIf::CreateSkinWeight"},
	{"EnableTex3D",(PyCFunction)EPGRMeshIf_EnableTex3D,METH_VARARGS,"EPGRMeshIf::EnableTex3D"},
	{"GetColors",(PyCFunction)EPGRMeshIf_GetColors,METH_NOARGS,"EPGRMeshIf::GetColors"},
	{"GetFaceNormals",(PyCFunction)EPGRMeshIf_GetFaceNormals,METH_NOARGS,"EPGRMeshIf::GetFaceNormals"},
	{"GetFaces",(PyCFunction)EPGRMeshIf_GetFaces,METH_NOARGS,"EPGRMeshIf::GetFaces"},
	{"GetMaterialIndices",(PyCFunction)EPGRMeshIf_GetMaterialIndices,METH_NOARGS,"EPGRMeshIf::GetMaterialIndices"},
	{"GetNormals",(PyCFunction)EPGRMeshIf_GetNormals,METH_NOARGS,"EPGRMeshIf::GetNormals"},
	{"GetTexCoords",(PyCFunction)EPGRMeshIf_GetTexCoords,METH_NOARGS,"EPGRMeshIf::GetTexCoords"},
	{"GetVertices",(PyCFunction)EPGRMeshIf_GetVertices,METH_NOARGS,"EPGRMeshIf::GetVertices"},
	{"IsTex3D",(PyCFunction)EPGRMeshIf_IsTex3D,METH_NOARGS,"EPGRMeshIf::IsTex3D"},
	{"NFace",(PyCFunction)EPGRMeshIf_NFace,METH_NOARGS,"EPGRMeshIf::NFace"},
	{"NTriangle",(PyCFunction)EPGRMeshIf_NTriangle,METH_NOARGS,"EPGRMeshIf::NTriangle"},
	{"NVertex",(PyCFunction)EPGRMeshIf_NVertex,METH_NOARGS,"EPGRMeshIf::NVertex"},
	{"SwitchCoordinate",(PyCFunction)EPGRMeshIf_SwitchCoordinate,METH_NOARGS,"EPGRMeshIf::SwitchCoordinate"},
	{NULL}
};
void EPGRMeshIf_dealloc(PyObject* self)
{
}
PyObject* EPGRMeshIf_str()
{
	return Py_BuildValue("s","This is EPGRMeshIf.");
}
int EPGRMeshIf_init(EPGRMeshIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRMeshIf* EPGRMeshIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRMeshIf *self;
	self = (EPGRMeshIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRMeshIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRMeshIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRMeshIf",/*tp_name*/
	sizeof(EPGRMeshIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRMeshIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRMeshIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRMeshIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRMeshIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRMeshIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRMeshIf_new,/*tp_new*/
};
void initEPGRMeshIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRMeshIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRMeshIfType);
	PyModule_AddObject(m,"GRMeshIf",(PyObject *)&EPGRMeshIfType);//モジュールに追加
}
PyObject* newEPGRMeshIf(struct GRMeshIf org)
{
	EPGRMeshIf *ret = EPGRMeshIf_new(&EPGRMeshIfType,NULL,NULL);
	ret->ptr = new GRMeshIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRMeshIf(struct GRMeshIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRMeshIf *ret = EPGRMeshIf_new(&EPGRMeshIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRMeshIf

//{*********EPGRMeshDesc*******
static PyMethodDef EPGRMeshDesc_methods[] =
{
	{NULL}
};
void EPGRMeshDesc_dealloc(PyObject* self)
{
	delete ((EPGRMeshDesc*)self)->ptr;
}
PyObject* EPGRMeshDesc_str()
{
	return Py_BuildValue("s","This is EPGRMeshDesc.");
}
int EPGRMeshDesc_init(EPGRMeshDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRMeshDesc* EPGRMeshDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRMeshDesc *self;
	self = (EPGRMeshDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRMeshDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRMeshDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRMeshDesc",/*tp_name*/
	sizeof(EPGRMeshDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRMeshDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRMeshDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRMeshDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRMeshDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRMeshDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRMeshDesc_new,/*tp_new*/
};
void initEPGRMeshDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRMeshDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRMeshDescType);
	PyModule_AddObject(m,"GRMeshDesc",(PyObject *)&EPGRMeshDescType);//モジュールに追加
}
PyObject* newEPGRMeshDesc(struct GRMeshDesc org)
{
	EPGRMeshDesc *ret = EPGRMeshDesc_new(&EPGRMeshDescType,NULL,NULL);
	ret->ptr = new GRMeshDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRMeshDesc(struct GRMeshDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRMeshDesc *ret = EPGRMeshDesc_new(&EPGRMeshDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRMeshDesc

//{*********EPGRFont*******
PyObject* EPGRFont_SetColor( EPGRFont* self,EPunsignedlong* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPunsignedlong_Check(var1))
	{
		self->ptr->SetColor((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRFont_methods[] =
{
	{"SetColor",(PyCFunction)EPGRFont_SetColor,METH_O,"EPGRFont::SetColor"},
	{NULL}
};
void EPGRFont_dealloc(PyObject* self)
{
	delete ((EPGRFont*)self)->ptr;
}
PyObject* EPGRFont_str()
{
	return Py_BuildValue("s","This is EPGRFont.");
}
int EPGRFont_init(EPGRFont* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRFont* EPGRFont_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRFont *self;
	self = (EPGRFont*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRFont_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRFontType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRFont",/*tp_name*/
	sizeof(EPGRFont),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRFont_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRFont_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRFont",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRFont_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRFont_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRFont_new,/*tp_new*/
};
void initEPGRFont(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRFontType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRFontType);
	PyModule_AddObject(m,"GRFont",(PyObject *)&EPGRFontType);//モジュールに追加
}
PyObject* newEPGRFont(GRFont org)
{
	EPGRFont *ret = EPGRFont_new(&EPGRFontType,NULL,NULL);
	ret->ptr = new GRFont();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRFont(GRFont* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRFont *ret = EPGRFont_new(&EPGRFontType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRFont

//{*********EPGRLightIf*******
static PyMethodDef EPGRLightIf_methods[] =
{
	{NULL}
};
void EPGRLightIf_dealloc(PyObject* self)
{
}
PyObject* EPGRLightIf_str()
{
	return Py_BuildValue("s","This is EPGRLightIf.");
}
int EPGRLightIf_init(EPGRLightIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRLightIf* EPGRLightIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRLightIf *self;
	self = (EPGRLightIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRLightIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRLightIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRLightIf",/*tp_name*/
	sizeof(EPGRLightIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRLightIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRLightIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRLightIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRLightIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRLightIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRLightIf_new,/*tp_new*/
};
void initEPGRLightIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRLightIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRLightIfType);
	PyModule_AddObject(m,"GRLightIf",(PyObject *)&EPGRLightIfType);//モジュールに追加
}
PyObject* newEPGRLightIf(struct GRLightIf org)
{
	EPGRLightIf *ret = EPGRLightIf_new(&EPGRLightIfType,NULL,NULL);
	ret->ptr = new GRLightIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRLightIf(struct GRLightIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRLightIf *ret = EPGRLightIf_new(&EPGRLightIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRLightIf

//{*********EPGRLightDesc*******
static PyMethodDef EPGRLightDesc_methods[] =
{
	{NULL}
};
void EPGRLightDesc_dealloc(PyObject* self)
{
	delete ((EPGRLightDesc*)self)->ptr;
}
PyObject* EPGRLightDesc_str()
{
	return Py_BuildValue("s","This is EPGRLightDesc.");
}
int EPGRLightDesc_init(EPGRLightDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRLightDesc* EPGRLightDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRLightDesc *self;
	self = (EPGRLightDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRLightDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRLightDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRLightDesc",/*tp_name*/
	sizeof(EPGRLightDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRLightDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRLightDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRLightDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRLightDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRLightDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRLightDesc_new,/*tp_new*/
};
void initEPGRLightDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRLightDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRLightDescType);
	PyModule_AddObject(m,"GRLightDesc",(PyObject *)&EPGRLightDescType);//モジュールに追加
}
PyObject* newEPGRLightDesc(struct GRLightDesc org)
{
	EPGRLightDesc *ret = EPGRLightDesc_new(&EPGRLightDescType,NULL,NULL);
	ret->ptr = new GRLightDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRLightDesc(struct GRLightDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRLightDesc *ret = EPGRLightDesc_new(&EPGRLightDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRLightDesc

//{*********EPGRMaterialIf*******
PyObject* EPGRMaterialIf_IsOpaque( EPGRMaterialIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsOpaque()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRMaterialIf_methods[] =
{
	{"IsOpaque",(PyCFunction)EPGRMaterialIf_IsOpaque,METH_NOARGS,"EPGRMaterialIf::IsOpaque"},
	{NULL}
};
void EPGRMaterialIf_dealloc(PyObject* self)
{
}
PyObject* EPGRMaterialIf_str()
{
	return Py_BuildValue("s","This is EPGRMaterialIf.");
}
int EPGRMaterialIf_init(EPGRMaterialIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRMaterialIf* EPGRMaterialIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRMaterialIf *self;
	self = (EPGRMaterialIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRMaterialIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRMaterialIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRMaterialIf",/*tp_name*/
	sizeof(EPGRMaterialIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRMaterialIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRMaterialIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRMaterialIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRMaterialIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRMaterialIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRMaterialIf_new,/*tp_new*/
};
void initEPGRMaterialIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRMaterialIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRMaterialIfType);
	PyModule_AddObject(m,"GRMaterialIf",(PyObject *)&EPGRMaterialIfType);//モジュールに追加
}
PyObject* newEPGRMaterialIf(struct GRMaterialIf org)
{
	EPGRMaterialIf *ret = EPGRMaterialIf_new(&EPGRMaterialIfType,NULL,NULL);
	ret->ptr = new GRMaterialIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRMaterialIf(struct GRMaterialIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRMaterialIf *ret = EPGRMaterialIf_new(&EPGRMaterialIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRMaterialIf

//{*********EPGRMaterialDesc*******
PyObject* EPGRMaterialDesc_Is3D( EPGRMaterialDesc* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->Is3D()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRMaterialDesc_IsOpaque( EPGRMaterialDesc* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsOpaque()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRMaterialDesc_methods[] =
{
	{"Is3D",(PyCFunction)EPGRMaterialDesc_Is3D,METH_NOARGS,"EPGRMaterialDesc::Is3D"},
	{"IsOpaque",(PyCFunction)EPGRMaterialDesc_IsOpaque,METH_NOARGS,"EPGRMaterialDesc::IsOpaque"},
	{NULL}
};
void EPGRMaterialDesc_dealloc(PyObject* self)
{
	delete ((EPGRMaterialDesc*)self)->ptr;
}
PyObject* EPGRMaterialDesc_str()
{
	return Py_BuildValue("s","This is EPGRMaterialDesc.");
}
int EPGRMaterialDesc_init(EPGRMaterialDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRMaterialDesc* EPGRMaterialDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRMaterialDesc *self;
	self = (EPGRMaterialDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRMaterialDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRMaterialDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRMaterialDesc",/*tp_name*/
	sizeof(EPGRMaterialDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRMaterialDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRMaterialDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRMaterialDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRMaterialDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRMaterialDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRMaterialDesc_new,/*tp_new*/
};
void initEPGRMaterialDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRMaterialDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRMaterialDescType);
	PyModule_AddObject(m,"GRMaterialDesc",(PyObject *)&EPGRMaterialDescType);//モジュールに追加
}
PyObject* newEPGRMaterialDesc(struct GRMaterialDesc org)
{
	EPGRMaterialDesc *ret = EPGRMaterialDesc_new(&EPGRMaterialDescType,NULL,NULL);
	ret->ptr = new GRMaterialDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRMaterialDesc(struct GRMaterialDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRMaterialDesc *ret = EPGRMaterialDesc_new(&EPGRMaterialDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRMaterialDesc

//{*********EPGRCameraIf*******
PyObject* EPGRCameraIf_GetFrame( EPGRCameraIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRFrameIf* ret = (EPGRFrameIf*)newEPGRFrameIf(self->ptr->GetFrame());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRCameraIf_SetFrame( EPGRCameraIf* self,EPGRFrameIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRFrameIf_Check(var1))
	{
		self->ptr->SetFrame((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRCameraIf_methods[] =
{
	{"GetFrame",(PyCFunction)EPGRCameraIf_GetFrame,METH_NOARGS,"EPGRCameraIf::GetFrame"},
	{"SetFrame",(PyCFunction)EPGRCameraIf_SetFrame,METH_O,"EPGRCameraIf::SetFrame"},
	{NULL}
};
void EPGRCameraIf_dealloc(PyObject* self)
{
}
PyObject* EPGRCameraIf_str()
{
	return Py_BuildValue("s","This is EPGRCameraIf.");
}
int EPGRCameraIf_init(EPGRCameraIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRCameraIf* EPGRCameraIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRCameraIf *self;
	self = (EPGRCameraIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRCameraIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRCameraIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRCameraIf",/*tp_name*/
	sizeof(EPGRCameraIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRCameraIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRCameraIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRCameraIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRCameraIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRCameraIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRCameraIf_new,/*tp_new*/
};
void initEPGRCameraIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRCameraIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRCameraIfType);
	PyModule_AddObject(m,"GRCameraIf",(PyObject *)&EPGRCameraIfType);//モジュールに追加
}
PyObject* newEPGRCameraIf(struct GRCameraIf org)
{
	EPGRCameraIf *ret = EPGRCameraIf_new(&EPGRCameraIfType,NULL,NULL);
	ret->ptr = new GRCameraIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRCameraIf(struct GRCameraIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRCameraIf *ret = EPGRCameraIf_new(&EPGRCameraIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRCameraIf

//{*********EPGRCameraDesc*******
static PyMethodDef EPGRCameraDesc_methods[] =
{
	{NULL}
};
void EPGRCameraDesc_dealloc(PyObject* self)
{
	delete ((EPGRCameraDesc*)self)->ptr;
}
PyObject* EPGRCameraDesc_str()
{
	return Py_BuildValue("s","This is EPGRCameraDesc.");
}
int EPGRCameraDesc_init(EPGRCameraDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRCameraDesc* EPGRCameraDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRCameraDesc *self;
	self = (EPGRCameraDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRCameraDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRCameraDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRCameraDesc",/*tp_name*/
	sizeof(EPGRCameraDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRCameraDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRCameraDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRCameraDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRCameraDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRCameraDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRCameraDesc_new,/*tp_new*/
};
void initEPGRCameraDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRCameraDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRCameraDescType);
	PyModule_AddObject(m,"GRCameraDesc",(PyObject *)&EPGRCameraDescType);//モジュールに追加
}
PyObject* newEPGRCameraDesc(struct GRCameraDesc org)
{
	EPGRCameraDesc *ret = EPGRCameraDesc_new(&EPGRCameraDescType,NULL,NULL);
	ret->ptr = new GRCameraDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRCameraDesc(struct GRCameraDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRCameraDesc *ret = EPGRCameraDesc_new(&EPGRCameraDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRCameraDesc

//{*********EPGRRenderBaseIf*******
PyObject* EPGRRenderBaseIf_BeginScene( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->BeginScene();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_ClearBlendMatrix( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ClearBlendMatrix();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_ClearBuffer( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ClearBuffer();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_DrawCone( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))) && PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,3))))
	{
		self->ptr->DrawCone(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))),(Py_True == ((PyObject*)PyTuple_GetItem(tuple,3))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->DrawCone(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_DrawCylinder( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))) && PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,3))))
	{
		self->ptr->DrawCylinder(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))),(Py_True == ((PyObject*)PyTuple_GetItem(tuple,3))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->DrawCylinder(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_DrawDirect( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && EPvoid_Check(((EPvoid*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,3))))
	{
		self->ptr->DrawDirect(((Spr::GRRenderBaseIf::TPrimitiveType)PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0)))),(((EPvoid*)PyTuple_GetItem(tuple,1))->ptr),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,3))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && EPvoid_Check(((EPvoid*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->DrawDirect(((Spr::GRRenderBaseIf::TPrimitiveType)PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0)))),(((EPvoid*)PyTuple_GetItem(tuple,1))->ptr),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_DrawFont( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,0))) && EPstring_Check(((EPstring*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->DrawFont((*(((EPVec2f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPstring*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPstring_Check(((EPstring*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->DrawFont((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPstring*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_DrawList( EPGRRenderBaseIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->DrawList(PyLong_AsLong((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_DrawSphere( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))) && PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,3))))
	{
		self->ptr->DrawSphere(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))),(Py_True == ((PyObject*)PyTuple_GetItem(tuple,3))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->DrawSphere(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_EndList( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->EndList();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_EndScene( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->EndScene();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_GetClearColor( EPGRRenderBaseIf* self,EPVec4f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec4f_Check(var1))
	{
		self->ptr->GetClearColor((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_GetModelMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->GetModelMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_GetProjectionMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->GetProjectionMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_GetShaderLocation( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && EPvoid_Check(((EPvoid*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->GetShaderLocation((GRHandler)PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),(((EPvoid*)PyTuple_GetItem(tuple,1))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_GetViewMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->GetViewMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_InitShader( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->InitShader();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_LoadTexture( EPGRRenderBaseIf* self,EPstring* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPstring_Check(var1))
	{
		EPunsignedint* ret = (EPunsignedint*)newEPunsignedint(self->ptr->LoadTexture((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_MultModelMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->MultModelMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_PopLight( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->PopLight();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_PopModelMatrix( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->PopModelMatrix();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_PushLight( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPGRLightDesc_Check(((EPGRLightDesc*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->PushLight((*(((EPGRLightDesc*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPGRLightIf_Check(((EPGRLightIf*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->PushLight((((EPGRLightIf*)PyTuple_GetItem(tuple,0))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_PushModelMatrix( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->PushModelMatrix();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_ReadShaderSource( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && EPstring_Check(((EPstring*)PyTuple_GetItem(tuple,1))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->ReadShaderSource((GRHandler)PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),(*(((EPstring*)PyTuple_GetItem(tuple,1))->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_ReleaseList( EPGRRenderBaseIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->ReleaseList(PyLong_AsLong((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetAlphaMode( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetAlphaMode(((Spr::GRRenderBaseIf::TBlendFunc)PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0)))),((Spr::GRRenderBaseIf::TBlendFunc)PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetAlphaTest( EPGRRenderBaseIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetAlphaTest((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetBlendMatrix( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPAffinef_Check(((EPAffinef*)PyTuple_GetItem(tuple,0))) && EPunsignedint_Check(((EPunsignedint*)PyTuple_GetItem(tuple,1))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->SetBlendMatrix((*(((EPAffinef*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPunsignedint*)PyTuple_GetItem(tuple,1))->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPAffinef_Check(((EPAffinef*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->SetBlendMatrix((*(((EPAffinef*)PyTuple_GetItem(tuple,0))->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetClearColor( EPGRRenderBaseIf* self,EPVec4f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec4f_Check(var1))
	{
		self->ptr->SetClearColor((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetDepthFunc( EPGRRenderBaseIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->SetDepthFunc(((Spr::GRRenderBaseIf::TDepthFunc)PyLong_AsLong((PyObject*)var1)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetDepthTest( EPGRRenderBaseIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetDepthTest((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetDepthWrite( EPGRRenderBaseIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetDepthWrite((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetFont( EPGRRenderBaseIf* self,EPGRFont* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRFont_Check(var1))
	{
		self->ptr->SetFont((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetLighting( EPGRRenderBaseIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetLighting((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetLineWidth( EPGRRenderBaseIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetLineWidth(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetMaterial( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPGRMaterialDesc_Check(((EPGRMaterialDesc*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->SetMaterial((*(((EPGRMaterialDesc*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPGRMaterialIf_Check(((EPGRMaterialIf*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->SetMaterial((((EPGRMaterialIf*)PyTuple_GetItem(tuple,0))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetModelMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->SetModelMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetProjectionMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->SetProjectionMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetShaderFormat( EPGRRenderBaseIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->SetShaderFormat(((Spr::GRShaderFormat::ShaderType)PyLong_AsLong((PyObject*)var1)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetTextureImage( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 6&&EPstring_Check(((EPstring*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,2))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,3))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,4))) && EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,5))))
	{
		self->ptr->SetTextureImage((*(((EPstring*)PyTuple_GetItem(tuple,0))->ptr)),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,2))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,3))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,4))),(((EPchar*)PyTuple_GetItem(tuple,5))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetVertexFormat( EPGRRenderBaseIf* self,EPGRVertexElement* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRVertexElement_Check(var1))
	{
		self->ptr->SetVertexFormat((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetVertexShader( EPGRRenderBaseIf* self,EPvoid* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPvoid_Check(var1))
	{
		self->ptr->SetVertexShader((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetViewMatrix( EPGRRenderBaseIf* self,EPAffinef* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPAffinef_Check(var1))
	{
		self->ptr->SetViewMatrix((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_SetViewport( EPGRRenderBaseIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetViewport((*(((EPVec2f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderBaseIf_StartList( EPGRRenderBaseIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->StartList());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRRenderBaseIf_methods[] =
{
	{"BeginScene",(PyCFunction)EPGRRenderBaseIf_BeginScene,METH_NOARGS,"EPGRRenderBaseIf::BeginScene"},
	{"ClearBlendMatrix",(PyCFunction)EPGRRenderBaseIf_ClearBlendMatrix,METH_NOARGS,"EPGRRenderBaseIf::ClearBlendMatrix"},
	{"ClearBuffer",(PyCFunction)EPGRRenderBaseIf_ClearBuffer,METH_NOARGS,"EPGRRenderBaseIf::ClearBuffer"},
	{"DrawCone",(PyCFunction)EPGRRenderBaseIf_DrawCone,METH_VARARGS,"EPGRRenderBaseIf::DrawCone"},
	{"DrawCylinder",(PyCFunction)EPGRRenderBaseIf_DrawCylinder,METH_VARARGS,"EPGRRenderBaseIf::DrawCylinder"},
	{"DrawDirect",(PyCFunction)EPGRRenderBaseIf_DrawDirect,METH_VARARGS,"EPGRRenderBaseIf::DrawDirect"},
	{"DrawFont",(PyCFunction)EPGRRenderBaseIf_DrawFont,METH_VARARGS,"EPGRRenderBaseIf::DrawFont"},
	{"DrawList",(PyCFunction)EPGRRenderBaseIf_DrawList,METH_O,"EPGRRenderBaseIf::DrawList"},
	{"DrawSphere",(PyCFunction)EPGRRenderBaseIf_DrawSphere,METH_VARARGS,"EPGRRenderBaseIf::DrawSphere"},
	{"EndList",(PyCFunction)EPGRRenderBaseIf_EndList,METH_NOARGS,"EPGRRenderBaseIf::EndList"},
	{"EndScene",(PyCFunction)EPGRRenderBaseIf_EndScene,METH_NOARGS,"EPGRRenderBaseIf::EndScene"},
	{"GetClearColor",(PyCFunction)EPGRRenderBaseIf_GetClearColor,METH_O,"EPGRRenderBaseIf::GetClearColor"},
	{"GetModelMatrix",(PyCFunction)EPGRRenderBaseIf_GetModelMatrix,METH_O,"EPGRRenderBaseIf::GetModelMatrix"},
	{"GetProjectionMatrix",(PyCFunction)EPGRRenderBaseIf_GetProjectionMatrix,METH_O,"EPGRRenderBaseIf::GetProjectionMatrix"},
	{"GetShaderLocation",(PyCFunction)EPGRRenderBaseIf_GetShaderLocation,METH_VARARGS,"EPGRRenderBaseIf::GetShaderLocation"},
	{"GetViewMatrix",(PyCFunction)EPGRRenderBaseIf_GetViewMatrix,METH_O,"EPGRRenderBaseIf::GetViewMatrix"},
	{"InitShader",(PyCFunction)EPGRRenderBaseIf_InitShader,METH_NOARGS,"EPGRRenderBaseIf::InitShader"},
	{"LoadTexture",(PyCFunction)EPGRRenderBaseIf_LoadTexture,METH_O,"EPGRRenderBaseIf::LoadTexture"},
	{"MultModelMatrix",(PyCFunction)EPGRRenderBaseIf_MultModelMatrix,METH_O,"EPGRRenderBaseIf::MultModelMatrix"},
	{"PopLight",(PyCFunction)EPGRRenderBaseIf_PopLight,METH_NOARGS,"EPGRRenderBaseIf::PopLight"},
	{"PopModelMatrix",(PyCFunction)EPGRRenderBaseIf_PopModelMatrix,METH_NOARGS,"EPGRRenderBaseIf::PopModelMatrix"},
	{"PushLight",(PyCFunction)EPGRRenderBaseIf_PushLight,METH_VARARGS,"EPGRRenderBaseIf::PushLight"},
	{"PushModelMatrix",(PyCFunction)EPGRRenderBaseIf_PushModelMatrix,METH_NOARGS,"EPGRRenderBaseIf::PushModelMatrix"},
	{"ReadShaderSource",(PyCFunction)EPGRRenderBaseIf_ReadShaderSource,METH_VARARGS,"EPGRRenderBaseIf::ReadShaderSource"},
	{"ReleaseList",(PyCFunction)EPGRRenderBaseIf_ReleaseList,METH_O,"EPGRRenderBaseIf::ReleaseList"},
	{"SetAlphaMode",(PyCFunction)EPGRRenderBaseIf_SetAlphaMode,METH_VARARGS,"EPGRRenderBaseIf::SetAlphaMode"},
	{"SetAlphaTest",(PyCFunction)EPGRRenderBaseIf_SetAlphaTest,METH_O,"EPGRRenderBaseIf::SetAlphaTest"},
	{"SetBlendMatrix",(PyCFunction)EPGRRenderBaseIf_SetBlendMatrix,METH_VARARGS,"EPGRRenderBaseIf::SetBlendMatrix"},
	{"SetClearColor",(PyCFunction)EPGRRenderBaseIf_SetClearColor,METH_O,"EPGRRenderBaseIf::SetClearColor"},
	{"SetDepthFunc",(PyCFunction)EPGRRenderBaseIf_SetDepthFunc,METH_O,"EPGRRenderBaseIf::SetDepthFunc"},
	{"SetDepthTest",(PyCFunction)EPGRRenderBaseIf_SetDepthTest,METH_O,"EPGRRenderBaseIf::SetDepthTest"},
	{"SetDepthWrite",(PyCFunction)EPGRRenderBaseIf_SetDepthWrite,METH_O,"EPGRRenderBaseIf::SetDepthWrite"},
	{"SetFont",(PyCFunction)EPGRRenderBaseIf_SetFont,METH_O,"EPGRRenderBaseIf::SetFont"},
	{"SetLighting",(PyCFunction)EPGRRenderBaseIf_SetLighting,METH_O,"EPGRRenderBaseIf::SetLighting"},
	{"SetLineWidth",(PyCFunction)EPGRRenderBaseIf_SetLineWidth,METH_O,"EPGRRenderBaseIf::SetLineWidth"},
	{"SetMaterial",(PyCFunction)EPGRRenderBaseIf_SetMaterial,METH_VARARGS,"EPGRRenderBaseIf::SetMaterial"},
	{"SetModelMatrix",(PyCFunction)EPGRRenderBaseIf_SetModelMatrix,METH_O,"EPGRRenderBaseIf::SetModelMatrix"},
	{"SetProjectionMatrix",(PyCFunction)EPGRRenderBaseIf_SetProjectionMatrix,METH_O,"EPGRRenderBaseIf::SetProjectionMatrix"},
	{"SetShaderFormat",(PyCFunction)EPGRRenderBaseIf_SetShaderFormat,METH_O,"EPGRRenderBaseIf::SetShaderFormat"},
	{"SetTextureImage",(PyCFunction)EPGRRenderBaseIf_SetTextureImage,METH_VARARGS,"EPGRRenderBaseIf::SetTextureImage"},
	{"SetVertexFormat",(PyCFunction)EPGRRenderBaseIf_SetVertexFormat,METH_O,"EPGRRenderBaseIf::SetVertexFormat"},
	{"SetVertexShader",(PyCFunction)EPGRRenderBaseIf_SetVertexShader,METH_O,"EPGRRenderBaseIf::SetVertexShader"},
	{"SetViewMatrix",(PyCFunction)EPGRRenderBaseIf_SetViewMatrix,METH_O,"EPGRRenderBaseIf::SetViewMatrix"},
	{"SetViewport",(PyCFunction)EPGRRenderBaseIf_SetViewport,METH_VARARGS,"EPGRRenderBaseIf::SetViewport"},
	{"StartList",(PyCFunction)EPGRRenderBaseIf_StartList,METH_NOARGS,"EPGRRenderBaseIf::StartList"},
	{NULL}
};
void EPGRRenderBaseIf_dealloc(PyObject* self)
{
}
PyObject* EPGRRenderBaseIf_str()
{
	return Py_BuildValue("s","This is EPGRRenderBaseIf.");
}
int EPGRRenderBaseIf_init(EPGRRenderBaseIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRRenderBaseIf* EPGRRenderBaseIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRRenderBaseIf *self;
	self = (EPGRRenderBaseIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRRenderBaseIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRRenderBaseIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRRenderBaseIf",/*tp_name*/
	sizeof(EPGRRenderBaseIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRRenderBaseIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRRenderBaseIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRRenderBaseIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRRenderBaseIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRRenderBaseIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRRenderBaseIf_new,/*tp_new*/
};
void initEPGRRenderBaseIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRRenderBaseIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRRenderBaseIfType);
	PyModule_AddObject(m,"GRRenderBaseIf",(PyObject *)&EPGRRenderBaseIfType);//モジュールに追加
}
PyObject* newEPGRRenderBaseIf(struct GRRenderBaseIf org)
{
	EPGRRenderBaseIf *ret = EPGRRenderBaseIf_new(&EPGRRenderBaseIfType,NULL,NULL);
	ret->ptr = new GRRenderBaseIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRRenderBaseIf(struct GRRenderBaseIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRRenderBaseIf *ret = EPGRRenderBaseIf_new(&EPGRRenderBaseIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRRenderBaseIf

//{*********EPGRRenderIf*******
PyObject* EPGRRenderIf_GetCamera( EPGRRenderIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRCameraDesc* ret = (EPGRCameraDesc*)newEPGRCameraDesc(self->ptr->GetCamera());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_GetDevice( EPGRRenderIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRDeviceIf* ret = (EPGRDeviceIf*)newEPGRDeviceIf(self->ptr->GetDevice());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_GetPixelSize( EPGRRenderIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->GetPixelSize());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_GetViewportPos( EPGRRenderIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->GetViewportPos());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_GetViewportSize( EPGRRenderIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec2f* ret = (EPVec2f*)newEPVec2f(self->ptr->GetViewportSize());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_Reshape( EPGRRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,0))) && EPVec2f_Check(((EPVec2f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->Reshape((*(((EPVec2f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec2f*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_ScreenToCamera( EPGRRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))) && PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,3))))
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->ScreenToCamera(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))),(Py_True == ((PyObject*)PyTuple_GetItem(tuple,3)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 3&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->ScreenToCamera(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_SetCamera( EPGRRenderIf* self,EPGRCameraDesc* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRCameraDesc_Check(var1))
	{
		self->ptr->SetCamera((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRRenderIf_SetDevice( EPGRRenderIf* self,EPGRDeviceIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRDeviceIf_Check(var1))
	{
		self->ptr->SetDevice((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRRenderIf_methods[] =
{
	{"GetCamera",(PyCFunction)EPGRRenderIf_GetCamera,METH_NOARGS,"EPGRRenderIf::GetCamera"},
	{"GetDevice",(PyCFunction)EPGRRenderIf_GetDevice,METH_NOARGS,"EPGRRenderIf::GetDevice"},
	{"GetPixelSize",(PyCFunction)EPGRRenderIf_GetPixelSize,METH_NOARGS,"EPGRRenderIf::GetPixelSize"},
	{"GetViewportPos",(PyCFunction)EPGRRenderIf_GetViewportPos,METH_NOARGS,"EPGRRenderIf::GetViewportPos"},
	{"GetViewportSize",(PyCFunction)EPGRRenderIf_GetViewportSize,METH_NOARGS,"EPGRRenderIf::GetViewportSize"},
	{"Reshape",(PyCFunction)EPGRRenderIf_Reshape,METH_VARARGS,"EPGRRenderIf::Reshape"},
	{"ScreenToCamera",(PyCFunction)EPGRRenderIf_ScreenToCamera,METH_VARARGS,"EPGRRenderIf::ScreenToCamera"},
	{"SetCamera",(PyCFunction)EPGRRenderIf_SetCamera,METH_O,"EPGRRenderIf::SetCamera"},
	{"SetDevice",(PyCFunction)EPGRRenderIf_SetDevice,METH_O,"EPGRRenderIf::SetDevice"},
	{NULL}
};
void EPGRRenderIf_dealloc(PyObject* self)
{
}
PyObject* EPGRRenderIf_str()
{
	return Py_BuildValue("s","This is EPGRRenderIf.");
}
int EPGRRenderIf_init(EPGRRenderIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRRenderIf* EPGRRenderIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRRenderIf *self;
	self = (EPGRRenderIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRRenderIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRRenderIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRRenderIf",/*tp_name*/
	sizeof(EPGRRenderIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRRenderIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRRenderIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRRenderIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRRenderIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRRenderBaseIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRRenderIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRRenderIf_new,/*tp_new*/
};
void initEPGRRenderIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRRenderIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRRenderIfType);
	PyModule_AddObject(m,"GRRenderIf",(PyObject *)&EPGRRenderIfType);//モジュールに追加
}
PyObject* newEPGRRenderIf(struct GRRenderIf org)
{
	EPGRRenderIf *ret = EPGRRenderIf_new(&EPGRRenderIfType,NULL,NULL);
	ret->ptr = new GRRenderIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRRenderIf(struct GRRenderIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRRenderIf *ret = EPGRRenderIf_new(&EPGRRenderIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRRenderIf

//{*********EPGRDeviceIf*******
PyObject* EPGRDeviceIf_Init( EPGRDeviceIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Init();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDeviceIf_Print( EPGRDeviceIf* self,EPostream* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPostream_Check(var1))
	{
		self->ptr->Print((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRDeviceIf_methods[] =
{
	{"Init",(PyCFunction)EPGRDeviceIf_Init,METH_NOARGS,"EPGRDeviceIf::Init"},
	{"Print",(PyCFunction)EPGRDeviceIf_Print,METH_O,"EPGRDeviceIf::Print"},
	{NULL}
};
void EPGRDeviceIf_dealloc(PyObject* self)
{
}
PyObject* EPGRDeviceIf_str()
{
	return Py_BuildValue("s","This is EPGRDeviceIf.");
}
int EPGRDeviceIf_init(EPGRDeviceIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRDeviceIf* EPGRDeviceIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRDeviceIf *self;
	self = (EPGRDeviceIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRDeviceIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRDeviceIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRDeviceIf",/*tp_name*/
	sizeof(EPGRDeviceIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRDeviceIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRDeviceIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRDeviceIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRDeviceIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRRenderBaseIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRDeviceIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRDeviceIf_new,/*tp_new*/
};
void initEPGRDeviceIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRDeviceIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRDeviceIfType);
	PyModule_AddObject(m,"GRDeviceIf",(PyObject *)&EPGRDeviceIfType);//モジュールに追加
}
PyObject* newEPGRDeviceIf(struct GRDeviceIf org)
{
	EPGRDeviceIf *ret = EPGRDeviceIf_new(&EPGRDeviceIfType,NULL,NULL);
	ret->ptr = new GRDeviceIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRDeviceIf(struct GRDeviceIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRDeviceIf *ret = EPGRDeviceIf_new(&EPGRDeviceIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRDeviceIf

//{*********EPGRDeviceGLIf*******
static PyMethodDef EPGRDeviceGLIf_methods[] =
{
	{NULL}
};
void EPGRDeviceGLIf_dealloc(PyObject* self)
{
}
PyObject* EPGRDeviceGLIf_str()
{
	return Py_BuildValue("s","This is EPGRDeviceGLIf.");
}
int EPGRDeviceGLIf_init(EPGRDeviceGLIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRDeviceGLIf* EPGRDeviceGLIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRDeviceGLIf *self;
	self = (EPGRDeviceGLIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRDeviceGLIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRDeviceGLIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRDeviceGLIf",/*tp_name*/
	sizeof(EPGRDeviceGLIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRDeviceGLIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRDeviceGLIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRDeviceGLIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRDeviceGLIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRDeviceIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRDeviceGLIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRDeviceGLIf_new,/*tp_new*/
};
void initEPGRDeviceGLIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRDeviceGLIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRDeviceGLIfType);
	PyModule_AddObject(m,"GRDeviceGLIf",(PyObject *)&EPGRDeviceGLIfType);//モジュールに追加
}
PyObject* newEPGRDeviceGLIf(struct GRDeviceGLIf org)
{
	EPGRDeviceGLIf *ret = EPGRDeviceGLIf_new(&EPGRDeviceGLIfType,NULL,NULL);
	ret->ptr = new GRDeviceGLIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRDeviceGLIf(struct GRDeviceGLIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRDeviceGLIf *ret = EPGRDeviceGLIf_new(&EPGRDeviceGLIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRDeviceGLIf

//{*********EPGRDebugRenderIf*******
PyObject* EPGRDebugRenderIf_DrawFaceSolid( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPCDFaceIf_Check(((EPCDFaceIf*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->DrawFaceSolid((((EPCDFaceIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_DrawFaceWire( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPCDFaceIf_Check(((EPCDFaceIf*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->DrawFaceWire((((EPCDFaceIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_DrawScene( EPGRDebugRenderIf* self,EPPHSceneIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHSceneIf_Check(var1))
	{
		self->ptr->DrawScene((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_DrawSolid( EPGRDebugRenderIf* self,EPPHSolidIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHSolidIf_Check(var1))
	{
		self->ptr->DrawSolid((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_EnableGrid( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->EnableGrid((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->EnableGrid((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableGrid((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableGrid();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_EnableRenderAxis( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->EnableRenderAxis((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableRenderAxis((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableRenderAxis();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_EnableRenderContact( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableRenderContact((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableRenderContact();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_EnableRenderForce( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->EnableRenderForce((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableRenderForce((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableRenderForce();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_EnableRenderIK( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->EnableRenderIK((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableRenderIK((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableRenderIK();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_EnableRenderWorldAxis( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->EnableRenderWorldAxis((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->EnableRenderWorldAxis();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_SetMaterialSample( EPGRDebugRenderIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->SetMaterialSample(((Spr::GRDebugRenderIf::TMaterialSample)PyLong_AsLong((PyObject*)var1)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRDebugRenderIf_SetRenderMode( EPGRDebugRenderIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))) && PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetRenderMode((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))),(Py_True == ((PyObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->SetRenderMode((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->SetRenderMode();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRDebugRenderIf_methods[] =
{
	{"DrawFaceSolid",(PyCFunction)EPGRDebugRenderIf_DrawFaceSolid,METH_VARARGS,"EPGRDebugRenderIf::DrawFaceSolid"},
	{"DrawFaceWire",(PyCFunction)EPGRDebugRenderIf_DrawFaceWire,METH_VARARGS,"EPGRDebugRenderIf::DrawFaceWire"},
	{"DrawScene",(PyCFunction)EPGRDebugRenderIf_DrawScene,METH_O,"EPGRDebugRenderIf::DrawScene"},
	{"DrawSolid",(PyCFunction)EPGRDebugRenderIf_DrawSolid,METH_O,"EPGRDebugRenderIf::DrawSolid"},
	{"EnableGrid",(PyCFunction)EPGRDebugRenderIf_EnableGrid,METH_VARARGS,"EPGRDebugRenderIf::EnableGrid"},
	{"EnableRenderAxis",(PyCFunction)EPGRDebugRenderIf_EnableRenderAxis,METH_VARARGS,"EPGRDebugRenderIf::EnableRenderAxis"},
	{"EnableRenderContact",(PyCFunction)EPGRDebugRenderIf_EnableRenderContact,METH_VARARGS,"EPGRDebugRenderIf::EnableRenderContact"},
	{"EnableRenderForce",(PyCFunction)EPGRDebugRenderIf_EnableRenderForce,METH_VARARGS,"EPGRDebugRenderIf::EnableRenderForce"},
	{"EnableRenderIK",(PyCFunction)EPGRDebugRenderIf_EnableRenderIK,METH_VARARGS,"EPGRDebugRenderIf::EnableRenderIK"},
	{"EnableRenderWorldAxis",(PyCFunction)EPGRDebugRenderIf_EnableRenderWorldAxis,METH_VARARGS,"EPGRDebugRenderIf::EnableRenderWorldAxis"},
	{"SetMaterialSample",(PyCFunction)EPGRDebugRenderIf_SetMaterialSample,METH_O,"EPGRDebugRenderIf::SetMaterialSample"},
	{"SetRenderMode",(PyCFunction)EPGRDebugRenderIf_SetRenderMode,METH_VARARGS,"EPGRDebugRenderIf::SetRenderMode"},
	{NULL}
};
void EPGRDebugRenderIf_dealloc(PyObject* self)
{
}
PyObject* EPGRDebugRenderIf_str()
{
	return Py_BuildValue("s","This is EPGRDebugRenderIf.");
}
int EPGRDebugRenderIf_init(EPGRDebugRenderIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRDebugRenderIf* EPGRDebugRenderIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRDebugRenderIf *self;
	self = (EPGRDebugRenderIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRDebugRenderIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRDebugRenderIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRDebugRenderIf",/*tp_name*/
	sizeof(EPGRDebugRenderIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRDebugRenderIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRDebugRenderIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRDebugRenderIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRDebugRenderIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRRenderIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRDebugRenderIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRDebugRenderIf_new,/*tp_new*/
};
void initEPGRDebugRenderIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRDebugRenderIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRDebugRenderIfType);
	PyModule_AddObject(m,"GRDebugRenderIf",(PyObject *)&EPGRDebugRenderIfType);//モジュールに追加
}
PyObject* newEPGRDebugRenderIf(struct GRDebugRenderIf org)
{
	EPGRDebugRenderIf *ret = EPGRDebugRenderIf_new(&EPGRDebugRenderIfType,NULL,NULL);
	ret->ptr = new GRDebugRenderIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRDebugRenderIf(struct GRDebugRenderIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRDebugRenderIf *ret = EPGRDebugRenderIf_new(&EPGRDebugRenderIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRDebugRenderIf

//{*********EPGRSceneDesc*******
static PyMethodDef EPGRSceneDesc_methods[] =
{
	{NULL}
};
void EPGRSceneDesc_dealloc(PyObject* self)
{
	delete ((EPGRSceneDesc*)self)->ptr;
}
PyObject* EPGRSceneDesc_str()
{
	return Py_BuildValue("s","This is EPGRSceneDesc.");
}
int EPGRSceneDesc_init(EPGRSceneDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSceneDesc* EPGRSceneDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSceneDesc *self;
	self = (EPGRSceneDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSceneDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSceneDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSceneDesc",/*tp_name*/
	sizeof(EPGRSceneDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSceneDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSceneDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSceneDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSceneDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSceneDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSceneDesc_new,/*tp_new*/
};
void initEPGRSceneDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSceneDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSceneDescType);
	PyModule_AddObject(m,"GRSceneDesc",(PyObject *)&EPGRSceneDescType);//モジュールに追加
}
PyObject* newEPGRSceneDesc(struct GRSceneDesc org)
{
	EPGRSceneDesc *ret = EPGRSceneDesc_new(&EPGRSceneDescType,NULL,NULL);
	ret->ptr = new GRSceneDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSceneDesc(struct GRSceneDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSceneDesc *ret = EPGRSceneDesc_new(&EPGRSceneDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSceneDesc

//{*********EPGRSceneIf*******
PyObject* EPGRSceneIf_CreateVisual( EPGRSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPGRVisualDesc_Check(((EPGRVisualDesc*)PyTuple_GetItem(tuple,1))) && EPGRFrameIf_Check(((EPGRFrameIf*)PyTuple_GetItem(tuple,2))))
	{
		EPGRVisualIf* ret = (EPGRVisualIf*)newEPGRVisualIf(self->ptr->CreateVisual((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPGRVisualDesc*)PyTuple_GetItem(tuple,1))->ptr)),(((EPGRFrameIf*)PyTuple_GetItem(tuple,2))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPGRVisualDesc_Check(((EPGRVisualDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPGRVisualIf* ret = (EPGRVisualIf*)newEPGRVisualIf(self->ptr->CreateVisual((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPGRVisualDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSceneIf_GetAnimationController( EPGRSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRAnimationControllerIf* ret = (EPGRAnimationControllerIf*)newEPGRAnimationControllerIf(self->ptr->GetAnimationController());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSceneIf_GetCamera( EPGRSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRCameraIf* ret = (EPGRCameraIf*)newEPGRCameraIf(self->ptr->GetCamera());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSceneIf_GetSdk( EPGRSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRSdkIf* ret = (EPGRSdkIf*)newEPGRSdkIf(self->ptr->GetSdk());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSceneIf_GetWorld( EPGRSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRFrameIf* ret = (EPGRFrameIf*)newEPGRFrameIf(self->ptr->GetWorld());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSceneIf_Render( EPGRSceneIf* self,EPGRRenderIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRRenderIf_Check(var1))
	{
		self->ptr->Render((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSceneIf_SetCamera( EPGRSceneIf* self,EPGRCameraDesc* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRCameraDesc_Check(var1))
	{
		self->ptr->SetCamera((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRSceneIf_methods[] =
{
	{"CreateVisual",(PyCFunction)EPGRSceneIf_CreateVisual,METH_VARARGS,"EPGRSceneIf::CreateVisual"},
	{"GetAnimationController",(PyCFunction)EPGRSceneIf_GetAnimationController,METH_NOARGS,"EPGRSceneIf::GetAnimationController"},
	{"GetCamera",(PyCFunction)EPGRSceneIf_GetCamera,METH_NOARGS,"EPGRSceneIf::GetCamera"},
	{"GetSdk",(PyCFunction)EPGRSceneIf_GetSdk,METH_NOARGS,"EPGRSceneIf::GetSdk"},
	{"GetWorld",(PyCFunction)EPGRSceneIf_GetWorld,METH_NOARGS,"EPGRSceneIf::GetWorld"},
	{"Render",(PyCFunction)EPGRSceneIf_Render,METH_O,"EPGRSceneIf::Render"},
	{"SetCamera",(PyCFunction)EPGRSceneIf_SetCamera,METH_O,"EPGRSceneIf::SetCamera"},
	{NULL}
};
void EPGRSceneIf_dealloc(PyObject* self)
{
}
PyObject* EPGRSceneIf_str()
{
	return Py_BuildValue("s","This is EPGRSceneIf.");
}
int EPGRSceneIf_init(EPGRSceneIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSceneIf* EPGRSceneIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSceneIf *self;
	self = (EPGRSceneIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSceneIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSceneIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSceneIf",/*tp_name*/
	sizeof(EPGRSceneIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSceneIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSceneIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSceneIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSceneIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSceneIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSceneIf_new,/*tp_new*/
};
void initEPGRSceneIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSceneIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSceneIfType);
	PyModule_AddObject(m,"GRSceneIf",(PyObject *)&EPGRSceneIfType);//モジュールに追加
}
PyObject* newEPGRSceneIf(struct GRSceneIf org)
{
	EPGRSceneIf *ret = EPGRSceneIf_new(&EPGRSceneIfType,NULL,NULL);
	ret->ptr = new GRSceneIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSceneIf(struct GRSceneIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSceneIf *ret = EPGRSceneIf_new(&EPGRSceneIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSceneIf

//{*********EPGRSdkDesc*******
static PyMethodDef EPGRSdkDesc_methods[] =
{
	{NULL}
};
void EPGRSdkDesc_dealloc(PyObject* self)
{
	delete ((EPGRSdkDesc*)self)->ptr;
}
PyObject* EPGRSdkDesc_str()
{
	return Py_BuildValue("s","This is EPGRSdkDesc.");
}
int EPGRSdkDesc_init(EPGRSdkDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSdkDesc* EPGRSdkDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSdkDesc *self;
	self = (EPGRSdkDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSdkDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSdkDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSdkDesc",/*tp_name*/
	sizeof(EPGRSdkDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSdkDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSdkDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSdkDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSdkDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSdkDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSdkDesc_new,/*tp_new*/
};
void initEPGRSdkDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSdkDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSdkDescType);
	PyModule_AddObject(m,"GRSdkDesc",(PyObject *)&EPGRSdkDescType);//モジュールに追加
}
PyObject* newEPGRSdkDesc(struct GRSdkDesc org)
{
	EPGRSdkDesc *ret = EPGRSdkDesc_new(&EPGRSdkDescType,NULL,NULL);
	ret->ptr = new GRSdkDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSdkDesc(struct GRSdkDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSdkDesc *ret = EPGRSdkDesc_new(&EPGRSdkDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSdkDesc

//{*********EPGRSdkIf*******
PyObject* EPGRSdkIf_CreateDebugRender( EPGRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRDebugRenderIf* ret = (EPGRDebugRenderIf*)newEPGRDebugRenderIf(self->ptr->CreateDebugRender());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_CreateDeviceGL( EPGRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRDeviceGLIf* ret = (EPGRDeviceGLIf*)newEPGRDeviceGLIf(self->ptr->CreateDeviceGL());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_CreateScene( EPGRSdkIf* self,EPGRSceneDesc* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRSceneDesc_Check(var1))
	{
		EPGRSceneIf* ret = (EPGRSceneIf*)newEPGRSceneIf(self->ptr->CreateScene((*(var1->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_CreateSdk( EPGRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPGRSdkIf* ret = (EPGRSdkIf*)newEPGRSdkIf(GRSdkIf::CreateSdk());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_GetScene( EPGRSdkIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRSceneIf* ret = (EPGRSceneIf*)newEPGRSceneIf(self->ptr->GetScene(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_MergeScene( EPGRSdkIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPGRSceneIf_Check(((EPGRSceneIf*)PyTuple_GetItem(tuple,0))) && EPGRSceneIf_Check(((EPGRSceneIf*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->MergeScene((((EPGRSceneIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPGRSceneIf*)PyTuple_GetItem(tuple,1))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_NScene( EPGRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("n",self->ptr->NScene());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRSdkIf_RegisterSdk( EPGRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->RegisterSdk();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRSdkIf_methods[] =
{
	{"CreateDebugRender",(PyCFunction)EPGRSdkIf_CreateDebugRender,METH_NOARGS,"EPGRSdkIf::CreateDebugRender"},
	{"CreateDeviceGL",(PyCFunction)EPGRSdkIf_CreateDeviceGL,METH_NOARGS,"EPGRSdkIf::CreateDeviceGL"},
	{"CreateScene",(PyCFunction)EPGRSdkIf_CreateScene,METH_O,"EPGRSdkIf::CreateScene"},
	{"CreateSdk",(PyCFunction)EPGRSdkIf_CreateSdk,METH_NOARGS,"EPGRSdkIf::CreateSdk"},
	{"GetScene",(PyCFunction)EPGRSdkIf_GetScene,METH_O,"EPGRSdkIf::GetScene"},
	{"MergeScene",(PyCFunction)EPGRSdkIf_MergeScene,METH_VARARGS,"EPGRSdkIf::MergeScene"},
	{"NScene",(PyCFunction)EPGRSdkIf_NScene,METH_NOARGS,"EPGRSdkIf::NScene"},
	{"RegisterSdk",(PyCFunction)EPGRSdkIf_RegisterSdk,METH_NOARGS,"EPGRSdkIf::RegisterSdk"},
	{NULL}
};
void EPGRSdkIf_dealloc(PyObject* self)
{
}
PyObject* EPGRSdkIf_str()
{
	return Py_BuildValue("s","This is EPGRSdkIf.");
}
int EPGRSdkIf_init(EPGRSdkIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSdkIf* EPGRSdkIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSdkIf *self;
	self = (EPGRSdkIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSdkIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSdkIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSdkIf",/*tp_name*/
	sizeof(EPGRSdkIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSdkIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSdkIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSdkIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSdkIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSdkIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSdkIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSdkIf_new,/*tp_new*/
};
void initEPGRSdkIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSdkIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSdkIfType);
	PyModule_AddObject(m,"GRSdkIf",(PyObject *)&EPGRSdkIfType);//モジュールに追加
}
PyObject* newEPGRSdkIf(struct GRSdkIf org)
{
	EPGRSdkIf *ret = EPGRSdkIf_new(&EPGRSdkIfType,NULL,NULL);
	ret->ptr = new GRSdkIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSdkIf(struct GRSdkIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSdkIf *ret = EPGRSdkIf_new(&EPGRSdkIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSdkIf

//{*********EPSFBlendLocation*******
static PyMethodDef EPSFBlendLocation_methods[] =
{
	{NULL}
};
void EPSFBlendLocation_dealloc(PyObject* self)
{
	delete ((EPSFBlendLocation*)self)->ptr;
}
PyObject* EPSFBlendLocation_str()
{
	return Py_BuildValue("s","This is EPSFBlendLocation.");
}
int EPSFBlendLocation_init(EPSFBlendLocation* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPSFBlendLocation* EPSFBlendLocation_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPSFBlendLocation *self;
	self = (EPSFBlendLocation*) type->tp_alloc(type,0);
	if ( self != NULL ) EPSFBlendLocation_init(self,args,kwds);
	return self;
}
PyTypeObject EPSFBlendLocationType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPSFBlendLocation",/*tp_name*/
	sizeof(EPSFBlendLocation),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPSFBlendLocation_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPSFBlendLocation_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"SFBlendLocation",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPSFBlendLocation_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPSFBlendLocation_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPSFBlendLocation_new,/*tp_new*/
};
void initEPSFBlendLocation(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPSFBlendLocationType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPSFBlendLocationType);
	PyModule_AddObject(m,"SFBlendLocation",(PyObject *)&EPSFBlendLocationType);//モジュールに追加
}
PyObject* newEPSFBlendLocation(struct SFBlendLocation org)
{
	EPSFBlendLocation *ret = EPSFBlendLocation_new(&EPSFBlendLocationType,NULL,NULL);
	ret->ptr = new SFBlendLocation();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPSFBlendLocation(struct SFBlendLocation* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPSFBlendLocation *ret = EPSFBlendLocation_new(&EPSFBlendLocationType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}SFBlendLocation

//{*********EPGRShaderFormat*******
static PyMethodDef EPGRShaderFormat_methods[] =
{
	{NULL}
};
void EPGRShaderFormat_dealloc(PyObject* self)
{
	delete ((EPGRShaderFormat*)self)->ptr;
}
PyObject* EPGRShaderFormat_str()
{
	return Py_BuildValue("s","This is EPGRShaderFormat.");
}
int EPGRShaderFormat_init(EPGRShaderFormat* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRShaderFormat* EPGRShaderFormat_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRShaderFormat *self;
	self = (EPGRShaderFormat*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRShaderFormat_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRShaderFormatType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRShaderFormat",/*tp_name*/
	sizeof(EPGRShaderFormat),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRShaderFormat_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRShaderFormat_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRShaderFormat",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRShaderFormat_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRShaderFormat_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRShaderFormat_new,/*tp_new*/
};
void initEPGRShaderFormat(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRShaderFormatType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRShaderFormatType);
	PyModule_AddObject(m,"GRShaderFormat",(PyObject *)&EPGRShaderFormatType);//モジュールに追加
}
PyObject* newEPGRShaderFormat(struct GRShaderFormat org)
{
	EPGRShaderFormat *ret = EPGRShaderFormat_new(&EPGRShaderFormatType,NULL,NULL);
	ret->ptr = new GRShaderFormat();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRShaderFormat(struct GRShaderFormat* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRShaderFormat *ret = EPGRShaderFormat_new(&EPGRShaderFormatType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRShaderFormat

//{*********EPGRSphereDesc*******
static PyMethodDef EPGRSphereDesc_methods[] =
{
	{NULL}
};
void EPGRSphereDesc_dealloc(PyObject* self)
{
	delete ((EPGRSphereDesc*)self)->ptr;
}
PyObject* EPGRSphereDesc_str()
{
	return Py_BuildValue("s","This is EPGRSphereDesc.");
}
int EPGRSphereDesc_init(EPGRSphereDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSphereDesc* EPGRSphereDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSphereDesc *self;
	self = (EPGRSphereDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSphereDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSphereDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSphereDesc",/*tp_name*/
	sizeof(EPGRSphereDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSphereDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSphereDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSphereDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSphereDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSphereDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSphereDesc_new,/*tp_new*/
};
void initEPGRSphereDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSphereDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSphereDescType);
	PyModule_AddObject(m,"GRSphereDesc",(PyObject *)&EPGRSphereDescType);//モジュールに追加
}
PyObject* newEPGRSphereDesc(struct GRSphereDesc org)
{
	EPGRSphereDesc *ret = EPGRSphereDesc_new(&EPGRSphereDescType,NULL,NULL);
	ret->ptr = new GRSphereDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSphereDesc(struct GRSphereDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSphereDesc *ret = EPGRSphereDesc_new(&EPGRSphereDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSphereDesc

//{*********EPGRSphereIf*******
static PyMethodDef EPGRSphereIf_methods[] =
{
	{NULL}
};
void EPGRSphereIf_dealloc(PyObject* self)
{
}
PyObject* EPGRSphereIf_str()
{
	return Py_BuildValue("s","This is EPGRSphereIf.");
}
int EPGRSphereIf_init(EPGRSphereIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRSphereIf* EPGRSphereIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRSphereIf *self;
	self = (EPGRSphereIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRSphereIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRSphereIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRSphereIf",/*tp_name*/
	sizeof(EPGRSphereIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRSphereIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRSphereIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRSphereIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRSphereIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPGRVisualIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRSphereIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRSphereIf_new,/*tp_new*/
};
void initEPGRSphereIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRSphereIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRSphereIfType);
	PyModule_AddObject(m,"GRSphereIf",(PyObject *)&EPGRSphereIfType);//モジュールに追加
}
PyObject* newEPGRSphereIf(struct GRSphereIf org)
{
	EPGRSphereIf *ret = EPGRSphereIf_new(&EPGRSphereIfType,NULL,NULL);
	ret->ptr = new GRSphereIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRSphereIf(struct GRSphereIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRSphereIf *ret = EPGRSphereIf_new(&EPGRSphereIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRSphereIf

//{*********EPGRVertexElement*******
PyObject* EPGRVertexElement_Blend1f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Blend1f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Blend2f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Blend2f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Blend3f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Blend3f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Blend4f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Blend4f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_BlendNf( EPGRVertexElement* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::BlendNf(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Color3f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Color3f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Color4b( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Color4b(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Color4f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Color4f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_MatrixIndices4f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::MatrixIndices4f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_MatrixIndicesNf( EPGRVertexElement* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::MatrixIndicesNf(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Normal3f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Normal3f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_NumBones4f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::NumBones4f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_NumBonesNf( EPGRVertexElement* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::NumBonesNf(PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsSsize_t((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Position3f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Position3f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_Position4f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::Position4f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_TexCoords2f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::TexCoords2f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPGRVertexElement_TexCoords4f( EPGRVertexElement* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPGRVertexElement* ret = (EPGRVertexElement*)newEPGRVertexElement(GRVertexElement::TexCoords4f(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPGRVertexElement_methods[] =
{
	{"Blend1f",(PyCFunction)EPGRVertexElement_Blend1f,METH_O,"EPGRVertexElement::Blend1f"},
	{"Blend2f",(PyCFunction)EPGRVertexElement_Blend2f,METH_O,"EPGRVertexElement::Blend2f"},
	{"Blend3f",(PyCFunction)EPGRVertexElement_Blend3f,METH_O,"EPGRVertexElement::Blend3f"},
	{"Blend4f",(PyCFunction)EPGRVertexElement_Blend4f,METH_O,"EPGRVertexElement::Blend4f"},
	{"BlendNf",(PyCFunction)EPGRVertexElement_BlendNf,METH_VARARGS,"EPGRVertexElement::BlendNf"},
	{"Color3f",(PyCFunction)EPGRVertexElement_Color3f,METH_O,"EPGRVertexElement::Color3f"},
	{"Color4b",(PyCFunction)EPGRVertexElement_Color4b,METH_O,"EPGRVertexElement::Color4b"},
	{"Color4f",(PyCFunction)EPGRVertexElement_Color4f,METH_O,"EPGRVertexElement::Color4f"},
	{"MatrixIndices4f",(PyCFunction)EPGRVertexElement_MatrixIndices4f,METH_O,"EPGRVertexElement::MatrixIndices4f"},
	{"MatrixIndicesNf",(PyCFunction)EPGRVertexElement_MatrixIndicesNf,METH_VARARGS,"EPGRVertexElement::MatrixIndicesNf"},
	{"Normal3f",(PyCFunction)EPGRVertexElement_Normal3f,METH_O,"EPGRVertexElement::Normal3f"},
	{"NumBones4f",(PyCFunction)EPGRVertexElement_NumBones4f,METH_O,"EPGRVertexElement::NumBones4f"},
	{"NumBonesNf",(PyCFunction)EPGRVertexElement_NumBonesNf,METH_VARARGS,"EPGRVertexElement::NumBonesNf"},
	{"Position3f",(PyCFunction)EPGRVertexElement_Position3f,METH_O,"EPGRVertexElement::Position3f"},
	{"Position4f",(PyCFunction)EPGRVertexElement_Position4f,METH_O,"EPGRVertexElement::Position4f"},
	{"TexCoords2f",(PyCFunction)EPGRVertexElement_TexCoords2f,METH_O,"EPGRVertexElement::TexCoords2f"},
	{"TexCoords4f",(PyCFunction)EPGRVertexElement_TexCoords4f,METH_O,"EPGRVertexElement::TexCoords4f"},
	{NULL}
};
void EPGRVertexElement_dealloc(PyObject* self)
{
	delete ((EPGRVertexElement*)self)->ptr;
}
PyObject* EPGRVertexElement_str()
{
	return Py_BuildValue("s","This is EPGRVertexElement.");
}
int EPGRVertexElement_init(EPGRVertexElement* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRVertexElement* EPGRVertexElement_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRVertexElement *self;
	self = (EPGRVertexElement*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRVertexElement_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRVertexElementType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRVertexElement",/*tp_name*/
	sizeof(EPGRVertexElement),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRVertexElement_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRVertexElement_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRVertexElement",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRVertexElement_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRVertexElement_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRVertexElement_new,/*tp_new*/
};
void initEPGRVertexElement(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRVertexElementType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRVertexElementType);
	PyModule_AddObject(m,"GRVertexElement",(PyObject *)&EPGRVertexElementType);//モジュールに追加
}
PyObject* newEPGRVertexElement(struct GRVertexElement org)
{
	EPGRVertexElement *ret = EPGRVertexElement_new(&EPGRVertexElementType,NULL,NULL);
	ret->ptr = new GRVertexElement();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRVertexElement(struct GRVertexElement* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRVertexElement *ret = EPGRVertexElement_new(&EPGRVertexElementType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRVertexElement

//{*********EPGRVertexArray*******
static PyMethodDef EPGRVertexArray_methods[] =
{
	{NULL}
};
void EPGRVertexArray_dealloc(PyObject* self)
{
	delete ((EPGRVertexArray*)self)->ptr;
}
PyObject* EPGRVertexArray_str()
{
	return Py_BuildValue("s","This is EPGRVertexArray.");
}
int EPGRVertexArray_init(EPGRVertexArray* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPGRVertexArray* EPGRVertexArray_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPGRVertexArray *self;
	self = (EPGRVertexArray*) type->tp_alloc(type,0);
	if ( self != NULL ) EPGRVertexArray_init(self,args,kwds);
	return self;
}
PyTypeObject EPGRVertexArrayType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Graphics.EPGRVertexArray",/*tp_name*/
	sizeof(EPGRVertexArray),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPGRVertexArray_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPGRVertexArray_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"GRVertexArray",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPGRVertexArray_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPGRVertexArray_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPGRVertexArray_new,/*tp_new*/
};
void initEPGRVertexArray(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPGRVertexArrayType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Graphics");
	Py_INCREF(&EPGRVertexArrayType);
	PyModule_AddObject(m,"GRVertexArray",(PyObject *)&EPGRVertexArrayType);//モジュールに追加
}
PyObject* newEPGRVertexArray(struct GRVertexArray org)
{
	EPGRVertexArray *ret = EPGRVertexArray_new(&EPGRVertexArrayType,NULL,NULL);
	ret->ptr = new GRVertexArray();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPGRVertexArray(struct GRVertexArray* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPGRVertexArray *ret = EPGRVertexArray_new(&EPGRVertexArrayType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}GRVertexArray
/**************** for Module ******************/
void initGraphics(void)
{
	initEPGRBlendMeshDesc();
	initEPGRBlendMeshIf();
	initEPGRVisualDesc();
	initEPGRVisualIf();
	initEPGRFrameTransformMatrix();
	initEPGRFrameIf();
	initEPGRFrameDesc();
	initEPGRDummyFrameIf();
	initEPGRDummyFrameDesc();
	initEPGRKey();
	initEPGRAnimationKey();
	initEPGRAnimationIf();
	initEPGRAnimationDesc();
	initEPGRAnimationSetIf();
	initEPGRAnimationSetDesc();
	initEPGRAnimationControllerIf();
	initEPGRAnimationControllerDesc();
	initEPGRMeshFace();
	initEPGRSkinWeightIf();
	initEPGRSkinWeightDesc();
	initEPGRMeshIf();
	initEPGRMeshDesc();
	initEPGRFont();
	initEPGRLightIf();
	initEPGRLightDesc();
	initEPGRMaterialIf();
	initEPGRMaterialDesc();
	initEPGRCameraIf();
	initEPGRCameraDesc();
	initEPGRRenderBaseIf();
	initEPGRRenderIf();
	initEPGRDeviceIf();
	initEPGRDeviceGLIf();
	initEPGRDebugRenderIf();
	initEPGRSceneDesc();
	initEPGRSceneIf();
	initEPGRSdkDesc();
	initEPGRSdkIf();
	initEPSFBlendLocation();
	initEPGRShaderFormat();
	initEPGRSphereDesc();
	initEPGRSphereIf();
	initEPGRVertexElement();
	initEPGRVertexArray();
}
