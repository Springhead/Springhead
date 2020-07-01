using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SprCs {
    public partial class Vec3d : CsObject {
        public Vec3d(Vec3d v) { _this = SprExport.Spr_new_Vec3d_4(v.x, v.y, v.z); _flag = true; }
    }
}
