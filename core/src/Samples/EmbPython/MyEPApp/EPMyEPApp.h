#ifndef EPMYEPAPP_H
#define EPMYEPAPP_H

//{*********EPSampleApp*******
void toEPSampleApp( EPObject* obj);
//}EPSampleApp

//{*********EPHogeshi*******
void toEPHogeshi( EPObject* obj);
//}EPHogeshi

//{*********EPMyEPApp*******
void toEPMyEPApp( EPObject* obj);
PyObject* __PYDECL EPMyEPApp_Drop( PyObject* self,PyObject* arg );
PyObject* __PYDECL EPMyEPApp_OnAction( PyObject* self,PyObject* arg );
PyObject* __PYDECL EPMyEPApp_OnDraw( PyObject* self,PyObject* arg );
PyObject* __PYDECL EPMyEPApp_OnStep( PyObject* self );
//}EPMyEPApp
#endif
