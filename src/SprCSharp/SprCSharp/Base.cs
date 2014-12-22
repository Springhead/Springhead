using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Spr {
    partial class SprExport {
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_new_Vec3d_0();
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_new_Vec3d_1(double x, double y, double z);
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_delete_Vec3d(IntPtr v);
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_Vec3d_operator_plus(IntPtr a, IntPtr b);
        [DllImport("SprExport.dll")]
        [return: MarshalAs(UnmanagedType.BStr)]
        public static extern string Spr_Vec3d_ToString(IntPtr v);
    }

    public class Vec3d {
        protected IntPtr _ptr_;

        protected Vec3d(IntPtr p) {
            _ptr_ = p;
        }

        public Vec3d() {
            _ptr_ = SprExport.Spr_new_Vec3d_0();
        }

        public Vec3d(double x, double y, double z) {
            _ptr_ = SprExport.Spr_new_Vec3d_1(x, y, z);
        }

        ~Vec3d() {
            SprExport.Spr_delete_Vec3d(_ptr_);
        }

        public static Vec3d operator +(Vec3d a, Vec3d b) {
            return new Vec3d(SprExport.Spr_Vec3d_operator_plus(a._ptr_, b._ptr_));
        }

        public override string ToString() {
            return SprExport.Spr_Vec3d_ToString(_ptr_);
        }
    }
}
