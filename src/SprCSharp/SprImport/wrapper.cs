// wrapper.cs
//
using System;
using System.Runtime.InteropServices;

namespace SprCs {
    public partial class SprExport {
        // std::vector
        //  int
        [DllImport("SprExport.dll")]
        public static extern int Spr_vector_get_int(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_vector_set_int(IntPtr _ptr, int index, int value);
        //  unsigned int
        [DllImport("SprExport.dll")]
        public static extern uint Spr_vector_get_unsigned_int(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_vector_set_unsigned_int(IntPtr _ptr, int index, uint value);
        //  size_t
        [DllImport("SprExport.dll")]
        public static extern ulong Spr_vector_get_size_t(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_vector_set_size_t(IntPtr _ptr, int index, ulong value);
        //  float
        [DllImport("SprExport.dll")]
        public static extern float Spr_vector_get_float(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_vector_set_float(IntPtr _ptr, int index, float value);
        //  string
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_vector_get_string(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_vector_set_string(IntPtr _ptr, int index, IntPtr value);

        // array
        //  int
        [DllImport("SprExport.dll")]
        public static extern int Spr_array_get_int(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_array_set_int(IntPtr _ptr, int index, int value);
        //  float
        [DllImport("SprExport.dll")]
        public static extern float Spr_array_get_float(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_array_set_float(IntPtr _ptr, int index, float value);
        //  double
        [DllImport("SprExport.dll")]
        public static extern double Spr_array_get_double(IntPtr _ptr, int index);
        [DllImport("SprExport.dll")]
        public static extern void Spr_array_set_double(IntPtr _ptr, int index, double value);
    }
}

