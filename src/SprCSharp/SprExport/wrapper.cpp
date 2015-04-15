// wrapper.cpp
//
#include <Windows.h>
#include <vector>

using namespace std;

extern "C" {
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
    //  unsigned int
    __declspec(dllexport) unsigned int __cdecl Spr_vector_get_unsigned_int(HANDLE ptr, int index) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_unsigned_int(HANDLE ptr, int index, unsigned int value) {
        vector<unsigned int>* vecptr = (vector<unsigned int>*) ptr;
        (*vecptr)[index] = value;
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
    //  float
    __declspec(dllexport) float __cdecl Spr_vector_get_float(HANDLE ptr, int index) {
        vector<float>* vecptr = (vector<float>*) ptr;
        return (*vecptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_vector_set_float(HANDLE ptr, int index, float value) {
        vector<float>* vecptr = (vector<float>*) ptr;
        (*vecptr)[index] = value;
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

    // array
    //  int
    __declspec(dllexport) int __cdecl Spr_array_get_int(HANDLE ptr, int index) {
        int** aryptr = (int**) ptr;
        return (*aryptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_int(HANDLE ptr, int index, int value) {
        int** aryptr = (int**) ptr;
        (*aryptr)[index] = value;
    }
    //  float
    __declspec(dllexport) float __cdecl Spr_array_get_float(HANDLE ptr, int index) {
        float** aryptr = (float**) ptr;
        return (*aryptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_float(HANDLE ptr, int index, float value) {
        float** aryptr = (float**) ptr;
        (*aryptr)[index] = value;
    }
    //  double
    __declspec(dllexport) double __cdecl Spr_array_get_double(HANDLE ptr, int index) {
        double** aryptr = (double**) ptr;
        return (*aryptr)[index];
    }
    __declspec(dllexport) void __cdecl Spr_array_set_double(HANDLE ptr, int index, double value) {
        double** aryptr = (double**) ptr;
        (*aryptr)[index] = value;
    }
}

