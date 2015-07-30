// wrapper.cs
//
using System;
using System.Runtime.InteropServices;

namespace SprCs {
    // wrapper base class
    //
    public class wrapper : CsObject {
        public wrapper(IntPtr ptr) { _this = ptr; _nelm = 0; }
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
        public int size() { return (int) SprExport.Spr_vector_size_int(get()); }
        public void push_back(int value) { SprExport.Spr_vector_push_back_int(get(), value); }
        public void clear() { SprExport.Spr_vector_clear_int(get()); }
        public int this[int index] {
            get { return (int) SprExport.Spr_vector_get_int(get(), index); }
            set { SprExport.Spr_vector_set_int(get(), index, value); }
        }
    }
    //  unsigned int
    public class vectorwrapper_unsigned_int : wrapper {
        public vectorwrapper_unsigned_int(IntPtr ptr) : base(ptr) {}
        public int size() { return (int) SprExport.Spr_vector_size_unsigned_int(get()); }
        public void push_back(uint value) { SprExport.Spr_vector_push_back_unsigned_int(get(), value); }
        public void clear() { SprExport.Spr_vector_clear_unsigned_int(get()); }
        public uint this[int index] {
            get { return (uint) SprExport.Spr_vector_get_unsigned_int(get(), index); }
            set { SprExport.Spr_vector_set_unsigned_int(get(), index, value); }
        }
    }
    //  size_t
    public class vectorwrapper_size_t : wrapper {
        public vectorwrapper_size_t(IntPtr ptr) : base(ptr) {}
        public int size() { return (int) SprExport.Spr_vector_size_size_t(get()); }
        public void push_back(ulong value) { SprExport.Spr_vector_push_back_size_t(get(), value); }
        public void clear() { SprExport.Spr_vector_clear_size_t(get()); }
        public ulong this[int index] {
            get { return (ulong) SprExport.Spr_vector_get_size_t(get(), index); }
            set { SprExport.Spr_vector_set_size_t(get(), index, value); }
        }
    }
    //  float
    public class vectorwrapper_float : wrapper {
        public vectorwrapper_float(IntPtr ptr) : base(ptr) {}
        public int size() { return (int) SprExport.Spr_vector_size_float(get()); }
        public void push_back(float value) { SprExport.Spr_vector_push_back_float(get(), value); }
        public void clear() { SprExport.Spr_vector_clear_float(get()); }
        public float this[int index] {
            get { return (float) SprExport.Spr_vector_get_float(get(), index); }
            set { SprExport.Spr_vector_set_float(get(), index, value); }
        }
    }
    //  double
    public class vectorwrapper_double : wrapper {
        public vectorwrapper_double(IntPtr ptr) : base(ptr) {}
        public int size() { return (int) SprExport.Spr_vector_size_double(get()); }
        public void push_back(double value) { SprExport.Spr_vector_push_back_double(get(), value); }
        public void clear() { SprExport.Spr_vector_clear_double(get()); }
        public double this[int index] {
            get { return (double) SprExport.Spr_vector_get_double(get(), index); }
            set { SprExport.Spr_vector_set_double(get(), index, value); }
        }
    }
    //  string
    public class vectorwrapper_string : wrapper {
        public vectorwrapper_string(IntPtr ptr) : base(ptr) {}
        public int size() { return (int) SprExport.Spr_vector_size_string(get()); }
        public void push_back(string value) { SprExport.Spr_vector_push_back_string(get(), value); }
        public void clear() { SprExport.Spr_vector_clear_string(get()); }
        public string this[int index] {
            get {
                IntPtr ptr = SprExport.Spr_vector_get_string(get(), index);
                string bstr = Marshal.PtrToStringBSTR(ptr);
                Marshal.FreeCoTaskMem(ptr);
                return bstr;
            }
            set {
                IntPtr pbstr = Marshal.StringToBSTR(value);
                SprExport.Spr_vector_set_string(get(), index, pbstr);
                Marshal.FreeBSTR(pbstr);
            }
        }
    }

    // array
    //  int
    public class arraywrapper_int : arraywrapper {
        public arraywrapper_int(IntPtr ptr) : base(ptr) {}
        public arraywrapper_int(uint nelm) : base(sizeof(int), nelm) {}
        public int this[int index] {
            get { return (int) SprExport.Spr_array_get_int(get(), index); }
            set { SprExport.Spr_array_set_int(get(), index, value); }
        }
    }
    //  float
    public class arraywrapper_float : arraywrapper {
        public arraywrapper_float(IntPtr ptr) : base(ptr) {}
        public arraywrapper_float(uint nelm) : base(sizeof(float), nelm) {}
        public float this[int index] {
            get { return (float) SprExport.Spr_array_get_float(get(), index); }
            set { SprExport.Spr_array_set_float(get(), index, value); }
        }
    }
    //  double
    public class arraywrapper_double : arraywrapper {
        public arraywrapper_double(IntPtr ptr) : base(ptr) {}
        public arraywrapper_double(uint nelm) : base(sizeof(double), nelm) {}
        public double this[int index] {
            get { return (double) SprExport.Spr_array_get_double(get(), index); }
            set { SprExport.Spr_array_set_double(get(), index, value); }
        }
    }
}

