// wrapper.cpp
//
#include <Springhead.h>
#include <Windows.h>
#include <vector>

using namespace std;
using namespace Spr;

extern "C" {
    // wrapper base class
    //  vectorwrapper
    //  arraywrapper
    __declspec(dllexport) HANDLE __cdecl Spr_arraywrapper_new(unsigned int size, unsigned int nelm) {
        return (HANDLE) new char[size * nelm];
    }

    // std::vector
    //  int
    __declspec(dllexport) int __cdecl Spr_vector_get_int(HANDLE ptr, int index) {
        vector<int>* vecptr = (vector<int>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_int(HANDLE ptr, int index, int value) {
        vector<int>* vecptr = (vector<int>*) ptr;
        (*vecptr)[index] = value;
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_int(HANDLE ptr) {
        vector<int>* vecptr = (vector<int>*) ptr;
        return (int) (*vecptr).size();
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_int(HANDLE ptr, int value) {
        vector<int>* vecptr = (vector<int>*) ptr;
        (*vecptr).push_back(value);
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_int(HANDLE ptr) {
        vector<int>* vecptr = (vector<int>*) ptr;
        (*vecptr).clear();
    }
    //  unsigned int
    __declspec(dllexport) unsigned int __cdecl Spr_vector_get_unsigned_int(HANDLE ptr, int index) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_unsigned_int(HANDLE ptr, int index, unsigned int value) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        (*vecptr)[index] = value;
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_unsigned_int(HANDLE ptr) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        return (int) (*vecptr).size();
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_unsigned_int(HANDLE ptr, unsigned int value) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        (*vecptr).push_back(value);
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_unsigned_int(HANDLE ptr) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        (*vecptr).clear();
    }
    //  size_t
    __declspec(dllexport) size_t __cdecl Spr_vector_get_size_t(HANDLE ptr, int index) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_size_t(HANDLE ptr, int index, size_t value) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        (*vecptr)[index] = value;
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_size_t(HANDLE ptr) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        return (int) (*vecptr).size();
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_size_t(HANDLE ptr, size_t value) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        (*vecptr).push_back(value);
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_size_t(HANDLE ptr) {
        vector<size_t>* vecptr = (vector<size_t>*) ptr;
        (*vecptr).clear();
    }
    //  float
    __declspec(dllexport) float __cdecl Spr_vector_get_float(HANDLE ptr, int index) {
        vector<float>* vecptr = (vector<float>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_float(HANDLE ptr, int index, float value) {
        vector<float>* vecptr = (vector<float>*) ptr;
        (*vecptr)[index] = value;
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_float(HANDLE ptr) {
        vector<float>* vecptr = (vector<float>*) ptr;
        return (int) (*vecptr).size();
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_float(HANDLE ptr, float value) {
        vector<float>* vecptr = (vector<float>*) ptr;
        (*vecptr).push_back(value);
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_float(HANDLE ptr) {
        vector<float>* vecptr = (vector<float>*) ptr;
        (*vecptr).clear();
    }
    //  double
    __declspec(dllexport) double __cdecl Spr_vector_get_double(HANDLE ptr, int index) {
        vector<double>* vecptr = (vector<double>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_double(HANDLE ptr, int index, double value) {
        vector<double>* vecptr = (vector<double>*) ptr;
        (*vecptr)[index] = value;
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_double(HANDLE ptr) {
        vector<double>* vecptr = (vector<double>*) ptr;
        return (int) (*vecptr).size();
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_double(HANDLE ptr, double value) {
        vector<double>* vecptr = (vector<double>*) ptr;
        (*vecptr).push_back(value);
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_double(HANDLE ptr) {
        vector<double>* vecptr = (vector<double>*) ptr;
        (*vecptr).clear();
    }
    //  string
    __declspec(dllexport) HANDLE __cdecl Spr_vector_get_string(HANDLE ptr, int index) {
        BSTR result = NULL;
        vector<string>* vecptr = (vector<string>*) ptr;
        const char* cstr = (*vecptr)[index].c_str();
        int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
        if (lenW > 0) {
            result = ::SysAllocStringLen(0, lenW);
            ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, result, lenW);
        }
        return (HANDLE) result;
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_string(HANDLE ptr, int index, HANDLE value) {
        int lenMB = ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, NULL, 0, NULL, NULL);
        if (lenMB > 0) {
            LPSTR addr = (LPSTR) ::SysAllocStringLen(0, lenMB);
            ::WideCharToMultiByte(CP_ACP, 0, (LPCWSTR) value, -1, addr, lenMB, NULL, NULL);
            vector<string>* vecptr = (vector<string>*) ptr;
            (*vecptr)[index] = string(addr);
        }
    }
    __declspec(dllexport) int __cdecl Spr_vector_size_string(HANDLE ptr) {
        vector<string>* vecptr = (vector<string>*) ptr;
        return (int) (*vecptr).size();
    }
    __declspec(dllexport) void __cdecl Spr_vector_push_back_string(HANDLE ptr, string value) {
        vector<string>* vecptr = (vector<string>*) ptr;
        (*vecptr).push_back(value);
    }
    __declspec(dllexport) void __cdecl Spr_vector_clear_string(HANDLE ptr) {
        vector<string>* vecptr = (vector<string>*) ptr;
        (*vecptr).clear();
    }

    // array
    //  int
    __declspec(dllexport) int __cdecl Spr_array_get_int(HANDLE ptr, int index) {
        int* aryptr = (int*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_int(HANDLE ptr, int index, int value) {
        int* aryptr = (int*) ptr;
        aryptr[index] = value;
    }
    //  float
    __declspec(dllexport) float __cdecl Spr_array_get_float(HANDLE ptr, int index) {
        float* aryptr = (float*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_float(HANDLE ptr, int index, float value) {
        float* aryptr = (float*) ptr;
        aryptr[index] = value;
    }
    //  double
    __declspec(dllexport) double __cdecl Spr_array_get_double(HANDLE ptr, int index) {
        double* aryptr = (double*) ptr;
        return aryptr[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_double(HANDLE ptr, int index, double value) {
        double* aryptr = (double*) ptr;
        aryptr[index] = value;
    }
    //  char*
    __declspec(dllexport) void __cdecl Spr_array_delete_char_p(HANDLE ptr, unsigned int nelm) {
	if (ptr == NULL) return;
        char** aryptr = (char**) ptr;
        for (unsigned int i = 0; i < nelm; i++) { if (aryptr[i] != NULL) delete aryptr[i]; }
        delete aryptr;
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

