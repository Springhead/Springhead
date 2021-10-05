// wrapper.cs
//
using System;
using System.Runtime.InteropServices;

namespace SprCs {
    public partial class SprExport {
        // wrapper base class
        //  vectorwrapper
        //  arraywrapper
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern IntPtr Spr_arraywrapper_new(uint size, uint nelm);

        // std::vector
        //  int
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_get_int(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_int(IntPtr ptr, int index, int value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_int(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_int(IntPtr ptr, int _val);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_int(IntPtr ptr);
        //  unsigned int
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern uint Spr_vector_get_unsigned_int(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_unsigned_int(IntPtr ptr, int index, uint value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_unsigned_int(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_unsigned_int(IntPtr ptr, uint value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_unsigned_int(IntPtr ptr);
        //  size_t
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern ulong Spr_vector_get_size_t(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_size_t(IntPtr ptr, int index, ulong value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_size_t(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_size_t(IntPtr ptr, ulong value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_size_t(IntPtr ptr);
        //  float
        [DllImport("SprExport.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Spr_vector_new_float();
        [DllImport("SprExport.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Spr_vector_delete_float(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern float Spr_vector_get_float(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_float(IntPtr ptr, int index, float value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_float(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_float(IntPtr ptr, float value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_float(IntPtr ptr);
        //  double
        [DllImport("SprExport.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Spr_vector_new_double();
        [DllImport("SprExport.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Spr_vector_delete_double(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern double Spr_vector_get_double(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_double(IntPtr ptr, int index, double value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_double(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_double(IntPtr ptr, double value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_double(IntPtr ptr);
        //  string
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern IntPtr Spr_vector_get_string(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_set_string(IntPtr ptr, int index, IntPtr value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_vector_size_string(IntPtr ptr);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_push_back_string(IntPtr ptr, string value);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_vector_clear_string(IntPtr ptr);

        // array
        //  int
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern int Spr_array_get_int(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_int(IntPtr ptr, int index, int value);
        //  float
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern float Spr_array_get_float(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_float(IntPtr ptr, int index, float value);
        //  double
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern double Spr_array_get_double(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_double(IntPtr ptr, int index, double value);
        //  char*
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_delete_char_p(IntPtr ptr, uint nelm);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_init_char_p(IntPtr ptr, uint nelm);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern IntPtr Spr_array_get_char_p(IntPtr ptr, int index);
        [DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
        public static extern void Spr_array_set_char_p(IntPtr ptr, int index, IntPtr value);
    }
}

