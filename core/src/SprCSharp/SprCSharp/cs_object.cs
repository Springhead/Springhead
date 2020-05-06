// cs_object.cs
//
using System;
using System.Collections.Generic;

namespace SprCs {
    public class CsObject {
        public IntPtr _this;
        public bool _flag;
        protected CsObject() { }
        protected CsObject(IntPtr ptr, bool flag = false) { _this = ptr; _flag = flag; }
        ~CsObject() { }
        public static implicit operator IntPtr(CsObject obj) {
            return (obj == null) ? IntPtr.Zero : obj._this;
        }
        public static bool operator ==(CsObject a, CsObject b) {
            if ((object)a == null || (object)b == null) {
                if ((object)a == null && (object)b == null) { return true; } else { return false; }
            } else {
                return a._this == b._this;
            }
        }
        public static bool operator !=(CsObject a, CsObject b) {
            return !(a == b);
        }
        public override bool Equals(object obj) {
            return this == (CsObject)obj;
        }
        public override int GetHashCode() {
            return this.GetHashCode();
        }
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

    public partial class IfInfoToCsType {
        public static Type FindType(IfInfo ifinfo) {
            Type t = null;
#if	TARGET_Physics
            if (mapPhysics.TryGetValue(ifinfo, out t)) { return t; }
            if (mapCollision.TryGetValue(ifinfo, out t)) { return t; }
            if (mapFoundation.TryGetValue(ifinfo, out t)) { return t; }
            if (mapBase.TryGetValue(ifinfo, out t)) { return t; }
#else	// ALL
            if (mapPhysics.TryGetValue(ifinfo, out t)) { return t; }
            if (mapCollision.TryGetValue(ifinfo, out t)) { return t; }
            if (mapFramework.TryGetValue(ifinfo, out t)) { return t; }
            if (mapGraphics.TryGetValue(ifinfo, out t)) { return t; }
            if (mapHumanInterface.TryGetValue(ifinfo, out t)) { return t; }
            if (mapCreature.TryGetValue(ifinfo, out t)) { return t; }
            if (mapFileIO.TryGetValue(ifinfo, out t)) { return t; }
            if (mapFoundation.TryGetValue(ifinfo, out t)) { return t; }
            if (mapBase.TryGetValue(ifinfo, out t)) { return t; }
#endif
            return null;
        }
    }
}
