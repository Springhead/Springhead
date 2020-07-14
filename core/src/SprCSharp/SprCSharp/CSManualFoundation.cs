using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SprCs {
    public partial class Vec3d : CsObject {
        public Vec3d(Vec3d v) { _this = SprExport.Spr_new_Vec3d_4(v.x, v.y, v.z); _flag = true; }
    }
    public partial class Vec6d : CsObject {
        public Vec6d(Vec6d v) { _this = SprExport.Spr_new_Vec6d_7(v.vx, v.vy, v.vz, v.wx, v.wy, v.wz); _flag = true; }
    }
    public partial class Quaterniond : CsObject {
        public Quaterniond(Quaterniond q) { _this = SprExport.Spr_new_Quaterniond_5(q.w, q.x, q.y, q.z); _flag = true; }
    }
    public partial class Posed : CsObject {
        public Posed(Posed p) { _this = SprExport.Spr_new_Posed_9(p.w, p.x, p.y, p.z, p.px, p.py, p.pz); _flag = true; }
    }
    public partial class ObjectIf : CsObject {
        public IntPtr _this2;
    }
}
