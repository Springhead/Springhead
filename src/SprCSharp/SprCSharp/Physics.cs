using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;

namespace Spr {
    partial class SprExport {
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_PHSceneDesc_get_gravity(IntPtr _this);
        [DllImport("SprExport.dll")]
        public static extern void Spr_PHSceneDesc_set_gravity(IntPtr _this, IntPtr value);
        [DllImport("SprExport.dll")]
        public static extern double Spr_PHSceneDesc_get_airResistanceRate(IntPtr _this);
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_PHSceneDesc_set_airResistanceRate(IntPtr _this, double value);
        [DllImport("SprExport.dll")]
        public static extern IntPtr Spr_new_PHSceneDesc();
        [DllImport("SprExport.dll")]
        public static extern void Spr_PHSceneDesc_Init(IntPtr _this);
        [DllImport("SprExport.dll")]
        public static extern void Spr_delete_PHSceneDesc(IntPtr _this);
    }

    public class PHSceneDesc {
        protected IntPtr _this;

        protected PHSceneDesc(IntPtr p) {
            _this = p;
        }

        public PHSceneDesc() {
            _this = SprExport.Spr_new_PHSceneDesc();
        }

        ~PHSceneDesc() {
            SprExport.Spr_delete_PHSceneDesc(_this);
        }

        public Vec3d gravity {
            get { return new Vec3d(SprExport.Spr_PHSceneDesc_get_gravity(_this)); }
            set { SprExport.Spr_PHSceneDesc_set_gravity(_this, value._ptr_); }
        }

        public double airResistanceRate {
            get { return SprExport.Spr_PHSceneDesc_get_airResistanceRate(_this); }
            set { SprExport.Spr_PHSceneDesc_set_airResistanceRate(_this, value); }
        }
    }
}
