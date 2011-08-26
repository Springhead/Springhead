#include "..\..\..\include\EmbPython\Utility\SprEPCast.h"
using namespace std;
#include <locale>

double PyObject_asDouble(PyObject* obj)
{
	if(PyFloat_Check(obj))
		return PyFloat_AsDouble(obj);
	else if (PyLong_Check(obj))
		return PyLong_AsDouble(obj);
	else 
		assert(0);
	
	return 0;
}

long PyObject_asLong(PyObject* obj)
{
	if(PyFloat_Check(obj))
		return (long)PyFloat_AsDouble(obj);
	else if (PyLong_Check(obj))
		return PyLong_AsLong(obj);
	else 
		assert(0);

	return 0;
}

//char* PyObject_asChar(PyObject* obj)
//{
//	PyObject* tmp = ((*((PyTupleObject*)obj)).ob_item[0]);
//	bool b = PyUnicode_Check(tmp);
//
//	//ワイド文字列(WCHAR*)をマルチバイト文字列(char*)に変換
//	//errno_t wcstombs_s(
//	//   size_t *pReturnValue,	//変換された文字数
//	//   char *mbstr,		//変換結果のマルチバイト文字列用のバッファのアドレス(変換先)
//	//   size_t sizeInBytes,	//mbstr バッファのサイズ 
//	//   const wchar_t *wcstr,	//変換されるワイド文字列のアドレス(変換元)
//	//   size_t count 		//wcstr に格納するワイド文字の最大数
//	//);
//	char buff[256];
//	wcstombs(buff,((const wchar_t *)((PyUnicodeObject *)(tmp))->str),sizeof(buff));
//
//	//basic_string<wchar_t> test(
//	//PyUnicode_Check(op)),
//	EPObject_Cast(self,CRBodyControllerIf)->SetTargetPos(buff,(*EPObject_Cast((PyTuple_GetItem(arg,1)),Vec3d)));
//	Py_RETURN_NONE;
//	Return_ArgError;
//}

ConvertStr::ConvertStr( PyObject *obj )
{
	buff = NULL;

	if(PyUnicode_Check(obj))
	{
		Py_ssize_t size = PyObject_Length( obj );
		buff = new char[size+1];
		//setlocale( LC_CTYPE, "jpn" );
		int ret = wcstombs(buff,(const wchar_t*)PyUnicode_AS_DATA(obj),size);
		buff[ret] = '\0';
	}
}

ConvertStr::~ConvertStr( )
{
	delete [] buff;
}

string ConvertStr::GetString()
{
	return string(buff);
}

const char* ConvertStr::GetBuffer()
{
	return buff;
}


EPCastfuncMap EPObject_CastMap;

void EPObject_RegistCastfunc(string cls,EPCastfunc func)
{
	EPObject_CastMap.insert(pair<string,EPCastfunc>(cls,func));
}

void EPObject_RuntimeDCast(EPObject* obj,string cls)
{
	EPCastfuncMap::iterator it = EPObject_CastMap.find(cls);
	if(it != EPObject_CastMap.end()) 
		it->second(obj);
}