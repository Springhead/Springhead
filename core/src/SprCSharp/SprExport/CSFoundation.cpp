#include <Springhead.h>
#include <Windows.h>
#include <sstream>
#include <SprFoundation.h>
#include <Framework/SprFWApp.h>
#include <Physics/SprPHJointMotor.h>
#include <Framework/SprFWEditor.h>
#include <Framework/SprFWOpObj.h>
#include <Framework/FWScene.h>
#include <../src/SprCSharp/SprExport/CSUtility.h>
using namespace Spr;
using namespace std;
extern "C" {
    __declspec(dllexport) HANDLE __cdecl Spr_new_ObjectIfs() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new ObjectIfs(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_ObjectIfs(HANDLE v) {
        try { delete (ObjectIfs*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIfs_Push(HANDLE _this, HANDLE obj) {
	try { ((ObjectIfs*) _this)->Push((ObjectIf*) obj); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIfs_Pop(HANDLE _this) {
	try { ((ObjectIfs*) _this)->Pop(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIfs_Top(HANDLE _this) {
	ObjectIf* _ptr = (ObjectIf*) 0;
	try { _ptr = (ObjectIf*) ((ObjectIfs*) _this)->Top(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTAccessBase_Create(HANDLE _this) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTAccessBase*) _this)->Create(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTAccessBase_Delete(HANDLE _this, HANDLE ptr) {
	try { ((UTAccessBase*) _this)->Delete(ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTAccessBase_VectorPush(HANDLE _this, HANDLE v) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTAccessBase*) _this)->VectorPush(v); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTAccessBase_VectorPop(HANDLE _this, HANDLE v) {
	try { ((UTAccessBase*) _this)->VectorPop(v); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTAccessBase_VectorAt(HANDLE _this, HANDLE v, int pos) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTAccessBase*) _this)->VectorAt(v, pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) size_t __cdecl Spr_UTAccessBase_VectorSize(HANDLE _this, HANDLE v) {
	size_t _val = (size_t) 0;
	try { _val = ((UTAccessBase*) _this)->VectorSize(v); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) size_t __cdecl Spr_UTAccessBase_SizeOfVector(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((UTAccessBase*) _this)->SizeOfVector(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec2f() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec2f_3(float x, float y) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f(x, y); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec2f(HANDLE v) {
        try { delete (Vec2f*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec2f(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f(-(*((Vec2f*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f((*((Vec2f*) ptr1)) + (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f((*((Vec2f*) ptr1)) - (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vfv_Vec2f(float a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f((a) * (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvf_Vec2f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f((*((Vec2f*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvf_Vec2f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2f((*((Vec2f*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) float __cdecl Spr_binary_operator_sprod_fvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	float _val = (float) 0;
	try { _val = (float) ((*((Vec2f*) ptr1)) * (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_binary_operator_vprod1_fvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	float _val = (float) 0;
	try { _val = (float) ((*((Vec2f*) ptr1)) % (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_binary_operator_vprod2_fvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	float _val = (float) 0;
	try { _val = (float) ((*((Vec2f*) ptr1)) ^ (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec2f*) ptr1)) == (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec2f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec2f*) ptr1)) != (*((Vec2f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_unary_operator_index_fi_get_Vec2f(HANDLE ptr, int i) {
	float _val = (float) 0;
	try { _val = (float) (*((Vec2f*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_fi_set_Vec2f(HANDLE ptr, int i, float value) {
	try { (*((Vec2f*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec2f_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec2f*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2f_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec2f_get_x(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec2f*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2f_set_x(HANDLE _this, float value) {
        try { (*((Vec2f*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec2f_get_y(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec2f*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2f_set_y(HANDLE _this, float value) {
        try { (*((Vec2f*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec2f_square(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec2f*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Vec2f_norm(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec2f*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2f_unitize(HANDLE _this) {
	try { ((Vec2f*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec2d() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec2d_3(double x, double y) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d(x, y); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec2d(HANDLE v) {
        try { delete (Vec2d*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec2d(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d(-(*((Vec2d*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d((*((Vec2d*) ptr1)) + (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d((*((Vec2d*) ptr1)) - (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vdv_Vec2d(double a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d((a) * (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvd_Vec2d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d((*((Vec2d*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvd_Vec2d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2d((*((Vec2d*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) double __cdecl Spr_binary_operator_sprod_dvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	double _val = (double) 0;
	try { _val = (double) ((*((Vec2d*) ptr1)) * (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_binary_operator_vprod1_dvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	double _val = (double) 0;
	try { _val = (double) ((*((Vec2d*) ptr1)) % (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_binary_operator_vprod2_dvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	double _val = (double) 0;
	try { _val = (double) ((*((Vec2d*) ptr1)) ^ (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec2d*) ptr1)) == (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec2d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec2d*) ptr1)) != (*((Vec2d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_unary_operator_index_di_get_Vec2d(HANDLE ptr, int i) {
	double _val = (double) 0;
	try { _val = (double) (*((Vec2d*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_di_set_Vec2d(HANDLE ptr, int i, double value) {
	try { (*((Vec2d*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec2d_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec2d*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2d_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec2d_get_x(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec2d*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2d_set_x(HANDLE _this, double value) {
        try { (*((Vec2d*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec2d_get_y(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec2d*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2d_set_y(HANDLE _this, double value) {
        try { (*((Vec2d*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec2d_square(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec2d*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Vec2d_norm(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec2d*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2d_unitize(HANDLE _this) {
	try { ((Vec2d*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec3f() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec3f_4(float x, float y, float z) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f(x, y, z); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec3f(HANDLE v) {
        try { delete (Vec3f*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec3f(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f(-(*((Vec3f*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) + (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) - (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vfv_Vec3f(float a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((a) * (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvf_Vec3f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvf_Vec3f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) float __cdecl Spr_binary_operator_sprod_fvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	float _val = (float) 0;
	try { _val = (float) ((*((Vec3f*) ptr1)) * (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_vprod1_vvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) % (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_vprod2_vvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) ^ (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec3f*) ptr1)) == (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec3f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec3f*) ptr1)) != (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_unary_operator_index_fi_get_Vec3f(HANDLE ptr, int i) {
	float _val = (float) 0;
	try { _val = (float) (*((Vec3f*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_fi_set_Vec3f(HANDLE ptr, int i, float value) {
	try { (*((Vec3f*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec3f_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec3f*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3f_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec3f_get_x(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec3f*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3f_set_x(HANDLE _this, float value) {
        try { (*((Vec3f*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec3f_get_y(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec3f*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3f_set_y(HANDLE _this, float value) {
        try { (*((Vec3f*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec3f_get_z(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec3f*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3f_set_z(HANDLE _this, float value) {
        try { (*((Vec3f*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec3f_square(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec3f*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Vec3f_norm(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec3f*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3f_unitize(HANDLE _this) {
	try { ((Vec3f*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec3d() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec3d_4(double x, double y, double z) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d(x, y, z); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec3d(HANDLE v) {
        try { delete (Vec3d*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec3d(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d(-(*((Vec3d*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) + (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) - (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vdv_Vec3d(double a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((a) * (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvd_Vec3d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvd_Vec3d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) double __cdecl Spr_binary_operator_sprod_dvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	double _val = (double) 0;
	try { _val = (double) ((*((Vec3d*) ptr1)) * (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_vprod1_vvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) % (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_vprod2_vvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) ^ (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec3d*) ptr1)) == (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec3d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec3d*) ptr1)) != (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_unary_operator_index_di_get_Vec3d(HANDLE ptr, int i) {
	double _val = (double) 0;
	try { _val = (double) (*((Vec3d*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_di_set_Vec3d(HANDLE ptr, int i, double value) {
	try { (*((Vec3d*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec3d_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec3d*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3d_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec3d_get_x(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec3d*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3d_set_x(HANDLE _this, double value) {
        try { (*((Vec3d*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec3d_get_y(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec3d*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3d_set_y(HANDLE _this, double value) {
        try { (*((Vec3d*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec3d_get_z(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec3d*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3d_set_z(HANDLE _this, double value) {
        try { (*((Vec3d*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec3d_square(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec3d*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Vec3d_norm(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec3d*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3d_unitize(HANDLE _this) {
	try { ((Vec3d*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec4f() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec4f_5(float x, float y, float z, float w) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f(x, y, z, w); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec4f(HANDLE v) {
        try { delete (Vec4f*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec4f(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f(-(*((Vec4f*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec4f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f((*((Vec4f*) ptr1)) + (*((Vec4f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec4f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f((*((Vec4f*) ptr1)) - (*((Vec4f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vfv_Vec4f(float a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f((a) * (*((Vec4f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvf_Vec4f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f((*((Vec4f*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvf_Vec4f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4f((*((Vec4f*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec4f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec4f*) ptr1)) == (*((Vec4f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec4f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec4f*) ptr1)) != (*((Vec4f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_unary_operator_index_fi_get_Vec4f(HANDLE ptr, int i) {
	float _val = (float) 0;
	try { _val = (float) (*((Vec4f*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_fi_set_Vec4f(HANDLE ptr, int i, float value) {
	try { (*((Vec4f*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec4f_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec4f*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4f_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec4f_get_x(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec4f*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4f_set_x(HANDLE _this, float value) {
        try { (*((Vec4f*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec4f_get_y(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec4f*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4f_set_y(HANDLE _this, float value) {
        try { (*((Vec4f*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec4f_get_z(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec4f*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4f_set_z(HANDLE _this, float value) {
        try { (*((Vec4f*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec4f_get_w(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec4f*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4f_set_w(HANDLE _this, float value) {
        try { (*((Vec4f*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec4f_square(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec4f*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Vec4f_norm(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec4f*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4f_unitize(HANDLE _this) {
	try { ((Vec4f*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec4d() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec4d_5(double x, double y, double z, double w) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d(x, y, z, w); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec4d(HANDLE v) {
        try { delete (Vec4d*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec4d(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d(-(*((Vec4d*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec4d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d((*((Vec4d*) ptr1)) + (*((Vec4d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec4d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d((*((Vec4d*) ptr1)) - (*((Vec4d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vdv_Vec4d(double a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d((a) * (*((Vec4d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvd_Vec4d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d((*((Vec4d*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvd_Vec4d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4d((*((Vec4d*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec4d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec4d*) ptr1)) == (*((Vec4d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec4d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec4d*) ptr1)) != (*((Vec4d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_unary_operator_index_di_get_Vec4d(HANDLE ptr, int i) {
	double _val = (double) 0;
	try { _val = (double) (*((Vec4d*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_di_set_Vec4d(HANDLE ptr, int i, double value) {
	try { (*((Vec4d*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec4d_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec4d*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4d_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec4d_get_x(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec4d*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4d_set_x(HANDLE _this, double value) {
        try { (*((Vec4d*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec4d_get_y(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec4d*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4d_set_y(HANDLE _this, double value) {
        try { (*((Vec4d*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec4d_get_z(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec4d*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4d_set_z(HANDLE _this, double value) {
        try { (*((Vec4d*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec4d_get_w(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec4d*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4d_set_w(HANDLE _this, double value) {
        try { (*((Vec4d*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec4d_square(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec4d*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Vec4d_norm(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec4d*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4d_unitize(HANDLE _this) {
	try { ((Vec4d*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Quaternionf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Quaternionf_5(float w, float x, float y, float z) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf(w, x, y, z); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Quaternionf(HANDLE v) {
        try { delete (Quaternionf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_qqq_Quaternionf(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf((*((Quaternionf*) ptr1)) + (*((Quaternionf*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_qqq_Quaternionf(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf((*((Quaternionf*) ptr1)) - (*((Quaternionf*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_qfq_Quaternionf(float a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf((a) * (*((Quaternionf*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_qqf_Quaternionf(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf((*((Quaternionf*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_qqf_Quaternionf(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf((*((Quaternionf*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vqv_Quaternionf(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Quaternionf*) ptr1)) * (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_qqq_Quaternionf(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaternionf((*((Quaternionf*) ptr1)) * (*((Quaternionf*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_mqm_Quaternionf(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f((*((Quaternionf*) ptr1)) * (*((Matrix3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Quaternionf*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_get_x(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Quaternionf*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_set_x(HANDLE _this, float value) {
        try { (*((Quaternionf*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_get_y(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Quaternionf*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_set_y(HANDLE _this, float value) {
        try { (*((Quaternionf*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_get_z(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Quaternionf*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_set_z(HANDLE _this, float value) {
        try { (*((Quaternionf*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_get_w(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Quaternionf*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_set_w(HANDLE _this, float value) {
        try { (*((Quaternionf*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_W(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Quaternionf*) _this)->W(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_X(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Quaternionf*) _this)->X(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_Y(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Quaternionf*) _this)->Y(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_Z(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Quaternionf*) _this)->Z(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_V(HANDLE _this) {
	Vec3f* _ptr = (Vec3f*) 0;
	try { _ptr = new Vec3f(); (*_ptr) = ((Quaternionf*) _this)->V(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_RotationHalf(HANDLE _this) {
	Vec3f* _ptr = (Vec3f*) 0;
	try { _ptr = new Vec3f(); (*_ptr) = ((Quaternionf*) _this)->RotationHalf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Rotation(HANDLE _this) {
	Vec3f* _ptr = (Vec3f*) 0;
	try { _ptr = new Vec3f(); (*_ptr) = ((Quaternionf*) _this)->Rotation(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Axis(HANDLE _this) {
	Vec3f* _ptr = (Vec3f*) 0;
	try { _ptr = new Vec3f(); (*_ptr) = ((Quaternionf*) _this)->Axis(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) float __cdecl Spr_Quaternionf_Theta(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Quaternionf*) _this)->Theta(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Rot(float angle, HANDLE axis) {
	Vec3f arg02_ = *((Vec3f*) axis);
	Quaternionf* _ptr = (Quaternionf*) 0;
	try { _ptr = new Quaternionf(); (*_ptr) = Quaternionf::Rot(angle, (Vec3f) arg02_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Rot_1(float angle, char axis) {
	Quaternionf* _ptr = (Quaternionf*) 0;
	try { _ptr = new Quaternionf(); (*_ptr) = Quaternionf::Rot(angle, axis); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Rot_2(HANDLE rot) {
	Quaternionf* _ptr = (Quaternionf*) 0;
	try { _ptr = new Quaternionf(); (*_ptr) = Quaternionf::Rot((Vec3f&) *((Vec3f*) rot)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Inv(HANDLE _this) {
	Quaternionf* _ptr = (Quaternionf*) 0;
	try { _ptr = new Quaternionf(); (*_ptr) = ((Quaternionf*) _this)->Inv(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_Conjugate(HANDLE _this) {
	try { ((Quaternionf*) _this)->Conjugate(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaternionf_Conjugated(HANDLE _this) {
	Quaternionf* _ptr = (Quaternionf*) 0;
	try { _ptr = new Quaternionf(); (*_ptr) = ((Quaternionf*) _this)->Conjugated(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_RotationArc(HANDLE _this, HANDLE lhs, HANDLE rhs) {
	try { ((Quaternionf*) _this)->RotationArc((Vec3f&) *((Vec3f*) lhs), (Vec3f&) *((Vec3f*) rhs)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_Quaternionf_Euler(HANDLE _this, float heading, float attitude, float bank) {
	try { ((Quaternionf*) _this)->Euler(heading, attitude, bank); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Quaterniond() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Quaterniond_5(double w, double x, double y, double z) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond(w, x, y, z); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Quaterniond(HANDLE v) {
        try { delete (Quaterniond*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_qqq_Quaterniond(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond((*((Quaterniond*) ptr1)) + (*((Quaterniond*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_qqq_Quaterniond(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond((*((Quaterniond*) ptr1)) - (*((Quaterniond*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_qdq_Quaterniond(double a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond((a) * (*((Quaterniond*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_qqd_Quaterniond(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond((*((Quaterniond*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_qqd_Quaterniond(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond((*((Quaterniond*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vqv_Quaterniond(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Quaterniond*) ptr1)) * (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_qqq_Quaterniond(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Quaterniond((*((Quaterniond*) ptr1)) * (*((Quaterniond*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_mqm_Quaterniond(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d((*((Quaterniond*) ptr1)) * (*((Matrix3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Quaterniond*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_get_x(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Quaterniond*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_set_x(HANDLE _this, double value) {
        try { (*((Quaterniond*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_get_y(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Quaterniond*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_set_y(HANDLE _this, double value) {
        try { (*((Quaterniond*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_get_z(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Quaterniond*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_set_z(HANDLE _this, double value) {
        try { (*((Quaterniond*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_get_w(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Quaterniond*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_set_w(HANDLE _this, double value) {
        try { (*((Quaterniond*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_W(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Quaterniond*) _this)->W(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_X(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Quaterniond*) _this)->X(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_Y(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Quaterniond*) _this)->Y(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_Z(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Quaterniond*) _this)->Z(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_V(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((Quaterniond*) _this)->V(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_RotationHalf(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((Quaterniond*) _this)->RotationHalf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Rotation(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((Quaterniond*) _this)->Rotation(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Axis(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((Quaterniond*) _this)->Axis(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) double __cdecl Spr_Quaterniond_Theta(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Quaterniond*) _this)->Theta(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Rot(double angle, HANDLE axis) {
	Vec3d arg02_ = *((Vec3d*) axis);
	Quaterniond* _ptr = (Quaterniond*) 0;
	try { _ptr = new Quaterniond(); (*_ptr) = Quaterniond::Rot(angle, (Vec3d) arg02_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Rot_1(double angle, char axis) {
	Quaterniond* _ptr = (Quaterniond*) 0;
	try { _ptr = new Quaterniond(); (*_ptr) = Quaterniond::Rot(angle, axis); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Rot_2(HANDLE rot) {
	Quaterniond* _ptr = (Quaterniond*) 0;
	try { _ptr = new Quaterniond(); (*_ptr) = Quaterniond::Rot((Vec3d&) *((Vec3d*) rot)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Inv(HANDLE _this) {
	Quaterniond* _ptr = (Quaterniond*) 0;
	try { _ptr = new Quaterniond(); (*_ptr) = ((Quaterniond*) _this)->Inv(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_Conjugate(HANDLE _this) {
	try { ((Quaterniond*) _this)->Conjugate(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Quaterniond_Conjugated(HANDLE _this) {
	Quaterniond* _ptr = (Quaterniond*) 0;
	try { _ptr = new Quaterniond(); (*_ptr) = ((Quaterniond*) _this)->Conjugated(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_RotationArc(HANDLE _this, HANDLE lhs, HANDLE rhs) {
	try { ((Quaterniond*) _this)->RotationArc((Vec3d&) *((Vec3d*) lhs), (Vec3d&) *((Vec3d*) rhs)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_Quaterniond_Euler(HANDLE _this, double heading, double attitude, double bank) {
	try { ((Quaterniond*) _this)->Euler(heading, attitude, bank); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Posef() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posef(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Posef_8(HANDLE p, HANDLE q) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posef(*((const Vec3f*) p), *((const Quaternionf*) q)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Posef_9(float w, float x, float y, float z, float px, float py, float pz) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posef(w, x, y, z, px, py, pz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Posef(HANDLE v) {
        try { delete (Posef*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vpv_Posef(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Posef*) ptr1)) * (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_ppp_Posef(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posef((*((Posef*) ptr1)) * (*((Posef*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Posef*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_w(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_w(HANDLE _this, float value) {
        try { (*((Posef*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_x(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_x(HANDLE _this, float value) {
        try { (*((Posef*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_y(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_y(HANDLE _this, float value) {
        try { (*((Posef*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_z(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_z(HANDLE _this, float value) {
        try { (*((Posef*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_px(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).px; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_px(HANDLE _this, float value) {
        try { (*((Posef*)_this)).px = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_py(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).py; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_py(HANDLE _this, float value) {
        try { (*((Posef*)_this)).py = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Posef_get_pz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Posef*)_this)).pz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posef_set_pz(HANDLE _this, float value) {
        try { (*((Posef*)_this)).pz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Pos(HANDLE _this) {
	Vec3f* _ptr = (Vec3f*) 0;
	try { _ptr = new Vec3f(); (*_ptr) = ((Posef*) _this)->Pos(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Ori(HANDLE _this) {
	Quaternionf* _ptr = (Quaternionf*) 0;
	try { _ptr = new Quaternionf(); (*_ptr) = ((Posef*) _this)->Ori(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_W(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->W(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_X(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->X(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_Y(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->Y(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_Z(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->Z(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_Px(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->Px(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_Py(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->Py(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_Pz(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->Pz(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_PosX(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->PosX(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_PosY(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->PosY(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_PosZ(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->PosZ(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_OriX(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->OriX(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_OriY(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->OriY(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_OriZ(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->OriZ(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Posef_OriW(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Posef*) _this)->OriW(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Inv(HANDLE _this) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = ((Posef*) _this)->Inv(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Unit() {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Unit(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Trn(float px, float py, float pz) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Trn(px, py, pz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Trn_1(HANDLE v) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Trn((Vec3f&) *((Vec3f*) v)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Rot(float wi, float xi, float yi, float zi) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Rot(wi, xi, yi, zi); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Rot_1(float angle, HANDLE axis) {
	Vec3f arg02_ = *((Vec3f*) axis);
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Rot(angle, (Vec3f) arg02_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Rot_2(float angle, char axis) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Rot(angle, axis); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Rot_3(HANDLE rot) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Rot((Vec3f&) *((Vec3f*) rot)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posef_Rot_4(HANDLE q) {
	Posef* _ptr = (Posef*) 0;
	try { _ptr = new Posef(); (*_ptr) = Posef::Rot((Quaternionf&) *((Quaternionf*) q)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Posed() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posed(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Posed_8(HANDLE p, HANDLE q) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posed(*((const Vec3d*) p), *((const Quaterniond*) q)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Posed_9(double w, double x, double y, double z, double px, double py, double pz) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posed(w, x, y, z, px, py, pz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Posed(HANDLE v) {
        try { delete (Posed*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vpv_Posed(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Posed*) ptr1)) * (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_ppp_Posed(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Posed((*((Posed*) ptr1)) * (*((Posed*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Posed*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_w(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_w(HANDLE _this, double value) {
        try { (*((Posed*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_x(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_x(HANDLE _this, double value) {
        try { (*((Posed*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_y(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_y(HANDLE _this, double value) {
        try { (*((Posed*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_z(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_z(HANDLE _this, double value) {
        try { (*((Posed*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_px(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).px; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_px(HANDLE _this, double value) {
        try { (*((Posed*)_this)).px = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_py(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).py; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_py(HANDLE _this, double value) {
        try { (*((Posed*)_this)).py = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Posed_get_pz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Posed*)_this)).pz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Posed_set_pz(HANDLE _this, double value) {
        try { (*((Posed*)_this)).pz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Pos(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((Posed*) _this)->Pos(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Ori(HANDLE _this) {
	Quaterniond* _ptr = (Quaterniond*) 0;
	try { _ptr = new Quaterniond(); (*_ptr) = ((Posed*) _this)->Ori(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_W(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->W(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_X(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->X(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_Y(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->Y(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_Z(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->Z(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_Px(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->Px(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_Py(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->Py(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_Pz(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->Pz(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_PosX(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->PosX(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_PosY(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->PosY(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_PosZ(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->PosZ(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_OriX(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->OriX(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_OriY(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->OriY(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_OriZ(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->OriZ(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Posed_OriW(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Posed*) _this)->OriW(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Inv(HANDLE _this) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = ((Posed*) _this)->Inv(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Unit() {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Unit(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Trn(double px, double py, double pz) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Trn(px, py, pz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Trn_1(HANDLE v) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Trn((Vec3f&) *((Vec3f*) v)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Rot(double wi, double xi, double yi, double zi) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Rot(wi, xi, yi, zi); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Rot_1(double angle, HANDLE axis) {
	Vec3f arg02_ = *((Vec3f*) axis);
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Rot(angle, (Vec3f) arg02_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Rot_2(double angle, char axis) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Rot(angle, axis); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Rot_3(HANDLE rot) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Rot((Vec3d&) *((Vec3d*) rot)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Posed_Rot_4(HANDLE q) {
	Posed* _ptr = (Posed*) 0;
	try { _ptr = new Posed(); (*_ptr) = Posed::Rot((Quaterniond&) *((Quaterniond*) q)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec2i() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec2i_3(int x, int y) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i(x, y); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec2i(HANDLE v) {
        try { delete (Vec2i*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec2i(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i(-(*((Vec2i*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i((*((Vec2i*) ptr1)) + (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i((*((Vec2i*) ptr1)) - (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_viv_Vec2i(int a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i((a) * (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvi_Vec2i(HANDLE ptr1, int b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i((*((Vec2i*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvi_Vec2i(HANDLE ptr1, int b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec2i((*((Vec2i*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) int __cdecl Spr_binary_operator_sprod_ivv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	int _val = (int) 0;
	try { _val = (int) ((*((Vec2i*) ptr1)) * (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_binary_operator_vprod1_ivv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	int _val = (int) 0;
	try { _val = (int) ((*((Vec2i*) ptr1)) % (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_binary_operator_vprod2_ivv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	int _val = (int) 0;
	try { _val = (int) ((*((Vec2i*) ptr1)) ^ (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec2i*) ptr1)) == (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec2i(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec2i*) ptr1)) != (*((Vec2i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_unary_operator_index_ii_get_Vec2i(HANDLE ptr, int i) {
	int _val = (int) 0;
	try { _val = (int) (*((Vec2i*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_ii_set_Vec2i(HANDLE ptr, int i, int value) {
	try { (*((Vec2i*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec2i_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec2i*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2i_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec2i_get_x(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec2i*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2i_set_x(HANDLE _this, int value) {
        try { (*((Vec2i*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec2i_get_y(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec2i*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2i_set_y(HANDLE _this, int value) {
        try { (*((Vec2i*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec2i_square(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((Vec2i*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_Vec2i_norm(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((Vec2i*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec2i_unitize(HANDLE _this) {
	try { ((Vec2i*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec3i() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec3i_4(int x, int y, int z) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i(x, y, z); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec3i(HANDLE v) {
        try { delete (Vec3i*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec3i(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i(-(*((Vec3i*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((*((Vec3i*) ptr1)) + (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((*((Vec3i*) ptr1)) - (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_viv_Vec3i(int a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((a) * (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvi_Vec3i(HANDLE ptr1, int b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((*((Vec3i*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvi_Vec3i(HANDLE ptr1, int b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((*((Vec3i*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) int __cdecl Spr_binary_operator_sprod_ivv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	int _val = (int) 0;
	try { _val = (int) ((*((Vec3i*) ptr1)) * (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_vprod1_vvv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((*((Vec3i*) ptr1)) % (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_vprod2_vvv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3i((*((Vec3i*) ptr1)) ^ (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec3i*) ptr1)) == (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec3i(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec3i*) ptr1)) != (*((Vec3i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_unary_operator_index_ii_get_Vec3i(HANDLE ptr, int i) {
	int _val = (int) 0;
	try { _val = (int) (*((Vec3i*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_ii_set_Vec3i(HANDLE ptr, int i, int value) {
	try { (*((Vec3i*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec3i_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec3i*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3i_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec3i_get_x(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec3i*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3i_set_x(HANDLE _this, int value) {
        try { (*((Vec3i*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec3i_get_y(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec3i*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3i_set_y(HANDLE _this, int value) {
        try { (*((Vec3i*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec3i_get_z(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec3i*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3i_set_z(HANDLE _this, int value) {
        try { (*((Vec3i*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec3i_square(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((Vec3i*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_Vec3i_norm(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((Vec3i*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec3i_unitize(HANDLE _this) {
	try { ((Vec3i*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec4i() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec4i_5(int x, int y, int z, int w) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i(x, y, z, w); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec4i(HANDLE v) {
        try { delete (Vec4i*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec4i(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i(-(*((Vec4i*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec4i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i((*((Vec4i*) ptr1)) + (*((Vec4i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec4i(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i((*((Vec4i*) ptr1)) - (*((Vec4i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_viv_Vec4i(int a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i((a) * (*((Vec4i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvi_Vec4i(HANDLE ptr1, int b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i((*((Vec4i*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvi_Vec4i(HANDLE ptr1, int b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec4i((*((Vec4i*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec4i(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec4i*) ptr1)) == (*((Vec4i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec4i(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec4i*) ptr1)) != (*((Vec4i*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_unary_operator_index_ii_get_Vec4i(HANDLE ptr, int i) {
	int _val = (int) 0;
	try { _val = (int) (*((Vec4i*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_ii_set_Vec4i(HANDLE ptr, int i, int value) {
	try { (*((Vec4i*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec4i_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec4i*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4i_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec4i_get_x(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec4i*)_this)).x; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4i_set_x(HANDLE _this, int value) {
        try { (*((Vec4i*)_this)).x = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec4i_get_y(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec4i*)_this)).y; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4i_set_y(HANDLE _this, int value) {
        try { (*((Vec4i*)_this)).y = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec4i_get_z(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec4i*)_this)).z; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4i_set_z(HANDLE _this, int value) {
        try { (*((Vec4i*)_this)).z = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec4i_get_w(HANDLE _this) {
        int _val = 0;
        try { _val = (*((Vec4i*)_this)).w; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4i_set_w(HANDLE _this, int value) {
        try { (*((Vec4i*)_this)).w = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_Vec4i_square(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((Vec4i*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_Vec4i_norm(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((Vec4i*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec4i_unitize(HANDLE _this) {
	try { ((Vec4i*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec6f() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec6f_7(float vx, float vy, float vz, float wx, float wy, float wz) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f(vx, vy, vz, wx, wy, wz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec6f(HANDLE v) {
        try { delete (Vec6f*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec6f(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f(-(*((Vec6f*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec6f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f((*((Vec6f*) ptr1)) + (*((Vec6f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec6f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f((*((Vec6f*) ptr1)) - (*((Vec6f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vfv_Vec6f(float a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f((a) * (*((Vec6f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvf_Vec6f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f((*((Vec6f*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvf_Vec6f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6f((*((Vec6f*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec6f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec6f*) ptr1)) == (*((Vec6f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec6f(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec6f*) ptr1)) != (*((Vec6f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_unary_operator_index_fi_get_Vec6f(HANDLE ptr, int i) {
	float _val = (float) 0;
	try { _val = (float) (*((Vec6f*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_fi_set_Vec6f(HANDLE ptr, int i, float value) {
	try { (*((Vec6f*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec6f_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec6f*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_get_vx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec6f*)_this)).vx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_set_vx(HANDLE _this, float value) {
        try { (*((Vec6f*)_this)).vx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_get_vy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec6f*)_this)).vy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_set_vy(HANDLE _this, float value) {
        try { (*((Vec6f*)_this)).vy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_get_vz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec6f*)_this)).vz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_set_vz(HANDLE _this, float value) {
        try { (*((Vec6f*)_this)).vz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_get_wx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec6f*)_this)).wx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_set_wx(HANDLE _this, float value) {
        try { (*((Vec6f*)_this)).wx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_get_wy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec6f*)_this)).wy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_set_wy(HANDLE _this, float value) {
        try { (*((Vec6f*)_this)).wy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_get_wz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Vec6f*)_this)).wz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_set_wz(HANDLE _this, float value) {
        try { (*((Vec6f*)_this)).wz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_square(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec6f*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) float __cdecl Spr_Vec6f_norm(HANDLE _this) {
	float _val = (float) 0;
	try { _val = ((Vec6f*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6f_unitize(HANDLE _this) {
	try { ((Vec6f*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec6d() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Vec6d_7(double vx, double vy, double vz, double wx, double wy, double wz) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d(vx, vy, vz, wx, wy, wz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Vec6d(HANDLE v) {
        try { delete (Vec6d*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_vv_Vec6d(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d(-(*((Vec6d*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_vvv_Vec6d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d((*((Vec6d*) ptr1)) + (*((Vec6d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_vvv_Vec6d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d((*((Vec6d*) ptr1)) - (*((Vec6d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vdv_Vec6d(double a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d((a) * (*((Vec6d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_vvd_Vec6d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d((*((Vec6d*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_div_vvd_Vec6d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec6d((*((Vec6d*) ptr1)) / (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_eq_bvv_Vec6d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec6d*) ptr1)) == (*((Vec6d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_binary_operator_ne_bvv_Vec6d(HANDLE ptr1, HANDLE ptr2) {
	char _val = (char) 0;
	try { _val = (char) ((*((Vec6d*) ptr1)) != (*((Vec6d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_unary_operator_index_di_get_Vec6d(HANDLE ptr, int i) {
	double _val = (double) 0;
	try { _val = (double) (*((Vec6d*) ptr))[i]; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_di_set_Vec6d(HANDLE ptr, int i, double value) {
	try { (*((Vec6d*) ptr))[i] = value; }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Vec6d_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Vec6d*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_get_vx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec6d*)_this)).vx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_set_vx(HANDLE _this, double value) {
        try { (*((Vec6d*)_this)).vx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_get_vy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec6d*)_this)).vy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_set_vy(HANDLE _this, double value) {
        try { (*((Vec6d*)_this)).vy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_get_vz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec6d*)_this)).vz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_set_vz(HANDLE _this, double value) {
        try { (*((Vec6d*)_this)).vz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_get_wx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec6d*)_this)).wx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_set_wx(HANDLE _this, double value) {
        try { (*((Vec6d*)_this)).wx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_get_wy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec6d*)_this)).wy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_set_wy(HANDLE _this, double value) {
        try { (*((Vec6d*)_this)).wy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_get_wz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Vec6d*)_this)).wz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_set_wz(HANDLE _this, double value) {
        try { (*((Vec6d*)_this)).wz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_square(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec6d*) _this)->square(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) double __cdecl Spr_Vec6d_norm(HANDLE _this) {
	double _val = (double) 0;
	try { _val = ((Vec6d*) _this)->norm(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Vec6d_unitize(HANDLE _this) {
	try { ((Vec6d*) _this)->unitize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Curve3f() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Curve3f(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Curve3f(HANDLE v) {
        try { delete (Curve3f*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Matrix3f() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Matrix3f_10(HANDLE ex, HANDLE ey, HANDLE ez) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f(*((Vec3f*) ex), *((Vec3f*) ey), *((Vec3f*) ez)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Matrix3f_11(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f(m11, m12, m13, m21, m22, m23, m31, m32, m33); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Matrix3f(HANDLE v) {
        try { delete (Matrix3f*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_mm_Matrix3f(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f(-(*((Matrix3f*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_mmm_Matrix3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f((*((Matrix3f*) ptr1)) + (*((Matrix3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_mmm_Matrix3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f((*((Matrix3f*) ptr1)) - (*((Matrix3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_mfm_Matrix3f(float a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f((a) * (*((Matrix3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_mmf_Matrix3f(HANDLE ptr1, float b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3f((*((Matrix3f*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vmv_Matrix3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Matrix3f*) ptr1)) * (*((Vec3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vvm_Matrix3f(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Vec3f*) ptr1)) * (*((Matrix3f*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_index_vi_get_Matrix3f(HANDLE ptr, int i) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3f((*((Matrix3f*) ptr))[i]); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_vi_set_Matrix3f(HANDLE ptr1, int i, HANDLE ptr2) {
	try { (*((Matrix3f*) ptr1))[i] = *((Vec3f*) ptr2); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Matrix3f_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Matrix3f*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_xx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).xx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_xx(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).xx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_xy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).xy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_xy(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).xy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_xz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).xz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_xz(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).xz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_yx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).yx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_yx(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).yx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_yy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).yy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_yy(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).yy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_yz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).yz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_yz(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).yz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_zx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).zx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_zx(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).zx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_zy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).zy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_zy(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).zy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Matrix3f_get_zz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Matrix3f*)_this)).zz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_set_zz(HANDLE _this, float value) {
        try { (*((Matrix3f*)_this)).zz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_Matrix3f_height(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((Matrix3f*) _this)->height(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) size_t __cdecl Spr_Matrix3f_width(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((Matrix3f*) _this)->width(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_resize(HANDLE _this, size_t h, size_t w) {
	try { ((Matrix3f*) _this)->resize(h, w); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_Matrix3f_stride(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((Matrix3f*) _this)->stride(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3f_clear(HANDLE _this) {
	try { ((Matrix3f*) _this)->clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Matrix3d() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Matrix3d_10(HANDLE ex, HANDLE ey, HANDLE ez) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d(*((Vec3d*) ex), *((Vec3d*) ey), *((Vec3d*) ez)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Matrix3d_11(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d(m11, m12, m13, m21, m22, m23, m31, m32, m33); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Matrix3d(HANDLE v) {
        try { delete (Matrix3d*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_neg_mm_Matrix3d(HANDLE ptr1) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d(-(*((Matrix3d*) ptr1))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_add_mmm_Matrix3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d((*((Matrix3d*) ptr1)) + (*((Matrix3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sub_mmm_Matrix3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d((*((Matrix3d*) ptr1)) - (*((Matrix3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_mdm_Matrix3d(double a, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d((a) * (*((Matrix3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_mul_mmd_Matrix3d(HANDLE ptr1, double b) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Matrix3d((*((Matrix3d*) ptr1)) * (b)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vmv_Matrix3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Matrix3d*) ptr1)) * (*((Vec3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_binary_operator_sprod_vvm_Matrix3d(HANDLE ptr1, HANDLE ptr2) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Vec3d*) ptr1)) * (*((Matrix3d*) ptr2))); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_unary_operator_index_vi_get_Matrix3d(HANDLE ptr, int i) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Vec3d((*((Matrix3d*) ptr))[i]); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_unary_operator_index_vi_set_Matrix3d(HANDLE ptr1, int i, HANDLE ptr2) {
	try { (*((Matrix3d*) ptr1))[i] = *((Vec3d*) ptr2); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Matrix3d_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Matrix3d*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_xx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).xx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_xx(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).xx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_xy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).xy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_xy(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).xy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_xz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).xz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_xz(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).xz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_yx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).yx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_yx(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).yx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_yy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).yy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_yy(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).yy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_yz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).yz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_yz(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).yz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_zx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).zx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_zx(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).zx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_zy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).zy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_zy(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).zy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Matrix3d_get_zz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Matrix3d*)_this)).zz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_set_zz(HANDLE _this, double value) {
        try { (*((Matrix3d*)_this)).zz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_Matrix3d_height(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((Matrix3d*) _this)->height(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) size_t __cdecl Spr_Matrix3d_width(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((Matrix3d*) _this)->width(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_resize(HANDLE _this, size_t h, size_t w) {
	try { ((Matrix3d*) _this)->resize(h, w); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_Matrix3d_stride(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((Matrix3d*) _this)->stride(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Matrix3d_clear(HANDLE _this) {
	try { ((Matrix3d*) _this)->clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Affinef() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Affinef(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Affinef_17(HANDLE a) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Affinef(*((Affinef*) a)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Affinef(HANDLE v) {
        try { delete (Affinef*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Affinef_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Affinef*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_xx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).xx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_xx(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).xx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_xy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).xy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_xy(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).xy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_xz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).xz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_xz(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).xz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_xw(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).xw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_xw(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).xw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_yx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).yx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_yx(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).yx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_yy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).yy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_yy(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).yy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_yz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).yz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_yz(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).yz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_yw(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).yw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_yw(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).yw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_zx(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).zx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_zx(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).zx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_zy(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).zy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_zy(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).zy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_zz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).zz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_zz(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).zz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_zw(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).zw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_zw(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).zw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_px(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).px; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_px(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).px = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_py(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).py; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_py(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).py = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_pz(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).pz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_pz(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).pz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) float __cdecl Spr_Affinef_get_pw(HANDLE _this) {
        float _val = 0;
        try { _val = (*((Affinef*)_this)).pw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affinef_set_pw(HANDLE _this, float value) {
        try { (*((Affinef*)_this)).pw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Affined() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Affined(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_Affined_17(HANDLE a) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new Affined(*((Affined*) a)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_Affined(HANDLE v) {
        try { delete (Affined*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_Affined_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            oss << (*((Affined*) _this));
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_xx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).xx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_xx(HANDLE _this, double value) {
        try { (*((Affined*)_this)).xx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_xy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).xy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_xy(HANDLE _this, double value) {
        try { (*((Affined*)_this)).xy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_xz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).xz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_xz(HANDLE _this, double value) {
        try { (*((Affined*)_this)).xz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_xw(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).xw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_xw(HANDLE _this, double value) {
        try { (*((Affined*)_this)).xw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_yx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).yx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_yx(HANDLE _this, double value) {
        try { (*((Affined*)_this)).yx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_yy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).yy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_yy(HANDLE _this, double value) {
        try { (*((Affined*)_this)).yy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_yz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).yz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_yz(HANDLE _this, double value) {
        try { (*((Affined*)_this)).yz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_yw(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).yw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_yw(HANDLE _this, double value) {
        try { (*((Affined*)_this)).yw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_zx(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).zx; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_zx(HANDLE _this, double value) {
        try { (*((Affined*)_this)).zx = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_zy(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).zy; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_zy(HANDLE _this, double value) {
        try { (*((Affined*)_this)).zy = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_zz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).zz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_zz(HANDLE _this, double value) {
        try { (*((Affined*)_this)).zz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_zw(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).zw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_zw(HANDLE _this, double value) {
        try { (*((Affined*)_this)).zw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_px(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).px; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_px(HANDLE _this, double value) {
        try { (*((Affined*)_this)).px = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_py(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).py; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_py(HANDLE _this, double value) {
        try { (*((Affined*)_this)).py = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_pz(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).pz; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_pz(HANDLE _this, double value) {
        try { (*((Affined*)_this)).pz = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_Affined_get_pw(HANDLE _this) {
        double _val = 0;
        try { _val = (*((Affined*)_this)).pw; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return _val;
    }
    __declspec(dllexport) void __cdecl Spr_Affined_set_pw(HANDLE _this, double value) {
        try { (*((Affined*)_this)).pw = value; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_SpatialVector() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new SpatialVector(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_SpatialVector_2(HANDLE _v, HANDLE _w) {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new SpatialVector(((const Vec3d&) _v), ((const Vec3d&) _w)); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_SpatialVector(HANDLE v) {
        try { delete (SpatialVector*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SpatialVector_v(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((SpatialVector*) _this)->v(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SpatialVector_w(HANDLE _this) {
	Vec3d* _ptr = (Vec3d*) 0;
	try { _ptr = new Vec3d(); (*_ptr) = ((SpatialVector*) _this)->w(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_ObjectIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new ObjectIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_ObjectIf(HANDLE v) {
        try { delete (ObjectIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            (*((ObjectIf*) _this)).Print(oss);
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((ObjectIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ObjectIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) int __cdecl Spr_ObjectIf_AddRef(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((ObjectIf*) _this)->AddRef(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_ObjectIf_RefCount(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((ObjectIf*) _this)->RefCount(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) size_t __cdecl Spr_ObjectIf_NChildObject(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((ObjectIf*) _this)->NChildObject(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_GetChildObject(HANDLE _this, size_t pos) {
	ObjectIf* _ptr = (ObjectIf*) 0;
	try { _ptr = (ObjectIf*) ((ObjectIf*) _this)->GetChildObject(pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_ObjectIf_AddChildObject(HANDLE _this, HANDLE o) {
	bool _val = (bool) 0;
	try { _val = ((ObjectIf*) _this)->AddChildObject((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_ObjectIf_DelChildObject(HANDLE _this, HANDLE o) {
	bool _val = (bool) 0;
	try { _val = ((ObjectIf*) _this)->DelChildObject((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIf_Clear(HANDLE _this) {
	try { ((ObjectIf*) _this)->Clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_CreateObject(HANDLE _this, HANDLE info, HANDLE desc) {
	ObjectIf* _ptr = (ObjectIf*) 0;
	try { _ptr = (ObjectIf*) ((ObjectIf*) _this)->CreateObject((const IfInfo*) info, desc); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_GetDescAddress(HANDLE _this) {
	const void* _ptr = (const void*) 0;
	try { _ptr = (void*)((ObjectIf*) _this)->GetDescAddress(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_ObjectIf_GetDesc(HANDLE _this, HANDLE desc) {
	bool _val = (bool) 0;
	try { _val = ((ObjectIf*) _this)->GetDesc(desc); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIf_SetDesc(HANDLE _this, HANDLE desc) {
	try { ((ObjectIf*) _this)->SetDesc(desc); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_ObjectIf_GetDescSize(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((ObjectIf*) _this)->GetDescSize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectIf_GetStateAddress(HANDLE _this) {
	const void* _ptr = (const void*) 0;
	try { _ptr = (void*)((ObjectIf*) _this)->GetStateAddress(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_ObjectIf_GetState(HANDLE _this, HANDLE state) {
	bool _val = (bool) 0;
	try { _val = ((ObjectIf*) _this)->GetState(state); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIf_SetState(HANDLE _this, HANDLE state) {
	try { ((ObjectIf*) _this)->SetState(state); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) char __cdecl Spr_ObjectIf_WriteState(HANDLE _this, BSTR fileName) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) fileName, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) fileName, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	bool _val = (bool) 0;
	try { _val = ((ObjectIf*) _this)->WriteState((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_ObjectIf_ReadState(HANDLE _this, BSTR fileName) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) fileName, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) fileName, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	bool _val = (bool) 0;
	try { _val = ((ObjectIf*) _this)->ReadState((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) size_t __cdecl Spr_ObjectIf_GetStateSize(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((ObjectIf*) _this)->GetStateSize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIf_ConstructState(HANDLE _this, HANDLE m) {
	try { ((ObjectIf*) _this)->ConstructState(m); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectIf_DestructState(HANDLE _this, HANDLE m) {
	try { ((ObjectIf*) _this)->DestructState(m); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_NamedObjectIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new NamedObjectIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_NamedObjectIf(HANDLE v) {
        try { delete (NamedObjectIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NamedObjectIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((NamedObjectIf*) _this)) {
                (*((NamedObjectIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_NamedObjectIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NamedObjectIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((NamedObjectIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NamedObjectIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) NamedObjectIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NamedObjectIf_GetName(HANDLE _this) {
	const char* _ptr = (const char*) 0;
	try { _ptr = ((NamedObjectIf*) _this)->GetName(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_NamedObjectIf_SetName(HANDLE _this, HANDLE n) {
	try { ((NamedObjectIf*) _this)->SetName((const char*) n); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NamedObjectIf_GetNameManager(HANDLE _this) {
	NameManagerIf* _ptr = (NameManagerIf*) 0;
	try { _ptr = (NameManagerIf*) ((NamedObjectIf*) _this)->GetNameManager(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_SceneObjectIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new SceneObjectIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_SceneObjectIf(HANDLE v) {
        try { delete (SceneObjectIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneObjectIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((SceneObjectIf*) _this)) {
                (*((SceneObjectIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_SceneObjectIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneObjectIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((SceneObjectIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneObjectIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) SceneObjectIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneObjectIf_GetScene(HANDLE _this) {
	SceneIf* _ptr = (SceneIf*) 0;
	try { _ptr = (SceneIf*) ((SceneObjectIf*) _this)->GetScene(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneObjectIf_CloneObject(HANDLE _this) {
	SceneObjectIf* _ptr = (SceneObjectIf*) 0;
	try { _ptr = (SceneObjectIf*) ((SceneObjectIf*) _this)->CloneObject(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_ObjectStatesIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new ObjectStatesIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_ObjectStatesIf(HANDLE v) {
        try { delete (ObjectStatesIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectStatesIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((ObjectStatesIf*) _this)) {
                (*((ObjectStatesIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectStatesIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((ObjectStatesIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectStatesIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ObjectStatesIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_AllocateState(HANDLE _this, HANDLE o) {
	try { ((ObjectStatesIf*) _this)->AllocateState((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_ReleaseState(HANDLE _this, HANDLE o) {
	try { ((ObjectStatesIf*) _this)->ReleaseState((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_ObjectStatesIf_CalcStateSize(HANDLE _this, HANDLE o) {
	size_t _val = (size_t) 0;
	try { _val = ((ObjectStatesIf*) _this)->CalcStateSize((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_SaveState(HANDLE _this, HANDLE o) {
	try { ((ObjectStatesIf*) _this)->SaveState((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_LoadState(HANDLE _this, HANDLE o) {
	try { ((ObjectStatesIf*) _this)->LoadState((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_SingleSave(HANDLE _this, HANDLE o) {
	try { ((ObjectStatesIf*) _this)->SingleSave((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_ObjectStatesIf_SingleLoad(HANDLE _this, HANDLE o) {
	try { ((ObjectStatesIf*) _this)->SingleLoad((ObjectIf*) o); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) char __cdecl Spr_ObjectStatesIf_IsAllocated(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((ObjectStatesIf*) _this)->IsAllocated(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_ObjectStatesIf_Create() {
	ObjectStatesIf* _ptr = (ObjectStatesIf*) 0;
	try { _ptr = (ObjectStatesIf*) ObjectStatesIf::Create(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_UTTypeDescIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new UTTypeDescIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_UTTypeDescIf(HANDLE v) {
        try { delete (UTTypeDescIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            (*((UTTypeDescIf*) _this)).Print(oss);
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((UTTypeDescIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) UTTypeDescIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_FindTypeDesc(HANDLE typeName, HANDLE moduleName) {
	UTTypeDescIf* _ptr = (UTTypeDescIf*) 0;
	try { _ptr = (UTTypeDescIf*) UTTypeDescIf::FindTypeDesc((const char*) typeName, (const char*) moduleName); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_Create(BSTR tn, int sz) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	UTTypeDescIf* _ptr = (UTTypeDescIf*) 0;
	try { _ptr = (UTTypeDescIf*) UTTypeDescIf::Create((string) arg01_, sz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_Create_1(BSTR tn) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	UTTypeDescIf* _ptr = (UTTypeDescIf*) 0;
	try { _ptr = (UTTypeDescIf*) UTTypeDescIf::Create((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_SetSize(HANDLE _this, size_t sz) {
	try { ((UTTypeDescIf*) _this)->SetSize(sz); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_SetAccess(HANDLE _this, HANDLE a) {
	try { ((UTTypeDescIf*) _this)->SetAccess((UTAccessBase*) a); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_SetOffset(HANDLE _this, int field, int offset) {
	try { ((UTTypeDescIf*) _this)->SetOffset(field, offset); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_SetIfInfo(HANDLE _this, HANDLE info) {
	try { ((UTTypeDescIf*) _this)->SetIfInfo((const IfInfo*) info); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) int __cdecl Spr_UTTypeDescIf_AddField(HANDLE _this, BSTR pre, BSTR ty, BSTR n, BSTR post) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) pre, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) pre, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	string arg02_("");
	int lenMB2 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) ty, -1, NULL, 0, NULL, NULL);
	if (lenMB2 > 0) {
	    LPSTR addr2 = (LPSTR) ::SysAllocStringLen(0, lenMB2);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) ty, -1, addr2, lenMB2, NULL, NULL);
	    arg02_ = (string) addr2;
	}
	string arg03_("");
	int lenMB3 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) n, -1, NULL, 0, NULL, NULL);
	if (lenMB3 > 0) {
	    LPSTR addr3 = (LPSTR) ::SysAllocStringLen(0, lenMB3);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) n, -1, addr3, lenMB3, NULL, NULL);
	    arg03_ = (string) addr3;
	}
	string arg04_("");
	int lenMB4 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) post, -1, NULL, 0, NULL, NULL);
	if (lenMB4 > 0) {
	    LPSTR addr4 = (LPSTR) ::SysAllocStringLen(0, lenMB4);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) post, -1, addr4, lenMB4, NULL, NULL);
	    arg04_ = (string) addr4;
	}
	int _val = (int) 0;
	try { _val = ((UTTypeDescIf*) _this)->AddField((string) arg01_, (string) arg02_, (string) arg03_, (string) arg04_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_UTTypeDescIf_AddBase(HANDLE _this, BSTR tn) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	int _val = (int) 0;
	try { _val = ((UTTypeDescIf*) _this)->AddBase((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_AddEnumConst(HANDLE _this, int field, BSTR name, int val) {
	string arg02_("");
	int lenMB2 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) name, -1, NULL, 0, NULL, NULL);
	if (lenMB2 > 0) {
	    LPSTR addr2 = (LPSTR) ::SysAllocStringLen(0, lenMB2);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) name, -1, addr2, lenMB2, NULL, NULL);
	    arg02_ = (string) addr2;
	}
	try { ((UTTypeDescIf*) _this)->AddEnumConst(field, (string) arg02_, val); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) BSTR __cdecl Spr_UTTypeDescIf_GetTypeName(HANDLE _this) {
	string _val = (string) 0;
	try { _val = ((UTTypeDescIf*) _this)->GetTypeName(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	BSTR wstr = NULL;
	const char* cstr = _val.c_str();
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
	if (lenW > 0) {
	    wstr = ::SysAllocStringLen(0, lenW);
	    ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, wstr, lenW);
	}
	return (BSTR) wstr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_FreeString_GetTypeName(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_SetTypeName(HANDLE _this, HANDLE s) {
	try { ((UTTypeDescIf*) _this)->SetTypeName((const char*) s); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) size_t __cdecl Spr_UTTypeDescIf_GetSize(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((UTTypeDescIf*) _this)->GetSize(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_IsPrimitive(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->IsPrimitive(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_IsSimple(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->IsSimple(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) int __cdecl Spr_UTTypeDescIf_NFields(HANDLE _this) {
	int _val = (int) 0;
	try { _val = ((UTTypeDescIf*) _this)->NFields(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_UTTypeDescIf_GetFieldLength(HANDLE _this, int i) {
	int _val = (int) 0;
	try { _val = ((UTTypeDescIf*) _this)->GetFieldLength(i); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) int __cdecl Spr_UTTypeDescIf_GetFieldVectorSize(HANDLE _this, int i, HANDLE base) {
	int _val = (int) 0;
	try { _val = ((UTTypeDescIf*) _this)->GetFieldVectorSize(i, base); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetFieldLengthName(HANDLE _this, int i) {
	const char* _ptr = (const char*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->GetFieldLengthName(i); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) int __cdecl Spr_UTTypeDescIf_GetFieldVarType(HANDLE _this, int i) {
	UTTypeDescIf::FieldType _val = (UTTypeDescIf::FieldType) 0;
	try { _val = ((UTTypeDescIf*) _this)->GetFieldVarType(i); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (int) _val;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_GetFieldIsReference(HANDLE _this, int i) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->GetFieldIsReference(i); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetFieldName(HANDLE _this, int i) {
	const char* _ptr = (const char*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->GetFieldName(i); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetFieldAddress(HANDLE _this, int i, HANDLE base, int pos) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->GetFieldAddress(i, base, pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetFieldAddressEx(HANDLE _this, int i, HANDLE base, int pos) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->GetFieldAddressEx(i, base, pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) BSTR __cdecl Spr_UTTypeDescIf_ReadToString(HANDLE _this, int i, HANDLE base, int pos) {
	string _val = (string) 0;
	try { _val = ((UTTypeDescIf*) _this)->ReadToString(i, base, pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	BSTR wstr = NULL;
	const char* cstr = _val.c_str();
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
	if (lenW > 0) {
	    wstr = ::SysAllocStringLen(0, lenW);
	    ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, wstr, lenW);
	}
	return (BSTR) wstr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_FreeString_ReadToString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_WriteFromString(HANDLE _this, BSTR from, int i, HANDLE base, int pos) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) from, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) from, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	try { ((UTTypeDescIf*) _this)->WriteFromString((string) arg01_, i, base, pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_GetIfInfoOfType(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((UTTypeDescIf*) _this)->GetIfInfoOfType(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_IsBool(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->IsBool(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_IsNumber(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->IsNumber(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_IsString(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->IsString(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescIf_ReadBool(HANDLE _this, HANDLE ptr) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescIf*) _this)->ReadBool(ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_WriteBool(HANDLE _this, char val, HANDLE ptr) {
	bool arg01_ = (val == 0) ? false : true;
	try { ((UTTypeDescIf*) _this)->WriteBool((bool) arg01_, ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) double __cdecl Spr_UTTypeDescIf_ReadNumber(HANDLE _this, HANDLE ptr) {
	double _val = (double) 0;
	try { _val = ((UTTypeDescIf*) _this)->ReadNumber(ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_WriteNumber(HANDLE _this, double val, HANDLE ptr) {
	try { ((UTTypeDescIf*) _this)->WriteNumber(val, ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) BSTR __cdecl Spr_UTTypeDescIf_ReadString(HANDLE _this, HANDLE ptr) {
	string _val = (string) 0;
	try { _val = ((UTTypeDescIf*) _this)->ReadString(ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	BSTR wstr = NULL;
	const char* cstr = _val.c_str();
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
	if (lenW > 0) {
	    wstr = ::SysAllocStringLen(0, lenW);
	    ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, wstr, lenW);
	}
	return (BSTR) wstr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_FreeString_ReadString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_WriteString(HANDLE _this, HANDLE val, HANDLE ptr) {
	try { ((UTTypeDescIf*) _this)->WriteString((const char*) val, ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_Create_2(HANDLE _this) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->Create(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_Delete(HANDLE _this, HANDLE ptr) {
	try { ((UTTypeDescIf*) _this)->Delete(ptr); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_VectorPush(HANDLE _this, HANDLE v) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->VectorPush(v); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescIf_VectorPop(HANDLE _this, HANDLE v) {
	try { ((UTTypeDescIf*) _this)->VectorPop(v); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescIf_VectorAt(HANDLE _this, HANDLE v, int pos) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTTypeDescIf*) _this)->VectorAt(v, pos); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) size_t __cdecl Spr_UTTypeDescIf_VectorSize(HANDLE _this, HANDLE v) {
	size_t _val = (size_t) 0;
	try { _val = ((UTTypeDescIf*) _this)->VectorSize(v); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) size_t __cdecl Spr_UTTypeDescIf_SizeOfVector(HANDLE _this) {
	size_t _val = (size_t) 0;
	try { _val = ((UTTypeDescIf*) _this)->SizeOfVector(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_UTTypeDescDbIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new UTTypeDescDbIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_UTTypeDescDbIf(HANDLE v) {
        try { delete (UTTypeDescDbIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescDbIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str;
            (*((UTTypeDescDbIf*) _this)).Print(oss);
            str = oss.str();
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescDbIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((UTTypeDescDbIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescDbIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) UTTypeDescDbIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescDbIf_GetDb(BSTR n) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) n, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) n, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	UTTypeDescDbIf* _ptr = (UTTypeDescDbIf*) 0;
	try { _ptr = (UTTypeDescDbIf*) UTTypeDescDbIf::GetDb((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_RegisterDesc(HANDLE _this, HANDLE n) {
	try { ((UTTypeDescDbIf*) _this)->RegisterDesc((UTTypeDescIf*) n); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_RegisterAlias(HANDLE _this, HANDLE src, HANDLE dest) {
	try { ((UTTypeDescDbIf*) _this)->RegisterAlias((const char*) src, (const char*) dest); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_SetPrefix(HANDLE _this, BSTR p) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) p, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) p, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	try { ((UTTypeDescDbIf*) _this)->SetPrefix((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTypeDescDbIf_Find(HANDLE _this, BSTR tn) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) tn, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	UTTypeDescIf* _ptr = (UTTypeDescIf*) 0;
	try { _ptr = (UTTypeDescIf*) ((UTTypeDescDbIf*) _this)->Find((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_Link(HANDLE _this, HANDLE db) {
	try { ((UTTypeDescDbIf*) _this)->Link((UTTypeDescDbIf*) db); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_Link_1(HANDLE _this) {
	try { ((UTTypeDescDbIf*) _this)->Link(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_LinkAll(HANDLE _this) {
	try { ((UTTypeDescDbIf*) _this)->LinkAll(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) char __cdecl Spr_UTTypeDescDbIf_LinkCheck(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTypeDescDbIf*) _this)->LinkCheck(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) BSTR __cdecl Spr_UTTypeDescDbIf_GetGroup(HANDLE _this) {
	string _val = (string) 0;
	try { _val = ((UTTypeDescDbIf*) _this)->GetGroup(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	BSTR wstr = NULL;
	const char* cstr = _val.c_str();
	int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, NULL, 0);
	if (lenW > 0) {
	    wstr = ::SysAllocStringLen(0, lenW);
	    ::MultiByteToWideChar(CP_ACP, 0, cstr, -1, wstr, lenW);
	}
	return (BSTR) wstr;
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_FreeString_GetGroup(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) void __cdecl Spr_UTTypeDescDbIf_Clear(HANDLE _this) {
	try { ((UTTypeDescDbIf*) _this)->Clear(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_NameManagerIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new NameManagerIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_NameManagerIf(HANDLE v) {
        try { delete (NameManagerIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NameManagerIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((NameManagerIf*) _this)) {
                (*((NameManagerIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_NameManagerIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NameManagerIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((NameManagerIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NameManagerIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) NameManagerIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NameManagerIf_FindObject_2(HANDLE _this, BSTR name, BSTR cls) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) name, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) name, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	string arg02_("");
	int lenMB2 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) cls, -1, NULL, 0, NULL, NULL);
	if (lenMB2 > 0) {
	    LPSTR addr2 = (LPSTR) ::SysAllocStringLen(0, lenMB2);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) cls, -1, addr2, lenMB2, NULL, NULL);
	    arg02_ = (string) addr2;
	}
	NamedObjectIf* _ptr = (NamedObjectIf*) 0;
	try { _ptr = (NamedObjectIf*) ((NameManagerIf*) _this)->FindObject((string) arg01_, (string) arg02_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_NameManagerIf_FindObject_3(HANDLE _this, BSTR name) {
	string arg01_("");
	int lenMB1 = ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) name, -1, NULL, 0, NULL, NULL);
	if (lenMB1 > 0) {
	    LPSTR addr1 = (LPSTR) ::SysAllocStringLen(0, lenMB1);
	    ::WideCharToMultiByte(CP_ACP, (DWORD) 0, (LPCWSTR) name, -1, addr1, lenMB1, NULL, NULL);
	    arg01_ = (string) addr1;
	}
	NamedObjectIf* _ptr = (NamedObjectIf*) 0;
	try { _ptr = (NamedObjectIf*) ((NameManagerIf*) _this)->FindObject((string) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_SceneIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new SceneIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_SceneIf(HANDLE v) {
        try { delete (SceneIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((SceneIf*) _this)) {
                (*((SceneIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_SceneIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((SceneIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SceneIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) SceneIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_SdkIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new SdkIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_SdkIf(HANDLE v) {
        try { delete (SdkIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SdkIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((SdkIf*) _this)) {
                (*((SdkIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_SdkIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SdkIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((SdkIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_SdkIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) SdkIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_new_UTTimerIf() {
	HANDLE _ptr = (HANDLE) 0;
	try { _ptr = new UTTimerIf(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _ptr;
    }
    __declspec(dllexport) void __cdecl Spr_delete_UTTimerIf(HANDLE v) {
        try { delete (UTTimerIf*) v; }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_ToString(HANDLE _this) {
        BSTR result = NULL;
        try {
            ostringstream oss;
            string str = "";
            if (dynamic_cast<ObjectIf*>((UTTimerIf*) _this)) {
                (*((UTTimerIf*) _this)).GetObjectIf()->Print(oss);
                str = oss.str();
            }
            const char* cstr = str.c_str();
            int lenA = str.size();
            int lenW = ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, NULL, 0);
            if (lenW >= 0) {
                result = ::SysAllocStringLen(0, lenW);
                ::MultiByteToWideChar(CP_ACP, 0, cstr, lenA, result, lenW);
            }
        }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
        return result;
    }
    __declspec(dllexport) void __cdecl Spr_UTTimerIf_FreeString(BSTR ptr) {
        try { ::SysFreeString(ptr); }
        catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_GetIfInfo(HANDLE _this) {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) ((UTTimerIf*) _this)->GetIfInfo(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_GetIfInfoStatic() {
	IfInfo* _ptr = (IfInfo*) 0;
	try { _ptr = (IfInfo*) UTTimerIf::GetIfInfoStatic(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_TimerFunc(HANDLE _this, int id) {
	void* _ptr = (void*) 0;
	try { _ptr = UTTimerIf::TimerFunc(id); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) unsigned int __cdecl Spr_UTTimerIf_NTimers() {
	unsigned int _val = (unsigned int) 0;
	try { _val = UTTimerIf::NTimers(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_Get(unsigned int id) {
	UTTimerIf* _ptr = (UTTimerIf*) 0;
	try { _ptr = (UTTimerIf*) UTTimerIf::Get(id); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_Create() {
	UTTimerIf* _ptr = (UTTimerIf*) 0;
	try { _ptr = (UTTimerIf*) UTTimerIf::Create(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) unsigned int __cdecl Spr_UTTimerIf_GetID(HANDLE _this) {
	unsigned int _val = (unsigned int) 0;
	try { _val = ((UTTimerIf*) _this)->GetID(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) unsigned int __cdecl Spr_UTTimerIf_GetResolution(HANDLE _this) {
	unsigned int _val = (unsigned int) 0;
	try { _val = ((UTTimerIf*) _this)->GetResolution(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_SetResolution(HANDLE _this, unsigned int r) {
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->SetResolution(r); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) unsigned int __cdecl Spr_UTTimerIf_GetInterval(HANDLE _this) {
	unsigned int _val = (unsigned int) 0;
	try { _val = ((UTTimerIf*) _this)->GetInterval(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return _val;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_SetInterval(HANDLE _this, unsigned int i) {
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->SetInterval(i); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_UTTimerIf_GetCallbackArg(HANDLE _this) {
	void* _ptr = (void*) 0;
	try { _ptr = ((UTTimerIf*) _this)->GetCallbackArg(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
    __declspec(dllexport) int __cdecl Spr_UTTimerIf_GetMode(HANDLE _this) {
	UTTimerIf::Mode _val = (UTTimerIf::Mode) 0;
	try { _val = ((UTTimerIf*) _this)->GetMode(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (int) _val;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_SetMode(HANDLE _this, int m) {
	UTTimerIf::Mode arg01_ = ((UTTimerIf::Mode) m);
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->SetMode((UTTimerIf::Mode) arg01_); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_IsStarted(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->IsStarted(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_IsRunning(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->IsRunning(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_Start(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->Start(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) char __cdecl Spr_UTTimerIf_Stop(HANDLE _this) {
	bool _val = (bool) 0;
	try { _val = ((UTTimerIf*) _this)->Stop(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) void __cdecl Spr_UTTimerIf_Call(HANDLE _this) {
	try { ((UTTimerIf*) _this)->Call(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
    }
    __declspec(dllexport) char __cdecl Spr_IfInfo_Inherit(HANDLE _this, HANDLE info) {
	bool _val = (bool) 0;
	try { _val = ((IfInfo*) _this)->Inherit((const IfInfo*) info); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	char _ret = _val ? 1 : 0;
	return _ret;
    }
    __declspec(dllexport) HANDLE __cdecl Spr_IfInfo_ClassName(HANDLE _this) {
	const char* _ptr = (const char*) 0;
	try { _ptr = ((IfInfo*) _this)->ClassName(); }
	catch (SEH_Exception e) { e.raise_managed_exception("SprExport.dll"); }
	return (HANDLE) _ptr;
    }
}
