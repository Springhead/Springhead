#include "../../include\EmbPython\SprEPCreature.h" 

//{*********EPCRBodyPartIf*******
PyObject* EPCRBodyPartIf_GetLabel( EPCRBodyPartIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPchar* ret = (EPchar*)newEPchar(self->ptr->GetLabel());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRBodyPartIf_SetLabel( EPCRBodyPartIf* self,EPchar* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPchar_Check(var1))
	{
		self->ptr->SetLabel((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRBodyPartIf_methods[] =
{
	{"GetLabel",(PyCFunction)EPCRBodyPartIf_GetLabel,METH_NOARGS,"EPCRBodyPartIf::GetLabel"},
	{"SetLabel",(PyCFunction)EPCRBodyPartIf_SetLabel,METH_O,"EPCRBodyPartIf::SetLabel"},
	{NULL}
};
void EPCRBodyPartIf_dealloc(PyObject* self)
{
}
PyObject* EPCRBodyPartIf_str()
{
	return Py_BuildValue("s","This is EPCRBodyPartIf.");
}
int EPCRBodyPartIf_init(EPCRBodyPartIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRBodyPartIf* EPCRBodyPartIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRBodyPartIf *self;
	self = (EPCRBodyPartIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRBodyPartIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRBodyPartIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRBodyPartIf",/*tp_name*/
	sizeof(EPCRBodyPartIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRBodyPartIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRBodyPartIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRBodyPartIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRBodyPartIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRBodyPartIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRBodyPartIf_new,/*tp_new*/
};
void initEPCRBodyPartIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRBodyPartIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRBodyPartIfType);
	PyModule_AddObject(m,"CRBodyPartIf",(PyObject *)&EPCRBodyPartIfType);//モジュールに追加
}
PyObject* newEPCRBodyPartIf(struct CRBodyPartIf org)
{
	EPCRBodyPartIf *ret = EPCRBodyPartIf_new(&EPCRBodyPartIfType,NULL,NULL);
	ret->ptr = new CRBodyPartIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRBodyPartIf(struct CRBodyPartIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRBodyPartIf *ret = EPCRBodyPartIf_new(&EPCRBodyPartIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRBodyPartIf

//{*********EPCRBodyPartDesc*******
static PyMethodDef EPCRBodyPartDesc_methods[] =
{
	{NULL}
};
void EPCRBodyPartDesc_dealloc(PyObject* self)
{
	delete ((EPCRBodyPartDesc*)self)->ptr;
}
PyObject* EPCRBodyPartDesc_str()
{
	return Py_BuildValue("s","This is EPCRBodyPartDesc.");
}
int EPCRBodyPartDesc_init(EPCRBodyPartDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRBodyPartDesc* EPCRBodyPartDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRBodyPartDesc *self;
	self = (EPCRBodyPartDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRBodyPartDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRBodyPartDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRBodyPartDesc",/*tp_name*/
	sizeof(EPCRBodyPartDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRBodyPartDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRBodyPartDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRBodyPartDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRBodyPartDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRBodyPartDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRBodyPartDesc_new,/*tp_new*/
};
void initEPCRBodyPartDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRBodyPartDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRBodyPartDescType);
	PyModule_AddObject(m,"CRBodyPartDesc",(PyObject *)&EPCRBodyPartDescType);//モジュールに追加
}
PyObject* newEPCRBodyPartDesc(struct CRBodyPartDesc org)
{
	EPCRBodyPartDesc *ret = EPCRBodyPartDesc_new(&EPCRBodyPartDescType,NULL,NULL);
	ret->ptr = new CRBodyPartDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRBodyPartDesc(struct CRBodyPartDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRBodyPartDesc *ret = EPCRBodyPartDesc_new(&EPCRBodyPartDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRBodyPartDesc

//{*********EPCRSolidIf*******
PyObject* EPCRSolidIf_GetPHSolid( EPCRSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->GetPHSolid());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRSolidIf_SetPHSolid( EPCRSolidIf* self,EPPHSolidIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHSolidIf_Check(var1))
	{
		self->ptr->SetPHSolid((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRSolidIf_methods[] =
{
	{"GetPHSolid",(PyCFunction)EPCRSolidIf_GetPHSolid,METH_NOARGS,"EPCRSolidIf::GetPHSolid"},
	{"SetPHSolid",(PyCFunction)EPCRSolidIf_SetPHSolid,METH_O,"EPCRSolidIf::SetPHSolid"},
	{NULL}
};
void EPCRSolidIf_dealloc(PyObject* self)
{
}
PyObject* EPCRSolidIf_str()
{
	return Py_BuildValue("s","This is EPCRSolidIf.");
}
int EPCRSolidIf_init(EPCRSolidIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRSolidIf* EPCRSolidIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRSolidIf *self;
	self = (EPCRSolidIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRSolidIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRSolidIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRSolidIf",/*tp_name*/
	sizeof(EPCRSolidIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRSolidIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRSolidIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRSolidIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRSolidIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRBodyPartIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRSolidIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRSolidIf_new,/*tp_new*/
};
void initEPCRSolidIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRSolidIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRSolidIfType);
	PyModule_AddObject(m,"CRSolidIf",(PyObject *)&EPCRSolidIfType);//モジュールに追加
}
PyObject* newEPCRSolidIf(struct CRSolidIf org)
{
	EPCRSolidIf *ret = EPCRSolidIf_new(&EPCRSolidIfType,NULL,NULL);
	ret->ptr = new CRSolidIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRSolidIf(struct CRSolidIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRSolidIf *ret = EPCRSolidIf_new(&EPCRSolidIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRSolidIf

//{*********EPCRSolidDesc*******
static PyMethodDef EPCRSolidDesc_methods[] =
{
	{NULL}
};
void EPCRSolidDesc_dealloc(PyObject* self)
{
	delete ((EPCRSolidDesc*)self)->ptr;
}
PyObject* EPCRSolidDesc_str()
{
	return Py_BuildValue("s","This is EPCRSolidDesc.");
}
int EPCRSolidDesc_init(EPCRSolidDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRSolidDesc* EPCRSolidDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRSolidDesc *self;
	self = (EPCRSolidDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRSolidDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRSolidDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRSolidDesc",/*tp_name*/
	sizeof(EPCRSolidDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRSolidDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRSolidDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRSolidDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRSolidDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRBodyPartDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRSolidDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRSolidDesc_new,/*tp_new*/
};
void initEPCRSolidDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRSolidDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRSolidDescType);
	PyModule_AddObject(m,"CRSolidDesc",(PyObject *)&EPCRSolidDescType);//モジュールに追加
}
PyObject* newEPCRSolidDesc(struct CRSolidDesc org)
{
	EPCRSolidDesc *ret = EPCRSolidDesc_new(&EPCRSolidDescType,NULL,NULL);
	ret->ptr = new CRSolidDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRSolidDesc(struct CRSolidDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRSolidDesc *ret = EPCRSolidDesc_new(&EPCRSolidDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRSolidDesc

//{*********EPCRIKSolidIf*******
PyObject* EPCRIKSolidIf_GetIKEndEffector( EPCRIKSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHIKEndEffectorIf* ret = (EPPHIKEndEffectorIf*)newEPPHIKEndEffectorIf(self->ptr->GetIKEndEffector());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRIKSolidIf_SetIKEndEffector( EPCRIKSolidIf* self,EPPHIKEndEffectorIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHIKEndEffectorIf_Check(var1))
	{
		self->ptr->SetIKEndEffector((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRIKSolidIf_methods[] =
{
	{"GetIKEndEffector",(PyCFunction)EPCRIKSolidIf_GetIKEndEffector,METH_NOARGS,"EPCRIKSolidIf::GetIKEndEffector"},
	{"SetIKEndEffector",(PyCFunction)EPCRIKSolidIf_SetIKEndEffector,METH_O,"EPCRIKSolidIf::SetIKEndEffector"},
	{NULL}
};
void EPCRIKSolidIf_dealloc(PyObject* self)
{
}
PyObject* EPCRIKSolidIf_str()
{
	return Py_BuildValue("s","This is EPCRIKSolidIf.");
}
int EPCRIKSolidIf_init(EPCRIKSolidIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRIKSolidIf* EPCRIKSolidIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRIKSolidIf *self;
	self = (EPCRIKSolidIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRIKSolidIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRIKSolidIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRIKSolidIf",/*tp_name*/
	sizeof(EPCRIKSolidIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRIKSolidIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRIKSolidIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRIKSolidIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRIKSolidIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRSolidIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRIKSolidIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRIKSolidIf_new,/*tp_new*/
};
void initEPCRIKSolidIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRIKSolidIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRIKSolidIfType);
	PyModule_AddObject(m,"CRIKSolidIf",(PyObject *)&EPCRIKSolidIfType);//モジュールに追加
}
PyObject* newEPCRIKSolidIf(struct CRIKSolidIf org)
{
	EPCRIKSolidIf *ret = EPCRIKSolidIf_new(&EPCRIKSolidIfType,NULL,NULL);
	ret->ptr = new CRIKSolidIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRIKSolidIf(struct CRIKSolidIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRIKSolidIf *ret = EPCRIKSolidIf_new(&EPCRIKSolidIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRIKSolidIf

//{*********EPCRIKSolidDesc*******
static PyMethodDef EPCRIKSolidDesc_methods[] =
{
	{NULL}
};
void EPCRIKSolidDesc_dealloc(PyObject* self)
{
	delete ((EPCRIKSolidDesc*)self)->ptr;
}
PyObject* EPCRIKSolidDesc_str()
{
	return Py_BuildValue("s","This is EPCRIKSolidDesc.");
}
int EPCRIKSolidDesc_init(EPCRIKSolidDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRIKSolidDesc* EPCRIKSolidDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRIKSolidDesc *self;
	self = (EPCRIKSolidDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRIKSolidDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRIKSolidDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRIKSolidDesc",/*tp_name*/
	sizeof(EPCRIKSolidDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRIKSolidDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRIKSolidDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRIKSolidDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRIKSolidDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRSolidDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRIKSolidDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRIKSolidDesc_new,/*tp_new*/
};
void initEPCRIKSolidDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRIKSolidDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRIKSolidDescType);
	PyModule_AddObject(m,"CRIKSolidDesc",(PyObject *)&EPCRIKSolidDescType);//モジュールに追加
}
PyObject* newEPCRIKSolidDesc(struct CRIKSolidDesc org)
{
	EPCRIKSolidDesc *ret = EPCRIKSolidDesc_new(&EPCRIKSolidDescType,NULL,NULL);
	ret->ptr = new CRIKSolidDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRIKSolidDesc(struct CRIKSolidDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRIKSolidDesc *ret = EPCRIKSolidDesc_new(&EPCRIKSolidDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRIKSolidDesc

//{*********EPCRJointIf*******
PyObject* EPCRJointIf_GetPHJoint( EPCRJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHJointIf* ret = (EPPHJointIf*)newEPPHJointIf(self->ptr->GetPHJoint());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRJointIf_SetPHJoint( EPCRJointIf* self,EPPHJointIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHJointIf_Check(var1))
	{
		self->ptr->SetPHJoint((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRJointIf_SetSpringRatio( EPCRJointIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetSpringRatio(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRJointIf_methods[] =
{
	{"GetPHJoint",(PyCFunction)EPCRJointIf_GetPHJoint,METH_NOARGS,"EPCRJointIf::GetPHJoint"},
	{"SetPHJoint",(PyCFunction)EPCRJointIf_SetPHJoint,METH_O,"EPCRJointIf::SetPHJoint"},
	{"SetSpringRatio",(PyCFunction)EPCRJointIf_SetSpringRatio,METH_VARARGS,"EPCRJointIf::SetSpringRatio"},
	{NULL}
};
void EPCRJointIf_dealloc(PyObject* self)
{
}
PyObject* EPCRJointIf_str()
{
	return Py_BuildValue("s","This is EPCRJointIf.");
}
int EPCRJointIf_init(EPCRJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRJointIf* EPCRJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRJointIf *self;
	self = (EPCRJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRJointIf",/*tp_name*/
	sizeof(EPCRJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRBodyPartIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRJointIf_new,/*tp_new*/
};
void initEPCRJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRJointIfType);
	PyModule_AddObject(m,"CRJointIf",(PyObject *)&EPCRJointIfType);//モジュールに追加
}
PyObject* newEPCRJointIf(struct CRJointIf org)
{
	EPCRJointIf *ret = EPCRJointIf_new(&EPCRJointIfType,NULL,NULL);
	ret->ptr = new CRJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRJointIf(struct CRJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRJointIf *ret = EPCRJointIf_new(&EPCRJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRJointIf

//{*********EPCRJointDesc*******
static PyMethodDef EPCRJointDesc_methods[] =
{
	{NULL}
};
void EPCRJointDesc_dealloc(PyObject* self)
{
	delete ((EPCRJointDesc*)self)->ptr;
}
PyObject* EPCRJointDesc_str()
{
	return Py_BuildValue("s","This is EPCRJointDesc.");
}
int EPCRJointDesc_init(EPCRJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRJointDesc* EPCRJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRJointDesc *self;
	self = (EPCRJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRJointDesc",/*tp_name*/
	sizeof(EPCRJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRBodyPartDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRJointDesc_new,/*tp_new*/
};
void initEPCRJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRJointDescType);
	PyModule_AddObject(m,"CRJointDesc",(PyObject *)&EPCRJointDescType);//モジュールに追加
}
PyObject* newEPCRJointDesc(struct CRJointDesc org)
{
	EPCRJointDesc *ret = EPCRJointDesc_new(&EPCRJointDescType,NULL,NULL);
	ret->ptr = new CRJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRJointDesc(struct CRJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRJointDesc *ret = EPCRJointDesc_new(&EPCRJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRJointDesc

//{*********EPCRIKJointIf*******
PyObject* EPCRIKJointIf_GetIKActuator( EPCRIKJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHIKActuatorIf* ret = (EPPHIKActuatorIf*)newEPPHIKActuatorIf(self->ptr->GetIKActuator());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRIKJointIf_SetIKActuator( EPCRIKJointIf* self,EPPHIKActuatorIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHIKActuatorIf_Check(var1))
	{
		self->ptr->SetIKActuator((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRIKJointIf_SetIKSpringRatio( EPCRIKJointIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetIKSpringRatio(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRIKJointIf_methods[] =
{
	{"GetIKActuator",(PyCFunction)EPCRIKJointIf_GetIKActuator,METH_NOARGS,"EPCRIKJointIf::GetIKActuator"},
	{"SetIKActuator",(PyCFunction)EPCRIKJointIf_SetIKActuator,METH_O,"EPCRIKJointIf::SetIKActuator"},
	{"SetIKSpringRatio",(PyCFunction)EPCRIKJointIf_SetIKSpringRatio,METH_VARARGS,"EPCRIKJointIf::SetIKSpringRatio"},
	{NULL}
};
void EPCRIKJointIf_dealloc(PyObject* self)
{
}
PyObject* EPCRIKJointIf_str()
{
	return Py_BuildValue("s","This is EPCRIKJointIf.");
}
int EPCRIKJointIf_init(EPCRIKJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRIKJointIf* EPCRIKJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRIKJointIf *self;
	self = (EPCRIKJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRIKJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRIKJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRIKJointIf",/*tp_name*/
	sizeof(EPCRIKJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRIKJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRIKJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRIKJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRIKJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRJointIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRIKJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRIKJointIf_new,/*tp_new*/
};
void initEPCRIKJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRIKJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRIKJointIfType);
	PyModule_AddObject(m,"CRIKJointIf",(PyObject *)&EPCRIKJointIfType);//モジュールに追加
}
PyObject* newEPCRIKJointIf(struct CRIKJointIf org)
{
	EPCRIKJointIf *ret = EPCRIKJointIf_new(&EPCRIKJointIfType,NULL,NULL);
	ret->ptr = new CRIKJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRIKJointIf(struct CRIKJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRIKJointIf *ret = EPCRIKJointIf_new(&EPCRIKJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRIKJointIf

//{*********EPCRIKJointDesc*******
static PyMethodDef EPCRIKJointDesc_methods[] =
{
	{NULL}
};
void EPCRIKJointDesc_dealloc(PyObject* self)
{
	delete ((EPCRIKJointDesc*)self)->ptr;
}
PyObject* EPCRIKJointDesc_str()
{
	return Py_BuildValue("s","This is EPCRIKJointDesc.");
}
int EPCRIKJointDesc_init(EPCRIKJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRIKJointDesc* EPCRIKJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRIKJointDesc *self;
	self = (EPCRIKJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRIKJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRIKJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRIKJointDesc",/*tp_name*/
	sizeof(EPCRIKJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRIKJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRIKJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRIKJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRIKJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRJointDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRIKJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRIKJointDesc_new,/*tp_new*/
};
void initEPCRIKJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRIKJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRIKJointDescType);
	PyModule_AddObject(m,"CRIKJointDesc",(PyObject *)&EPCRIKJointDescType);//モジュールに追加
}
PyObject* newEPCRIKJointDesc(struct CRIKJointDesc org)
{
	EPCRIKJointDesc *ret = EPCRIKJointDesc_new(&EPCRIKJointDescType,NULL,NULL);
	ret->ptr = new CRIKJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRIKJointDesc(struct CRIKJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRIKJointDesc *ret = EPCRIKJointDesc_new(&EPCRIKJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRIKJointDesc

//{*********EPCRBodyIf*******
PyObject* EPCRBodyIf_GetCenterOfMass( EPCRBodyIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetCenterOfMass());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRBodyIf_GetJoint( EPCRBodyIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCRJointIf* ret = (EPCRJointIf*)newEPCRJointIf(self->ptr->GetJoint(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRBodyIf_GetSolid( EPCRBodyIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCRSolidIf* ret = (EPCRSolidIf*)newEPCRSolidIf(self->ptr->GetSolid(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRBodyIf_GetSumOfMass( EPCRBodyIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSumOfMass());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRBodyIf_NJoints( EPCRBodyIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NJoints());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRBodyIf_NSolids( EPCRBodyIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NSolids());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRBodyIf_methods[] =
{
	{"GetCenterOfMass",(PyCFunction)EPCRBodyIf_GetCenterOfMass,METH_NOARGS,"EPCRBodyIf::GetCenterOfMass"},
	{"GetJoint",(PyCFunction)EPCRBodyIf_GetJoint,METH_O,"EPCRBodyIf::GetJoint"},
	{"GetSolid",(PyCFunction)EPCRBodyIf_GetSolid,METH_O,"EPCRBodyIf::GetSolid"},
	{"GetSumOfMass",(PyCFunction)EPCRBodyIf_GetSumOfMass,METH_NOARGS,"EPCRBodyIf::GetSumOfMass"},
	{"NJoints",(PyCFunction)EPCRBodyIf_NJoints,METH_NOARGS,"EPCRBodyIf::NJoints"},
	{"NSolids",(PyCFunction)EPCRBodyIf_NSolids,METH_NOARGS,"EPCRBodyIf::NSolids"},
	{NULL}
};
void EPCRBodyIf_dealloc(PyObject* self)
{
}
PyObject* EPCRBodyIf_str()
{
	return Py_BuildValue("s","This is EPCRBodyIf.");
}
int EPCRBodyIf_init(EPCRBodyIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRBodyIf* EPCRBodyIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRBodyIf *self;
	self = (EPCRBodyIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRBodyIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRBodyIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRBodyIf",/*tp_name*/
	sizeof(EPCRBodyIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRBodyIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRBodyIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRBodyIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRBodyIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRBodyIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRBodyIf_new,/*tp_new*/
};
void initEPCRBodyIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRBodyIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRBodyIfType);
	PyModule_AddObject(m,"CRBodyIf",(PyObject *)&EPCRBodyIfType);//モジュールに追加
}
PyObject* newEPCRBodyIf(struct CRBodyIf org)
{
	EPCRBodyIf *ret = EPCRBodyIf_new(&EPCRBodyIfType,NULL,NULL);
	ret->ptr = new CRBodyIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRBodyIf(struct CRBodyIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRBodyIf *ret = EPCRBodyIf_new(&EPCRBodyIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRBodyIf

//{*********EPCRBodyDesc*******
static PyMethodDef EPCRBodyDesc_methods[] =
{
	{NULL}
};
void EPCRBodyDesc_dealloc(PyObject* self)
{
	delete ((EPCRBodyDesc*)self)->ptr;
}
PyObject* EPCRBodyDesc_str()
{
	return Py_BuildValue("s","This is EPCRBodyDesc.");
}
int EPCRBodyDesc_init(EPCRBodyDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRBodyDesc* EPCRBodyDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRBodyDesc *self;
	self = (EPCRBodyDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRBodyDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRBodyDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRBodyDesc",/*tp_name*/
	sizeof(EPCRBodyDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRBodyDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRBodyDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRBodyDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRBodyDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRBodyDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRBodyDesc_new,/*tp_new*/
};
void initEPCRBodyDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRBodyDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRBodyDescType);
	PyModule_AddObject(m,"CRBodyDesc",(PyObject *)&EPCRBodyDescType);//モジュールに追加
}
PyObject* newEPCRBodyDesc(struct CRBodyDesc org)
{
	EPCRBodyDesc *ret = EPCRBodyDesc_new(&EPCRBodyDescType,NULL,NULL);
	ret->ptr = new CRBodyDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRBodyDesc(struct CRBodyDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRBodyDesc *ret = EPCRBodyDesc_new(&EPCRBodyDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRBodyDesc

//{*********EPCRCreatureIf*******
PyObject* EPCRCreatureIf_CreateBody( EPCRCreatureIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPCRBodyDesc_Check(((EPCRBodyDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPCRBodyIf* ret = (EPCRBodyIf*)newEPCRBodyIf(self->ptr->CreateBody((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPCRBodyDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_CreateEngine( EPCRCreatureIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPCREngineDesc_Check(((EPCREngineDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPCREngineIf* ret = (EPCREngineIf*)newEPCREngineIf(self->ptr->CreateEngine((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPCREngineDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_CreateScene( EPCRCreatureIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPCRSceneDesc_Check(((EPCRSceneDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPCRSceneIf* ret = (EPCRSceneIf*)newEPCRSceneIf(self->ptr->CreateScene((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPCRSceneDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_GetBody( EPCRCreatureIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCRBodyIf* ret = (EPCRBodyIf*)newEPCRBodyIf(self->ptr->GetBody(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_GetEngine( EPCRCreatureIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCREngineIf* ret = (EPCREngineIf*)newEPCREngineIf(self->ptr->GetEngine(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_GetPHScene( EPCRCreatureIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSceneIf* ret = (EPPHSceneIf*)newEPPHSceneIf(self->ptr->GetPHScene());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_GetScene( EPCRCreatureIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCRSceneIf* ret = (EPCRSceneIf*)newEPCRSceneIf(self->ptr->GetScene(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_NBodies( EPCRCreatureIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NBodies());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_NEngines( EPCRCreatureIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NEngines());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_NScenes( EPCRCreatureIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NScenes());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRCreatureIf_Step( EPCRCreatureIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Step();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRCreatureIf_methods[] =
{
	{"CreateBody",(PyCFunction)EPCRCreatureIf_CreateBody,METH_VARARGS,"EPCRCreatureIf::CreateBody"},
	{"CreateEngine",(PyCFunction)EPCRCreatureIf_CreateEngine,METH_VARARGS,"EPCRCreatureIf::CreateEngine"},
	{"CreateScene",(PyCFunction)EPCRCreatureIf_CreateScene,METH_VARARGS,"EPCRCreatureIf::CreateScene"},
	{"GetBody",(PyCFunction)EPCRCreatureIf_GetBody,METH_O,"EPCRCreatureIf::GetBody"},
	{"GetEngine",(PyCFunction)EPCRCreatureIf_GetEngine,METH_O,"EPCRCreatureIf::GetEngine"},
	{"GetPHScene",(PyCFunction)EPCRCreatureIf_GetPHScene,METH_NOARGS,"EPCRCreatureIf::GetPHScene"},
	{"GetScene",(PyCFunction)EPCRCreatureIf_GetScene,METH_O,"EPCRCreatureIf::GetScene"},
	{"NBodies",(PyCFunction)EPCRCreatureIf_NBodies,METH_NOARGS,"EPCRCreatureIf::NBodies"},
	{"NEngines",(PyCFunction)EPCRCreatureIf_NEngines,METH_NOARGS,"EPCRCreatureIf::NEngines"},
	{"NScenes",(PyCFunction)EPCRCreatureIf_NScenes,METH_NOARGS,"EPCRCreatureIf::NScenes"},
	{"Step",(PyCFunction)EPCRCreatureIf_Step,METH_NOARGS,"EPCRCreatureIf::Step"},
	{NULL}
};
void EPCRCreatureIf_dealloc(PyObject* self)
{
}
PyObject* EPCRCreatureIf_str()
{
	return Py_BuildValue("s","This is EPCRCreatureIf.");
}
int EPCRCreatureIf_init(EPCRCreatureIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRCreatureIf* EPCRCreatureIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRCreatureIf *self;
	self = (EPCRCreatureIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRCreatureIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRCreatureIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRCreatureIf",/*tp_name*/
	sizeof(EPCRCreatureIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRCreatureIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRCreatureIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRCreatureIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRCreatureIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRCreatureIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRCreatureIf_new,/*tp_new*/
};
void initEPCRCreatureIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRCreatureIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRCreatureIfType);
	PyModule_AddObject(m,"CRCreatureIf",(PyObject *)&EPCRCreatureIfType);//モジュールに追加
}
PyObject* newEPCRCreatureIf(struct CRCreatureIf org)
{
	EPCRCreatureIf *ret = EPCRCreatureIf_new(&EPCRCreatureIfType,NULL,NULL);
	ret->ptr = new CRCreatureIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRCreatureIf(struct CRCreatureIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRCreatureIf *ret = EPCRCreatureIf_new(&EPCRCreatureIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRCreatureIf

//{*********EPCRCreatureDesc*******
static PyMethodDef EPCRCreatureDesc_methods[] =
{
	{NULL}
};
void EPCRCreatureDesc_dealloc(PyObject* self)
{
	delete ((EPCRCreatureDesc*)self)->ptr;
}
PyObject* EPCRCreatureDesc_str()
{
	return Py_BuildValue("s","This is EPCRCreatureDesc.");
}
int EPCRCreatureDesc_init(EPCRCreatureDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRCreatureDesc* EPCRCreatureDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRCreatureDesc *self;
	self = (EPCRCreatureDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRCreatureDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRCreatureDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRCreatureDesc",/*tp_name*/
	sizeof(EPCRCreatureDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRCreatureDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRCreatureDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRCreatureDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRCreatureDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRCreatureDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRCreatureDesc_new,/*tp_new*/
};
void initEPCRCreatureDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRCreatureDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRCreatureDescType);
	PyModule_AddObject(m,"CRCreatureDesc",(PyObject *)&EPCRCreatureDescType);//モジュールに追加
}
PyObject* newEPCRCreatureDesc(struct CRCreatureDesc org)
{
	EPCRCreatureDesc *ret = EPCRCreatureDesc_new(&EPCRCreatureDescType,NULL,NULL);
	ret->ptr = new CRCreatureDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRCreatureDesc(struct CRCreatureDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRCreatureDesc *ret = EPCRCreatureDesc_new(&EPCRCreatureDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRCreatureDesc

//{*********EPCREngineIf*******
PyObject* EPCREngineIf_GetPriority( EPCREngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetPriority());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCREngineIf_IsEnabled( EPCREngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsEnabled()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCREngineIf_Render( EPCREngineIf* self,EPGRRenderIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPGRRenderIf_Check(var1))
	{
		self->ptr->Render((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCREngineIf_SetEnable( EPCREngineIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetEnable((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCREngineIf_Step( EPCREngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Step();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCREngineIf_methods[] =
{
	{"GetPriority",(PyCFunction)EPCREngineIf_GetPriority,METH_NOARGS,"EPCREngineIf::GetPriority"},
	{"IsEnabled",(PyCFunction)EPCREngineIf_IsEnabled,METH_NOARGS,"EPCREngineIf::IsEnabled"},
	{"Render",(PyCFunction)EPCREngineIf_Render,METH_O,"EPCREngineIf::Render"},
	{"SetEnable",(PyCFunction)EPCREngineIf_SetEnable,METH_O,"EPCREngineIf::SetEnable"},
	{"Step",(PyCFunction)EPCREngineIf_Step,METH_NOARGS,"EPCREngineIf::Step"},
	{NULL}
};
void EPCREngineIf_dealloc(PyObject* self)
{
}
PyObject* EPCREngineIf_str()
{
	return Py_BuildValue("s","This is EPCREngineIf.");
}
int EPCREngineIf_init(EPCREngineIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCREngineIf* EPCREngineIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCREngineIf *self;
	self = (EPCREngineIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCREngineIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCREngineIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCREngineIf",/*tp_name*/
	sizeof(EPCREngineIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCREngineIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCREngineIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CREngineIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCREngineIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCREngineIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCREngineIf_new,/*tp_new*/
};
void initEPCREngineIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCREngineIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCREngineIfType);
	PyModule_AddObject(m,"CREngineIf",(PyObject *)&EPCREngineIfType);//モジュールに追加
}
PyObject* newEPCREngineIf(struct CREngineIf org)
{
	EPCREngineIf *ret = EPCREngineIf_new(&EPCREngineIfType,NULL,NULL);
	ret->ptr = new CREngineIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCREngineIf(struct CREngineIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCREngineIf *ret = EPCREngineIf_new(&EPCREngineIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CREngineIf

//{*********EPCREngineDesc*******
static PyMethodDef EPCREngineDesc_methods[] =
{
	{NULL}
};
void EPCREngineDesc_dealloc(PyObject* self)
{
	delete ((EPCREngineDesc*)self)->ptr;
}
PyObject* EPCREngineDesc_str()
{
	return Py_BuildValue("s","This is EPCREngineDesc.");
}
int EPCREngineDesc_init(EPCREngineDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCREngineDesc* EPCREngineDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCREngineDesc *self;
	self = (EPCREngineDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCREngineDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCREngineDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCREngineDesc",/*tp_name*/
	sizeof(EPCREngineDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCREngineDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCREngineDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CREngineDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCREngineDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCREngineDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCREngineDesc_new,/*tp_new*/
};
void initEPCREngineDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCREngineDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCREngineDescType);
	PyModule_AddObject(m,"CREngineDesc",(PyObject *)&EPCREngineDescType);//モジュールに追加
}
PyObject* newEPCREngineDesc(struct CREngineDesc org)
{
	EPCREngineDesc *ret = EPCREngineDesc_new(&EPCREngineDescType,NULL,NULL);
	ret->ptr = new CREngineDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCREngineDesc(struct CREngineDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCREngineDesc *ret = EPCREngineDesc_new(&EPCREngineDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CREngineDesc

//{*********EPCREyeControllerIf*******
PyObject* EPCREyeControllerIf_IsSaccading( EPCREyeControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsSaccading()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCREyeControllerIf_LookAt( EPCREyeControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->LookAt((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCREyeControllerIf_methods[] =
{
	{"IsSaccading",(PyCFunction)EPCREyeControllerIf_IsSaccading,METH_NOARGS,"EPCREyeControllerIf::IsSaccading"},
	{"LookAt",(PyCFunction)EPCREyeControllerIf_LookAt,METH_VARARGS,"EPCREyeControllerIf::LookAt"},
	{NULL}
};
void EPCREyeControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCREyeControllerIf_str()
{
	return Py_BuildValue("s","This is EPCREyeControllerIf.");
}
int EPCREyeControllerIf_init(EPCREyeControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCREyeControllerIf* EPCREyeControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCREyeControllerIf *self;
	self = (EPCREyeControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCREyeControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCREyeControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCREyeControllerIf",/*tp_name*/
	sizeof(EPCREyeControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCREyeControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCREyeControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CREyeControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCREyeControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCREyeControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCREyeControllerIf_new,/*tp_new*/
};
void initEPCREyeControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCREyeControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCREyeControllerIfType);
	PyModule_AddObject(m,"CREyeControllerIf",(PyObject *)&EPCREyeControllerIfType);//モジュールに追加
}
PyObject* newEPCREyeControllerIf(struct CREyeControllerIf org)
{
	EPCREyeControllerIf *ret = EPCREyeControllerIf_new(&EPCREyeControllerIfType,NULL,NULL);
	ret->ptr = new CREyeControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCREyeControllerIf(struct CREyeControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCREyeControllerIf *ret = EPCREyeControllerIf_new(&EPCREyeControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CREyeControllerIf

//{*********EPCREyeControllerState*******
static PyMethodDef EPCREyeControllerState_methods[] =
{
	{NULL}
};
void EPCREyeControllerState_dealloc(PyObject* self)
{
	delete ((EPCREyeControllerState*)self)->ptr;
}
PyObject* EPCREyeControllerState_str()
{
	return Py_BuildValue("s","This is EPCREyeControllerState.");
}
int EPCREyeControllerState_init(EPCREyeControllerState* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCREyeControllerState* EPCREyeControllerState_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCREyeControllerState *self;
	self = (EPCREyeControllerState*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCREyeControllerState_init(self,args,kwds);
	return self;
}
PyTypeObject EPCREyeControllerStateType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCREyeControllerState",/*tp_name*/
	sizeof(EPCREyeControllerState),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCREyeControllerState_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCREyeControllerState_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CREyeControllerState",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCREyeControllerState_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCREyeControllerState_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCREyeControllerState_new,/*tp_new*/
};
void initEPCREyeControllerState(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCREyeControllerStateType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCREyeControllerStateType);
	PyModule_AddObject(m,"CREyeControllerState",(PyObject *)&EPCREyeControllerStateType);//モジュールに追加
}
PyObject* newEPCREyeControllerState(struct CREyeControllerState org)
{
	EPCREyeControllerState *ret = EPCREyeControllerState_new(&EPCREyeControllerStateType,NULL,NULL);
	ret->ptr = new CREyeControllerState();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCREyeControllerState(struct CREyeControllerState* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCREyeControllerState *ret = EPCREyeControllerState_new(&EPCREyeControllerStateType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CREyeControllerState

//{*********EPCREyeControllerDesc*******
static PyMethodDef EPCREyeControllerDesc_methods[] =
{
	{NULL}
};
void EPCREyeControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCREyeControllerDesc*)self)->ptr;
}
PyObject* EPCREyeControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCREyeControllerDesc.");
}
int EPCREyeControllerDesc_init(EPCREyeControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCREyeControllerDesc* EPCREyeControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCREyeControllerDesc *self;
	self = (EPCREyeControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCREyeControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCREyeControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCREyeControllerDesc",/*tp_name*/
	sizeof(EPCREyeControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCREyeControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCREyeControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CREyeControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCREyeControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREyeControllerStateType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCREyeControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCREyeControllerDesc_new,/*tp_new*/
};
void initEPCREyeControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCREyeControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCREyeControllerDescType);
	PyModule_AddObject(m,"CREyeControllerDesc",(PyObject *)&EPCREyeControllerDescType);//モジュールに追加
}
PyObject* newEPCREyeControllerDesc(struct CREyeControllerDesc org)
{
	EPCREyeControllerDesc *ret = EPCREyeControllerDesc_new(&EPCREyeControllerDescType,NULL,NULL);
	ret->ptr = new CREyeControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCREyeControllerDesc(struct CREyeControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCREyeControllerDesc *ret = EPCREyeControllerDesc_new(&EPCREyeControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CREyeControllerDesc

//{*********EPCRNeckControllerIf*******
PyObject* EPCRNeckControllerIf_LookAt( EPCRNeckControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->LookAt((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRNeckControllerIf_methods[] =
{
	{"LookAt",(PyCFunction)EPCRNeckControllerIf_LookAt,METH_VARARGS,"EPCRNeckControllerIf::LookAt"},
	{NULL}
};
void EPCRNeckControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCRNeckControllerIf_str()
{
	return Py_BuildValue("s","This is EPCRNeckControllerIf.");
}
int EPCRNeckControllerIf_init(EPCRNeckControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRNeckControllerIf* EPCRNeckControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRNeckControllerIf *self;
	self = (EPCRNeckControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRNeckControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRNeckControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRNeckControllerIf",/*tp_name*/
	sizeof(EPCRNeckControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRNeckControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRNeckControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRNeckControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRNeckControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRNeckControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRNeckControllerIf_new,/*tp_new*/
};
void initEPCRNeckControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRNeckControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRNeckControllerIfType);
	PyModule_AddObject(m,"CRNeckControllerIf",(PyObject *)&EPCRNeckControllerIfType);//モジュールに追加
}
PyObject* newEPCRNeckControllerIf(struct CRNeckControllerIf org)
{
	EPCRNeckControllerIf *ret = EPCRNeckControllerIf_new(&EPCRNeckControllerIfType,NULL,NULL);
	ret->ptr = new CRNeckControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRNeckControllerIf(struct CRNeckControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRNeckControllerIf *ret = EPCRNeckControllerIf_new(&EPCRNeckControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRNeckControllerIf

//{*********EPCRNeckControllerState*******
static PyMethodDef EPCRNeckControllerState_methods[] =
{
	{NULL}
};
void EPCRNeckControllerState_dealloc(PyObject* self)
{
	delete ((EPCRNeckControllerState*)self)->ptr;
}
PyObject* EPCRNeckControllerState_str()
{
	return Py_BuildValue("s","This is EPCRNeckControllerState.");
}
int EPCRNeckControllerState_init(EPCRNeckControllerState* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRNeckControllerState* EPCRNeckControllerState_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRNeckControllerState *self;
	self = (EPCRNeckControllerState*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRNeckControllerState_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRNeckControllerStateType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRNeckControllerState",/*tp_name*/
	sizeof(EPCRNeckControllerState),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRNeckControllerState_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRNeckControllerState_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRNeckControllerState",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRNeckControllerState_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRNeckControllerState_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRNeckControllerState_new,/*tp_new*/
};
void initEPCRNeckControllerState(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRNeckControllerStateType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRNeckControllerStateType);
	PyModule_AddObject(m,"CRNeckControllerState",(PyObject *)&EPCRNeckControllerStateType);//モジュールに追加
}
PyObject* newEPCRNeckControllerState(struct CRNeckControllerState org)
{
	EPCRNeckControllerState *ret = EPCRNeckControllerState_new(&EPCRNeckControllerStateType,NULL,NULL);
	ret->ptr = new CRNeckControllerState();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRNeckControllerState(struct CRNeckControllerState* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRNeckControllerState *ret = EPCRNeckControllerState_new(&EPCRNeckControllerStateType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRNeckControllerState

//{*********EPCRNeckControllerDesc*******
static PyMethodDef EPCRNeckControllerDesc_methods[] =
{
	{NULL}
};
void EPCRNeckControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCRNeckControllerDesc*)self)->ptr;
}
PyObject* EPCRNeckControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCRNeckControllerDesc.");
}
int EPCRNeckControllerDesc_init(EPCRNeckControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRNeckControllerDesc* EPCRNeckControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRNeckControllerDesc *self;
	self = (EPCRNeckControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRNeckControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRNeckControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRNeckControllerDesc",/*tp_name*/
	sizeof(EPCRNeckControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRNeckControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRNeckControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRNeckControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRNeckControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRNeckControllerStateType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRNeckControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRNeckControllerDesc_new,/*tp_new*/
};
void initEPCRNeckControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRNeckControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRNeckControllerDescType);
	PyModule_AddObject(m,"CRNeckControllerDesc",(PyObject *)&EPCRNeckControllerDescType);//モジュールに追加
}
PyObject* newEPCRNeckControllerDesc(struct CRNeckControllerDesc org)
{
	EPCRNeckControllerDesc *ret = EPCRNeckControllerDesc_new(&EPCRNeckControllerDescType,NULL,NULL);
	ret->ptr = new CRNeckControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRNeckControllerDesc(struct CRNeckControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRNeckControllerDesc *ret = EPCRNeckControllerDesc_new(&EPCRNeckControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRNeckControllerDesc

//{*********EPCRGazeControllerIf*******
PyObject* EPCRGazeControllerIf_LookAt( EPCRGazeControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))) && EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->LookAt((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3f*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRGazeControllerIf_methods[] =
{
	{"LookAt",(PyCFunction)EPCRGazeControllerIf_LookAt,METH_VARARGS,"EPCRGazeControllerIf::LookAt"},
	{NULL}
};
void EPCRGazeControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCRGazeControllerIf_str()
{
	return Py_BuildValue("s","This is EPCRGazeControllerIf.");
}
int EPCRGazeControllerIf_init(EPCRGazeControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRGazeControllerIf* EPCRGazeControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRGazeControllerIf *self;
	self = (EPCRGazeControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRGazeControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRGazeControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRGazeControllerIf",/*tp_name*/
	sizeof(EPCRGazeControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRGazeControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRGazeControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRGazeControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRGazeControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRGazeControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRGazeControllerIf_new,/*tp_new*/
};
void initEPCRGazeControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRGazeControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRGazeControllerIfType);
	PyModule_AddObject(m,"CRGazeControllerIf",(PyObject *)&EPCRGazeControllerIfType);//モジュールに追加
}
PyObject* newEPCRGazeControllerIf(struct CRGazeControllerIf org)
{
	EPCRGazeControllerIf *ret = EPCRGazeControllerIf_new(&EPCRGazeControllerIfType,NULL,NULL);
	ret->ptr = new CRGazeControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRGazeControllerIf(struct CRGazeControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRGazeControllerIf *ret = EPCRGazeControllerIf_new(&EPCRGazeControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRGazeControllerIf

//{*********EPCRGazeControllerState*******
static PyMethodDef EPCRGazeControllerState_methods[] =
{
	{NULL}
};
void EPCRGazeControllerState_dealloc(PyObject* self)
{
	delete ((EPCRGazeControllerState*)self)->ptr;
}
PyObject* EPCRGazeControllerState_str()
{
	return Py_BuildValue("s","This is EPCRGazeControllerState.");
}
int EPCRGazeControllerState_init(EPCRGazeControllerState* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRGazeControllerState* EPCRGazeControllerState_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRGazeControllerState *self;
	self = (EPCRGazeControllerState*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRGazeControllerState_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRGazeControllerStateType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRGazeControllerState",/*tp_name*/
	sizeof(EPCRGazeControllerState),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRGazeControllerState_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRGazeControllerState_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRGazeControllerState",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRGazeControllerState_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRGazeControllerState_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRGazeControllerState_new,/*tp_new*/
};
void initEPCRGazeControllerState(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRGazeControllerStateType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRGazeControllerStateType);
	PyModule_AddObject(m,"CRGazeControllerState",(PyObject *)&EPCRGazeControllerStateType);//モジュールに追加
}
PyObject* newEPCRGazeControllerState(struct CRGazeControllerState org)
{
	EPCRGazeControllerState *ret = EPCRGazeControllerState_new(&EPCRGazeControllerStateType,NULL,NULL);
	ret->ptr = new CRGazeControllerState();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRGazeControllerState(struct CRGazeControllerState* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRGazeControllerState *ret = EPCRGazeControllerState_new(&EPCRGazeControllerStateType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRGazeControllerState

//{*********EPCRGazeControllerDesc*******
static PyMethodDef EPCRGazeControllerDesc_methods[] =
{
	{NULL}
};
void EPCRGazeControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCRGazeControllerDesc*)self)->ptr;
}
PyObject* EPCRGazeControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCRGazeControllerDesc.");
}
int EPCRGazeControllerDesc_init(EPCRGazeControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRGazeControllerDesc* EPCRGazeControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRGazeControllerDesc *self;
	self = (EPCRGazeControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRGazeControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRGazeControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRGazeControllerDesc",/*tp_name*/
	sizeof(EPCRGazeControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRGazeControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRGazeControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRGazeControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRGazeControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRGazeControllerStateType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRGazeControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRGazeControllerDesc_new,/*tp_new*/
};
void initEPCRGazeControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRGazeControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRGazeControllerDescType);
	PyModule_AddObject(m,"CRGazeControllerDesc",(PyObject *)&EPCRGazeControllerDescType);//モジュールに追加
}
PyObject* newEPCRGazeControllerDesc(struct CRGazeControllerDesc org)
{
	EPCRGazeControllerDesc *ret = EPCRGazeControllerDesc_new(&EPCRGazeControllerDescType,NULL,NULL);
	ret->ptr = new CRGazeControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRGazeControllerDesc(struct CRGazeControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRGazeControllerDesc *ret = EPCRGazeControllerDesc_new(&EPCRGazeControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRGazeControllerDesc

//{*********EPCRAttentionControllerIf*******
static PyMethodDef EPCRAttentionControllerIf_methods[] =
{
	{NULL}
};
void EPCRAttentionControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCRAttentionControllerIf_str()
{
	return Py_BuildValue("s","This is EPCRAttentionControllerIf.");
}
int EPCRAttentionControllerIf_init(EPCRAttentionControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRAttentionControllerIf* EPCRAttentionControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRAttentionControllerIf *self;
	self = (EPCRAttentionControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRAttentionControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRAttentionControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRAttentionControllerIf",/*tp_name*/
	sizeof(EPCRAttentionControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRAttentionControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRAttentionControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRAttentionControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRAttentionControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRAttentionControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRAttentionControllerIf_new,/*tp_new*/
};
void initEPCRAttentionControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRAttentionControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRAttentionControllerIfType);
	PyModule_AddObject(m,"CRAttentionControllerIf",(PyObject *)&EPCRAttentionControllerIfType);//モジュールに追加
}
PyObject* newEPCRAttentionControllerIf(struct CRAttentionControllerIf org)
{
	EPCRAttentionControllerIf *ret = EPCRAttentionControllerIf_new(&EPCRAttentionControllerIfType,NULL,NULL);
	ret->ptr = new CRAttentionControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRAttentionControllerIf(struct CRAttentionControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRAttentionControllerIf *ret = EPCRAttentionControllerIf_new(&EPCRAttentionControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRAttentionControllerIf

//{*********EPCRAttentionControllerDesc*******
static PyMethodDef EPCRAttentionControllerDesc_methods[] =
{
	{NULL}
};
void EPCRAttentionControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCRAttentionControllerDesc*)self)->ptr;
}
PyObject* EPCRAttentionControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCRAttentionControllerDesc.");
}
int EPCRAttentionControllerDesc_init(EPCRAttentionControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRAttentionControllerDesc* EPCRAttentionControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRAttentionControllerDesc *self;
	self = (EPCRAttentionControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRAttentionControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRAttentionControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRAttentionControllerDesc",/*tp_name*/
	sizeof(EPCRAttentionControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRAttentionControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRAttentionControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRAttentionControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRAttentionControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRAttentionControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRAttentionControllerDesc_new,/*tp_new*/
};
void initEPCRAttentionControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRAttentionControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRAttentionControllerDescType);
	PyModule_AddObject(m,"CRAttentionControllerDesc",(PyObject *)&EPCRAttentionControllerDescType);//モジュールに追加
}
PyObject* newEPCRAttentionControllerDesc(struct CRAttentionControllerDesc org)
{
	EPCRAttentionControllerDesc *ret = EPCRAttentionControllerDesc_new(&EPCRAttentionControllerDescType,NULL,NULL);
	ret->ptr = new CRAttentionControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRAttentionControllerDesc(struct CRAttentionControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRAttentionControllerDesc *ret = EPCRAttentionControllerDesc_new(&EPCRAttentionControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRAttentionControllerDesc

//{*********EPCRReachingControllerIf*******
PyObject* EPCRReachingControllerIf_GetCRSolid( EPCRReachingControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPCRIKSolidIf* ret = (EPCRIKSolidIf*)newEPCRIKSolidIf(self->ptr->GetCRSolid());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRReachingControllerIf_SetCRSolid( EPCRReachingControllerIf* self,EPCRIKSolidIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPCRIKSolidIf_Check(var1))
	{
		self->ptr->SetCRSolid((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRReachingControllerIf_SetPos( EPCRReachingControllerIf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->SetPos((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRReachingControllerIf_Start( EPCRReachingControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,2))))
	{
		self->ptr->Start((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,2))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRReachingControllerIf_Stop( EPCRReachingControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Stop();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRReachingControllerIf_methods[] =
{
	{"GetCRSolid",(PyCFunction)EPCRReachingControllerIf_GetCRSolid,METH_NOARGS,"EPCRReachingControllerIf::GetCRSolid"},
	{"SetCRSolid",(PyCFunction)EPCRReachingControllerIf_SetCRSolid,METH_O,"EPCRReachingControllerIf::SetCRSolid"},
	{"SetPos",(PyCFunction)EPCRReachingControllerIf_SetPos,METH_O,"EPCRReachingControllerIf::SetPos"},
	{"Start",(PyCFunction)EPCRReachingControllerIf_Start,METH_VARARGS,"EPCRReachingControllerIf::Start"},
	{"Stop",(PyCFunction)EPCRReachingControllerIf_Stop,METH_NOARGS,"EPCRReachingControllerIf::Stop"},
	{NULL}
};
void EPCRReachingControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCRReachingControllerIf_str()
{
	return Py_BuildValue("s","This is EPCRReachingControllerIf.");
}
int EPCRReachingControllerIf_init(EPCRReachingControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRReachingControllerIf* EPCRReachingControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRReachingControllerIf *self;
	self = (EPCRReachingControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRReachingControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRReachingControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRReachingControllerIf",/*tp_name*/
	sizeof(EPCRReachingControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRReachingControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRReachingControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRReachingControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRReachingControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRReachingControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRReachingControllerIf_new,/*tp_new*/
};
void initEPCRReachingControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRReachingControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRReachingControllerIfType);
	PyModule_AddObject(m,"CRReachingControllerIf",(PyObject *)&EPCRReachingControllerIfType);//モジュールに追加
}
PyObject* newEPCRReachingControllerIf(struct CRReachingControllerIf org)
{
	EPCRReachingControllerIf *ret = EPCRReachingControllerIf_new(&EPCRReachingControllerIfType,NULL,NULL);
	ret->ptr = new CRReachingControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRReachingControllerIf(struct CRReachingControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRReachingControllerIf *ret = EPCRReachingControllerIf_new(&EPCRReachingControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRReachingControllerIf

//{*********EPCRReachingControllerDesc*******
static PyMethodDef EPCRReachingControllerDesc_methods[] =
{
	{NULL}
};
void EPCRReachingControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCRReachingControllerDesc*)self)->ptr;
}
PyObject* EPCRReachingControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCRReachingControllerDesc.");
}
int EPCRReachingControllerDesc_init(EPCRReachingControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRReachingControllerDesc* EPCRReachingControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRReachingControllerDesc *self;
	self = (EPCRReachingControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRReachingControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRReachingControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRReachingControllerDesc",/*tp_name*/
	sizeof(EPCRReachingControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRReachingControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRReachingControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRReachingControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRReachingControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRReachingControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRReachingControllerDesc_new,/*tp_new*/
};
void initEPCRReachingControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRReachingControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRReachingControllerDescType);
	PyModule_AddObject(m,"CRReachingControllerDesc",(PyObject *)&EPCRReachingControllerDescType);//モジュールに追加
}
PyObject* newEPCRReachingControllerDesc(struct CRReachingControllerDesc org)
{
	EPCRReachingControllerDesc *ret = EPCRReachingControllerDesc_new(&EPCRReachingControllerDescType,NULL,NULL);
	ret->ptr = new CRReachingControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRReachingControllerDesc(struct CRReachingControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRReachingControllerDesc *ret = EPCRReachingControllerDesc_new(&EPCRReachingControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRReachingControllerDesc

//{*********EPCRReachingControllersIf*******
PyObject* EPCRReachingControllersIf_GetReachingController( EPCRReachingControllersIf* self,EPPHSolidIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHSolidIf_Check(var1))
	{
		EPCRReachingControllerIf* ret = (EPCRReachingControllerIf*)newEPCRReachingControllerIf(self->ptr->GetReachingController((var1->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRReachingControllersIf_methods[] =
{
	{"GetReachingController",(PyCFunction)EPCRReachingControllersIf_GetReachingController,METH_O,"EPCRReachingControllersIf::GetReachingController"},
	{NULL}
};
void EPCRReachingControllersIf_dealloc(PyObject* self)
{
}
PyObject* EPCRReachingControllersIf_str()
{
	return Py_BuildValue("s","This is EPCRReachingControllersIf.");
}
int EPCRReachingControllersIf_init(EPCRReachingControllersIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRReachingControllersIf* EPCRReachingControllersIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRReachingControllersIf *self;
	self = (EPCRReachingControllersIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRReachingControllersIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRReachingControllersIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRReachingControllersIf",/*tp_name*/
	sizeof(EPCRReachingControllersIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRReachingControllersIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRReachingControllersIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRReachingControllersIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRReachingControllersIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRReachingControllersIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRReachingControllersIf_new,/*tp_new*/
};
void initEPCRReachingControllersIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRReachingControllersIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRReachingControllersIfType);
	PyModule_AddObject(m,"CRReachingControllersIf",(PyObject *)&EPCRReachingControllersIfType);//モジュールに追加
}
PyObject* newEPCRReachingControllersIf(struct CRReachingControllersIf org)
{
	EPCRReachingControllersIf *ret = EPCRReachingControllersIf_new(&EPCRReachingControllersIfType,NULL,NULL);
	ret->ptr = new CRReachingControllersIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRReachingControllersIf(struct CRReachingControllersIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRReachingControllersIf *ret = EPCRReachingControllersIf_new(&EPCRReachingControllersIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRReachingControllersIf

//{*********EPCRReachingControllersDesc*******
static PyMethodDef EPCRReachingControllersDesc_methods[] =
{
	{NULL}
};
void EPCRReachingControllersDesc_dealloc(PyObject* self)
{
	delete ((EPCRReachingControllersDesc*)self)->ptr;
}
PyObject* EPCRReachingControllersDesc_str()
{
	return Py_BuildValue("s","This is EPCRReachingControllersDesc.");
}
int EPCRReachingControllersDesc_init(EPCRReachingControllersDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRReachingControllersDesc* EPCRReachingControllersDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRReachingControllersDesc *self;
	self = (EPCRReachingControllersDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRReachingControllersDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRReachingControllersDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRReachingControllersDesc",/*tp_name*/
	sizeof(EPCRReachingControllersDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRReachingControllersDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRReachingControllersDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRReachingControllersDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRReachingControllersDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRReachingControllersDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRReachingControllersDesc_new,/*tp_new*/
};
void initEPCRReachingControllersDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRReachingControllersDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRReachingControllersDescType);
	PyModule_AddObject(m,"CRReachingControllersDesc",(PyObject *)&EPCRReachingControllersDescType);//モジュールに追加
}
PyObject* newEPCRReachingControllersDesc(struct CRReachingControllersDesc org)
{
	EPCRReachingControllersDesc *ret = EPCRReachingControllersDesc_new(&EPCRReachingControllersDescType,NULL,NULL);
	ret->ptr = new CRReachingControllersDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRReachingControllersDesc(struct CRReachingControllersDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRReachingControllersDesc *ret = EPCRReachingControllersDesc_new(&EPCRReachingControllersDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRReachingControllersDesc

//{*********EPCRGrabControllerIf*******
PyObject* EPCRGrabControllerIf_Abort( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Abort();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_AbortAll( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->AbortAll();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_GetControlState( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetControlState());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_IsPlaceComplete( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsPlaceComplete()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_IsPlaceable( EPCRGrabControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsPlaceable((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsPlaceable((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_IsReachComplete( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsReachComplete()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_IsReachable( EPCRGrabControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsReachable((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsReachable((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_IsUpholdComplete( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsUpholdComplete()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_IsUpholdable( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsUpholdable()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_Place( EPCRGrabControllerIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->Place((*(var1->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_Reach( EPCRGrabControllerIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->Reach((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRGrabControllerIf_Uphold( EPCRGrabControllerIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->Uphold()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRGrabControllerIf_methods[] =
{
	{"Abort",(PyCFunction)EPCRGrabControllerIf_Abort,METH_NOARGS,"EPCRGrabControllerIf::Abort"},
	{"AbortAll",(PyCFunction)EPCRGrabControllerIf_AbortAll,METH_NOARGS,"EPCRGrabControllerIf::AbortAll"},
	{"GetControlState",(PyCFunction)EPCRGrabControllerIf_GetControlState,METH_NOARGS,"EPCRGrabControllerIf::GetControlState"},
	{"IsPlaceComplete",(PyCFunction)EPCRGrabControllerIf_IsPlaceComplete,METH_NOARGS,"EPCRGrabControllerIf::IsPlaceComplete"},
	{"IsPlaceable",(PyCFunction)EPCRGrabControllerIf_IsPlaceable,METH_VARARGS,"EPCRGrabControllerIf::IsPlaceable"},
	{"IsReachComplete",(PyCFunction)EPCRGrabControllerIf_IsReachComplete,METH_NOARGS,"EPCRGrabControllerIf::IsReachComplete"},
	{"IsReachable",(PyCFunction)EPCRGrabControllerIf_IsReachable,METH_VARARGS,"EPCRGrabControllerIf::IsReachable"},
	{"IsUpholdComplete",(PyCFunction)EPCRGrabControllerIf_IsUpholdComplete,METH_NOARGS,"EPCRGrabControllerIf::IsUpholdComplete"},
	{"IsUpholdable",(PyCFunction)EPCRGrabControllerIf_IsUpholdable,METH_NOARGS,"EPCRGrabControllerIf::IsUpholdable"},
	{"Place",(PyCFunction)EPCRGrabControllerIf_Place,METH_O,"EPCRGrabControllerIf::Place"},
	{"Reach",(PyCFunction)EPCRGrabControllerIf_Reach,METH_VARARGS,"EPCRGrabControllerIf::Reach"},
	{"Uphold",(PyCFunction)EPCRGrabControllerIf_Uphold,METH_NOARGS,"EPCRGrabControllerIf::Uphold"},
	{NULL}
};
void EPCRGrabControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCRGrabControllerIf_str()
{
	return Py_BuildValue("s","This is EPCRGrabControllerIf.");
}
int EPCRGrabControllerIf_init(EPCRGrabControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRGrabControllerIf* EPCRGrabControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRGrabControllerIf *self;
	self = (EPCRGrabControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRGrabControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRGrabControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRGrabControllerIf",/*tp_name*/
	sizeof(EPCRGrabControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRGrabControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRGrabControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRGrabControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRGrabControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRGrabControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRGrabControllerIf_new,/*tp_new*/
};
void initEPCRGrabControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRGrabControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRGrabControllerIfType);
	PyModule_AddObject(m,"CRGrabControllerIf",(PyObject *)&EPCRGrabControllerIfType);//モジュールに追加
}
PyObject* newEPCRGrabControllerIf(struct CRGrabControllerIf org)
{
	EPCRGrabControllerIf *ret = EPCRGrabControllerIf_new(&EPCRGrabControllerIfType,NULL,NULL);
	ret->ptr = new CRGrabControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRGrabControllerIf(struct CRGrabControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRGrabControllerIf *ret = EPCRGrabControllerIf_new(&EPCRGrabControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRGrabControllerIf

//{*********EPCRGrabControllerDesc*******
static PyMethodDef EPCRGrabControllerDesc_methods[] =
{
	{NULL}
};
void EPCRGrabControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCRGrabControllerDesc*)self)->ptr;
}
PyObject* EPCRGrabControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCRGrabControllerDesc.");
}
int EPCRGrabControllerDesc_init(EPCRGrabControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRGrabControllerDesc* EPCRGrabControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRGrabControllerDesc *self;
	self = (EPCRGrabControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRGrabControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRGrabControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRGrabControllerDesc",/*tp_name*/
	sizeof(EPCRGrabControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRGrabControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRGrabControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRGrabControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRGrabControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRGrabControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRGrabControllerDesc_new,/*tp_new*/
};
void initEPCRGrabControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRGrabControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRGrabControllerDescType);
	PyModule_AddObject(m,"CRGrabControllerDesc",(PyObject *)&EPCRGrabControllerDescType);//モジュールに追加
}
PyObject* newEPCRGrabControllerDesc(struct CRGrabControllerDesc org)
{
	EPCRGrabControllerDesc *ret = EPCRGrabControllerDesc_new(&EPCRGrabControllerDescType,NULL,NULL);
	ret->ptr = new CRGrabControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRGrabControllerDesc(struct CRGrabControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRGrabControllerDesc *ret = EPCRGrabControllerDesc_new(&EPCRGrabControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRGrabControllerDesc

//{*********EPCRSceneIf*******
static PyMethodDef EPCRSceneIf_methods[] =
{
	{NULL}
};
void EPCRSceneIf_dealloc(PyObject* self)
{
}
PyObject* EPCRSceneIf_str()
{
	return Py_BuildValue("s","This is EPCRSceneIf.");
}
int EPCRSceneIf_init(EPCRSceneIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRSceneIf* EPCRSceneIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRSceneIf *self;
	self = (EPCRSceneIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRSceneIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRSceneIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRSceneIf",/*tp_name*/
	sizeof(EPCRSceneIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRSceneIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRSceneIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRSceneIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRSceneIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRSceneIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRSceneIf_new,/*tp_new*/
};
void initEPCRSceneIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRSceneIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRSceneIfType);
	PyModule_AddObject(m,"CRSceneIf",(PyObject *)&EPCRSceneIfType);//モジュールに追加
}
PyObject* newEPCRSceneIf(struct CRSceneIf org)
{
	EPCRSceneIf *ret = EPCRSceneIf_new(&EPCRSceneIfType,NULL,NULL);
	ret->ptr = new CRSceneIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRSceneIf(struct CRSceneIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRSceneIf *ret = EPCRSceneIf_new(&EPCRSceneIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRSceneIf

//{*********EPCRSceneDesc*******
static PyMethodDef EPCRSceneDesc_methods[] =
{
	{NULL}
};
void EPCRSceneDesc_dealloc(PyObject* self)
{
	delete ((EPCRSceneDesc*)self)->ptr;
}
PyObject* EPCRSceneDesc_str()
{
	return Py_BuildValue("s","This is EPCRSceneDesc.");
}
int EPCRSceneDesc_init(EPCRSceneDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRSceneDesc* EPCRSceneDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRSceneDesc *self;
	self = (EPCRSceneDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRSceneDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRSceneDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRSceneDesc",/*tp_name*/
	sizeof(EPCRSceneDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRSceneDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRSceneDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRSceneDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRSceneDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRSceneDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRSceneDesc_new,/*tp_new*/
};
void initEPCRSceneDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRSceneDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRSceneDescType);
	PyModule_AddObject(m,"CRSceneDesc",(PyObject *)&EPCRSceneDescType);//モジュールに追加
}
PyObject* newEPCRSceneDesc(struct CRSceneDesc org)
{
	EPCRSceneDesc *ret = EPCRSceneDesc_new(&EPCRSceneDescType,NULL,NULL);
	ret->ptr = new CRSceneDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRSceneDesc(struct CRSceneDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRSceneDesc *ret = EPCRSceneDesc_new(&EPCRSceneDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRSceneDesc

//{*********EPCRSdkIf*******
PyObject* EPCRSdkIf_CreateCreature( EPCRSdkIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPCRCreatureDesc_Check(((EPCRCreatureDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPCRCreatureIf* ret = (EPCRCreatureIf*)newEPCRCreatureIf(self->ptr->CreateCreature((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPCRCreatureDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRSdkIf_CreateSdk( EPCRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPCRSdkIf* ret = (EPCRSdkIf*)newEPCRSdkIf(CRSdkIf::CreateSdk());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRSdkIf_GetCreature( EPCRSdkIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCRCreatureIf* ret = (EPCRCreatureIf*)newEPCRCreatureIf(self->ptr->GetCreature(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRSdkIf_NCreatures( EPCRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NCreatures());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRSdkIf_RegisterSdk( EPCRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->RegisterSdk();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRSdkIf_Step( EPCRSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Step();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRSdkIf_methods[] =
{
	{"CreateCreature",(PyCFunction)EPCRSdkIf_CreateCreature,METH_VARARGS,"EPCRSdkIf::CreateCreature"},
	{"CreateSdk",(PyCFunction)EPCRSdkIf_CreateSdk,METH_NOARGS,"EPCRSdkIf::CreateSdk"},
	{"GetCreature",(PyCFunction)EPCRSdkIf_GetCreature,METH_O,"EPCRSdkIf::GetCreature"},
	{"NCreatures",(PyCFunction)EPCRSdkIf_NCreatures,METH_NOARGS,"EPCRSdkIf::NCreatures"},
	{"RegisterSdk",(PyCFunction)EPCRSdkIf_RegisterSdk,METH_NOARGS,"EPCRSdkIf::RegisterSdk"},
	{"Step",(PyCFunction)EPCRSdkIf_Step,METH_NOARGS,"EPCRSdkIf::Step"},
	{NULL}
};
void EPCRSdkIf_dealloc(PyObject* self)
{
}
PyObject* EPCRSdkIf_str()
{
	return Py_BuildValue("s","This is EPCRSdkIf.");
}
int EPCRSdkIf_init(EPCRSdkIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRSdkIf* EPCRSdkIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRSdkIf *self;
	self = (EPCRSdkIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRSdkIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRSdkIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRSdkIf",/*tp_name*/
	sizeof(EPCRSdkIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRSdkIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRSdkIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRSdkIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRSdkIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSdkIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRSdkIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRSdkIf_new,/*tp_new*/
};
void initEPCRSdkIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRSdkIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRSdkIfType);
	PyModule_AddObject(m,"CRSdkIf",(PyObject *)&EPCRSdkIfType);//モジュールに追加
}
PyObject* newEPCRSdkIf(struct CRSdkIf org)
{
	EPCRSdkIf *ret = EPCRSdkIf_new(&EPCRSdkIfType,NULL,NULL);
	ret->ptr = new CRSdkIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRSdkIf(struct CRSdkIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRSdkIf *ret = EPCRSdkIf_new(&EPCRSdkIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRSdkIf

//{*********EPCRSdkDesc*******
static PyMethodDef EPCRSdkDesc_methods[] =
{
	{NULL}
};
void EPCRSdkDesc_dealloc(PyObject* self)
{
	delete ((EPCRSdkDesc*)self)->ptr;
}
PyObject* EPCRSdkDesc_str()
{
	return Py_BuildValue("s","This is EPCRSdkDesc.");
}
int EPCRSdkDesc_init(EPCRSdkDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRSdkDesc* EPCRSdkDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRSdkDesc *self;
	self = (EPCRSdkDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRSdkDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRSdkDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRSdkDesc",/*tp_name*/
	sizeof(EPCRSdkDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRSdkDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRSdkDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRSdkDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRSdkDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRSdkDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRSdkDesc_new,/*tp_new*/
};
void initEPCRSdkDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRSdkDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRSdkDescType);
	PyModule_AddObject(m,"CRSdkDesc",(PyObject *)&EPCRSdkDescType);//モジュールに追加
}
PyObject* newEPCRSdkDesc(struct CRSdkDesc org)
{
	EPCRSdkDesc *ret = EPCRSdkDesc_new(&EPCRSdkDescType,NULL,NULL);
	ret->ptr = new CRSdkDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRSdkDesc(struct CRSdkDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRSdkDesc *ret = EPCRSdkDesc_new(&EPCRSdkDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRSdkDesc

//{*********EPCRContactInfo*******
static PyMethodDef EPCRContactInfo_methods[] =
{
	{NULL}
};
void EPCRContactInfo_dealloc(PyObject* self)
{
	delete ((EPCRContactInfo*)self)->ptr;
}
PyObject* EPCRContactInfo_str()
{
	return Py_BuildValue("s","This is EPCRContactInfo.");
}
int EPCRContactInfo_init(EPCRContactInfo* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRContactInfo* EPCRContactInfo_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRContactInfo *self;
	self = (EPCRContactInfo*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRContactInfo_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRContactInfoType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRContactInfo",/*tp_name*/
	sizeof(EPCRContactInfo),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRContactInfo_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRContactInfo_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRContactInfo",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRContactInfo_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRContactInfo_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRContactInfo_new,/*tp_new*/
};
void initEPCRContactInfo(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRContactInfoType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRContactInfoType);
	PyModule_AddObject(m,"CRContactInfo",(PyObject *)&EPCRContactInfoType);//モジュールに追加
}
PyObject* newEPCRContactInfo(struct CRContactInfo org)
{
	EPCRContactInfo *ret = EPCRContactInfo_new(&EPCRContactInfoType,NULL,NULL);
	ret->ptr = new CRContactInfo();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRContactInfo(struct CRContactInfo* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRContactInfo *ret = EPCRContactInfo_new(&EPCRContactInfoType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRContactInfo

//{*********EPCRTouchSensorIf*******
PyObject* EPCRTouchSensorIf_GetContact( EPCRTouchSensorIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCRContactInfo* ret = (EPCRContactInfo*)newEPCRContactInfo(self->ptr->GetContact(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTouchSensorIf_NContacts( EPCRTouchSensorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NContacts());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRTouchSensorIf_methods[] =
{
	{"GetContact",(PyCFunction)EPCRTouchSensorIf_GetContact,METH_O,"EPCRTouchSensorIf::GetContact"},
	{"NContacts",(PyCFunction)EPCRTouchSensorIf_NContacts,METH_NOARGS,"EPCRTouchSensorIf::NContacts"},
	{NULL}
};
void EPCRTouchSensorIf_dealloc(PyObject* self)
{
}
PyObject* EPCRTouchSensorIf_str()
{
	return Py_BuildValue("s","This is EPCRTouchSensorIf.");
}
int EPCRTouchSensorIf_init(EPCRTouchSensorIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTouchSensorIf* EPCRTouchSensorIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTouchSensorIf *self;
	self = (EPCRTouchSensorIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTouchSensorIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTouchSensorIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTouchSensorIf",/*tp_name*/
	sizeof(EPCRTouchSensorIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTouchSensorIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTouchSensorIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTouchSensorIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTouchSensorIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTouchSensorIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTouchSensorIf_new,/*tp_new*/
};
void initEPCRTouchSensorIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTouchSensorIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTouchSensorIfType);
	PyModule_AddObject(m,"CRTouchSensorIf",(PyObject *)&EPCRTouchSensorIfType);//モジュールに追加
}
PyObject* newEPCRTouchSensorIf(struct CRTouchSensorIf org)
{
	EPCRTouchSensorIf *ret = EPCRTouchSensorIf_new(&EPCRTouchSensorIfType,NULL,NULL);
	ret->ptr = new CRTouchSensorIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTouchSensorIf(struct CRTouchSensorIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTouchSensorIf *ret = EPCRTouchSensorIf_new(&EPCRTouchSensorIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTouchSensorIf

//{*********EPCRTouchSensorDesc*******
static PyMethodDef EPCRTouchSensorDesc_methods[] =
{
	{NULL}
};
void EPCRTouchSensorDesc_dealloc(PyObject* self)
{
	delete ((EPCRTouchSensorDesc*)self)->ptr;
}
PyObject* EPCRTouchSensorDesc_str()
{
	return Py_BuildValue("s","This is EPCRTouchSensorDesc.");
}
int EPCRTouchSensorDesc_init(EPCRTouchSensorDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTouchSensorDesc* EPCRTouchSensorDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTouchSensorDesc *self;
	self = (EPCRTouchSensorDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTouchSensorDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTouchSensorDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTouchSensorDesc",/*tp_name*/
	sizeof(EPCRTouchSensorDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTouchSensorDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTouchSensorDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTouchSensorDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTouchSensorDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTouchSensorDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTouchSensorDesc_new,/*tp_new*/
};
void initEPCRTouchSensorDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTouchSensorDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTouchSensorDescType);
	PyModule_AddObject(m,"CRTouchSensorDesc",(PyObject *)&EPCRTouchSensorDescType);//モジュールに追加
}
PyObject* newEPCRTouchSensorDesc(struct CRTouchSensorDesc org)
{
	EPCRTouchSensorDesc *ret = EPCRTouchSensorDesc_new(&EPCRTouchSensorDescType,NULL,NULL);
	ret->ptr = new CRTouchSensorDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTouchSensorDesc(struct CRTouchSensorDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTouchSensorDesc *ret = EPCRTouchSensorDesc_new(&EPCRTouchSensorDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTouchSensorDesc

//{*********EPCRTrajectoryIf*******
PyObject* EPCRTrajectoryIf_Enable( EPCRTrajectoryIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->Enable((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_EnableAngularVelocityControl( EPCRTrajectoryIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnableAngularVelocityControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_EnableOrientationControl( EPCRTrajectoryIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnableOrientationControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_EnablePositionControl( EPCRTrajectoryIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnablePositionControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_EnableVelocityControl( EPCRTrajectoryIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnableVelocityControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_GetAngularVelocity( EPCRTrajectoryIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetAngularVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_GetOrientation( EPCRTrajectoryIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(self->ptr->GetOrientation());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_GetPosition( EPCRTrajectoryIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_GetVelocity( EPCRTrajectoryIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3f* ret = (EPVec3f*)newEPVec3f(self->ptr->GetVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_SetTargetAngularVelocity( EPCRTrajectoryIf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->SetTargetAngularVelocity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_SetTargetOrientation( EPCRTrajectoryIf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->SetTargetOrientation((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_SetTargetPosition( EPCRTrajectoryIf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->SetTargetPosition((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_SetTargetPositionInSolid( EPCRTrajectoryIf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->SetTargetPositionInSolid((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_SetTargetVelocity( EPCRTrajectoryIf* self,EPVec3f* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3f_Check(var1))
	{
		self->ptr->SetTargetVelocity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_SetTimeLimit( EPCRTrajectoryIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTimeLimit(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_Start( EPCRTrajectoryIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Start();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRTrajectoryIf_Step( EPCRTrajectoryIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Step();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRTrajectoryIf_methods[] =
{
	{"Enable",(PyCFunction)EPCRTrajectoryIf_Enable,METH_O,"EPCRTrajectoryIf::Enable"},
	{"EnableAngularVelocityControl",(PyCFunction)EPCRTrajectoryIf_EnableAngularVelocityControl,METH_O,"EPCRTrajectoryIf::EnableAngularVelocityControl"},
	{"EnableOrientationControl",(PyCFunction)EPCRTrajectoryIf_EnableOrientationControl,METH_O,"EPCRTrajectoryIf::EnableOrientationControl"},
	{"EnablePositionControl",(PyCFunction)EPCRTrajectoryIf_EnablePositionControl,METH_O,"EPCRTrajectoryIf::EnablePositionControl"},
	{"EnableVelocityControl",(PyCFunction)EPCRTrajectoryIf_EnableVelocityControl,METH_O,"EPCRTrajectoryIf::EnableVelocityControl"},
	{"GetAngularVelocity",(PyCFunction)EPCRTrajectoryIf_GetAngularVelocity,METH_NOARGS,"EPCRTrajectoryIf::GetAngularVelocity"},
	{"GetOrientation",(PyCFunction)EPCRTrajectoryIf_GetOrientation,METH_NOARGS,"EPCRTrajectoryIf::GetOrientation"},
	{"GetPosition",(PyCFunction)EPCRTrajectoryIf_GetPosition,METH_NOARGS,"EPCRTrajectoryIf::GetPosition"},
	{"GetVelocity",(PyCFunction)EPCRTrajectoryIf_GetVelocity,METH_NOARGS,"EPCRTrajectoryIf::GetVelocity"},
	{"SetTargetAngularVelocity",(PyCFunction)EPCRTrajectoryIf_SetTargetAngularVelocity,METH_O,"EPCRTrajectoryIf::SetTargetAngularVelocity"},
	{"SetTargetOrientation",(PyCFunction)EPCRTrajectoryIf_SetTargetOrientation,METH_O,"EPCRTrajectoryIf::SetTargetOrientation"},
	{"SetTargetPosition",(PyCFunction)EPCRTrajectoryIf_SetTargetPosition,METH_O,"EPCRTrajectoryIf::SetTargetPosition"},
	{"SetTargetPositionInSolid",(PyCFunction)EPCRTrajectoryIf_SetTargetPositionInSolid,METH_O,"EPCRTrajectoryIf::SetTargetPositionInSolid"},
	{"SetTargetVelocity",(PyCFunction)EPCRTrajectoryIf_SetTargetVelocity,METH_O,"EPCRTrajectoryIf::SetTargetVelocity"},
	{"SetTimeLimit",(PyCFunction)EPCRTrajectoryIf_SetTimeLimit,METH_O,"EPCRTrajectoryIf::SetTimeLimit"},
	{"Start",(PyCFunction)EPCRTrajectoryIf_Start,METH_NOARGS,"EPCRTrajectoryIf::Start"},
	{"Step",(PyCFunction)EPCRTrajectoryIf_Step,METH_NOARGS,"EPCRTrajectoryIf::Step"},
	{NULL}
};
void EPCRTrajectoryIf_dealloc(PyObject* self)
{
}
PyObject* EPCRTrajectoryIf_str()
{
	return Py_BuildValue("s","This is EPCRTrajectoryIf.");
}
int EPCRTrajectoryIf_init(EPCRTrajectoryIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTrajectoryIf* EPCRTrajectoryIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTrajectoryIf *self;
	self = (EPCRTrajectoryIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTrajectoryIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTrajectoryIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTrajectoryIf",/*tp_name*/
	sizeof(EPCRTrajectoryIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTrajectoryIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTrajectoryIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTrajectoryIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTrajectoryIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTrajectoryIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTrajectoryIf_new,/*tp_new*/
};
void initEPCRTrajectoryIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTrajectoryIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTrajectoryIfType);
	PyModule_AddObject(m,"CRTrajectoryIf",(PyObject *)&EPCRTrajectoryIfType);//モジュールに追加
}
PyObject* newEPCRTrajectoryIf(struct CRTrajectoryIf org)
{
	EPCRTrajectoryIf *ret = EPCRTrajectoryIf_new(&EPCRTrajectoryIfType,NULL,NULL);
	ret->ptr = new CRTrajectoryIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTrajectoryIf(struct CRTrajectoryIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTrajectoryIf *ret = EPCRTrajectoryIf_new(&EPCRTrajectoryIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTrajectoryIf

//{*********EPCRTrajectoryState*******
static PyMethodDef EPCRTrajectoryState_methods[] =
{
	{NULL}
};
void EPCRTrajectoryState_dealloc(PyObject* self)
{
	delete ((EPCRTrajectoryState*)self)->ptr;
}
PyObject* EPCRTrajectoryState_str()
{
	return Py_BuildValue("s","This is EPCRTrajectoryState.");
}
int EPCRTrajectoryState_init(EPCRTrajectoryState* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTrajectoryState* EPCRTrajectoryState_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTrajectoryState *self;
	self = (EPCRTrajectoryState*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTrajectoryState_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTrajectoryStateType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTrajectoryState",/*tp_name*/
	sizeof(EPCRTrajectoryState),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTrajectoryState_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTrajectoryState_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTrajectoryState",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTrajectoryState_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTrajectoryState_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTrajectoryState_new,/*tp_new*/
};
void initEPCRTrajectoryState(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTrajectoryStateType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTrajectoryStateType);
	PyModule_AddObject(m,"CRTrajectoryState",(PyObject *)&EPCRTrajectoryStateType);//モジュールに追加
}
PyObject* newEPCRTrajectoryState(struct CRTrajectoryState org)
{
	EPCRTrajectoryState *ret = EPCRTrajectoryState_new(&EPCRTrajectoryStateType,NULL,NULL);
	ret->ptr = new CRTrajectoryState();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTrajectoryState(struct CRTrajectoryState* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTrajectoryState *ret = EPCRTrajectoryState_new(&EPCRTrajectoryStateType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTrajectoryState

//{*********EPCRTrajectoryDesc*******
static PyMethodDef EPCRTrajectoryDesc_methods[] =
{
	{NULL}
};
void EPCRTrajectoryDesc_dealloc(PyObject* self)
{
	delete ((EPCRTrajectoryDesc*)self)->ptr;
}
PyObject* EPCRTrajectoryDesc_str()
{
	return Py_BuildValue("s","This is EPCRTrajectoryDesc.");
}
int EPCRTrajectoryDesc_init(EPCRTrajectoryDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTrajectoryDesc* EPCRTrajectoryDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTrajectoryDesc *self;
	self = (EPCRTrajectoryDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTrajectoryDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTrajectoryDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTrajectoryDesc",/*tp_name*/
	sizeof(EPCRTrajectoryDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTrajectoryDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTrajectoryDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTrajectoryDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTrajectoryDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCRTrajectoryStateType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTrajectoryDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTrajectoryDesc_new,/*tp_new*/
};
void initEPCRTrajectoryDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTrajectoryDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTrajectoryDescType);
	PyModule_AddObject(m,"CRTrajectoryDesc",(PyObject *)&EPCRTrajectoryDescType);//モジュールに追加
}
PyObject* newEPCRTrajectoryDesc(struct CRTrajectoryDesc org)
{
	EPCRTrajectoryDesc *ret = EPCRTrajectoryDesc_new(&EPCRTrajectoryDescType,NULL,NULL);
	ret->ptr = new CRTrajectoryDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTrajectoryDesc(struct CRTrajectoryDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTrajectoryDesc *ret = EPCRTrajectoryDesc_new(&EPCRTrajectoryDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTrajectoryDesc

//{*********EPCRTrajectoryControllerIf*******
static PyMethodDef EPCRTrajectoryControllerIf_methods[] =
{
	{NULL}
};
void EPCRTrajectoryControllerIf_dealloc(PyObject* self)
{
}
PyObject* EPCRTrajectoryControllerIf_str()
{
	return Py_BuildValue("s","This is EPCRTrajectoryControllerIf.");
}
int EPCRTrajectoryControllerIf_init(EPCRTrajectoryControllerIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTrajectoryControllerIf* EPCRTrajectoryControllerIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTrajectoryControllerIf *self;
	self = (EPCRTrajectoryControllerIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTrajectoryControllerIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTrajectoryControllerIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTrajectoryControllerIf",/*tp_name*/
	sizeof(EPCRTrajectoryControllerIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTrajectoryControllerIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTrajectoryControllerIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTrajectoryControllerIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTrajectoryControllerIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTrajectoryControllerIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTrajectoryControllerIf_new,/*tp_new*/
};
void initEPCRTrajectoryControllerIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTrajectoryControllerIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTrajectoryControllerIfType);
	PyModule_AddObject(m,"CRTrajectoryControllerIf",(PyObject *)&EPCRTrajectoryControllerIfType);//モジュールに追加
}
PyObject* newEPCRTrajectoryControllerIf(struct CRTrajectoryControllerIf org)
{
	EPCRTrajectoryControllerIf *ret = EPCRTrajectoryControllerIf_new(&EPCRTrajectoryControllerIfType,NULL,NULL);
	ret->ptr = new CRTrajectoryControllerIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTrajectoryControllerIf(struct CRTrajectoryControllerIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTrajectoryControllerIf *ret = EPCRTrajectoryControllerIf_new(&EPCRTrajectoryControllerIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTrajectoryControllerIf

//{*********EPCRTrajectoryControllerDesc*******
static PyMethodDef EPCRTrajectoryControllerDesc_methods[] =
{
	{NULL}
};
void EPCRTrajectoryControllerDesc_dealloc(PyObject* self)
{
	delete ((EPCRTrajectoryControllerDesc*)self)->ptr;
}
PyObject* EPCRTrajectoryControllerDesc_str()
{
	return Py_BuildValue("s","This is EPCRTrajectoryControllerDesc.");
}
int EPCRTrajectoryControllerDesc_init(EPCRTrajectoryControllerDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRTrajectoryControllerDesc* EPCRTrajectoryControllerDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRTrajectoryControllerDesc *self;
	self = (EPCRTrajectoryControllerDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRTrajectoryControllerDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRTrajectoryControllerDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRTrajectoryControllerDesc",/*tp_name*/
	sizeof(EPCRTrajectoryControllerDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRTrajectoryControllerDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRTrajectoryControllerDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRTrajectoryControllerDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRTrajectoryControllerDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRTrajectoryControllerDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRTrajectoryControllerDesc_new,/*tp_new*/
};
void initEPCRTrajectoryControllerDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRTrajectoryControllerDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRTrajectoryControllerDescType);
	PyModule_AddObject(m,"CRTrajectoryControllerDesc",(PyObject *)&EPCRTrajectoryControllerDescType);//モジュールに追加
}
PyObject* newEPCRTrajectoryControllerDesc(struct CRTrajectoryControllerDesc org)
{
	EPCRTrajectoryControllerDesc *ret = EPCRTrajectoryControllerDesc_new(&EPCRTrajectoryControllerDescType,NULL,NULL);
	ret->ptr = new CRTrajectoryControllerDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRTrajectoryControllerDesc(struct CRTrajectoryControllerDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRTrajectoryControllerDesc *ret = EPCRTrajectoryControllerDesc_new(&EPCRTrajectoryControllerDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRTrajectoryControllerDesc

//{*********EPCRVisualSensorIf*******
PyObject* EPCRVisualSensorIf_IsInCenter( EPCRVisualSensorIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsInCenter((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsInCenter((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRVisualSensorIf_IsSelfSolid( EPCRVisualSensorIf* self,EPPHSolidIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHSolidIf_Check(var1))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsSelfSolid((var1->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPCRVisualSensorIf_IsVisible( EPCRVisualSensorIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsVisible((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3f_Check(((EPVec3f*)PyTuple_GetItem(tuple,0))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsVisible((*(((EPVec3f*)PyTuple_GetItem(tuple,0))->ptr)))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPCRVisualSensorIf_methods[] =
{
	{"IsInCenter",(PyCFunction)EPCRVisualSensorIf_IsInCenter,METH_VARARGS,"EPCRVisualSensorIf::IsInCenter"},
	{"IsSelfSolid",(PyCFunction)EPCRVisualSensorIf_IsSelfSolid,METH_O,"EPCRVisualSensorIf::IsSelfSolid"},
	{"IsVisible",(PyCFunction)EPCRVisualSensorIf_IsVisible,METH_VARARGS,"EPCRVisualSensorIf::IsVisible"},
	{NULL}
};
void EPCRVisualSensorIf_dealloc(PyObject* self)
{
}
PyObject* EPCRVisualSensorIf_str()
{
	return Py_BuildValue("s","This is EPCRVisualSensorIf.");
}
int EPCRVisualSensorIf_init(EPCRVisualSensorIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRVisualSensorIf* EPCRVisualSensorIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRVisualSensorIf *self;
	self = (EPCRVisualSensorIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRVisualSensorIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRVisualSensorIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRVisualSensorIf",/*tp_name*/
	sizeof(EPCRVisualSensorIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRVisualSensorIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRVisualSensorIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRVisualSensorIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRVisualSensorIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRVisualSensorIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRVisualSensorIf_new,/*tp_new*/
};
void initEPCRVisualSensorIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRVisualSensorIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRVisualSensorIfType);
	PyModule_AddObject(m,"CRVisualSensorIf",(PyObject *)&EPCRVisualSensorIfType);//モジュールに追加
}
PyObject* newEPCRVisualSensorIf(struct CRVisualSensorIf org)
{
	EPCRVisualSensorIf *ret = EPCRVisualSensorIf_new(&EPCRVisualSensorIfType,NULL,NULL);
	ret->ptr = new CRVisualSensorIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRVisualSensorIf(struct CRVisualSensorIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRVisualSensorIf *ret = EPCRVisualSensorIf_new(&EPCRVisualSensorIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRVisualSensorIf

//{*********EPCRVisualSensorDesc*******
static PyMethodDef EPCRVisualSensorDesc_methods[] =
{
	{NULL}
};
void EPCRVisualSensorDesc_dealloc(PyObject* self)
{
	delete ((EPCRVisualSensorDesc*)self)->ptr;
}
PyObject* EPCRVisualSensorDesc_str()
{
	return Py_BuildValue("s","This is EPCRVisualSensorDesc.");
}
int EPCRVisualSensorDesc_init(EPCRVisualSensorDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPCRVisualSensorDesc* EPCRVisualSensorDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPCRVisualSensorDesc *self;
	self = (EPCRVisualSensorDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPCRVisualSensorDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPCRVisualSensorDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Creature.EPCRVisualSensorDesc",/*tp_name*/
	sizeof(EPCRVisualSensorDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPCRVisualSensorDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPCRVisualSensorDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"CRVisualSensorDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPCRVisualSensorDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPCREngineDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPCRVisualSensorDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPCRVisualSensorDesc_new,/*tp_new*/
};
void initEPCRVisualSensorDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPCRVisualSensorDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Creature");
	Py_INCREF(&EPCRVisualSensorDescType);
	PyModule_AddObject(m,"CRVisualSensorDesc",(PyObject *)&EPCRVisualSensorDescType);//モジュールに追加
}
PyObject* newEPCRVisualSensorDesc(struct CRVisualSensorDesc org)
{
	EPCRVisualSensorDesc *ret = EPCRVisualSensorDesc_new(&EPCRVisualSensorDescType,NULL,NULL);
	ret->ptr = new CRVisualSensorDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPCRVisualSensorDesc(struct CRVisualSensorDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPCRVisualSensorDesc *ret = EPCRVisualSensorDesc_new(&EPCRVisualSensorDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}CRVisualSensorDesc
/**************** for Module ******************/
void initCreature(void)
{
	initEPCRBodyPartIf();
	initEPCRBodyPartDesc();
	initEPCRSolidIf();
	initEPCRSolidDesc();
	initEPCRIKSolidIf();
	initEPCRIKSolidDesc();
	initEPCRJointIf();
	initEPCRJointDesc();
	initEPCRIKJointIf();
	initEPCRIKJointDesc();
	initEPCRBodyIf();
	initEPCRBodyDesc();
	initEPCRCreatureIf();
	initEPCRCreatureDesc();
	initEPCREngineIf();
	initEPCREngineDesc();
	initEPCREyeControllerIf();
	initEPCREyeControllerState();
	initEPCREyeControllerDesc();
	initEPCRNeckControllerIf();
	initEPCRNeckControllerState();
	initEPCRNeckControllerDesc();
	initEPCRGazeControllerIf();
	initEPCRGazeControllerState();
	initEPCRGazeControllerDesc();
	initEPCRAttentionControllerIf();
	initEPCRAttentionControllerDesc();
	initEPCRReachingControllerIf();
	initEPCRReachingControllerDesc();
	initEPCRReachingControllersIf();
	initEPCRReachingControllersDesc();
	initEPCRGrabControllerIf();
	initEPCRGrabControllerDesc();
	initEPCRSceneIf();
	initEPCRSceneDesc();
	initEPCRSdkIf();
	initEPCRSdkDesc();
	initEPCRContactInfo();
	initEPCRTouchSensorIf();
	initEPCRTouchSensorDesc();
	initEPCRTrajectoryIf();
	initEPCRTrajectoryState();
	initEPCRTrajectoryDesc();
	initEPCRTrajectoryControllerIf();
	initEPCRTrajectoryControllerDesc();
	initEPCRVisualSensorIf();
	initEPCRVisualSensorDesc();
}
