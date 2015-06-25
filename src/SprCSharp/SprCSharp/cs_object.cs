// cs_object.cs
//
using System;

namespace SprCs {
    public class CsObject {
        protected IntPtr _this;
        protected bool _flag;
        protected CsObject() {}
        ~CsObject() {}
	public IntPtr get() { return _this; }
    }
}

