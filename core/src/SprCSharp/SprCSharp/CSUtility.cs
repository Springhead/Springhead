using System;
using System.Runtime.InteropServices;
using System.IO;
using System.Diagnostics;
using SprCs;

public delegate void RaiseExceptionDelegate(String msg);
public delegate void ExceptionCallbackDelegate();

public class SEH_Exception : SEHException {
	public SEH_Exception() {}
	~SEH_Exception() {}
	private static ExceptionRaiser er = null;
	private static bool st = false;
	public static void init(ExceptionCallbackDelegate func) {
		ExceptionRaiser.SetCallback(func);
		init();
	}
	public static void init() {
		if (!st) { SprExport.Spr_set_se_translator(); st = true; }
		er = er ?? new ExceptionRaiser();
	}
	public static String what() {
		IntPtr ptr = SprExport.Spr_SEH_Exception_what();
		return Marshal.PtrToStringBSTR(ptr);
	}
	public static String what(System.Exception e) {
		IntPtr ptr = SprExport.Spr_SEH_Exception_what();
		String s1 = Marshal.PtrToStringBSTR(ptr);
		String s2 = e.ToString();
		return s1.Substring(0, s1.Length-1) + "\n" + s2;
	}
	public static String where(String str = "System.Exception caught!") {
		StackFrame CallStack = new StackFrame(1, true);
		String[] comp = CallStack.GetFileName().Split('\\');
		String file = comp[comp.Length-1];
		String line = CallStack.GetFileLineNumber().ToString();
		String func = CallStack.GetMethod().Name;
		return file + ":" + line + ": " + func + "(): " + str;
	}
}

public class ExceptionRaiser {
	private static ExceptionCallbackDelegate callback = null;
	public ExceptionRaiser() {
		RaiseExceptionDelegate red = RaiseException;
		//GCHandle _gcHandle = GCHandle.Alloc(red);
		SprExport.Spr_register_exception_raiser(red);
	}
	public static void SetCallback(ExceptionCallbackDelegate func) {
		callback = func;
	}
	static void RaiseException(String msg) {
		String str = msg + SEH_Exception.what();
		if (callback != null) callback();
		throw new System.Exception(str);
	}
}

public static class CSlog {
	public static void Print(String str) {
		StackFrame CallStack = new StackFrame(0, true);
		String[] curr = CallStack.GetFileName().Split('\\');
		int bias = 0;
		for (int i = 0; i < curr.Length-1; i++) {
			if (curr[i] == "Unity") { bias = i + 1; break; }
		}
		String[] root = new String[bias];
		for (int i = 0; i < bias; i++) { root[i] = curr[i]; }
		String path = String.Join("/", root);
		//
		FileStream ofs = new FileStream(path + "/SprCS.log", FileMode.Append, FileAccess.Write);
		StreamWriter sw = new StreamWriter(ofs);
  		sw.WriteLine(str);
  		sw.Close();
  		ofs.Close();
	}
} 
