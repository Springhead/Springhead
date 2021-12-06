// wrapper.cpp
//
#include <Springhead.h>
#include <Windows.h>
#include <vector>
#include "CSUtility.h"
#include <stdio.h>
#include <../src/SprCSharp/SprExport/CSUtility.h>

using namespace std;
using namespace Spr;

extern "C" {
    // wrapper base class
    // vectorwrapper
    // arraywrapper

    // std::vector
    //  int
    __declspec(dllexport) HANDLE __cdecl Spr_vector_new_int() {
	HANDLE _ptr = (HANDLE) NULL;
	try { _ptr = new vector<int>; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_vector_delete_int(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<int>* vec = (vector<int>*) ptr;
        try { vector<int>().swap(*vec); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_get_int(HANDLE ptr, int index) {
        vector<int>* vecptr = (vector<int>*) ptr;
	int _val = 0;
        try { _val = (*vecptr)[index]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_int(HANDLE ptr, int index, int value) {
        vector<int>* vecptr = (vector<int>*) ptr;
        try { (*vecptr)[index] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_int(HANDLE ptr) {
	if (ptr == NULL) return 0;
        vector<int>* vecptr = (vector<int>*) ptr;
	int _val = 0;
        try { _val = (int) (*vecptr).size(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_int(HANDLE ptr, int value) {
        vector<int>* vecptr = (vector<int>*) ptr;
        try { (*vecptr).push_back(value); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_int(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<int>* vecptr = (vector<int>*) ptr;
        try { (*vecptr).clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    //  unsigned int
    __declspec(dllexport) HANDLE __cdecl Spr_vector_new_unsigned_int() {
	HANDLE _ptr = (HANDLE) NULL;
        try { _ptr = new vector<unsigned int>; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_vector_delete_unsigned_int(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<unsigned int>* vec = (vector<unsigned int>*) ptr;
        try { vector<unsigned int>().swap(*vec); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) unsigned int __cdecl Spr_vector_get_unsigned_int(HANDLE ptr, int index) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
	unsigned int _val = 0;
        try { _val = (*vecptr)[index]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_unsigned_int(HANDLE ptr, int index, unsigned int value) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        try { (*vecptr)[index] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_unsigned_int(HANDLE ptr) {
	if (ptr == NULL) return 0;
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
	int _val = 0;
        try { _val = (int) (*vecptr).size(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_unsigned_int(HANDLE ptr, unsigned int value) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        try { (*vecptr).push_back(value); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_unsigned_int(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        try { (*vecptr).clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    //  size_t
    __declspec(dllexport) HANDLE __cdecl Spr_vector_new_size_t() {
	HANDLE _ptr = (HANDLE) NULL;
        try { _ptr = new vector<size_t>; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_vector_delete_size_t(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<size_t>* vec = (vector<size_t>*) ptr;
        try { vector<size_t>().swap(*vec); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_vector_get_size_t(HANDLE ptr, int index) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
	size_t _val = 0;
        try { _val = (*vecptr)[index]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_size_t(HANDLE ptr, int index, size_t value) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        try { (*vecptr)[index] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_size_t(HANDLE ptr) {
	if (ptr == NULL) return 0;
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
	int _val = 0;
        try { _val = (int) (*vecptr).size(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_size_t(HANDLE ptr, size_t value) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        try { (*vecptr).push_back(value); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_size_t(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        try { (*vecptr).clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    //  float
    __declspec(dllexport) HANDLE __cdecl Spr_vector_new_float() {
	HANDLE _ptr = (HANDLE) NULL;
	try { _ptr = new vector<float>; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_vector_delete_float(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<float>* vecptr = (vector<float>*) ptr;
	try { vector<float>().swap(*vecptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_vector_get_float(HANDLE ptr, int index) {
        vector<float>* vecptr = (vector<float>*) ptr;
	float _val = 0;
	try { _val = (*vecptr)[index]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_float(HANDLE ptr, int index, float value) {
        vector<float>* vecptr = (vector<float>*) ptr;
	try { (*vecptr)[index] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_float(HANDLE ptr) {
	if (ptr == NULL) return 0;
        vector<float>* vecptr = (vector<float>*) ptr;
	int _val = 0;
	try { _val = (int) (*vecptr).size(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_float(HANDLE ptr, float value) {
        vector<float>* vecptr = (vector<float>*) ptr;
	try { (*vecptr).push_back(value); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_float(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<float>* vecptr = (vector<float>*) ptr;
	try { (*vecptr).clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    //  double
    __declspec(dllexport) HANDLE __cdecl Spr_vector_new_double() {
	HANDLE _ptr = (HANDLE) NULL;
        try { _ptr = new vector<double>; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_vector_delete_double(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<double>* vecptr = (vector<double>*) ptr;
        try { vector<double>().swap(*vecptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_vector_get_double(HANDLE ptr, int index) {
        vector<double>* vecptr = (vector<double>*) ptr;
	double _val = 0;
        try { _val = (*vecptr)[index]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_double(HANDLE ptr, int index, double value) {
        vector<double>* vecptr = (vector<double>*) ptr;
        try { (*vecptr)[index] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_double(HANDLE ptr) {
	if (ptr == NULL) return 0;
        vector<double>* vecptr = (vector<double>*) ptr;
	int _val = 0;
        try { _val = (int) (*vecptr).size(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_double(HANDLE ptr, double value) {
        vector<double>* vecptr = (vector<double>*) ptr;
        try { (*vecptr).push_back(value); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_double(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<double>* vecptr = (vector<double>*) ptr;
        try { (*vecptr).clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    //  string
    __declspec(dllexport) HANDLE __cdecl Spr_vector_new_string() {
	HANDLE _ptr = (HANDLE) NULL;
        try { _ptr = new vector<string>; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_vector_delete_string(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<string>* vecptr = (vector<string>*) ptr;
        try { vector<string>().swap(*vecptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_vector_get_string(HANDLE ptr, int index) {
        BSTR result = NULL;
        vector<string>* vecptr = (vector<string>*) ptr;
	try {
            const char* cstr = (*vecptr)[index].c_str();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
            if (lenW > 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, result, lenW);
            }
	}
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_string(HANDLE ptr, int index, HANDLE value) {
	try {
            int lenMB = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, NULL, 0, NULL, NULL);
            if (lenMB > 0) {
                LPSTR addr = (LPSTR) ::SysAllocStringLen(0, lenMB);
                ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, addr, lenMB, NULL, NULL);
                vector<string>* vecptr = (vector<string>*) ptr;
                (*vecptr)[index] = string(addr);
            }
	}
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_string(HANDLE ptr) {
	if (ptr == NULL) return 0;
        vector<string>* vecptr = (vector<string>*) ptr;
	int _val = 0;
        try { _val = (int) (*vecptr).size(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_string(HANDLE ptr, HANDLE value) {
	try {
            int lenMB = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, NULL, 0, NULL, NULL);
            if (lenMB > 0) {
                LPSTR addr = (LPSTR) ::SysAllocStringLen(0, lenMB);
                ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, addr, lenMB, NULL, NULL);
                vector<string>* vecptr = (vector<string>*) ptr;
                (*vecptr).push_back(string(addr));
            }
	}
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_string(HANDLE ptr) {
	if (ptr == NULL) return;
        vector<string>* vecptr = (vector<string>*) ptr;
        try { (*vecptr).clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }

    // array
    //  int
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_int(unsigned int nelm) {
	HANDLE _ptr = (HANDLE) NULL;
	try { _ptr = new int[nelm]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_int(HANDLE ptr) {
	int* _ptr = (int*) ptr;
        try { delete[] _ptr; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_array_get_int(HANDLE ptr, int index) {
        int* aryptr = (int*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_int(HANDLE ptr, int index, int value) {
        int* aryptr = (int*) ptr;
        aryptr[index] = value;
    }
    //  unsigned int
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_unsigned_int(unsigned int nelm) {
        return (HANDLE) new unsigned int[nelm];
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_unsigned_int(HANDLE ptr) {
	if (ptr == NULL) return;
	delete[] ptr;
    }
    __declspec(dllexport) unsigned int __cdecl Spr_array_get_unsigned_int(HANDLE ptr, int index) {
        unsigned int* aryptr = (unsigned int*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_unsigned_int(HANDLE ptr, int index, unsigned int value) {
        unsigned int* aryptr = (unsigned int*) ptr;
        aryptr[index] = value;
    }
    //  size_t
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_size_t(unsigned int nelm) {
        return (HANDLE) new size_t[nelm];
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_size_t(HANDLE ptr) {
	if (ptr == NULL) return;
	delete[] ptr;
    }
    __declspec(dllexport) size_t __cdecl Spr_array_get_size_t(HANDLE ptr, int index) {
        size_t* aryptr = (size_t*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_size_t(HANDLE ptr, int index, size_t value) {
        size_t* aryptr = (size_t*) ptr;
        aryptr[index] = value;
    }
    //  float
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_float(unsigned int nelm) {
        return (HANDLE) new float[nelm];
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_float(HANDLE ptr) {
	if (ptr == NULL) return;
	delete[] ptr;
    }
    __declspec(dllexport) float __cdecl Spr_array_get_float(HANDLE ptr, int index) {
        float* aryptr = (float*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_float(HANDLE ptr, int index, float value) {
        float* aryptr = (float*) ptr;
        aryptr[index] = value;
    }
    //  double
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_double(unsigned int nelm) {
        return (HANDLE) new double[nelm];
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_double(HANDLE ptr) {
	if (ptr == NULL) return;
	delete[] ptr;
    }
    __declspec(dllexport) double __cdecl Spr_array_get_double(HANDLE ptr, int index) {
        double* aryptr = (double*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_double(HANDLE ptr, int index, double value) {
        double* aryptr = (double*) ptr;
        aryptr[index] = value;
    }
    //  string
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_string(unsigned int nelm) {
        return (HANDLE) new std::string[nelm];
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_string(HANDLE ptr, unsigned int nelm) {
	if (ptr == NULL) return;
        std::string* aryptr = (std::string*) ptr;
        std::string().swap(*aryptr);
    }
    __declspec(dllexport) HANDLE __cdecl Spr_array_get_string(HANDLE ptr, int index) {
        BSTR result = NULL;
        std::string* aryptr = (std::string*) ptr;
        std::string str = aryptr[index];
        int lenW = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
        if (lenW > 0) {
            result = ::SysAllocStringLen(0, lenW);
            ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, result, lenW);
        }
        return (HANDLE) result;
    }
    __declspec(dllexport) void __cdecl Spr_array_set_string(HANDLE ptr, int index, HANDLE value) {
        int lenMB = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, NULL, 0, NULL, NULL);
        if (lenMB > 0) {
            LPSTR addr = (LPSTR) ::SysAllocStringLen(0, lenMB);
            ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, addr, lenMB, NULL, NULL);
            int len = strlen(addr);
            char* strptr = new char[len+1];
	    strptr[len] = '\0';
            strcpy_s(strptr, len+1, addr);
            //SysFreeString((BSTR) addr);
            //char** aryptr = (char**) ptr;
            std::string* aryptr = (std::string*) ptr;
            aryptr[index] = std::string(strptr);
            SysFreeString((BSTR) addr);
        }
    }
    //  char*
    __declspec(dllexport) HANDLE __cdecl Spr_array_new_char_p(unsigned int nelm) {
        return (HANDLE) new char**[nelm];
    }
    __declspec(dllexport) void __cdecl Spr_array_delete_char_p(HANDLE ptr, unsigned int nelm) {
	if (ptr == NULL) return;
        char** aryptr = (char**) ptr;
        for (unsigned int i = 0; i < nelm; i++) {
		if (aryptr[i] != NULL) delete aryptr[i];
	}
        delete[] aryptr;
    }
    __declspec(dllexport) void __cdecl Spr_array_init_char_p(HANDLE ptr, unsigned int nelm) {
	if (ptr == NULL) return;
        char** aryptr = (char**) ptr;
        for (unsigned int i = 0; i < nelm; i++) { aryptr[i] = NULL; }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_array_get_char_p(HANDLE ptr, int index) {
        BSTR result = NULL;
        char** aryptr = (char**) ptr;
        char* cstr = aryptr[index];
        int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
        if (lenW > 0) {
            result = ::SysAllocStringLen(0, lenW);
            ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, result, lenW);
        }
        return (HANDLE) result;
    }
    __declspec(dllexport) void __cdecl Spr_array_set_char_p(HANDLE ptr, int index, HANDLE value) {
        int lenMB = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, NULL, 0, NULL, NULL);
        if (lenMB > 0) {
            LPSTR addr = (LPSTR) ::SysAllocStringLen(0, lenMB);
            ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, addr, lenMB, NULL, NULL);
            int len = strlen(addr);
            char* strptr = new char[len+1];
            strcpy_s(strptr, len+1, addr);
            SysFreeString((BSTR) addr);
            char** aryptr = (char**) ptr;
            aryptr[index] = strptr;
        }
    }
}

