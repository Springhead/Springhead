// cs_object.cs
//
using System;

namespace SprCs {
    public class CsObject {
        protected IntPtr _this;
        protected bool _flag;
        protected CsObject() {}
	protected CsObject(IntPtr ptr, bool flag = false) { _this = ptr; _flag = flag; }
        ~CsObject() {}
	public IntPtr get() { return _this; }
    }
}

