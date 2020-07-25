using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
#pragma warning disable 0108
namespace SprCs {
    public partial class ObjectIfs : CsObject {
	public ObjectIfs() { _this = SprExport.Spr_new_ObjectIfs(); _flag = true; }
	public ObjectIfs(IntPtr ptr) : base(ptr) {}
	public ObjectIfs(IntPtr ptr, bool flag) : base(ptr, flag) {}
	~ObjectIfs() { if (_flag) { SprExport.Spr_delete_ObjectIfs(_this); _flag = false; } }
	public void Push(ObjectIf obj) {
	    SprExport.Spr_ObjectIfs_Push((IntPtr) _this, (IntPtr) obj);
	}
	public void Pop() {
	    SprExport.Spr_ObjectIfs_Pop((IntPtr) _this);
	}
	public ObjectIf Top() {
	    IntPtr ptr = SprExport.Spr_ObjectIfs_Top((IntPtr) _this);
            if (ptr == IntPtr.Zero) { return null; } 
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == NamedObjectIf.GetIfInfoStatic()) { return new NamedObjectIf(ptr); }
            if (obj.GetIfInfo() == ObjectStatesIf.GetIfInfoStatic()) { return new ObjectStatesIf(ptr); }
            if (obj.GetIfInfo() == UTTypeDescIf.GetIfInfoStatic()) { return new UTTypeDescIf(ptr); }
            if (obj.GetIfInfo() == UTTypeDescDbIf.GetIfInfoStatic()) { return new UTTypeDescDbIf(ptr); }
            if (obj.GetIfInfo() == UTTimerIf.GetIfInfoStatic()) { return new UTTimerIf(ptr); }
            return obj;
	}
    }
    public partial class UTAccessBase : CsObject {
	protected UTAccessBase() {}
	public UTAccessBase(IntPtr ptr) : base(ptr) { }
	public UTAccessBase(IntPtr ptr, bool flag) : base(ptr, flag) { }
	public IntPtr Create() {
	    IntPtr result = (IntPtr) SprExport.Spr_UTAccessBase_Create((IntPtr) _this);
	    return result;
	}
	public void Delete(CsObject ptr_) {
	    SprExport.Spr_UTAccessBase_Delete((IntPtr) _this, (IntPtr) ptr_);
	}
	public IntPtr VectorPush(CsObject v) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTAccessBase_VectorPush((IntPtr) _this, (IntPtr) v);
	    return result;
	}
	public void VectorPop(CsObject v) {
	    SprExport.Spr_UTAccessBase_VectorPop((IntPtr) _this, (IntPtr) v);
	}
	public IntPtr VectorAt(CsObject v, int pos) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTAccessBase_VectorAt((IntPtr) _this, (IntPtr) v, (int) pos);
	    return result;
	}
	public ulong VectorSize(CsObject v) {
	    ulong result = (ulong) SprExport.Spr_UTAccessBase_VectorSize((IntPtr) _this, (IntPtr) v);
	    return result;
	}
	public ulong SizeOfVector() {
	    ulong result = (ulong) SprExport.Spr_UTAccessBase_SizeOfVector((IntPtr) _this);
	    return result;
	}
    }
    public partial class Vec2f : CsObject {
	public Vec2f() { _this = SprExport.Spr_new_Vec2f(); _flag = true; }
	public Vec2f(IntPtr ptr) : base(ptr) {}
	public Vec2f(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec2f(float x, float y) { _this = SprExport.Spr_new_Vec2f_3(x, y); _flag = true; }
	~Vec2f() { if (_flag) { SprExport.Spr_delete_Vec2f(_this); _flag = false; } }
	public static explicit operator Vec2f(Vec2d d) {
	    Vec2f f = new Vec2f();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    return f;
	}
	public static Vec2f operator- (Vec2f a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec2f(a);
	    return new Vec2f(ptr, true);
	}
	public static Vec2f operator+ (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec2f(a, b);
	    return new Vec2f(ptr, true);
	}
	public static Vec2f operator- (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec2f(a, b);
	    return new Vec2f(ptr, true);
	}
	public static Vec2f operator* (float a, Vec2f b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vfv_Vec2f(a, b);
	    return new Vec2f(ptr, true);
	}
	public static Vec2f operator* (Vec2f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvf_Vec2f(a, b);
	    return new Vec2f(ptr, true);
	}
	public static Vec2f operator/ (Vec2f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvf_Vec2f(a, b);
	    return new Vec2f(ptr, true);
	}
	public static float operator* (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (float) SprExport.Spr_binary_operator_sprod_fvv_Vec2f(a, b);
	}
	public static float operator% (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (float) SprExport.Spr_binary_operator_vprod1_fvv_Vec2f(a, b);
	}
	public static float operator^ (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (float) SprExport.Spr_binary_operator_vprod2_fvv_Vec2f(a, b);
	}
	public static bool operator== (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec2f(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec2f a, Vec2f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec2f(a, b);
	    return (ret == 0) ? false : true;
	}
	public float this[int i] {
	    get { return (float) SprExport.Spr_unary_operator_index_fi_get_Vec2f(this, i); }
	    set { SprExport.Spr_unary_operator_index_fi_set_Vec2f(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec2f_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec2f_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float x {
	    get { return SprExport.Spr_Vec2f_get_x(_this); }
	    set { SprExport.Spr_Vec2f_set_x(_this, value); }
	}
	public float y {
	    get { return SprExport.Spr_Vec2f_get_y(_this); }
	    set { SprExport.Spr_Vec2f_set_y(_this, value); }
	}
	public float square() {
	    float result = (float) SprExport.Spr_Vec2f_square((IntPtr) _this);
	    return result;
	}
	public float norm() {
	    float result = (float) SprExport.Spr_Vec2f_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec2f_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec2fStruct : CsObject {
        public Vec2fStruct() {
            Vec2f desc = new Vec2f();
            ApplyFrom(desc, false);
        }
	public float x;
	public float y;
	public void ApplyTo(Vec2f r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
	}
	public void ApplyFrom(Vec2f r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
	}
	public static implicit operator Vec2f(Vec2fStruct m) {
	    Vec2f r = new Vec2f();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec2fStruct(Vec2f r) {
	    Vec2fStruct m = new Vec2fStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator Vec2dStruct(Vec2fStruct f) {
	    Vec2dStruct d = new Vec2dStruct();
	    d.x = f.x;
	    d.y = f.y;
	    return d;
	}
    }
    public partial class Vec2d : CsObject {
	public Vec2d() { _this = SprExport.Spr_new_Vec2d(); _flag = true; }
	public Vec2d(IntPtr ptr) : base(ptr) {}
	public Vec2d(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec2d(double x, double y) { _this = SprExport.Spr_new_Vec2d_3(x, y); _flag = true; }
	~Vec2d() { if (_flag) { SprExport.Spr_delete_Vec2d(_this); _flag = false; } }
	public static implicit operator Vec2d(Vec2f f) {
	    Vec2d d = new Vec2d();
	    d.x = f.x;
	    d.y = f.y;
	    return d;
	}
	public static Vec2d operator- (Vec2d a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec2d(a);
	    return new Vec2d(ptr, true);
	}
	public static Vec2d operator+ (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec2d(a, b);
	    return new Vec2d(ptr, true);
	}
	public static Vec2d operator- (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec2d(a, b);
	    return new Vec2d(ptr, true);
	}
	public static Vec2d operator* (double a, Vec2d b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vdv_Vec2d(a, b);
	    return new Vec2d(ptr, true);
	}
	public static Vec2d operator* (Vec2d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvd_Vec2d(a, b);
	    return new Vec2d(ptr, true);
	}
	public static Vec2d operator/ (Vec2d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvd_Vec2d(a, b);
	    return new Vec2d(ptr, true);
	}
	public static double operator* (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (double) SprExport.Spr_binary_operator_sprod_dvv_Vec2d(a, b);
	}
	public static double operator% (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (double) SprExport.Spr_binary_operator_vprod1_dvv_Vec2d(a, b);
	}
	public static double operator^ (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (double) SprExport.Spr_binary_operator_vprod2_dvv_Vec2d(a, b);
	}
	public static bool operator== (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec2d(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec2d a, Vec2d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec2d(a, b);
	    return (ret == 0) ? false : true;
	}
	public double this[int i] {
	    get { return (double) SprExport.Spr_unary_operator_index_di_get_Vec2d(this, i); }
	    set { SprExport.Spr_unary_operator_index_di_set_Vec2d(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec2d_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec2d_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double x {
	    get { return SprExport.Spr_Vec2d_get_x(_this); }
	    set { SprExport.Spr_Vec2d_set_x(_this, value); }
	}
	public double y {
	    get { return SprExport.Spr_Vec2d_get_y(_this); }
	    set { SprExport.Spr_Vec2d_set_y(_this, value); }
	}
	public double square() {
	    double result = (double) SprExport.Spr_Vec2d_square((IntPtr) _this);
	    return result;
	}
	public double norm() {
	    double result = (double) SprExport.Spr_Vec2d_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec2d_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec2dStruct : CsObject {
        public Vec2dStruct() {
            Vec2d desc = new Vec2d();
            ApplyFrom(desc, false);
        }
	public double x;
	public double y;
	public void ApplyTo(Vec2d r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
	}
	public void ApplyFrom(Vec2d r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
	}
	public static implicit operator Vec2d(Vec2dStruct m) {
	    Vec2d r = new Vec2d();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec2dStruct(Vec2d r) {
	    Vec2dStruct m = new Vec2dStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator Vec2fStruct(Vec2dStruct d) {
	    Vec2fStruct f = new Vec2fStruct();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    return f;
	}
    }
    public partial class Vec3f : CsObject {
	public Vec3f() { _this = SprExport.Spr_new_Vec3f(); _flag = true; }
	public Vec3f(IntPtr ptr) : base(ptr) {}
	public Vec3f(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec3f(float x, float y, float z) { _this = SprExport.Spr_new_Vec3f_4(x, y, z); _flag = true; }
	~Vec3f() { if (_flag) { SprExport.Spr_delete_Vec3f(_this); _flag = false; } }
	public static explicit operator Vec3f(Vec3d d) {
	    Vec3f f = new Vec3f();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    return f;
	}
	public static Vec3f operator- (Vec3f a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec3f(a);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator+ (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator- (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator* (float a, Vec3f b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vfv_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator* (Vec3f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvf_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator/ (Vec3f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvf_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static float operator* (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (float) SprExport.Spr_binary_operator_sprod_fvv_Vec3f(a, b);
	}
	public static Vec3f operator% (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_vprod1_vvv_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator^ (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_vprod2_vvv_Vec3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static bool operator== (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec3f(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec3f(a, b);
	    return (ret == 0) ? false : true;
	}
	public float this[int i] {
	    get { return (float) SprExport.Spr_unary_operator_index_fi_get_Vec3f(this, i); }
	    set { SprExport.Spr_unary_operator_index_fi_set_Vec3f(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec3f_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec3f_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float x {
	    get { return SprExport.Spr_Vec3f_get_x(_this); }
	    set { SprExport.Spr_Vec3f_set_x(_this, value); }
	}
	public float y {
	    get { return SprExport.Spr_Vec3f_get_y(_this); }
	    set { SprExport.Spr_Vec3f_set_y(_this, value); }
	}
	public float z {
	    get { return SprExport.Spr_Vec3f_get_z(_this); }
	    set { SprExport.Spr_Vec3f_set_z(_this, value); }
	}
	public float square() {
	    float result = (float) SprExport.Spr_Vec3f_square((IntPtr) _this);
	    return result;
	}
	public float norm() {
	    float result = (float) SprExport.Spr_Vec3f_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec3f_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec3fStruct : CsObject {
        public Vec3fStruct() {
            Vec3f desc = new Vec3f();
            ApplyFrom(desc, false);
        }
	public float x;
	public float y;
	public float z;
	public void ApplyTo(Vec3f r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
		r.z = z;
	}
	public void ApplyFrom(Vec3f r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
		z = r.z;
	}
	public static implicit operator Vec3f(Vec3fStruct m) {
	    Vec3f r = new Vec3f();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec3fStruct(Vec3f r) {
	    Vec3fStruct m = new Vec3fStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator Vec3dStruct(Vec3fStruct f) {
	    Vec3dStruct d = new Vec3dStruct();
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    return d;
	}
    }
    public partial class Vec3d : CsObject {
	public Vec3d() { _this = SprExport.Spr_new_Vec3d(); _flag = true; }
	public Vec3d(IntPtr ptr) : base(ptr) {}
	public Vec3d(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec3d(double x, double y, double z) { _this = SprExport.Spr_new_Vec3d_4(x, y, z); _flag = true; }
	~Vec3d() { if (_flag) { SprExport.Spr_delete_Vec3d(_this); _flag = false; } }
	public static implicit operator Vec3d(Vec3f f) {
	    Vec3d d = new Vec3d();
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    return d;
	}
	public static Vec3d operator- (Vec3d a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec3d(a);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator+ (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator- (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator* (double a, Vec3d b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vdv_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator* (Vec3d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvd_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator/ (Vec3d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvd_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static double operator* (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (double) SprExport.Spr_binary_operator_sprod_dvv_Vec3d(a, b);
	}
	public static Vec3d operator% (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_vprod1_vvv_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator^ (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_vprod2_vvv_Vec3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static bool operator== (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec3d(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec3d(a, b);
	    return (ret == 0) ? false : true;
	}
	public double this[int i] {
	    get { return (double) SprExport.Spr_unary_operator_index_di_get_Vec3d(this, i); }
	    set { SprExport.Spr_unary_operator_index_di_set_Vec3d(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec3d_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec3d_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double x {
	    get { return SprExport.Spr_Vec3d_get_x(_this); }
	    set { SprExport.Spr_Vec3d_set_x(_this, value); }
	}
	public double y {
	    get { return SprExport.Spr_Vec3d_get_y(_this); }
	    set { SprExport.Spr_Vec3d_set_y(_this, value); }
	}
	public double z {
	    get { return SprExport.Spr_Vec3d_get_z(_this); }
	    set { SprExport.Spr_Vec3d_set_z(_this, value); }
	}
	public double square() {
	    double result = (double) SprExport.Spr_Vec3d_square((IntPtr) _this);
	    return result;
	}
	public double norm() {
	    double result = (double) SprExport.Spr_Vec3d_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec3d_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec3dStruct : CsObject {
        public Vec3dStruct() {
            Vec3d desc = new Vec3d();
            ApplyFrom(desc, false);
        }
	public double x;
	public double y;
	public double z;
	public void ApplyTo(Vec3d r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
		r.z = z;
	}
	public void ApplyFrom(Vec3d r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
		z = r.z;
	}
	public static implicit operator Vec3d(Vec3dStruct m) {
	    Vec3d r = new Vec3d();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec3dStruct(Vec3d r) {
	    Vec3dStruct m = new Vec3dStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator Vec3fStruct(Vec3dStruct d) {
	    Vec3fStruct f = new Vec3fStruct();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    return f;
	}
    }
    public partial class Vec4f : CsObject {
	public Vec4f() { _this = SprExport.Spr_new_Vec4f(); _flag = true; }
	public Vec4f(IntPtr ptr) : base(ptr) {}
	public Vec4f(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec4f(float x, float y, float z, float w) { _this = SprExport.Spr_new_Vec4f_5(x, y, z, w); _flag = true; }
	~Vec4f() { if (_flag) { SprExport.Spr_delete_Vec4f(_this); _flag = false; } }
	public static explicit operator Vec4f(Vec4d d) {
	    Vec4f f = new Vec4f();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    f.w = (float) d.w;
	    return f;
	}
	public static Vec4f operator- (Vec4f a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec4f(a);
	    return new Vec4f(ptr, true);
	}
	public static Vec4f operator+ (Vec4f a, Vec4f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec4f(a, b);
	    return new Vec4f(ptr, true);
	}
	public static Vec4f operator- (Vec4f a, Vec4f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec4f(a, b);
	    return new Vec4f(ptr, true);
	}
	public static Vec4f operator* (float a, Vec4f b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vfv_Vec4f(a, b);
	    return new Vec4f(ptr, true);
	}
	public static Vec4f operator* (Vec4f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvf_Vec4f(a, b);
	    return new Vec4f(ptr, true);
	}
	public static Vec4f operator/ (Vec4f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvf_Vec4f(a, b);
	    return new Vec4f(ptr, true);
	}
	public static bool operator== (Vec4f a, Vec4f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec4f(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec4f a, Vec4f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec4f(a, b);
	    return (ret == 0) ? false : true;
	}
	public float this[int i] {
	    get { return (float) SprExport.Spr_unary_operator_index_fi_get_Vec4f(this, i); }
	    set { SprExport.Spr_unary_operator_index_fi_set_Vec4f(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec4f_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec4f_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float x {
	    get { return SprExport.Spr_Vec4f_get_x(_this); }
	    set { SprExport.Spr_Vec4f_set_x(_this, value); }
	}
	public float y {
	    get { return SprExport.Spr_Vec4f_get_y(_this); }
	    set { SprExport.Spr_Vec4f_set_y(_this, value); }
	}
	public float z {
	    get { return SprExport.Spr_Vec4f_get_z(_this); }
	    set { SprExport.Spr_Vec4f_set_z(_this, value); }
	}
	public float w {
	    get { return SprExport.Spr_Vec4f_get_w(_this); }
	    set { SprExport.Spr_Vec4f_set_w(_this, value); }
	}
	public float square() {
	    float result = (float) SprExport.Spr_Vec4f_square((IntPtr) _this);
	    return result;
	}
	public float norm() {
	    float result = (float) SprExport.Spr_Vec4f_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec4f_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec4fStruct : CsObject {
        public Vec4fStruct() {
            Vec4f desc = new Vec4f();
            ApplyFrom(desc, false);
        }
	public float x;
	public float y;
	public float z;
	public float w;
	public void ApplyTo(Vec4f r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
		r.z = z;
		r.w = w;
	}
	public void ApplyFrom(Vec4f r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
		z = r.z;
		w = r.w;
	}
	public static implicit operator Vec4f(Vec4fStruct m) {
	    Vec4f r = new Vec4f();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec4fStruct(Vec4f r) {
	    Vec4fStruct m = new Vec4fStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator Vec4dStruct(Vec4fStruct f) {
	    Vec4dStruct d = new Vec4dStruct();
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    d.w = f.w;
	    return d;
	}
    }
    public partial class Vec4d : CsObject {
	public Vec4d() { _this = SprExport.Spr_new_Vec4d(); _flag = true; }
	public Vec4d(IntPtr ptr) : base(ptr) {}
	public Vec4d(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec4d(double x, double y, double z, double w) { _this = SprExport.Spr_new_Vec4d_5(x, y, z, w); _flag = true; }
	~Vec4d() { if (_flag) { SprExport.Spr_delete_Vec4d(_this); _flag = false; } }
	public static implicit operator Vec4d(Vec4f f) {
	    Vec4d d = new Vec4d();
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    d.w = f.w;
	    return d;
	}
	public static Vec4d operator- (Vec4d a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec4d(a);
	    return new Vec4d(ptr, true);
	}
	public static Vec4d operator+ (Vec4d a, Vec4d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec4d(a, b);
	    return new Vec4d(ptr, true);
	}
	public static Vec4d operator- (Vec4d a, Vec4d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec4d(a, b);
	    return new Vec4d(ptr, true);
	}
	public static Vec4d operator* (double a, Vec4d b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vdv_Vec4d(a, b);
	    return new Vec4d(ptr, true);
	}
	public static Vec4d operator* (Vec4d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvd_Vec4d(a, b);
	    return new Vec4d(ptr, true);
	}
	public static Vec4d operator/ (Vec4d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvd_Vec4d(a, b);
	    return new Vec4d(ptr, true);
	}
	public static bool operator== (Vec4d a, Vec4d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec4d(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec4d a, Vec4d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec4d(a, b);
	    return (ret == 0) ? false : true;
	}
	public double this[int i] {
	    get { return (double) SprExport.Spr_unary_operator_index_di_get_Vec4d(this, i); }
	    set { SprExport.Spr_unary_operator_index_di_set_Vec4d(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec4d_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec4d_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double x {
	    get { return SprExport.Spr_Vec4d_get_x(_this); }
	    set { SprExport.Spr_Vec4d_set_x(_this, value); }
	}
	public double y {
	    get { return SprExport.Spr_Vec4d_get_y(_this); }
	    set { SprExport.Spr_Vec4d_set_y(_this, value); }
	}
	public double z {
	    get { return SprExport.Spr_Vec4d_get_z(_this); }
	    set { SprExport.Spr_Vec4d_set_z(_this, value); }
	}
	public double w {
	    get { return SprExport.Spr_Vec4d_get_w(_this); }
	    set { SprExport.Spr_Vec4d_set_w(_this, value); }
	}
	public double square() {
	    double result = (double) SprExport.Spr_Vec4d_square((IntPtr) _this);
	    return result;
	}
	public double norm() {
	    double result = (double) SprExport.Spr_Vec4d_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec4d_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec4dStruct : CsObject {
        public Vec4dStruct() {
            Vec4d desc = new Vec4d();
            ApplyFrom(desc, false);
        }
	public double x;
	public double y;
	public double z;
	public double w;
	public void ApplyTo(Vec4d r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
		r.z = z;
		r.w = w;
	}
	public void ApplyFrom(Vec4d r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
		z = r.z;
		w = r.w;
	}
	public static implicit operator Vec4d(Vec4dStruct m) {
	    Vec4d r = new Vec4d();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec4dStruct(Vec4d r) {
	    Vec4dStruct m = new Vec4dStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator Vec4fStruct(Vec4dStruct d) {
	    Vec4fStruct f = new Vec4fStruct();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    f.w = (float) d.w;
	    return f;
	}
    }
    public partial class Quaternionf : CsObject {
	public Quaternionf() { _this = SprExport.Spr_new_Quaternionf(); _flag = true; }
	public Quaternionf(IntPtr ptr) : base(ptr) {}
	public Quaternionf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Quaternionf(float w, float x, float y, float z) { _this = SprExport.Spr_new_Quaternionf_5(w, x, y, z); _flag = true; }
	~Quaternionf() { if (_flag) { SprExport.Spr_delete_Quaternionf(_this); _flag = false; } }
	public static explicit operator Quaternionf(Quaterniond d) {
	    Quaternionf f = new Quaternionf();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    f.w = (float) d.w;
	    return f;
	}
	public static Quaternionf operator+ (Quaternionf a, Quaternionf b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_qqq_Quaternionf(a, b);
	    return new Quaternionf(ptr, true);
	}
	public static Quaternionf operator- (Quaternionf a, Quaternionf b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_qqq_Quaternionf(a, b);
	    return new Quaternionf(ptr, true);
	}
	public static Quaternionf operator* (float a, Quaternionf b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_qfq_Quaternionf(a, b);
	    return new Quaternionf(ptr, true);
	}
	public static Quaternionf operator* (Quaternionf a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_qqf_Quaternionf(a, b);
	    return new Quaternionf(ptr, true);
	}
	public static Quaternionf operator/ (Quaternionf a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_qqf_Quaternionf(a, b);
	    return new Quaternionf(ptr, true);
	}
	public static Vec3f operator* (Quaternionf a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vqv_Quaternionf(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Quaternionf operator* (Quaternionf a, Quaternionf b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_qqq_Quaternionf(a, b);
	    return new Quaternionf(ptr, true);
	}
	public static Matrix3f operator* (Quaternionf a, Matrix3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_mqm_Quaternionf(a, b);
	    return new Matrix3f(ptr, true);
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Quaternionf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Quaternionf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float x {
	    get { return SprExport.Spr_Quaternionf_get_x(_this); }
	    set { SprExport.Spr_Quaternionf_set_x(_this, value); }
	}
	public float y {
	    get { return SprExport.Spr_Quaternionf_get_y(_this); }
	    set { SprExport.Spr_Quaternionf_set_y(_this, value); }
	}
	public float z {
	    get { return SprExport.Spr_Quaternionf_get_z(_this); }
	    set { SprExport.Spr_Quaternionf_set_z(_this, value); }
	}
	public float w {
	    get { return SprExport.Spr_Quaternionf_get_w(_this); }
	    set { SprExport.Spr_Quaternionf_set_w(_this, value); }
	}
	public float W() {
	    float result = (float) SprExport.Spr_Quaternionf_W((IntPtr) _this);
	    return result;
	}
	public float X() {
	    float result = (float) SprExport.Spr_Quaternionf_X((IntPtr) _this);
	    return result;
	}
	public float Y() {
	    float result = (float) SprExport.Spr_Quaternionf_Y((IntPtr) _this);
	    return result;
	}
	public float Z() {
	    float result = (float) SprExport.Spr_Quaternionf_Z((IntPtr) _this);
	    return result;
	}
	public Vec3f V() {
	    IntPtr ptr = SprExport.Spr_Quaternionf_V((IntPtr) _this);
            return new Vec3f(ptr, true);
	}
	public Vec3f RotationHalf() {
	    IntPtr ptr = SprExport.Spr_Quaternionf_RotationHalf((IntPtr) _this);
            return new Vec3f(ptr, true);
	}
	public Vec3f Rotation() {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Rotation((IntPtr) _this);
            return new Vec3f(ptr, true);
	}
	public Vec3f Axis() {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Axis((IntPtr) _this);
            return new Vec3f(ptr, true);
	}
	public float Theta() {
	    float result = (float) SprExport.Spr_Quaternionf_Theta((IntPtr) _this);
	    return result;
	}
	public static Quaternionf Rot(float angle, Vec3f axis) {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Rot((float) angle, (IntPtr) axis);
            return new Quaternionf(ptr, true);
	}
	public static Quaternionf Rot(float angle, sbyte axis) {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Rot_1((float) angle, (sbyte) axis);
            return new Quaternionf(ptr, true);
	}
	public static Quaternionf Rot(Vec3f rot) {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Rot_2((IntPtr) rot);
            return new Quaternionf(ptr, true);
	}
	public Quaternionf Inv() {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Inv((IntPtr) _this);
            return new Quaternionf(ptr, true);
	}
	public void Conjugate() {
	    SprExport.Spr_Quaternionf_Conjugate((IntPtr) _this);
	}
	public Quaternionf Conjugated() {
	    IntPtr ptr = SprExport.Spr_Quaternionf_Conjugated((IntPtr) _this);
            return new Quaternionf(ptr, true);
	}
	public void RotationArc(Vec3f lhs, Vec3f rhs) {
	    SprExport.Spr_Quaternionf_RotationArc((IntPtr) _this, (IntPtr) lhs, (IntPtr) rhs);
	}
	public void Euler(float heading, float attitude, float bank) {
	    SprExport.Spr_Quaternionf_Euler((IntPtr) _this, (float) heading, (float) attitude, (float) bank);
	}
    }
    [System.Serializable]
    public class QuaternionfStruct : CsObject {
        public QuaternionfStruct() {
            Quaternionf desc = new Quaternionf();
            ApplyFrom(desc, false);
        }
	public float x;
	public float y;
	public float z;
	public float w;
	public void ApplyTo(Quaternionf r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
		r.z = z;
		r.w = w;
	}
	public void ApplyFrom(Quaternionf r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
		z = r.z;
		w = r.w;
	}
	public static implicit operator Quaternionf(QuaternionfStruct m) {
	    Quaternionf r = new Quaternionf();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator QuaternionfStruct(Quaternionf r) {
	    QuaternionfStruct m = new QuaternionfStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator QuaterniondStruct(QuaternionfStruct f) {
	    QuaterniondStruct d = new QuaterniondStruct();
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    d.w = f.w;
	    return d;
	}
    }
    public partial class Quaterniond : CsObject {
	public Quaterniond() { _this = SprExport.Spr_new_Quaterniond(); _flag = true; }
	public Quaterniond(IntPtr ptr) : base(ptr) {}
	public Quaterniond(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Quaterniond(double w, double x, double y, double z) { _this = SprExport.Spr_new_Quaterniond_5(w, x, y, z); _flag = true; }
	~Quaterniond() { if (_flag) { SprExport.Spr_delete_Quaterniond(_this); _flag = false; } }
	public static implicit operator Quaterniond(Quaternionf f) {
	    Quaterniond d = new Quaterniond();
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    d.w = f.w;
	    return d;
	}
	public static Quaterniond operator+ (Quaterniond a, Quaterniond b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_qqq_Quaterniond(a, b);
	    return new Quaterniond(ptr, true);
	}
	public static Quaterniond operator- (Quaterniond a, Quaterniond b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_qqq_Quaterniond(a, b);
	    return new Quaterniond(ptr, true);
	}
	public static Quaterniond operator* (double a, Quaterniond b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_qdq_Quaterniond(a, b);
	    return new Quaterniond(ptr, true);
	}
	public static Quaterniond operator* (Quaterniond a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_qqd_Quaterniond(a, b);
	    return new Quaterniond(ptr, true);
	}
	public static Quaterniond operator/ (Quaterniond a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_qqd_Quaterniond(a, b);
	    return new Quaterniond(ptr, true);
	}
	public static Vec3d operator* (Quaterniond a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vqv_Quaterniond(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Quaterniond operator* (Quaterniond a, Quaterniond b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_qqq_Quaterniond(a, b);
	    return new Quaterniond(ptr, true);
	}
	public static Matrix3d operator* (Quaterniond a, Matrix3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_mqm_Quaterniond(a, b);
	    return new Matrix3d(ptr, true);
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Quaterniond_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Quaterniond_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double x {
	    get { return SprExport.Spr_Quaterniond_get_x(_this); }
	    set { SprExport.Spr_Quaterniond_set_x(_this, value); }
	}
	public double y {
	    get { return SprExport.Spr_Quaterniond_get_y(_this); }
	    set { SprExport.Spr_Quaterniond_set_y(_this, value); }
	}
	public double z {
	    get { return SprExport.Spr_Quaterniond_get_z(_this); }
	    set { SprExport.Spr_Quaterniond_set_z(_this, value); }
	}
	public double w {
	    get { return SprExport.Spr_Quaterniond_get_w(_this); }
	    set { SprExport.Spr_Quaterniond_set_w(_this, value); }
	}
	public double W() {
	    double result = (double) SprExport.Spr_Quaterniond_W((IntPtr) _this);
	    return result;
	}
	public double X() {
	    double result = (double) SprExport.Spr_Quaterniond_X((IntPtr) _this);
	    return result;
	}
	public double Y() {
	    double result = (double) SprExport.Spr_Quaterniond_Y((IntPtr) _this);
	    return result;
	}
	public double Z() {
	    double result = (double) SprExport.Spr_Quaterniond_Z((IntPtr) _this);
	    return result;
	}
	public Vec3d V() {
	    IntPtr ptr = SprExport.Spr_Quaterniond_V((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
	public Vec3d RotationHalf() {
	    IntPtr ptr = SprExport.Spr_Quaterniond_RotationHalf((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
	public Vec3d Rotation() {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Rotation((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
	public Vec3d Axis() {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Axis((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
	public double Theta() {
	    double result = (double) SprExport.Spr_Quaterniond_Theta((IntPtr) _this);
	    return result;
	}
	public static Quaterniond Rot(double angle, Vec3d axis) {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Rot((double) angle, (IntPtr) axis);
            return new Quaterniond(ptr, true);
	}
	public static Quaterniond Rot(double angle, sbyte axis) {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Rot_1((double) angle, (sbyte) axis);
            return new Quaterniond(ptr, true);
	}
	public static Quaterniond Rot(Vec3d rot) {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Rot_2((IntPtr) rot);
            return new Quaterniond(ptr, true);
	}
	public Quaterniond Inv() {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Inv((IntPtr) _this);
            return new Quaterniond(ptr, true);
	}
	public void Conjugate() {
	    SprExport.Spr_Quaterniond_Conjugate((IntPtr) _this);
	}
	public Quaterniond Conjugated() {
	    IntPtr ptr = SprExport.Spr_Quaterniond_Conjugated((IntPtr) _this);
            return new Quaterniond(ptr, true);
	}
	public void RotationArc(Vec3d lhs, Vec3d rhs) {
	    SprExport.Spr_Quaterniond_RotationArc((IntPtr) _this, (IntPtr) lhs, (IntPtr) rhs);
	}
	public void Euler(double heading, double attitude, double bank) {
	    SprExport.Spr_Quaterniond_Euler((IntPtr) _this, (double) heading, (double) attitude, (double) bank);
	}
    }
    [System.Serializable]
    public class QuaterniondStruct : CsObject {
        public QuaterniondStruct() {
            Quaterniond desc = new Quaterniond();
            ApplyFrom(desc, false);
        }
	public double x;
	public double y;
	public double z;
	public double w;
	public void ApplyTo(Quaterniond r, bool apply_to_base_class = false) {
		r.x = x;
		r.y = y;
		r.z = z;
		r.w = w;
	}
	public void ApplyFrom(Quaterniond r, bool apply_to_base_class = false) {
		x = r.x;
		y = r.y;
		z = r.z;
		w = r.w;
	}
	public static implicit operator Quaterniond(QuaterniondStruct m) {
	    Quaterniond r = new Quaterniond();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator QuaterniondStruct(Quaterniond r) {
	    QuaterniondStruct m = new QuaterniondStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator QuaternionfStruct(QuaterniondStruct d) {
	    QuaternionfStruct f = new QuaternionfStruct();
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    f.w = (float) d.w;
	    return f;
	}
    }
    public partial class Posef : CsObject {
	public Posef() { _this = SprExport.Spr_new_Posef(); _flag = true; }
	public Posef(IntPtr ptr) : base(ptr) {}
	public Posef(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Posef(Vec3f p, Quaternionf q) { _this = SprExport.Spr_new_Posef_8(p, q); _flag = true; }
	public Posef(float w, float x, float y, float z, float px, float py, float pz) { _this = SprExport.Spr_new_Posef_9(w, x, y, z, px, py, pz); _flag = true; }
	~Posef() { if (_flag) { SprExport.Spr_delete_Posef(_this); _flag = false; } }
	public static explicit operator Posef(Posed d) {
	    Posef f = new Posef();
	    f.w = (float) d.w;
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    f.px = (float) d.px;
	    f.py = (float) d.py;
	    f.pz = (float) d.pz;
	    return f;
	}
	public static Vec3f operator* (Posef a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vpv_Posef(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Posef operator* (Posef a, Posef b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_ppp_Posef(a, b);
	    return new Posef(ptr, true);
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Posef_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Posef_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float w {
	    get { return SprExport.Spr_Posef_get_w(_this); }
	    set { SprExport.Spr_Posef_set_w(_this, value); }
	}
	public float x {
	    get { return SprExport.Spr_Posef_get_x(_this); }
	    set { SprExport.Spr_Posef_set_x(_this, value); }
	}
	public float y {
	    get { return SprExport.Spr_Posef_get_y(_this); }
	    set { SprExport.Spr_Posef_set_y(_this, value); }
	}
	public float z {
	    get { return SprExport.Spr_Posef_get_z(_this); }
	    set { SprExport.Spr_Posef_set_z(_this, value); }
	}
	public float px {
	    get { return SprExport.Spr_Posef_get_px(_this); }
	    set { SprExport.Spr_Posef_set_px(_this, value); }
	}
	public float py {
	    get { return SprExport.Spr_Posef_get_py(_this); }
	    set { SprExport.Spr_Posef_set_py(_this, value); }
	}
	public float pz {
	    get { return SprExport.Spr_Posef_get_pz(_this); }
	    set { SprExport.Spr_Posef_set_pz(_this, value); }
	}
	public Vec3f Pos() {
	    IntPtr ptr = SprExport.Spr_Posef_Pos((IntPtr) _this);
            return new Vec3f(ptr, true);
	}
	public Quaternionf Ori() {
	    IntPtr ptr = SprExport.Spr_Posef_Ori((IntPtr) _this);
            return new Quaternionf(ptr, true);
	}
	public float W() {
	    float result = (float) SprExport.Spr_Posef_W((IntPtr) _this);
	    return result;
	}
	public float X() {
	    float result = (float) SprExport.Spr_Posef_X((IntPtr) _this);
	    return result;
	}
	public float Y() {
	    float result = (float) SprExport.Spr_Posef_Y((IntPtr) _this);
	    return result;
	}
	public float Z() {
	    float result = (float) SprExport.Spr_Posef_Z((IntPtr) _this);
	    return result;
	}
	public float Px() {
	    float result = (float) SprExport.Spr_Posef_Px((IntPtr) _this);
	    return result;
	}
	public float Py() {
	    float result = (float) SprExport.Spr_Posef_Py((IntPtr) _this);
	    return result;
	}
	public float Pz() {
	    float result = (float) SprExport.Spr_Posef_Pz((IntPtr) _this);
	    return result;
	}
	public float PosX() {
	    float result = (float) SprExport.Spr_Posef_PosX((IntPtr) _this);
	    return result;
	}
	public float PosY() {
	    float result = (float) SprExport.Spr_Posef_PosY((IntPtr) _this);
	    return result;
	}
	public float PosZ() {
	    float result = (float) SprExport.Spr_Posef_PosZ((IntPtr) _this);
	    return result;
	}
	public float OriX() {
	    float result = (float) SprExport.Spr_Posef_OriX((IntPtr) _this);
	    return result;
	}
	public float OriY() {
	    float result = (float) SprExport.Spr_Posef_OriY((IntPtr) _this);
	    return result;
	}
	public float OriZ() {
	    float result = (float) SprExport.Spr_Posef_OriZ((IntPtr) _this);
	    return result;
	}
	public float OriW() {
	    float result = (float) SprExport.Spr_Posef_OriW((IntPtr) _this);
	    return result;
	}
	public Posef Inv() {
	    IntPtr ptr = SprExport.Spr_Posef_Inv((IntPtr) _this);
            return new Posef(ptr, true);
	}
	public static Posef Unit() {
	    IntPtr ptr = SprExport.Spr_Posef_Unit();
            return new Posef(ptr, true);
	}
	public static Posef Trn(float px, float py, float pz) {
	    IntPtr ptr = SprExport.Spr_Posef_Trn((float) px, (float) py, (float) pz);
            return new Posef(ptr, true);
	}
	public static Posef Trn(Vec3f v) {
	    IntPtr ptr = SprExport.Spr_Posef_Trn_1((IntPtr) v);
            return new Posef(ptr, true);
	}
	public static Posef Rot(float wi, float xi, float yi, float zi) {
	    IntPtr ptr = SprExport.Spr_Posef_Rot((float) wi, (float) xi, (float) yi, (float) zi);
            return new Posef(ptr, true);
	}
	public static Posef Rot(float angle, Vec3f axis) {
	    IntPtr ptr = SprExport.Spr_Posef_Rot_1((float) angle, (IntPtr) axis);
            return new Posef(ptr, true);
	}
	public static Posef Rot(float angle, sbyte axis) {
	    IntPtr ptr = SprExport.Spr_Posef_Rot_2((float) angle, (sbyte) axis);
            return new Posef(ptr, true);
	}
	public static Posef Rot(Vec3f rot) {
	    IntPtr ptr = SprExport.Spr_Posef_Rot_3((IntPtr) rot);
            return new Posef(ptr, true);
	}
	public static Posef Rot(Quaternionf q) {
	    IntPtr ptr = SprExport.Spr_Posef_Rot_4((IntPtr) q);
            return new Posef(ptr, true);
	}
    }
    [System.Serializable]
    public class PosefStruct : CsObject {
        public PosefStruct() {
            Posef desc = new Posef();
            ApplyFrom(desc, false);
        }
	public float w;
	public float x;
	public float y;
	public float z;
	public float px;
	public float py;
	public float pz;
	public void ApplyTo(Posef r, bool apply_to_base_class = false) {
		r.w = w;
		r.x = x;
		r.y = y;
		r.z = z;
		r.px = px;
		r.py = py;
		r.pz = pz;
	}
	public void ApplyFrom(Posef r, bool apply_to_base_class = false) {
		w = r.w;
		x = r.x;
		y = r.y;
		z = r.z;
		px = r.px;
		py = r.py;
		pz = r.pz;
	}
	public static implicit operator Posef(PosefStruct m) {
	    Posef r = new Posef();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator PosefStruct(Posef r) {
	    PosefStruct m = new PosefStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator PosedStruct(PosefStruct f) {
	    PosedStruct d = new PosedStruct();
	    d.w = f.w;
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    d.px = f.px;
	    d.py = f.py;
	    d.pz = f.pz;
	    return d;
	}
    }
    public partial class Posed : CsObject {
	public Posed() { _this = SprExport.Spr_new_Posed(); _flag = true; }
	public Posed(IntPtr ptr) : base(ptr) {}
	public Posed(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Posed(Vec3d p, Quaterniond q) { _this = SprExport.Spr_new_Posed_8(p, q); _flag = true; }
	public Posed(double w, double x, double y, double z, double px, double py, double pz) { _this = SprExport.Spr_new_Posed_9(w, x, y, z, px, py, pz); _flag = true; }
	~Posed() { if (_flag) { SprExport.Spr_delete_Posed(_this); _flag = false; } }
	public static implicit operator Posed(Posef f) {
	    Posed d = new Posed();
	    d.w = f.w;
	    d.x = f.x;
	    d.y = f.y;
	    d.z = f.z;
	    d.px = f.px;
	    d.py = f.py;
	    d.pz = f.pz;
	    return d;
	}
	public static Vec3d operator* (Posed a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vpv_Posed(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Posed operator* (Posed a, Posed b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_ppp_Posed(a, b);
	    return new Posed(ptr, true);
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Posed_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Posed_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double w {
	    get { return SprExport.Spr_Posed_get_w(_this); }
	    set { SprExport.Spr_Posed_set_w(_this, value); }
	}
	public double x {
	    get { return SprExport.Spr_Posed_get_x(_this); }
	    set { SprExport.Spr_Posed_set_x(_this, value); }
	}
	public double y {
	    get { return SprExport.Spr_Posed_get_y(_this); }
	    set { SprExport.Spr_Posed_set_y(_this, value); }
	}
	public double z {
	    get { return SprExport.Spr_Posed_get_z(_this); }
	    set { SprExport.Spr_Posed_set_z(_this, value); }
	}
	public double px {
	    get { return SprExport.Spr_Posed_get_px(_this); }
	    set { SprExport.Spr_Posed_set_px(_this, value); }
	}
	public double py {
	    get { return SprExport.Spr_Posed_get_py(_this); }
	    set { SprExport.Spr_Posed_set_py(_this, value); }
	}
	public double pz {
	    get { return SprExport.Spr_Posed_get_pz(_this); }
	    set { SprExport.Spr_Posed_set_pz(_this, value); }
	}
	public Vec3d Pos() {
	    IntPtr ptr = SprExport.Spr_Posed_Pos((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
	public Quaterniond Ori() {
	    IntPtr ptr = SprExport.Spr_Posed_Ori((IntPtr) _this);
            return new Quaterniond(ptr, true);
	}
	public double W() {
	    double result = (double) SprExport.Spr_Posed_W((IntPtr) _this);
	    return result;
	}
	public double X() {
	    double result = (double) SprExport.Spr_Posed_X((IntPtr) _this);
	    return result;
	}
	public double Y() {
	    double result = (double) SprExport.Spr_Posed_Y((IntPtr) _this);
	    return result;
	}
	public double Z() {
	    double result = (double) SprExport.Spr_Posed_Z((IntPtr) _this);
	    return result;
	}
	public double Px() {
	    double result = (double) SprExport.Spr_Posed_Px((IntPtr) _this);
	    return result;
	}
	public double Py() {
	    double result = (double) SprExport.Spr_Posed_Py((IntPtr) _this);
	    return result;
	}
	public double Pz() {
	    double result = (double) SprExport.Spr_Posed_Pz((IntPtr) _this);
	    return result;
	}
	public double PosX() {
	    double result = (double) SprExport.Spr_Posed_PosX((IntPtr) _this);
	    return result;
	}
	public double PosY() {
	    double result = (double) SprExport.Spr_Posed_PosY((IntPtr) _this);
	    return result;
	}
	public double PosZ() {
	    double result = (double) SprExport.Spr_Posed_PosZ((IntPtr) _this);
	    return result;
	}
	public double OriX() {
	    double result = (double) SprExport.Spr_Posed_OriX((IntPtr) _this);
	    return result;
	}
	public double OriY() {
	    double result = (double) SprExport.Spr_Posed_OriY((IntPtr) _this);
	    return result;
	}
	public double OriZ() {
	    double result = (double) SprExport.Spr_Posed_OriZ((IntPtr) _this);
	    return result;
	}
	public double OriW() {
	    double result = (double) SprExport.Spr_Posed_OriW((IntPtr) _this);
	    return result;
	}
	public Posed Inv() {
	    IntPtr ptr = SprExport.Spr_Posed_Inv((IntPtr) _this);
            return new Posed(ptr, true);
	}
	public static Posed Unit() {
	    IntPtr ptr = SprExport.Spr_Posed_Unit();
            return new Posed(ptr, true);
	}
	public static Posed Trn(double px, double py, double pz) {
	    IntPtr ptr = SprExport.Spr_Posed_Trn((double) px, (double) py, (double) pz);
            return new Posed(ptr, true);
	}
	public static Posed Trn(Vec3f v) {
	    IntPtr ptr = SprExport.Spr_Posed_Trn_1((IntPtr) v);
            return new Posed(ptr, true);
	}
	public static Posed Rot(double wi, double xi, double yi, double zi) {
	    IntPtr ptr = SprExport.Spr_Posed_Rot((double) wi, (double) xi, (double) yi, (double) zi);
            return new Posed(ptr, true);
	}
	public static Posed Rot(double angle, Vec3f axis) {
	    IntPtr ptr = SprExport.Spr_Posed_Rot_1((double) angle, (IntPtr) axis);
            return new Posed(ptr, true);
	}
	public static Posed Rot(double angle, sbyte axis) {
	    IntPtr ptr = SprExport.Spr_Posed_Rot_2((double) angle, (sbyte) axis);
            return new Posed(ptr, true);
	}
	public static Posed Rot(Vec3d rot) {
	    IntPtr ptr = SprExport.Spr_Posed_Rot_3((IntPtr) rot);
            return new Posed(ptr, true);
	}
	public static Posed Rot(Quaterniond q) {
	    IntPtr ptr = SprExport.Spr_Posed_Rot_4((IntPtr) q);
            return new Posed(ptr, true);
	}
    }
    [System.Serializable]
    public class PosedStruct : CsObject {
        public PosedStruct() {
            Posed desc = new Posed();
            ApplyFrom(desc, false);
        }
	public double w;
	public double x;
	public double y;
	public double z;
	public double px;
	public double py;
	public double pz;
	public void ApplyTo(Posed r, bool apply_to_base_class = false) {
		r.w = w;
		r.x = x;
		r.y = y;
		r.z = z;
		r.px = px;
		r.py = py;
		r.pz = pz;
	}
	public void ApplyFrom(Posed r, bool apply_to_base_class = false) {
		w = r.w;
		x = r.x;
		y = r.y;
		z = r.z;
		px = r.px;
		py = r.py;
		pz = r.pz;
	}
	public static implicit operator Posed(PosedStruct m) {
	    Posed r = new Posed();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator PosedStruct(Posed r) {
	    PosedStruct m = new PosedStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator PosefStruct(PosedStruct d) {
	    PosefStruct f = new PosefStruct();
	    f.w = (float) d.w;
	    f.x = (float) d.x;
	    f.y = (float) d.y;
	    f.z = (float) d.z;
	    f.px = (float) d.px;
	    f.py = (float) d.py;
	    f.pz = (float) d.pz;
	    return f;
	}
    }
    public partial class Vec2i : CsObject {
	public Vec2i() { _this = SprExport.Spr_new_Vec2i(); _flag = true; }
	public Vec2i(IntPtr ptr) : base(ptr) {}
	public Vec2i(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec2i(int x, int y) { _this = SprExport.Spr_new_Vec2i_3(x, y); _flag = true; }
	~Vec2i() { if (_flag) { SprExport.Spr_delete_Vec2i(_this); _flag = false; } }
	public static Vec2i operator- (Vec2i a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec2i(a);
	    return new Vec2i(ptr, true);
	}
	public static Vec2i operator+ (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec2i(a, b);
	    return new Vec2i(ptr, true);
	}
	public static Vec2i operator- (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec2i(a, b);
	    return new Vec2i(ptr, true);
	}
	public static Vec2i operator* (int a, Vec2i b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_viv_Vec2i(a, b);
	    return new Vec2i(ptr, true);
	}
	public static Vec2i operator* (Vec2i a, int b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvi_Vec2i(a, b);
	    return new Vec2i(ptr, true);
	}
	public static Vec2i operator/ (Vec2i a, int b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvi_Vec2i(a, b);
	    return new Vec2i(ptr, true);
	}
	public static int operator* (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (int) SprExport.Spr_binary_operator_sprod_ivv_Vec2i(a, b);
	}
	public static int operator% (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (int) SprExport.Spr_binary_operator_vprod1_ivv_Vec2i(a, b);
	}
	public static int operator^ (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (int) SprExport.Spr_binary_operator_vprod2_ivv_Vec2i(a, b);
	}
	public static bool operator== (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec2i(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec2i a, Vec2i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec2i(a, b);
	    return (ret == 0) ? false : true;
	}
	public int this[int i] {
	    get { return (int) SprExport.Spr_unary_operator_index_ii_get_Vec2i(this, i); }
	    set { SprExport.Spr_unary_operator_index_ii_set_Vec2i(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec2i_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec2i_FreeString((IntPtr) ptr);
            return bstr;
        }
	public int x {
	    get { return SprExport.Spr_Vec2i_get_x(_this); }
	    set { SprExport.Spr_Vec2i_set_x(_this, value); }
	}
	public int y {
	    get { return SprExport.Spr_Vec2i_get_y(_this); }
	    set { SprExport.Spr_Vec2i_set_y(_this, value); }
	}
	public int square() {
	    int result = (int) SprExport.Spr_Vec2i_square((IntPtr) _this);
	    return result;
	}
	public int norm() {
	    int result = (int) SprExport.Spr_Vec2i_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec2i_unitize((IntPtr) _this);
	}
    }
    public partial class Vec3i : CsObject {
	public Vec3i() { _this = SprExport.Spr_new_Vec3i(); _flag = true; }
	public Vec3i(IntPtr ptr) : base(ptr) {}
	public Vec3i(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec3i(int x, int y, int z) { _this = SprExport.Spr_new_Vec3i_4(x, y, z); _flag = true; }
	~Vec3i() { if (_flag) { SprExport.Spr_delete_Vec3i(_this); _flag = false; } }
	public static Vec3i operator- (Vec3i a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec3i(a);
	    return new Vec3i(ptr, true);
	}
	public static Vec3i operator+ (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static Vec3i operator- (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static Vec3i operator* (int a, Vec3i b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_viv_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static Vec3i operator* (Vec3i a, int b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvi_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static Vec3i operator/ (Vec3i a, int b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvi_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static int operator* (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    return (int) SprExport.Spr_binary_operator_sprod_ivv_Vec3i(a, b);
	}
	public static Vec3i operator% (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_vprod1_vvv_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static Vec3i operator^ (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_vprod2_vvv_Vec3i(a, b);
	    return new Vec3i(ptr, true);
	}
	public static bool operator== (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec3i(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec3i a, Vec3i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec3i(a, b);
	    return (ret == 0) ? false : true;
	}
	public int this[int i] {
	    get { return (int) SprExport.Spr_unary_operator_index_ii_get_Vec3i(this, i); }
	    set { SprExport.Spr_unary_operator_index_ii_set_Vec3i(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec3i_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec3i_FreeString((IntPtr) ptr);
            return bstr;
        }
	public int x {
	    get { return SprExport.Spr_Vec3i_get_x(_this); }
	    set { SprExport.Spr_Vec3i_set_x(_this, value); }
	}
	public int y {
	    get { return SprExport.Spr_Vec3i_get_y(_this); }
	    set { SprExport.Spr_Vec3i_set_y(_this, value); }
	}
	public int z {
	    get { return SprExport.Spr_Vec3i_get_z(_this); }
	    set { SprExport.Spr_Vec3i_set_z(_this, value); }
	}
	public int square() {
	    int result = (int) SprExport.Spr_Vec3i_square((IntPtr) _this);
	    return result;
	}
	public int norm() {
	    int result = (int) SprExport.Spr_Vec3i_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec3i_unitize((IntPtr) _this);
	}
    }
    public partial class Vec4i : CsObject {
	public Vec4i() { _this = SprExport.Spr_new_Vec4i(); _flag = true; }
	public Vec4i(IntPtr ptr) : base(ptr) {}
	public Vec4i(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec4i(int x, int y, int z, int w) { _this = SprExport.Spr_new_Vec4i_5(x, y, z, w); _flag = true; }
	~Vec4i() { if (_flag) { SprExport.Spr_delete_Vec4i(_this); _flag = false; } }
	public static Vec4i operator- (Vec4i a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec4i(a);
	    return new Vec4i(ptr, true);
	}
	public static Vec4i operator+ (Vec4i a, Vec4i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec4i(a, b);
	    return new Vec4i(ptr, true);
	}
	public static Vec4i operator- (Vec4i a, Vec4i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec4i(a, b);
	    return new Vec4i(ptr, true);
	}
	public static Vec4i operator* (int a, Vec4i b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_viv_Vec4i(a, b);
	    return new Vec4i(ptr, true);
	}
	public static Vec4i operator* (Vec4i a, int b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvi_Vec4i(a, b);
	    return new Vec4i(ptr, true);
	}
	public static Vec4i operator/ (Vec4i a, int b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvi_Vec4i(a, b);
	    return new Vec4i(ptr, true);
	}
	public static bool operator== (Vec4i a, Vec4i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec4i(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec4i a, Vec4i b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec4i(a, b);
	    return (ret == 0) ? false : true;
	}
	public int this[int i] {
	    get { return (int) SprExport.Spr_unary_operator_index_ii_get_Vec4i(this, i); }
	    set { SprExport.Spr_unary_operator_index_ii_set_Vec4i(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec4i_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec4i_FreeString((IntPtr) ptr);
            return bstr;
        }
	public int x {
	    get { return SprExport.Spr_Vec4i_get_x(_this); }
	    set { SprExport.Spr_Vec4i_set_x(_this, value); }
	}
	public int y {
	    get { return SprExport.Spr_Vec4i_get_y(_this); }
	    set { SprExport.Spr_Vec4i_set_y(_this, value); }
	}
	public int z {
	    get { return SprExport.Spr_Vec4i_get_z(_this); }
	    set { SprExport.Spr_Vec4i_set_z(_this, value); }
	}
	public int w {
	    get { return SprExport.Spr_Vec4i_get_w(_this); }
	    set { SprExport.Spr_Vec4i_set_w(_this, value); }
	}
	public int square() {
	    int result = (int) SprExport.Spr_Vec4i_square((IntPtr) _this);
	    return result;
	}
	public int norm() {
	    int result = (int) SprExport.Spr_Vec4i_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec4i_unitize((IntPtr) _this);
	}
    }
    public partial class Vec6f : CsObject {
	public Vec6f() { _this = SprExport.Spr_new_Vec6f(); _flag = true; }
	public Vec6f(IntPtr ptr) : base(ptr) {}
	public Vec6f(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec6f(float vx, float vy, float vz, float wx, float wy, float wz) { _this = SprExport.Spr_new_Vec6f_7(vx, vy, vz, wx, wy, wz); _flag = true; }
	~Vec6f() { if (_flag) { SprExport.Spr_delete_Vec6f(_this); _flag = false; } }
	public static explicit operator Vec6f(Vec6d d) {
	    Vec6f f = new Vec6f();
	    f.vx = (float) d.vx;
	    f.vy = (float) d.vy;
	    f.vz = (float) d.vz;
	    f.wx = (float) d.wx;
	    f.wy = (float) d.wy;
	    f.wz = (float) d.wz;
	    return f;
	}
	public static Vec6f operator- (Vec6f a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec6f(a);
	    return new Vec6f(ptr, true);
	}
	public static Vec6f operator+ (Vec6f a, Vec6f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec6f(a, b);
	    return new Vec6f(ptr, true);
	}
	public static Vec6f operator- (Vec6f a, Vec6f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec6f(a, b);
	    return new Vec6f(ptr, true);
	}
	public static Vec6f operator* (float a, Vec6f b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vfv_Vec6f(a, b);
	    return new Vec6f(ptr, true);
	}
	public static Vec6f operator* (Vec6f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvf_Vec6f(a, b);
	    return new Vec6f(ptr, true);
	}
	public static Vec6f operator/ (Vec6f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvf_Vec6f(a, b);
	    return new Vec6f(ptr, true);
	}
	public static bool operator== (Vec6f a, Vec6f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec6f(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec6f a, Vec6f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec6f(a, b);
	    return (ret == 0) ? false : true;
	}
	public float this[int i] {
	    get { return (float) SprExport.Spr_unary_operator_index_fi_get_Vec6f(this, i); }
	    set { SprExport.Spr_unary_operator_index_fi_set_Vec6f(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec6f_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec6f_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float vx {
	    get { return SprExport.Spr_Vec6f_get_vx(_this); }
	    set { SprExport.Spr_Vec6f_set_vx(_this, value); }
	}
	public float vy {
	    get { return SprExport.Spr_Vec6f_get_vy(_this); }
	    set { SprExport.Spr_Vec6f_set_vy(_this, value); }
	}
	public float vz {
	    get { return SprExport.Spr_Vec6f_get_vz(_this); }
	    set { SprExport.Spr_Vec6f_set_vz(_this, value); }
	}
	public float wx {
	    get { return SprExport.Spr_Vec6f_get_wx(_this); }
	    set { SprExport.Spr_Vec6f_set_wx(_this, value); }
	}
	public float wy {
	    get { return SprExport.Spr_Vec6f_get_wy(_this); }
	    set { SprExport.Spr_Vec6f_set_wy(_this, value); }
	}
	public float wz {
	    get { return SprExport.Spr_Vec6f_get_wz(_this); }
	    set { SprExport.Spr_Vec6f_set_wz(_this, value); }
	}
	public float square() {
	    float result = (float) SprExport.Spr_Vec6f_square((IntPtr) _this);
	    return result;
	}
	public float norm() {
	    float result = (float) SprExport.Spr_Vec6f_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec6f_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec6fStruct : CsObject {
        public Vec6fStruct() {
            Vec6f desc = new Vec6f();
            ApplyFrom(desc, false);
        }
	public float vx;
	public float vy;
	public float vz;
	public float wx;
	public float wy;
	public float wz;
	public void ApplyTo(Vec6f r, bool apply_to_base_class = false) {
		r.vx = vx;
		r.vy = vy;
		r.vz = vz;
		r.wx = wx;
		r.wy = wy;
		r.wz = wz;
	}
	public void ApplyFrom(Vec6f r, bool apply_to_base_class = false) {
		vx = r.vx;
		vy = r.vy;
		vz = r.vz;
		wx = r.wx;
		wy = r.wy;
		wz = r.wz;
	}
	public static implicit operator Vec6f(Vec6fStruct m) {
	    Vec6f r = new Vec6f();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec6fStruct(Vec6f r) {
	    Vec6fStruct m = new Vec6fStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator Vec6dStruct(Vec6fStruct f) {
	    Vec6dStruct d = new Vec6dStruct();
	    d.vx = f.vx;
	    d.vy = f.vy;
	    d.vz = f.vz;
	    d.wx = f.wx;
	    d.wy = f.wy;
	    d.wz = f.wz;
	    return d;
	}
    }
    public partial class Vec6d : CsObject {
	public Vec6d() { _this = SprExport.Spr_new_Vec6d(); _flag = true; }
	public Vec6d(IntPtr ptr) : base(ptr) {}
	public Vec6d(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Vec6d(double vx, double vy, double vz, double wx, double wy, double wz) { _this = SprExport.Spr_new_Vec6d_7(vx, vy, vz, wx, wy, wz); _flag = true; }
	~Vec6d() { if (_flag) { SprExport.Spr_delete_Vec6d(_this); _flag = false; } }
	public static implicit operator Vec6d(Vec6f f) {
	    Vec6d d = new Vec6d();
	    d.vx = f.vx;
	    d.vy = f.vy;
	    d.vz = f.vz;
	    d.wx = f.wx;
	    d.wy = f.wy;
	    d.wz = f.wz;
	    return d;
	}
	public static Vec6d operator- (Vec6d a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_vv_Vec6d(a);
	    return new Vec6d(ptr, true);
	}
	public static Vec6d operator+ (Vec6d a, Vec6d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_vvv_Vec6d(a, b);
	    return new Vec6d(ptr, true);
	}
	public static Vec6d operator- (Vec6d a, Vec6d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_vvv_Vec6d(a, b);
	    return new Vec6d(ptr, true);
	}
	public static Vec6d operator* (double a, Vec6d b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vdv_Vec6d(a, b);
	    return new Vec6d(ptr, true);
	}
	public static Vec6d operator* (Vec6d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_vvd_Vec6d(a, b);
	    return new Vec6d(ptr, true);
	}
	public static Vec6d operator/ (Vec6d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_div_vvd_Vec6d(a, b);
	    return new Vec6d(ptr, true);
	}
	public static bool operator== (Vec6d a, Vec6d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return ap && bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_eq_bvv_Vec6d(a, b);
	    return (ret == 0) ? false : true;
	}
	public override int GetHashCode() { return base.GetHashCode(); }
	public override bool Equals(object o) { return true; }
	public static bool operator!= (Vec6d a, Vec6d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    if (ap || bp) { return !ap || !bp; }
	    byte ret = (byte) SprExport.Spr_binary_operator_ne_bvv_Vec6d(a, b);
	    return (ret == 0) ? false : true;
	}
	public double this[int i] {
	    get { return (double) SprExport.Spr_unary_operator_index_di_get_Vec6d(this, i); }
	    set { SprExport.Spr_unary_operator_index_di_set_Vec6d(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Vec6d_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Vec6d_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double vx {
	    get { return SprExport.Spr_Vec6d_get_vx(_this); }
	    set { SprExport.Spr_Vec6d_set_vx(_this, value); }
	}
	public double vy {
	    get { return SprExport.Spr_Vec6d_get_vy(_this); }
	    set { SprExport.Spr_Vec6d_set_vy(_this, value); }
	}
	public double vz {
	    get { return SprExport.Spr_Vec6d_get_vz(_this); }
	    set { SprExport.Spr_Vec6d_set_vz(_this, value); }
	}
	public double wx {
	    get { return SprExport.Spr_Vec6d_get_wx(_this); }
	    set { SprExport.Spr_Vec6d_set_wx(_this, value); }
	}
	public double wy {
	    get { return SprExport.Spr_Vec6d_get_wy(_this); }
	    set { SprExport.Spr_Vec6d_set_wy(_this, value); }
	}
	public double wz {
	    get { return SprExport.Spr_Vec6d_get_wz(_this); }
	    set { SprExport.Spr_Vec6d_set_wz(_this, value); }
	}
	public double square() {
	    double result = (double) SprExport.Spr_Vec6d_square((IntPtr) _this);
	    return result;
	}
	public double norm() {
	    double result = (double) SprExport.Spr_Vec6d_norm((IntPtr) _this);
	    return result;
	}
	public void unitize() {
	    SprExport.Spr_Vec6d_unitize((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Vec6dStruct : CsObject {
        public Vec6dStruct() {
            Vec6d desc = new Vec6d();
            ApplyFrom(desc, false);
        }
	public double vx;
	public double vy;
	public double vz;
	public double wx;
	public double wy;
	public double wz;
	public void ApplyTo(Vec6d r, bool apply_to_base_class = false) {
		r.vx = vx;
		r.vy = vy;
		r.vz = vz;
		r.wx = wx;
		r.wy = wy;
		r.wz = wz;
	}
	public void ApplyFrom(Vec6d r, bool apply_to_base_class = false) {
		vx = r.vx;
		vy = r.vy;
		vz = r.vz;
		wx = r.wx;
		wy = r.wy;
		wz = r.wz;
	}
	public static implicit operator Vec6d(Vec6dStruct m) {
	    Vec6d r = new Vec6d();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Vec6dStruct(Vec6d r) {
	    Vec6dStruct m = new Vec6dStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator Vec6fStruct(Vec6dStruct d) {
	    Vec6fStruct f = new Vec6fStruct();
	    f.vx = (float) d.vx;
	    f.vy = (float) d.vy;
	    f.vz = (float) d.vz;
	    f.wx = (float) d.wx;
	    f.wy = (float) d.wy;
	    f.wz = (float) d.wz;
	    return f;
	}
    }
    public partial class Curve3f : CsObject {
	public Curve3f() { _this = SprExport.Spr_new_Curve3f(); _flag = true; }
	public Curve3f(IntPtr ptr) : base(ptr) {}
	public Curve3f(IntPtr ptr, bool flag) : base(ptr, flag) {}
	~Curve3f() { if (_flag) { SprExport.Spr_delete_Curve3f(_this); _flag = false; } }
    }
    [System.Serializable]
    public class Curve3fStruct : CsObject {
        public Curve3fStruct() {
            Curve3f desc = new Curve3f();
            ApplyFrom(desc, false);
        }
	public void ApplyTo(Curve3f r, bool apply_to_base_class = false) {
	}
	public void ApplyFrom(Curve3f r, bool apply_to_base_class = false) {
	}
	public static implicit operator Curve3f(Curve3fStruct m) {
	    Curve3f r = new Curve3f();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Curve3fStruct(Curve3f r) {
	    Curve3fStruct m = new Curve3fStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
    }
    public partial class Matrix3f : CsObject {
	public Matrix3f() { _this = SprExport.Spr_new_Matrix3f(); _flag = true; }
	public Matrix3f(IntPtr ptr) : base(ptr) {}
	public Matrix3f(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Matrix3f(Vec3f ex, Vec3f ey, Vec3f ez) { _this = SprExport.Spr_new_Matrix3f_10(ex, ey, ez); _flag = true; }
	public Matrix3f(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) { _this = SprExport.Spr_new_Matrix3f_11(m11, m12, m13, m21, m22, m23, m31, m32, m33); _flag = true; }
	~Matrix3f() { if (_flag) { SprExport.Spr_delete_Matrix3f(_this); _flag = false; } }
	public static explicit operator Matrix3f(Matrix3d d) {
	    Matrix3f f = new Matrix3f();
	    f.xx = (float) d.xx;
	    f.xy = (float) d.xy;
	    f.xz = (float) d.xz;
	    f.yx = (float) d.yx;
	    f.yy = (float) d.yy;
	    f.yz = (float) d.yz;
	    f.zx = (float) d.zx;
	    f.zy = (float) d.zy;
	    f.zz = (float) d.zz;
	    return f;
	}
	public static Matrix3f operator- (Matrix3f a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_mm_Matrix3f(a);
	    return new Matrix3f(ptr, true);
	}
	public static Matrix3f operator+ (Matrix3f a, Matrix3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_mmm_Matrix3f(a, b);
	    return new Matrix3f(ptr, true);
	}
	public static Matrix3f operator- (Matrix3f a, Matrix3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_mmm_Matrix3f(a, b);
	    return new Matrix3f(ptr, true);
	}
	public static Matrix3f operator* (float a, Matrix3f b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_mfm_Matrix3f(a, b);
	    return new Matrix3f(ptr, true);
	}
	public static Matrix3f operator* (Matrix3f a, float b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_mmf_Matrix3f(a, b);
	    return new Matrix3f(ptr, true);
	}
	public static Vec3f operator* (Matrix3f a, Vec3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vmv_Matrix3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public static Vec3f operator* (Vec3f a, Matrix3f b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vvm_Matrix3f(a, b);
	    return new Vec3f(ptr, true);
	}
	public Vec3f this[int i] {
	    get {
	        IntPtr ptr = SprExport.Spr_unary_operator_index_vi_get_Matrix3f(this, i);
	        return new Vec3f(ptr, true);
	    }
	    set { SprExport.Spr_unary_operator_index_vi_set_Matrix3f(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Matrix3f_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Matrix3f_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float xx {
	    get { return SprExport.Spr_Matrix3f_get_xx(_this); }
	    set { SprExport.Spr_Matrix3f_set_xx(_this, value); }
	}
	public float xy {
	    get { return SprExport.Spr_Matrix3f_get_xy(_this); }
	    set { SprExport.Spr_Matrix3f_set_xy(_this, value); }
	}
	public float xz {
	    get { return SprExport.Spr_Matrix3f_get_xz(_this); }
	    set { SprExport.Spr_Matrix3f_set_xz(_this, value); }
	}
	public float yx {
	    get { return SprExport.Spr_Matrix3f_get_yx(_this); }
	    set { SprExport.Spr_Matrix3f_set_yx(_this, value); }
	}
	public float yy {
	    get { return SprExport.Spr_Matrix3f_get_yy(_this); }
	    set { SprExport.Spr_Matrix3f_set_yy(_this, value); }
	}
	public float yz {
	    get { return SprExport.Spr_Matrix3f_get_yz(_this); }
	    set { SprExport.Spr_Matrix3f_set_yz(_this, value); }
	}
	public float zx {
	    get { return SprExport.Spr_Matrix3f_get_zx(_this); }
	    set { SprExport.Spr_Matrix3f_set_zx(_this, value); }
	}
	public float zy {
	    get { return SprExport.Spr_Matrix3f_get_zy(_this); }
	    set { SprExport.Spr_Matrix3f_set_zy(_this, value); }
	}
	public float zz {
	    get { return SprExport.Spr_Matrix3f_get_zz(_this); }
	    set { SprExport.Spr_Matrix3f_set_zz(_this, value); }
	}
	public ulong height() {
	    ulong result = (ulong) SprExport.Spr_Matrix3f_height((IntPtr) _this);
	    return result;
	}
	public ulong width() {
	    ulong result = (ulong) SprExport.Spr_Matrix3f_width((IntPtr) _this);
	    return result;
	}
	public void resize(ulong h, ulong w) {
	    SprExport.Spr_Matrix3f_resize((IntPtr) _this, (ulong) h, (ulong) w);
	}
	public ulong stride() {
	    ulong result = (ulong) SprExport.Spr_Matrix3f_stride((IntPtr) _this);
	    return result;
	}
	public void clear() {
	    SprExport.Spr_Matrix3f_clear((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Matrix3fStruct : CsObject {
        public Matrix3fStruct() {
            Matrix3f desc = new Matrix3f();
            ApplyFrom(desc, false);
        }
	public float xx;
	public float xy;
	public float xz;
	public float yx;
	public float yy;
	public float yz;
	public float zx;
	public float zy;
	public float zz;
	public void ApplyTo(Matrix3f r, bool apply_to_base_class = false) {
		r.xx = xx;
		r.xy = xy;
		r.xz = xz;
		r.yx = yx;
		r.yy = yy;
		r.yz = yz;
		r.zx = zx;
		r.zy = zy;
		r.zz = zz;
	}
	public void ApplyFrom(Matrix3f r, bool apply_to_base_class = false) {
		xx = r.xx;
		xy = r.xy;
		xz = r.xz;
		yx = r.yx;
		yy = r.yy;
		yz = r.yz;
		zx = r.zx;
		zy = r.zy;
		zz = r.zz;
	}
	public static implicit operator Matrix3f(Matrix3fStruct m) {
	    Matrix3f r = new Matrix3f();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Matrix3fStruct(Matrix3f r) {
	    Matrix3fStruct m = new Matrix3fStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator Matrix3dStruct(Matrix3fStruct f) {
	    Matrix3dStruct d = new Matrix3dStruct();
	    d.xx = f.xx;
	    d.xy = f.xy;
	    d.xz = f.xz;
	    d.yx = f.yx;
	    d.yy = f.yy;
	    d.yz = f.yz;
	    d.zx = f.zx;
	    d.zy = f.zy;
	    d.zz = f.zz;
	    return d;
	}
    }
    public partial class Matrix3d : CsObject {
	public Matrix3d() { _this = SprExport.Spr_new_Matrix3d(); _flag = true; }
	public Matrix3d(IntPtr ptr) : base(ptr) {}
	public Matrix3d(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Matrix3d(Vec3d ex, Vec3d ey, Vec3d ez) { _this = SprExport.Spr_new_Matrix3d_10(ex, ey, ez); _flag = true; }
	public Matrix3d(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33) { _this = SprExport.Spr_new_Matrix3d_11(m11, m12, m13, m21, m22, m23, m31, m32, m33); _flag = true; }
	~Matrix3d() { if (_flag) { SprExport.Spr_delete_Matrix3d(_this); _flag = false; } }
	public static implicit operator Matrix3d(Matrix3f f) {
	    Matrix3d d = new Matrix3d();
	    d.xx = f.xx;
	    d.xy = f.xy;
	    d.xz = f.xz;
	    d.yx = f.yx;
	    d.yy = f.yy;
	    d.yz = f.yz;
	    d.zx = f.zx;
	    d.zy = f.zy;
	    d.zz = f.zz;
	    return d;
	}
	public static Matrix3d operator- (Matrix3d a) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_unary_operator_neg_mm_Matrix3d(a);
	    return new Matrix3d(ptr, true);
	}
	public static Matrix3d operator+ (Matrix3d a, Matrix3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_add_mmm_Matrix3d(a, b);
	    return new Matrix3d(ptr, true);
	}
	public static Matrix3d operator- (Matrix3d a, Matrix3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sub_mmm_Matrix3d(a, b);
	    return new Matrix3d(ptr, true);
	}
	public static Matrix3d operator* (double a, Matrix3d b) {
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_mdm_Matrix3d(a, b);
	    return new Matrix3d(ptr, true);
	}
	public static Matrix3d operator* (Matrix3d a, double b) {
	    bool ap = (object) a == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_mul_mmd_Matrix3d(a, b);
	    return new Matrix3d(ptr, true);
	}
	public static Vec3d operator* (Matrix3d a, Vec3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vmv_Matrix3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public static Vec3d operator* (Vec3d a, Matrix3d b) {
	    bool ap = (object) a == null;
	    bool bp = (object) b == null;
	    IntPtr ptr = SprExport.Spr_binary_operator_sprod_vvm_Matrix3d(a, b);
	    return new Vec3d(ptr, true);
	}
	public Vec3d this[int i] {
	    get {
	        IntPtr ptr = SprExport.Spr_unary_operator_index_vi_get_Matrix3d(this, i);
	        return new Vec3d(ptr, true);
	    }
	    set { SprExport.Spr_unary_operator_index_vi_set_Matrix3d(this, i, value); }
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Matrix3d_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Matrix3d_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double xx {
	    get { return SprExport.Spr_Matrix3d_get_xx(_this); }
	    set { SprExport.Spr_Matrix3d_set_xx(_this, value); }
	}
	public double xy {
	    get { return SprExport.Spr_Matrix3d_get_xy(_this); }
	    set { SprExport.Spr_Matrix3d_set_xy(_this, value); }
	}
	public double xz {
	    get { return SprExport.Spr_Matrix3d_get_xz(_this); }
	    set { SprExport.Spr_Matrix3d_set_xz(_this, value); }
	}
	public double yx {
	    get { return SprExport.Spr_Matrix3d_get_yx(_this); }
	    set { SprExport.Spr_Matrix3d_set_yx(_this, value); }
	}
	public double yy {
	    get { return SprExport.Spr_Matrix3d_get_yy(_this); }
	    set { SprExport.Spr_Matrix3d_set_yy(_this, value); }
	}
	public double yz {
	    get { return SprExport.Spr_Matrix3d_get_yz(_this); }
	    set { SprExport.Spr_Matrix3d_set_yz(_this, value); }
	}
	public double zx {
	    get { return SprExport.Spr_Matrix3d_get_zx(_this); }
	    set { SprExport.Spr_Matrix3d_set_zx(_this, value); }
	}
	public double zy {
	    get { return SprExport.Spr_Matrix3d_get_zy(_this); }
	    set { SprExport.Spr_Matrix3d_set_zy(_this, value); }
	}
	public double zz {
	    get { return SprExport.Spr_Matrix3d_get_zz(_this); }
	    set { SprExport.Spr_Matrix3d_set_zz(_this, value); }
	}
	public ulong height() {
	    ulong result = (ulong) SprExport.Spr_Matrix3d_height((IntPtr) _this);
	    return result;
	}
	public ulong width() {
	    ulong result = (ulong) SprExport.Spr_Matrix3d_width((IntPtr) _this);
	    return result;
	}
	public void resize(ulong h, ulong w) {
	    SprExport.Spr_Matrix3d_resize((IntPtr) _this, (ulong) h, (ulong) w);
	}
	public ulong stride() {
	    ulong result = (ulong) SprExport.Spr_Matrix3d_stride((IntPtr) _this);
	    return result;
	}
	public void clear() {
	    SprExport.Spr_Matrix3d_clear((IntPtr) _this);
	}
    }
    [System.Serializable]
    public class Matrix3dStruct : CsObject {
        public Matrix3dStruct() {
            Matrix3d desc = new Matrix3d();
            ApplyFrom(desc, false);
        }
	public double xx;
	public double xy;
	public double xz;
	public double yx;
	public double yy;
	public double yz;
	public double zx;
	public double zy;
	public double zz;
	public void ApplyTo(Matrix3d r, bool apply_to_base_class = false) {
		r.xx = xx;
		r.xy = xy;
		r.xz = xz;
		r.yx = yx;
		r.yy = yy;
		r.yz = yz;
		r.zx = zx;
		r.zy = zy;
		r.zz = zz;
	}
	public void ApplyFrom(Matrix3d r, bool apply_to_base_class = false) {
		xx = r.xx;
		xy = r.xy;
		xz = r.xz;
		yx = r.yx;
		yy = r.yy;
		yz = r.yz;
		zx = r.zx;
		zy = r.zy;
		zz = r.zz;
	}
	public static implicit operator Matrix3d(Matrix3dStruct m) {
	    Matrix3d r = new Matrix3d();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator Matrix3dStruct(Matrix3d r) {
	    Matrix3dStruct m = new Matrix3dStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator Matrix3fStruct(Matrix3dStruct d) {
	    Matrix3fStruct f = new Matrix3fStruct();
	    f.xx = (float) d.xx;
	    f.xy = (float) d.xy;
	    f.xz = (float) d.xz;
	    f.yx = (float) d.yx;
	    f.yy = (float) d.yy;
	    f.yz = (float) d.yz;
	    f.zx = (float) d.zx;
	    f.zy = (float) d.zy;
	    f.zz = (float) d.zz;
	    return f;
	}
    }
    public partial class Affinef : CsObject {
	public Affinef() { _this = SprExport.Spr_new_Affinef(); _flag = true; }
	public Affinef(IntPtr ptr) : base(ptr) {}
	public Affinef(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Affinef(Affinef a) { _this = SprExport.Spr_new_Affinef_17(a); _flag = true; }
	~Affinef() { if (_flag) { SprExport.Spr_delete_Affinef(_this); _flag = false; } }
	public static explicit operator Affinef(Affined d) {
	    Affinef f = new Affinef();
	    f.xx = (float) d.xx;
	    f.xy = (float) d.xy;
	    f.xz = (float) d.xz;
	    f.xw = (float) d.xw;
	    f.yx = (float) d.yx;
	    f.yy = (float) d.yy;
	    f.yz = (float) d.yz;
	    f.yw = (float) d.yw;
	    f.zx = (float) d.zx;
	    f.zy = (float) d.zy;
	    f.zz = (float) d.zz;
	    f.zw = (float) d.zw;
	    f.px = (float) d.px;
	    f.py = (float) d.py;
	    f.pz = (float) d.pz;
	    f.pw = (float) d.pw;
	    return f;
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Affinef_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Affinef_FreeString((IntPtr) ptr);
            return bstr;
        }
	public float xx {
	    get { return SprExport.Spr_Affinef_get_xx(_this); }
	    set { SprExport.Spr_Affinef_set_xx(_this, value); }
	}
	public float xy {
	    get { return SprExport.Spr_Affinef_get_xy(_this); }
	    set { SprExport.Spr_Affinef_set_xy(_this, value); }
	}
	public float xz {
	    get { return SprExport.Spr_Affinef_get_xz(_this); }
	    set { SprExport.Spr_Affinef_set_xz(_this, value); }
	}
	public float xw {
	    get { return SprExport.Spr_Affinef_get_xw(_this); }
	    set { SprExport.Spr_Affinef_set_xw(_this, value); }
	}
	public float yx {
	    get { return SprExport.Spr_Affinef_get_yx(_this); }
	    set { SprExport.Spr_Affinef_set_yx(_this, value); }
	}
	public float yy {
	    get { return SprExport.Spr_Affinef_get_yy(_this); }
	    set { SprExport.Spr_Affinef_set_yy(_this, value); }
	}
	public float yz {
	    get { return SprExport.Spr_Affinef_get_yz(_this); }
	    set { SprExport.Spr_Affinef_set_yz(_this, value); }
	}
	public float yw {
	    get { return SprExport.Spr_Affinef_get_yw(_this); }
	    set { SprExport.Spr_Affinef_set_yw(_this, value); }
	}
	public float zx {
	    get { return SprExport.Spr_Affinef_get_zx(_this); }
	    set { SprExport.Spr_Affinef_set_zx(_this, value); }
	}
	public float zy {
	    get { return SprExport.Spr_Affinef_get_zy(_this); }
	    set { SprExport.Spr_Affinef_set_zy(_this, value); }
	}
	public float zz {
	    get { return SprExport.Spr_Affinef_get_zz(_this); }
	    set { SprExport.Spr_Affinef_set_zz(_this, value); }
	}
	public float zw {
	    get { return SprExport.Spr_Affinef_get_zw(_this); }
	    set { SprExport.Spr_Affinef_set_zw(_this, value); }
	}
	public float px {
	    get { return SprExport.Spr_Affinef_get_px(_this); }
	    set { SprExport.Spr_Affinef_set_px(_this, value); }
	}
	public float py {
	    get { return SprExport.Spr_Affinef_get_py(_this); }
	    set { SprExport.Spr_Affinef_set_py(_this, value); }
	}
	public float pz {
	    get { return SprExport.Spr_Affinef_get_pz(_this); }
	    set { SprExport.Spr_Affinef_set_pz(_this, value); }
	}
	public float pw {
	    get { return SprExport.Spr_Affinef_get_pw(_this); }
	    set { SprExport.Spr_Affinef_set_pw(_this, value); }
	}
    }
    [System.Serializable]
    public class AffinefStruct : CsObject {
        public AffinefStruct() {
            Affinef desc = new Affinef();
            ApplyFrom(desc, false);
        }
	public float xx;
	public float xy;
	public float xz;
	public float xw;
	public float yx;
	public float yy;
	public float yz;
	public float yw;
	public float zx;
	public float zy;
	public float zz;
	public float zw;
	public float px;
	public float py;
	public float pz;
	public float pw;
	public void ApplyTo(Affinef r, bool apply_to_base_class = false) {
		r.xx = xx;
		r.xy = xy;
		r.xz = xz;
		r.xw = xw;
		r.yx = yx;
		r.yy = yy;
		r.yz = yz;
		r.yw = yw;
		r.zx = zx;
		r.zy = zy;
		r.zz = zz;
		r.zw = zw;
		r.px = px;
		r.py = py;
		r.pz = pz;
		r.pw = pw;
	}
	public void ApplyFrom(Affinef r, bool apply_to_base_class = false) {
		xx = r.xx;
		xy = r.xy;
		xz = r.xz;
		xw = r.xw;
		yx = r.yx;
		yy = r.yy;
		yz = r.yz;
		yw = r.yw;
		zx = r.zx;
		zy = r.zy;
		zz = r.zz;
		zw = r.zw;
		px = r.px;
		py = r.py;
		pz = r.pz;
		pw = r.pw;
	}
	public static implicit operator Affinef(AffinefStruct m) {
	    Affinef r = new Affinef();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator AffinefStruct(Affinef r) {
	    AffinefStruct m = new AffinefStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static implicit operator AffinedStruct(AffinefStruct f) {
	    AffinedStruct d = new AffinedStruct();
	    d.xx = f.xx;
	    d.xy = f.xy;
	    d.xz = f.xz;
	    d.xw = f.xw;
	    d.yx = f.yx;
	    d.yy = f.yy;
	    d.yz = f.yz;
	    d.yw = f.yw;
	    d.zx = f.zx;
	    d.zy = f.zy;
	    d.zz = f.zz;
	    d.zw = f.zw;
	    d.px = f.px;
	    d.py = f.py;
	    d.pz = f.pz;
	    d.pw = f.pw;
	    return d;
	}
    }
    public partial class Affined : CsObject {
	public Affined() { _this = SprExport.Spr_new_Affined(); _flag = true; }
	public Affined(IntPtr ptr) : base(ptr) {}
	public Affined(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public Affined(Affined a) { _this = SprExport.Spr_new_Affined_17(a); _flag = true; }
	~Affined() { if (_flag) { SprExport.Spr_delete_Affined(_this); _flag = false; } }
	public static implicit operator Affined(Affinef f) {
	    Affined d = new Affined();
	    d.xx = f.xx;
	    d.xy = f.xy;
	    d.xz = f.xz;
	    d.xw = f.xw;
	    d.yx = f.yx;
	    d.yy = f.yy;
	    d.yz = f.yz;
	    d.yw = f.yw;
	    d.zx = f.zx;
	    d.zy = f.zy;
	    d.zz = f.zz;
	    d.zw = f.zw;
	    d.px = f.px;
	    d.py = f.py;
	    d.pz = f.pz;
	    d.pw = f.pw;
	    return d;
	}
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_Affined_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_Affined_FreeString((IntPtr) ptr);
            return bstr;
        }
	public double xx {
	    get { return SprExport.Spr_Affined_get_xx(_this); }
	    set { SprExport.Spr_Affined_set_xx(_this, value); }
	}
	public double xy {
	    get { return SprExport.Spr_Affined_get_xy(_this); }
	    set { SprExport.Spr_Affined_set_xy(_this, value); }
	}
	public double xz {
	    get { return SprExport.Spr_Affined_get_xz(_this); }
	    set { SprExport.Spr_Affined_set_xz(_this, value); }
	}
	public double xw {
	    get { return SprExport.Spr_Affined_get_xw(_this); }
	    set { SprExport.Spr_Affined_set_xw(_this, value); }
	}
	public double yx {
	    get { return SprExport.Spr_Affined_get_yx(_this); }
	    set { SprExport.Spr_Affined_set_yx(_this, value); }
	}
	public double yy {
	    get { return SprExport.Spr_Affined_get_yy(_this); }
	    set { SprExport.Spr_Affined_set_yy(_this, value); }
	}
	public double yz {
	    get { return SprExport.Spr_Affined_get_yz(_this); }
	    set { SprExport.Spr_Affined_set_yz(_this, value); }
	}
	public double yw {
	    get { return SprExport.Spr_Affined_get_yw(_this); }
	    set { SprExport.Spr_Affined_set_yw(_this, value); }
	}
	public double zx {
	    get { return SprExport.Spr_Affined_get_zx(_this); }
	    set { SprExport.Spr_Affined_set_zx(_this, value); }
	}
	public double zy {
	    get { return SprExport.Spr_Affined_get_zy(_this); }
	    set { SprExport.Spr_Affined_set_zy(_this, value); }
	}
	public double zz {
	    get { return SprExport.Spr_Affined_get_zz(_this); }
	    set { SprExport.Spr_Affined_set_zz(_this, value); }
	}
	public double zw {
	    get { return SprExport.Spr_Affined_get_zw(_this); }
	    set { SprExport.Spr_Affined_set_zw(_this, value); }
	}
	public double px {
	    get { return SprExport.Spr_Affined_get_px(_this); }
	    set { SprExport.Spr_Affined_set_px(_this, value); }
	}
	public double py {
	    get { return SprExport.Spr_Affined_get_py(_this); }
	    set { SprExport.Spr_Affined_set_py(_this, value); }
	}
	public double pz {
	    get { return SprExport.Spr_Affined_get_pz(_this); }
	    set { SprExport.Spr_Affined_set_pz(_this, value); }
	}
	public double pw {
	    get { return SprExport.Spr_Affined_get_pw(_this); }
	    set { SprExport.Spr_Affined_set_pw(_this, value); }
	}
    }
    [System.Serializable]
    public class AffinedStruct : CsObject {
        public AffinedStruct() {
            Affined desc = new Affined();
            ApplyFrom(desc, false);
        }
	public double xx;
	public double xy;
	public double xz;
	public double xw;
	public double yx;
	public double yy;
	public double yz;
	public double yw;
	public double zx;
	public double zy;
	public double zz;
	public double zw;
	public double px;
	public double py;
	public double pz;
	public double pw;
	public void ApplyTo(Affined r, bool apply_to_base_class = false) {
		r.xx = xx;
		r.xy = xy;
		r.xz = xz;
		r.xw = xw;
		r.yx = yx;
		r.yy = yy;
		r.yz = yz;
		r.yw = yw;
		r.zx = zx;
		r.zy = zy;
		r.zz = zz;
		r.zw = zw;
		r.px = px;
		r.py = py;
		r.pz = pz;
		r.pw = pw;
	}
	public void ApplyFrom(Affined r, bool apply_to_base_class = false) {
		xx = r.xx;
		xy = r.xy;
		xz = r.xz;
		xw = r.xw;
		yx = r.yx;
		yy = r.yy;
		yz = r.yz;
		yw = r.yw;
		zx = r.zx;
		zy = r.zy;
		zz = r.zz;
		zw = r.zw;
		px = r.px;
		py = r.py;
		pz = r.pz;
		pw = r.pw;
	}
	public static implicit operator Affined(AffinedStruct m) {
	    Affined r = new Affined();
	    m.ApplyTo(r, true);
	    return r;
	}
	public static implicit operator AffinedStruct(Affined r) {
	    AffinedStruct m = new AffinedStruct();
	    m.ApplyFrom(r, true);
	    return m;
	}
	public static explicit operator AffinefStruct(AffinedStruct d) {
	    AffinefStruct f = new AffinefStruct();
	    f.xx = (float) d.xx;
	    f.xy = (float) d.xy;
	    f.xz = (float) d.xz;
	    f.xw = (float) d.xw;
	    f.yx = (float) d.yx;
	    f.yy = (float) d.yy;
	    f.yz = (float) d.yz;
	    f.yw = (float) d.yw;
	    f.zx = (float) d.zx;
	    f.zy = (float) d.zy;
	    f.zz = (float) d.zz;
	    f.zw = (float) d.zw;
	    f.px = (float) d.px;
	    f.py = (float) d.py;
	    f.pz = (float) d.pz;
	    f.pw = (float) d.pw;
	    return f;
	}
    }
    public partial class SpatialVector : CsObject {
	public SpatialVector() { _this = SprExport.Spr_new_SpatialVector(); _flag = true; }
	public SpatialVector(IntPtr ptr) : base(ptr) {}
	public SpatialVector(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public SpatialVector(Vec3d _v, Vec3d _w) { _this = SprExport.Spr_new_SpatialVector_2(_v, _w); _flag = true; }
	~SpatialVector() { if (_flag) { SprExport.Spr_delete_SpatialVector(_this); _flag = false; } }
	public Vec3d v() {
	    IntPtr ptr = SprExport.Spr_SpatialVector_v((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
	public Vec3d w() {
	    IntPtr ptr = SprExport.Spr_SpatialVector_w((IntPtr) _this);
            return new Vec3d(ptr, true);
	}
    }
    public partial class ObjectIf : CsObject {
	public ObjectIf() { _this = SprExport.Spr_new_ObjectIf(); _flag = true; }
	public ObjectIf(IntPtr ptr) : base(ptr) {}
	public ObjectIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator ObjectIf(CsCastObject target) {
		return (target._info.Inherit(ObjectIf.GetIfInfoStatic()) ? new ObjectIf(target._this, target._flag) : null);
	}
	~ObjectIf() { if (_flag) { SprExport.Spr_delete_ObjectIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_ObjectIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_ObjectIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
            IntPtr ptr = IntPtr.Zero;
            if(_thisArray[0] != IntPtr.Zero) {
	            ptr = SprExport.Spr_ObjectIf_GetIfInfo((IntPtr) _thisArray[0]);
            } else if(_thisArray[1] != IntPtr.Zero) {
	            ptr = SprExport.Spr_ObjectIf_GetIfInfo((IntPtr) _thisArray[1]);
            } else if(_thisArray[2] != IntPtr.Zero) {
	            ptr = SprExport.Spr_ObjectIf_GetIfInfo((IntPtr) _thisArray[2]);
            } else {
	            ptr = SprExport.Spr_ObjectIf_GetIfInfo((IntPtr) _this);
                var newIfInfo = new IfInfo(ptr);
                Console.WriteLine(newIfInfo.ClassName() + " use _this");
                return newIfInfo;
            }
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_ObjectIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public int AddRef() {
	    int result = (int) SprExport.Spr_ObjectIf_AddRef((IntPtr) _this);
	    return result;
	}
	public int RefCount() {
	    int result = (int) SprExport.Spr_ObjectIf_RefCount((IntPtr) _this);
	    return result;
	}
	public ulong NChildObject() {
	    ulong result = (ulong) SprExport.Spr_ObjectIf_NChildObject((IntPtr) _this);
	    return result;
	}
	public ObjectIf GetChildObject(ulong pos) {
	    IntPtr ptr = SprExport.Spr_ObjectIf_GetChildObject((IntPtr) _this, (ulong) pos);
            if (ptr == IntPtr.Zero) { return null; } 
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == NamedObjectIf.GetIfInfoStatic()) { return new NamedObjectIf(ptr); }
            if (obj.GetIfInfo() == ObjectStatesIf.GetIfInfoStatic()) { return new ObjectStatesIf(ptr); }
            if (obj.GetIfInfo() == UTTypeDescIf.GetIfInfoStatic()) { return new UTTypeDescIf(ptr); }
            if (obj.GetIfInfo() == UTTypeDescDbIf.GetIfInfoStatic()) { return new UTTypeDescDbIf(ptr); }
            if (obj.GetIfInfo() == UTTimerIf.GetIfInfoStatic()) { return new UTTimerIf(ptr); }
            if (obj.GetIfInfo() == SceneObjectIf.GetIfInfoStatic()) { return new SceneObjectIf(ptr); }
            if (obj.GetIfInfo() == NameManagerIf.GetIfInfoStatic()) { return new NameManagerIf(ptr); }
            if (obj.GetIfInfo() == SceneIf.GetIfInfoStatic()) { return new SceneIf(ptr); }
            if (obj.GetIfInfo() == SdkIf.GetIfInfoStatic()) { return new SdkIf(ptr); }
            return obj;
	}
	public bool AddChildObject(ObjectIf o) {
	    char ret = SprExport.Spr_ObjectIf_AddChildObject((IntPtr) _this, (IntPtr) o);
	    return (ret == 0) ? false : true;
	}
	public bool DelChildObject(ObjectIf o) {
            Console.WriteLine("DelChildObject(default) " + this.GetIfInfo().ClassName() + " " + o.GetIfInfo().ClassName()); // <!!> GravityEngineはC++内部で実装されてる？
            char ret0 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[0], (IntPtr)o._thisArray[0]);
            Console.WriteLine("DelChildObject _thisArray[0]");
            if (_thisArray[1] != IntPtr.Zero) {
                char ret1 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[1], (IntPtr)o._thisArray[1]);
            Console.WriteLine("DelChildObject _thisArray[1]");
            }
            if (_thisArray[2] != IntPtr.Zero) {
                char ret2 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr)_thisArray[2], (IntPtr)o._thisArray[2]);
            Console.WriteLine("DelChildObject _thisArray[1]");
            }
            //char ret2 = SprExport.Spr_ObjectIf_DelChildObject((IntPtr) _this, (IntPtr) o._this2); // 上手くいかない
            if(ret0 == 0) {
                Console.Write("failed DelChildObject");
                return false;
            } else {
                return true;
            }
            //return (ret0 == 0||ret1 == 0 || ret2 == 0) ? false : true;
	}
	public void Clear() {
	    SprExport.Spr_ObjectIf_Clear((IntPtr) _this);
	}
	public ObjectIf CreateObject(IfInfo info, CsObject desc) {
	    IntPtr ptr = SprExport.Spr_ObjectIf_CreateObject((IntPtr) _this, (IntPtr) info, (IntPtr) desc);
            if (ptr == IntPtr.Zero) { return null; } 
            ObjectIf obj = new ObjectIf(ptr);
            if (obj.GetIfInfo() == NamedObjectIf.GetIfInfoStatic()) { return new NamedObjectIf(ptr); }
            if (obj.GetIfInfo() == ObjectStatesIf.GetIfInfoStatic()) { return new ObjectStatesIf(ptr); }
            if (obj.GetIfInfo() == UTTypeDescIf.GetIfInfoStatic()) { return new UTTypeDescIf(ptr); }
            if (obj.GetIfInfo() == UTTypeDescDbIf.GetIfInfoStatic()) { return new UTTypeDescDbIf(ptr); }
            if (obj.GetIfInfo() == UTTimerIf.GetIfInfoStatic()) { return new UTTimerIf(ptr); }
            if (obj.GetIfInfo() == SceneObjectIf.GetIfInfoStatic()) { return new SceneObjectIf(ptr); }
            if (obj.GetIfInfo() == NameManagerIf.GetIfInfoStatic()) { return new NameManagerIf(ptr); }
            if (obj.GetIfInfo() == SceneIf.GetIfInfoStatic()) { return new SceneIf(ptr); }
            if (obj.GetIfInfo() == SdkIf.GetIfInfoStatic()) { return new SdkIf(ptr); }
            return obj;
	}
	public IntPtr GetDescAddress() {
	    IntPtr result = (IntPtr) SprExport.Spr_ObjectIf_GetDescAddress((IntPtr) _this);
	    return result;
	}
	public virtual bool GetDesc(CsObject desc) { // CDShapeIfの処理
	    char ret = (char)0; // <!!> これいいのか？
            ret = SprExport.Spr_ObjectIf_GetDesc((IntPtr)_this, (IntPtr)desc);
            return (ret == 0) ? false : true;
	}
	public virtual void SetDesc(CsObject desc) {
            // <!!> CDShapeは_thisだけしか作らないためnullチェックが必要、ここにもlockを掛ける必要があるがPHSceneIfにアクセスできない
        SprExport.Spr_ObjectIf_SetDesc((IntPtr)_this, (IntPtr)desc);
	}
	public ulong GetDescSize() {
	    ulong result = (ulong) SprExport.Spr_ObjectIf_GetDescSize((IntPtr) _this);
	    return result;
	}
	public IntPtr GetStateAddress() {
	    IntPtr result = (IntPtr) SprExport.Spr_ObjectIf_GetStateAddress((IntPtr) _this);
	    return result;
	}
	public bool GetState(CsObject state) {
	    char ret = SprExport.Spr_ObjectIf_GetState((IntPtr) _this, (IntPtr) state);
	    return (ret == 0) ? false : true;
	}
	public void SetState(CsObject state) {
	    SprExport.Spr_ObjectIf_SetState((IntPtr) _this, (IntPtr) state);
	}
	public bool WriteState(string fileName) {
            IntPtr remote1 = Marshal.StringToBSTR(fileName);
	    char ret = SprExport.Spr_ObjectIf_WriteState((IntPtr) _this, (IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
	    return (ret == 0) ? false : true;
	}
	public bool ReadState(string fileName) {
            IntPtr remote1 = Marshal.StringToBSTR(fileName);
	    char ret = SprExport.Spr_ObjectIf_ReadState((IntPtr) _this, (IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
	    return (ret == 0) ? false : true;
	}
	public ulong GetStateSize() {
	    ulong result = (ulong) SprExport.Spr_ObjectIf_GetStateSize((IntPtr) _this);
	    return result;
	}
	public void ConstructState(CsObject m) {
	    SprExport.Spr_ObjectIf_ConstructState((IntPtr) _this, (IntPtr) m);
	}
	public void DestructState(CsObject m) {
	    SprExport.Spr_ObjectIf_DestructState((IntPtr) _this, (IntPtr) m);
	}
    }
    public partial class NamedObjectIf : ObjectIf {
	public NamedObjectIf() { _this = SprExport.Spr_new_NamedObjectIf(); _flag = true; }
	public NamedObjectIf(IntPtr ptr) : base(ptr) {}
	public NamedObjectIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator NamedObjectIf(CsCastObject target) {
		return (target._info.Inherit(NamedObjectIf.GetIfInfoStatic()) ? new NamedObjectIf(target._this, target._flag) : null);
	}
	~NamedObjectIf() { if (_flag) { SprExport.Spr_delete_NamedObjectIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_NamedObjectIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_NamedObjectIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_NamedObjectIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_NamedObjectIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public string GetName() {
	    IntPtr ptr = SprExport.Spr_NamedObjectIf_GetName((IntPtr) _this);
	    return Marshal.PtrToStringAnsi(ptr);
	}
	public void SetName(string n) {
	    //SprExport.Spr_NamedObjectIf_SetName((IntPtr) _this, (string) n);
	}
	public NameManagerIf GetNameManager() {
	    IntPtr ptr = SprExport.Spr_NamedObjectIf_GetNameManager((IntPtr) _this);
            if (ptr == IntPtr.Zero) { return null; } 
            NameManagerIf obj = new NameManagerIf(ptr);
            if (obj.GetIfInfo() == SceneIf.GetIfInfoStatic()) { return new SceneIf(ptr); }
            if (obj.GetIfInfo() == SdkIf.GetIfInfoStatic()) { return new SdkIf(ptr); }
            return obj;
	}
    }
    public partial class SceneObjectIf : NamedObjectIf {
	public SceneObjectIf() { _this = SprExport.Spr_new_SceneObjectIf(); _flag = true; }
	public SceneObjectIf(IntPtr ptr) : base(ptr) {}
	public SceneObjectIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator SceneObjectIf(CsCastObject target) {
		return (target._info.Inherit(SceneObjectIf.GetIfInfoStatic()) ? new SceneObjectIf(target._this, target._flag) : null);
	}
	~SceneObjectIf() { if (_flag) { SprExport.Spr_delete_SceneObjectIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_SceneObjectIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_SceneObjectIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_SceneObjectIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_SceneObjectIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public SceneIf GetScene() {
	    IntPtr ptr = SprExport.Spr_SceneObjectIf_GetScene((IntPtr) _this);
            if (ptr == IntPtr.Zero) { return null; } 
            SceneIf obj = new SceneIf(ptr);
            return obj;
	}
	public SceneObjectIf CloneObject() {
	    IntPtr ptr = SprExport.Spr_SceneObjectIf_CloneObject((IntPtr) _this);
            if (ptr == IntPtr.Zero) { return null; } 
            SceneObjectIf obj = new SceneObjectIf(ptr);
            return obj;
	}
    }
    public partial class ObjectStatesIf : ObjectIf {
	public ObjectStatesIf() { _this = SprExport.Spr_new_ObjectStatesIf(); _flag = true; }
	public ObjectStatesIf(IntPtr ptr) : base(ptr) {}
	public ObjectStatesIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator ObjectStatesIf(CsCastObject target) {
		return (target._info.Inherit(ObjectStatesIf.GetIfInfoStatic()) ? new ObjectStatesIf(target._this, target._flag) : null);
	}
	~ObjectStatesIf() { if (_flag) { SprExport.Spr_delete_ObjectStatesIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_ObjectStatesIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_ObjectStatesIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_ObjectStatesIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_ObjectStatesIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public void AllocateState(ObjectIf o) {
	    SprExport.Spr_ObjectStatesIf_AllocateState((IntPtr) _this, (IntPtr) o);
	}
	public void ReleaseState(ObjectIf o) {
	    SprExport.Spr_ObjectStatesIf_ReleaseState((IntPtr) _this, (IntPtr) o);
	}
	public ulong CalcStateSize(ObjectIf o) {
	    ulong result = (ulong) SprExport.Spr_ObjectStatesIf_CalcStateSize((IntPtr) _this, (IntPtr) o);
	    return result;
	}
	public void SaveState(ObjectIf o) {
	    SprExport.Spr_ObjectStatesIf_SaveState((IntPtr) _this, (IntPtr) o);
	}
	public void LoadState(ObjectIf o) {
	    SprExport.Spr_ObjectStatesIf_LoadState((IntPtr) _this, (IntPtr) o);
	}
	public void SingleSave(ObjectIf o) {
	    SprExport.Spr_ObjectStatesIf_SingleSave((IntPtr) _this, (IntPtr) o);
	}
	public void SingleLoad(ObjectIf o) {
	    SprExport.Spr_ObjectStatesIf_SingleLoad((IntPtr) _this, (IntPtr) o);
	}
	public bool IsAllocated() {
	    char ret = SprExport.Spr_ObjectStatesIf_IsAllocated((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public static ObjectStatesIf Create() {
	    IntPtr ptr = SprExport.Spr_ObjectStatesIf_Create();
            if (ptr == IntPtr.Zero) { return null; } 
            ObjectStatesIf obj = new ObjectStatesIf(ptr);
            return obj;
	}
    }
    public partial class UTTypeDescIf : ObjectIf {
	public UTTypeDescIf() { _this = SprExport.Spr_new_UTTypeDescIf(); _flag = true; }
	public UTTypeDescIf(IntPtr ptr) : base(ptr) {}
	public UTTypeDescIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator UTTypeDescIf(CsCastObject target) {
		return (target._info.Inherit(UTTypeDescIf.GetIfInfoStatic()) ? new UTTypeDescIf(target._this, target._flag) : null);
	}
	~UTTypeDescIf() { if (_flag) { SprExport.Spr_delete_UTTypeDescIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_UTTypeDescIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTypeDescIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public static UTTypeDescIf FindTypeDesc(string typeName, string moduleName) {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_FindTypeDesc((string) typeName, (string) moduleName);
            if (ptr == IntPtr.Zero) { return null; } 
            UTTypeDescIf obj = new UTTypeDescIf(ptr);
            return obj;
	}
	public static UTTypeDescIf Create(string tn, int sz) {
            IntPtr remote1 = Marshal.StringToBSTR(tn);
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_Create((IntPtr) remote1, (int) sz);
            Marshal.FreeBSTR(remote1);
            if (ptr == IntPtr.Zero) { return null; } 
            UTTypeDescIf obj = new UTTypeDescIf(ptr);
            return obj;
	}
	public static UTTypeDescIf Create(string tn) {
            IntPtr remote1 = Marshal.StringToBSTR(tn);
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_Create_1((IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
            if (ptr == IntPtr.Zero) { return null; } 
            UTTypeDescIf obj = new UTTypeDescIf(ptr);
            return obj;
	}
	public void SetSize(ulong sz) {
	    SprExport.Spr_UTTypeDescIf_SetSize((IntPtr) _this, (ulong) sz);
	}
	public void SetAccess(UTAccessBase a) {
	    SprExport.Spr_UTTypeDescIf_SetAccess((IntPtr) _this, (IntPtr) a);
	}
	public void SetOffset(int field, int offset) {
	    SprExport.Spr_UTTypeDescIf_SetOffset((IntPtr) _this, (int) field, (int) offset);
	}
	public void SetIfInfo(IfInfo info) {
	    SprExport.Spr_UTTypeDescIf_SetIfInfo((IntPtr) _this, (IntPtr) info);
	}
	public int AddField(string pre, string ty, string n, string post) {
            IntPtr remote1 = Marshal.StringToBSTR(pre);
            IntPtr remote2 = Marshal.StringToBSTR(ty);
            IntPtr remote3 = Marshal.StringToBSTR(n);
            IntPtr remote4 = Marshal.StringToBSTR(post);
	    int result = (int) SprExport.Spr_UTTypeDescIf_AddField((IntPtr) _this, (IntPtr) remote1, (IntPtr) remote2, (IntPtr) remote3, (IntPtr) remote4);
            Marshal.FreeBSTR(remote1);
            Marshal.FreeBSTR(remote2);
            Marshal.FreeBSTR(remote3);
            Marshal.FreeBSTR(remote4);
	    return result;
	}
	public int AddBase(string tn) {
            IntPtr remote1 = Marshal.StringToBSTR(tn);
	    int result = (int) SprExport.Spr_UTTypeDescIf_AddBase((IntPtr) _this, (IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
	    return result;
	}
	public void AddEnumConst(int field, string name, int val) {
            IntPtr remote2 = Marshal.StringToBSTR(name);
	    SprExport.Spr_UTTypeDescIf_AddEnumConst((IntPtr) _this, (int) field, (IntPtr) remote2, (int) val);
            Marshal.FreeBSTR(remote2);
	}
	public string GetTypeName() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_GetTypeName((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTypeDescIf_FreeString_GetTypeName(ptr);
            return bstr;
	}
	public void SetTypeName(string s) {
	    SprExport.Spr_UTTypeDescIf_SetTypeName((IntPtr) _this, (string) s);
	}
	public ulong GetSize() {
	    ulong result = (ulong) SprExport.Spr_UTTypeDescIf_GetSize((IntPtr) _this);
	    return result;
	}
	public bool IsPrimitive() {
	    char ret = SprExport.Spr_UTTypeDescIf_IsPrimitive((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool IsSimple() {
	    char ret = SprExport.Spr_UTTypeDescIf_IsSimple((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public int NFields() {
	    int result = (int) SprExport.Spr_UTTypeDescIf_NFields((IntPtr) _this);
	    return result;
	}
	public int GetFieldLength(int i) {
	    int result = (int) SprExport.Spr_UTTypeDescIf_GetFieldLength((IntPtr) _this, (int) i);
	    return result;
	}
	public int GetFieldVectorSize(int i, CsObject base_) {
	    int result = (int) SprExport.Spr_UTTypeDescIf_GetFieldVectorSize((IntPtr) _this, (int) i, (IntPtr) base_);
	    return result;
	}
	public string GetFieldLengthName(int i) {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_GetFieldLengthName((IntPtr) _this, (int) i);
	    return Marshal.PtrToStringAnsi(ptr);
	}
	public FieldType GetFieldVarType(int i) {
	    int result = SprExport.Spr_UTTypeDescIf_GetFieldVarType((IntPtr) _this, (int) i);
            return (UTTypeDescIf.FieldType) result;
	}
	public bool GetFieldIsReference(int i) {
	    char ret = SprExport.Spr_UTTypeDescIf_GetFieldIsReference((IntPtr) _this, (int) i);
	    return (ret == 0) ? false : true;
	}
	public string GetFieldName(int i) {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_GetFieldName((IntPtr) _this, (int) i);
	    return Marshal.PtrToStringAnsi(ptr);
	}
	public IntPtr GetFieldAddress(int i, CsObject base_, int pos) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTypeDescIf_GetFieldAddress((IntPtr) _this, (int) i, (IntPtr) base_, (int) pos);
	    return result;
	}
	public IntPtr GetFieldAddressEx(int i, CsObject base_, int pos) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTypeDescIf_GetFieldAddressEx((IntPtr) _this, (int) i, (IntPtr) base_, (int) pos);
	    return result;
	}
	public string ReadToString(int i, CsObject base_, int pos) {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_ReadToString((IntPtr) _this, (int) i, (IntPtr) base_, (int) pos);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTypeDescIf_FreeString_ReadToString(ptr);
            return bstr;
	}
	public void WriteFromString(string from, int i, CsObject base_, int pos) {
            IntPtr remote1 = Marshal.StringToBSTR(from);
	    SprExport.Spr_UTTypeDescIf_WriteFromString((IntPtr) _this, (IntPtr) remote1, (int) i, (IntPtr) base_, (int) pos);
            Marshal.FreeBSTR(remote1);
	}
	public IfInfo GetIfInfoOfType() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_GetIfInfoOfType((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public bool IsBool() {
	    char ret = SprExport.Spr_UTTypeDescIf_IsBool((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool IsNumber() {
	    char ret = SprExport.Spr_UTTypeDescIf_IsNumber((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool IsString() {
	    char ret = SprExport.Spr_UTTypeDescIf_IsString((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool ReadBool(CsObject ptr_) {
	    char ret = SprExport.Spr_UTTypeDescIf_ReadBool((IntPtr) _this, (IntPtr) ptr_);
	    return (ret == 0) ? false : true;
	}
	public void WriteBool(bool val, CsObject ptr_) {
	    SprExport.Spr_UTTypeDescIf_WriteBool((IntPtr) _this, (bool) val, (IntPtr) ptr_);
	}
	public double ReadNumber(CsObject ptr_) {
	    double result = (double) SprExport.Spr_UTTypeDescIf_ReadNumber((IntPtr) _this, (IntPtr) ptr_);
	    return result;
	}
	public void WriteNumber(double val, CsObject ptr_) {
	    SprExport.Spr_UTTypeDescIf_WriteNumber((IntPtr) _this, (double) val, (IntPtr) ptr_);
	}
	public string ReadString(CsObject ptr_) {
	    IntPtr ptr = SprExport.Spr_UTTypeDescIf_ReadString((IntPtr) _this, (IntPtr) ptr_);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTypeDescIf_FreeString_ReadString(ptr);
            return bstr;
	}
	public void WriteString(string val, CsObject ptr_) {
	    SprExport.Spr_UTTypeDescIf_WriteString((IntPtr) _this, (string) val, (IntPtr) ptr_);
	}
	public IntPtr Create() {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTypeDescIf_Create_2((IntPtr) _this);
	    return result;
	}
	public void Delete(CsObject ptr_) {
	    SprExport.Spr_UTTypeDescIf_Delete((IntPtr) _this, (IntPtr) ptr_);
	}
	public IntPtr VectorPush(CsObject v) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTypeDescIf_VectorPush((IntPtr) _this, (IntPtr) v);
	    return result;
	}
	public void VectorPop(CsObject v) {
	    SprExport.Spr_UTTypeDescIf_VectorPop((IntPtr) _this, (IntPtr) v);
	}
	public IntPtr VectorAt(CsObject v, int pos) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTypeDescIf_VectorAt((IntPtr) _this, (IntPtr) v, (int) pos);
	    return result;
	}
	public ulong VectorSize(CsObject v) {
	    ulong result = (ulong) SprExport.Spr_UTTypeDescIf_VectorSize((IntPtr) _this, (IntPtr) v);
	    return result;
	}
	public ulong SizeOfVector() {
	    ulong result = (ulong) SprExport.Spr_UTTypeDescIf_SizeOfVector((IntPtr) _this);
	    return result;
	}
	public enum FieldType : int {
	    SINGLE = 0, ARRAY = SINGLE + 1, VECTOR = ARRAY + 1
	}
    }
    public partial class UTTypeDescDbIf : ObjectIf {
	public UTTypeDescDbIf() { _this = SprExport.Spr_new_UTTypeDescDbIf(); _flag = true; }
	public UTTypeDescDbIf(IntPtr ptr) : base(ptr) {}
	public UTTypeDescDbIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator UTTypeDescDbIf(CsCastObject target) {
		return (target._info.Inherit(UTTypeDescDbIf.GetIfInfoStatic()) ? new UTTypeDescDbIf(target._this, target._flag) : null);
	}
	~UTTypeDescDbIf() { if (_flag) { SprExport.Spr_delete_UTTypeDescDbIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_UTTypeDescDbIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTypeDescDbIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescDbIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescDbIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public static UTTypeDescDbIf GetDb(string n) {
            IntPtr remote1 = Marshal.StringToBSTR(n);
	    IntPtr ptr = SprExport.Spr_UTTypeDescDbIf_GetDb((IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
            if (ptr == IntPtr.Zero) { return null; } 
            UTTypeDescDbIf obj = new UTTypeDescDbIf(ptr);
            return obj;
	}
	public void RegisterDesc(UTTypeDescIf n) {
	    SprExport.Spr_UTTypeDescDbIf_RegisterDesc((IntPtr) _this, (IntPtr) n);
	}
	public void RegisterAlias(string src, string dest) {
	    SprExport.Spr_UTTypeDescDbIf_RegisterAlias((IntPtr) _this, (string) src, (string) dest);
	}
	public void SetPrefix(string p) {
            IntPtr remote1 = Marshal.StringToBSTR(p);
	    SprExport.Spr_UTTypeDescDbIf_SetPrefix((IntPtr) _this, (IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
	}
	public UTTypeDescIf Find(string tn) {
            IntPtr remote1 = Marshal.StringToBSTR(tn);
	    IntPtr ptr = SprExport.Spr_UTTypeDescDbIf_Find((IntPtr) _this, (IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
            if (ptr == IntPtr.Zero) { return null; } 
            UTTypeDescIf obj = new UTTypeDescIf(ptr);
            return obj;
	}
	public void Link(UTTypeDescDbIf db) {
	    SprExport.Spr_UTTypeDescDbIf_Link((IntPtr) _this, (IntPtr) db);
	}
	public void Link() {
	    SprExport.Spr_UTTypeDescDbIf_Link_1((IntPtr) _this);
	}
	public void LinkAll() {
	    SprExport.Spr_UTTypeDescDbIf_LinkAll((IntPtr) _this);
	}
	public bool LinkCheck() {
	    char ret = SprExport.Spr_UTTypeDescDbIf_LinkCheck((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public string GetGroup() {
	    IntPtr ptr = SprExport.Spr_UTTypeDescDbIf_GetGroup((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTypeDescDbIf_FreeString_GetGroup(ptr);
            return bstr;
	}
	public void Clear() {
	    SprExport.Spr_UTTypeDescDbIf_Clear((IntPtr) _this);
	}
    }
    public partial class NameManagerIf : NamedObjectIf {
	public NameManagerIf() { _this = SprExport.Spr_new_NameManagerIf(); _flag = true; }
	public NameManagerIf(IntPtr ptr) : base(ptr) {}
	public NameManagerIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator NameManagerIf(CsCastObject target) {
		return (target._info.Inherit(NameManagerIf.GetIfInfoStatic()) ? new NameManagerIf(target._this, target._flag) : null);
	}
	~NameManagerIf() { if (_flag) { SprExport.Spr_delete_NameManagerIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_NameManagerIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_NameManagerIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_NameManagerIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_NameManagerIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public NamedObjectIf FindObject(string name, string cls) {
            IntPtr remote1 = Marshal.StringToBSTR(name);
            IntPtr remote2 = Marshal.StringToBSTR(cls);
	    IntPtr ptr = SprExport.Spr_NameManagerIf_FindObject_2((IntPtr) _this, (IntPtr) remote1, (IntPtr) remote2);
            Marshal.FreeBSTR(remote1);
            Marshal.FreeBSTR(remote2);
            if (ptr == IntPtr.Zero) { return null; } 
            NamedObjectIf obj = new NamedObjectIf(ptr);
            if (obj.GetIfInfo() == SceneObjectIf.GetIfInfoStatic()) { return new SceneObjectIf(ptr); }
            if (obj.GetIfInfo() == NameManagerIf.GetIfInfoStatic()) { return new NameManagerIf(ptr); }
            if (obj.GetIfInfo() == SceneIf.GetIfInfoStatic()) { return new SceneIf(ptr); }
            if (obj.GetIfInfo() == SdkIf.GetIfInfoStatic()) { return new SdkIf(ptr); }
            return obj;
	}
	public NamedObjectIf FindObject(string name) {
            IntPtr remote1 = Marshal.StringToBSTR(name);
	    IntPtr ptr = SprExport.Spr_NameManagerIf_FindObject_3((IntPtr) _this, (IntPtr) remote1);
            Marshal.FreeBSTR(remote1);
            if (ptr == IntPtr.Zero) { return null; } 
            NamedObjectIf obj = new NamedObjectIf(ptr);
            if (obj.GetIfInfo() == SceneObjectIf.GetIfInfoStatic()) { return new SceneObjectIf(ptr); }
            if (obj.GetIfInfo() == NameManagerIf.GetIfInfoStatic()) { return new NameManagerIf(ptr); }
            if (obj.GetIfInfo() == SceneIf.GetIfInfoStatic()) { return new SceneIf(ptr); }
            if (obj.GetIfInfo() == SdkIf.GetIfInfoStatic()) { return new SdkIf(ptr); }
            return obj;
	}
    }
    public partial class SceneIf : NameManagerIf {
	public SceneIf() { _this = SprExport.Spr_new_SceneIf(); _flag = true; }
	public SceneIf(IntPtr ptr) : base(ptr) {}
	public SceneIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator SceneIf(CsCastObject target) {
		return (target._info.Inherit(SceneIf.GetIfInfoStatic()) ? new SceneIf(target._this, target._flag) : null);
	}
	~SceneIf() { if (_flag) { SprExport.Spr_delete_SceneIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_SceneIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_SceneIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_SceneIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_SceneIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
    }
    public partial class SdkIf : NameManagerIf {
	public SdkIf() { _this = SprExport.Spr_new_SdkIf(); _flag = true; }
	public SdkIf(IntPtr ptr) : base(ptr) {}
	public SdkIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator SdkIf(CsCastObject target) {
		return (target._info.Inherit(SdkIf.GetIfInfoStatic()) ? new SdkIf(target._this, target._flag) : null);
	}
	~SdkIf() { if (_flag) { SprExport.Spr_delete_SdkIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_SdkIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_SdkIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_SdkIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_SdkIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
    }
    public partial class UTTimerIf : ObjectIf {
	public UTTimerIf() { _this = SprExport.Spr_new_UTTimerIf(); _flag = true; }
	public UTTimerIf(IntPtr ptr) : base(ptr) {}
	public UTTimerIf(IntPtr ptr, bool flag) : base(ptr, flag) {}
	public static implicit operator UTTimerIf(CsCastObject target) {
		return (target._info.Inherit(UTTimerIf.GetIfInfoStatic()) ? new UTTimerIf(target._this, target._flag) : null);
	}
	~UTTimerIf() { if (_flag) { SprExport.Spr_delete_UTTimerIf(_this); _flag = false; } }
        public override string ToString() {
            IntPtr ptr = SprExport.Spr_UTTimerIf_ToString((IntPtr) _this);
            string bstr = Marshal.PtrToStringBSTR(ptr);
            SprExport.Spr_UTTimerIf_FreeString((IntPtr) ptr);
            return bstr;
        }
	public IfInfo GetIfInfo() {
	    IntPtr ptr = SprExport.Spr_UTTimerIf_GetIfInfo((IntPtr) _this);
            return new IfInfo(ptr);
	}
	public static IfInfo GetIfInfoStatic() {
	    IntPtr ptr = SprExport.Spr_UTTimerIf_GetIfInfoStatic();
            return new IfInfo(ptr);
	}
	public IntPtr TimerFunc(int id) {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTimerIf_TimerFunc((IntPtr) _this, (int) id);
	    return result;
	}
	public static uint NTimers() {
	    uint result = (uint) SprExport.Spr_UTTimerIf_NTimers();
	    return result;
	}
	public static UTTimerIf Get(uint id) {
	    IntPtr ptr = SprExport.Spr_UTTimerIf_Get((uint) id);
            if (ptr == IntPtr.Zero) { return null; } 
            UTTimerIf obj = new UTTimerIf(ptr);
            return obj;
	}
	public static UTTimerIf Create() {
	    IntPtr ptr = SprExport.Spr_UTTimerIf_Create();
            if (ptr == IntPtr.Zero) { return null; } 
            UTTimerIf obj = new UTTimerIf(ptr);
            return obj;
	}
	public uint GetID() {
	    uint result = (uint) SprExport.Spr_UTTimerIf_GetID((IntPtr) _this);
	    return result;
	}
	public uint GetResolution() {
	    uint result = (uint) SprExport.Spr_UTTimerIf_GetResolution((IntPtr) _this);
	    return result;
	}
	public bool SetResolution(uint r) {
	    char ret = SprExport.Spr_UTTimerIf_SetResolution((IntPtr) _this, (uint) r);
	    return (ret == 0) ? false : true;
	}
	public uint GetInterval() {
	    uint result = (uint) SprExport.Spr_UTTimerIf_GetInterval((IntPtr) _this);
	    return result;
	}
	public bool SetInterval(uint i) {
	    char ret = SprExport.Spr_UTTimerIf_SetInterval((IntPtr) _this, (uint) i);
	    return (ret == 0) ? false : true;
	}
	public IntPtr GetCallbackArg() {
	    IntPtr result = (IntPtr) SprExport.Spr_UTTimerIf_GetCallbackArg((IntPtr) _this);
	    return result;
	}
	public Mode GetMode() {
	    int result = SprExport.Spr_UTTimerIf_GetMode((IntPtr) _this);
            return (UTTimerIf.Mode) result;
	}
	public bool SetMode(UTTimerIf.Mode m) {
	    char ret = SprExport.Spr_UTTimerIf_SetMode((IntPtr) _this, (int) m);
	    return (ret == 0) ? false : true;
	}
	public bool IsStarted() {
	    char ret = SprExport.Spr_UTTimerIf_IsStarted((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool IsRunning() {
	    char ret = SprExport.Spr_UTTimerIf_IsRunning((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool Start() {
	    char ret = SprExport.Spr_UTTimerIf_Start((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public bool Stop() {
	    char ret = SprExport.Spr_UTTimerIf_Stop((IntPtr) _this);
	    return (ret == 0) ? false : true;
	}
	public void Call() {
	    SprExport.Spr_UTTimerIf_Call((IntPtr) _this);
	}
	public enum Mode : int {
	    MULTIMEDIA = 0, THREAD = MULTIMEDIA + 1, FRAMEWORK = THREAD + 1, IDLE = FRAMEWORK + 1
	}
    }
    public partial class IfInfo : CsObject {
	protected IfInfo() {}
	public IfInfo(IntPtr ptr) : base(ptr) { }
	public IfInfo(IntPtr ptr, bool flag) : base(ptr, flag) { }
	public bool Inherit(IfInfo info) {
	    char ret = SprExport.Spr_IfInfo_Inherit((IntPtr) _this, (IntPtr) info);
	    return (ret == 0) ? false : true;
	}
	public string ClassName() {
	    IntPtr ptr = SprExport.Spr_IfInfo_ClassName((IntPtr) _this);
	    return Marshal.PtrToStringAnsi(ptr);
	}
    }
	public partial class IfInfoToCsType {
		public static Dictionary<IntPtr, Type> mapFoundation = new Dictionary<IntPtr, Type>() {
			{ObjectIf.GetIfInfoStatic(), typeof(ObjectIf)},
			{NamedObjectIf.GetIfInfoStatic(), typeof(NamedObjectIf)},
			{SceneObjectIf.GetIfInfoStatic(), typeof(SceneObjectIf)},
			{ObjectStatesIf.GetIfInfoStatic(), typeof(ObjectStatesIf)},
			{UTTypeDescIf.GetIfInfoStatic(), typeof(UTTypeDescIf)},
			{UTTypeDescDbIf.GetIfInfoStatic(), typeof(UTTypeDescDbIf)},
			{NameManagerIf.GetIfInfoStatic(), typeof(NameManagerIf)},
			{SceneIf.GetIfInfoStatic(), typeof(SceneIf)},
			{SdkIf.GetIfInfoStatic(), typeof(SdkIf)},
			{UTTimerIf.GetIfInfoStatic(), typeof(UTTimerIf)},
		};
	}
}
