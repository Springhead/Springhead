// wrapper.cs
//
using System;
using System.Runtime.InteropServices;

namespace SprCs {
    public partial class SprExport {
        // std::vector
        //  int
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_get_int(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_int(IntPtr _ptr, int index, int value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_int(IntPtr _ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_int(IntPtr _ptr, int _val);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_int(IntPtr _ptr);
        //  unsigned int
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern uint Spr_vector_get_unsigned_int(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_unsigned_int(IntPtr _ptr, int index, uint value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_unsigned_int(IntPtr _ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_unsigned_int(IntPtr _ptr, uint value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_unsigned_int(IntPtr _ptr);
        //  size_t
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern ulong Spr_vector_get_size_t(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_size_t(IntPtr _ptr, int index, ulong value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_size_t(IntPtr _ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_size_t(IntPtr _ptr, ulong value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_size_t(IntPtr _ptr);
        //  float
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern float Spr_vector_get_float(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_float(IntPtr _ptr, int index, float value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_float(IntPtr _ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_float(IntPtr _ptr, float value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_float(IntPtr _ptr);
        //  double
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern double Spr_vector_get_double(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_double(IntPtr _ptr, int index, double value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_double(IntPtr _ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_double(IntPtr _ptr, double value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_double(IntPtr _ptr);
        //  string
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern IntPtr Spr_vector_get_string(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_string(IntPtr _ptr, int index, IntPtr value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_string(IntPtr _ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_string(IntPtr _ptr, string value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_string(IntPtr _ptr);

        // array
        //  int
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_array_get_int(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_int(IntPtr _ptr, int index, int value);
        //  float
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern float Spr_array_get_float(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_float(IntPtr _ptr, int index, float value);
        //  double
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern double Spr_array_get_double(IntPtr _ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_double(IntPtr _ptr, int index, double value);
    }
}

