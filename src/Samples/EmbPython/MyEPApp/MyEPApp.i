%module MyEPApp



%define EXTEND_NEW(type)
%header%{
	PyObject* __EPDECL newEP##type(type var1);
	PyObject* __EPDECL newEP##type();
%}

%wrapper%{
	PyObject* newEP##type(type var1)
	{
		PyObject *ret = EP##type##_new(&EP##type##Type,NULL,NULL);
		EPObject_Ptr(ret) = new type(var1);
		((EPObject*)ret)->mm = EP_MM_PY;
		return ret;
	}
	
	PyObject* newEP##type()
	{
		PyObject *ret = EP##type##_new(&EP##type##Type,NULL,NULL);
		EPObject_Ptr(ret) = new type();
		((EPObject*)ret)->mm = EP_MM_PY;
		return ret;
	}
%}
%enddef


EXTEND_NEW(Hogeshi)


%ignore MyEPApp::MyEPApp;
%ignore MyEPApp::Init;
%ignore MyEPApp::hiSdk;
%ignore MyEPApp::spaceNavigator0;
%ignore MyEPApp::spaceNavigator1;
%ignore MyEPApp::argv;
%ignore MyEPApp::argc;

class SampleApp {};

%include "MyEPApp.h"
