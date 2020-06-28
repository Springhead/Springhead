using System;
using System.Runtime.InteropServices;

namespace SprCs {
    public partial class SprExport {
        [DllImport("SprExport.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Spr_UTTimerIf_SetCallback(IntPtr _this, IntPtr callback);
    }
}
