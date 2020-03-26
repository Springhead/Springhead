#ifndef SPREPMYEPAPP_H
#define SPREPMYEPAPP_H

//{*********EPSampleApp*******
extern PyTypeObject EPSampleAppType;
#define EPSampleApp_Check(ob) PyObject_TypeCheck(ob, &EPSampleAppType)
PyObject* newEPSampleApp(const SampleApp*);
//}EPSampleApp

//{*********EPHogeshi*******
extern PyTypeObject EPHogeshiType;
#define EPHogeshi_Check(ob) PyObject_TypeCheck(ob, &EPHogeshiType)
PyObject* newEPHogeshi(const struct Hogeshi*);
//}EPHogeshi

//{*********EPMyEPApp*******
extern PyTypeObject EPMyEPAppType;
#define EPMyEPApp_Check(ob) PyObject_TypeCheck(ob, &EPMyEPAppType)
PyObject* newEPMyEPApp(const MyEPApp*);
//}EPMyEPApp

PyObject* __EPDECL newEPHogeshi(Hogeshi var1);
PyObject* __EPDECL newEPHogeshi();
void initMyEPApp(PyObject *rootModule = NULL) ;
#endif
