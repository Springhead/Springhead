//_[module: 1900] 
using System;
using System.Runtime.InteropServices;

public partial class SprExport {
	[DllImport("SprExport.dll", CallingConvention=CallingConvention.Cdecl)]
	public static extern IntPtr Spr_SEH_Exception_what();
}
