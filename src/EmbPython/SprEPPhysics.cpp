#include "../../include\EmbPython\SprEPPhysics.h" 

//{*********EPPHEngineIf*******
static PyMethodDef EPPHEngineIf_methods[] =
{
	{NULL}
};
void EPPHEngineIf_dealloc(PyObject* self)
{
}
PyObject* EPPHEngineIf_str()
{
	return Py_BuildValue("s","This is EPPHEngineIf.");
}
int EPPHEngineIf_init(EPPHEngineIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHEngineIf* EPPHEngineIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHEngineIf *self;
	self = (EPPHEngineIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHEngineIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHEngineIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHEngineIf",/*tp_name*/
	sizeof(EPPHEngineIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHEngineIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHEngineIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHEngineIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHEngineIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHEngineIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHEngineIf_new,/*tp_new*/
};
void initEPPHEngineIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHEngineIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHEngineIfType);
	PyModule_AddObject(m,"PHEngineIf",(PyObject *)&EPPHEngineIfType);//モジュールに追加
}
PyObject* newEPPHEngineIf(struct PHEngineIf org)
{
	EPPHEngineIf *ret = EPPHEngineIf_new(&EPPHEngineIfType,NULL,NULL);
	ret->ptr = new PHEngineIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHEngineIf(struct PHEngineIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHEngineIf *ret = EPPHEngineIf_new(&EPPHEngineIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHEngineIf

//{*********EPPHConstraintEngineIf*******
PyObject* EPPHConstraintEngineIf_GetContactCorrectionRate( EPPHConstraintEngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetContactCorrectionRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_GetContactPoints( EPPHConstraintEngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHConstraintsIf* ret = (EPPHConstraintsIf*)newEPPHConstraintsIf(self->ptr->GetContactPoints());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_GetPosCorrectionRate( EPPHConstraintEngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetPosCorrectionRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_GetShrinkRate( EPPHConstraintEngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetShrinkRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_GetShrinkRateCorrection( EPPHConstraintEngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetShrinkRateCorrection());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_GetVelCorrectionRate( EPPHConstraintEngineIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetVelCorrectionRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetBSaveConstraints( EPPHConstraintEngineIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetBSaveConstraints((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetContactCorrectionRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetContactCorrectionRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetPosCorrectionRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetPosCorrectionRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetShrinkRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetShrinkRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetShrinkRateCorrection( EPPHConstraintEngineIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetShrinkRateCorrection(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetUpdateAllSolidState( EPPHConstraintEngineIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetUpdateAllSolidState((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetUseContactSurface( EPPHConstraintEngineIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetUseContactSurface((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintEngineIf_SetVelCorrectionRate( EPPHConstraintEngineIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetVelCorrectionRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHConstraintEngineIf_methods[] =
{
	{"GetContactCorrectionRate",(PyCFunction)EPPHConstraintEngineIf_GetContactCorrectionRate,METH_NOARGS,"EPPHConstraintEngineIf::GetContactCorrectionRate"},
	{"GetContactPoints",(PyCFunction)EPPHConstraintEngineIf_GetContactPoints,METH_NOARGS,"EPPHConstraintEngineIf::GetContactPoints"},
	{"GetPosCorrectionRate",(PyCFunction)EPPHConstraintEngineIf_GetPosCorrectionRate,METH_NOARGS,"EPPHConstraintEngineIf::GetPosCorrectionRate"},
	{"GetShrinkRate",(PyCFunction)EPPHConstraintEngineIf_GetShrinkRate,METH_NOARGS,"EPPHConstraintEngineIf::GetShrinkRate"},
	{"GetShrinkRateCorrection",(PyCFunction)EPPHConstraintEngineIf_GetShrinkRateCorrection,METH_NOARGS,"EPPHConstraintEngineIf::GetShrinkRateCorrection"},
	{"GetVelCorrectionRate",(PyCFunction)EPPHConstraintEngineIf_GetVelCorrectionRate,METH_NOARGS,"EPPHConstraintEngineIf::GetVelCorrectionRate"},
	{"SetBSaveConstraints",(PyCFunction)EPPHConstraintEngineIf_SetBSaveConstraints,METH_O,"EPPHConstraintEngineIf::SetBSaveConstraints"},
	{"SetContactCorrectionRate",(PyCFunction)EPPHConstraintEngineIf_SetContactCorrectionRate,METH_O,"EPPHConstraintEngineIf::SetContactCorrectionRate"},
	{"SetPosCorrectionRate",(PyCFunction)EPPHConstraintEngineIf_SetPosCorrectionRate,METH_O,"EPPHConstraintEngineIf::SetPosCorrectionRate"},
	{"SetShrinkRate",(PyCFunction)EPPHConstraintEngineIf_SetShrinkRate,METH_O,"EPPHConstraintEngineIf::SetShrinkRate"},
	{"SetShrinkRateCorrection",(PyCFunction)EPPHConstraintEngineIf_SetShrinkRateCorrection,METH_O,"EPPHConstraintEngineIf::SetShrinkRateCorrection"},
	{"SetUpdateAllSolidState",(PyCFunction)EPPHConstraintEngineIf_SetUpdateAllSolidState,METH_O,"EPPHConstraintEngineIf::SetUpdateAllSolidState"},
	{"SetUseContactSurface",(PyCFunction)EPPHConstraintEngineIf_SetUseContactSurface,METH_O,"EPPHConstraintEngineIf::SetUseContactSurface"},
	{"SetVelCorrectionRate",(PyCFunction)EPPHConstraintEngineIf_SetVelCorrectionRate,METH_O,"EPPHConstraintEngineIf::SetVelCorrectionRate"},
	{NULL}
};
void EPPHConstraintEngineIf_dealloc(PyObject* self)
{
}
PyObject* EPPHConstraintEngineIf_str()
{
	return Py_BuildValue("s","This is EPPHConstraintEngineIf.");
}
int EPPHConstraintEngineIf_init(EPPHConstraintEngineIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHConstraintEngineIf* EPPHConstraintEngineIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHConstraintEngineIf *self;
	self = (EPPHConstraintEngineIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHConstraintEngineIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHConstraintEngineIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHConstraintEngineIf",/*tp_name*/
	sizeof(EPPHConstraintEngineIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHConstraintEngineIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHConstraintEngineIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHConstraintEngineIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHConstraintEngineIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHEngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHConstraintEngineIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHConstraintEngineIf_new,/*tp_new*/
};
void initEPPHConstraintEngineIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHConstraintEngineIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHConstraintEngineIfType);
	PyModule_AddObject(m,"PHConstraintEngineIf",(PyObject *)&EPPHConstraintEngineIfType);//モジュールに追加
}
PyObject* newEPPHConstraintEngineIf(struct PHConstraintEngineIf org)
{
	EPPHConstraintEngineIf *ret = EPPHConstraintEngineIf_new(&EPPHConstraintEngineIfType,NULL,NULL);
	ret->ptr = new PHConstraintEngineIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHConstraintEngineIf(struct PHConstraintEngineIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHConstraintEngineIf *ret = EPPHConstraintEngineIf_new(&EPPHConstraintEngineIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHConstraintEngineIf

//{*********EPPHGravityEngineIf*******
static PyMethodDef EPPHGravityEngineIf_methods[] =
{
	{NULL}
};
void EPPHGravityEngineIf_dealloc(PyObject* self)
{
}
PyObject* EPPHGravityEngineIf_str()
{
	return Py_BuildValue("s","This is EPPHGravityEngineIf.");
}
int EPPHGravityEngineIf_init(EPPHGravityEngineIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHGravityEngineIf* EPPHGravityEngineIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHGravityEngineIf *self;
	self = (EPPHGravityEngineIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHGravityEngineIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHGravityEngineIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHGravityEngineIf",/*tp_name*/
	sizeof(EPPHGravityEngineIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHGravityEngineIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHGravityEngineIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHGravityEngineIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHGravityEngineIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHEngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHGravityEngineIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHGravityEngineIf_new,/*tp_new*/
};
void initEPPHGravityEngineIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHGravityEngineIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHGravityEngineIfType);
	PyModule_AddObject(m,"PHGravityEngineIf",(PyObject *)&EPPHGravityEngineIfType);//モジュールに追加
}
PyObject* newEPPHGravityEngineIf(struct PHGravityEngineIf org)
{
	EPPHGravityEngineIf *ret = EPPHGravityEngineIf_new(&EPPHGravityEngineIfType,NULL,NULL);
	ret->ptr = new PHGravityEngineIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHGravityEngineIf(struct PHGravityEngineIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHGravityEngineIf *ret = EPPHGravityEngineIf_new(&EPPHGravityEngineIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHGravityEngineIf

//{*********EPPHPenaltyEngineIf*******
static PyMethodDef EPPHPenaltyEngineIf_methods[] =
{
	{NULL}
};
void EPPHPenaltyEngineIf_dealloc(PyObject* self)
{
}
PyObject* EPPHPenaltyEngineIf_str()
{
	return Py_BuildValue("s","This is EPPHPenaltyEngineIf.");
}
int EPPHPenaltyEngineIf_init(EPPHPenaltyEngineIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPenaltyEngineIf* EPPHPenaltyEngineIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPenaltyEngineIf *self;
	self = (EPPHPenaltyEngineIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPenaltyEngineIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPenaltyEngineIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPenaltyEngineIf",/*tp_name*/
	sizeof(EPPHPenaltyEngineIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPenaltyEngineIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPenaltyEngineIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPenaltyEngineIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPenaltyEngineIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHEngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPenaltyEngineIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPenaltyEngineIf_new,/*tp_new*/
};
void initEPPHPenaltyEngineIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPenaltyEngineIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPenaltyEngineIfType);
	PyModule_AddObject(m,"PHPenaltyEngineIf",(PyObject *)&EPPHPenaltyEngineIfType);//モジュールに追加
}
PyObject* newEPPHPenaltyEngineIf(struct PHPenaltyEngineIf org)
{
	EPPHPenaltyEngineIf *ret = EPPHPenaltyEngineIf_new(&EPPHPenaltyEngineIfType,NULL,NULL);
	ret->ptr = new PHPenaltyEngineIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPenaltyEngineIf(struct PHPenaltyEngineIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPenaltyEngineIf *ret = EPPHPenaltyEngineIf_new(&EPPHPenaltyEngineIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPenaltyEngineIf

//{*********EPPHIKEngineIf*******
PyObject* EPPHIKEngineIf_Enable( EPPHIKEngineIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->Enable((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEngineIf_IsEnabled( EPPHIKEngineIf* self )
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
PyObject* EPPHIKEngineIf_SetNumIter( EPPHIKEngineIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->SetNumIter(PyLong_AsLong((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHIKEngineIf_methods[] =
{
	{"Enable",(PyCFunction)EPPHIKEngineIf_Enable,METH_O,"EPPHIKEngineIf::Enable"},
	{"IsEnabled",(PyCFunction)EPPHIKEngineIf_IsEnabled,METH_NOARGS,"EPPHIKEngineIf::IsEnabled"},
	{"SetNumIter",(PyCFunction)EPPHIKEngineIf_SetNumIter,METH_O,"EPPHIKEngineIf::SetNumIter"},
	{NULL}
};
void EPPHIKEngineIf_dealloc(PyObject* self)
{
}
PyObject* EPPHIKEngineIf_str()
{
	return Py_BuildValue("s","This is EPPHIKEngineIf.");
}
int EPPHIKEngineIf_init(EPPHIKEngineIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKEngineIf* EPPHIKEngineIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKEngineIf *self;
	self = (EPPHIKEngineIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKEngineIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKEngineIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKEngineIf",/*tp_name*/
	sizeof(EPPHIKEngineIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKEngineIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKEngineIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKEngineIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKEngineIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHEngineIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKEngineIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKEngineIf_new,/*tp_new*/
};
void initEPPHIKEngineIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKEngineIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKEngineIfType);
	PyModule_AddObject(m,"PHIKEngineIf",(PyObject *)&EPPHIKEngineIfType);//モジュールに追加
}
PyObject* newEPPHIKEngineIf(struct PHIKEngineIf org)
{
	EPPHIKEngineIf *ret = EPPHIKEngineIf_new(&EPPHIKEngineIfType,NULL,NULL);
	ret->ptr = new PHIKEngineIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKEngineIf(struct PHIKEngineIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKEngineIf *ret = EPPHIKEngineIf_new(&EPPHIKEngineIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKEngineIf

//{*********EPPHIKEndEffectorIf*******
PyObject* EPPHIKEndEffectorIf_Enable( EPPHIKEndEffectorIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->Enable((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_EnableForceControl( EPPHIKEndEffectorIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnableForceControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_EnableOrientationControl( EPPHIKEndEffectorIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnableOrientationControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_EnablePositionControl( EPPHIKEndEffectorIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnablePositionControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_EnableTorqueControl( EPPHIKEndEffectorIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->EnableTorqueControl((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetSolid( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->GetSolid());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetTargetForce( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTargetForce());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetTargetForceWorkingPoint( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTargetForceWorkingPoint());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetTargetLocalPosition( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTargetLocalPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetTargetOrientation( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->GetTargetOrientation());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetTargetPosition( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTargetPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_GetTargetTorque( EPPHIKEndEffectorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTargetTorque());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_IsEnabled( EPPHIKEndEffectorIf* self )
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
PyObject* EPPHIKEndEffectorIf_SetOrientationPriority( EPPHIKEndEffectorIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetOrientationPriority(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_SetPositionPriority( EPPHIKEndEffectorIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetPositionPriority(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_SetSolid( EPPHIKEndEffectorIf* self,EPPHSolidIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHSolidIf_Check(var1))
	{
		self->ptr->SetSolid((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_SetTargetForce( EPPHIKEndEffectorIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetTargetForce((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->SetTargetForce((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_SetTargetOrientation( EPPHIKEndEffectorIf* self,EPQuaterniond* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPQuaterniond_Check(var1))
	{
		self->ptr->SetTargetOrientation((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_SetTargetPosition( EPPHIKEndEffectorIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetTargetPosition((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->SetTargetPosition((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKEndEffectorIf_SetTargetTorque( EPPHIKEndEffectorIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetTargetTorque((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHIKEndEffectorIf_methods[] =
{
	{"Enable",(PyCFunction)EPPHIKEndEffectorIf_Enable,METH_O,"EPPHIKEndEffectorIf::Enable"},
	{"EnableForceControl",(PyCFunction)EPPHIKEndEffectorIf_EnableForceControl,METH_O,"EPPHIKEndEffectorIf::EnableForceControl"},
	{"EnableOrientationControl",(PyCFunction)EPPHIKEndEffectorIf_EnableOrientationControl,METH_O,"EPPHIKEndEffectorIf::EnableOrientationControl"},
	{"EnablePositionControl",(PyCFunction)EPPHIKEndEffectorIf_EnablePositionControl,METH_O,"EPPHIKEndEffectorIf::EnablePositionControl"},
	{"EnableTorqueControl",(PyCFunction)EPPHIKEndEffectorIf_EnableTorqueControl,METH_O,"EPPHIKEndEffectorIf::EnableTorqueControl"},
	{"GetSolid",(PyCFunction)EPPHIKEndEffectorIf_GetSolid,METH_NOARGS,"EPPHIKEndEffectorIf::GetSolid"},
	{"GetTargetForce",(PyCFunction)EPPHIKEndEffectorIf_GetTargetForce,METH_NOARGS,"EPPHIKEndEffectorIf::GetTargetForce"},
	{"GetTargetForceWorkingPoint",(PyCFunction)EPPHIKEndEffectorIf_GetTargetForceWorkingPoint,METH_NOARGS,"EPPHIKEndEffectorIf::GetTargetForceWorkingPoint"},
	{"GetTargetLocalPosition",(PyCFunction)EPPHIKEndEffectorIf_GetTargetLocalPosition,METH_NOARGS,"EPPHIKEndEffectorIf::GetTargetLocalPosition"},
	{"GetTargetOrientation",(PyCFunction)EPPHIKEndEffectorIf_GetTargetOrientation,METH_NOARGS,"EPPHIKEndEffectorIf::GetTargetOrientation"},
	{"GetTargetPosition",(PyCFunction)EPPHIKEndEffectorIf_GetTargetPosition,METH_NOARGS,"EPPHIKEndEffectorIf::GetTargetPosition"},
	{"GetTargetTorque",(PyCFunction)EPPHIKEndEffectorIf_GetTargetTorque,METH_NOARGS,"EPPHIKEndEffectorIf::GetTargetTorque"},
	{"IsEnabled",(PyCFunction)EPPHIKEndEffectorIf_IsEnabled,METH_NOARGS,"EPPHIKEndEffectorIf::IsEnabled"},
	{"SetOrientationPriority",(PyCFunction)EPPHIKEndEffectorIf_SetOrientationPriority,METH_O,"EPPHIKEndEffectorIf::SetOrientationPriority"},
	{"SetPositionPriority",(PyCFunction)EPPHIKEndEffectorIf_SetPositionPriority,METH_O,"EPPHIKEndEffectorIf::SetPositionPriority"},
	{"SetSolid",(PyCFunction)EPPHIKEndEffectorIf_SetSolid,METH_O,"EPPHIKEndEffectorIf::SetSolid"},
	{"SetTargetForce",(PyCFunction)EPPHIKEndEffectorIf_SetTargetForce,METH_VARARGS,"EPPHIKEndEffectorIf::SetTargetForce"},
	{"SetTargetOrientation",(PyCFunction)EPPHIKEndEffectorIf_SetTargetOrientation,METH_O,"EPPHIKEndEffectorIf::SetTargetOrientation"},
	{"SetTargetPosition",(PyCFunction)EPPHIKEndEffectorIf_SetTargetPosition,METH_VARARGS,"EPPHIKEndEffectorIf::SetTargetPosition"},
	{"SetTargetTorque",(PyCFunction)EPPHIKEndEffectorIf_SetTargetTorque,METH_O,"EPPHIKEndEffectorIf::SetTargetTorque"},
	{NULL}
};
void EPPHIKEndEffectorIf_dealloc(PyObject* self)
{
}
PyObject* EPPHIKEndEffectorIf_str()
{
	return Py_BuildValue("s","This is EPPHIKEndEffectorIf.");
}
int EPPHIKEndEffectorIf_init(EPPHIKEndEffectorIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKEndEffectorIf* EPPHIKEndEffectorIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKEndEffectorIf *self;
	self = (EPPHIKEndEffectorIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKEndEffectorIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKEndEffectorIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKEndEffectorIf",/*tp_name*/
	sizeof(EPPHIKEndEffectorIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKEndEffectorIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKEndEffectorIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKEndEffectorIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKEndEffectorIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKEndEffectorIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKEndEffectorIf_new,/*tp_new*/
};
void initEPPHIKEndEffectorIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKEndEffectorIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKEndEffectorIfType);
	PyModule_AddObject(m,"PHIKEndEffectorIf",(PyObject *)&EPPHIKEndEffectorIfType);//モジュールに追加
}
PyObject* newEPPHIKEndEffectorIf(struct PHIKEndEffectorIf org)
{
	EPPHIKEndEffectorIf *ret = EPPHIKEndEffectorIf_new(&EPPHIKEndEffectorIfType,NULL,NULL);
	ret->ptr = new PHIKEndEffectorIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKEndEffectorIf(struct PHIKEndEffectorIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKEndEffectorIf *ret = EPPHIKEndEffectorIf_new(&EPPHIKEndEffectorIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKEndEffectorIf

//{*********EPPHIKEndEffectorDesc*******
static PyMethodDef EPPHIKEndEffectorDesc_methods[] =
{
	{NULL}
};
void EPPHIKEndEffectorDesc_dealloc(PyObject* self)
{
	delete ((EPPHIKEndEffectorDesc*)self)->ptr;
}
PyObject* EPPHIKEndEffectorDesc_str()
{
	return Py_BuildValue("s","This is EPPHIKEndEffectorDesc.");
}
int EPPHIKEndEffectorDesc_init(EPPHIKEndEffectorDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKEndEffectorDesc* EPPHIKEndEffectorDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKEndEffectorDesc *self;
	self = (EPPHIKEndEffectorDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKEndEffectorDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKEndEffectorDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKEndEffectorDesc",/*tp_name*/
	sizeof(EPPHIKEndEffectorDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKEndEffectorDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKEndEffectorDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKEndEffectorDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKEndEffectorDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKEndEffectorDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKEndEffectorDesc_new,/*tp_new*/
};
void initEPPHIKEndEffectorDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKEndEffectorDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKEndEffectorDescType);
	PyModule_AddObject(m,"PHIKEndEffectorDesc",(PyObject *)&EPPHIKEndEffectorDescType);//モジュールに追加
}
PyObject* newEPPHIKEndEffectorDesc(struct PHIKEndEffectorDesc org)
{
	EPPHIKEndEffectorDesc *ret = EPPHIKEndEffectorDesc_new(&EPPHIKEndEffectorDescType,NULL,NULL);
	ret->ptr = new PHIKEndEffectorDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKEndEffectorDesc(struct PHIKEndEffectorDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKEndEffectorDesc *ret = EPPHIKEndEffectorDesc_new(&EPPHIKEndEffectorDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKEndEffectorDesc

//{*********EPPHIKActuatorIf*******
PyObject* EPPHIKActuatorIf_Enable( EPPHIKActuatorIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->Enable((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_GetBias( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("f",self->ptr->GetBias());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_GetDamper( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_GetSpring( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSpring());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_IsEnabled( EPPHIKActuatorIf* self )
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
PyObject* EPPHIKActuatorIf_Move( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Move();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_MoveToNaturalPosition( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->MoveToNaturalPosition();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_PrepareSolve( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->PrepareSolve();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_ProceedSolve( EPPHIKActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ProceedSolve();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_RegisterEndEffector( EPPHIKActuatorIf* self,EPPHIKEndEffectorIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHIKEndEffectorIf_Check(var1))
	{
		self->ptr->RegisterEndEffector((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_SetBias( EPPHIKActuatorIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetBias(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_SetDamper( EPPHIKActuatorIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetDamper(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKActuatorIf_SetSpring( EPPHIKActuatorIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetSpring(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHIKActuatorIf_methods[] =
{
	{"Enable",(PyCFunction)EPPHIKActuatorIf_Enable,METH_O,"EPPHIKActuatorIf::Enable"},
	{"GetBias",(PyCFunction)EPPHIKActuatorIf_GetBias,METH_NOARGS,"EPPHIKActuatorIf::GetBias"},
	{"GetDamper",(PyCFunction)EPPHIKActuatorIf_GetDamper,METH_NOARGS,"EPPHIKActuatorIf::GetDamper"},
	{"GetSpring",(PyCFunction)EPPHIKActuatorIf_GetSpring,METH_NOARGS,"EPPHIKActuatorIf::GetSpring"},
	{"IsEnabled",(PyCFunction)EPPHIKActuatorIf_IsEnabled,METH_NOARGS,"EPPHIKActuatorIf::IsEnabled"},
	{"Move",(PyCFunction)EPPHIKActuatorIf_Move,METH_NOARGS,"EPPHIKActuatorIf::Move"},
	{"MoveToNaturalPosition",(PyCFunction)EPPHIKActuatorIf_MoveToNaturalPosition,METH_NOARGS,"EPPHIKActuatorIf::MoveToNaturalPosition"},
	{"PrepareSolve",(PyCFunction)EPPHIKActuatorIf_PrepareSolve,METH_NOARGS,"EPPHIKActuatorIf::PrepareSolve"},
	{"ProceedSolve",(PyCFunction)EPPHIKActuatorIf_ProceedSolve,METH_NOARGS,"EPPHIKActuatorIf::ProceedSolve"},
	{"RegisterEndEffector",(PyCFunction)EPPHIKActuatorIf_RegisterEndEffector,METH_O,"EPPHIKActuatorIf::RegisterEndEffector"},
	{"SetBias",(PyCFunction)EPPHIKActuatorIf_SetBias,METH_O,"EPPHIKActuatorIf::SetBias"},
	{"SetDamper",(PyCFunction)EPPHIKActuatorIf_SetDamper,METH_O,"EPPHIKActuatorIf::SetDamper"},
	{"SetSpring",(PyCFunction)EPPHIKActuatorIf_SetSpring,METH_O,"EPPHIKActuatorIf::SetSpring"},
	{NULL}
};
void EPPHIKActuatorIf_dealloc(PyObject* self)
{
}
PyObject* EPPHIKActuatorIf_str()
{
	return Py_BuildValue("s","This is EPPHIKActuatorIf.");
}
int EPPHIKActuatorIf_init(EPPHIKActuatorIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKActuatorIf* EPPHIKActuatorIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKActuatorIf *self;
	self = (EPPHIKActuatorIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKActuatorIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKActuatorIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKActuatorIf",/*tp_name*/
	sizeof(EPPHIKActuatorIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKActuatorIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKActuatorIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKActuatorIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKActuatorIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKActuatorIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKActuatorIf_new,/*tp_new*/
};
void initEPPHIKActuatorIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKActuatorIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKActuatorIfType);
	PyModule_AddObject(m,"PHIKActuatorIf",(PyObject *)&EPPHIKActuatorIfType);//モジュールに追加
}
PyObject* newEPPHIKActuatorIf(struct PHIKActuatorIf org)
{
	EPPHIKActuatorIf *ret = EPPHIKActuatorIf_new(&EPPHIKActuatorIfType,NULL,NULL);
	ret->ptr = new PHIKActuatorIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKActuatorIf(struct PHIKActuatorIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKActuatorIf *ret = EPPHIKActuatorIf_new(&EPPHIKActuatorIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKActuatorIf

//{*********EPPHIKActuatorDesc*******
static PyMethodDef EPPHIKActuatorDesc_methods[] =
{
	{NULL}
};
void EPPHIKActuatorDesc_dealloc(PyObject* self)
{
	delete ((EPPHIKActuatorDesc*)self)->ptr;
}
PyObject* EPPHIKActuatorDesc_str()
{
	return Py_BuildValue("s","This is EPPHIKActuatorDesc.");
}
int EPPHIKActuatorDesc_init(EPPHIKActuatorDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKActuatorDesc* EPPHIKActuatorDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKActuatorDesc *self;
	self = (EPPHIKActuatorDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKActuatorDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKActuatorDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKActuatorDesc",/*tp_name*/
	sizeof(EPPHIKActuatorDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKActuatorDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKActuatorDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKActuatorDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKActuatorDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKActuatorDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKActuatorDesc_new,/*tp_new*/
};
void initEPPHIKActuatorDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKActuatorDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKActuatorDescType);
	PyModule_AddObject(m,"PHIKActuatorDesc",(PyObject *)&EPPHIKActuatorDescType);//モジュールに追加
}
PyObject* newEPPHIKActuatorDesc(struct PHIKActuatorDesc org)
{
	EPPHIKActuatorDesc *ret = EPPHIKActuatorDesc_new(&EPPHIKActuatorDescType,NULL,NULL);
	ret->ptr = new PHIKActuatorDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKActuatorDesc(struct PHIKActuatorDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKActuatorDesc *ret = EPPHIKActuatorDesc_new(&EPPHIKActuatorDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKActuatorDesc

//{*********EPPHIKBallActuatorIf*******
PyObject* EPPHIKBallActuatorIf_GetJoint( EPPHIKBallActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHBallJointIf* ret = (EPPHBallJointIf*)newEPPHBallJointIf(self->ptr->GetJoint());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKBallActuatorIf_SetJoint( EPPHIKBallActuatorIf* self,EPPHBallJointIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHBallJointIf_Check(var1))
	{
		self->ptr->SetJoint((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHIKBallActuatorIf_methods[] =
{
	{"GetJoint",(PyCFunction)EPPHIKBallActuatorIf_GetJoint,METH_NOARGS,"EPPHIKBallActuatorIf::GetJoint"},
	{"SetJoint",(PyCFunction)EPPHIKBallActuatorIf_SetJoint,METH_O,"EPPHIKBallActuatorIf::SetJoint"},
	{NULL}
};
void EPPHIKBallActuatorIf_dealloc(PyObject* self)
{
}
PyObject* EPPHIKBallActuatorIf_str()
{
	return Py_BuildValue("s","This is EPPHIKBallActuatorIf.");
}
int EPPHIKBallActuatorIf_init(EPPHIKBallActuatorIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKBallActuatorIf* EPPHIKBallActuatorIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKBallActuatorIf *self;
	self = (EPPHIKBallActuatorIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKBallActuatorIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKBallActuatorIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKBallActuatorIf",/*tp_name*/
	sizeof(EPPHIKBallActuatorIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKBallActuatorIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKBallActuatorIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKBallActuatorIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKBallActuatorIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHIKActuatorIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKBallActuatorIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKBallActuatorIf_new,/*tp_new*/
};
void initEPPHIKBallActuatorIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKBallActuatorIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKBallActuatorIfType);
	PyModule_AddObject(m,"PHIKBallActuatorIf",(PyObject *)&EPPHIKBallActuatorIfType);//モジュールに追加
}
PyObject* newEPPHIKBallActuatorIf(struct PHIKBallActuatorIf org)
{
	EPPHIKBallActuatorIf *ret = EPPHIKBallActuatorIf_new(&EPPHIKBallActuatorIfType,NULL,NULL);
	ret->ptr = new PHIKBallActuatorIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKBallActuatorIf(struct PHIKBallActuatorIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKBallActuatorIf *ret = EPPHIKBallActuatorIf_new(&EPPHIKBallActuatorIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKBallActuatorIf

//{*********EPPHIKBallActuatorDesc*******
static PyMethodDef EPPHIKBallActuatorDesc_methods[] =
{
	{NULL}
};
void EPPHIKBallActuatorDesc_dealloc(PyObject* self)
{
	delete ((EPPHIKBallActuatorDesc*)self)->ptr;
}
PyObject* EPPHIKBallActuatorDesc_str()
{
	return Py_BuildValue("s","This is EPPHIKBallActuatorDesc.");
}
int EPPHIKBallActuatorDesc_init(EPPHIKBallActuatorDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKBallActuatorDesc* EPPHIKBallActuatorDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKBallActuatorDesc *self;
	self = (EPPHIKBallActuatorDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKBallActuatorDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKBallActuatorDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKBallActuatorDesc",/*tp_name*/
	sizeof(EPPHIKBallActuatorDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKBallActuatorDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKBallActuatorDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKBallActuatorDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKBallActuatorDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHIKActuatorDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKBallActuatorDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKBallActuatorDesc_new,/*tp_new*/
};
void initEPPHIKBallActuatorDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKBallActuatorDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKBallActuatorDescType);
	PyModule_AddObject(m,"PHIKBallActuatorDesc",(PyObject *)&EPPHIKBallActuatorDescType);//モジュールに追加
}
PyObject* newEPPHIKBallActuatorDesc(struct PHIKBallActuatorDesc org)
{
	EPPHIKBallActuatorDesc *ret = EPPHIKBallActuatorDesc_new(&EPPHIKBallActuatorDescType,NULL,NULL);
	ret->ptr = new PHIKBallActuatorDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKBallActuatorDesc(struct PHIKBallActuatorDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKBallActuatorDesc *ret = EPPHIKBallActuatorDesc_new(&EPPHIKBallActuatorDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKBallActuatorDesc

//{*********EPPHIKHingeActuatorIf*******
PyObject* EPPHIKHingeActuatorIf_GetJoint( EPPHIKHingeActuatorIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHHingeJointIf* ret = (EPPHHingeJointIf*)newEPPHHingeJointIf(self->ptr->GetJoint());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHIKHingeActuatorIf_SetJoint( EPPHIKHingeActuatorIf* self,EPPHHingeJointIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPHHingeJointIf_Check(var1))
	{
		self->ptr->SetJoint((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHIKHingeActuatorIf_methods[] =
{
	{"GetJoint",(PyCFunction)EPPHIKHingeActuatorIf_GetJoint,METH_NOARGS,"EPPHIKHingeActuatorIf::GetJoint"},
	{"SetJoint",(PyCFunction)EPPHIKHingeActuatorIf_SetJoint,METH_O,"EPPHIKHingeActuatorIf::SetJoint"},
	{NULL}
};
void EPPHIKHingeActuatorIf_dealloc(PyObject* self)
{
}
PyObject* EPPHIKHingeActuatorIf_str()
{
	return Py_BuildValue("s","This is EPPHIKHingeActuatorIf.");
}
int EPPHIKHingeActuatorIf_init(EPPHIKHingeActuatorIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKHingeActuatorIf* EPPHIKHingeActuatorIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKHingeActuatorIf *self;
	self = (EPPHIKHingeActuatorIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKHingeActuatorIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKHingeActuatorIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKHingeActuatorIf",/*tp_name*/
	sizeof(EPPHIKHingeActuatorIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKHingeActuatorIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKHingeActuatorIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKHingeActuatorIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKHingeActuatorIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHIKActuatorIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKHingeActuatorIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKHingeActuatorIf_new,/*tp_new*/
};
void initEPPHIKHingeActuatorIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKHingeActuatorIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKHingeActuatorIfType);
	PyModule_AddObject(m,"PHIKHingeActuatorIf",(PyObject *)&EPPHIKHingeActuatorIfType);//モジュールに追加
}
PyObject* newEPPHIKHingeActuatorIf(struct PHIKHingeActuatorIf org)
{
	EPPHIKHingeActuatorIf *ret = EPPHIKHingeActuatorIf_new(&EPPHIKHingeActuatorIfType,NULL,NULL);
	ret->ptr = new PHIKHingeActuatorIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKHingeActuatorIf(struct PHIKHingeActuatorIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKHingeActuatorIf *ret = EPPHIKHingeActuatorIf_new(&EPPHIKHingeActuatorIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKHingeActuatorIf

//{*********EPPHIKHingeActuatorDesc*******
static PyMethodDef EPPHIKHingeActuatorDesc_methods[] =
{
	{NULL}
};
void EPPHIKHingeActuatorDesc_dealloc(PyObject* self)
{
	delete ((EPPHIKHingeActuatorDesc*)self)->ptr;
}
PyObject* EPPHIKHingeActuatorDesc_str()
{
	return Py_BuildValue("s","This is EPPHIKHingeActuatorDesc.");
}
int EPPHIKHingeActuatorDesc_init(EPPHIKHingeActuatorDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHIKHingeActuatorDesc* EPPHIKHingeActuatorDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHIKHingeActuatorDesc *self;
	self = (EPPHIKHingeActuatorDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHIKHingeActuatorDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHIKHingeActuatorDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHIKHingeActuatorDesc",/*tp_name*/
	sizeof(EPPHIKHingeActuatorDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHIKHingeActuatorDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHIKHingeActuatorDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHIKHingeActuatorDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHIKHingeActuatorDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHIKActuatorDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHIKHingeActuatorDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHIKHingeActuatorDesc_new,/*tp_new*/
};
void initEPPHIKHingeActuatorDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHIKHingeActuatorDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHIKHingeActuatorDescType);
	PyModule_AddObject(m,"PHIKHingeActuatorDesc",(PyObject *)&EPPHIKHingeActuatorDescType);//モジュールに追加
}
PyObject* newEPPHIKHingeActuatorDesc(struct PHIKHingeActuatorDesc org)
{
	EPPHIKHingeActuatorDesc *ret = EPPHIKHingeActuatorDesc_new(&EPPHIKHingeActuatorDescType,NULL,NULL);
	ret->ptr = new PHIKHingeActuatorDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHIKHingeActuatorDesc(struct PHIKHingeActuatorDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHIKHingeActuatorDesc *ret = EPPHIKHingeActuatorDesc_new(&EPPHIKHingeActuatorDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHIKHingeActuatorDesc

//{*********EPPHConstraintDesc*******
static PyMethodDef EPPHConstraintDesc_methods[] =
{
	{NULL}
};
void EPPHConstraintDesc_dealloc(PyObject* self)
{
	delete ((EPPHConstraintDesc*)self)->ptr;
}
PyObject* EPPHConstraintDesc_str()
{
	return Py_BuildValue("s","This is EPPHConstraintDesc.");
}
int EPPHConstraintDesc_init(EPPHConstraintDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHConstraintDesc* EPPHConstraintDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHConstraintDesc *self;
	self = (EPPHConstraintDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHConstraintDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHConstraintDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHConstraintDesc",/*tp_name*/
	sizeof(EPPHConstraintDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHConstraintDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHConstraintDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHConstraintDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHConstraintDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHConstraintDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHConstraintDesc_new,/*tp_new*/
};
void initEPPHConstraintDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHConstraintDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHConstraintDescType);
	PyModule_AddObject(m,"PHConstraintDesc",(PyObject *)&EPPHConstraintDescType);//モジュールに追加
}
PyObject* newEPPHConstraintDesc(struct PHConstraintDesc org)
{
	EPPHConstraintDesc *ret = EPPHConstraintDesc_new(&EPPHConstraintDescType,NULL,NULL);
	ret->ptr = new PHConstraintDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHConstraintDesc(struct PHConstraintDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHConstraintDesc *ret = EPPHConstraintDesc_new(&EPPHConstraintDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHConstraintDesc

//{*********EPPHJointDesc*******
static PyMethodDef EPPHJointDesc_methods[] =
{
	{NULL}
};
void EPPHJointDesc_dealloc(PyObject* self)
{
	delete ((EPPHJointDesc*)self)->ptr;
}
PyObject* EPPHJointDesc_str()
{
	return Py_BuildValue("s","This is EPPHJointDesc.");
}
int EPPHJointDesc_init(EPPHJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHJointDesc* EPPHJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHJointDesc *self;
	self = (EPPHJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHJointDesc",/*tp_name*/
	sizeof(EPPHJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHConstraintDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHJointDesc_new,/*tp_new*/
};
void initEPPHJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHJointDescType);
	PyModule_AddObject(m,"PHJointDesc",(PyObject *)&EPPHJointDescType);//モジュールに追加
}
PyObject* newEPPHJointDesc(struct PHJointDesc org)
{
	EPPHJointDesc *ret = EPPHJointDesc_new(&EPPHJointDescType,NULL,NULL);
	ret->ptr = new PHJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHJointDesc(struct PHJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHJointDesc *ret = EPPHJointDesc_new(&EPPHJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHJointDesc

//{*********EPPHJoint1DDesc*******
static PyMethodDef EPPHJoint1DDesc_methods[] =
{
	{NULL}
};
void EPPHJoint1DDesc_dealloc(PyObject* self)
{
	delete ((EPPHJoint1DDesc*)self)->ptr;
}
PyObject* EPPHJoint1DDesc_str()
{
	return Py_BuildValue("s","This is EPPHJoint1DDesc.");
}
int EPPHJoint1DDesc_init(EPPHJoint1DDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHJoint1DDesc* EPPHJoint1DDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHJoint1DDesc *self;
	self = (EPPHJoint1DDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHJoint1DDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHJoint1DDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHJoint1DDesc",/*tp_name*/
	sizeof(EPPHJoint1DDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHJoint1DDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHJoint1DDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHJoint1DDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHJoint1DDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJointDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHJoint1DDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHJoint1DDesc_new,/*tp_new*/
};
void initEPPHJoint1DDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHJoint1DDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHJoint1DDescType);
	PyModule_AddObject(m,"PHJoint1DDesc",(PyObject *)&EPPHJoint1DDescType);//モジュールに追加
}
PyObject* newEPPHJoint1DDesc(struct PHJoint1DDesc org)
{
	EPPHJoint1DDesc *ret = EPPHJoint1DDesc_new(&EPPHJoint1DDescType,NULL,NULL);
	ret->ptr = new PHJoint1DDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHJoint1DDesc(struct PHJoint1DDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHJoint1DDesc *ret = EPPHJoint1DDesc_new(&EPPHJoint1DDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHJoint1DDesc

//{*********EPPHTreeNodeDesc*******
static PyMethodDef EPPHTreeNodeDesc_methods[] =
{
	{NULL}
};
void EPPHTreeNodeDesc_dealloc(PyObject* self)
{
	delete ((EPPHTreeNodeDesc*)self)->ptr;
}
PyObject* EPPHTreeNodeDesc_str()
{
	return Py_BuildValue("s","This is EPPHTreeNodeDesc.");
}
int EPPHTreeNodeDesc_init(EPPHTreeNodeDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHTreeNodeDesc* EPPHTreeNodeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHTreeNodeDesc *self;
	self = (EPPHTreeNodeDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHTreeNodeDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHTreeNodeDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHTreeNodeDesc",/*tp_name*/
	sizeof(EPPHTreeNodeDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHTreeNodeDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHTreeNodeDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHTreeNodeDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHTreeNodeDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHTreeNodeDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHTreeNodeDesc_new,/*tp_new*/
};
void initEPPHTreeNodeDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHTreeNodeDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHTreeNodeDescType);
	PyModule_AddObject(m,"PHTreeNodeDesc",(PyObject *)&EPPHTreeNodeDescType);//モジュールに追加
}
PyObject* newEPPHTreeNodeDesc(struct PHTreeNodeDesc org)
{
	EPPHTreeNodeDesc *ret = EPPHTreeNodeDesc_new(&EPPHTreeNodeDescType,NULL,NULL);
	ret->ptr = new PHTreeNodeDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHTreeNodeDesc(struct PHTreeNodeDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHTreeNodeDesc *ret = EPPHTreeNodeDesc_new(&EPPHTreeNodeDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHTreeNodeDesc

//{*********EPPHRootNodeDesc*******
static PyMethodDef EPPHRootNodeDesc_methods[] =
{
	{NULL}
};
void EPPHRootNodeDesc_dealloc(PyObject* self)
{
	delete ((EPPHRootNodeDesc*)self)->ptr;
}
PyObject* EPPHRootNodeDesc_str()
{
	return Py_BuildValue("s","This is EPPHRootNodeDesc.");
}
int EPPHRootNodeDesc_init(EPPHRootNodeDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHRootNodeDesc* EPPHRootNodeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHRootNodeDesc *self;
	self = (EPPHRootNodeDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHRootNodeDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHRootNodeDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHRootNodeDesc",/*tp_name*/
	sizeof(EPPHRootNodeDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHRootNodeDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHRootNodeDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHRootNodeDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHRootNodeDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNodeDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHRootNodeDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHRootNodeDesc_new,/*tp_new*/
};
void initEPPHRootNodeDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHRootNodeDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHRootNodeDescType);
	PyModule_AddObject(m,"PHRootNodeDesc",(PyObject *)&EPPHRootNodeDescType);//モジュールに追加
}
PyObject* newEPPHRootNodeDesc(struct PHRootNodeDesc org)
{
	EPPHRootNodeDesc *ret = EPPHRootNodeDesc_new(&EPPHRootNodeDescType,NULL,NULL);
	ret->ptr = new PHRootNodeDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHRootNodeDesc(struct PHRootNodeDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHRootNodeDesc *ret = EPPHRootNodeDesc_new(&EPPHRootNodeDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHRootNodeDesc

//{*********EPPHTreeNode1DDesc*******
static PyMethodDef EPPHTreeNode1DDesc_methods[] =
{
	{NULL}
};
void EPPHTreeNode1DDesc_dealloc(PyObject* self)
{
	delete ((EPPHTreeNode1DDesc*)self)->ptr;
}
PyObject* EPPHTreeNode1DDesc_str()
{
	return Py_BuildValue("s","This is EPPHTreeNode1DDesc.");
}
int EPPHTreeNode1DDesc_init(EPPHTreeNode1DDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHTreeNode1DDesc* EPPHTreeNode1DDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHTreeNode1DDesc *self;
	self = (EPPHTreeNode1DDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHTreeNode1DDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHTreeNode1DDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHTreeNode1DDesc",/*tp_name*/
	sizeof(EPPHTreeNode1DDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHTreeNode1DDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHTreeNode1DDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHTreeNode1DDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHTreeNode1DDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNodeDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHTreeNode1DDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHTreeNode1DDesc_new,/*tp_new*/
};
void initEPPHTreeNode1DDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHTreeNode1DDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHTreeNode1DDescType);
	PyModule_AddObject(m,"PHTreeNode1DDesc",(PyObject *)&EPPHTreeNode1DDescType);//モジュールに追加
}
PyObject* newEPPHTreeNode1DDesc(struct PHTreeNode1DDesc org)
{
	EPPHTreeNode1DDesc *ret = EPPHTreeNode1DDesc_new(&EPPHTreeNode1DDescType,NULL,NULL);
	ret->ptr = new PHTreeNode1DDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHTreeNode1DDesc(struct PHTreeNode1DDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHTreeNode1DDesc *ret = EPPHTreeNode1DDesc_new(&EPPHTreeNode1DDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHTreeNode1DDesc

//{*********EPPHHingeJointNodeDesc*******
static PyMethodDef EPPHHingeJointNodeDesc_methods[] =
{
	{NULL}
};
void EPPHHingeJointNodeDesc_dealloc(PyObject* self)
{
	delete ((EPPHHingeJointNodeDesc*)self)->ptr;
}
PyObject* EPPHHingeJointNodeDesc_str()
{
	return Py_BuildValue("s","This is EPPHHingeJointNodeDesc.");
}
int EPPHHingeJointNodeDesc_init(EPPHHingeJointNodeDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHHingeJointNodeDesc* EPPHHingeJointNodeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHHingeJointNodeDesc *self;
	self = (EPPHHingeJointNodeDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHHingeJointNodeDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHHingeJointNodeDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHHingeJointNodeDesc",/*tp_name*/
	sizeof(EPPHHingeJointNodeDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHHingeJointNodeDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHHingeJointNodeDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHHingeJointNodeDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHHingeJointNodeDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNode1DDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHHingeJointNodeDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHHingeJointNodeDesc_new,/*tp_new*/
};
void initEPPHHingeJointNodeDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHHingeJointNodeDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHHingeJointNodeDescType);
	PyModule_AddObject(m,"PHHingeJointNodeDesc",(PyObject *)&EPPHHingeJointNodeDescType);//モジュールに追加
}
PyObject* newEPPHHingeJointNodeDesc(struct PHHingeJointNodeDesc org)
{
	EPPHHingeJointNodeDesc *ret = EPPHHingeJointNodeDesc_new(&EPPHHingeJointNodeDescType,NULL,NULL);
	ret->ptr = new PHHingeJointNodeDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHHingeJointNodeDesc(struct PHHingeJointNodeDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHHingeJointNodeDesc *ret = EPPHHingeJointNodeDesc_new(&EPPHHingeJointNodeDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHHingeJointNodeDesc

//{*********EPPHSliderJointNodeDesc*******
static PyMethodDef EPPHSliderJointNodeDesc_methods[] =
{
	{NULL}
};
void EPPHSliderJointNodeDesc_dealloc(PyObject* self)
{
	delete ((EPPHSliderJointNodeDesc*)self)->ptr;
}
PyObject* EPPHSliderJointNodeDesc_str()
{
	return Py_BuildValue("s","This is EPPHSliderJointNodeDesc.");
}
int EPPHSliderJointNodeDesc_init(EPPHSliderJointNodeDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSliderJointNodeDesc* EPPHSliderJointNodeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSliderJointNodeDesc *self;
	self = (EPPHSliderJointNodeDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSliderJointNodeDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSliderJointNodeDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSliderJointNodeDesc",/*tp_name*/
	sizeof(EPPHSliderJointNodeDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSliderJointNodeDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSliderJointNodeDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSliderJointNodeDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSliderJointNodeDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNode1DDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSliderJointNodeDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSliderJointNodeDesc_new,/*tp_new*/
};
void initEPPHSliderJointNodeDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSliderJointNodeDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSliderJointNodeDescType);
	PyModule_AddObject(m,"PHSliderJointNodeDesc",(PyObject *)&EPPHSliderJointNodeDescType);//モジュールに追加
}
PyObject* newEPPHSliderJointNodeDesc(struct PHSliderJointNodeDesc org)
{
	EPPHSliderJointNodeDesc *ret = EPPHSliderJointNodeDesc_new(&EPPHSliderJointNodeDescType,NULL,NULL);
	ret->ptr = new PHSliderJointNodeDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSliderJointNodeDesc(struct PHSliderJointNodeDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSliderJointNodeDesc *ret = EPPHSliderJointNodeDesc_new(&EPPHSliderJointNodeDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSliderJointNodeDesc

//{*********EPPHPathJointNodeDesc*******
static PyMethodDef EPPHPathJointNodeDesc_methods[] =
{
	{NULL}
};
void EPPHPathJointNodeDesc_dealloc(PyObject* self)
{
	delete ((EPPHPathJointNodeDesc*)self)->ptr;
}
PyObject* EPPHPathJointNodeDesc_str()
{
	return Py_BuildValue("s","This is EPPHPathJointNodeDesc.");
}
int EPPHPathJointNodeDesc_init(EPPHPathJointNodeDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathJointNodeDesc* EPPHPathJointNodeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathJointNodeDesc *self;
	self = (EPPHPathJointNodeDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathJointNodeDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathJointNodeDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathJointNodeDesc",/*tp_name*/
	sizeof(EPPHPathJointNodeDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathJointNodeDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathJointNodeDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathJointNodeDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathJointNodeDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNode1DDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathJointNodeDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathJointNodeDesc_new,/*tp_new*/
};
void initEPPHPathJointNodeDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathJointNodeDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathJointNodeDescType);
	PyModule_AddObject(m,"PHPathJointNodeDesc",(PyObject *)&EPPHPathJointNodeDescType);//モジュールに追加
}
PyObject* newEPPHPathJointNodeDesc(struct PHPathJointNodeDesc org)
{
	EPPHPathJointNodeDesc *ret = EPPHPathJointNodeDesc_new(&EPPHPathJointNodeDescType,NULL,NULL);
	ret->ptr = new PHPathJointNodeDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathJointNodeDesc(struct PHPathJointNodeDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathJointNodeDesc *ret = EPPHPathJointNodeDesc_new(&EPPHPathJointNodeDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathJointNodeDesc

//{*********EPPHBallJointNodeDesc*******
static PyMethodDef EPPHBallJointNodeDesc_methods[] =
{
	{NULL}
};
void EPPHBallJointNodeDesc_dealloc(PyObject* self)
{
	delete ((EPPHBallJointNodeDesc*)self)->ptr;
}
PyObject* EPPHBallJointNodeDesc_str()
{
	return Py_BuildValue("s","This is EPPHBallJointNodeDesc.");
}
int EPPHBallJointNodeDesc_init(EPPHBallJointNodeDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHBallJointNodeDesc* EPPHBallJointNodeDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHBallJointNodeDesc *self;
	self = (EPPHBallJointNodeDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHBallJointNodeDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHBallJointNodeDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHBallJointNodeDesc",/*tp_name*/
	sizeof(EPPHBallJointNodeDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHBallJointNodeDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHBallJointNodeDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHBallJointNodeDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHBallJointNodeDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNodeDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHBallJointNodeDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHBallJointNodeDesc_new,/*tp_new*/
};
void initEPPHBallJointNodeDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHBallJointNodeDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHBallJointNodeDescType);
	PyModule_AddObject(m,"PHBallJointNodeDesc",(PyObject *)&EPPHBallJointNodeDescType);//モジュールに追加
}
PyObject* newEPPHBallJointNodeDesc(struct PHBallJointNodeDesc org)
{
	EPPHBallJointNodeDesc *ret = EPPHBallJointNodeDesc_new(&EPPHBallJointNodeDescType,NULL,NULL);
	ret->ptr = new PHBallJointNodeDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHBallJointNodeDesc(struct PHBallJointNodeDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHBallJointNodeDesc *ret = EPPHBallJointNodeDesc_new(&EPPHBallJointNodeDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHBallJointNodeDesc

//{*********EPPHGearDesc*******
static PyMethodDef EPPHGearDesc_methods[] =
{
	{NULL}
};
void EPPHGearDesc_dealloc(PyObject* self)
{
	delete ((EPPHGearDesc*)self)->ptr;
}
PyObject* EPPHGearDesc_str()
{
	return Py_BuildValue("s","This is EPPHGearDesc.");
}
int EPPHGearDesc_init(EPPHGearDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHGearDesc* EPPHGearDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHGearDesc *self;
	self = (EPPHGearDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHGearDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHGearDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHGearDesc",/*tp_name*/
	sizeof(EPPHGearDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHGearDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHGearDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHGearDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHGearDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHGearDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHGearDesc_new,/*tp_new*/
};
void initEPPHGearDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHGearDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHGearDescType);
	PyModule_AddObject(m,"PHGearDesc",(PyObject *)&EPPHGearDescType);//モジュールに追加
}
PyObject* newEPPHGearDesc(struct PHGearDesc org)
{
	EPPHGearDesc *ret = EPPHGearDesc_new(&EPPHGearDescType,NULL,NULL);
	ret->ptr = new PHGearDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHGearDesc(struct PHGearDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHGearDesc *ret = EPPHGearDesc_new(&EPPHGearDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHGearDesc

//{*********EPPHConstraintIf*******
PyObject* EPPHConstraintIf_Enable( EPPHConstraintIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->Enable((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->Enable();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetAbsolutePoseQ( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaternionf* ret = (EPQuaternionf*)newEPQuaternionf(self->ptr->GetAbsolutePoseQ());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetConstraintForce( EPPHConstraintIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->GetConstraintForce((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetLimitf( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetLimitf());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetMotorf( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetMotorf());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetPlugPose( EPPHConstraintIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->GetPlugPose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetPlugSolid( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->GetPlugSolid());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetRelativePose( EPPHConstraintIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->GetRelativePose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetRelativePoseQ( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->GetRelativePoseQ());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetRelativePoseR( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetRelativePoseR());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetRelativeVelocity( EPPHConstraintIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->GetRelativeVelocity((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetScene( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSceneIf* ret = (EPPHSceneIf*)newEPPHSceneIf(self->ptr->GetScene());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetSocketPose( EPPHConstraintIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->GetSocketPose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_GetSocketSolid( EPPHConstraintIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->GetSocketSolid());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_IsEnabled( EPPHConstraintIf* self )
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
PyObject* EPPHConstraintIf_SetPlugPose( EPPHConstraintIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->SetPlugPose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintIf_SetSocketPose( EPPHConstraintIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->SetSocketPose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHConstraintIf_methods[] =
{
	{"Enable",(PyCFunction)EPPHConstraintIf_Enable,METH_VARARGS,"EPPHConstraintIf::Enable"},
	{"GetAbsolutePoseQ",(PyCFunction)EPPHConstraintIf_GetAbsolutePoseQ,METH_NOARGS,"EPPHConstraintIf::GetAbsolutePoseQ"},
	{"GetConstraintForce",(PyCFunction)EPPHConstraintIf_GetConstraintForce,METH_VARARGS,"EPPHConstraintIf::GetConstraintForce"},
	{"GetLimitf",(PyCFunction)EPPHConstraintIf_GetLimitf,METH_NOARGS,"EPPHConstraintIf::GetLimitf"},
	{"GetMotorf",(PyCFunction)EPPHConstraintIf_GetMotorf,METH_NOARGS,"EPPHConstraintIf::GetMotorf"},
	{"GetPlugPose",(PyCFunction)EPPHConstraintIf_GetPlugPose,METH_O,"EPPHConstraintIf::GetPlugPose"},
	{"GetPlugSolid",(PyCFunction)EPPHConstraintIf_GetPlugSolid,METH_NOARGS,"EPPHConstraintIf::GetPlugSolid"},
	{"GetRelativePose",(PyCFunction)EPPHConstraintIf_GetRelativePose,METH_O,"EPPHConstraintIf::GetRelativePose"},
	{"GetRelativePoseQ",(PyCFunction)EPPHConstraintIf_GetRelativePoseQ,METH_NOARGS,"EPPHConstraintIf::GetRelativePoseQ"},
	{"GetRelativePoseR",(PyCFunction)EPPHConstraintIf_GetRelativePoseR,METH_NOARGS,"EPPHConstraintIf::GetRelativePoseR"},
	{"GetRelativeVelocity",(PyCFunction)EPPHConstraintIf_GetRelativeVelocity,METH_VARARGS,"EPPHConstraintIf::GetRelativeVelocity"},
	{"GetScene",(PyCFunction)EPPHConstraintIf_GetScene,METH_NOARGS,"EPPHConstraintIf::GetScene"},
	{"GetSocketPose",(PyCFunction)EPPHConstraintIf_GetSocketPose,METH_O,"EPPHConstraintIf::GetSocketPose"},
	{"GetSocketSolid",(PyCFunction)EPPHConstraintIf_GetSocketSolid,METH_NOARGS,"EPPHConstraintIf::GetSocketSolid"},
	{"IsEnabled",(PyCFunction)EPPHConstraintIf_IsEnabled,METH_NOARGS,"EPPHConstraintIf::IsEnabled"},
	{"SetPlugPose",(PyCFunction)EPPHConstraintIf_SetPlugPose,METH_O,"EPPHConstraintIf::SetPlugPose"},
	{"SetSocketPose",(PyCFunction)EPPHConstraintIf_SetSocketPose,METH_O,"EPPHConstraintIf::SetSocketPose"},
	{NULL}
};
void EPPHConstraintIf_dealloc(PyObject* self)
{
}
PyObject* EPPHConstraintIf_str()
{
	return Py_BuildValue("s","This is EPPHConstraintIf.");
}
int EPPHConstraintIf_init(EPPHConstraintIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHConstraintIf* EPPHConstraintIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHConstraintIf *self;
	self = (EPPHConstraintIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHConstraintIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHConstraintIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHConstraintIf",/*tp_name*/
	sizeof(EPPHConstraintIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHConstraintIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHConstraintIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHConstraintIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHConstraintIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHConstraintIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHConstraintIf_new,/*tp_new*/
};
void initEPPHConstraintIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHConstraintIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHConstraintIfType);
	PyModule_AddObject(m,"PHConstraintIf",(PyObject *)&EPPHConstraintIfType);//モジュールに追加
}
PyObject* newEPPHConstraintIf(struct PHConstraintIf org)
{
	EPPHConstraintIf *ret = EPPHConstraintIf_new(&EPPHConstraintIfType,NULL,NULL);
	ret->ptr = new PHConstraintIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHConstraintIf(struct PHConstraintIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHConstraintIf *ret = EPPHConstraintIf_new(&EPPHConstraintIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHConstraintIf

//{*********EPPHConstraintsIf*******
PyObject* EPPHConstraintsIf_FindBySolidPair( EPPHConstraintsIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))) && EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))))
	{
		EPPHConstraintIf* ret = (EPPHConstraintIf*)newEPPHConstraintIf(self->ptr->FindBySolidPair((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHConstraintsIf_GetTotalForce( EPPHConstraintsIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))) && EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTotalForce((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHConstraintsIf_methods[] =
{
	{"FindBySolidPair",(PyCFunction)EPPHConstraintsIf_FindBySolidPair,METH_VARARGS,"EPPHConstraintsIf::FindBySolidPair"},
	{"GetTotalForce",(PyCFunction)EPPHConstraintsIf_GetTotalForce,METH_VARARGS,"EPPHConstraintsIf::GetTotalForce"},
	{NULL}
};
void EPPHConstraintsIf_dealloc(PyObject* self)
{
}
PyObject* EPPHConstraintsIf_str()
{
	return Py_BuildValue("s","This is EPPHConstraintsIf.");
}
int EPPHConstraintsIf_init(EPPHConstraintsIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHConstraintsIf* EPPHConstraintsIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHConstraintsIf *self;
	self = (EPPHConstraintsIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHConstraintsIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHConstraintsIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHConstraintsIf",/*tp_name*/
	sizeof(EPPHConstraintsIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHConstraintsIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHConstraintsIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHConstraintsIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHConstraintsIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHConstraintsIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHConstraintsIf_new,/*tp_new*/
};
void initEPPHConstraintsIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHConstraintsIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHConstraintsIfType);
	PyModule_AddObject(m,"PHConstraintsIf",(PyObject *)&EPPHConstraintsIfType);//モジュールに追加
}
PyObject* newEPPHConstraintsIf(struct PHConstraintsIf org)
{
	EPPHConstraintsIf *ret = EPPHConstraintsIf_new(&EPPHConstraintsIfType,NULL,NULL);
	ret->ptr = new PHConstraintsIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHConstraintsIf(struct PHConstraintsIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHConstraintsIf *ret = EPPHConstraintsIf_new(&EPPHConstraintsIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHConstraintsIf

//{*********EPPHContactPointIf*******
static PyMethodDef EPPHContactPointIf_methods[] =
{
	{NULL}
};
void EPPHContactPointIf_dealloc(PyObject* self)
{
}
PyObject* EPPHContactPointIf_str()
{
	return Py_BuildValue("s","This is EPPHContactPointIf.");
}
int EPPHContactPointIf_init(EPPHContactPointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHContactPointIf* EPPHContactPointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHContactPointIf *self;
	self = (EPPHContactPointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHContactPointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHContactPointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHContactPointIf",/*tp_name*/
	sizeof(EPPHContactPointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHContactPointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHContactPointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHContactPointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHContactPointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHConstraintIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHContactPointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHContactPointIf_new,/*tp_new*/
};
void initEPPHContactPointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHContactPointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHContactPointIfType);
	PyModule_AddObject(m,"PHContactPointIf",(PyObject *)&EPPHContactPointIfType);//モジュールに追加
}
PyObject* newEPPHContactPointIf(struct PHContactPointIf org)
{
	EPPHContactPointIf *ret = EPPHContactPointIf_new(&EPPHContactPointIfType,NULL,NULL);
	ret->ptr = new PHContactPointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHContactPointIf(struct PHContactPointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHContactPointIf *ret = EPPHContactPointIf_new(&EPPHContactPointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHContactPointIf

//{*********EPPHJointIf*******
PyObject* EPPHJointIf_GetDefomationType( EPPHJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetDefomationType());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJointIf_SetDefomationType( EPPHJointIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->SetDefomationType(PyLong_AsLong((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHJointIf_methods[] =
{
	{"GetDefomationType",(PyCFunction)EPPHJointIf_GetDefomationType,METH_NOARGS,"EPPHJointIf::GetDefomationType"},
	{"SetDefomationType",(PyCFunction)EPPHJointIf_SetDefomationType,METH_O,"EPPHJointIf::SetDefomationType"},
	{NULL}
};
void EPPHJointIf_dealloc(PyObject* self)
{
}
PyObject* EPPHJointIf_str()
{
	return Py_BuildValue("s","This is EPPHJointIf.");
}
int EPPHJointIf_init(EPPHJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHJointIf* EPPHJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHJointIf *self;
	self = (EPPHJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHJointIf",/*tp_name*/
	sizeof(EPPHJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHConstraintIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHJointIf_new,/*tp_new*/
};
void initEPPHJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHJointIfType);
	PyModule_AddObject(m,"PHJointIf",(PyObject *)&EPPHJointIfType);//モジュールに追加
}
PyObject* newEPPHJointIf(struct PHJointIf org)
{
	EPPHJointIf *ret = EPPHJointIf_new(&EPPHJointIfType,NULL,NULL);
	ret->ptr = new PHJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHJointIf(struct PHJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHJointIf *ret = EPPHJointIf_new(&EPPHJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHJointIf

//{*********EPPHJoint1DIf*******
PyObject* EPPHJoint1DIf_GetDamper( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetDeformationMode( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetDeformationMode());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetHardnessRate( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetHardnessRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetMotorTorque( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetMotorTorque());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetOffsetForce( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetOffsetForce());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetPosition( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetRange( EPPHJoint1DIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->GetRange(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetRangeDamper( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetRangeDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetRangeSpring( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetRangeSpring());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetSecondDamper( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSecondDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetSpring( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSpring());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetTargetPosition( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTargetPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetTargetVelocity( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTargetVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetTorqueMax( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTorqueMax());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetTrajectoryVelocity( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTrajectoryVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetVelocity( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_GetYieldStress( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetYieldStress());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_IsLimit( EPPHJoint1DIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsLimit()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetDamper( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetDamper(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetHardnessRate( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetHardnessRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetMotorTorque( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetMotorTorque(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetOffsetForce( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetOffsetForce(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetRange( EPPHJoint1DIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetRange(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,1))));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetRangeDamper( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetRangeDamper(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetRangeSpring( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetRangeSpring(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetSecondDamper( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetSecondDamper(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetSpring( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetSpring(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetTargetPosition( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTargetPosition(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetTargetVelocity( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTargetVelocity(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetTorqueMax( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTorqueMax(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetTrajectoryVelocity( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTrajectoryVelocity(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHJoint1DIf_SetYieldStress( EPPHJoint1DIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetYieldStress(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHJoint1DIf_methods[] =
{
	{"GetDamper",(PyCFunction)EPPHJoint1DIf_GetDamper,METH_NOARGS,"EPPHJoint1DIf::GetDamper"},
	{"GetDeformationMode",(PyCFunction)EPPHJoint1DIf_GetDeformationMode,METH_NOARGS,"EPPHJoint1DIf::GetDeformationMode"},
	{"GetHardnessRate",(PyCFunction)EPPHJoint1DIf_GetHardnessRate,METH_NOARGS,"EPPHJoint1DIf::GetHardnessRate"},
	{"GetMotorTorque",(PyCFunction)EPPHJoint1DIf_GetMotorTorque,METH_NOARGS,"EPPHJoint1DIf::GetMotorTorque"},
	{"GetOffsetForce",(PyCFunction)EPPHJoint1DIf_GetOffsetForce,METH_NOARGS,"EPPHJoint1DIf::GetOffsetForce"},
	{"GetPosition",(PyCFunction)EPPHJoint1DIf_GetPosition,METH_NOARGS,"EPPHJoint1DIf::GetPosition"},
	{"GetRange",(PyCFunction)EPPHJoint1DIf_GetRange,METH_VARARGS,"EPPHJoint1DIf::GetRange"},
	{"GetRangeDamper",(PyCFunction)EPPHJoint1DIf_GetRangeDamper,METH_NOARGS,"EPPHJoint1DIf::GetRangeDamper"},
	{"GetRangeSpring",(PyCFunction)EPPHJoint1DIf_GetRangeSpring,METH_NOARGS,"EPPHJoint1DIf::GetRangeSpring"},
	{"GetSecondDamper",(PyCFunction)EPPHJoint1DIf_GetSecondDamper,METH_NOARGS,"EPPHJoint1DIf::GetSecondDamper"},
	{"GetSpring",(PyCFunction)EPPHJoint1DIf_GetSpring,METH_NOARGS,"EPPHJoint1DIf::GetSpring"},
	{"GetTargetPosition",(PyCFunction)EPPHJoint1DIf_GetTargetPosition,METH_NOARGS,"EPPHJoint1DIf::GetTargetPosition"},
	{"GetTargetVelocity",(PyCFunction)EPPHJoint1DIf_GetTargetVelocity,METH_NOARGS,"EPPHJoint1DIf::GetTargetVelocity"},
	{"GetTorqueMax",(PyCFunction)EPPHJoint1DIf_GetTorqueMax,METH_NOARGS,"EPPHJoint1DIf::GetTorqueMax"},
	{"GetTrajectoryVelocity",(PyCFunction)EPPHJoint1DIf_GetTrajectoryVelocity,METH_NOARGS,"EPPHJoint1DIf::GetTrajectoryVelocity"},
	{"GetVelocity",(PyCFunction)EPPHJoint1DIf_GetVelocity,METH_NOARGS,"EPPHJoint1DIf::GetVelocity"},
	{"GetYieldStress",(PyCFunction)EPPHJoint1DIf_GetYieldStress,METH_NOARGS,"EPPHJoint1DIf::GetYieldStress"},
	{"IsLimit",(PyCFunction)EPPHJoint1DIf_IsLimit,METH_NOARGS,"EPPHJoint1DIf::IsLimit"},
	{"SetDamper",(PyCFunction)EPPHJoint1DIf_SetDamper,METH_O,"EPPHJoint1DIf::SetDamper"},
	{"SetHardnessRate",(PyCFunction)EPPHJoint1DIf_SetHardnessRate,METH_O,"EPPHJoint1DIf::SetHardnessRate"},
	{"SetMotorTorque",(PyCFunction)EPPHJoint1DIf_SetMotorTorque,METH_O,"EPPHJoint1DIf::SetMotorTorque"},
	{"SetOffsetForce",(PyCFunction)EPPHJoint1DIf_SetOffsetForce,METH_O,"EPPHJoint1DIf::SetOffsetForce"},
	{"SetRange",(PyCFunction)EPPHJoint1DIf_SetRange,METH_VARARGS,"EPPHJoint1DIf::SetRange"},
	{"SetRangeDamper",(PyCFunction)EPPHJoint1DIf_SetRangeDamper,METH_O,"EPPHJoint1DIf::SetRangeDamper"},
	{"SetRangeSpring",(PyCFunction)EPPHJoint1DIf_SetRangeSpring,METH_O,"EPPHJoint1DIf::SetRangeSpring"},
	{"SetSecondDamper",(PyCFunction)EPPHJoint1DIf_SetSecondDamper,METH_O,"EPPHJoint1DIf::SetSecondDamper"},
	{"SetSpring",(PyCFunction)EPPHJoint1DIf_SetSpring,METH_O,"EPPHJoint1DIf::SetSpring"},
	{"SetTargetPosition",(PyCFunction)EPPHJoint1DIf_SetTargetPosition,METH_O,"EPPHJoint1DIf::SetTargetPosition"},
	{"SetTargetVelocity",(PyCFunction)EPPHJoint1DIf_SetTargetVelocity,METH_O,"EPPHJoint1DIf::SetTargetVelocity"},
	{"SetTorqueMax",(PyCFunction)EPPHJoint1DIf_SetTorqueMax,METH_O,"EPPHJoint1DIf::SetTorqueMax"},
	{"SetTrajectoryVelocity",(PyCFunction)EPPHJoint1DIf_SetTrajectoryVelocity,METH_O,"EPPHJoint1DIf::SetTrajectoryVelocity"},
	{"SetYieldStress",(PyCFunction)EPPHJoint1DIf_SetYieldStress,METH_O,"EPPHJoint1DIf::SetYieldStress"},
	{NULL}
};
void EPPHJoint1DIf_dealloc(PyObject* self)
{
}
PyObject* EPPHJoint1DIf_str()
{
	return Py_BuildValue("s","This is EPPHJoint1DIf.");
}
int EPPHJoint1DIf_init(EPPHJoint1DIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHJoint1DIf* EPPHJoint1DIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHJoint1DIf *self;
	self = (EPPHJoint1DIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHJoint1DIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHJoint1DIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHJoint1DIf",/*tp_name*/
	sizeof(EPPHJoint1DIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHJoint1DIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHJoint1DIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHJoint1DIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHJoint1DIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJointIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHJoint1DIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHJoint1DIf_new,/*tp_new*/
};
void initEPPHJoint1DIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHJoint1DIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHJoint1DIfType);
	PyModule_AddObject(m,"PHJoint1DIf",(PyObject *)&EPPHJoint1DIfType);//モジュールに追加
}
PyObject* newEPPHJoint1DIf(struct PHJoint1DIf org)
{
	EPPHJoint1DIf *ret = EPPHJoint1DIf_new(&EPPHJoint1DIfType,NULL,NULL);
	ret->ptr = new PHJoint1DIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHJoint1DIf(struct PHJoint1DIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHJoint1DIf *ret = EPPHJoint1DIf_new(&EPPHJoint1DIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHJoint1DIf

//{*********EPPHHingeJointIf*******
static PyMethodDef EPPHHingeJointIf_methods[] =
{
	{NULL}
};
void EPPHHingeJointIf_dealloc(PyObject* self)
{
}
PyObject* EPPHHingeJointIf_str()
{
	return Py_BuildValue("s","This is EPPHHingeJointIf.");
}
int EPPHHingeJointIf_init(EPPHHingeJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHHingeJointIf* EPPHHingeJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHHingeJointIf *self;
	self = (EPPHHingeJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHHingeJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHHingeJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHHingeJointIf",/*tp_name*/
	sizeof(EPPHHingeJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHHingeJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHHingeJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHHingeJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHHingeJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJoint1DIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHHingeJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHHingeJointIf_new,/*tp_new*/
};
void initEPPHHingeJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHHingeJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHHingeJointIfType);
	PyModule_AddObject(m,"PHHingeJointIf",(PyObject *)&EPPHHingeJointIfType);//モジュールに追加
}
PyObject* newEPPHHingeJointIf(struct PHHingeJointIf org)
{
	EPPHHingeJointIf *ret = EPPHHingeJointIf_new(&EPPHHingeJointIfType,NULL,NULL);
	ret->ptr = new PHHingeJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHHingeJointIf(struct PHHingeJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHHingeJointIf *ret = EPPHHingeJointIf_new(&EPPHHingeJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHHingeJointIf

//{*********EPPHHingeJointDesc*******
static PyMethodDef EPPHHingeJointDesc_methods[] =
{
	{NULL}
};
void EPPHHingeJointDesc_dealloc(PyObject* self)
{
	delete ((EPPHHingeJointDesc*)self)->ptr;
}
PyObject* EPPHHingeJointDesc_str()
{
	return Py_BuildValue("s","This is EPPHHingeJointDesc.");
}
int EPPHHingeJointDesc_init(EPPHHingeJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHHingeJointDesc* EPPHHingeJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHHingeJointDesc *self;
	self = (EPPHHingeJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHHingeJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHHingeJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHHingeJointDesc",/*tp_name*/
	sizeof(EPPHHingeJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHHingeJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHHingeJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHHingeJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHHingeJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJoint1DDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHHingeJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHHingeJointDesc_new,/*tp_new*/
};
void initEPPHHingeJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHHingeJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHHingeJointDescType);
	PyModule_AddObject(m,"PHHingeJointDesc",(PyObject *)&EPPHHingeJointDescType);//モジュールに追加
}
PyObject* newEPPHHingeJointDesc(struct PHHingeJointDesc org)
{
	EPPHHingeJointDesc *ret = EPPHHingeJointDesc_new(&EPPHHingeJointDescType,NULL,NULL);
	ret->ptr = new PHHingeJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHHingeJointDesc(struct PHHingeJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHHingeJointDesc *ret = EPPHHingeJointDesc_new(&EPPHHingeJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHHingeJointDesc

//{*********EPPHSliderJointIf*******
static PyMethodDef EPPHSliderJointIf_methods[] =
{
	{NULL}
};
void EPPHSliderJointIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSliderJointIf_str()
{
	return Py_BuildValue("s","This is EPPHSliderJointIf.");
}
int EPPHSliderJointIf_init(EPPHSliderJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSliderJointIf* EPPHSliderJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSliderJointIf *self;
	self = (EPPHSliderJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSliderJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSliderJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSliderJointIf",/*tp_name*/
	sizeof(EPPHSliderJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSliderJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSliderJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSliderJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSliderJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJoint1DIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSliderJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSliderJointIf_new,/*tp_new*/
};
void initEPPHSliderJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSliderJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSliderJointIfType);
	PyModule_AddObject(m,"PHSliderJointIf",(PyObject *)&EPPHSliderJointIfType);//モジュールに追加
}
PyObject* newEPPHSliderJointIf(struct PHSliderJointIf org)
{
	EPPHSliderJointIf *ret = EPPHSliderJointIf_new(&EPPHSliderJointIfType,NULL,NULL);
	ret->ptr = new PHSliderJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSliderJointIf(struct PHSliderJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSliderJointIf *ret = EPPHSliderJointIf_new(&EPPHSliderJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSliderJointIf

//{*********EPPHSliderJointDesc*******
static PyMethodDef EPPHSliderJointDesc_methods[] =
{
	{NULL}
};
void EPPHSliderJointDesc_dealloc(PyObject* self)
{
	delete ((EPPHSliderJointDesc*)self)->ptr;
}
PyObject* EPPHSliderJointDesc_str()
{
	return Py_BuildValue("s","This is EPPHSliderJointDesc.");
}
int EPPHSliderJointDesc_init(EPPHSliderJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSliderJointDesc* EPPHSliderJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSliderJointDesc *self;
	self = (EPPHSliderJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSliderJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSliderJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSliderJointDesc",/*tp_name*/
	sizeof(EPPHSliderJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSliderJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSliderJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSliderJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSliderJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJoint1DDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSliderJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSliderJointDesc_new,/*tp_new*/
};
void initEPPHSliderJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSliderJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSliderJointDescType);
	PyModule_AddObject(m,"PHSliderJointDesc",(PyObject *)&EPPHSliderJointDescType);//モジュールに追加
}
PyObject* newEPPHSliderJointDesc(struct PHSliderJointDesc org)
{
	EPPHSliderJointDesc *ret = EPPHSliderJointDesc_new(&EPPHSliderJointDescType,NULL,NULL);
	ret->ptr = new PHSliderJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSliderJointDesc(struct PHSliderJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSliderJointDesc *ret = EPPHSliderJointDesc_new(&EPPHSliderJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSliderJointDesc

//{*********EPPHPathPoint*******
static PyMethodDef EPPHPathPoint_methods[] =
{
	{NULL}
};
void EPPHPathPoint_dealloc(PyObject* self)
{
	delete ((EPPHPathPoint*)self)->ptr;
}
PyObject* EPPHPathPoint_str()
{
	return Py_BuildValue("s","This is EPPHPathPoint.");
}
int EPPHPathPoint_init(EPPHPathPoint* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathPoint* EPPHPathPoint_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathPoint *self;
	self = (EPPHPathPoint*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathPoint_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathPointType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathPoint",/*tp_name*/
	sizeof(EPPHPathPoint),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathPoint_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathPoint_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathPoint",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathPoint_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathPoint_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathPoint_new,/*tp_new*/
};
void initEPPHPathPoint(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathPointType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathPointType);
	PyModule_AddObject(m,"PHPathPoint",(PyObject *)&EPPHPathPointType);//モジュールに追加
}
PyObject* newEPPHPathPoint(struct PHPathPoint org)
{
	EPPHPathPoint *ret = EPPHPathPoint_new(&EPPHPathPointType,NULL,NULL);
	ret->ptr = new PHPathPoint();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathPoint(struct PHPathPoint* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathPoint *ret = EPPHPathPoint_new(&EPPHPathPointType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathPoint

//{*********EPPHPathDesc*******
static PyMethodDef EPPHPathDesc_methods[] =
{
	{NULL}
};
void EPPHPathDesc_dealloc(PyObject* self)
{
	delete ((EPPHPathDesc*)self)->ptr;
}
PyObject* EPPHPathDesc_str()
{
	return Py_BuildValue("s","This is EPPHPathDesc.");
}
int EPPHPathDesc_init(EPPHPathDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathDesc* EPPHPathDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathDesc *self;
	self = (EPPHPathDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathDesc",/*tp_name*/
	sizeof(EPPHPathDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathDesc_new,/*tp_new*/
};
void initEPPHPathDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathDescType);
	PyModule_AddObject(m,"PHPathDesc",(PyObject *)&EPPHPathDescType);//モジュールに追加
}
PyObject* newEPPHPathDesc(struct PHPathDesc org)
{
	EPPHPathDesc *ret = EPPHPathDesc_new(&EPPHPathDescType,NULL,NULL);
	ret->ptr = new PHPathDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathDesc(struct PHPathDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathDesc *ret = EPPHPathDesc_new(&EPPHPathDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathDesc

//{*********EPPHPathIf*******
PyObject* EPPHPathIf_AddPoint( EPPHPathIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyFloat_Check(((PyFloatObject*)PyTuple_GetItem(tuple,0))) && EPPosed_Check(((EPPosed*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->AddPoint(PyFloat_AS_DOUBLE((PyObject*)((PyFloatObject*)PyTuple_GetItem(tuple,0))),(*(((EPPosed*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHPathIf_IsLoop( EPPHPathIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsLoop()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHPathIf_SetLoop( EPPHPathIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->SetLoop((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->SetLoop();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHPathIf_methods[] =
{
	{"AddPoint",(PyCFunction)EPPHPathIf_AddPoint,METH_VARARGS,"EPPHPathIf::AddPoint"},
	{"IsLoop",(PyCFunction)EPPHPathIf_IsLoop,METH_NOARGS,"EPPHPathIf::IsLoop"},
	{"SetLoop",(PyCFunction)EPPHPathIf_SetLoop,METH_VARARGS,"EPPHPathIf::SetLoop"},
	{NULL}
};
void EPPHPathIf_dealloc(PyObject* self)
{
}
PyObject* EPPHPathIf_str()
{
	return Py_BuildValue("s","This is EPPHPathIf.");
}
int EPPHPathIf_init(EPPHPathIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathIf* EPPHPathIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathIf *self;
	self = (EPPHPathIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathIf",/*tp_name*/
	sizeof(EPPHPathIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathIf_new,/*tp_new*/
};
void initEPPHPathIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathIfType);
	PyModule_AddObject(m,"PHPathIf",(PyObject *)&EPPHPathIfType);//モジュールに追加
}
PyObject* newEPPHPathIf(struct PHPathIf org)
{
	EPPHPathIf *ret = EPPHPathIf_new(&EPPHPathIfType,NULL,NULL);
	ret->ptr = new PHPathIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathIf(struct PHPathIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathIf *ret = EPPHPathIf_new(&EPPHPathIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathIf

//{*********EPPHPathJointIf*******
PyObject* EPPHPathJointIf_SetPosition( EPPHPathJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetPosition(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHPathJointIf_methods[] =
{
	{"SetPosition",(PyCFunction)EPPHPathJointIf_SetPosition,METH_O,"EPPHPathJointIf::SetPosition"},
	{NULL}
};
void EPPHPathJointIf_dealloc(PyObject* self)
{
}
PyObject* EPPHPathJointIf_str()
{
	return Py_BuildValue("s","This is EPPHPathJointIf.");
}
int EPPHPathJointIf_init(EPPHPathJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathJointIf* EPPHPathJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathJointIf *self;
	self = (EPPHPathJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathJointIf",/*tp_name*/
	sizeof(EPPHPathJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJoint1DIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathJointIf_new,/*tp_new*/
};
void initEPPHPathJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathJointIfType);
	PyModule_AddObject(m,"PHPathJointIf",(PyObject *)&EPPHPathJointIfType);//モジュールに追加
}
PyObject* newEPPHPathJointIf(struct PHPathJointIf org)
{
	EPPHPathJointIf *ret = EPPHPathJointIf_new(&EPPHPathJointIfType,NULL,NULL);
	ret->ptr = new PHPathJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathJointIf(struct PHPathJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathJointIf *ret = EPPHPathJointIf_new(&EPPHPathJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathJointIf

//{*********EPPHPathJointDesc*******
static PyMethodDef EPPHPathJointDesc_methods[] =
{
	{NULL}
};
void EPPHPathJointDesc_dealloc(PyObject* self)
{
	delete ((EPPHPathJointDesc*)self)->ptr;
}
PyObject* EPPHPathJointDesc_str()
{
	return Py_BuildValue("s","This is EPPHPathJointDesc.");
}
int EPPHPathJointDesc_init(EPPHPathJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathJointDesc* EPPHPathJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathJointDesc *self;
	self = (EPPHPathJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathJointDesc",/*tp_name*/
	sizeof(EPPHPathJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJoint1DDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathJointDesc_new,/*tp_new*/
};
void initEPPHPathJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathJointDescType);
	PyModule_AddObject(m,"PHPathJointDesc",(PyObject *)&EPPHPathJointDescType);//モジュールに追加
}
PyObject* newEPPHPathJointDesc(struct PHPathJointDesc org)
{
	EPPHPathJointDesc *ret = EPPHPathJointDesc_new(&EPPHPathJointDescType,NULL,NULL);
	ret->ptr = new PHPathJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathJointDesc(struct PHPathJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathJointDesc *ret = EPPHPathJointDesc_new(&EPPHPathJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathJointDesc

//{*********EPPHBallJointIf*******
PyObject* EPPHBallJointIf_GetAngle( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetAngle());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetConstLine( EPPHBallJointIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetConstLine(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetDamper( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetDeformationMode( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetDeformationMode());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetHardnessRate( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetHardnessRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetInertia( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetInertia());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetMotorTorque( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetMotorTorque());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetOffsetForce( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetOffsetForce());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetPosition( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->GetPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetSecondDamper( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSecondDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetSpring( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSpring());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetSwingRange( EPPHBallJointIf* self,EPVec2d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec2d_Check(var1))
	{
		self->ptr->GetSwingRange((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetTargetPosition( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->GetTargetPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetTargetVelocity( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTargetVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetTorqueMax( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTorqueMax());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetTrajectoryVelocity( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTrajectoryVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetTwistRange( EPPHBallJointIf* self,EPVec2d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec2d_Check(var1))
	{
		self->ptr->GetTwistRange((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetVelocity( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetYieldStress( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetYieldStress());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_GetmotorfNorm( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetmotorfNorm());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_IsLimit( EPPHBallJointIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsLimit()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetConstLine( EPPHBallJointIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPchar_Check(((EPchar*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->SetConstLine((((EPchar*)PyTuple_GetItem(tuple,0))->ptr),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1))))? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetDamper( EPPHBallJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetDamper(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetHardnessRate( EPPHBallJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetHardnessRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetInertia( EPPHBallJointIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetInertia((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetMotorTorque( EPPHBallJointIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetMotorTorque((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetOffsetForce( EPPHBallJointIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetOffsetForce((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetSecondDamper( EPPHBallJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetSecondDamper(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetSpring( EPPHBallJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetSpring(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetSwingRange( EPPHBallJointIf* self,EPVec2d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec2d_Check(var1))
	{
		self->ptr->SetSwingRange((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetTargetPosition( EPPHBallJointIf* self,EPQuaterniond* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPQuaterniond_Check(var1))
	{
		self->ptr->SetTargetPosition((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetTargetVelocity( EPPHBallJointIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetTargetVelocity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetTorqueMax( EPPHBallJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTorqueMax(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetTrajectoryVelocity( EPPHBallJointIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetTrajectoryVelocity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetTwistPole( EPPHBallJointIf* self,EPVec2d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec2d_Check(var1))
	{
		self->ptr->SetTwistPole((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetTwistRange( EPPHBallJointIf* self,EPVec2d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec2d_Check(var1))
	{
		self->ptr->SetTwistRange((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHBallJointIf_SetYieldStress( EPPHBallJointIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetYieldStress(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHBallJointIf_methods[] =
{
	{"GetAngle",(PyCFunction)EPPHBallJointIf_GetAngle,METH_NOARGS,"EPPHBallJointIf::GetAngle"},
	{"GetConstLine",(PyCFunction)EPPHBallJointIf_GetConstLine,METH_VARARGS,"EPPHBallJointIf::GetConstLine"},
	{"GetDamper",(PyCFunction)EPPHBallJointIf_GetDamper,METH_NOARGS,"EPPHBallJointIf::GetDamper"},
	{"GetDeformationMode",(PyCFunction)EPPHBallJointIf_GetDeformationMode,METH_NOARGS,"EPPHBallJointIf::GetDeformationMode"},
	{"GetHardnessRate",(PyCFunction)EPPHBallJointIf_GetHardnessRate,METH_NOARGS,"EPPHBallJointIf::GetHardnessRate"},
	{"GetInertia",(PyCFunction)EPPHBallJointIf_GetInertia,METH_NOARGS,"EPPHBallJointIf::GetInertia"},
	{"GetMotorTorque",(PyCFunction)EPPHBallJointIf_GetMotorTorque,METH_NOARGS,"EPPHBallJointIf::GetMotorTorque"},
	{"GetOffsetForce",(PyCFunction)EPPHBallJointIf_GetOffsetForce,METH_NOARGS,"EPPHBallJointIf::GetOffsetForce"},
	{"GetPosition",(PyCFunction)EPPHBallJointIf_GetPosition,METH_NOARGS,"EPPHBallJointIf::GetPosition"},
	{"GetSecondDamper",(PyCFunction)EPPHBallJointIf_GetSecondDamper,METH_NOARGS,"EPPHBallJointIf::GetSecondDamper"},
	{"GetSpring",(PyCFunction)EPPHBallJointIf_GetSpring,METH_NOARGS,"EPPHBallJointIf::GetSpring"},
	{"GetSwingRange",(PyCFunction)EPPHBallJointIf_GetSwingRange,METH_O,"EPPHBallJointIf::GetSwingRange"},
	{"GetTargetPosition",(PyCFunction)EPPHBallJointIf_GetTargetPosition,METH_NOARGS,"EPPHBallJointIf::GetTargetPosition"},
	{"GetTargetVelocity",(PyCFunction)EPPHBallJointIf_GetTargetVelocity,METH_NOARGS,"EPPHBallJointIf::GetTargetVelocity"},
	{"GetTorqueMax",(PyCFunction)EPPHBallJointIf_GetTorqueMax,METH_NOARGS,"EPPHBallJointIf::GetTorqueMax"},
	{"GetTrajectoryVelocity",(PyCFunction)EPPHBallJointIf_GetTrajectoryVelocity,METH_NOARGS,"EPPHBallJointIf::GetTrajectoryVelocity"},
	{"GetTwistRange",(PyCFunction)EPPHBallJointIf_GetTwistRange,METH_O,"EPPHBallJointIf::GetTwistRange"},
	{"GetVelocity",(PyCFunction)EPPHBallJointIf_GetVelocity,METH_NOARGS,"EPPHBallJointIf::GetVelocity"},
	{"GetYieldStress",(PyCFunction)EPPHBallJointIf_GetYieldStress,METH_NOARGS,"EPPHBallJointIf::GetYieldStress"},
	{"GetmotorfNorm",(PyCFunction)EPPHBallJointIf_GetmotorfNorm,METH_NOARGS,"EPPHBallJointIf::GetmotorfNorm"},
	{"IsLimit",(PyCFunction)EPPHBallJointIf_IsLimit,METH_NOARGS,"EPPHBallJointIf::IsLimit"},
	{"SetConstLine",(PyCFunction)EPPHBallJointIf_SetConstLine,METH_VARARGS,"EPPHBallJointIf::SetConstLine"},
	{"SetDamper",(PyCFunction)EPPHBallJointIf_SetDamper,METH_O,"EPPHBallJointIf::SetDamper"},
	{"SetHardnessRate",(PyCFunction)EPPHBallJointIf_SetHardnessRate,METH_O,"EPPHBallJointIf::SetHardnessRate"},
	{"SetInertia",(PyCFunction)EPPHBallJointIf_SetInertia,METH_O,"EPPHBallJointIf::SetInertia"},
	{"SetMotorTorque",(PyCFunction)EPPHBallJointIf_SetMotorTorque,METH_O,"EPPHBallJointIf::SetMotorTorque"},
	{"SetOffsetForce",(PyCFunction)EPPHBallJointIf_SetOffsetForce,METH_O,"EPPHBallJointIf::SetOffsetForce"},
	{"SetSecondDamper",(PyCFunction)EPPHBallJointIf_SetSecondDamper,METH_O,"EPPHBallJointIf::SetSecondDamper"},
	{"SetSpring",(PyCFunction)EPPHBallJointIf_SetSpring,METH_O,"EPPHBallJointIf::SetSpring"},
	{"SetSwingRange",(PyCFunction)EPPHBallJointIf_SetSwingRange,METH_O,"EPPHBallJointIf::SetSwingRange"},
	{"SetTargetPosition",(PyCFunction)EPPHBallJointIf_SetTargetPosition,METH_O,"EPPHBallJointIf::SetTargetPosition"},
	{"SetTargetVelocity",(PyCFunction)EPPHBallJointIf_SetTargetVelocity,METH_O,"EPPHBallJointIf::SetTargetVelocity"},
	{"SetTorqueMax",(PyCFunction)EPPHBallJointIf_SetTorqueMax,METH_O,"EPPHBallJointIf::SetTorqueMax"},
	{"SetTrajectoryVelocity",(PyCFunction)EPPHBallJointIf_SetTrajectoryVelocity,METH_O,"EPPHBallJointIf::SetTrajectoryVelocity"},
	{"SetTwistPole",(PyCFunction)EPPHBallJointIf_SetTwistPole,METH_O,"EPPHBallJointIf::SetTwistPole"},
	{"SetTwistRange",(PyCFunction)EPPHBallJointIf_SetTwistRange,METH_O,"EPPHBallJointIf::SetTwistRange"},
	{"SetYieldStress",(PyCFunction)EPPHBallJointIf_SetYieldStress,METH_O,"EPPHBallJointIf::SetYieldStress"},
	{NULL}
};
void EPPHBallJointIf_dealloc(PyObject* self)
{
}
PyObject* EPPHBallJointIf_str()
{
	return Py_BuildValue("s","This is EPPHBallJointIf.");
}
int EPPHBallJointIf_init(EPPHBallJointIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHBallJointIf* EPPHBallJointIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHBallJointIf *self;
	self = (EPPHBallJointIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHBallJointIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHBallJointIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHBallJointIf",/*tp_name*/
	sizeof(EPPHBallJointIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHBallJointIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHBallJointIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHBallJointIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHBallJointIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJointIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHBallJointIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHBallJointIf_new,/*tp_new*/
};
void initEPPHBallJointIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHBallJointIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHBallJointIfType);
	PyModule_AddObject(m,"PHBallJointIf",(PyObject *)&EPPHBallJointIfType);//モジュールに追加
}
PyObject* newEPPHBallJointIf(struct PHBallJointIf org)
{
	EPPHBallJointIf *ret = EPPHBallJointIf_new(&EPPHBallJointIfType,NULL,NULL);
	ret->ptr = new PHBallJointIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHBallJointIf(struct PHBallJointIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHBallJointIf *ret = EPPHBallJointIf_new(&EPPHBallJointIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHBallJointIf

//{*********EPPHBallJointDesc*******
static PyMethodDef EPPHBallJointDesc_methods[] =
{
	{NULL}
};
void EPPHBallJointDesc_dealloc(PyObject* self)
{
	delete ((EPPHBallJointDesc*)self)->ptr;
}
PyObject* EPPHBallJointDesc_str()
{
	return Py_BuildValue("s","This is EPPHBallJointDesc.");
}
int EPPHBallJointDesc_init(EPPHBallJointDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHBallJointDesc* EPPHBallJointDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHBallJointDesc *self;
	self = (EPPHBallJointDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHBallJointDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHBallJointDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHBallJointDesc",/*tp_name*/
	sizeof(EPPHBallJointDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHBallJointDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHBallJointDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHBallJointDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHBallJointDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJointDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHBallJointDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHBallJointDesc_new,/*tp_new*/
};
void initEPPHBallJointDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHBallJointDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHBallJointDescType);
	PyModule_AddObject(m,"PHBallJointDesc",(PyObject *)&EPPHBallJointDescType);//モジュールに追加
}
PyObject* newEPPHBallJointDesc(struct PHBallJointDesc org)
{
	EPPHBallJointDesc *ret = EPPHBallJointDesc_new(&EPPHBallJointDescType,NULL,NULL);
	ret->ptr = new PHBallJointDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHBallJointDesc(struct PHBallJointDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHBallJointDesc *ret = EPPHBallJointDesc_new(&EPPHBallJointDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHBallJointDesc

//{*********EPPHSpringIf*******
PyObject* EPPHSpringIf_GetDamper( EPPHSpringIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetDamper());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_GetDamperOri( EPPHSpringIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetDamperOri());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_GetSpring( EPPHSpringIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetSpring());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_GetSpringOri( EPPHSpringIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetSpringOri());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_SetDamper( EPPHSpringIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetDamper((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_SetDamperOri( EPPHSpringIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetDamperOri(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_SetSpring( EPPHSpringIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetSpring((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSpringIf_SetSpringOri( EPPHSpringIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetSpringOri(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSpringIf_methods[] =
{
	{"GetDamper",(PyCFunction)EPPHSpringIf_GetDamper,METH_NOARGS,"EPPHSpringIf::GetDamper"},
	{"GetDamperOri",(PyCFunction)EPPHSpringIf_GetDamperOri,METH_NOARGS,"EPPHSpringIf::GetDamperOri"},
	{"GetSpring",(PyCFunction)EPPHSpringIf_GetSpring,METH_NOARGS,"EPPHSpringIf::GetSpring"},
	{"GetSpringOri",(PyCFunction)EPPHSpringIf_GetSpringOri,METH_NOARGS,"EPPHSpringIf::GetSpringOri"},
	{"SetDamper",(PyCFunction)EPPHSpringIf_SetDamper,METH_O,"EPPHSpringIf::SetDamper"},
	{"SetDamperOri",(PyCFunction)EPPHSpringIf_SetDamperOri,METH_O,"EPPHSpringIf::SetDamperOri"},
	{"SetSpring",(PyCFunction)EPPHSpringIf_SetSpring,METH_O,"EPPHSpringIf::SetSpring"},
	{"SetSpringOri",(PyCFunction)EPPHSpringIf_SetSpringOri,METH_O,"EPPHSpringIf::SetSpringOri"},
	{NULL}
};
void EPPHSpringIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSpringIf_str()
{
	return Py_BuildValue("s","This is EPPHSpringIf.");
}
int EPPHSpringIf_init(EPPHSpringIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSpringIf* EPPHSpringIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSpringIf *self;
	self = (EPPHSpringIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSpringIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSpringIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSpringIf",/*tp_name*/
	sizeof(EPPHSpringIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSpringIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSpringIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSpringIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSpringIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJointIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSpringIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSpringIf_new,/*tp_new*/
};
void initEPPHSpringIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSpringIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSpringIfType);
	PyModule_AddObject(m,"PHSpringIf",(PyObject *)&EPPHSpringIfType);//モジュールに追加
}
PyObject* newEPPHSpringIf(struct PHSpringIf org)
{
	EPPHSpringIf *ret = EPPHSpringIf_new(&EPPHSpringIfType,NULL,NULL);
	ret->ptr = new PHSpringIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSpringIf(struct PHSpringIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSpringIf *ret = EPPHSpringIf_new(&EPPHSpringIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSpringIf

//{*********EPPHSpringDesc*******
static PyMethodDef EPPHSpringDesc_methods[] =
{
	{NULL}
};
void EPPHSpringDesc_dealloc(PyObject* self)
{
	delete ((EPPHSpringDesc*)self)->ptr;
}
PyObject* EPPHSpringDesc_str()
{
	return Py_BuildValue("s","This is EPPHSpringDesc.");
}
int EPPHSpringDesc_init(EPPHSpringDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSpringDesc* EPPHSpringDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSpringDesc *self;
	self = (EPPHSpringDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSpringDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSpringDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSpringDesc",/*tp_name*/
	sizeof(EPPHSpringDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSpringDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSpringDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSpringDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSpringDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHJointDescType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSpringDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSpringDesc_new,/*tp_new*/
};
void initEPPHSpringDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSpringDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSpringDescType);
	PyModule_AddObject(m,"PHSpringDesc",(PyObject *)&EPPHSpringDescType);//モジュールに追加
}
PyObject* newEPPHSpringDesc(struct PHSpringDesc org)
{
	EPPHSpringDesc *ret = EPPHSpringDesc_new(&EPPHSpringDescType,NULL,NULL);
	ret->ptr = new PHSpringDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSpringDesc(struct PHSpringDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSpringDesc *ret = EPPHSpringDesc_new(&EPPHSpringDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSpringDesc

//{*********EPPHTreeNodeIf*******
PyObject* EPPHTreeNodeIf_Enable( EPPHTreeNodeIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&PyBool_Check(((PyObject*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->Enable((Py_True == ((PyObject*)PyTuple_GetItem(tuple,0))));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		self->ptr->Enable();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHTreeNodeIf_GetChild( EPPHTreeNodeIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHTreeNodeIf* ret = (EPPHTreeNodeIf*)newEPPHTreeNodeIf(self->ptr->GetChild(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHTreeNodeIf_IsEnabled( EPPHTreeNodeIf* self )
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
PyObject* EPPHTreeNodeIf_NChildren( EPPHTreeNodeIf* self )
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
static PyMethodDef EPPHTreeNodeIf_methods[] =
{
	{"Enable",(PyCFunction)EPPHTreeNodeIf_Enable,METH_VARARGS,"EPPHTreeNodeIf::Enable"},
	{"GetChild",(PyCFunction)EPPHTreeNodeIf_GetChild,METH_O,"EPPHTreeNodeIf::GetChild"},
	{"IsEnabled",(PyCFunction)EPPHTreeNodeIf_IsEnabled,METH_NOARGS,"EPPHTreeNodeIf::IsEnabled"},
	{"NChildren",(PyCFunction)EPPHTreeNodeIf_NChildren,METH_NOARGS,"EPPHTreeNodeIf::NChildren"},
	{NULL}
};
void EPPHTreeNodeIf_dealloc(PyObject* self)
{
}
PyObject* EPPHTreeNodeIf_str()
{
	return Py_BuildValue("s","This is EPPHTreeNodeIf.");
}
int EPPHTreeNodeIf_init(EPPHTreeNodeIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHTreeNodeIf* EPPHTreeNodeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHTreeNodeIf *self;
	self = (EPPHTreeNodeIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHTreeNodeIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHTreeNodeIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHTreeNodeIf",/*tp_name*/
	sizeof(EPPHTreeNodeIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHTreeNodeIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHTreeNodeIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHTreeNodeIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHTreeNodeIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHTreeNodeIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHTreeNodeIf_new,/*tp_new*/
};
void initEPPHTreeNodeIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHTreeNodeIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHTreeNodeIfType);
	PyModule_AddObject(m,"PHTreeNodeIf",(PyObject *)&EPPHTreeNodeIfType);//モジュールに追加
}
PyObject* newEPPHTreeNodeIf(struct PHTreeNodeIf org)
{
	EPPHTreeNodeIf *ret = EPPHTreeNodeIf_new(&EPPHTreeNodeIfType,NULL,NULL);
	ret->ptr = new PHTreeNodeIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHTreeNodeIf(struct PHTreeNodeIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHTreeNodeIf *ret = EPPHTreeNodeIf_new(&EPPHTreeNodeIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHTreeNodeIf

//{*********EPPHRootNodeIf*******
static PyMethodDef EPPHRootNodeIf_methods[] =
{
	{NULL}
};
void EPPHRootNodeIf_dealloc(PyObject* self)
{
}
PyObject* EPPHRootNodeIf_str()
{
	return Py_BuildValue("s","This is EPPHRootNodeIf.");
}
int EPPHRootNodeIf_init(EPPHRootNodeIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHRootNodeIf* EPPHRootNodeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHRootNodeIf *self;
	self = (EPPHRootNodeIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHRootNodeIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHRootNodeIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHRootNodeIf",/*tp_name*/
	sizeof(EPPHRootNodeIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHRootNodeIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHRootNodeIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHRootNodeIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHRootNodeIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNodeIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHRootNodeIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHRootNodeIf_new,/*tp_new*/
};
void initEPPHRootNodeIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHRootNodeIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHRootNodeIfType);
	PyModule_AddObject(m,"PHRootNodeIf",(PyObject *)&EPPHRootNodeIfType);//モジュールに追加
}
PyObject* newEPPHRootNodeIf(struct PHRootNodeIf org)
{
	EPPHRootNodeIf *ret = EPPHRootNodeIf_new(&EPPHRootNodeIfType,NULL,NULL);
	ret->ptr = new PHRootNodeIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHRootNodeIf(struct PHRootNodeIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHRootNodeIf *ret = EPPHRootNodeIf_new(&EPPHRootNodeIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHRootNodeIf

//{*********EPPHTreeNode1DIf*******
static PyMethodDef EPPHTreeNode1DIf_methods[] =
{
	{NULL}
};
void EPPHTreeNode1DIf_dealloc(PyObject* self)
{
}
PyObject* EPPHTreeNode1DIf_str()
{
	return Py_BuildValue("s","This is EPPHTreeNode1DIf.");
}
int EPPHTreeNode1DIf_init(EPPHTreeNode1DIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHTreeNode1DIf* EPPHTreeNode1DIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHTreeNode1DIf *self;
	self = (EPPHTreeNode1DIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHTreeNode1DIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHTreeNode1DIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHTreeNode1DIf",/*tp_name*/
	sizeof(EPPHTreeNode1DIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHTreeNode1DIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHTreeNode1DIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHTreeNode1DIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHTreeNode1DIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNodeIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHTreeNode1DIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHTreeNode1DIf_new,/*tp_new*/
};
void initEPPHTreeNode1DIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHTreeNode1DIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHTreeNode1DIfType);
	PyModule_AddObject(m,"PHTreeNode1DIf",(PyObject *)&EPPHTreeNode1DIfType);//モジュールに追加
}
PyObject* newEPPHTreeNode1DIf(struct PHTreeNode1DIf org)
{
	EPPHTreeNode1DIf *ret = EPPHTreeNode1DIf_new(&EPPHTreeNode1DIfType,NULL,NULL);
	ret->ptr = new PHTreeNode1DIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHTreeNode1DIf(struct PHTreeNode1DIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHTreeNode1DIf *ret = EPPHTreeNode1DIf_new(&EPPHTreeNode1DIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHTreeNode1DIf

//{*********EPPHHingeJointNodeIf*******
static PyMethodDef EPPHHingeJointNodeIf_methods[] =
{
	{NULL}
};
void EPPHHingeJointNodeIf_dealloc(PyObject* self)
{
}
PyObject* EPPHHingeJointNodeIf_str()
{
	return Py_BuildValue("s","This is EPPHHingeJointNodeIf.");
}
int EPPHHingeJointNodeIf_init(EPPHHingeJointNodeIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHHingeJointNodeIf* EPPHHingeJointNodeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHHingeJointNodeIf *self;
	self = (EPPHHingeJointNodeIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHHingeJointNodeIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHHingeJointNodeIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHHingeJointNodeIf",/*tp_name*/
	sizeof(EPPHHingeJointNodeIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHHingeJointNodeIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHHingeJointNodeIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHHingeJointNodeIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHHingeJointNodeIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNode1DIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHHingeJointNodeIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHHingeJointNodeIf_new,/*tp_new*/
};
void initEPPHHingeJointNodeIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHHingeJointNodeIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHHingeJointNodeIfType);
	PyModule_AddObject(m,"PHHingeJointNodeIf",(PyObject *)&EPPHHingeJointNodeIfType);//モジュールに追加
}
PyObject* newEPPHHingeJointNodeIf(struct PHHingeJointNodeIf org)
{
	EPPHHingeJointNodeIf *ret = EPPHHingeJointNodeIf_new(&EPPHHingeJointNodeIfType,NULL,NULL);
	ret->ptr = new PHHingeJointNodeIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHHingeJointNodeIf(struct PHHingeJointNodeIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHHingeJointNodeIf *ret = EPPHHingeJointNodeIf_new(&EPPHHingeJointNodeIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHHingeJointNodeIf

//{*********EPPHSliderJointNodeIf*******
static PyMethodDef EPPHSliderJointNodeIf_methods[] =
{
	{NULL}
};
void EPPHSliderJointNodeIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSliderJointNodeIf_str()
{
	return Py_BuildValue("s","This is EPPHSliderJointNodeIf.");
}
int EPPHSliderJointNodeIf_init(EPPHSliderJointNodeIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSliderJointNodeIf* EPPHSliderJointNodeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSliderJointNodeIf *self;
	self = (EPPHSliderJointNodeIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSliderJointNodeIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSliderJointNodeIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSliderJointNodeIf",/*tp_name*/
	sizeof(EPPHSliderJointNodeIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSliderJointNodeIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSliderJointNodeIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSliderJointNodeIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSliderJointNodeIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNode1DIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSliderJointNodeIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSliderJointNodeIf_new,/*tp_new*/
};
void initEPPHSliderJointNodeIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSliderJointNodeIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSliderJointNodeIfType);
	PyModule_AddObject(m,"PHSliderJointNodeIf",(PyObject *)&EPPHSliderJointNodeIfType);//モジュールに追加
}
PyObject* newEPPHSliderJointNodeIf(struct PHSliderJointNodeIf org)
{
	EPPHSliderJointNodeIf *ret = EPPHSliderJointNodeIf_new(&EPPHSliderJointNodeIfType,NULL,NULL);
	ret->ptr = new PHSliderJointNodeIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSliderJointNodeIf(struct PHSliderJointNodeIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSliderJointNodeIf *ret = EPPHSliderJointNodeIf_new(&EPPHSliderJointNodeIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSliderJointNodeIf

//{*********EPPHPathJointNodeIf*******
static PyMethodDef EPPHPathJointNodeIf_methods[] =
{
	{NULL}
};
void EPPHPathJointNodeIf_dealloc(PyObject* self)
{
}
PyObject* EPPHPathJointNodeIf_str()
{
	return Py_BuildValue("s","This is EPPHPathJointNodeIf.");
}
int EPPHPathJointNodeIf_init(EPPHPathJointNodeIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHPathJointNodeIf* EPPHPathJointNodeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHPathJointNodeIf *self;
	self = (EPPHPathJointNodeIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHPathJointNodeIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHPathJointNodeIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHPathJointNodeIf",/*tp_name*/
	sizeof(EPPHPathJointNodeIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHPathJointNodeIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHPathJointNodeIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHPathJointNodeIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHPathJointNodeIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNode1DIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHPathJointNodeIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHPathJointNodeIf_new,/*tp_new*/
};
void initEPPHPathJointNodeIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHPathJointNodeIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHPathJointNodeIfType);
	PyModule_AddObject(m,"PHPathJointNodeIf",(PyObject *)&EPPHPathJointNodeIfType);//モジュールに追加
}
PyObject* newEPPHPathJointNodeIf(struct PHPathJointNodeIf org)
{
	EPPHPathJointNodeIf *ret = EPPHPathJointNodeIf_new(&EPPHPathJointNodeIfType,NULL,NULL);
	ret->ptr = new PHPathJointNodeIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHPathJointNodeIf(struct PHPathJointNodeIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHPathJointNodeIf *ret = EPPHPathJointNodeIf_new(&EPPHPathJointNodeIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHPathJointNodeIf

//{*********EPPHBallJointNodeIf*******
static PyMethodDef EPPHBallJointNodeIf_methods[] =
{
	{NULL}
};
void EPPHBallJointNodeIf_dealloc(PyObject* self)
{
}
PyObject* EPPHBallJointNodeIf_str()
{
	return Py_BuildValue("s","This is EPPHBallJointNodeIf.");
}
int EPPHBallJointNodeIf_init(EPPHBallJointNodeIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHBallJointNodeIf* EPPHBallJointNodeIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHBallJointNodeIf *self;
	self = (EPPHBallJointNodeIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHBallJointNodeIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHBallJointNodeIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHBallJointNodeIf",/*tp_name*/
	sizeof(EPPHBallJointNodeIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHBallJointNodeIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHBallJointNodeIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHBallJointNodeIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHBallJointNodeIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHTreeNodeIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHBallJointNodeIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHBallJointNodeIf_new,/*tp_new*/
};
void initEPPHBallJointNodeIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHBallJointNodeIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHBallJointNodeIfType);
	PyModule_AddObject(m,"PHBallJointNodeIf",(PyObject *)&EPPHBallJointNodeIfType);//モジュールに追加
}
PyObject* newEPPHBallJointNodeIf(struct PHBallJointNodeIf org)
{
	EPPHBallJointNodeIf *ret = EPPHBallJointNodeIf_new(&EPPHBallJointNodeIfType,NULL,NULL);
	ret->ptr = new PHBallJointNodeIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHBallJointNodeIf(struct PHBallJointNodeIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHBallJointNodeIf *ret = EPPHBallJointNodeIf_new(&EPPHBallJointNodeIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHBallJointNodeIf

//{*********EPPHGearIf*******
static PyMethodDef EPPHGearIf_methods[] =
{
	{NULL}
};
void EPPHGearIf_dealloc(PyObject* self)
{
}
PyObject* EPPHGearIf_str()
{
	return Py_BuildValue("s","This is EPPHGearIf.");
}
int EPPHGearIf_init(EPPHGearIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHGearIf* EPPHGearIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHGearIf *self;
	self = (EPPHGearIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHGearIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHGearIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHGearIf",/*tp_name*/
	sizeof(EPPHGearIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHGearIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHGearIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHGearIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHGearIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHGearIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHGearIf_new,/*tp_new*/
};
void initEPPHGearIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHGearIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHGearIfType);
	PyModule_AddObject(m,"PHGearIf",(PyObject *)&EPPHGearIfType);//モジュールに追加
}
PyObject* newEPPHGearIf(struct PHGearIf org)
{
	EPPHGearIf *ret = EPPHGearIf_new(&EPPHGearIfType,NULL,NULL);
	ret->ptr = new PHGearIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHGearIf(struct PHGearIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHGearIf *ret = EPPHGearIf_new(&EPPHGearIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHGearIf

//{*********EPPHInactiveSolids*******
static PyMethodDef EPPHInactiveSolids_methods[] =
{
	{NULL}
};
void EPPHInactiveSolids_dealloc(PyObject* self)
{
	delete ((EPPHInactiveSolids*)self)->ptr;
}
PyObject* EPPHInactiveSolids_str()
{
	return Py_BuildValue("s","This is EPPHInactiveSolids.");
}
int EPPHInactiveSolids_init(EPPHInactiveSolids* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHInactiveSolids* EPPHInactiveSolids_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHInactiveSolids *self;
	self = (EPPHInactiveSolids*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHInactiveSolids_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHInactiveSolidsType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHInactiveSolids",/*tp_name*/
	sizeof(EPPHInactiveSolids),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHInactiveSolids_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHInactiveSolids_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHInactiveSolids",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHInactiveSolids_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHInactiveSolids_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHInactiveSolids_new,/*tp_new*/
};
void initEPPHInactiveSolids(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHInactiveSolidsType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHInactiveSolidsType);
	PyModule_AddObject(m,"PHInactiveSolids",(PyObject *)&EPPHInactiveSolidsType);//モジュールに追加
}
PyObject* newEPPHInactiveSolids(struct PHInactiveSolids org)
{
	EPPHInactiveSolids *ret = EPPHInactiveSolids_new(&EPPHInactiveSolidsType,NULL,NULL);
	ret->ptr = new PHInactiveSolids();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHInactiveSolids(struct PHInactiveSolids* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHInactiveSolids *ret = EPPHInactiveSolids_new(&EPPHInactiveSolidsType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHInactiveSolids

//{*********EPPHRaycastHit*******
static PyMethodDef EPPHRaycastHit_methods[] =
{
	{NULL}
};
void EPPHRaycastHit_dealloc(PyObject* self)
{
	delete ((EPPHRaycastHit*)self)->ptr;
}
PyObject* EPPHRaycastHit_str()
{
	return Py_BuildValue("s","This is EPPHRaycastHit.");
}
int EPPHRaycastHit_init(EPPHRaycastHit* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHRaycastHit* EPPHRaycastHit_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHRaycastHit *self;
	self = (EPPHRaycastHit*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHRaycastHit_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHRaycastHitType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHRaycastHit",/*tp_name*/
	sizeof(EPPHRaycastHit),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHRaycastHit_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHRaycastHit_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHRaycastHit",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHRaycastHit_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHRaycastHit_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHRaycastHit_new,/*tp_new*/
};
void initEPPHRaycastHit(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHRaycastHitType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHRaycastHitType);
	PyModule_AddObject(m,"PHRaycastHit",(PyObject *)&EPPHRaycastHitType);//モジュールに追加
}
PyObject* newEPPHRaycastHit(struct PHRaycastHit org)
{
	EPPHRaycastHit *ret = EPPHRaycastHit_new(&EPPHRaycastHitType,NULL,NULL);
	ret->ptr = new PHRaycastHit();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHRaycastHit(struct PHRaycastHit* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHRaycastHit *ret = EPPHRaycastHit_new(&EPPHRaycastHitType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHRaycastHit

//{*********EPPHRayDesc*******
static PyMethodDef EPPHRayDesc_methods[] =
{
	{NULL}
};
void EPPHRayDesc_dealloc(PyObject* self)
{
	delete ((EPPHRayDesc*)self)->ptr;
}
PyObject* EPPHRayDesc_str()
{
	return Py_BuildValue("s","This is EPPHRayDesc.");
}
int EPPHRayDesc_init(EPPHRayDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHRayDesc* EPPHRayDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHRayDesc *self;
	self = (EPPHRayDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHRayDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHRayDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHRayDesc",/*tp_name*/
	sizeof(EPPHRayDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHRayDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHRayDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHRayDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHRayDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHRayDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHRayDesc_new,/*tp_new*/
};
void initEPPHRayDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHRayDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHRayDescType);
	PyModule_AddObject(m,"PHRayDesc",(PyObject *)&EPPHRayDescType);//モジュールに追加
}
PyObject* newEPPHRayDesc(struct PHRayDesc org)
{
	EPPHRayDesc *ret = EPPHRayDesc_new(&EPPHRayDescType,NULL,NULL);
	ret->ptr = new PHRayDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHRayDesc(struct PHRayDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHRayDesc *ret = EPPHRayDesc_new(&EPPHRayDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHRayDesc

//{*********EPPHRayIf*******
PyObject* EPPHRayIf_Apply( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Apply();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_GetDirection( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetDirection());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_GetDynamicalNearest( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHRaycastHit* ret = (EPPHRaycastHit*)newEPPHRaycastHit(self->ptr->GetDynamicalNearest());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_GetHits( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHRaycastHit* ret = (EPPHRaycastHit*)newEPPHRaycastHit(self->ptr->GetHits());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_GetNearest( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHRaycastHit* ret = (EPPHRaycastHit*)newEPPHRaycastHit(self->ptr->GetNearest());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_GetOrigin( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetOrigin());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_NHits( EPPHRayIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NHits());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_SetDirection( EPPHRayIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetDirection((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHRayIf_SetOrigin( EPPHRayIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetOrigin((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHRayIf_methods[] =
{
	{"Apply",(PyCFunction)EPPHRayIf_Apply,METH_NOARGS,"EPPHRayIf::Apply"},
	{"GetDirection",(PyCFunction)EPPHRayIf_GetDirection,METH_NOARGS,"EPPHRayIf::GetDirection"},
	{"GetDynamicalNearest",(PyCFunction)EPPHRayIf_GetDynamicalNearest,METH_NOARGS,"EPPHRayIf::GetDynamicalNearest"},
	{"GetHits",(PyCFunction)EPPHRayIf_GetHits,METH_NOARGS,"EPPHRayIf::GetHits"},
	{"GetNearest",(PyCFunction)EPPHRayIf_GetNearest,METH_NOARGS,"EPPHRayIf::GetNearest"},
	{"GetOrigin",(PyCFunction)EPPHRayIf_GetOrigin,METH_NOARGS,"EPPHRayIf::GetOrigin"},
	{"NHits",(PyCFunction)EPPHRayIf_NHits,METH_NOARGS,"EPPHRayIf::NHits"},
	{"SetDirection",(PyCFunction)EPPHRayIf_SetDirection,METH_O,"EPPHRayIf::SetDirection"},
	{"SetOrigin",(PyCFunction)EPPHRayIf_SetOrigin,METH_O,"EPPHRayIf::SetOrigin"},
	{NULL}
};
void EPPHRayIf_dealloc(PyObject* self)
{
}
PyObject* EPPHRayIf_str()
{
	return Py_BuildValue("s","This is EPPHRayIf.");
}
int EPPHRayIf_init(EPPHRayIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHRayIf* EPPHRayIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHRayIf *self;
	self = (EPPHRayIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHRayIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHRayIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHRayIf",/*tp_name*/
	sizeof(EPPHRayIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHRayIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHRayIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHRayIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHRayIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHRayIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHRayIf_new,/*tp_new*/
};
void initEPPHRayIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHRayIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHRayIfType);
	PyModule_AddObject(m,"PHRayIf",(PyObject *)&EPPHRayIfType);//モジュールに追加
}
PyObject* newEPPHRayIf(struct PHRayIf org)
{
	EPPHRayIf *ret = EPPHRayIf_new(&EPPHRayIfType,NULL,NULL);
	ret->ptr = new PHRayIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHRayIf(struct PHRayIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHRayIf *ret = EPPHRayIf_new(&EPPHRayIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHRayIf

//{*********EPPHSceneState*******
PyObject* EPPHSceneState_Init( EPPHSceneState* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Init();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSceneState_methods[] =
{
	{"Init",(PyCFunction)EPPHSceneState_Init,METH_NOARGS,"EPPHSceneState::Init"},
	{NULL}
};
void EPPHSceneState_dealloc(PyObject* self)
{
	delete ((EPPHSceneState*)self)->ptr;
}
PyObject* EPPHSceneState_str()
{
	return Py_BuildValue("s","This is EPPHSceneState.");
}
int EPPHSceneState_init(EPPHSceneState* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSceneState* EPPHSceneState_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSceneState *self;
	self = (EPPHSceneState*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSceneState_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSceneStateType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSceneState",/*tp_name*/
	sizeof(EPPHSceneState),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSceneState_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSceneState_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSceneState",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSceneState_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSceneState_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSceneState_new,/*tp_new*/
};
void initEPPHSceneState(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSceneStateType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSceneStateType);
	PyModule_AddObject(m,"PHSceneState",(PyObject *)&EPPHSceneStateType);//モジュールに追加
}
PyObject* newEPPHSceneState(struct PHSceneState org)
{
	EPPHSceneState *ret = EPPHSceneState_new(&EPPHSceneStateType,NULL,NULL);
	ret->ptr = new PHSceneState();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSceneState(struct PHSceneState* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSceneState *ret = EPPHSceneState_new(&EPPHSceneStateType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSceneState

//{*********EPPHSceneDesc*******
PyObject* EPPHSceneDesc_Init( EPPHSceneDesc* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Init();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSceneDesc_methods[] =
{
	{"Init",(PyCFunction)EPPHSceneDesc_Init,METH_NOARGS,"EPPHSceneDesc::Init"},
	{NULL}
};
void EPPHSceneDesc_dealloc(PyObject* self)
{
	delete ((EPPHSceneDesc*)self)->ptr;
}
PyObject* EPPHSceneDesc_str()
{
	return Py_BuildValue("s","This is EPPHSceneDesc.");
}
int EPPHSceneDesc_init(EPPHSceneDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSceneDesc* EPPHSceneDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSceneDesc *self;
	self = (EPPHSceneDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSceneDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSceneDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSceneDesc",/*tp_name*/
	sizeof(EPPHSceneDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSceneDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSceneDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSceneDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSceneDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHSceneStateType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSceneDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSceneDesc_new,/*tp_new*/
};
void initEPPHSceneDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSceneDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSceneDescType);
	PyModule_AddObject(m,"PHSceneDesc",(PyObject *)&EPPHSceneDescType);//モジュールに追加
}
PyObject* newEPPHSceneDesc(struct PHSceneDesc org)
{
	EPPHSceneDesc *ret = EPPHSceneDesc_new(&EPPHSceneDescType,NULL,NULL);
	ret->ptr = new PHSceneDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSceneDesc(struct PHSceneDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSceneDesc *ret = EPPHSceneDesc_new(&EPPHSceneDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSceneDesc

//{*********EPPHSceneIf*******
PyObject* EPPHSceneIf_Clear( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Clear();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_ClearForce( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ClearForce();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreateGear( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPPHJoint1DIf_Check(((EPPHJoint1DIf*)PyTuple_GetItem(tuple,0))) && EPPHJoint1DIf_Check(((EPPHJoint1DIf*)PyTuple_GetItem(tuple,1))) && EPPHGearDesc_Check(((EPPHGearDesc*)PyTuple_GetItem(tuple,2))))
	{
		EPPHGearIf* ret = (EPPHGearIf*)newEPPHGearIf(self->ptr->CreateGear((((EPPHJoint1DIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHJoint1DIf*)PyTuple_GetItem(tuple,1))->ptr),(*(((EPPHGearDesc*)PyTuple_GetItem(tuple,2))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPPHJoint1DIf_Check(((EPPHJoint1DIf*)PyTuple_GetItem(tuple,0))) && EPPHJoint1DIf_Check(((EPPHJoint1DIf*)PyTuple_GetItem(tuple,1))))
	{
		EPPHGearIf* ret = (EPPHGearIf*)newEPPHGearIf(self->ptr->CreateGear((((EPPHJoint1DIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHJoint1DIf*)PyTuple_GetItem(tuple,1))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreateJoint( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 4&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))) && EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))) && EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,2))) && EPPHJointDesc_Check(((EPPHJointDesc*)PyTuple_GetItem(tuple,3))))
	{
		EPPHJointIf* ret = (EPPHJointIf*)newEPPHJointIf(self->ptr->CreateJoint((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))->ptr),(((EPIfInfo*)PyTuple_GetItem(tuple,2))->ptr),(*(((EPPHJointDesc*)PyTuple_GetItem(tuple,3))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreatePath( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPPHPathDesc_Check(((EPPHPathDesc*)PyTuple_GetItem(tuple,0))))
	{
		EPPHPathIf* ret = (EPPHPathIf*)newEPPHPathIf(self->ptr->CreatePath((*(((EPPHPathDesc*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPPHPathIf* ret = (EPPHPathIf*)newEPPHPathIf(self->ptr->CreatePath());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreateRay( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPPHRayDesc_Check(((EPPHRayDesc*)PyTuple_GetItem(tuple,0))))
	{
		EPPHRayIf* ret = (EPPHRayIf*)newEPPHRayIf(self->ptr->CreateRay((*(((EPPHRayDesc*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPPHRayIf* ret = (EPPHRayIf*)newEPPHRayIf(self->ptr->CreateRay());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreateRootNode( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))) && EPPHRootNodeDesc_Check(((EPPHRootNodeDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPPHRootNodeIf* ret = (EPPHRootNodeIf*)newEPPHRootNodeIf(self->ptr->CreateRootNode((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPPHRootNodeDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,0))))
	{
		EPPHRootNodeIf* ret = (EPPHRootNodeIf*)newEPPHRootNodeIf(self->ptr->CreateRootNode((((EPPHSolidIf*)PyTuple_GetItem(tuple,0))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreateSolid( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPPHSolidDesc_Check(((EPPHSolidDesc*)PyTuple_GetItem(tuple,0))))
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->CreateSolid((*(((EPPHSolidDesc*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->CreateSolid());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_CreateTreeNode( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 3&&EPPHTreeNodeIf_Check(((EPPHTreeNodeIf*)PyTuple_GetItem(tuple,0))) && EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))) && EPPHTreeNodeDesc_Check(((EPPHTreeNodeDesc*)PyTuple_GetItem(tuple,2))))
	{
		EPPHTreeNodeIf* ret = (EPPHTreeNodeIf*)newEPPHTreeNodeIf(self->ptr->CreateTreeNode((((EPPHTreeNodeIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))->ptr),(*(((EPPHTreeNodeDesc*)PyTuple_GetItem(tuple,2))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPPHTreeNodeIf_Check(((EPPHTreeNodeIf*)PyTuple_GetItem(tuple,0))) && EPPHSolidIf_Check(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))))
	{
		EPPHTreeNodeIf* ret = (EPPHTreeNodeIf*)newEPPHTreeNodeIf(self->ptr->CreateTreeNode((((EPPHTreeNodeIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHSolidIf*)PyTuple_GetItem(tuple,1))->ptr)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GenerateForce( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->GenerateForce();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetAirResistanceRate( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetAirResistanceRate());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetConstraintEngine( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHConstraintEngineIf* ret = (EPPHConstraintEngineIf*)newEPPHConstraintEngineIf(self->ptr->GetConstraintEngine());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetContact( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHContactPointIf* ret = (EPPHContactPointIf*)newEPPHContactPointIf(self->ptr->GetContact(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetCount( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPunsignedint* ret = (EPunsignedint*)newEPunsignedint(self->ptr->GetCount());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetEngine( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHEngineIf* ret = (EPPHEngineIf*)newEPPHEngineIf(self->ptr->GetEngine(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetGear( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHGearIf* ret = (EPPHGearIf*)newEPPHGearIf(self->ptr->GetGear(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetGravity( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetGravity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetGravityEngine( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHGravityEngineIf* ret = (EPPHGravityEngineIf*)newEPPHGravityEngineIf(self->ptr->GetGravityEngine());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetIKActuator( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHIKActuatorIf* ret = (EPPHIKActuatorIf*)newEPPHIKActuatorIf(self->ptr->GetIKActuator(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetIKEndEffector( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHIKEndEffectorIf* ret = (EPPHIKEndEffectorIf*)newEPPHIKEndEffectorIf(self->ptr->GetIKEndEffector(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetIKEngine( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHIKEngineIf* ret = (EPPHIKEngineIf*)newEPPHIKEngineIf(self->ptr->GetIKEngine());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetJoint( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHConstraintIf* ret = (EPPHConstraintIf*)newEPPHConstraintIf(self->ptr->GetJoint(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetNumIteration( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetNumIteration());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetPath( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHPathIf* ret = (EPPHPathIf*)newEPPHPathIf(self->ptr->GetPath(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetPenaltyEngine( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHPenaltyEngineIf* ret = (EPPHPenaltyEngineIf*)newEPPHPenaltyEngineIf(self->ptr->GetPenaltyEngine());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetRay( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHRayIf* ret = (EPPHRayIf*)newEPPHRayIf(self->ptr->GetRay(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetRootNode( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHRootNodeIf* ret = (EPPHRootNodeIf*)newEPPHRootNodeIf(self->ptr->GetRootNode(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetSdk( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSdkIf* ret = (EPPHSdkIf*)newEPPHSdkIf(self->ptr->GetSdk());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetSolidPair( EPPHSceneIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPPHSolidPairForLCPIf* ret = (EPPHSolidPairForLCPIf*)newEPPHSolidPairForLCPIf(self->ptr->GetSolidPair(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetTimeStep( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTimeStep());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_GetTimeStepInv( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetTimeStepInv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_Integrate( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Integrate();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_IntegratePart1( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->IntegratePart1();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_IntegratePart2( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->IntegratePart2();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NContacts( EPPHSceneIf* self )
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
PyObject* EPPHSceneIf_NEngines( EPPHSceneIf* self )
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
PyObject* EPPHSceneIf_NGears( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NGears());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NIKActuators( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NIKActuators());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NIKEndEffectors( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NIKEndEffectors());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NJoints( EPPHSceneIf* self )
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
PyObject* EPPHSceneIf_NPaths( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NPaths());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NRays( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NRays());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NRootNodes( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NRootNodes());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NSolidPairs( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NSolidPairs());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_NSolids( EPPHSceneIf* self )
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
PyObject* EPPHSceneIf_SetAirResistanceRate( EPPHSceneIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetAirResistanceRate(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_SetCount( EPPHSceneIf* self,EPunsignedint* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPunsignedint_Check(var1))
	{
		self->ptr->SetCount((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_SetGravity( EPPHSceneIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetGravity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_SetNumIteration( EPPHSceneIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		self->ptr->SetNumIteration(PyLong_AsLong((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_SetStateMode( EPPHSceneIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetStateMode((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_SetTimeStep( EPPHSceneIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetTimeStep(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSceneIf_Step( EPPHSceneIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Step();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSceneIf_methods[] =
{
	{"Clear",(PyCFunction)EPPHSceneIf_Clear,METH_NOARGS,"EPPHSceneIf::Clear"},
	{"ClearForce",(PyCFunction)EPPHSceneIf_ClearForce,METH_NOARGS,"EPPHSceneIf::ClearForce"},
	{"CreateGear",(PyCFunction)EPPHSceneIf_CreateGear,METH_VARARGS,"EPPHSceneIf::CreateGear"},
	{"CreateJoint",(PyCFunction)EPPHSceneIf_CreateJoint,METH_VARARGS,"EPPHSceneIf::CreateJoint"},
	{"CreatePath",(PyCFunction)EPPHSceneIf_CreatePath,METH_VARARGS,"EPPHSceneIf::CreatePath"},
	{"CreateRay",(PyCFunction)EPPHSceneIf_CreateRay,METH_VARARGS,"EPPHSceneIf::CreateRay"},
	{"CreateRootNode",(PyCFunction)EPPHSceneIf_CreateRootNode,METH_VARARGS,"EPPHSceneIf::CreateRootNode"},
	{"CreateSolid",(PyCFunction)EPPHSceneIf_CreateSolid,METH_VARARGS,"EPPHSceneIf::CreateSolid"},
	{"CreateTreeNode",(PyCFunction)EPPHSceneIf_CreateTreeNode,METH_VARARGS,"EPPHSceneIf::CreateTreeNode"},
	{"GenerateForce",(PyCFunction)EPPHSceneIf_GenerateForce,METH_NOARGS,"EPPHSceneIf::GenerateForce"},
	{"GetAirResistanceRate",(PyCFunction)EPPHSceneIf_GetAirResistanceRate,METH_NOARGS,"EPPHSceneIf::GetAirResistanceRate"},
	{"GetConstraintEngine",(PyCFunction)EPPHSceneIf_GetConstraintEngine,METH_NOARGS,"EPPHSceneIf::GetConstraintEngine"},
	{"GetContact",(PyCFunction)EPPHSceneIf_GetContact,METH_O,"EPPHSceneIf::GetContact"},
	{"GetCount",(PyCFunction)EPPHSceneIf_GetCount,METH_NOARGS,"EPPHSceneIf::GetCount"},
	{"GetEngine",(PyCFunction)EPPHSceneIf_GetEngine,METH_O,"EPPHSceneIf::GetEngine"},
	{"GetGear",(PyCFunction)EPPHSceneIf_GetGear,METH_O,"EPPHSceneIf::GetGear"},
	{"GetGravity",(PyCFunction)EPPHSceneIf_GetGravity,METH_NOARGS,"EPPHSceneIf::GetGravity"},
	{"GetGravityEngine",(PyCFunction)EPPHSceneIf_GetGravityEngine,METH_NOARGS,"EPPHSceneIf::GetGravityEngine"},
	{"GetIKActuator",(PyCFunction)EPPHSceneIf_GetIKActuator,METH_O,"EPPHSceneIf::GetIKActuator"},
	{"GetIKEndEffector",(PyCFunction)EPPHSceneIf_GetIKEndEffector,METH_O,"EPPHSceneIf::GetIKEndEffector"},
	{"GetIKEngine",(PyCFunction)EPPHSceneIf_GetIKEngine,METH_NOARGS,"EPPHSceneIf::GetIKEngine"},
	{"GetJoint",(PyCFunction)EPPHSceneIf_GetJoint,METH_O,"EPPHSceneIf::GetJoint"},
	{"GetNumIteration",(PyCFunction)EPPHSceneIf_GetNumIteration,METH_NOARGS,"EPPHSceneIf::GetNumIteration"},
	{"GetPath",(PyCFunction)EPPHSceneIf_GetPath,METH_O,"EPPHSceneIf::GetPath"},
	{"GetPenaltyEngine",(PyCFunction)EPPHSceneIf_GetPenaltyEngine,METH_NOARGS,"EPPHSceneIf::GetPenaltyEngine"},
	{"GetRay",(PyCFunction)EPPHSceneIf_GetRay,METH_O,"EPPHSceneIf::GetRay"},
	{"GetRootNode",(PyCFunction)EPPHSceneIf_GetRootNode,METH_O,"EPPHSceneIf::GetRootNode"},
	{"GetSdk",(PyCFunction)EPPHSceneIf_GetSdk,METH_NOARGS,"EPPHSceneIf::GetSdk"},
	{"GetSolidPair",(PyCFunction)EPPHSceneIf_GetSolidPair,METH_VARARGS,"EPPHSceneIf::GetSolidPair"},
	{"GetTimeStep",(PyCFunction)EPPHSceneIf_GetTimeStep,METH_NOARGS,"EPPHSceneIf::GetTimeStep"},
	{"GetTimeStepInv",(PyCFunction)EPPHSceneIf_GetTimeStepInv,METH_NOARGS,"EPPHSceneIf::GetTimeStepInv"},
	{"Integrate",(PyCFunction)EPPHSceneIf_Integrate,METH_NOARGS,"EPPHSceneIf::Integrate"},
	{"IntegratePart1",(PyCFunction)EPPHSceneIf_IntegratePart1,METH_NOARGS,"EPPHSceneIf::IntegratePart1"},
	{"IntegratePart2",(PyCFunction)EPPHSceneIf_IntegratePart2,METH_NOARGS,"EPPHSceneIf::IntegratePart2"},
	{"NContacts",(PyCFunction)EPPHSceneIf_NContacts,METH_NOARGS,"EPPHSceneIf::NContacts"},
	{"NEngines",(PyCFunction)EPPHSceneIf_NEngines,METH_NOARGS,"EPPHSceneIf::NEngines"},
	{"NGears",(PyCFunction)EPPHSceneIf_NGears,METH_NOARGS,"EPPHSceneIf::NGears"},
	{"NIKActuators",(PyCFunction)EPPHSceneIf_NIKActuators,METH_NOARGS,"EPPHSceneIf::NIKActuators"},
	{"NIKEndEffectors",(PyCFunction)EPPHSceneIf_NIKEndEffectors,METH_NOARGS,"EPPHSceneIf::NIKEndEffectors"},
	{"NJoints",(PyCFunction)EPPHSceneIf_NJoints,METH_NOARGS,"EPPHSceneIf::NJoints"},
	{"NPaths",(PyCFunction)EPPHSceneIf_NPaths,METH_NOARGS,"EPPHSceneIf::NPaths"},
	{"NRays",(PyCFunction)EPPHSceneIf_NRays,METH_NOARGS,"EPPHSceneIf::NRays"},
	{"NRootNodes",(PyCFunction)EPPHSceneIf_NRootNodes,METH_NOARGS,"EPPHSceneIf::NRootNodes"},
	{"NSolidPairs",(PyCFunction)EPPHSceneIf_NSolidPairs,METH_NOARGS,"EPPHSceneIf::NSolidPairs"},
	{"NSolids",(PyCFunction)EPPHSceneIf_NSolids,METH_NOARGS,"EPPHSceneIf::NSolids"},
	{"SetAirResistanceRate",(PyCFunction)EPPHSceneIf_SetAirResistanceRate,METH_O,"EPPHSceneIf::SetAirResistanceRate"},
	{"SetCount",(PyCFunction)EPPHSceneIf_SetCount,METH_O,"EPPHSceneIf::SetCount"},
	{"SetGravity",(PyCFunction)EPPHSceneIf_SetGravity,METH_O,"EPPHSceneIf::SetGravity"},
	{"SetNumIteration",(PyCFunction)EPPHSceneIf_SetNumIteration,METH_O,"EPPHSceneIf::SetNumIteration"},
	{"SetStateMode",(PyCFunction)EPPHSceneIf_SetStateMode,METH_O,"EPPHSceneIf::SetStateMode"},
	{"SetTimeStep",(PyCFunction)EPPHSceneIf_SetTimeStep,METH_O,"EPPHSceneIf::SetTimeStep"},
	{"Step",(PyCFunction)EPPHSceneIf_Step,METH_NOARGS,"EPPHSceneIf::Step"},
	{NULL}
};
void EPPHSceneIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSceneIf_str()
{
	return Py_BuildValue("s","This is EPPHSceneIf.");
}
int EPPHSceneIf_init(EPPHSceneIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSceneIf* EPPHSceneIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSceneIf *self;
	self = (EPPHSceneIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSceneIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSceneIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSceneIf",/*tp_name*/
	sizeof(EPPHSceneIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSceneIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSceneIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSceneIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSceneIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSceneIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSceneIf_new,/*tp_new*/
};
void initEPPHSceneIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSceneIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSceneIfType);
	PyModule_AddObject(m,"PHSceneIf",(PyObject *)&EPPHSceneIfType);//モジュールに追加
}
PyObject* newEPPHSceneIf(struct PHSceneIf org)
{
	EPPHSceneIf *ret = EPPHSceneIf_new(&EPPHSceneIfType,NULL,NULL);
	ret->ptr = new PHSceneIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSceneIf(struct PHSceneIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSceneIf *ret = EPPHSceneIf_new(&EPPHSceneIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSceneIf

//{*********EPPHSdkDesc*******
static PyMethodDef EPPHSdkDesc_methods[] =
{
	{NULL}
};
void EPPHSdkDesc_dealloc(PyObject* self)
{
	delete ((EPPHSdkDesc*)self)->ptr;
}
PyObject* EPPHSdkDesc_str()
{
	return Py_BuildValue("s","This is EPPHSdkDesc.");
}
int EPPHSdkDesc_init(EPPHSdkDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSdkDesc* EPPHSdkDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSdkDesc *self;
	self = (EPPHSdkDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSdkDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSdkDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSdkDesc",/*tp_name*/
	sizeof(EPPHSdkDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSdkDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSdkDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSdkDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSdkDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSdkDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSdkDesc_new,/*tp_new*/
};
void initEPPHSdkDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSdkDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSdkDescType);
	PyModule_AddObject(m,"PHSdkDesc",(PyObject *)&EPPHSdkDescType);//モジュールに追加
}
PyObject* newEPPHSdkDesc(struct PHSdkDesc org)
{
	EPPHSdkDesc *ret = EPPHSdkDesc_new(&EPPHSdkDescType,NULL,NULL);
	ret->ptr = new PHSdkDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSdkDesc(struct PHSdkDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSdkDesc *ret = EPPHSdkDesc_new(&EPPHSdkDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSdkDesc

//{*********EPPHSdkIf*******
PyObject* EPPHSdkIf_CreateScene( EPPHSdkIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPPHSceneIf* ret = (EPPHSceneIf*)newEPPHSceneIf(self->ptr->CreateScene());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 1&&EPPHSceneDesc_Check(((EPPHSceneDesc*)PyTuple_GetItem(tuple,0))))
	{
		EPPHSceneIf* ret = (EPPHSceneIf*)newEPPHSceneIf(self->ptr->CreateScene((*(((EPPHSceneDesc*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_CreateSdk( EPPHSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHSdkIf* ret = (EPPHSdkIf*)newEPPHSdkIf(PHSdkIf::CreateSdk());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_CreateShape( EPPHSdkIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPIfInfo_Check(((EPIfInfo*)PyTuple_GetItem(tuple,0))) && EPCDShapeDesc_Check(((EPCDShapeDesc*)PyTuple_GetItem(tuple,1))))
	{
		EPCDShapeIf* ret = (EPCDShapeIf*)newEPCDShapeIf(self->ptr->CreateShape((((EPIfInfo*)PyTuple_GetItem(tuple,0))->ptr),(*(((EPCDShapeDesc*)PyTuple_GetItem(tuple,1))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_GetScene( EPPHSdkIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHSceneIf* ret = (EPPHSceneIf*)newEPPHSceneIf(self->ptr->GetScene(PyLong_AsSsize_t((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_GetShape( EPPHSdkIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCDShapeIf* ret = (EPCDShapeIf*)newEPCDShapeIf(self->ptr->GetShape(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_MergeScene( EPPHSdkIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&EPPHSceneIf_Check(((EPPHSceneIf*)PyTuple_GetItem(tuple,0))) && EPPHSceneIf_Check(((EPPHSceneIf*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->MergeScene((((EPPHSceneIf*)PyTuple_GetItem(tuple,0))->ptr),(((EPPHSceneIf*)PyTuple_GetItem(tuple,1))->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_NScene( EPPHSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NScene());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_NShape( EPPHSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NShape());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSdkIf_RegisterSdk( EPPHSdkIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->RegisterSdk();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSdkIf_methods[] =
{
	{"CreateScene",(PyCFunction)EPPHSdkIf_CreateScene,METH_VARARGS,"EPPHSdkIf::CreateScene"},
	{"CreateSdk",(PyCFunction)EPPHSdkIf_CreateSdk,METH_NOARGS,"EPPHSdkIf::CreateSdk"},
	{"CreateShape",(PyCFunction)EPPHSdkIf_CreateShape,METH_VARARGS,"EPPHSdkIf::CreateShape"},
	{"GetScene",(PyCFunction)EPPHSdkIf_GetScene,METH_O,"EPPHSdkIf::GetScene"},
	{"GetShape",(PyCFunction)EPPHSdkIf_GetShape,METH_O,"EPPHSdkIf::GetShape"},
	{"MergeScene",(PyCFunction)EPPHSdkIf_MergeScene,METH_VARARGS,"EPPHSdkIf::MergeScene"},
	{"NScene",(PyCFunction)EPPHSdkIf_NScene,METH_NOARGS,"EPPHSdkIf::NScene"},
	{"NShape",(PyCFunction)EPPHSdkIf_NShape,METH_NOARGS,"EPPHSdkIf::NShape"},
	{"RegisterSdk",(PyCFunction)EPPHSdkIf_RegisterSdk,METH_NOARGS,"EPPHSdkIf::RegisterSdk"},
	{NULL}
};
void EPPHSdkIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSdkIf_str()
{
	return Py_BuildValue("s","This is EPPHSdkIf.");
}
int EPPHSdkIf_init(EPPHSdkIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSdkIf* EPPHSdkIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSdkIf *self;
	self = (EPPHSdkIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSdkIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSdkIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSdkIf",/*tp_name*/
	sizeof(EPPHSdkIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSdkIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSdkIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSdkIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSdkIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSdkIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSdkIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSdkIf_new,/*tp_new*/
};
void initEPPHSdkIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSdkIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSdkIfType);
	PyModule_AddObject(m,"PHSdkIf",(PyObject *)&EPPHSdkIfType);//モジュールに追加
}
PyObject* newEPPHSdkIf(struct PHSdkIf org)
{
	EPPHSdkIf *ret = EPPHSdkIf_new(&EPPHSdkIfType,NULL,NULL);
	ret->ptr = new PHSdkIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSdkIf(struct PHSdkIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSdkIf *ret = EPPHSdkIf_new(&EPPHSdkIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSdkIf

//{*********EPPHFrameDesc*******
static PyMethodDef EPPHFrameDesc_methods[] =
{
	{NULL}
};
void EPPHFrameDesc_dealloc(PyObject* self)
{
	delete ((EPPHFrameDesc*)self)->ptr;
}
PyObject* EPPHFrameDesc_str()
{
	return Py_BuildValue("s","This is EPPHFrameDesc.");
}
int EPPHFrameDesc_init(EPPHFrameDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHFrameDesc* EPPHFrameDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHFrameDesc *self;
	self = (EPPHFrameDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHFrameDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHFrameDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHFrameDesc",/*tp_name*/
	sizeof(EPPHFrameDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHFrameDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHFrameDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHFrameDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHFrameDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHFrameDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHFrameDesc_new,/*tp_new*/
};
void initEPPHFrameDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHFrameDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHFrameDescType);
	PyModule_AddObject(m,"PHFrameDesc",(PyObject *)&EPPHFrameDescType);//モジュールに追加
}
PyObject* newEPPHFrameDesc(struct PHFrameDesc org)
{
	EPPHFrameDesc *ret = EPPHFrameDesc_new(&EPPHFrameDescType,NULL,NULL);
	ret->ptr = new PHFrameDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHFrameDesc(struct PHFrameDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHFrameDesc *ret = EPPHFrameDesc_new(&EPPHFrameDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHFrameDesc

//{*********EPPHFrameIf*******
PyObject* EPPHFrameIf_GetPose( EPPHFrameIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPosed* ret = (EPPosed*)newEPPosed(self->ptr->GetPose());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHFrameIf_SetPose( EPPHFrameIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->SetPose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHFrameIf_methods[] =
{
	{"GetPose",(PyCFunction)EPPHFrameIf_GetPose,METH_NOARGS,"EPPHFrameIf::GetPose"},
	{"SetPose",(PyCFunction)EPPHFrameIf_SetPose,METH_O,"EPPHFrameIf::SetPose"},
	{NULL}
};
void EPPHFrameIf_dealloc(PyObject* self)
{
}
PyObject* EPPHFrameIf_str()
{
	return Py_BuildValue("s","This is EPPHFrameIf.");
}
int EPPHFrameIf_init(EPPHFrameIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHFrameIf* EPPHFrameIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHFrameIf *self;
	self = (EPPHFrameIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHFrameIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHFrameIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHFrameIf",/*tp_name*/
	sizeof(EPPHFrameIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHFrameIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHFrameIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHFrameIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHFrameIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHFrameIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHFrameIf_new,/*tp_new*/
};
void initEPPHFrameIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHFrameIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHFrameIfType);
	PyModule_AddObject(m,"PHFrameIf",(PyObject *)&EPPHFrameIfType);//モジュールに追加
}
PyObject* newEPPHFrameIf(struct PHFrameIf org)
{
	EPPHFrameIf *ret = EPPHFrameIf_new(&EPPHFrameIfType,NULL,NULL);
	ret->ptr = new PHFrameIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHFrameIf(struct PHFrameIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHFrameIf *ret = EPPHFrameIf_new(&EPPHFrameIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHFrameIf

//{*********EPPHSolidState*******
static PyMethodDef EPPHSolidState_methods[] =
{
	{NULL}
};
void EPPHSolidState_dealloc(PyObject* self)
{
	delete ((EPPHSolidState*)self)->ptr;
}
PyObject* EPPHSolidState_str()
{
	return Py_BuildValue("s","This is EPPHSolidState.");
}
int EPPHSolidState_init(EPPHSolidState* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSolidState* EPPHSolidState_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSolidState *self;
	self = (EPPHSolidState*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSolidState_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSolidStateType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSolidState",/*tp_name*/
	sizeof(EPPHSolidState),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSolidState_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSolidState_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSolidState",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSolidState_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	0,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSolidState_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSolidState_new,/*tp_new*/
};
void initEPPHSolidState(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSolidStateType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSolidStateType);
	PyModule_AddObject(m,"PHSolidState",(PyObject *)&EPPHSolidStateType);//モジュールに追加
}
PyObject* newEPPHSolidState(struct PHSolidState org)
{
	EPPHSolidState *ret = EPPHSolidState_new(&EPPHSolidStateType,NULL,NULL);
	ret->ptr = new PHSolidState();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSolidState(struct PHSolidState* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSolidState *ret = EPPHSolidState_new(&EPPHSolidStateType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSolidState

//{*********EPPHSolidDesc*******
PyObject* EPPHSolidDesc_Init( EPPHSolidDesc* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->Init();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSolidDesc_methods[] =
{
	{"Init",(PyCFunction)EPPHSolidDesc_Init,METH_NOARGS,"EPPHSolidDesc::Init"},
	{NULL}
};
void EPPHSolidDesc_dealloc(PyObject* self)
{
	delete ((EPPHSolidDesc*)self)->ptr;
}
PyObject* EPPHSolidDesc_str()
{
	return Py_BuildValue("s","This is EPPHSolidDesc.");
}
int EPPHSolidDesc_init(EPPHSolidDesc* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSolidDesc* EPPHSolidDesc_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSolidDesc *self;
	self = (EPPHSolidDesc*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSolidDesc_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSolidDescType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSolidDesc",/*tp_name*/
	sizeof(EPPHSolidDesc),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSolidDesc_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSolidDesc_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSolidDesc",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSolidDesc_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPPHSolidStateType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSolidDesc_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSolidDesc_new,/*tp_new*/
};
void initEPPHSolidDesc(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSolidDescType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSolidDescType);
	PyModule_AddObject(m,"PHSolidDesc",(PyObject *)&EPPHSolidDescType);//モジュールに追加
}
PyObject* newEPPHSolidDesc(struct PHSolidDesc org)
{
	EPPHSolidDesc *ret = EPPHSolidDesc_new(&EPPHSolidDescType,NULL,NULL);
	ret->ptr = new PHSolidDesc();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSolidDesc(struct PHSolidDesc* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSolidDesc *ret = EPPHSolidDesc_new(&EPPHSolidDescType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSolidDesc

//{*********EPPHSolidIf*******
PyObject* EPPHSolidIf_AddForce( EPPHSolidIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		self->ptr->AddForce((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)));
		Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 2&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))) && EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->AddForce((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr)),(*(((EPVec3d*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_AddShape( EPPHSolidIf* self,EPCDShapeIf* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPCDShapeIf_Check(var1))
	{
		self->ptr->AddShape((var1->ptr));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_AddTorque( EPPHSolidIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->AddTorque((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_ClearShape( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->ClearShape();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_CompInertia( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		self->ptr->CompInertia();
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetAngularVelocity( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetAngularVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetCenterOfMass( EPPHSolidIf* self )
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
PyObject* EPPHSolidIf_GetCenterPosition( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetCenterPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetDeltaPosition( EPPHSolidIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 1&&EPVec3d_Check(((EPVec3d*)PyTuple_GetItem(tuple,0))))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetDeltaPosition((*(((EPVec3d*)PyTuple_GetItem(tuple,0))->ptr))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	if(PyTuple_Size(tuple) == 0&&PyTuple_Size(tuple) == 0)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetDeltaPosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetForce( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetForce());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetFramePosition( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetFramePosition());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetInertia( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(self->ptr->GetInertia());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetInertiaInv( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPMatrix3d* ret = (EPMatrix3d*)newEPMatrix3d(self->ptr->GetInertiaInv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetMass( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetMass());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetMassInv( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetMassInv());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetOrientation( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPQuaterniond* ret = (EPQuaterniond*)newEPQuaterniond(self->ptr->GetOrientation());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetPose( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPosed* ret = (EPPosed*)newEPPosed(self->ptr->GetPose());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetShape( EPPHSolidIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCDShapeIf* ret = (EPCDShapeIf*)newEPCDShapeIf(self->ptr->GetShape(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetShapePose( EPPHSolidIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPosed* ret = (EPPosed*)newEPPosed(self->ptr->GetShapePose(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetTorque( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetTorque());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetTreeNode( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPPHTreeNodeIf* ret = (EPPHTreeNodeIf*)newEPPHTreeNodeIf(self->ptr->GetTreeNode());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_GetVelocity( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetVelocity());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_IsDrawn( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsDrawn()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_IsDynamical( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsDynamical()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_IsIntegrate( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyObject* ret = (PyObject*)PyBool_FromLong(self->ptr->IsIntegrate()? 1 : 0);
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_NShape( EPPHSolidIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NShape());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetAngularVelocity( EPPHSolidIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetAngularVelocity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetCenterOfMass( EPPHSolidIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetCenterOfMass((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetCenterPosition( EPPHSolidIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetCenterPosition((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetDrawing( EPPHSolidIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetDrawing((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetDynamical( EPPHSolidIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetDynamical((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetFramePosition( EPPHSolidIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetFramePosition((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetGravity( EPPHSolidIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetGravity((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetInertia( EPPHSolidIf* self,EPMatrix3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPMatrix3d_Check(var1))
	{
		self->ptr->SetInertia((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetIntegrate( EPPHSolidIf* self,PyObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyBool_Check(var1))
	{
		self->ptr->SetIntegrate((Py_True == var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetMass( EPPHSolidIf* self,PyFloatObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyFloat_Check(var1))
	{
		self->ptr->SetMass(PyFloat_AS_DOUBLE((PyObject*)var1));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetOrientation( EPPHSolidIf* self,EPQuaterniond* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPQuaterniond_Check(var1))
	{
		self->ptr->SetOrientation((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetPose( EPPHSolidIf* self,EPPosed* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPPosed_Check(var1))
	{
		self->ptr->SetPose((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetShapePose( EPPHSolidIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && EPPosed_Check(((EPPosed*)PyTuple_GetItem(tuple,1))))
	{
		self->ptr->SetShapePose(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),(*(((EPPosed*)PyTuple_GetItem(tuple,1))->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidIf_SetVelocity( EPPHSolidIf* self,EPVec3d* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(EPVec3d_Check(var1))
	{
		self->ptr->SetVelocity((*(var1->ptr)));
		Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSolidIf_methods[] =
{
	{"AddForce",(PyCFunction)EPPHSolidIf_AddForce,METH_VARARGS,"EPPHSolidIf::AddForce"},
	{"AddShape",(PyCFunction)EPPHSolidIf_AddShape,METH_O,"EPPHSolidIf::AddShape"},
	{"AddTorque",(PyCFunction)EPPHSolidIf_AddTorque,METH_O,"EPPHSolidIf::AddTorque"},
	{"ClearShape",(PyCFunction)EPPHSolidIf_ClearShape,METH_NOARGS,"EPPHSolidIf::ClearShape"},
	{"CompInertia",(PyCFunction)EPPHSolidIf_CompInertia,METH_NOARGS,"EPPHSolidIf::CompInertia"},
	{"GetAngularVelocity",(PyCFunction)EPPHSolidIf_GetAngularVelocity,METH_NOARGS,"EPPHSolidIf::GetAngularVelocity"},
	{"GetCenterOfMass",(PyCFunction)EPPHSolidIf_GetCenterOfMass,METH_NOARGS,"EPPHSolidIf::GetCenterOfMass"},
	{"GetCenterPosition",(PyCFunction)EPPHSolidIf_GetCenterPosition,METH_NOARGS,"EPPHSolidIf::GetCenterPosition"},
	{"GetDeltaPosition",(PyCFunction)EPPHSolidIf_GetDeltaPosition,METH_VARARGS,"EPPHSolidIf::GetDeltaPosition"},
	{"GetForce",(PyCFunction)EPPHSolidIf_GetForce,METH_NOARGS,"EPPHSolidIf::GetForce"},
	{"GetFramePosition",(PyCFunction)EPPHSolidIf_GetFramePosition,METH_NOARGS,"EPPHSolidIf::GetFramePosition"},
	{"GetInertia",(PyCFunction)EPPHSolidIf_GetInertia,METH_NOARGS,"EPPHSolidIf::GetInertia"},
	{"GetInertiaInv",(PyCFunction)EPPHSolidIf_GetInertiaInv,METH_NOARGS,"EPPHSolidIf::GetInertiaInv"},
	{"GetMass",(PyCFunction)EPPHSolidIf_GetMass,METH_NOARGS,"EPPHSolidIf::GetMass"},
	{"GetMassInv",(PyCFunction)EPPHSolidIf_GetMassInv,METH_NOARGS,"EPPHSolidIf::GetMassInv"},
	{"GetOrientation",(PyCFunction)EPPHSolidIf_GetOrientation,METH_NOARGS,"EPPHSolidIf::GetOrientation"},
	{"GetPose",(PyCFunction)EPPHSolidIf_GetPose,METH_NOARGS,"EPPHSolidIf::GetPose"},
	{"GetShape",(PyCFunction)EPPHSolidIf_GetShape,METH_O,"EPPHSolidIf::GetShape"},
	{"GetShapePose",(PyCFunction)EPPHSolidIf_GetShapePose,METH_O,"EPPHSolidIf::GetShapePose"},
	{"GetTorque",(PyCFunction)EPPHSolidIf_GetTorque,METH_NOARGS,"EPPHSolidIf::GetTorque"},
	{"GetTreeNode",(PyCFunction)EPPHSolidIf_GetTreeNode,METH_NOARGS,"EPPHSolidIf::GetTreeNode"},
	{"GetVelocity",(PyCFunction)EPPHSolidIf_GetVelocity,METH_NOARGS,"EPPHSolidIf::GetVelocity"},
	{"IsDrawn",(PyCFunction)EPPHSolidIf_IsDrawn,METH_NOARGS,"EPPHSolidIf::IsDrawn"},
	{"IsDynamical",(PyCFunction)EPPHSolidIf_IsDynamical,METH_NOARGS,"EPPHSolidIf::IsDynamical"},
	{"IsIntegrate",(PyCFunction)EPPHSolidIf_IsIntegrate,METH_NOARGS,"EPPHSolidIf::IsIntegrate"},
	{"NShape",(PyCFunction)EPPHSolidIf_NShape,METH_NOARGS,"EPPHSolidIf::NShape"},
	{"SetAngularVelocity",(PyCFunction)EPPHSolidIf_SetAngularVelocity,METH_O,"EPPHSolidIf::SetAngularVelocity"},
	{"SetCenterOfMass",(PyCFunction)EPPHSolidIf_SetCenterOfMass,METH_O,"EPPHSolidIf::SetCenterOfMass"},
	{"SetCenterPosition",(PyCFunction)EPPHSolidIf_SetCenterPosition,METH_O,"EPPHSolidIf::SetCenterPosition"},
	{"SetDrawing",(PyCFunction)EPPHSolidIf_SetDrawing,METH_O,"EPPHSolidIf::SetDrawing"},
	{"SetDynamical",(PyCFunction)EPPHSolidIf_SetDynamical,METH_O,"EPPHSolidIf::SetDynamical"},
	{"SetFramePosition",(PyCFunction)EPPHSolidIf_SetFramePosition,METH_O,"EPPHSolidIf::SetFramePosition"},
	{"SetGravity",(PyCFunction)EPPHSolidIf_SetGravity,METH_O,"EPPHSolidIf::SetGravity"},
	{"SetInertia",(PyCFunction)EPPHSolidIf_SetInertia,METH_O,"EPPHSolidIf::SetInertia"},
	{"SetIntegrate",(PyCFunction)EPPHSolidIf_SetIntegrate,METH_O,"EPPHSolidIf::SetIntegrate"},
	{"SetMass",(PyCFunction)EPPHSolidIf_SetMass,METH_O,"EPPHSolidIf::SetMass"},
	{"SetOrientation",(PyCFunction)EPPHSolidIf_SetOrientation,METH_O,"EPPHSolidIf::SetOrientation"},
	{"SetPose",(PyCFunction)EPPHSolidIf_SetPose,METH_O,"EPPHSolidIf::SetPose"},
	{"SetShapePose",(PyCFunction)EPPHSolidIf_SetShapePose,METH_VARARGS,"EPPHSolidIf::SetShapePose"},
	{"SetVelocity",(PyCFunction)EPPHSolidIf_SetVelocity,METH_O,"EPPHSolidIf::SetVelocity"},
	{NULL}
};
void EPPHSolidIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSolidIf_str()
{
	return Py_BuildValue("s","This is EPPHSolidIf.");
}
int EPPHSolidIf_init(EPPHSolidIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSolidIf* EPPHSolidIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSolidIf *self;
	self = (EPPHSolidIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSolidIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSolidIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSolidIf",/*tp_name*/
	sizeof(EPPHSolidIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSolidIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSolidIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSolidIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSolidIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPSceneObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSolidIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSolidIf_new,/*tp_new*/
};
void initEPPHSolidIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSolidIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSolidIfType);
	PyModule_AddObject(m,"PHSolidIf",(PyObject *)&EPPHSolidIfType);//モジュールに追加
}
PyObject* newEPPHSolidIf(struct PHSolidIf org)
{
	EPPHSolidIf *ret = EPPHSolidIf_new(&EPPHSolidIfType,NULL,NULL);
	ret->ptr = new PHSolidIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSolidIf(struct PHSolidIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSolidIf *ret = EPPHSolidIf_new(&EPPHSolidIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSolidIf

//{*********EPPHShapePairForLCPIf*******
PyObject* EPPHShapePairForLCPIf_GetContactDimension( EPPHShapePairForLCPIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetContactDimension());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHShapePairForLCPIf_GetNormalVector( EPPHShapePairForLCPIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetNormalVector());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHShapePairForLCPIf_GetSectionVertex( EPPHShapePairForLCPIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetSectionVertex(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHShapePairForLCPIf_GetShape( EPPHShapePairForLCPIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPCDShapeIf* ret = (EPCDShapeIf*)newEPCDShapeIf(self->ptr->GetShape(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHShapePairForLCPIf_NSectionVertexes( EPPHShapePairForLCPIf* self )
{
	UTAutoLock lock(EPCriticalSection);
	if(true)
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->NSectionVertexes());
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHShapePairForLCPIf_methods[] =
{
	{"GetContactDimension",(PyCFunction)EPPHShapePairForLCPIf_GetContactDimension,METH_NOARGS,"EPPHShapePairForLCPIf::GetContactDimension"},
	{"GetNormalVector",(PyCFunction)EPPHShapePairForLCPIf_GetNormalVector,METH_NOARGS,"EPPHShapePairForLCPIf::GetNormalVector"},
	{"GetSectionVertex",(PyCFunction)EPPHShapePairForLCPIf_GetSectionVertex,METH_O,"EPPHShapePairForLCPIf::GetSectionVertex"},
	{"GetShape",(PyCFunction)EPPHShapePairForLCPIf_GetShape,METH_O,"EPPHShapePairForLCPIf::GetShape"},
	{"NSectionVertexes",(PyCFunction)EPPHShapePairForLCPIf_NSectionVertexes,METH_NOARGS,"EPPHShapePairForLCPIf::NSectionVertexes"},
	{NULL}
};
void EPPHShapePairForLCPIf_dealloc(PyObject* self)
{
}
PyObject* EPPHShapePairForLCPIf_str()
{
	return Py_BuildValue("s","This is EPPHShapePairForLCPIf.");
}
int EPPHShapePairForLCPIf_init(EPPHShapePairForLCPIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHShapePairForLCPIf* EPPHShapePairForLCPIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHShapePairForLCPIf *self;
	self = (EPPHShapePairForLCPIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHShapePairForLCPIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHShapePairForLCPIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHShapePairForLCPIf",/*tp_name*/
	sizeof(EPPHShapePairForLCPIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHShapePairForLCPIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHShapePairForLCPIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHShapePairForLCPIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHShapePairForLCPIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHShapePairForLCPIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHShapePairForLCPIf_new,/*tp_new*/
};
void initEPPHShapePairForLCPIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHShapePairForLCPIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHShapePairForLCPIfType);
	PyModule_AddObject(m,"PHShapePairForLCPIf",(PyObject *)&EPPHShapePairForLCPIfType);//モジュールに追加
}
PyObject* newEPPHShapePairForLCPIf(struct PHShapePairForLCPIf org)
{
	EPPHShapePairForLCPIf *ret = EPPHShapePairForLCPIf_new(&EPPHShapePairForLCPIfType,NULL,NULL);
	ret->ptr = new PHShapePairForLCPIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHShapePairForLCPIf(struct PHShapePairForLCPIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHShapePairForLCPIf *ret = EPPHShapePairForLCPIf_new(&EPPHShapePairForLCPIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHShapePairForLCPIf

//{*********EPPHSolidPairForLCPIf*******
PyObject* EPPHSolidPairForLCPIf_GetCommonPoint( EPPHSolidPairForLCPIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPVec3d* ret = (EPVec3d*)newEPVec3d(self->ptr->GetCommonPoint(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidPairForLCPIf_GetContactDepth( EPPHSolidPairForLCPIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		PyFloatObject* ret = (PyFloatObject*)Py_BuildValue("d",self->ptr->GetContactDepth(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidPairForLCPIf_GetContactState( EPPHSolidPairForLCPIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		PyLongObject* ret = (PyLongObject*)Py_BuildValue("i",self->ptr->GetContactState(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidPairForLCPIf_GetLastContactCount( EPPHSolidPairForLCPIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPunsignedint* ret = (EPunsignedint*)newEPunsignedint(self->ptr->GetLastContactCount(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidPairForLCPIf_GetShapePair( EPPHSolidPairForLCPIf* self,PyObject* tuple )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyTuple_Size(tuple) == 2&&PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,0))) && PyLong_Check(((PyLongObject*)PyTuple_GetItem(tuple,1))))
	{
		EPPHShapePairForLCPIf* ret = (EPPHShapePairForLCPIf*)newEPPHShapePairForLCPIf(self->ptr->GetShapePair(PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,0))),PyLong_AsLong((PyObject*)((PyLongObject*)PyTuple_GetItem(tuple,1)))));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
PyObject* EPPHSolidPairForLCPIf_GetSolid( EPPHSolidPairForLCPIf* self,PyLongObject* var1 )
{
	UTAutoLock lock(EPCriticalSection);
	if(PyLong_Check(var1))
	{
		EPPHSolidIf* ret = (EPPHSolidIf*)newEPPHSolidIf(self->ptr->GetSolid(PyLong_AsLong((PyObject*)var1)));
		if ( ret ) return (PyObject*) ret;
		else Py_RETURN_NONE;
	}
	Return_ArgError;
}
static PyMethodDef EPPHSolidPairForLCPIf_methods[] =
{
	{"GetCommonPoint",(PyCFunction)EPPHSolidPairForLCPIf_GetCommonPoint,METH_VARARGS,"EPPHSolidPairForLCPIf::GetCommonPoint"},
	{"GetContactDepth",(PyCFunction)EPPHSolidPairForLCPIf_GetContactDepth,METH_VARARGS,"EPPHSolidPairForLCPIf::GetContactDepth"},
	{"GetContactState",(PyCFunction)EPPHSolidPairForLCPIf_GetContactState,METH_VARARGS,"EPPHSolidPairForLCPIf::GetContactState"},
	{"GetLastContactCount",(PyCFunction)EPPHSolidPairForLCPIf_GetLastContactCount,METH_VARARGS,"EPPHSolidPairForLCPIf::GetLastContactCount"},
	{"GetShapePair",(PyCFunction)EPPHSolidPairForLCPIf_GetShapePair,METH_VARARGS,"EPPHSolidPairForLCPIf::GetShapePair"},
	{"GetSolid",(PyCFunction)EPPHSolidPairForLCPIf_GetSolid,METH_O,"EPPHSolidPairForLCPIf::GetSolid"},
	{NULL}
};
void EPPHSolidPairForLCPIf_dealloc(PyObject* self)
{
}
PyObject* EPPHSolidPairForLCPIf_str()
{
	return Py_BuildValue("s","This is EPPHSolidPairForLCPIf.");
}
int EPPHSolidPairForLCPIf_init(EPPHSolidPairForLCPIf* self,PyObject *args, PyObject *kwds)
{
	return 0;
}
EPPHSolidPairForLCPIf* EPPHSolidPairForLCPIf_new(PyTypeObject *type,PyObject *args, PyObject *kwds)
{
	EPPHSolidPairForLCPIf *self;
	self = (EPPHSolidPairForLCPIf*) type->tp_alloc(type,0);
	if ( self != NULL ) EPPHSolidPairForLCPIf_init(self,args,kwds);
	return self;
}
PyTypeObject EPPHSolidPairForLCPIfType =
{
	PyVarObject_HEAD_INIT(NULL,0)
	"Physics.EPPHSolidPairForLCPIf",/*tp_name*/
	sizeof(EPPHSolidPairForLCPIf),/*tp_basicsize*/
	0,/*tp_itemsize*/
	(destructor)EPPHSolidPairForLCPIf_dealloc,/*tp_dealloc*/
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
	(reprfunc)EPPHSolidPairForLCPIf_str,/*tp_str*/
	0,/*tp_getattro*/
	0,/*tp_setattro*/
	0,/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,/*tp_flags*/
	"PHSolidPairForLCPIf",/*tp_doc*/
	0,/*tp_traverse*/
	0,/*tp_clear*/
	0,/*tp_richcompare*/
	0,/*tp_weaklistoffset*/
	0,/*tp_iter*/
	0,/*tp_iternext*/
	EPPHSolidPairForLCPIf_methods,/*tp_methods*/
	0,/*tp_members*/
	0,/*tp_getset*/
	&EPObjectIfType,/*tp_base*/
	0,/*tp_dict*/
	0,/*tp_descr_get*/
	0,/*tp_descr_set*/
	0,/*tp_dictoffset*/
	(initproc)EPPHSolidPairForLCPIf_init,/*tp_init*/
	0,/*tp_alloc*/
	(newfunc)EPPHSolidPairForLCPIf_new,/*tp_new*/
};
void initEPPHSolidPairForLCPIf(void)
{
	PyObject* m;
	if ( PyType_Ready( &EPPHSolidPairForLCPIfType ) < 0 ) return ;//Pythonクラスの作成
	m = PyImport_AddModule("Physics");
	Py_INCREF(&EPPHSolidPairForLCPIfType);
	PyModule_AddObject(m,"PHSolidPairForLCPIf",(PyObject *)&EPPHSolidPairForLCPIfType);//モジュールに追加
}
PyObject* newEPPHSolidPairForLCPIf(struct PHSolidPairForLCPIf org)
{
	EPPHSolidPairForLCPIf *ret = EPPHSolidPairForLCPIf_new(&EPPHSolidPairForLCPIfType,NULL,NULL);
	ret->ptr = new PHSolidPairForLCPIf();
	*ret->ptr = org;
	return (PyObject*)ret;
}
PyObject* newEPPHSolidPairForLCPIf(struct PHSolidPairForLCPIf* org)
{
	if(org == NULL)
	{
		Return_NewNullError;
	}
	EPPHSolidPairForLCPIf *ret = EPPHSolidPairForLCPIf_new(&EPPHSolidPairForLCPIfType,NULL,NULL);
	ret->ptr = org;
	return (PyObject*)ret;
}

//}PHSolidPairForLCPIf
/**************** for Module ******************/
void initPhysics(void)
{
	initEPPHEngineIf();
	initEPPHConstraintEngineIf();
	initEPPHGravityEngineIf();
	initEPPHPenaltyEngineIf();
	initEPPHIKEngineIf();
	initEPPHIKEndEffectorIf();
	initEPPHIKEndEffectorDesc();
	initEPPHIKActuatorIf();
	initEPPHIKActuatorDesc();
	initEPPHIKBallActuatorIf();
	initEPPHIKBallActuatorDesc();
	initEPPHIKHingeActuatorIf();
	initEPPHIKHingeActuatorDesc();
	initEPPHConstraintDesc();
	initEPPHJointDesc();
	initEPPHJoint1DDesc();
	initEPPHTreeNodeDesc();
	initEPPHRootNodeDesc();
	initEPPHTreeNode1DDesc();
	initEPPHHingeJointNodeDesc();
	initEPPHSliderJointNodeDesc();
	initEPPHPathJointNodeDesc();
	initEPPHBallJointNodeDesc();
	initEPPHGearDesc();
	initEPPHConstraintIf();
	initEPPHConstraintsIf();
	initEPPHContactPointIf();
	initEPPHJointIf();
	initEPPHJoint1DIf();
	initEPPHHingeJointIf();
	initEPPHHingeJointDesc();
	initEPPHSliderJointIf();
	initEPPHSliderJointDesc();
	initEPPHPathPoint();
	initEPPHPathDesc();
	initEPPHPathIf();
	initEPPHPathJointIf();
	initEPPHPathJointDesc();
	initEPPHBallJointIf();
	initEPPHBallJointDesc();
	initEPPHSpringIf();
	initEPPHSpringDesc();
	initEPPHTreeNodeIf();
	initEPPHRootNodeIf();
	initEPPHTreeNode1DIf();
	initEPPHHingeJointNodeIf();
	initEPPHSliderJointNodeIf();
	initEPPHPathJointNodeIf();
	initEPPHBallJointNodeIf();
	initEPPHGearIf();
	initEPPHInactiveSolids();
	initEPPHRaycastHit();
	initEPPHRayDesc();
	initEPPHRayIf();
	initEPPHSceneState();
	initEPPHSceneDesc();
	initEPPHSceneIf();
	initEPPHSdkDesc();
	initEPPHSdkIf();
	initEPPHFrameDesc();
	initEPPHFrameIf();
	initEPPHSolidState();
	initEPPHSolidDesc();
	initEPPHSolidIf();
	initEPPHShapePairForLCPIf();
	initEPPHSolidPairForLCPIf();
}
