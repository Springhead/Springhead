using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Runtime.InteropServices;
using SprCs;

namespace SprCsSample {
    class Program {
        static string inc = "a";
        //static string brk = "n";	// set "y" if run under debugger
        static int OK = 0;
        static int NG = 0;

        static void Main(string[] args) {

	    // DLLのあるディレクトリを求めるための準備
	    //	  cmakeを使ったか否かでcurrent directotyが異なるためのKludge
	    var CurrDirectory = System.IO.Directory.GetCurrentDirectory();
	    var CurrDirectories = CurrDirectory.Split('\\');
	    var ToIndex = 0;
	    for (int i = CurrDirectories.Length-1; i >= 0; i--) {
		if (CurrDirectories[i].Equals("core")) {
		    ToIndex = i;
		    break;
		}
	    }
	    var TopDirectory = "";
	    for (int i = 0; i < ToIndex; i++) {
		TopDirectory += CurrDirectories[i] + @"\";
	    }
	    // ここまで
	    var dllPath_64 = TopDirectory + @"generated\bin\win64";
	    var dllPath_32 = TopDirectory + @"generated\bin\win32";
            var dllDirectory = dllPath_64 + ";" + dllPath_32;
            var dllPath = Environment.GetEnvironmentVariable("PATH") + ";" + dllDirectory;
            Environment.SetEnvironmentVariable("PATH", dllPath);

            ExceptionRaiser er = new ExceptionRaiser();
            try {
                if (check_test("a"))  test_argument_and_return_value();
            }
            catch (System.Exception e) {
                System.Console.WriteLine(SEH_Exception.what(e));
            }
            System.Console.WriteLine("----");
            System.Console.WriteLine("Passed: " + OK + ", Failed: " + NG);
        }

        static void test_argument_and_return_value() {
            test_name("argument and return value");

            string sv3d123 = "(111.1, 222.2, 333.3)";
            string sv3d456 = "(444.4, 555.5, 666.6)";
            string sv3d789 = "(777.7, 888.8, 999.9)";

        // Test for instrinsic data type.
            CSTestFunction _function = new CSTestFunction();

            System.Console.WriteLine("---- [ intrinsic type ] ----");
        // int
            int def_i = _function.intrinsic_int();
            put("int: arg def:", "1", def_i);
            int rtn_i = _function.intrinsic_int(111);
            put("int: arg set:", "111", rtn_i);
        // unsigned int
            uint def_u = _function.intrinsic_unsigned_int();
            put("unsisgned int: arg def:", "2", def_u);
            uint rtn_u = _function.intrinsic_unsigned_int(222);
            put("unsigned int: arg set:", "222", rtn_u);
        // size_t
            ulong def_s = _function.intrinsic_size_t();
            put("size_t: arg def:", "3", def_s);
            ulong rtn_s = _function.intrinsic_size_t(333);
            put("size_t: arg set:", "333", rtn_s);
        // float
            float def_f = _function.intrinsic_float();
            put("float: arg def:", "4.4", def_f);
            float rtn_f = _function.intrinsic_float(444.4F);
            put("float: arg set:", "444.4", rtn_f);
        // double
            double def_d = _function.intrinsic_double();
            put("double: arg def:", "5.5", def_d);
            double rtn_d = _function.intrinsic_double(555.5);
            put("double: arg set:", "555.5", rtn_d);
        // string
            string def_str = _function.intrinsic_string();
            put("string: arg def:", "xyz", def_str);
            string rtn_str = _function.intrinsic_string("abc");
            put("string: arg set:", "abc", rtn_str);
            put();

            System.Console.WriteLine("---- [ struct] ----");
            Vec3d arg = new Vec3d(111.1, 222.2, 333.3);
            Vec3d rtn_vec3d = _function.struct_vec3d(arg);
            put("struct: Vec3d: get", sv3d123, _function.vec3d);
            put("struct: Vec3d: rtn", sv3d123, rtn_vec3d);
            put();

            System.Console.WriteLine("---- [ vector ] ----");
        // int
            // from List, to List
            List<int> arg_int_list = new List<int> {111, 222, 333};
            List<int> vec_int_list = _function.vector_int(arg_int_list);
            put("vector<int>: from List: get", "3", _function.vec_int.size());
            put("vector<int>: from List: get", "111", _function.vec_int[0]);
            put("vector<int>: from List: get", "222", _function.vec_int[1]);
            put("vector<int>: from List: get", "333", _function.vec_int[2]);
            put("vector<int>: to List: returned", "3", vec_int_list.Count);
            put("vector<int>: to List: returned", "111", vec_int_list[0]);
            put("vector<int>: to List: returned", "222", vec_int_list[1]);
            put("vector<int>: to List: returned", "333", vec_int_list[2]);
        // Vec3d
            // from List, to List
            List<Vec3d> arg_vec3d_list = new List<Vec3d> {
                        new Vec3d(111.1, 222.2, 333.3),
                        new Vec3d(444.4, 555.5, 666.6),
                        new Vec3d(777.7, 888.8, 999.9)
            };
            List<Vec3d> rtn_vec3d_list = _function.vector_vec3d(arg_vec3d_list);
            put("vector<Vec3d>: from List: get", "3", _function.vec_vec3d.size());
            put("vector<Vec3d>: from List: get", sv3d123, _function.vec_vec3d[0]);
            put("vector<Vec3d>: from List: get", sv3d456, _function.vec_vec3d[1]);
            put("vector<Vec3d>: from List: get", sv3d789, _function.vec_vec3d[2]);
            put("vector<Vec3d>: to List: returned", "3", rtn_vec3d_list.Count);
            put("vector<Vec3d>: to List: returned", sv3d123, rtn_vec3d_list[0]);
            put("vector<Vec3d>: to List: returned", sv3d456, rtn_vec3d_list[1]);
            put("vector<Vec3d>: to List: returned", sv3d789, rtn_vec3d_list[2]);

            System.Console.WriteLine("---- [ array ] ----");
            // 戻り配列のサイズを得る方法がないのでうまくいかない
            // 要素が1つだけの配列を戻されるとして実装
        // int
            // from array, to array
            int[] arg_int_array = new int[3] {444, 555, 666};
            int[] ary_int_array = _function.array_int(arg_int_array);
            put("int[]: from array: get", "3", _function.ary_int.size());
            put("int[]: from array: get", "444", _function.ary_int[0]);
            put("int[]: from array: get", "555", _function.ary_int[1]);
            put("int[]: from array: get", "666", _function.ary_int[2]);
            put("int[]: to array: returned", "1", ary_int_array.Length);
            put("int[]: to array: returned", "444", ary_int_array[0]);
        // Vec3d
            Vec3d[] arg_vec3d_array = new Vec3d[3] {
                        new Vec3d(111.1, 222.2, 333.3),
                        new Vec3d(444.4, 555.5, 666.6),
                        new Vec3d(777.7, 888.8, 999.9)
            };
            Vec3d[] ary_vec3d_array = _function.array_vec3d(arg_vec3d_array);
            put("Vec3d[]: from array: get", "3", _function.ary_vec3d.size());
            put("Vec3d[]: from array: get", sv3d123, _function.ary_vec3d[0]);
            put("Vec3d[]: from array: get", sv3d456, _function.ary_vec3d[1]);
            put("Vec3d[]: from array: get", sv3d789, _function.ary_vec3d[2]);
            put("Vec3d[]: to array: returned", "1", ary_vec3d_array.Length);
            put("Vec3d[]: to array: returned", sv3d123, ary_vec3d_array[0]);
        }

	// ------------------
	//  helper functions
	// ------------------
        static bool check_test(string chk) {
            return inc.Contains(chk);
        }
	static void test_name(string test_name) {
            System.Console.WriteLine("\n---[ " + test_name + " ]---");
	}
	static void put(string s = "") { System.Console.WriteLine(s); }

	static void put(string title, object expected, object result, int fraction=1) {
            //System.Console.Write(result.GetType()+" ");
            switch (result) {
                case SprCs.Vec3f v3f:
                    result = edit_vector(v3f);
                    break;
                case SprCs.Vec3d v3d:
                    result = edit_vector(v3d, fraction);
                    break;
                case SprCs.Vec4f v4f:
                    result = edit_vector(v4f);
                    break;
                case SprCs.Matrix3f m3f:
                    result = edit_matrix(m3f);
                    break;
                default:
                    result = result.ToString().Trim();
                    break;
            }
            string exp = expected.ToString().Trim();
            string res = result.ToString().Trim();
            res = res.Replace("\0", "\\0");
            string judge = exp == res ? "OK" : "--";
            System.Console.Write("[" + judge + "] ");
            string line = title + ": expected: " + exp + ", result: " + res;
            System.Console.WriteLine(line);
            if (judge == "OK") OK++;
            if (judge != "OK") NG++;
	}
        static void put2(string title, object expected, object result) {
            //System.Console.Write(result.GetType()+" ");
            string exp = expected.ToString().Trim();
            string res = result.ToString().Trim();
            string judge = exp == res ? "OK" : "--";
            System.Console.Write("[" + judge + "] ");
            string line = title + ":\n"
                                + "     expected: " + exp + ",\n"
                                + "     result:   " + res;
            System.Console.WriteLine(line);
            if (judge == "OK") OK++;
            if (judge != "OK") NG++;
        }
	//
	static string edit_vector(Vec3i v) {
            return string.Format("({0}, {1}, {2})", v.x, v.y, v.z);
        }
	static string edit_vector(Vec3f v) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", v.x, v.y, v.z);
        }
	static string edit_vector(Vec3d v, int fraction=1) {
            string s;
            if (fraction == 2)
                s = string.Format("({0:f2}, {1:f2}, {2:f2})", v.x, v.y, v.z);
            else
                s = string.Format("({0:f1}, {1:f1}, {2:f1})", v.x, v.y, v.z);
            return s;
        }
	static string edit_vector(Vec4f v) {
            return string.Format("({0:f1}, {1:f1}, {2:f1}, {3:f1})", v.w, v.x, v.y, v.z);
        }
	static string edit_matrix(Matrix3f m) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", m.xx, m.yx, m.zx) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xy, m.yy, m.zy) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xz, m.yz, m.zz);
	}
	static string edit_matrix(Matrix3d m) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", m.xx, m.yx, m.zx) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xy, m.yy, m.zy) +
                   string.Format("({0:f1}, {1:f1}, {2:f1})", m.xz, m.yz, m.zz);
	}
	static string edit_quaternion(Quaternionf q) {
            return string.Format("({0:f1}, {1:f1}, {2:f1}, {3:f1})", q.w, q.x, q.y, q.z);
	}
	static string edit_pose(Posef p) {
            return string.Format("({0:f1}, {1:f1}, {2:f1}, {3:f1}, {4:f1}), {5:f1}, {6:f1}",
                                 p.w, p.x, p.y, p.z, p.px, p.py, p.pz);
	}

	static string edit_vectorwrapper(vectorwrapper_Vec3d v, int ix) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", v[ix].x, v[ix].y, v[ix].z);
	}
	static string edit_vectorwrapper(vectorwrapper_int v, int ix) {
	    return v[ix].ToString();
	}
/***
	static string edit_arraywrapper(arraywrapper_Vec3d a, int ix) {
            return string.Format("({0:f1}, {1:f1}, {2:f1})", a[ix].x, a[ix].y, a[ix].z);
	}
***/
	static string edit_arraywrapper(arraywrapper_int a, int ix) {
	    return a[ix].ToString();
	}
    }
}
