// wrapper.cs
//
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Data;

namespace SprCs {
    // wrapper base class
    //
    public class wrapper : CsObject {
        protected wrapper() {}
        public wrapper(IntPtr ptr) : base(ptr) {}
        public wrapper(IntPtr ptr, bool flag) : base(ptr, flag) {}
        ~wrapper() {}
    }
    public class vectorwrapper : wrapper {
	protected vectorwrapper() {}
        public vectorwrapper(IntPtr ptr) : base(ptr) {}
        public vectorwrapper(IntPtr ptr, bool flag) : base(ptr, flag) {}
    }
    public class arraywrapper : wrapper {
	protected arraywrapper(int nelm) { _attr(nelm); }
	public arraywrapper(IntPtr ptr, int nelm) : base(ptr) { _attr(nelm); }
	public arraywrapper(IntPtr ptr, int nelm, bool flag) : base(ptr, flag) { _attr(nelm); }
	private void _attr(int nelm) { _nelm = (uint) nelm; }
	protected uint _nelm;
    }

    // std::vector
    //  int
    public class vectorwrapper_int : vectorwrapper {
        public vectorwrapper_int() {
		_this = SprExport.Spr_vector_new_int();
		_flag = true;
	}
        public vectorwrapper_int(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_int(IntPtr ptr, bool flag) : base(ptr, flag) {}
        ~vectorwrapper_int() {
		if (_flag) { SprExport.Spr_vector_delete_int(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_int(_this); }
        public void push_back(int value) { SprExport.Spr_vector_push_back_int(_this, value); }
        public void clear() { SprExport.Spr_vector_clear_int(_this); }
        public int this[int index] {
            get { return (int) SprExport.Spr_vector_get_int(_this, index); }
            set { SprExport.Spr_vector_set_int(_this, index, value); }
        }
        public static implicit operator List<int>(vectorwrapper_int m) {
            List<int> r = new List<int>();
            for (int i = 0; i < SprExport.Spr_vector_size_int(m._this); i++)
                r.Add(SprExport.Spr_vector_get_int(m._this, i));
            return r;
        }
        public static implicit operator vectorwrapper_int(List<int> r) {
	    IntPtr vec = SprExport.Spr_vector_new_int();
            vectorwrapper_int m = new vectorwrapper_int(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_int(m._this, r[i]);
            return m;
        }
        public static implicit operator int[](vectorwrapper_int m) {
            int size = SprExport.Spr_vector_size_int(m._this);
            int[] r = new int[size];
            for (int i = 0; i < size; i++)
                r[i] = SprExport.Spr_vector_get_int(m._this, i);
            return r;
        }
        public static implicit operator vectorwrapper_int(int[] r) {
	    IntPtr vec = SprExport.Spr_vector_new_int();
            vectorwrapper_int m = new vectorwrapper_int(vec, true);
            for (int i = 0; i < r.Length; i++)
                SprExport.Spr_vector_push_back_int(m._this, r[i]);
            return m;
        }
    }

    //  unsigned int
    public class vectorwrapper_unsigned_int : vectorwrapper {
        public vectorwrapper_unsigned_int() {
		_this = SprExport.Spr_vector_new_unsigned_int();
		_flag = true;
	}
        public vectorwrapper_unsigned_int(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_unsigned_int(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_unsigned_int() {
		if (_flag) { SprExport.Spr_vector_delete_unsigned_int(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_unsigned_int(_this); }
        public void push_back(uint value) { SprExport.Spr_vector_push_back_unsigned_int(_this, value); }
        public void clear() { SprExport.Spr_vector_clear_unsigned_int(_this); }
        public uint this[int index] {
            get { return (uint) SprExport.Spr_vector_get_unsigned_int(_this, index); }
            set { SprExport.Spr_vector_set_unsigned_int(_this, index, value); }
        }
        public static implicit operator List<uint>(vectorwrapper_unsigned_int m) {
            List<uint> r = new List<uint>();
            for (int i = 0; i < SprExport.Spr_vector_size_unsigned_int(m._this); i++)
                r.Add(SprExport.Spr_vector_get_unsigned_int(m._this, i));
            return r;
        }
        public static implicit operator vectorwrapper_unsigned_int(List<uint> r) {
	    IntPtr vec = SprExport.Spr_vector_new_unsigned_int();
            vectorwrapper_unsigned_int m = new vectorwrapper_unsigned_int(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_unsigned_int(m._this, r[i]);
            return m;
        }
        public static implicit operator uint[](vectorwrapper_unsigned_int m) {
            int size = SprExport.Spr_vector_size_unsigned_int(m._this);
            uint[] r = new uint[size];
            for (int i = 0; i < size; i++)
                r[i] = (uint) SprExport.Spr_vector_get_unsigned_int(m._this, i);
            return r;
        }
        public static implicit operator vectorwrapper_unsigned_int(uint[] r) {
	    IntPtr vec = SprExport.Spr_vector_new_unsigned_int();
            vectorwrapper_unsigned_int m = new vectorwrapper_unsigned_int(vec, true);
            for (int i = 0; i < r.Length; i++)
                SprExport.Spr_vector_push_back_unsigned_int(m._this, r[i]);
            return m;
        }
    }

    //  size_t
    public class vectorwrapper_size_t : vectorwrapper {
        public vectorwrapper_size_t() {
		_this = SprExport.Spr_vector_new_size_t();
		_flag = true;
	}
        public vectorwrapper_size_t(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_size_t(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_size_t() {
		if (_flag) { SprExport.Spr_vector_delete_size_t(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_size_t(_this); }
        public void push_back(ulong value) { SprExport.Spr_vector_push_back_size_t(_this, value); }
        public void clear() { SprExport.Spr_vector_clear_size_t(_this); }
        public ulong this[int index] {
            get { return (ulong) SprExport.Spr_vector_get_size_t(_this, index); }
            set { SprExport.Spr_vector_set_size_t(_this, index, value); }
        }
        public static implicit operator List<ulong>(vectorwrapper_size_t m) {
            List<ulong> r = new List<ulong>();
            for (int i = 0; i < SprExport.Spr_vector_size_size_t(m._this); i++)
                r.Add(SprExport.Spr_vector_get_size_t(m._this, i));
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
            for (int i = 0; i < SprExport.Spr_vector_size_size_t(m._this); i++)
                r.Add((uint) SprExport.Spr_vector_get_size_t(m._this, i));
            return r;
        }
        public static implicit operator vectorwrapper_size_t(List<uint> r) {
	    IntPtr vec = SprExport.Spr_vector_new_size_t();
            vectorwrapper_size_t m = new vectorwrapper_size_t(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_size_t(m._this, r[i]);
            return m;
        }
        public static implicit operator ulong[](vectorwrapper_size_t m) {
            int size = SprExport.Spr_vector_size_size_t(m._this);
            ulong[] r = new ulong[size];
            for (int i = 0; i < size; i++)
                r[i] = (ulong) SprExport.Spr_vector_get_size_t(m._this, i);
            return r;
        }
        public static implicit operator vectorwrapper_size_t(ulong[] r) {
	    IntPtr vec = SprExport.Spr_vector_new_size_t();
            vectorwrapper_size_t m = new vectorwrapper_size_t(vec, true);
            for (int i = 0; i < r.Length; i++)
                SprExport.Spr_vector_push_back_size_t(m._this, r[i]);
            return m;
        }
    }

    //  float
    public class vectorwrapper_float : vectorwrapper {
        public vectorwrapper_float() {
		_this = SprExport.Spr_vector_new_float();
		_flag = true;
	}
        public vectorwrapper_float(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_float(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_float() {
		if (_flag) { SprExport.Spr_vector_delete_float(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_float(_this); }
        public void push_back(float value) { SprExport.Spr_vector_push_back_float(_this, value); }
        public void clear() { SprExport.Spr_vector_clear_float(_this); }
        public float this[int index] {
            get { return (float) SprExport.Spr_vector_get_float(_this, index); }
            set { SprExport.Spr_vector_set_float(_this, index, value); }
        }
        public static implicit operator List<float>(vectorwrapper_float m) {
            List<float> r = new List<float>();
            for (int i = 0; i < SprExport.Spr_vector_size_float(m._this); i++)
                r.Add(SprExport.Spr_vector_get_float(m._this, i));
            return r;
        }
        public static implicit operator vectorwrapper_float(List<float> r) {
	    IntPtr vec = SprExport.Spr_vector_new_float();
            vectorwrapper_float m = new vectorwrapper_float(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_float(m._this, r[i]);
            return m;
        }
        public static implicit operator float[](vectorwrapper_float m) {
            int size = SprExport.Spr_vector_size_float(m._this);
            float[] r = new float[size];
            for (int i = 0; i < size; i++)
                r[i] = (float) SprExport.Spr_vector_get_float(m._this, i);
            return r;
        }
        public static implicit operator vectorwrapper_float(float[] r) {
	    IntPtr vec = SprExport.Spr_vector_new_float();
            vectorwrapper_float m = new vectorwrapper_float(vec, true);
            for (int i = 0; i < r.Length; i++)
                SprExport.Spr_vector_push_back_float(m._this, r[i]);
            return m;
        }
    }

    //  double
    public class vectorwrapper_double : vectorwrapper {
        public vectorwrapper_double() {
		_this = SprExport.Spr_vector_new_double();
		_flag = true;
	}
        public vectorwrapper_double(IntPtr ptr) : base(ptr) { }
        public vectorwrapper_double(IntPtr ptr, bool flag) : base(ptr, flag) { }
        ~vectorwrapper_double() {
		if (_flag) { SprExport.Spr_vector_delete_double(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_double(_this); }
        public void push_back(double value) { SprExport.Spr_vector_push_back_double(_this, value); }
        public void clear() { SprExport.Spr_vector_clear_double(_this); }
        public double this[int index] {
            get { return (double) SprExport.Spr_vector_get_double(_this, index); }
            set { SprExport.Spr_vector_set_double(_this, index, value); }
        }
        public static implicit operator List<double>(vectorwrapper_double m) {
            List<double> r = new List<double>();
            for (int i = 0; i < SprExport.Spr_vector_size_double(m._this); i++)
                r.Add(SprExport.Spr_vector_get_double(m._this, i));
            return r;
        }
        public static implicit operator vectorwrapper_double(List<double> r) {
	    IntPtr vec = SprExport.Spr_vector_new_double();
            vectorwrapper_double m = new vectorwrapper_double(vec, true);
            for (int i = 0; i < r.Count; i++)
                SprExport.Spr_vector_push_back_double(m._this, r[i]);
            return m;
        }
        public static implicit operator double[](vectorwrapper_double m) {
            int size = SprExport.Spr_vector_size_double(m._this);
            double[] r = new double[size];
            for (int i = 0; i < size; i++)
                r[i] = (double) SprExport.Spr_vector_get_double(m._this, i);
            return r;
        }
        public static implicit operator vectorwrapper_double(double[] r) {
	    IntPtr vec = SprExport.Spr_vector_new_double();
            vectorwrapper_double m = new vectorwrapper_double(vec, true);
            for (int i = 0; i < r.Length; i++)
                SprExport.Spr_vector_push_back_double(m._this, r[i]);
            return m;
        }
    }

    //  string
    public class vectorwrapper_string : vectorwrapper {
        public vectorwrapper_string() {}
        public vectorwrapper_string(IntPtr ptr) : base(ptr) {}
        public vectorwrapper_string(IntPtr ptr, bool flag) : base(ptr, flag) { }
        public vectorwrapper_string(uint nelm) {
	    _this = SprExport.Spr_array_new_string(nelm);
	    _flag = true;
        }
        ~vectorwrapper_string() {
		if (_flag) { SprExport.Spr_vector_delete_string(_this); _flag = false;}
	}
        public int size() { return (int) SprExport.Spr_vector_size_string(_this); }
        public void push_back(IntPtr value) { SprExport.Spr_vector_push_back_string(_this, value); }
        public void clear() { SprExport.Spr_vector_clear_string(_this); }
        public string this[int index] {
            get {
                IntPtr ptr = SprExport.Spr_vector_get_string(_this, index);
                string bstr = Marshal.PtrToStringBSTR(ptr);
                Marshal.FreeBSTR(ptr);
                return bstr;
            }
            set {
                IntPtr pbstr = Marshal.StringToBSTR(value);
                SprExport.Spr_vector_set_string(_this, index, pbstr);
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
      
        public static implicit operator string[](vectorwrapper_string m) {
	    var ptr = (IntPtr) m;
            int size = SprExport.Spr_vector_size_string(m._this);
            string[] r = new string[size];
            for (int i = 0; i < size; i++) {
                IntPtr tmp = SprExport.Spr_vector_get_string(ptr, i);
                string bstr = Marshal.PtrToStringBSTR(tmp);
                Marshal.FreeBSTR(tmp);
                r[i] = bstr;
	    }
            return r;
        }
        public static implicit operator vectorwrapper_string(string[] r) {
	    IntPtr vec = SprExport.Spr_vector_new_string();
            vectorwrapper_string m = new vectorwrapper_string(vec, true);
            for (int i = 0; i < r.Length; i++) {
                IntPtr pbstr = Marshal.StringToBSTR(r[i]);
                SprExport.Spr_vector_push_back_string(m._this, pbstr);
	    }
            return m;
        }
    }

    // array
    //  int
    public class arraywrapper_int : arraywrapper {
        public arraywrapper_int(int nelm) : base(nelm) {
		_this = SprExport.Spr_array_new_int((uint) nelm);
		_flag = true;
	}
        public arraywrapper_int(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_int(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_int() {
		if (_flag) { SprExport.Spr_array_delete_int(_this); _flag = false;}
	}
        public int size() { return (int) _nelm; }
        public int this[int index] {
            get { return (int) SprExport.Spr_array_get_int(_this, index); }
            set { SprExport.Spr_array_set_int(_this, index, value); }
        }
        public static implicit operator List<int>(arraywrapper_int m) {
	    List<int> r = new List<int>();
            for (int i = 0; i < m._nelm; i++)
		r.Add(SprExport.Spr_array_get_int(m._this, i));
	    return r;
	}
        public static implicit operator arraywrapper_int(List<int> r) {
	    int size = r.Count;
	    IntPtr ptr = SprExport.Spr_array_new_int((uint) size);
            arraywrapper_int m = new arraywrapper_int(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_int(m._this, i, r[i]);
            return m;
        }
        public static implicit operator int[](arraywrapper_int m) {
            int size = (int) m._nelm;
	    int[] r = new int[size];
            for (int i = 0; i < size; i++)
		r[i] = SprExport.Spr_array_get_int(m._this, i);
	    return r;
	}
        public static implicit operator arraywrapper_int(int[] r) {
	    int size = r.Length;
	    IntPtr ptr = SprExport.Spr_array_new_int((uint) size);
            arraywrapper_int m = new arraywrapper_int(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_int(m._this, i, r[i]);
            return m;
        }
    }

    //  unsigned int
    public class arraywrapper_unsigned_int : arraywrapper {
        public arraywrapper_unsigned_int(int nelm) : base(nelm) {
		_this = SprExport.Spr_array_new_unsigned_int((uint) nelm);
		_flag = true;
	}
        public arraywrapper_unsigned_int(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_unsigned_int(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_unsigned_int() {
		if (_flag) { SprExport.Spr_array_delete_unsigned_int(_this); _flag = false;}
	}
        public int size() { return (int) _nelm; }
        public uint this[int index] {
            get { return (uint) SprExport.Spr_array_get_unsigned_int(_this, index); }
            set { SprExport.Spr_array_set_unsigned_int(_this, index, value); }
        }
        public static implicit operator List<uint>(arraywrapper_unsigned_int m) {
            List<uint> r = new List<uint>();
            for (int i = 0; i < m._nelm; i++)
		r.Add(SprExport.Spr_array_get_unsigned_int(m._this, i));
            return r;
        }
        public static implicit operator arraywrapper_unsigned_int(List<uint> r) {
	    int size = r.Count;
	    IntPtr ptr = SprExport.Spr_array_new_unsigned_int((uint) size);
            arraywrapper_unsigned_int m = new arraywrapper_unsigned_int(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_unsigned_int(m._this, i, r[i]);
            return m;
        }
        public static implicit operator uint[](arraywrapper_unsigned_int m) {
            int size = (int) m._nelm;
	    uint[] r = new uint[size];
            for (int i = 0; i < size; i++)
		r[i] = SprExport.Spr_array_get_unsigned_int(m._this, i);
	    return r;
	}
        public static implicit operator arraywrapper_unsigned_int(uint[] r) {
	    int size = r.Length;
	    IntPtr ptr = SprExport.Spr_array_new_unsigned_int((uint) size);
            arraywrapper_unsigned_int m = new arraywrapper_unsigned_int(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_unsigned_int(m._this, i, r[i]);
            return m;
        }
    }

    //  size_t
    public class arraywrapper_size_t : arraywrapper {
        public arraywrapper_size_t(int nelm) : base(nelm) {
		_this = SprExport.Spr_array_new_size_t((uint) nelm);
		_flag = true;
	}
        public arraywrapper_size_t(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_size_t(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_size_t() {
		if (_flag) { SprExport.Spr_array_delete_size_t(_this); _flag = false;}
	}
        public int size() { return (int) _nelm; }
        public ulong this[int index] {
            get { return (ulong) SprExport.Spr_array_get_size_t(_this, index); }
            set { SprExport.Spr_array_set_size_t(_this, index, value); }
        }
        public static implicit operator List<ulong>(arraywrapper_size_t m) {
            List<ulong> r = new List<ulong>();
            for (int i = 0; i < m._nelm; i++)
		r.Add(SprExport.Spr_array_get_size_t(m._this, i));
            return r;
        }
        public static implicit operator arraywrapper_size_t(List<ulong> r) {
	    int size = r.Count;
	    IntPtr ptr = SprExport.Spr_array_new_size_t((uint) size);
            arraywrapper_size_t m = new arraywrapper_size_t(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_size_t(m._this, i, r[i]);
            return m;
        }
        public static implicit operator ulong[](arraywrapper_size_t m) {
            int size = (int) m._nelm;
	    ulong[] r = new ulong[size];
            for (int i = 0; i < size; i++)
		r[i] = SprExport.Spr_array_get_size_t(m._this, i);
            return r;
        }
        public static implicit operator arraywrapper_size_t(ulong[] r) {
	    int size = r.Length;
	    IntPtr ptr = SprExport.Spr_array_new_size_t((uint) size);
            arraywrapper_size_t m = new arraywrapper_size_t(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_size_t(m._this, i, r[i]);
            return m;
        }
    }

    //  float
    public class arraywrapper_float : arraywrapper {
        public arraywrapper_float(int nelm) : base(nelm) {
		_this = SprExport.Spr_array_new_float((uint) nelm);
		_flag = true;
	}
        public arraywrapper_float(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_float(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_float() {
		if (_flag) { SprExport.Spr_array_delete_float(_this); _flag = false;}
	}
        public int size() { return (int) _nelm; }
        public float this[int index] {
            get { return (float) SprExport.Spr_array_get_float(this, index); }
            set { SprExport.Spr_array_set_float(this, index, value); }
        }
        public static implicit operator List<float>(arraywrapper_float m) {
            List<float> r = new List<float>();
            for (int i = 0; i < m._nelm; i++)
		r.Add(SprExport.Spr_array_get_float(m._this, i));
            return r;
        }
        public static implicit operator arraywrapper_float(List<float> r) {
	    int size = r.Count;
	    IntPtr ptr = SprExport.Spr_array_new_float((uint) size);
            arraywrapper_float m = new arraywrapper_float(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_float(m._this, i, r[i]);
            return m;
        }
        public static implicit operator float[](arraywrapper_float m) {
            int size = (int) m._nelm;
	    float[] r = new float[size];
            for (int i = 0; i < size; i++)
		r[i] = SprExport.Spr_array_get_float(m._this, i);
	    return r;
	}
        public static implicit operator arraywrapper_float(float[] r) {
	    int size = r.Length;
	    IntPtr ptr = SprExport.Spr_array_new_float((uint) size);
            arraywrapper_float m = new arraywrapper_float(ptr, size, true);
            for (int i = 0; i < r.Length; i++)
		SprExport.Spr_array_set_float(m._this, i, r[i]);
            return m;
        }
    }

    //  double
    public class arraywrapper_double : arraywrapper {
        public arraywrapper_double(int nelm) : base(nelm) {
		_this = SprExport.Spr_array_new_double((uint) nelm);
		_flag = true;
	}
        public arraywrapper_double(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_double(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_double() {
		if (_flag) { SprExport.Spr_array_delete_double(_this); _flag = false;}
	}
        public int size() { return (int) _nelm; }
        public double this[int index] {
            get { return (double) SprExport.Spr_array_get_double(this, index); }
            set { SprExport.Spr_array_set_double(this, index, value); }
        }
        public static implicit operator List<double>(arraywrapper_double m) {
            List<double> r = new List<double>();
            for (int i = 0; i < m._nelm; i++)
		r.Add(SprExport.Spr_array_get_double(m._this, i));
            return r;
        }
        public static implicit operator arraywrapper_double(List<double> r) {
	    int size = r.Count;
	    IntPtr ptr = SprExport.Spr_array_new_double((uint) size);
            arraywrapper_double m = new arraywrapper_double(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_double(m._this, i, r[i]);
            return m;
        }
        public static implicit operator double[](arraywrapper_double m) {
            int size = (int) m._nelm;
	    double[] r = new double[size];
            for (int i = 0; i < size; i++)
		r[i] = SprExport.Spr_array_get_double(m._this, i);
	    return r;
	}
        public static implicit operator arraywrapper_double(double[] r) {
	    int size = r.Length;
	    IntPtr ptr = SprExport.Spr_array_new_double((uint) size);
            arraywrapper_double m = new arraywrapper_double(ptr, size, true);
            for (int i = 0; i < size; i++)
		SprExport.Spr_array_set_double(m._this, i, r[i]);
            return m;
        }
    }

    //  string
    public class arraywrapper_string : arraywrapper {
        public arraywrapper_string(int nelm) : base(nelm) {
	    _this = SprExport.Spr_array_new_string((uint) nelm);
	    _flag = true;
        }
        public arraywrapper_string(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_string(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_string() {
	    if (_flag) { SprExport.Spr_array_delete_string(_this, _nelm); _flag = false; }
        }
        public int size() { return (int) _nelm; }
        public string this[int index] {
            get {
                IntPtr ptr = SprExport.Spr_array_get_string(this, index);
                string bstr = Marshal.PtrToStringBSTR(ptr);
                Marshal.FreeBSTR(ptr);
                return bstr;
            }
            set {
                IntPtr pbstr = Marshal.StringToBSTR(value);
                SprExport.Spr_array_set_string(this, index, pbstr);
                Marshal.FreeBSTR(pbstr);
            }
        }
    }

    //  char*
    public class arraywrapper_char_p : arraywrapper {
        public arraywrapper_char_p(int nelm) : base(nelm) {
	    _this = SprExport.Spr_array_new_char_p((uint) nelm);
	    _flag = true;
            SprExport.Spr_array_init_char_p(_this, (uint) nelm);
        }
        public arraywrapper_char_p(IntPtr ptr, int nelm) : base(ptr, nelm) {}
        public arraywrapper_char_p(IntPtr ptr, int nelm, bool flag) : base(ptr, nelm, flag) {}
        ~arraywrapper_char_p() {
	    if (_flag) { SprExport.Spr_array_delete_char_p(_this, _nelm); _flag = false; }
        }
        public int size() { return (int) _nelm; }
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

