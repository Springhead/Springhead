#ifndef SPREPERRORS_H
#define SPREPERRORS_H

#define Return_ArgError {\
	PyErr_SetString( PyExc_SyntaxError, "" );\
	PyErr_Print();\
	Py_RETURN_NONE;\
}

#define Return_ArgError_Int {\
	PyErr_SetString( PyExc_SyntaxError, "" );\
	PyErr_Print();\
	return -1;\
}

#define Return_NotImplementedError(msg) {\
	PyErr_SetString( PyExc_NotImplementedError, msg );\
	PyErr_Print();\
	Py_RETURN_NONE;\
}

#define Return_NewNullError {\
	PyErr_SetString( PyExc_SystemError, "Created Object by NULL." );\
	PyErr_Print();\
	Py_RETURN_NONE;\
}

#endif