// cs_object.cs
//
using System;
using System.Collections.Generic;

namespace SprCs {
    public class CsObject {
        public const int _thisNumber = 3;
        public IntPtr[] _thisArray = new IntPtr[_thisNumber] { IntPtr.Zero,IntPtr.Zero,IntPtr.Zero };
        public bool _flag;
        protected CsObject() { }
        protected CsObject(IntPtr ptr, int sceneIndex = 0, bool flag = false) {
            _thisArray[sceneIndex] = ptr;
            _flag = flag;
        }
        protected CsObject(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, bool flag = false) {
            _thisArray[0] = ptr0;
            _thisArray[1] = ptr1;
            _thisArray[2] = ptr2;
            _flag = flag;
        }
        protected CsObject(IntPtr ptr0, IntPtr ptr1, IntPtr ptr2, int ptr0Index, int ptr1Index, int ptr2Index, bool flag = false) {
            _thisArray[ptr0Index] = ptr0;
            _thisArray[ptr1Index] = ptr1;
            _thisArray[ptr2Index] = ptr2;
            _flag = flag;
        }
        ~CsObject() { }
        public static implicit operator IntPtr(CsObject obj) {
            return (obj == null) ? IntPtr.Zero : obj._thisArray[0];
        }
        public static bool operator ==(CsObject a, CsObject b) {
            if ((object)a == null || (object)b == null) {
                if ((object)a == null && (object)b == null) { return true; } else { return false; }
            } else {
                bool result = false;
                for(int i = 0; i < _thisNumber; i++) {
                    if(a._thisArray[i] != IntPtr.Zero && b._thisArray[i] != IntPtr.Zero && a._thisArray[i] == b._thisArray[i]) {
                        result = true;
                    }
                }
                if(a._thisArray[0] != IntPtr.Zero && b._thisArray[0] != IntPtr.Zero && a._thisArray[0] == b._thisArray[0] ) {
                    result = true;
                }
                return result;
            }
        }
        public static bool operator !=(CsObject a, CsObject b) {
            return !(a == b);
        }
        public override bool Equals(object obj) {
            return this == (CsObject)obj;
        }
        public override int GetHashCode() {
            return _thisArray[0].GetHashCode();
        }
    }

    public partial class ObjectIf {
        public CsCastObject Cast() { return new CsCastObject(this); }
    }

    public class CsCastObject {
        public const int _thisNumber = 3;
        public IntPtr[] _thisArray = new IntPtr[_thisNumber] { IntPtr.Zero,IntPtr.Zero,IntPtr.Zero };
        public bool _flag;
        public IfInfo _info;
        public CsCastObject(ObjectIf obj) {
            _thisArray[0] = obj._thisArray[0];
            _thisArray[1] = obj._thisArray[1];
            _thisArray[2] = obj._thisArray[2];
            _flag = obj._flag;
            _info = obj.GetIfInfo();
        }
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
