// cs_object.cs
//
using System;

namespace SprCs {
    public class CsObject {
        public IntPtr _this;
        public bool _flag;
        protected CsObject() {}
	protected CsObject(IntPtr ptr, bool flag = false) { _this = ptr; _flag = flag; }
        ~CsObject() {}
	public IntPtr get() { return _this; }
    }

    public partial class ObjectIf {
        public CsCastObject Cast() { return new CsCastObject(this); }
    }

    public class CsCastObject {
        public IntPtr _this;
        public bool _flag;
        public IfInfo _info;
        public CsCastObject(ObjectIf obj) { _this = obj._this; _flag = obj._flag; _info = obj.GetIfInfo(); }
    }

    /*
    public partial class CDConvexIf {
        public static implicit operator CDConvexIf(CsCastObject target) {
            return (target._info.Inherit(CDConvexIf.GetIfInfoStatic()) ? new CDConvexIf(target._this, target._flag) : null);
        }
    }

    public partial class CDSphereIf {
        public static implicit operator CDSphereIf(CsCastObject target) {
            if (target._info.Inherit(CDSphereIf.GetIfInfoStatic())) {
                return new CDSphereIf(target._this, target._flag);
            } else {
                return null;
            }
        }
    }

    public partial class CDBoxIf {
        public static implicit operator CDBoxIf(CsCastObject target) {
            if (target._info.Inherit(CDBoxIf.GetIfInfoStatic())) {
                return new CDBoxIf(target._this, target._flag);
            } else {
                return null;
            }
        }
    }
     * */

}

