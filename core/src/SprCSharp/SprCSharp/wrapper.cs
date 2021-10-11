// wrapper.cs
//
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace SprCs {
    // wrapper base class
    //
    public class wrapper : CsObject {
        public wrapper(IntPtr ptr) : base(ptr) { _nelm = 0; }
        public wrapper(IntPtr ptr, bool flag) : base(ptr, flag) { _nelm = 0; }
        protected wrapper() {}
        ~wrapper() {}
        protected uint _nelm;
    }
    public class vectorwrapper : wrapper {
        public vectorwrapper(IntPtr ptr) : base(ptr) {}
    }
    public class arraywrapper : wrapper {
        protected arraywrapper() {}
        public arraywrapper(IntPtr ptr) : base(ptr) {}
        public arraywrapper(uint size, uint nelm) {
            _this = SprExport.Spr_arraywrapper_new(size, nelm);
            _nelm = nelm;
        }
    }

    // std::vector
    //  int
    public class vectorwrapper_int : wrapper {
        public vectorwrapper_int(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_int(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_int() {
		if (_flag) { SprExport.Spr_vector_delete_int(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_int(this); }
        public void push_back(int value) { SprExport.Spr_vector_push_back_int(this, value); }
        public void clear() { SprExport.Spr_vector_clear_int(this); }
        public int this[int index] {
            get { return (int) SprExport.Spr_vector_get_int(this, index); }
            set { SprExport.Spr_vector_set_int(this, index, value); }
        }
        public static implicit operator List<int>(vectorwrapper_int m) {
            List<int> r = new List<int>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_int(ptr); i++)
                r.Add(SprExport.Spr_vector_get_int(ptr, i));
            return r;
        }
        public static implicit operator vectorwrapper_int(List<int> r) {
	    IntPtr vec = SprExport.Spr_vector_new_int();
            vectorwrapper_int m = new vectorwrapper_int(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_int(m._this, r[i]);
            return m;
        }
    }
    //  unsigned int
    public class vectorwrapper_unsigned_int : wrapper {
        public vectorwrapper_unsigned_int(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_unsigned_int(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_unsigned_int() {
		if (_flag) { SprExport.Spr_vector_delete_unsigned_int(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_unsigned_int(this); }
        public void push_back(uint value) { SprExport.Spr_vector_push_back_unsigned_int(this, value); }
        public void clear() { SprExport.Spr_vector_clear_unsigned_int(this); }
        public uint this[int index] {
            get { return (uint) SprExport.Spr_vector_get_unsigned_int(this, index); }
            set { SprExport.Spr_vector_set_unsigned_int(this, index, value); }
        }
        public static implicit operator List<uint>(vectorwrapper_unsigned_int m) {
            List<uint> r = new List<uint>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_unsigned_int(ptr); i++)
                r.Add(SprExport.Spr_vector_get_unsigned_int(ptr, i));
            return r;
        }
        public static implicit operator vectorwrapper_unsigned_int(List<uint> r) {
	    IntPtr vec = SprExport.Spr_vector_new_unsigned_int();
            vectorwrapper_unsigned_int m = new vectorwrapper_unsigned_int(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_unsigned_int(m._this, r[i]);
            return m;
        }
    }
    //  size_t
    public class vectorwrapper_size_t : wrapper {
        public vectorwrapper_size_t(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_size_t(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_size_t() {
		if (_flag) { SprExport.Spr_vector_delete_size_t(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_size_t(this); }
        public void push_back(ulong value) { SprExport.Spr_vector_push_back_size_t(this, value); }
        public void clear() { SprExport.Spr_vector_clear_size_t(this); }
        public ulong this[int index] {
            get { return (ulong) SprExport.Spr_vector_get_size_t(this, index); }
            set { SprExport.Spr_vector_set_size_t(this, index, value); }
        }
        public static implicit operator List<ulong>(vectorwrapper_size_t m) {
            List<ulong> r = new List<ulong>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_size_t(ptr); i++)
                r.Add(SprExport.Spr_vector_get_size_t(ptr, i));
            return r;
        }
        public static implicit operator vectorwrapper_size_t(List<ulong> r) {
	    IntPtr vec = SprExport.Spr_vector_new_size_t();
            vectorwrapper_size_t m = new vectorwrapper_size_t(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_size_t(m._this, r[i]);
            return m;
        }
        public static implicit operator List<uint>(vectorwrapper_size_t m) {
            List<uint> r = new List<uint>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_size_t(ptr); i++)
                r.Add((uint) SprExport.Spr_vector_get_size_t(ptr, i));
            return r;
        }
        public static implicit operator vectorwrapper_size_t(List<uint> r) {
	    IntPtr vec = SprExport.Spr_vector_new_size_t();
            vectorwrapper_size_t m = new vectorwrapper_size_t(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_size_t(m._this, r[i]);
            return m;
        }
    }
    //  float
    public class vectorwrapper_float : wrapper {
        public vectorwrapper_float(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_float(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_float() {
		if (_flag) { SprExport.Spr_vector_delete_float(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_float(this); }
        public void push_back(float value) { SprExport.Spr_vector_push_back_float(this, value); }
        public void clear() { SprExport.Spr_vector_clear_float(this); }
        public float this[int index] {
            get { return (float) SprExport.Spr_vector_get_float(this, index); }
            set { SprExport.Spr_vector_set_float(this, index, value); }
        }
        public static implicit operator List<float>(vectorwrapper_float m) {
            List<float> r = new List<float>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_float(ptr); i++)
                r.Add(SprExport.Spr_vector_get_float(ptr, i));
            return r;
        }
        public static implicit operator vectorwrapper_float(List<float> r) {
	    IntPtr vec = SprExport.Spr_vector_new_float();
            vectorwrapper_float m = new vectorwrapper_float(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_float(m._this, r[i]);
            return m;
        }
    }
    //  double
    public class vectorwrapper_double : wrapper {
        public vectorwrapper_double(IntPtr ptr) : base(ptr) { }
        public vectorwrapper_double(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_double() {
		if (_flag) { SprExport.Spr_vector_delete_double(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_double(this); }
        public void push_back(double value) { SprExport.Spr_vector_push_back_double(this, value); }
        public void clear() { SprExport.Spr_vector_clear_double(this); }
        public double this[int index] {
            get { return (double) SprExport.Spr_vector_get_double(this, index); }
            set { SprExport.Spr_vector_set_double(this, index, value); }
        }
        public static implicit operator List<double>(vectorwrapper_double m) {
            List<double> r = new List<double>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_double(ptr); i++)
                r.Add(SprExport.Spr_vector_get_double(ptr, i));
            return r;
        }
        public static implicit operator vectorwrapper_double(List<double> r) {
	    IntPtr vec = SprExport.Spr_vector_new_double();
            vectorwrapper_double m = new vectorwrapper_double(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_double(m._this, r[i]);
            return m;
        }
    }
    //  string
    public class vectorwrapper_string : wrapper {
        public vectorwrapper_string(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_string(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_string() {
		if (_flag) { SprExport.Spr_vector_delete_string(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_string(this); }
        public void push_back(IntPtr value) { SprExport.Spr_vector_push_back_string(this, value); }
        public void clear() { SprExport.Spr_vector_clear_string(this); }
        public string this[int index] {
            get {
                IntPtr ptr = SprExport.Spr_vector_get_string(this, index);
                string bstr = Marshal.PtrToStringBSTR(ptr);
                Marshal.FreeBSTR(ptr);
                return bstr;
            }
            set {
                IntPtr pbstr = Marshal.StringToBSTR(value);
                SprExport.Spr_vector_set_string(this, index, pbstr);
                Marshal.FreeBSTR(pbstr);
            }
        }
        public static implicit operator List<string>(vectorwrapper_string m) {
            List<string> r = new List<string>();
	    var ptr = (IntPtr) m;
            for (int i = 0; i < SprExport.Spr_vector_size_string(ptr); i++) {
                IntPtr tmp = SprExport.Spr_vector_get_string(ptr, i);
                string bstr = Marshal.PtrToStringBSTR(tmp);
                Marshal.FreeBSTR(tmp);
                r.Add(bstr);
	    }
            return r;
        }
        public static implicit operator vectorwrapper_string(List<string> r) {
	    IntPtr vec = SprExport.Spr_vector_new_string();
            vectorwrapper_string m = new vectorwrapper_string(vec, true);
            for (int i = 0; i < r.Count; i++) {
                IntPtr pbstr = Marshal.StringToBSTR(r[i]);
                SprExport.Spr_vector_push_back_string(m._this, pbstr);
	    }
            return m;
        }
    }

    // array
    //  int
    public class arraywrapper_int : arraywrapper {
        public arraywrapper_int(IntPtr ptr) : base(ptr) {}
        public arraywrapper_int(uint nelm) : base(sizeof(int), nelm) {}
        public int this[int index] {
            get { return (int) SprExport.Spr_array_get_int(this, index); }
            set { SprExport.Spr_array_set_int(this, index, value); }
        }
        public static implicit operator List<int>(arraywrapper_int m) {
            List<int> r = new List<int>();
            // ここで変換する（未実装）
            return r;
        }
        public static implicit operator arraywrapper_int(List<int> r) {
            arraywrapper_int m = new arraywrapper_int(IntPtr.Zero);
            // ここで変換する（未実装）
            return m;
        }
    }
    //  float
    public class arraywrapper_float : arraywrapper {
        public arraywrapper_float(IntPtr ptr) : base(ptr) {}
        public arraywrapper_float(uint nelm) : base(sizeof(float), nelm) {}
        public float this[int index] {
            get { return (float) SprExport.Spr_array_get_float(this, index); }
            set { SprExport.Spr_array_set_float(this, index, value); }
        }
        public static implicit operator List<float>(arraywrapper_float m) {
            List<float> r = new List<float>();
            // ここで変換する（未実装）
            return r;
        }
        public static implicit operator arraywrapper_float(List<float> r) {
            arraywrapper_float m = new arraywrapper_float(IntPtr.Zero);
            // ここで変換する（未実装）
            return m;
        }
    }
    //  double
    public class arraywrapper_double : arraywrapper {
        public arraywrapper_double(IntPtr ptr) : base(ptr) {}
        public arraywrapper_double(uint nelm) : base(sizeof(double), nelm) {}
        public double this[int index] {
            get { return (double) SprExport.Spr_array_get_double(this, index); }
            set { SprExport.Spr_array_set_double(this, index, value); }
        }
        public static implicit operator List<double>(arraywrapper_double m) {
            List<double> r = new List<double>();
            // ここで変換する（未実装）
            return r;
        }
        public static implicit operator arraywrapper_double(List<double> r) {
            arraywrapper_double m = new arraywrapper_double(IntPtr.Zero);
            // ここで変換する（未実装）
            return m;
        }
    }
    //  char*
    public class arraywrapper_char_p : arraywrapper {
        public arraywrapper_char_p(IntPtr ptr) : base(ptr) {}
        public arraywrapper_char_p(uint nelm) : base((uint) IntPtr.Size, nelm+1) {
            SprExport.Spr_array_init_char_p(_this, nelm+1);
        }
        ~arraywrapper_char_p() {
            SprExport.Spr_array_delete_char_p(_this, _nelm);
        }
        public string this[int index] {
            get {
                IntPtr ptr = SprExport.Spr_array_get_char_p(this, index);
                string bstr = Marshal.PtrToStringBSTR(ptr);
                Marshal.FreeBSTR(ptr);
                return bstr;
            }
            set {
                IntPtr pbstr = Marshal.StringToBSTR(value);
                SprExport.Spr_array_set_char_p(this, index, pbstr);
                Marshal.FreeBSTR(pbstr);
            }
        }
    }
}

