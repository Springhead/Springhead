using System;
using System.Runtime.InteropServices;

public class SEH_Exception : SEHException {
	public SEH_Exception() {}
	~SEH_Exception() {}
	public static String what() {
		IntPtr ptr = SprExport.Spr_SEH_Exception_what();
		return Marshal.PtrToStringBSTR(ptr);
	}
}
